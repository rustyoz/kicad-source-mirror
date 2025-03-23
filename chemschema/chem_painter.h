#ifndef CHEM_PAINTER_H
#define CHEM_PAINTER_H

#include <painter.h>
#include <chem_render_settings.h>

namespace KIGFX
{
    class GAL;
    class VIEW;

class CHEM_PAINTER : public PAINTER
{
public:
    CHEM_PAINTER( GAL* aGal );
    virtual ~CHEM_PAINTER() {}

    /// @copydoc PAINTER::Draw()
    virtual bool Draw( const VIEW_ITEM*, int ) override;

    /// @copydoc PAINTER::GetSettings()
    virtual RENDER_SETTINGS* GetSettings() override { return &m_settings; }

    static const std::vector<KICAD_T> g_ScaledSelectionTypes;

private:
    void draw( const SCH_ITEM* aItem, int aLayer ) const;
    void draw( const LIB_ITEM* aItem, int aLayer ) const;
    void draw( const SCH_SYMBOL* aSymbol, int aLayer ) const;
    void draw( const SCH_SHEET* aSheet, int aLayer ) const;
    void draw( const SCH_FIELD* aField, int aLayer, bool aDnp = false ) const;
    void draw( const SCH_HIERLABEL* aLabel, int aLayer, bool aDnp = false ) const;
    void draw( const SCH_GLOBALLABEL* aLabel, int aLayer, bool aDnp = false ) const;
    void draw( const SCH_TEXT* aText, int aLayer, bool aDnp = false ) const;
    void draw( const SCH_TEXTBOX* aTextBox, int aLayer, bool aDnp = false ) const;
    void draw( const SCH_JUNCTION* aJunction, int aLayer ) const;
    void draw( const SCH_LINE* aLine, int aLayer ) const;
    void draw( const SCH_BUS_WIRE_ENTRY* aEntry, int aLayer ) const;
    void draw( const SCH_BUS_BUS_ENTRY* aEntry, int aLayer ) const;
    void draw( const SCH_NO_CONNECT* aNC, int aLayer ) const;
    void draw( const SCH_PIN* aPin, int aLayer ) const;
    void draw( const LIB_SYMBOL* aSymbol, int aLayer ) const;
    void draw( const LIB_SHAPE* aShape, int aLayer ) const;
    void draw( const LIB_TEXT* aText, int aLayer ) const;
    void draw( const LIB_TEXTBOX* aTextBox, int aLayer ) const;
    void draw( const LIB_PIN* aPin, int aLayer ) const;
    void draw( const LIB_FIELD* aField, int aLayer ) const;

    void drawSymbolBorder( const SCH_SYMBOL* aSymbol, int aLayer ) const;
    void drawDanglingSymbolPin( const SCH_SYMBOL* aSymbol, const SCH_PIN* aPin ) const;
    void drawNoConnectMarker( const VECTOR2I& aPos ) const;
    void drawBusEntryShape( const VECTOR2I& aStart, const VECTOR2I& aEnd, bool aEntry ) const;
    void drawPinDot( const VECTOR2I& aPos, const COLOR4D& aColor ) const;
    void drawPinSymbol( const VECTOR2I& aPos, const VECTOR2I& aDir, bool aInverted,
                       ELECTRICAL_PINTYPE aType, const COLOR4D& aColor ) const;
    void drawPinText( const SCH_PIN* aPin, const wxString& aText, const VECTOR2I& aPos,
                     const COLOR4D& aColor, bool aHideIfSmall ) const;
    void drawFieldText( const SCH_FIELD* aField, const wxString& aText, const VECTOR2I& aPos,
                       const COLOR4D& aColor, bool aHideIfSmall ) const;
    void drawTextBox( const SCH_TEXTBOX* aTextBox, const wxString& aText, const VECTOR2I& aPos,
                     const COLOR4D& aColor, bool aHideIfSmall ) const;
    void drawLabel( const SCH_TEXT* aLabel, const wxString& aText, const VECTOR2I& aPos,
                   const COLOR4D& aColor, bool aHideIfSmall ) const;

private:
    CHEM_RENDER_SETTINGS m_settings;
};

} // namespace KIGFX

#endif // CHEM_PAINTER_H 