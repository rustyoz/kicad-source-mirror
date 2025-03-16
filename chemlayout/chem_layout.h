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

#ifndef CHEM_LAYOUT_H
#define CHEM_LAYOUT_H

#include <vector>
#include <wx/string.h>
#include <wx/filename.h>

// Forward declarations
class REACTOR;
class PIPE;
class LAYOUT_LABEL;

/**
 * Class to represent a chemical reactor layout
 */
class CHEM_LAYOUT
{
public:
    /**
     * Constructor
     */
    CHEM_LAYOUT();
    
    /**
     * Destructor
     */
    ~CHEM_LAYOUT();
    
    /**
     * Clear the layout
     */
    void Clear();
    
    /**
     * Add a reactor to the layout
     * @param aReactor - The reactor to add
     */
    void AddReactor( REACTOR* aReactor );
    
    /**
     * Remove a reactor from the layout
     * @param aReactor - The reactor to remove
     */
    void RemoveReactor( REACTOR* aReactor );
    
    /**
     * Get all reactors in the layout
     * @return Vector of reactors
     */
    const std::vector<REACTOR*>& GetReactors() const;
    
    /**
     * Add a pipe to the layout
     * @param aPipe - The pipe to add
     */
    void AddPipe( PIPE* aPipe );
    
    /**
     * Remove a pipe from the layout
     * @param aPipe - The pipe to remove
     */
    void RemovePipe( PIPE* aPipe );
    
    /**
     * Get all pipes in the layout
     * @return Vector of pipes
     */
    const std::vector<PIPE*>& GetPipes() const;
    
    /**
     * Add a label to the layout
     * @param aLabel - The label to add
     */
    void AddLabel( LAYOUT_LABEL* aLabel );
    
    /**
     * Remove a label from the layout
     * @param aLabel - The label to remove
     */
    void RemoveLabel( LAYOUT_LABEL* aLabel );
    
    /**
     * Get all labels in the layout
     * @return Vector of labels
     */
    const std::vector<LAYOUT_LABEL*>& GetLabels() const;
    
    /**
     * Set the layout filename
     * @param aFilename - The filename
     */
    void SetFilename( const wxFileName& aFilename );
    
    /**
     * Get the layout filename
     * @return The filename
     */
    const wxFileName& GetFilename() const;
    
    /**
     * Set the layout title
     * @param aTitle - The title
     */
    void SetTitle( const wxString& aTitle );
    
    /**
     * Get the layout title
     * @return The title
     */
    const wxString& GetTitle() const;
    
    /**
     * Set whether the layout has been modified
     * @param aModified - True if modified, false otherwise
     */
    void SetModified( bool aModified );
    
    /**
     * Check if the layout has been modified
     * @return True if modified, false otherwise
     */
    bool IsModified() const;
    
private:
    std::vector<REACTOR*> m_reactors;
    std::vector<PIPE*> m_pipes;
    std::vector<LAYOUT_LABEL*> m_labels;
    wxFileName m_filename;
    wxString m_title;
    bool m_modified;
};

#endif // CHEM_LAYOUT_H 