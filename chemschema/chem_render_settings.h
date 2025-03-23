#ifndef CHEM_RENDER_SETTINGS_H
#define CHEM_RENDER_SETTINGS_H

#include <render_settings.h>
#include <settings/color_settings.h>

namespace KIGFX
{

class CHEM_RENDER_SETTINGS : public RENDER_SETTINGS
{
public:
    CHEM_RENDER_SETTINGS();
    virtual ~CHEM_RENDER_SETTINGS() {}

    /// @copydoc RENDER_SETTINGS::LoadColors()
    virtual void LoadColors( const COLOR_SETTINGS* aSettings ) override;

    /// @copydoc RENDER_SETTINGS::GetColor()
    virtual const COLOR4D& GetColor( const VIEW_ITEM* aItem, int aLayer ) const override;

    bool m_ShowUnit;                  ///< Show selected unit for multi-unit symbols
    bool m_ShowBodyStyle;            ///< Show selected body style for symbols with alternate body styles
    bool m_ShowPinNumbers;           ///< Show pin numbers
    bool m_ShowPinNames;             ///< Show pin names
    bool m_ShowPinElectricalType;    ///< Show pin electrical type
    bool m_ShowDisabled;             ///< Show disabled items
    bool m_ShowGraphicsDisabled;     ///< Show disabled graphics
    bool m_ShowHiddenText;           ///< Show hidden text
    bool m_ShowHiddenPins;           ///< Show hidden pins
    bool m_ShowPinFunction;          ///< Show pin function
    bool m_ShowPinNetname;           ///< Show pin netname
    bool m_ShowPinType;              ///< Show pin type
    bool m_ShowPinShape;             ///< Show pin shape
    bool m_ShowPinAttributes;        ///< Show pin attributes
    bool m_ShowPinDescription;       ///< Show pin description
    bool m_ShowPinLength;            ///< Show pin length
    bool m_ShowPinPadName;          ///< Show pin pad name
    bool m_ShowPinPadNumber;        ///< Show pin pad number
    bool m_ShowPinPadShape;         ///< Show pin pad shape
    bool m_ShowPinPadSize;          ///< Show pin pad size
    bool m_ShowPinPadLayers;        ///< Show pin pad layers
    bool m_ShowPinPadType;          ///< Show pin pad type
    bool m_ShowPinPadOrientation;   ///< Show pin pad orientation
    bool m_ShowPinPadDrill;         ///< Show pin pad drill
    bool m_ShowPinPadOffset;        ///< Show pin pad offset
    bool m_ShowPinPadClearance;     ///< Show pin pad clearance
    bool m_ShowPinPadMask;          ///< Show pin pad mask
    bool m_ShowPinPadPaste;         ///< Show pin pad paste
    bool m_ShowPinPadThermal;       ///< Show pin pad thermal
    bool m_ShowPinPadZone;          ///< Show pin pad zone
    bool m_ShowPinPadNet;           ///< Show pin pad net
    bool m_ShowPinPadNetClass;      ///< Show pin pad net class
    bool m_ShowPinPadNetName;       ///< Show pin pad net name
    bool m_ShowPinPadNetNumber;     ///< Show pin pad net number
    bool m_ShowPinPadNetType;       ///< Show pin pad net type
    bool m_ShowPinPadNetAttributes; ///< Show pin pad net attributes
    bool m_ShowPinPadNetDescription; ///< Show pin pad net description
    bool m_ShowPinPadNetClass;      ///< Show pin pad net class
    bool m_ShowPinPadNetClassName;  ///< Show pin pad net class name
    bool m_ShowPinPadNetClassDescription; ///< Show pin pad net class description
    bool m_ShowPinPadNetClassAttributes; ///< Show pin pad net class attributes
    bool m_ShowPinPadNetClassClearance; ///< Show pin pad net class clearance
    bool m_ShowPinPadNetClassTrackWidth; ///< Show pin pad net class track width
    bool m_ShowPinPadNetClassViaDiameter; ///< Show pin pad net class via diameter
    bool m_ShowPinPadNetClassViaDrill; ///< Show pin pad net class via drill
    bool m_ShowPinPadNetClassDiffPairWidth; ///< Show pin pad net class diff pair width
    bool m_ShowPinPadNetClassDiffPairGap; ///< Show pin pad net class diff pair gap
    bool m_ShowPinPadNetClassDiffPairViaGap; ///< Show pin pad net class diff pair via gap
    bool m_ShowPinPadNetClassUViaSize; ///< Show pin pad net class uvia size
    bool m_ShowPinPadNetClassUViaDrill; ///< Show pin pad net class uvia drill
    bool m_ShowPinPadNetClassUViaPadSize; ///< Show pin pad net class uvia pad size
    bool m_ShowPinPadNetClassUViaMask; ///< Show pin pad net class uvia mask
    bool m_ShowPinPadNetClassUViaPaste; ///< Show pin pad net class uvia paste
    bool m_ShowPinPadNetClassUViaThermal; ///< Show pin pad net class uvia thermal
    bool m_ShowPinPadNetClassUViaZone; ///< Show pin pad net class uvia zone
    bool m_ShowPinPadNetClassUViaNet; ///< Show pin pad net class uvia net
    bool m_ShowPinPadNetClassUViaNetClass; ///< Show pin pad net class uvia net class
    bool m_ShowPinPadNetClassUViaNetName; ///< Show pin pad net class uvia net name
    bool m_ShowPinPadNetClassUViaNetNumber; ///< Show pin pad net class uvia net number
    bool m_ShowPinPadNetClassUViaNetType; ///< Show pin pad net class uvia net type
    bool m_ShowPinPadNetClassUViaNetAttributes; ///< Show pin pad net class uvia net attributes
    bool m_ShowPinPadNetClassUViaNetDescription; ///< Show pin pad net class uvia net description

    double m_DefaultLineWidth;       ///< Default line width
    double m_TextOffsetRatio;        ///< Text offset ratio
    double m_PinSymbolSize;          ///< Pin symbol size
    double m_JunctionSize;           ///< Junction size
    double m_DashedLineDashRatio;    ///< Dashed line dash ratio
    double m_DashedLineGapRatio;     ///< Dashed line gap ratio
    double m_LabelSizeRatio;         ///< Label size ratio
};

} // namespace KIGFX

#endif // CHEM_RENDER_SETTINGS_H 