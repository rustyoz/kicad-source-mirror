#ifndef CHEM_DRAW_PANEL_H
#define CHEM_DRAW_PANEL_H

#include <class_draw_panel_gal.h>
#include <chem_view.h>

class LIB_SYMBOL;
class CHEM_SCREEN;

/**
 * Specialized drawing panel for chemical schematic views.
 */
class CHEM_DRAW_PANEL : public EDA_DRAW_PANEL_GAL
{
public:
    CHEM_DRAW_PANEL( wxWindow* aParentWindow, wxWindowID aWindowId, const wxPoint& aPosition,
                     const wxSize& aSize, KIGFX::GAL_DISPLAY_OPTIONS& aOptions,
                     GAL_TYPE aGalType = GAL_TYPE_OPENGL );

    ~CHEM_DRAW_PANEL();

    void DisplaySymbol( LIB_SYMBOL* aSymbol );
    void DisplaySheet( CHEM_SCREEN* aScreen );


    bool SwitchBackend( GAL_TYPE aGalType ) override;

    KIGFX::CHEM_VIEW* GetView() const override;

protected:
    virtual void onPaint( wxPaintEvent& aEvent ) override;

    void OnShow() override;

    void setDefaultLayerOrder();    ///< Reassign layer order to the initial settings.
    void setDefaultLayerDeps();     ///< Set rendering targets & dependencies for layers.
};

#endif // CHEM_DRAW_PANEL_H