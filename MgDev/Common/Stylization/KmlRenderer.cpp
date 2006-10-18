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

#include "stdafx.h"

#include "KmlRenderer.h"
#include "LineStyle.h"
#include "SLDSymbols.h"
#include "UnicodeString.h"

/*#define ROUND(x) (int)((x) + 0.5)

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#define MAX4(x,y,z,w) \
    ((MAX((x),(y))) > (MAX((z),(w))) ? (MAX((x),(y))) : (MAX((z),(w))))
#define MIN4(x,y,z,w) \
    ((MIN((x),(y))) < (MIN((z),(w))) ? (MIN((x),(y))) : (MIN((z),(w))))

#define MAXX(x) MAX4(x[0],x[2],x[4],x[6])
#define MINX(x) MIN4(x[0],x[2],x[4],x[6])
#define MAXY(x) MAX4(x[1],x[3],x[5],x[7])
#define MINY(x) MIN4(x[1],x[3],x[5],x[7])

#define SYMBOL_BITMAP_SIZE 128
#define SYMBOL_BITMAP_MAX 1024*/

//using this in contructor
#pragma warning(disable:4355)

//default constructor
KmlRenderer::KmlRenderer(KmlContent* kmlContent, double scale, double dpi) :
    m_mainContent(kmlContent),
    m_kmlContent(kmlContent),
    m_styleContent(NULL),
    m_featureCount(0),
    m_layerInfo(NULL),
    m_featureClassInfo(NULL),
    m_scale(scale),
    m_styleId(0)
{
    m_kmlContent = m_mainContent;
    m_pixelSize = METERS_PER_INCH / dpi;
}


KmlRenderer::~KmlRenderer()
{
    ClearThemes();
    ClearStyles();
}

void KmlRenderer::StartMap(RS_MapUIInfo* mapInfo,
                          RS_Bounds&    extents,
                          double        mapScale,
                          double        dpi,
                          double        metersPerUnit,
                          CSysTransformer* /*xformToLL*/)
{
    m_featureCount = 0;
}


void KmlRenderer::EndMap()
{
}


void KmlRenderer::StartLayer(RS_LayerUIInfo*      legendInfo,
                            RS_FeatureClassInfo* classInfo)
{
    m_featureCount = 0;
    m_layerInfo = legendInfo;
    m_featureClassInfo = classInfo;
    ClearThemes();
    ClearStyles();
}


void KmlRenderer::EndLayer()
{
    //close the very last feature
    if(m_featureCount > 0)
    {
        //end last feature
        m_kmlContent->WriteString("</Placemark>");
    }
    m_kmlContent = m_mainContent;

    //write out the features for each theme
    for(ThemeMap::iterator iter = m_themeMap.begin(); iter != m_themeMap.end(); iter++)
    {
        m_kmlContent->WriteString("<Folder>");
        m_kmlContent->WriteString("<name><![CDATA[");
        m_kmlContent->WriteString((*iter).first);
        m_kmlContent->WriteString("]]></name>");
        m_kmlContent->WriteString((*iter).second->GetString());
        m_kmlContent->WriteString("</Folder>");
    }

    //write out the styles
    if(m_styleContent != NULL)
    {
        m_kmlContent->WriteString(m_styleContent->GetString());
    }

}


void KmlRenderer::StartFeature(RS_FeatureReader* /*feature*/,
                              const RS_String* tooltip,
                              const RS_String* url, 
                              const RS_String* theme)
{
    if(m_featureCount > 0)
    {
        //end last feature
        m_kmlContent->WriteString("</Placemark>");
    }
    if(theme == NULL || theme->empty())
    {
        m_kmlContent = m_mainContent;
    }
    else
    {
        ThemeMap::iterator iter = m_themeMap.find(*theme);
        if(iter != m_themeMap.end())
        {
            m_kmlContent = (*iter).second;
        }
        else
        {
            m_kmlContent = new KmlContent();
            m_themeMap[*theme] = m_kmlContent;
        }
    }
    m_kmlContent->WriteString("<Placemark>");
    if((tooltip != NULL && tooltip->length() > 0) || 
        (url != NULL && url->length() > 0))
    {
        m_kmlContent->WriteString("<description>");
        m_kmlContent->WriteString("<![CDATA[");
        if(tooltip != NULL && tooltip->length() > 0)
        {
            //replace line breaks
            RS_String newTT(*tooltip);
            RS_String::size_type lb = newTT.find(L"\\n");
            while(lb != RS_String::npos)
            {
                newTT = newTT.replace(lb, 2, L"<br/>");
                lb = newTT.find(L"\\n", lb);
            }
            m_kmlContent->WriteString(newTT);
            m_kmlContent->WriteString("<br/>");
        }
        if(url != NULL && url->length() > 0)
        {
            m_kmlContent->WriteString(*url);
        }
        m_kmlContent->WriteString("]]>");
        m_kmlContent->WriteString("</description>");
    }
    m_featureCount++;
}


void KmlRenderer::ProcessPolygon(LineBuffer* lb,
                                RS_FillStyle& fill)
{
    char buffer[256];

    //write style
    WriteStyle(fill);
    
    m_kmlContent->WriteString("<MultiGeometry>");
    int point_offset = 0;
    int numCntrs = lb->cntr_count();
    for(int i = 0; i < numCntrs; i++)
    {
        int cntr_size = lb->cntrs()[i];

        m_kmlContent->WriteString("<Polygon>");
        m_kmlContent->WriteString("<outerBoundaryIs>");
        m_kmlContent->WriteString("<LinearRing>");
        m_kmlContent->WriteString("<coordinates>");
        double* points = lb->points();
        for(int j = 0; j < cntr_size * 2; j += 2)
        {
            double x = points[point_offset + j];
            double y = points[point_offset + j + 1];
            if(j > 0)
            {
                m_kmlContent->WriteString(",");
            }
            sprintf(buffer, "%f, %f, 0.00000", x, y);
            m_kmlContent->WriteString(buffer);
        }
        m_kmlContent->WriteString("</coordinates>");
        m_kmlContent->WriteString("</LinearRing>");
        m_kmlContent->WriteString("</outerBoundaryIs>");
        m_kmlContent->WriteString("</Polygon>");

        point_offset += cntr_size * 2;
    }
    m_kmlContent->WriteString("</MultiGeometry>");
}


void KmlRenderer::ProcessPolyline(LineBuffer* srclb,
                                 RS_LineStroke& lsym)
{
    char buffer[256];
    
    //write style
    WriteStyle(lsym);

    m_kmlContent->WriteString("<MultiGeometry>");
    int point_offset = 0;
    int numCntrs = srclb->cntr_count();
    for(int i = 0; i < numCntrs; i++)
    {
        int cntr_size = srclb->cntrs()[i];

        m_kmlContent->WriteString("<LineString>");
        m_kmlContent->WriteString("<coordinates>");
        double* points = srclb->points();
        for(int j = 0; j < cntr_size * 2; j += 2)
        {
            double x = points[point_offset + j];
            double y = points[point_offset + j + 1];
            if(j > 0)
            {
                m_kmlContent->WriteString(",");
            }
            sprintf(buffer, "%f, %f, 0.00000", x, y);
            m_kmlContent->WriteString(buffer);
        }
        m_kmlContent->WriteString("</coordinates>");
        m_kmlContent->WriteString("</LineString>");

        point_offset += cntr_size * 2;
    }
    m_kmlContent->WriteString("</MultiGeometry>");
}


void KmlRenderer::ProcessRaster(unsigned char* data,
                               int length,
                               RS_ImageFormat format,
                               int width, int height,
                               RS_Bounds extents)
{
}


void KmlRenderer::ProcessMarker(LineBuffer* srclb, RS_MarkerDef& mdef, bool allowOverpost, RS_Bounds* bounds)
{
    for(int i = 0; i < srclb->point_count(); i++)
    {
        ProcessOneMarker(srclb->points()[2*i], srclb->points()[2*i+1], mdef, allowOverpost);
    }
}

void KmlRenderer::ProcessOneMarker(double x, double y, RS_MarkerDef& mdef, bool allowOverpost)
{
    char buffer[256];
    m_kmlContent->WriteString("<name>");
    m_kmlContent->WriteString(mdef.name());
    m_kmlContent->WriteString("</name>");
    m_kmlContent->WriteString("<Point>");
    m_kmlContent->WriteString("<coordinates>");
    sprintf(buffer, "%f, %f, 0.00000", x, y);
    m_kmlContent->WriteString(buffer);
    m_kmlContent->WriteString("</coordinates>");
    m_kmlContent->WriteString("</Point>");
}

void KmlRenderer::ProcessLabel(double x, double y, const RS_String& text, RS_TextDef& tdef)
{
}

void KmlRenderer::ProcessLabelGroup(RS_LabelInfo*    labels,
                                   int              nlabels,
                                   const RS_String& text,
                                   RS_OverpostType  type,
                                   bool             exclude,
                                   LineBuffer*      path)
{
    char buffer[256];
    m_kmlContent->WriteString("<name>");
    m_kmlContent->WriteString(text);
    m_kmlContent->WriteString("</name>");
/*    m_kmlContent->WriteString("<MultiGeometry>");
    for(int i = 0; i < nlabels; i++)
    {
        m_kmlContent->WriteString("<Point>");
        m_kmlContent->WriteString("<coordinates>");
        sprintf(buffer, "%f, %f, 0.00000", labels[i].x(), labels[i].y());
        m_kmlContent->WriteString(buffer);
        m_kmlContent->WriteString("</coordinates>");
        m_kmlContent->WriteString("</Point>");
    }
    m_kmlContent->WriteString("</MultiGeometry>");*/
}


void KmlRenderer::SetSymbolManager(RS_SymbolManager* manager)
{
}


RS_MapUIInfo* KmlRenderer::GetMapInfo()
{
    return NULL;
}


RS_LayerUIInfo* KmlRenderer::GetLayerInfo()
{
    return m_layerInfo;
}


RS_FeatureClassInfo* KmlRenderer::GetFeatureClassInfo()
{
    return m_featureClassInfo;
}


double KmlRenderer::GetMapScale()
{
    return m_scale;
}


RS_Bounds& KmlRenderer::GetBounds()
{
    return m_extents;
}


double KmlRenderer::GetDpi()
{
    return 96;
}


double KmlRenderer::GetMetersPerUnit()
{
    return 0;
}


double KmlRenderer::GetMapToScreenScale()
{
    return 0;
}


bool KmlRenderer::RequiresClipping()
{
    return false;
}


bool KmlRenderer::UseLocalOverposting()
{
    return false;
}

//Inserts the contents of a given DWF input stream
//into the current output W2D. The given coord sys
//transformation is applied and geometry will be clipped
//to the RS_Bounds context of the DWFRenderer
void KmlRenderer::AddDWFContent(RS_InputStream*   in,
                               CSysTransformer*  xformer,
                               const RS_String&  section,
                               const RS_String&  passwd,
                               const RS_String&  w2dfilter)
{
}

void KmlRenderer::ClearThemes()
{
    for(ThemeMap::iterator iter = m_themeMap.begin(); iter != m_themeMap.end(); iter++)
    {
        delete (*iter).second;
    }
    m_themeMap.clear();
}

void KmlRenderer::ClearStyles()
{
    if(m_styleContent != NULL)
    {
        delete m_styleContent;
        m_styleContent = NULL;
    }
}

void KmlRenderer::WriteStyle(RS_FillStyle& fill)
{
    if(m_styleContent == NULL)
    {
        m_styleContent = new KmlContent();
    }
    
    char buffer[256];

    int thisStyleId;
    KmlPolyStyle key(fill.outline().color().abgr(), fill.outline().width() / m_pixelSize, fill.color().abgr());
    KmlPolyStyleIdMap::iterator iter = m_polyStyleMap.find(key);
    if(iter != m_polyStyleMap.end())
    {
        thisStyleId = (*iter).second;
    }
    else
    {
        thisStyleId = m_polyStyleMap[key] = m_styleId;

        //write the style definitions into the style storage
        sprintf(buffer, "<Style id=\"%d\">", m_styleId++);
        m_styleContent->WriteString(buffer);

        //outline
        m_styleContent->WriteString("<LineStyle>");
        m_styleContent->WriteString("<color>");
        RS_Color color = fill.outline().color();
        sprintf(buffer, "%.2X%.2X%.2X%.2X", color.alpha(), color.blue(), color.green(), color.red());
        m_styleContent->WriteString(buffer);
        m_styleContent->WriteString("</color>");
        m_styleContent->WriteString("<width>");
        sprintf(buffer, "%f", key.m_lineWidth);
        m_styleContent->WriteString(buffer);
        m_styleContent->WriteString("</width>");
        m_styleContent->WriteString("</LineStyle>");

        //fill
        m_styleContent->WriteString("<PolyStyle>");
        m_styleContent->WriteString("<color>");
        color = fill.color();
        sprintf(buffer, "%.2X%.2X%.2X%.2X", color.alpha(), color.blue(), color.green(), color.red());
        m_styleContent->WriteString(buffer);
        m_styleContent->WriteString("</color>");
        m_styleContent->WriteString("</PolyStyle>");

        //icon
//        m_styleContent->WriteString("<IconStyle><Icon>");
//        m_styleContent->WriteString("<href>");
//        m_styleContent->WriteString("http://callt83-le:8008/mapguide/mapagent/mapagent.fcgi?OPERATION=GetLegendImage&amp;LAYERDEFINITION=Library://Samples/Sheboygan/Layers/Parcels.LayerDefinition&amp;WIDTH=32&amp;HEIGHT=32&amp;SCALE=4950&amp;FORMAT=PNG&amp;THEMECATEGORY=3&amp;TYPE=-1&amp;VERSION=1");
//        m_styleContent->WriteString("http://callt83-le:8008/mapguide/mapagent/icon.png");
//        m_styleContent->WriteString("</href>");        
//        m_styleContent->WriteString("</Icon></IconStyle>");
       
        m_styleContent->WriteString("</Style>");
    }
        
    //write a style reference into the main document
    sprintf(buffer, "<styleUrl>#%d</styleUrl>", thisStyleId);
    m_kmlContent->WriteString(buffer);
}

void KmlRenderer::WriteStyle(RS_LineStroke& lsym)
{
    if(m_styleContent == NULL)
    {
        m_styleContent = new KmlContent();
    }

    char buffer[256];
    int thisStyleId = 0;
    KmlLineStyle key(lsym.color().abgr(), lsym.width() / m_pixelSize );
    KmlLineStyleIdMap::iterator iter = m_lineStyleMap.find(key);
    if(iter != m_lineStyleMap.end())
    {
        thisStyleId = (*iter).second;
    }
    else
    {
        thisStyleId = m_lineStyleMap[key] = m_styleId;
        
        //write the style definitions into the style storage
        sprintf(buffer, "<Style id=\"%d\">", m_styleId++);
        m_styleContent->WriteString(buffer);

        //line style
        m_styleContent->WriteString("<LineStyle>");
        m_styleContent->WriteString("<color>");
        RS_Color color = lsym.color();
        sprintf(buffer, "%.2X%.2X%.2X%.2X", color.alpha(), color.blue(), color.green(), color.red());
        m_styleContent->WriteString(buffer);
        m_styleContent->WriteString("</color>");
        m_styleContent->WriteString("<width>");
        sprintf(buffer, "%f", key.m_width);
        m_styleContent->WriteString(buffer);
        m_styleContent->WriteString("</width>");
        m_styleContent->WriteString("</LineStyle>");
        
        m_styleContent->WriteString("</Style>");
    }

    //write a style reference into the main document
    sprintf(buffer, "<styleUrl>#%d</styleUrl>", thisStyleId);
    m_kmlContent->WriteString(buffer);
}


