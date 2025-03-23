/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2023 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <cmath>
#include <wx/log.h>
#include <trigo.h>
#include <gal/graphics_abstraction_layer.h>
#include <tool/tool_manager.h>
#include <tools/ee_grid_helper.h>
#include <tools/chem_selection_tool.h>
#include <tools/chem_line_tool.h>
#include <chem_actions.h>
#include <chem_commit.h>
#include <eda_item.h>
#include <chem_item.h>
#include <chem_symbol.h>
#include <chem_line.h>
#include <chem_junction.h>
#include <chem_edit_frame.h>
#include <pgm_base.h>
#include <view/view_controls.h>
#include <settings/settings_manager.h>
#include "chem_move_tool.h"


// For adding to or removing from selections
#define QUIET_MODE true


CHEM_MOVE_TOOL::CHEM_MOVE_TOOL() :
        CHEM_TOOL_BASE<CHEM_EDIT_FRAME>( "chemschema.InteractiveMove" ),
        m_inMoveTool( false ),
        m_moveInProgress( false ),
        m_isDrag( false ),
        m_moveOffset( 0, 0 )
{
}


bool CHEM_MOVE_TOOL::Init()
{
    CHEM_TOOL_BASE::Init();

    auto moveCondition =
            []( const SELECTION& aSel )
            {
                if( aSel.Empty() )
                    return false;

                return true;
            };

    // Add move actions to the selection tool menu
    //
    CONDITIONAL_MENU& selToolMenu = m_selectionTool->GetToolMenu().GetMenu();

    selToolMenu.AddItem( CHEM_ACTIONS::move, moveCondition, 150 );
    selToolMenu.AddItem( CHEM_ACTIONS::drag, moveCondition, 150 );
    selToolMenu.AddItem( CHEM_ACTIONS::alignToGrid, moveCondition, 150 );

    return true;
}


void CHEM_MOVE_TOOL::orthoLineDrag( CHEM_COMMIT* aCommit, CHEM_LINE* line, const VECTOR2I& splitDelta,
                                   int& xBendCount, int& yBendCount, const EE_GRID_HELPER& grid )
{
    // If the move is not the same angle as this move, then we need to do something special with
    // the unselected end to maintain orthogonality. Either drag some connected line that is the
    // same angle as the move or add two lines to make a 90 degree connection
    if( !EDA_ANGLE( splitDelta ).IsParallelTo( line->Angle() ) || line->GetLength() == 0 )
    {
        VECTOR2I unselectedEnd = line->HasFlag( STARTPOINT ) ? line->GetEndPoint()
                                                             : line->GetStartPoint();
        VECTOR2I selectedEnd = line->HasFlag( STARTPOINT ) ? line->GetStartPoint()
                                                           : line->GetEndPoint();

        // Look for pre-existing lines we can drag with us instead of creating new ones
        bool      foundAttachment = false;
        bool      foundJunction   = false;
        CHEM_LINE* foundLine       = nullptr;

        for( EDA_ITEM* cItem : m_lineConnectionCache[line] )
        {
            foundAttachment = true;

            // If the move is the same angle as a connected line, we can shrink/extend that line
            // endpoint
            if( cItem->Type() == CHEM_LINE_T )
            {
                CHEM_LINE* cLine = static_cast<CHEM_LINE*>( cItem );

                // A matching angle on a non-zero-length line means lengthen/shorten will work
                if( EDA_ANGLE( splitDelta ).IsParallelTo( cLine->Angle() )
                        && cLine->GetLength() != 0 )
                {
                    foundLine = cLine;
                }

                // Zero length lines are lines that this algorithm has shortened to 0 so they also
                // work but we should prefer using a segment with length and angle matching when
                // we can (otherwise the zero length line will draw overlapping segments on them)
                if( !foundLine && cLine->GetLength() == 0 )
                    foundLine = cLine;

                break;
            }
            else if( cItem->Type() == CHEM_JUNCTION_T )
            {
                foundJunction = true;
                break;
            }
        }

        // Ok... what if our original line is length zero from moving in its direction, and the
        // last added segment of the 90 bend we are connected to is zero from moving it in its
        // direction after it was added?
        //
        // If we are moving in original direction, we should lengthen the original drag wire.
        // Otherwise we should lengthen the new wire.
        bool preferOriginalLine = false;

        if( foundLine
                && foundLine->GetLength() == 0
                && line->GetLength() == 0
                && EDA_ANGLE( splitDelta ).IsParallelTo( line->GetStoredAngle() ) )
        {
            preferOriginalLine = true;
        }
        // If we have found an attachment, but not a line, we want to check if it's a junction.
        // These are special-cased and get a single line added instead of a 90-degree bend.
        else if( !foundLine && foundJunction )
        {
            // Create a new wire ending at the unselected end
            foundLine = new CHEM_LINE( unselectedEnd, line->GetLayer() );
            foundLine->SetFlags( IS_NEW );
            foundLine->SetLastResolvedState( line );
            m_frame->AddToScreen( foundLine, m_frame->GetScreen() );
            m_newDragLines.insert( foundLine );

            // We just broke off of the existing items, so replace all of them with our new
            // end connection.
            m_lineConnectionCache[foundLine] = m_lineConnectionCache[line];
            m_lineConnectionCache[line].clear();
            m_lineConnectionCache[line].emplace_back( foundLine );
        }

        // We want to drag our found line if it's in the same angle as the move or zero length,
        // but if the original drag line is also zero and the same original angle we should extend
        // that one first
        if( foundLine && !preferOriginalLine )
        {
            // Move the connected line found oriented in the direction of our move.
            //
            // Make sure we grab the right endpoint, it's not always STARTPOINT since the user can
            // draw a box of lines. We need to only move one though, and preferably the start point,
            // in case we have a zero length line that we are extending (we want the foundLine
            // start point to be attached to the unselected end of our drag line).
            //
            // Also, new lines are added already so they'll be in the undo list, skip adding them.

            if( !foundLine->HasFlag( IS_CHANGED ) && !foundLine->HasFlag( IS_NEW ) )
            {
                aCommit->Modify( foundLine, m_frame->GetScreen() );

                if( !foundLine->IsSelected() )
                    m_changedDragLines.insert( foundLine );
            }

            if( foundLine->GetStartPoint() == unselectedEnd )
                foundLine->MoveStart( splitDelta );
            else if( foundLine->GetEndPoint() == unselectedEnd )
                foundLine->MoveEnd( splitDelta );

            updateItem( foundLine, true );

            CHEM_LINE* bendLine = nullptr;

            if( m_lineConnectionCache.count( foundLine ) == 1
                    && m_lineConnectionCache[foundLine][0]->Type() == CHEM_LINE_T )
            {
                bendLine = static_cast<CHEM_LINE*>( m_lineConnectionCache[foundLine][0] );
            }

            // Remerge segments we've created if this is a segment that we've added whose only
            // other connection is also an added segment
            //
            // bendLine is first added segment at the original attachment point, foundLine is the
            // orthogonal line between bendLine and this line
            if( foundLine->HasFlag( IS_NEW )
                    && foundLine->GetLength() == 0
                    && bendLine && bendLine->HasFlag( IS_NEW ) )
            {
                if( line->HasFlag( STARTPOINT ) )
                    line->SetEndPoint( bendLine->GetEndPoint() );
                else
                    line->SetStartPoint( bendLine->GetEndPoint() );

                // Update our cache of the connected items.

                // First, re-attach our drag labels to the original line being re-merged.
                for( EDA_ITEM* candidate : m_lineConnectionCache[bendLine] )
                {
                    CHEM_LABEL* label = dynamic_cast<CHEM_LABEL*>( candidate );

                    if( label && m_specialCaseLabels.count( label ) )
                        m_specialCaseLabels[label].attachedLine = line;
                }

                m_lineConnectionCache[line] = m_lineConnectionCache[bendLine];
                m_lineConnectionCache[bendLine].clear();
                m_lineConnectionCache[foundLine].clear();

                m_frame->RemoveFromScreen( bendLine, m_frame->GetScreen() );
                m_frame->RemoveFromScreen( foundLine, m_frame->GetScreen() );

                m_newDragLines.erase( bendLine );
                m_newDragLines.erase( foundLine );

                delete bendLine;
                delete foundLine;
            }
            //Ok, move the unselected end of our item
            else
            {
                if( line->HasFlag( STARTPOINT ) )
                    line->MoveEnd( splitDelta );
                else
                    line->MoveStart( splitDelta );
            }

            updateItem( line, true );
        }
        else if( line->GetLength() == 0 )
        {
            // We didn't find another line to shorten/lengthen, (or we did but it's also zero)
            // so now is a good time to use our existing zero-length original line
        }
        // Either no line was at the "right" angle, or this was a junction. We
        // need to add segments to keep the soon-to-move unselected end connected to these items.
        //
        // To keep our drag selections all the same, we'll move our unselected end point and then
        // put wires between it and its original endpoint.
        else if( foundAttachment && line->IsOrthogonal() )
        {
            VECTOR2D lineGrid = grid.GetGridSize( grid.GetItemGrid( line ) );

            // The bend counter handles a group of wires all needing their offset one grid movement
            // further out from each other to not overlap.  The absolute value stuff finds the
            // direction of the line and hence the the bend increment on that axis
            unsigned int xMoveBit = splitDelta.x != 0;
            unsigned int yMoveBit = splitDelta.y != 0;
            int          xLength = abs( unselectedEnd.x - selectedEnd.x );
            int          yLength = abs( unselectedEnd.y - selectedEnd.y );
            int          xMove = ( xLength - ( xBendCount * lineGrid.x ) )
                                    * sign( selectedEnd.x - unselectedEnd.x );
            int          yMove = ( yLength - ( yBendCount * lineGrid.y ) )
                                    * sign( selectedEnd.y - unselectedEnd.y );

            // Create a new wire ending at the unselected end, we'll move the new wire's start
            // point to the unselected end
            CHEM_LINE* a = new CHEM_LINE( unselectedEnd, line->GetLayer() );
            a->MoveStart( VECTOR2I( xMove, yMove ) );
            a->SetFlags( IS_NEW );
            a->SetConnectivityDirty( true );
            a->SetLastResolvedState( line );
            m_frame->AddToScreen( a, m_frame->GetScreen() );
            m_newDragLines.insert( a );

            CHEM_LINE* b = new CHEM_LINE( a->GetStartPoint(), line->GetLayer() );
            b->MoveStart( VECTOR2I( splitDelta.x, splitDelta.y ) );
            b->SetFlags( IS_NEW | STARTPOINT );
            b->SetConnectivityDirty( true );
            b->SetLastResolvedState( line );
            m_frame->AddToScreen( b, m_frame->GetScreen() );
            m_newDragLines.insert( b );

            xBendCount += yMoveBit;
            yBendCount += xMoveBit;

            // Ok move the unselected end of our item
            if( line->HasFlag( STARTPOINT ) )
            {
                line->MoveEnd( VECTOR2I( splitDelta.x ? splitDelta.x : xMove,
                                         splitDelta.y ? splitDelta.y : yMove ) );
            }
            else
            {
                line->MoveStart( VECTOR2I( splitDelta.x ? splitDelta.x : xMove,
                                           splitDelta.y ? splitDelta.y : yMove ) );
            }

            // Update our cache of the connected items. First, attach our drag labels to the line
            // left behind.
            for( EDA_ITEM* candidate : m_lineConnectionCache[line] )
            {
                CHEM_LABEL* label = dynamic_cast<CHEM_LABEL*>( candidate );

                if( label && m_specialCaseLabels.count( label ) )
                    m_specialCaseLabels[label].attachedLine = a;
            }

            // We just broke off of the existing items, so replace all of them with our new end
            // connection.
            m_lineConnectionCache[a] = m_lineConnectionCache[line];
            m_lineConnectionCache[b].emplace_back( a );
            m_lineConnectionCache[line].clear();
            m_lineConnectionCache[line].emplace_back( b );
        }
        // Original line has no attachments, just move the unselected end
        else if( !foundAttachment )
        {
            if( line->HasFlag( STARTPOINT ) )
                line->MoveEnd( splitDelta );
            else
                line->MoveStart( splitDelta );
        }
    }
}


int CHEM_MOVE_TOOL::Main( const TOOL_EVENT& aEvent )
{
    m_isDrag = aEvent.IsAction( &CHEM_ACTIONS::drag );

    if( CHEM_COMMIT* commit = dynamic_cast<CHEM_COMMIT*>( aEvent.Commit() ) )
    {
        bool isSlice = false;

        if( m_isDrag )
            isSlice = aEvent.Parameter<bool>();

        wxCHECK( aEvent.SynchronousState(), 0 );
        aEvent.SynchronousState()->store( STS_RUNNING );

        if( doMoveSelection( aEvent, commit, isSlice ) )
            aEvent.SynchronousState()->store( STS_FINISHED );
        else
            aEvent.SynchronousState()->store( STS_CANCELLED );
    }
    else
    {
        CHEM_COMMIT localCommit( m_toolMgr );

        if( doMoveSelection( aEvent, &localCommit, false ) )
            localCommit.Push( m_isDrag ? _( "Drag" ) : _( "Move" ) );
        else
            localCommit.Revert();
    }

    return 0;
}


bool CHEM_MOVE_TOOL::doMoveSelection( const TOOL_EVENT& aEvent, CHEM_COMMIT* aCommit, bool aIsSlice )
{
    SETTINGS_MANAGER&     mgr = Pgm().GetSettingsManager();
    KIGFX::VIEW_CONTROLS* controls = getViewControls();
    EE_GRID_HELPER        grid( m_toolMgr );
    bool                  wasDragging = m_moveInProgress && m_isDrag;

    m_anchorPos.reset();

    if( m_moveInProgress )
    {
        if( m_isDrag != wasDragging )
        {
            EDA_ITEM* sel = m_selectionTool->GetSelection().Front();

            if( sel && !sel->IsNew() )
            {
                // Reset the selected items so we can start again with the current m_isDrag
                // state.
                aCommit->Revert();

                m_selectionTool->RemoveItemsFromSel( &m_dragAdditions, QUIET_MODE );
                m_anchorPos = m_cursor - m_moveOffset;
                m_moveInProgress = false;
                controls->SetAutoPan( false );

                // And give it a kick so it doesn't have to wait for the first mouse movement
                // to refresh.
                m_toolMgr->PostAction( CHEM_ACTIONS::restartMove );
            }
        }
        else
        {
            // The tool hotkey is interpreted as a click when already dragging/moving
            m_toolMgr->PostAction( ACTIONS::cursorClick );
        }

        return false;
    }

    if( m_inMoveTool )      // Must come after m_moveInProgress checks above...
        return false;

    REENTRANCY_GUARD guard( &m_inMoveTool );

    CHEM_SELECTION& userSelection = m_selectionTool->GetSelection();

    // Be sure that there is at least one item that we can move. If there's no selection try
    // looking for the stuff under mouse cursor (i.e. Kicad old-style hover selection).
    CHEM_SELECTION& selection = m_selectionTool->RequestSelection( CHEM_COLLECTOR::MovableItems,
                                                                  true );
    bool           unselect = selection.IsHover();

    // Keep an original copy of the starting points for cleanup after the move
    std::vector<DANGLING_END_ITEM> internalPoints;

    Activate();

    // Must be done after Activate() so that it gets set into the correct context
    controls->ShowCursor( true );

    m_frame->PushTool( aEvent );

    if( selection.Empty() )
    {
        // Note that it's important to go through push/pop even when the selection is empty.
        // This keeps other tools from having to special-case an empty move.
        m_frame->PopTool( aEvent );
        return false;
    }

    bool        restore_state = false;
    TOOL_EVENT  copy = aEvent;
    TOOL_EVENT* evt = &copy;
    VECTOR2I    prevPos;
    GRID_HELPER_GRIDS snapLayer = GRID_CURRENT;

    m_cursor = controls->GetCursorPosition();

    // Main loop: keep receiving events
    do
    {
        m_frame->GetCanvas()->SetCurrentCursor( KICURSOR::MOVING );
        grid.SetSnap( !evt->Modifier( MD_SHIFT ) );
        grid.SetUseGrid( getView()->GetGAL()->GetGridSnapping() && !evt->DisableGridSnapping() );

        if( evt->IsAction( &CHEM_ACTIONS::restartMove )
                || evt->IsAction( &CHEM_ACTIONS::move )
                || evt->IsAction( &CHEM_ACTIONS::drag )
                || evt->IsMotion()
                || evt->IsDrag( BUT_LEFT )
                || evt->IsAction( &ACTIONS::refreshPreview ) )
        {
            if( !m_moveInProgress )    // Prepare to start moving/dragging
            {
                CHEM_ITEM* chem_item = (CHEM_ITEM*) selection.Front();
                bool      placingNewItems = chem_item && chem_item->IsNew();

                //------------------------------------------------------------------------
                // Setup a drag or a move
                //
                m_dragAdditions.clear();
                m_specialCaseLabels.clear();
                internalPoints.clear();
                clearNewDragLines();

                for( CHEM_ITEM* it : m_frame->GetScreen()->Items() )
                {
                    it->ClearFlags( SELECTED_BY_DRAG );

                    if( !it->IsSelected() )
                        it->ClearFlags( STARTPOINT | ENDPOINT );
                }

                // Drag of split items start over top of their other segment so
                // we want to skip grabbing the segments we split from
                if( m_isDrag && !aIsSlice )
                {
                    EDA_ITEMS connectedDragItems;

                    // Add connections to the selection for a drag.
                    // Do all non-labels first so we don't add junctions to drag
                    // when the line will eventually be drag selected.
                    std::vector<CHEM_ITEM*> stageTwo;

                    for( EDA_ITEM* edaItem : selection )
                    {
                        CHEM_ITEM* item = static_cast<CHEM_ITEM*>( edaItem );
                        std::vector<VECTOR2I> connections;

                        switch( item->Type() )
                        {
                        case CHEM_LABEL_T:
                            stageTwo.emplace_back(item);
                            break;

                        case CHEM_LINE_T:
                            static_cast<CHEM_LINE*>( item )->GetSelectedPoints( connections );
                            break;
                        default:
                            connections = item->GetConnectionPoints();
                        }

                        for( const VECTOR2I& point : connections )
                            getConnectedDragItems( aCommit, item, point, connectedDragItems );
                    }

                    // Go back and get all label connections now that we can test for drag-selected
                    // lines the labels might be on
                    for( CHEM_ITEM* item : stageTwo )
                    {
                        for( const VECTOR2I& point : item->GetConnectionPoints() )
                            getConnectedDragItems( aCommit, item, point, connectedDragItems );
                    }

                    for( EDA_ITEM* item : connectedDragItems )
                    {
                        m_dragAdditions.push_back( item->m_Uuid );
                        m_selectionTool->AddItemToSel( item, QUIET_MODE );
                    }

                    // Pre-cache all connections of our selected objects so we can keep track of
                    // what they were originally connected to as we drag them around
                    for( EDA_ITEM* edaItem : selection )
                    {
                        CHEM_ITEM* chemItem = static_cast<CHEM_ITEM*>( edaItem );

                        if( chemItem->Type() == CHEM_LINE_T )
                        {
                            CHEM_LINE* line = static_cast<CHEM_LINE*>( chemItem );

                            //Also store the original angle of the line, is needed later to decide
                            //which segment to extend when they've become zero length
                            line->StoreAngle();

                            for( const VECTOR2I& point : line->GetConnectionPoints() )
                                getConnectedItems( line, point, m_lineConnectionCache[line] );
                        }
                    }
                }
                else
                {
                    // Mark the edges of the block with dangling flags for a move.
                    for( EDA_ITEM* item : selection )
                        static_cast<CHEM_ITEM*>( item )->GetEndPoints( internalPoints );

                    std::vector<DANGLING_END_ITEM> endPointsByType = internalPoints;
                    std::vector<DANGLING_END_ITEM> endPointsByPos = endPointsByType;
                    DANGLING_END_ITEM_HELPER::sort_dangling_end_items( endPointsByType,
                                                                       endPointsByPos );

                    for( EDA_ITEM* item : selection )
                        static_cast<CHEM_ITEM*>( item )->UpdateDanglingState( endPointsByType,
                                                                             endPointsByPos );
                }


                // Generic setup
                snapLayer = grid.GetSelectionGrid( selection );

                for( EDA_ITEM* item : selection )
                {
                    if( item->IsNew() )
                    {
                        // Item was added to commit in a previous command
                    }
                    else if( item->GetParent() && item->GetParent()->IsSelected() )
                    {
                        // Item will be (or has been) added to commit by parent
                    }
                    else
                    {
                        aCommit->Modify( item, m_frame->GetScreen() );
                    }

                    item->SetFlags( IS_MOVING );

                    if( CHEM_ITEM* chemItem = dynamic_cast<CHEM_ITEM*>( item ) )
                        chemItem->SetStoredPos( chemItem->GetPosition() );
                }

                // Set up the starting position and move/drag offset
                //
                m_cursor = controls->GetCursorPosition();

                if( evt->IsAction( &CHEM_ACTIONS::restartMove ) )
                {
                    wxASSERT_MSG( m_anchorPos, "Should be already set from previous cmd" );
                }
                else if( placingNewItems )
                {
                    m_anchorPos = selection.GetReferencePoint();
                }

                if( m_anchorPos )
                {
                    VECTOR2I delta = m_cursor - (*m_anchorPos);
                    bool     isPasted = false;

                    // Drag items to the current cursor position
                    for( EDA_ITEM* item : selection )
                    {
                        // Don't double move pins, fields, etc.
                        if( item->GetParent() && item->GetParent()->IsSelected() )
                            continue;

                        moveItem( item, delta );
                        updateItem( item, false );

                        isPasted |= ( item->GetFlags() & IS_PASTED ) != 0;
                        item->ClearFlags( IS_PASTED );
                    }

                    // The first time pasted items are moved we need to store the position of the
                    // cursor so that rotate while moving works as expected (instead of around the
                    // original anchor point
                    if( isPasted )
                        selection.SetReferencePoint( m_cursor );

                    m_anchorPos = m_cursor;
                }
                // For some items, moving the cursor to anchor is not good (for instance large
                // hierarchical sheets or symbols can have the anchor outside the view)
                else if( selection.Size() == 1 && !chem_item->IsMovableFromAnchorPoint() )
                {
                    m_cursor = getViewControls()->GetCursorPosition( true );
                    m_anchorPos = m_cursor;
                }
                else
                {
                    if( m_frame->GetMoveWarpsCursor() )
                    {
                        // User wants to warp the mouse
                        m_cursor = grid.BestDragOrigin( m_cursor, snapLayer, selection );
                        selection.SetReferencePoint( m_cursor );
                    }
                    else
                    {
                        // User does not want to warp the mouse
                        m_cursor = getViewControls()->GetCursorPosition( true );
                    }
                }

                controls->SetCursorPosition( m_cursor, false );

                prevPos = m_cursor;
                controls->SetAutoPan( true );
                m_moveInProgress = true;
            }

            //------------------------------------------------------------------------
            // Follow the mouse
            //
            m_cursor = grid.BestSnapAnchor( controls->GetCursorPosition( false ),
                                            snapLayer, selection );

            VECTOR2I delta( m_cursor - prevPos );
            m_anchorPos = m_cursor;

            // We need to check if the movement will change the net offset direction on the
            // X an Y axes. This is because we remerge added bend lines in realtime, and we
            // also account for the direction of the move when adding bend lines. So, if the
            // move direction changes, we need to split it into a move that gets us back to
            // zero, then the rest of the move.
            std::vector<VECTOR2I> splitMoves;

            if( alg::signbit( m_moveOffset.x ) != alg::signbit( ( m_moveOffset + delta ).x ) )
            {
                splitMoves.emplace_back( VECTOR2I( -1 * m_moveOffset.x, 0 ) );
                splitMoves.emplace_back( VECTOR2I( delta.x + m_moveOffset.x, 0 ) );
            }
            else
            {
                splitMoves.emplace_back( VECTOR2I( delta.x, 0 ) );
            }

            if( alg::signbit( m_moveOffset.y ) != alg::signbit( ( m_moveOffset + delta ).y ) )
            {
                splitMoves.emplace_back( VECTOR2I( 0, -1 * m_moveOffset.y ) );
                splitMoves.emplace_back( VECTOR2I( 0, delta.y + m_moveOffset.y ) );
            }
            else
            {
                splitMoves.emplace_back( VECTOR2I( 0, delta.y ) );
            }


            m_moveOffset += delta;
            prevPos = m_cursor;

            // Used for tracking how far off a drag end should have its 90 degree elbow added
            int xBendCount = 1;
            int yBendCount = 1;

            // Split the move into X and Y moves so we can correctly drag orthogonal lines
            for( const VECTOR2I& splitDelta : splitMoves )
            {
                // Skip non-moves
                if( splitDelta == VECTOR2I( 0, 0 ) )
                    continue;

                for( EDA_ITEM* item : selection.GetItemsSortedByTypeAndXY( ( delta.x >= 0 ),
                                                                           ( delta.y >= 0 ) ) )
                {
                    // Don't double move pins, fields, etc.
                    if( item->GetParent() && item->GetParent()->IsSelected() )
                        continue;

                    CHEM_LINE* line = dynamic_cast<CHEM_LINE*>( item );

                    // Only partially selected drag lines in orthogonal line mode need special
                    // handling
                    if( m_isDrag && line && line->HasFlag( STARTPOINT ) != line->HasFlag( ENDPOINT ) )
                    {
                        orthoLineDrag( aCommit, line, splitDelta, xBendCount, yBendCount, grid );
                    }

                    // Move all other items normally, including the selected end of partially
                    // selected lines
                    moveItem( item, splitDelta );
                    updateItem( item, false );
                }
            }

            if( selection.HasReferencePoint() )
                selection.SetReferencePoint( selection.GetReferencePoint() + delta );

            m_toolMgr->PostEvent( EVENTS::SelectedItemsMoved );
        }

        //------------------------------------------------------------------------
        // Handle cancel
        //
        else if( evt->IsCancelInteractive() || evt->IsActivate() )
        {
            if( evt->IsCancelInteractive() )
                m_frame->GetInfoBar()->Dismiss();

            if( m_moveInProgress )
            {
                if( evt->IsActivate() )
                {
                    // Allowing other tools to activate during a move runs the risk of race
                    // conditions in which we try to spool up both event loops at once.

                    if( m_isDrag )
                        m_frame->ShowInfoBarMsg( _( "Press <ESC> to cancel drag." ) );
                    else
                        m_frame->ShowInfoBarMsg( _( "Press <ESC> to cancel move." ) );

                    evt->SetPassEvent( false );
                    continue;
                }

                evt->SetPassEvent( false );
                restore_state = true;
            }

            clearNewDragLines();

            break;
        }
        //------------------------------------------------------------------------
        // Handle TOOL_ACTION special cases
        //
        else if( evt->Action() == TA_UNDO_REDO_PRE )
        {
            unselect = true;
            break;
        }
        else if( evt->IsAction( &ACTIONS::doDelete ) )
        {
            evt->SetPassEvent();
            // Exit on a delete; there will no longer be anything to drag.
            break;
        }
        else if( evt->IsAction( &ACTIONS::duplicate ) )
        {
            wxBell();
        }
        else if( evt->IsAction( &CHEM_ACTIONS::rotateCW ) )
        {
            m_toolMgr->RunSynchronousAction( CHEM_ACTIONS::rotateCW, aCommit );
        }
        else if( evt->IsAction( &CHEM_ACTIONS::rotateCCW ) )
        {
            m_toolMgr->RunSynchronousAction( CHEM_ACTIONS::rotateCCW, aCommit );
        }
        else if( evt->IsAction( &ACTIONS::increment ) )
        {
            m_toolMgr->RunSynchronousAction( ACTIONS::increment, aCommit,
                                             evt->Parameter<ACTIONS::INCREMENT>() );
        }
        else if( evt->IsAction( &CHEM_ACTIONS::highlightNet )
                    || evt->IsAction( &CHEM_ACTIONS::selectOnPCB ) )
        {
            // These don't make any sense during a move.  Eat them.
        }
        //------------------------------------------------------------------------
        // Handle context menu
        //
        else if( evt->IsClick( BUT_RIGHT ) )
        {
            m_menu->ShowContextMenu( m_selectionTool->GetSelection() );
        }
        //------------------------------------------------------------------------
        // Handle drop
        //
        else if( evt->IsMouseUp( BUT_LEFT )
                || evt->IsClick( BUT_LEFT )
                || evt->IsDblClick( BUT_LEFT ) )
        {
            break; // Finish
        }
        else
        {
            evt->SetPassEvent();
        }

        controls->SetAutoPan( m_moveInProgress );

    } while( ( evt = Wait() ) ); //Should be assignment not equality test

    // Create a selection of original selection, drag selected/changed items, and new
    // bend lines for later before we clear them in the aCommit. We'll need these
    // to check for new junctions needed, etc.
    CHEM_SELECTION selectionCopy( selection );

    for( CHEM_LINE* line : m_newDragLines )
        selectionCopy.Add( line );

    for( CHEM_LINE* line : m_changedDragLines )
        selectionCopy.Add( line );

    // Save whatever new bend lines and changed lines survived the drag
    for( CHEM_LINE* newLine : m_newDragLines )
    {
        newLine->ClearEditFlags();
        aCommit->Added( newLine, m_frame->GetScreen() );
    }

    // These lines have been changed, but aren't selected. We need
    // to manually clear these edit flags or they'll stick around.
    for( CHEM_LINE* oldLine : m_changedDragLines )
        oldLine->ClearEditFlags();

    m_newDragLines.clear();
    m_changedDragLines.clear();

    controls->ForceCursorPosition( false );
    controls->ShowCursor( false );
    controls->SetAutoPan( false );

    m_moveOffset = { 0, 0 };
    m_anchorPos.reset();

    if( restore_state )
    {
        m_selectionTool->RemoveItemsFromSel( &m_dragAdditions, QUIET_MODE );
    }
    else
    {
        // One last update after exiting loop (for slower stuff, such as updating SCREEN's RTree).
        for( EDA_ITEM* item : selection )
        {
            updateItem( item, true );

            if( CHEM_ITEM* chem_item = dynamic_cast<CHEM_ITEM*>( item ) )
                chem_item->SetConnectivityDirty( true );
        }

        if( selection.GetSize() == 1 && selection.Front()->IsNew() )
            m_frame->SaveCopyForRepeatItem( static_cast<CHEM_ITEM*>( selection.Front() ) );

        m_selectionTool->RemoveItemsFromSel( &m_dragAdditions, QUIET_MODE );

        // If we move items away from a junction, we _may_ want to add a junction there
        // to denote the state.
        for( const DANGLING_END_ITEM& it : internalPoints )
        {
            if( m_frame->GetScreen()->IsExplicitJunctionNeeded( it.GetPosition()) )
                m_frame->AddJunction( aCommit, m_frame->GetScreen(), it.GetPosition() );
        }

        CHEM_LINE_TOOL* lineTool = m_toolMgr->GetTool<CHEM_LINE_TOOL>();
        lineTool->TrimOverLappingWires( aCommit, &selectionCopy );
        lineTool->AddJunctionsIfNeeded( aCommit, &selectionCopy );

        // This needs to run prior to `RecalculateConnections` because we need to identify
        // the lines that are newly dangling
        if( m_isDrag && !aIsSlice )
            trimDanglingLines( aCommit );

        // Auto-rotate any moved labels
        for( EDA_ITEM* item : selection )
            m_frame->AutoRotateItem( m_frame->GetScreen(), static_cast<CHEM_ITEM*>( item ) );

        m_frame->SchematicCleanUp( aCommit );
    }

    for( EDA_ITEM* item : m_frame->GetScreen()->Items() )
        item->ClearEditFlags();

    // ensure any selected item not in screen main list (for instance symbol fields)
    // has its edit flags cleared
    for( EDA_ITEM* item : selectionCopy )
        item->ClearEditFlags();

    if( unselect )
        m_toolMgr->RunAction( CHEM_ACTIONS::clearSelection );
    else
        m_selectionTool->RebuildSelection();  // Schematic cleanup might have merged lines, etc.

    m_dragAdditions.clear();
    m_lineConnectionCache.clear();
    m_moveInProgress = false;
    m_frame->PopTool( aEvent );

    return !restore_state;
}


void CHEM_MOVE_TOOL::trimDanglingLines( CHEM_COMMIT* aCommit )
{
    // Need a local cleanup first to ensure we remove unneeded junctions
    m_frame->SchematicCleanUp( aCommit, m_frame->GetScreen() );

    std::set<CHEM_ITEM*> danglers;

    std::function<void( CHEM_ITEM* )> changeHandler =
            [&]( CHEM_ITEM* aChangedItem ) -> void
            {
                m_toolMgr->GetView()->Update( aChangedItem, KIGFX::REPAINT );

                // Delete newly dangling lines:
                // Find split segments (one segment is new, the other is changed) that
                // we aren't dragging and don't have selected
                if( aChangedItem->HasFlag( IS_BROKEN) && aChangedItem->IsDangling()
                  && !aChangedItem->IsSelected() )
                {
                    danglers.insert( aChangedItem );
                }
            };

    m_frame->GetScreen()->TestDanglingEnds( nullptr, &changeHandler );

    for( CHEM_ITEM* line : danglers )
    {
        line->SetFlags( STRUCT_DELETED );
        aCommit->Removed( line, m_frame->GetScreen() );

        updateItem( line, false );
        m_frame->RemoveFromScreen( line, m_frame->GetScreen() );
    }
}


void CHEM_MOVE_TOOL::getConnectedItems( CHEM_ITEM* aOriginalItem, const VECTOR2I& aPoint,
                                       EDA_ITEMS& aList )
{
    EE_RTREE&         items = m_frame->GetScreen()->Items();
    EE_RTREE::EE_TYPE itemsOverlapping = items.Overlapping( aOriginalItem->GetBoundingBox() );
    CHEM_ITEM*         foundJunction = nullptr;

    // If you're connected to a junction, you're only connected to the junction.
    for( CHEM_ITEM* item : itemsOverlapping )
    {
        if( item != aOriginalItem && item->IsConnected( aPoint ) )
        {
            if( item->Type() == CHEM_JUNCTION_T )
                foundJunction = item;
        }
    }

    if( foundJunction )
    {
        aList.push_back( foundJunction );
        return;
    }

    for( CHEM_ITEM* test : itemsOverlapping )
    {
        if( test == aOriginalItem || !test->CanConnect( aOriginalItem ) )
            continue;

        switch( test->Type() )
        {
        case CHEM_LINE_T:
        {
            CHEM_LINE* line = static_cast<CHEM_LINE*>( test );

            // When getting lines for the connection cache, it's important that we only add
            // items at the unselected end, since that is the only end that is handled specially.
            // Fully selected lines, and the selected end of a partially selected line, are moved
            // around normally and don't care about their connections.
            if( ( line->HasFlag( STARTPOINT ) && aPoint == line->GetStartPoint() )
                || ( line->HasFlag( ENDPOINT ) && aPoint == line->GetEndPoint() ) )
            {
                continue;
            }

            if( test->IsConnected( aPoint ) )
                aList.push_back( test );

            // Labels can connect to a wire anywhere along the length
            if( CHEM_LABEL* label = dynamic_cast<CHEM_LABEL*>( aOriginalItem ) )
            {
                if( static_cast<CHEM_LINE*>( test )->HitTest( label->GetPosition(), 1 ) )
                    aList.push_back( test );
            }

            break;
        }

        case CHEM_SYMBOL_T:
        case CHEM_JUNCTION_T:
            if( test->IsConnected( aPoint ) )
                aList.push_back( test );

            break;

        case CHEM_LABEL_T:
            // Labels can connect to a wire anywhere along the length
            if( aOriginalItem->Type() == CHEM_LINE_T && test->CanConnect( aOriginalItem ) )
            {
                CHEM_LABEL* label = static_cast<CHEM_LABEL*>( test );
                CHEM_LINE*  line = static_cast<CHEM_LINE*>( aOriginalItem );

                if( line->HitTest( label->GetPosition(), 1 ) )
                    aList.push_back( label );
            }

            break;

        default:
            break;
        }
    }
}


void CHEM_MOVE_TOOL::getConnectedDragItems( CHEM_COMMIT* aCommit, CHEM_ITEM* aSelectedItem,
                                           const VECTOR2I& aPoint, EDA_ITEMS& aList )
{
    EE_RTREE&         items = m_frame->GetScreen()->Items();
    EE_RTREE::EE_TYPE itemsOverlappingRTree = items.Overlapping( aSelectedItem->GetBoundingBox() );
    std::vector<CHEM_ITEM*> itemsConnectable;
    bool              ptHasUnselectedJunction = false;

    auto makeNewWire =
            [this]( CHEM_COMMIT* commit, CHEM_ITEM* fixed, CHEM_ITEM* selected, const VECTOR2I& start,
                    const VECTOR2I& end )
            {
                CHEM_LINE* newWire = new CHEM_LINE( start, LAYER_WIRE );
                newWire->SetFlags( IS_NEW );
                newWire->SetConnectivityDirty( true );

                if( dynamic_cast<const CHEM_LINE*>( selected ) )
                    newWire->SetLastResolvedState( selected );
                else if( dynamic_cast<const CHEM_LINE*>( fixed ) )
                    newWire->SetLastResolvedState( fixed );

                newWire->SetEndPoint( end );
                m_frame->AddToScreen( newWire, m_frame->GetScreen() );
                commit->Added( newWire, m_frame->GetScreen() );

                return newWire;
            };

    auto makeNewJunction =
            [this]( CHEM_COMMIT* commit, CHEM_LINE* line, const VECTOR2I& pt )
            {
                CHEM_JUNCTION* junction = new CHEM_JUNCTION( pt );
                junction->SetFlags( IS_NEW );
                junction->SetConnectivityDirty( true );
                junction->SetLastResolvedState( line );

                m_frame->AddToScreen( junction, m_frame->GetScreen() );
                commit->Added( junction, m_frame->GetScreen() );

                return junction;
            };

    for( CHEM_ITEM* item : itemsOverlappingRTree )
    {
        // Skip ourselves, skip already selected items (but not lines, they need both ends tested)
        // and skip unconnectable items
        if( item == aSelectedItem || ( item->Type() != CHEM_LINE_T && item->IsSelected() )
            || !item->CanConnect( aSelectedItem ) )
        {
            continue;
        }

        itemsConnectable.push_back( item );
    }

    for( CHEM_ITEM* item : itemsConnectable )
    {
        if( item->Type() == CHEM_JUNCTION_T && item->IsConnected( aPoint ) && !item->IsSelected() )
        {
            ptHasUnselectedJunction = true;
            break;
        }
    }

    CHEM_LINE* newWire = nullptr;

    for( CHEM_ITEM* test : itemsConnectable )
    {
        KICAD_T testType = test->Type();

        switch( testType )
        {
        case CHEM_LINE_T:
        {
            // Select the connected end of wires that don't have an unselected
            // junction isolating them from the drag
            if( ptHasUnselectedJunction )
                break;

            CHEM_LINE* line = static_cast<CHEM_LINE*>( test );

            if( line->GetStartPoint() == aPoint )
            {
                // It's possible to manually select one end of a line and get a drag
                // connected other end, so we set the flag and then early exit the loop
                // later if the other drag items like labels attached to the line have
                // already been grabbed during the partial selection process.
                line->SetFlags( STARTPOINT );

                if( line->HasFlag( SELECTED ) || line->HasFlag( SELECTED_BY_DRAG ) )
                {
                    continue;
                }
                else
                {
                    line->SetFlags( SELECTED_BY_DRAG );
                    aList.push_back( line );
                }
            }
            else if( line->GetEndPoint() == aPoint )
            {
                line->SetFlags( ENDPOINT );

                if( line->HasFlag( SELECTED ) || line->HasFlag( SELECTED_BY_DRAG ) )
                {
                    continue;
                }
                else
                {
                    line->SetFlags( SELECTED_BY_DRAG );
                    aList.push_back( line );
                }
            }
            else
            {
                switch( aSelectedItem->Type() )
                {
                // These items can connect anywhere along a line
                case CHEM_LABEL_T:
                    // Only add a line if this line is unselected; if the label and line are both
                    // selected they'll move together
                    if( line->HitTest( aPoint, 1 ) && !line->HasFlag( SELECTED )
                        && !line->HasFlag( SELECTED_BY_DRAG ) )
                    {
                        newWire = makeNewWire( aCommit, line, aSelectedItem, aPoint, aPoint );
                        newWire->SetFlags( SELECTED_BY_DRAG | STARTPOINT );
                        newWire->StoreAngle( ( line->Angle() + ANGLE_90 ).Normalize() );
                        aList.push_back( newWire );

                        if( aPoint != line->GetStartPoint() && aPoint != line->GetEndPoint() )
                        {
                            // Split line in half
                            if( !line->IsNew() )
                                aCommit->Modify( line, m_frame->GetScreen() );

                            VECTOR2I oldEnd = line->GetEndPoint();
                            line->SetEndPoint( aPoint );

                            makeNewWire( aCommit, line, line, aPoint, oldEnd );
                            makeNewJunction( aCommit, line, aPoint );
                        }
                        else
                        {
                            m_lineConnectionCache[ newWire ] = { line };
                            m_lineConnectionCache[ line ] = { newWire };
                        }
                    }
                    break;

                default:
                    break;
                }

                break;
            }

            // Since only one end is going to move, the movement vector of any labels attached to
            // it is scaled by the proportion of the line length the label is from the moving end.
            for( CHEM_ITEM* item : items.Overlapping( line->GetBoundingBox() ) )
            {
                CHEM_LABEL* label = dynamic_cast<CHEM_LABEL*>( item );

                if( !label || label->IsSelected() )
                    continue;   // These will be moved on their own because they're selected

                if( label->HasFlag( SELECTED_BY_DRAG ) )
                    continue;

                if( label->CanConnect( line ) && line->HitTest( label->GetPosition(), 1 ) )
                {
                    label->SetFlags( SELECTED_BY_DRAG );
                    aList.push_back( label );

                    SPECIAL_CASE_LABEL_INFO info;
                    info.attachedLine = line;
                    info.originalLabelPos = label->GetPosition();
                    m_specialCaseLabels[label] = info;
                }
            }

            break;
        }

        case CHEM_SYMBOL_T:
        case CHEM_JUNCTION_T:
            if( test->IsConnected( aPoint ) && !newWire )
            {
                // Add a new wire between the symbol or junction and the selected item so
                // the selected item can be dragged.
                newWire = makeNewWire( aCommit, test, aSelectedItem, aPoint, aPoint );
                newWire->SetFlags( SELECTED_BY_DRAG | STARTPOINT );
                aList.push_back( newWire );
            }

            break;

        case CHEM_LABEL_T:
            // Performance optimization:
            if( test->HasFlag( SELECTED_BY_DRAG ) )
                break;

            // Select labels that are connected to a wire being moved.
            if( aSelectedItem->Type() == CHEM_LINE_T && test->CanConnect( aSelectedItem ) )
            {
                CHEM_LABEL* label = static_cast<CHEM_LABEL*>( test );
                CHEM_LINE*  line = static_cast<CHEM_LINE*>( aSelectedItem );

                bool oneEndFixed = !line->HasFlag( STARTPOINT ) || !line->HasFlag( ENDPOINT );

                if( line->HitTest( label->GetTextPos(), 1 ) )
                {
                    if( ( !line->HasFlag( STARTPOINT )
                          && label->GetPosition() == line->GetStartPoint() )
                        || ( !line->HasFlag( ENDPOINT )
                             && label->GetPosition() == line->GetEndPoint() ) )
                    {
                        //If we have a line selected at only one end, don't grab labels
                        //connected directly to the unselected endpoint
                        break;
                    }
                    else
                    {
                        label->SetFlags( SELECTED_BY_DRAG );
                        aList.push_back( label );

                        if( oneEndFixed )
                        {
                            SPECIAL_CASE_LABEL_INFO info;
                            info.attachedLine = line;
                            info.originalLabelPos = label->GetPosition();
                            m_specialCaseLabels[label] = info;
                        }
                    }
                }
            }
            else if( test->IsConnected( aPoint ) && !newWire )
            {
                // Add a new wire between the label and the selected item so the selected item
                // can be dragged.
                newWire = makeNewWire( aCommit, test, aSelectedItem, aPoint, aPoint );
                newWire->SetFlags( SELECTED_BY_DRAG | STARTPOINT );
                aList.push_back( newWire );
            }

            break;

        default:
            break;
        }
    }
}


void CHEM_MOVE_TOOL::moveItem( EDA_ITEM* aItem, const VECTOR2I& aDelta )
{
    switch( aItem->Type() )
    {
    case CHEM_LINE_T:
    {
        CHEM_LINE* line = static_cast<CHEM_LINE*>( aItem );

        if( aItem->HasFlag( STARTPOINT ) || !m_isDrag )
            line->MoveStart( aDelta );

        if( aItem->HasFlag( ENDPOINT ) || !m_isDrag )
            line->MoveEnd( aDelta );

        break;
    }

    case CHEM_LABEL_T:
    {
        CHEM_LABEL* label = static_cast<CHEM_LABEL*>( aItem );

        if( m_specialCaseLabels.count( label ) )
        {
            SPECIAL_CASE_LABEL_INFO info = m_specialCaseLabels[ label ];
            SEG currentLine( info.attachedLine->GetStartPoint(), info.attachedLine->GetEndPoint() );
            label->SetPosition( currentLine.NearestPoint( info.originalLabelPos ) );
        }
        else
        {
            label->Move( aDelta );
        }

        break;
    }

    default:
        static_cast<CHEM_ITEM*>( aItem )->Move( aDelta );
        break;
    }

    aItem->SetFlags( IS_MOVING );
}


int CHEM_MOVE_TOOL::AlignToGrid( const TOOL_EVENT& aEvent )
{
    EE_GRID_HELPER    grid( m_toolMgr);
    CHEM_SELECTION&    selection = m_selectionTool->RequestSelection( CHEM_COLLECTOR::MovableItems );
    GRID_HELPER_GRIDS selectionGrid = grid.GetSelectionGrid( selection );
    CHEM_COMMIT        commit( m_toolMgr );

    auto doMoveItem =
            [&]( EDA_ITEM* item, const VECTOR2I& delta )
            {
                commit.Modify( item, m_frame->GetScreen() );

                // Ensure only one end is moved when calling moveItem
                // i.e. we are in drag mode
                bool tmp_isDrag = m_isDrag;
                m_isDrag = true;
                moveItem( item, delta );
                m_isDrag = tmp_isDrag;

                item->ClearFlags( IS_MOVING );
                updateItem( item, true );
            };

    for( CHEM_ITEM* it : m_frame->GetScreen()->Items() )
    {
        if( !it->IsSelected() )
            it->ClearFlags( STARTPOINT | ENDPOINT );

        if( !selection.IsHover() && it->IsSelected() )
            it->SetFlags( STARTPOINT | ENDPOINT );

        it->SetStoredPos( it->GetPosition() );
    }

    for( EDA_ITEM* item : selection )
    {
        if( item->Type() == CHEM_LINE_T )
        {
            CHEM_LINE*             line = static_cast<CHEM_LINE*>( item );
            std::vector<int>      flags{ STARTPOINT, ENDPOINT };
            std::vector<VECTOR2I> pts{ line->GetStartPoint(), line->GetEndPoint() };

            for( int ii = 0; ii < 2; ++ii )
            {
                EDA_ITEMS drag_items{ item };
                line->ClearFlags();
                line->SetFlags( SELECTED );
                line->SetFlags( flags[ii] );
                getConnectedDragItems( &commit, line, pts[ii], drag_items );
                std::set<EDA_ITEM*> unique_items( drag_items.begin(), drag_items.end() );

                VECTOR2I delta = grid.AlignGrid( pts[ii], selectionGrid ) - pts[ii];

                if( delta != VECTOR2I( 0, 0 ) )
                {
                    for( EDA_ITEM* dragItem : unique_items )
                    {
                        if( dragItem->GetParent() && dragItem->GetParent()->IsSelected() )
                            continue;

                        doMoveItem( dragItem, delta );
                    }
                }
            }
        }
        else
        {
            CHEM_ITEM*             chemItem = static_cast<CHEM_ITEM*>( item );
            std::vector<VECTOR2I> connections = chemItem->GetConnectionPoints();
            EDA_ITEMS             drag_items;

            for( const VECTOR2I& point : connections )
                getConnectedDragItems( &commit, chemItem, point, drag_items );

            std::map<VECTOR2I, int> shifts;
            VECTOR2I                most_common( 0, 0 );
            int                     max_count = 0;

            for( const VECTOR2I& conn : connections )
            {
                VECTOR2I gridpt = grid.AlignGrid( conn, selectionGrid ) - conn;

                shifts[gridpt]++;

                if( shifts[gridpt] > max_count )
                {
                    most_common = gridpt;
                    max_count = shifts[most_common];
                }
            }

            if( most_common != VECTOR2I( 0, 0 ) )
            {
                doMoveItem( item, most_common );

                for( EDA_ITEM* dragItem : drag_items )
                {
                    if( dragItem->GetParent() && dragItem->GetParent()->IsSelected() )
                        continue;

                    doMoveItem( dragItem, most_common );
                }
            }
        }
    }

    CHEM_LINE_TOOL* lineTool = m_toolMgr->GetTool<CHEM_LINE_TOOL>();
    lineTool->TrimOverLappingWires( &commit, &selection );
    lineTool->AddJunctionsIfNeeded( &commit, &selection );

    m_toolMgr->PostEvent( EVENTS::SelectedItemsMoved );

    m_frame->SchematicCleanUp( &commit );
    commit.Push( _( "Align Items to Grid" ) );
    return 0;
}


void CHEM_MOVE_TOOL::clearNewDragLines()
{
    // Remove new bend lines added during the drag
    for( CHEM_LINE* newLine : m_newDragLines )
    {
        m_frame->RemoveFromScreen( newLine, m_frame->GetScreen() );
        delete newLine;
    }

    m_newDragLines.clear();
}


void CHEM_MOVE_TOOL::setTransitions()
{
    Go( &CHEM_MOVE_TOOL::Main,               CHEM_ACTIONS::move.MakeEvent() );
    Go( &CHEM_MOVE_TOOL::Main,               CHEM_ACTIONS::drag.MakeEvent() );
    Go( &CHEM_MOVE_TOOL::AlignToGrid,        CHEM_ACTIONS::alignToGrid.MakeEvent() );
} 