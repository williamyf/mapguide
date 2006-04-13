//
//  Copyright (C) 2004-2006  Autodesk, Inc.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef FeatureInfoRenderer_H
#define FeatureInfoRenderer_H

#include "Renderer.h"

class MgFeatureReader;
class MgPropertyCollection;
class LineBuffer;
class KeyEncode;
class RSMgFeatureReader;

//A simple Renderer implementation for use by the QueryFeatures
//RenderingService API. Accumulates feature IDs and also attributes
//for one feature (case where a tooltip or attribute info needs to
//bedisplayed in the html viewer)
class MG_SERVER_RENDERING_API FeatureInfoRenderer : public Renderer
{

public:
    FeatureInfoRenderer(MgSelection* selection,
                        int maxFeatures,
                        double mapScale);
    virtual ~FeatureInfoRenderer();

    ///////////////////////////////////
    // Renderer implementation

    virtual void StartMap(RS_MapUIInfo* mapInfo,
                          RS_Bounds&    extents,
                          double        mapScale,
                          double        dpi,
                          double        metersPerUnit,
                          CSysTransformer* xformToLL)
    {}

    virtual void EndMap()
    {}

    virtual void StartLayer(RS_LayerUIInfo* legendInfo,
                            RS_FeatureClassInfo* classInfo);

    virtual void EndLayer()
    {}


    virtual void ProcessPolygon(LineBuffer* lb,
                                RS_FillStyle& fill)
    {}

    virtual void ProcessPolyline(LineBuffer* lb,
                                 RS_LineStroke& lsym)
    {}

    virtual void ProcessRaster(unsigned char* data,
                               int length,
                               RS_ImageFormat format,
                               int width, int height,
                               RS_Bounds extents)
    {}

    virtual void ProcessMarker(LineBuffer*   lb,
                               RS_MarkerDef& mdef,
                               bool          allowOverpost)
    {}


    virtual void ProcessLabel(double x, double y,
                              const RS_String& text, RS_TextDef& tdef)
    {}

    virtual void ProcessLabelGroup(RS_LabelInfo*    labels,
                                   int              nlabels,
                                   const RS_String& text,
                                   RS_OverpostType  type,
                                   bool             exclude,
                                   LineBuffer*      path)
    {}

    virtual void AddDWFContent(RS_InputStream*  in,
                               CSysTransformer* xformer,
                               const RS_String& section,
                               const RS_String& passwd,
                               const RS_String& filter)
    {}

    virtual void SetSymbolManager(RS_SymbolManager* manager)
    {}

    virtual void StartFeature(RS_FeatureReader* feature,
                              const RS_String*  tooltip = NULL,
                              const RS_String*  url = NULL);

    virtual double GetMapScale()
    {
        return m_mapScale;
    }

    virtual RS_Bounds& GetBounds()
    {
        // not directly used
        return m_extents;
    }

    virtual double GetDpi()
    {
        // not directly used - anything but zero is ok
        return 96.0;
    }

    virtual double GetMetersPerUnit()
    {
        // not directly used - anything but zero is ok
        return 1.0;
    }

    //note if we don't return false, we will need to return
    //correct bounds from GetBounds and we do not have them
    //available here, since out feature query is not a bounds query
    virtual bool RequiresClipping()
    {
        return false;
    }

    // not applicable
    virtual bool UseLocalOverposting()
    {
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    // FeatureInfoRenderer functions

    int GetNumFeaturesProcessed()
    {
        return m_numFeatures;
    }

    int GetNumMaxFeatures()
    {
        return m_nMaxFeatures;
    }

    RS_String GetUrl()
    {
        return m_url;
    }

    RS_String GetTooltip()
    {
        return m_tooltip;
    }

    MgPropertyCollection* GetProperties()
    {
        return SAFE_ADDREF(m_props);
    }

protected:
    //common to FeaturePropRenderer and FeatureInfoRenderer
    RS_String m_layerId;
    RS_String m_fcName;

    int m_numFeatures;
    int m_nMaxFeatures;

    KeyEncode* m_keyEncode;

    RS_FeatureClassInfo* m_fcInfo;

    MgSelection* m_selection;

    RS_Bounds m_extents;
    double m_mapScale;

private:
    
    //specific to FeatureInfoRenderer
    RS_String m_url;
    RS_String m_tooltip;
    MgPropertyCollection* m_props;
};

#endif
