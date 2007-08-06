//
//  Copyright (C) 2007 by Autodesk, Inc.
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
#include "SE_RenderProxies.h"
#include "SE_PositioningAlgorithms.h"
#include "SE_Renderer.h"
#include "SE_SymbolManager.h"
#include "SE_ConvexHull.h"
#include "SE_Bounds.h"
#include "Renderer.h"
#include "RS_FontEngine.h"

#include <algorithm>
#include <functional>


SE_RenderPointStyle* DeepClonePointStyle(SE_RenderPointStyle* st)
{
    SE_RenderPointStyle* ret = new SE_RenderPointStyle();
    *ret = *st;
    memcpy(ret->bounds, st->bounds, sizeof(ret->bounds));
    SE_RenderText* txt = new SE_RenderText();
    *txt = *((SE_RenderText*)st->symbol[0]);
    ret->symbol[0] = txt;

    return ret;
}


//recomputes the bounds of an SE_RenderPointStyle that contains a text
//whose alignment we have messed with
void UpdateStyleBounds(SE_RenderPointStyle* st, SE_Renderer* se_renderer)
{
    SE_RenderText* txt = ((SE_RenderText*)st->symbol[0]);

    RS_TextMetrics tm;
    SE_Matrix txf;
    se_renderer->GetRSFontEngine()->GetTextMetrics(txt->content, txt->tdef, tm, false);
    RS_F_Point fpts[4];

    //radian CCW rotation
    double rotation = txt->tdef.rotation() * M_PI180;
    double cos_a = cos(rotation);
    double sin_a = sin(rotation);

    RS_Bounds rotatedBounds(+DBL_MAX, +DBL_MAX, -DBL_MAX, -DBL_MAX);
    RS_Bounds unrotatedBounds(+DBL_MAX, +DBL_MAX, -DBL_MAX, -DBL_MAX);

    for (size_t k=0; k<tm.line_pos.size(); ++k)
    {
        //convert the unrotated measured bounds for the current line to a local point array
        memcpy(fpts, tm.line_pos[k].ext, sizeof(fpts));

        // process the extent points
        for (int j=0; j<4; ++j)
        {
            // update the overall unrotated bounds
            unrotatedBounds.add_point(fpts[j]);

            // rotate and translate to the insertion point
            double tmpX = fpts[j].x;
            double tmpY = fpts[j].y;
            fpts[j].x = tmpX * cos_a + tmpY * sin_a;
            fpts[j].y = tmpX * sin_a + tmpY * cos_a;

            // update the overall rotated bounds
            rotatedBounds.add_point(fpts[j]);
        }
    }

    txt->bounds[0].x = rotatedBounds.minx;
    txt->bounds[0].y = rotatedBounds.miny;
    txt->bounds[1].x = rotatedBounds.maxx;
    txt->bounds[1].y = rotatedBounds.miny;
    txt->bounds[2].x = rotatedBounds.maxx;
    txt->bounds[2].y = rotatedBounds.maxy;
    txt->bounds[3].x = rotatedBounds.minx;
    txt->bounds[3].y = rotatedBounds.maxy;

    memcpy(st->bounds, txt->bounds, sizeof(st->bounds));
}


void SE_PositioningAlgorithms::EightSurrounding(SE_Renderer*    se_renderer,
                                                LineBuffer*     geometry,
                                                SE_Matrix&      /*xform*/,
                                                SE_Style*       /*style*/,
                                                SE_RenderStyle* rstyle,
                                                double          mm2px)
{
    //this placement algorithm implements the MapGuide dynamic point labeling algorithm
    //which means 8 candidate labels generated for each symbol

    double cx = 0.0;
    double cy = 0.0;
    double dummy;

    //get actual feature point
    geometry->Centroid(LineBuffer::ctPoint, &cx, &cy, &dummy);

    //transform the point to screen space
    se_renderer->WorldToScreenPoint(cx, cy, cx, cy);

    //assume there is a single text label in the render symbol
    //and extract it from in there
    SE_RenderPointStyle* rstyle2 = new SE_RenderPointStyle();
    rstyle2->addToExclusionRegions = rstyle->addToExclusionRegions;
    rstyle2->checkExclusionRegions = rstyle->checkExclusionRegions;
    rstyle2->drawLast = rstyle->drawLast;
    rstyle2->renderPass = rstyle->renderPass;
    memcpy(rstyle2->bounds, rstyle->bounds, sizeof(rstyle2->bounds));
    SE_RenderText* txt = NULL;

    for (SE_RenderPrimitiveList::iterator iter = rstyle->symbol.begin(); iter != rstyle->symbol.end(); iter++)
    {
        if ((*iter)->type == SE_RenderTextPrimitive)
        {
            SE_RenderText* rt = (SE_RenderText*)(*iter);

            txt = new SE_RenderText();
            *txt = *rt;
            rstyle2->symbol.push_back(txt);

            break;
        }
    }

    //get the bounds of the last drawn point symbol, so that we know how much to offset the label
    //This call assumes the symbol draw right before the label and the symbol added its bounds
    //as an exclusion region

    const RS_F_Point* cfpts = se_renderer->GetLastExclusionRegion();
    RS_F_Point fpts[4];
    memcpy(fpts, cfpts, 4 * sizeof(RS_F_Point));

    double dx = fpts[1].x - fpts[0].x;
    double dy = fpts[1].y - fpts[0].y;
    double box_angle_rad = atan2(dy, dx);

    SE_Matrix ixform;
    ixform.translate(-cx, -cy); //factor out point position
    ixform.rotate(-box_angle_rad); //factor out rotation
    //double pixelToMeter = 0.001 / se_renderer->GetPixelsPerMillimeterScreen();
    //ixform.scale(pixelToMeter,  pixelToMeter); //convert from pixels to meters for labeling

    //factor out geometry position
    for (int i=0; i<4; i++)
        ixform.transform(fpts[i].x, fpts[i].y);

    //unrotated bounds
    RS_Bounds symbol_bounds(fpts[0].x, fpts[0].y, fpts[2].x, fpts[2].y);

    double symbol_width  = fpts[1].x - fpts[0].x; //symbol width in meters
    double symbol_height = fpts[2].y - fpts[1].y; //symbol height in meters
    double symbol_rot_deg = box_angle_rad / M_PI180;

    //calculate a 0.25 mm offset to allow for label ghosting
    double offsetmm = 0.25;             // offset in mm
    double offset = offsetmm * mm2px;   // offset in renderer pixels
    if (offset < 1.0)
        offset = 1.0;

    //compute how far label needs to be offset from
    //center point of symbol
    double w = 0.5 * symbol_width;
    double h = 0.5 * symbol_height;
    double ch = 0.0;    // vertical center point
    double cw = 0.0;    // horizontal center point

    w += offset;
    h += offset;

    bool useBounds = symbol_bounds.IsValid();
    if (useBounds)
    {
        symbol_bounds.maxx += offset;    symbol_bounds.maxy += offset;
        symbol_bounds.minx -= offset;    symbol_bounds.miny -= offset;
        ch = 0.5*(symbol_bounds.maxy + symbol_bounds.miny);
        cw = 0.5*(symbol_bounds.maxx + symbol_bounds.minx);
    }

    //take into account rotation of the symbol
    //find increased extents of the symbol bounds
    //due to the rotation
    double op_pts[16];
    if (symbol_rot_deg != 0.0)
    {
        double rotRad = symbol_rot_deg * M_PI180; //symbol_rot assumed to be in radians
        double cs = cos(rotRad);
        double sn = sin(rotRad);

        // check to see if the bounds have been set
        double wcs, nwcs, wsn, nwsn, hsn, nhsn, hcs, nhcs, cwsn, cwcs, chsn, chcs;
        if (useBounds)
        {
            wcs = symbol_bounds.maxx * cs;   nwcs = symbol_bounds.minx * cs;
            wsn = symbol_bounds.maxx * sn;   nwsn = symbol_bounds.minx * sn;
            hsn = symbol_bounds.maxy * sn;   nhsn = symbol_bounds.miny * sn;
            hcs = symbol_bounds.maxy * cs;   nhcs = symbol_bounds.miny * cs;
        }
        else
        {
            wcs = w * cs;   nwcs = -wcs;
            wsn = w * sn;   nwsn = -wsn;
            hsn = h * sn;   nhsn = -hsn;
            hcs = h * cs;   nhcs = -hcs;
        }

        cwsn = cw * sn;     chsn = ch * sn;
        cwcs = cw * cs;     chcs = ch * cs;

        // find the octant that the marker is rotated into, and shift the points accordingly.
        // this way, the overpost points are still within 22.5 degrees of an axis-aligned box.
        // (position 0 will always be the closest to Center-Right)
        double nangle = fmod(symbol_rot_deg, 360.0);
        if (nangle < 0.0)
            nangle += 360.0;
        int i = (((int)((nangle/45.0) + 0.5)) << 1) & 0x0000000f; // i is 2 * the octant
        op_pts[i++] = wcs - chsn;  op_pts[i++] = wsn + chcs;   i &= 0x0000000f; // & 15 does (mod 16)
        op_pts[i++] = wcs - hsn;   op_pts[i++] = wsn + hcs;    i &= 0x0000000f;
        op_pts[i++] = cwcs - hsn;  op_pts[i++] = cwsn + hcs;   i &= 0x0000000f;
        op_pts[i++] = nwcs - hsn;  op_pts[i++] = nwsn + hcs;   i &= 0x0000000f;
        op_pts[i++] = nwcs - chsn; op_pts[i++] = nwsn + chcs;  i &= 0x0000000f;
        op_pts[i++] = nwcs - nhsn; op_pts[i++] = nwsn + nhcs;  i &= 0x0000000f;
        op_pts[i++] = cwcs -nhsn;  op_pts[i++] = cwsn + nhcs;  i &= 0x0000000f;
        op_pts[i++] = wcs - nhsn;  op_pts[i  ] = wsn + nhcs;
    }
    else
    {
        if (!useBounds)
        {
            symbol_bounds.maxx = w; symbol_bounds.minx = -w;
            symbol_bounds.maxy = h; symbol_bounds.miny = -h;
        }
        op_pts[0 ] = symbol_bounds.maxx; op_pts[1 ] = ch;
        op_pts[2 ] = symbol_bounds.maxx; op_pts[3 ] = symbol_bounds.maxy;
        op_pts[4 ] = cw;                 op_pts[5 ] = symbol_bounds.maxy;
        op_pts[6 ] = symbol_bounds.minx; op_pts[7 ] = symbol_bounds.maxy;
        op_pts[8 ] = symbol_bounds.minx; op_pts[9 ] = ch;
        op_pts[10] = symbol_bounds.minx; op_pts[11] = symbol_bounds.miny;
        op_pts[12] = cw;                 op_pts[13] = symbol_bounds.miny;
        op_pts[14] = symbol_bounds.maxx; op_pts[15] = symbol_bounds.miny;
    }

    //OK, who says I can't write bad code? Behold:
    SE_LabelInfo candidates[8];
    double yScale = se_renderer->YPointsUp()? 1.0 : -1.0; //which way does y go in the renderer?

    SE_RenderPointStyle* st0 = DeepClonePointStyle(rstyle2);
    ((SE_RenderText*)st0->symbol[0])->tdef.halign() = RS_HAlignment_Left;
    ((SE_RenderText*)st0->symbol[0])->tdef.valign() = RS_VAlignment_Half;
    UpdateStyleBounds(st0, se_renderer);
    candidates[0] = SE_LabelInfo(cx + op_pts[0], cy + op_pts[1]*yScale, RS_Units_Device, 0.0, st0);

    SE_RenderPointStyle* st1 = DeepClonePointStyle(st0);
    ((SE_RenderText*)st1->symbol[0])->tdef.valign() = RS_VAlignment_Descent;
    UpdateStyleBounds(st1, se_renderer);
    candidates[1] = SE_LabelInfo(cx + op_pts[2], cy + op_pts[3]*yScale, RS_Units_Device, 0.0, st1);

    SE_RenderPointStyle* st2 = DeepClonePointStyle(st1);
    ((SE_RenderText*)st2->symbol[0])->tdef.halign() = RS_HAlignment_Center;
    UpdateStyleBounds(st2, se_renderer);
    candidates[2] = SE_LabelInfo(cx + op_pts[4], cy + op_pts[5]*yScale, RS_Units_Device, 0.0, st2);

    SE_RenderPointStyle* st3 = DeepClonePointStyle(st2);
    ((SE_RenderText*)st3->symbol[0])->tdef.halign() = RS_HAlignment_Right;
    UpdateStyleBounds(st3, se_renderer);
    candidates[3] = SE_LabelInfo(cx + op_pts[6], cy + op_pts[7]*yScale, RS_Units_Device, 0.0, st3);

    SE_RenderPointStyle* st4 = DeepClonePointStyle(st3);
    ((SE_RenderText*)st4->symbol[0])->tdef.valign() = RS_VAlignment_Half;
    UpdateStyleBounds(st4, se_renderer);
    candidates[4] = SE_LabelInfo(cx + op_pts[8], cy + op_pts[9]*yScale, RS_Units_Device, 0.0, st4);

    SE_RenderPointStyle* st5 = DeepClonePointStyle(st4);
    ((SE_RenderText*)st5->symbol[0])->tdef.valign() = RS_VAlignment_Ascent;
    UpdateStyleBounds(st5, se_renderer);
    candidates[5] = SE_LabelInfo(cx + op_pts[10], cy + op_pts[11]*yScale, RS_Units_Device, 0.0, st5);

    SE_RenderPointStyle* st6 = DeepClonePointStyle(st5);
    ((SE_RenderText*)st6->symbol[0])->tdef.halign() = RS_HAlignment_Center;
    UpdateStyleBounds(st6, se_renderer);
    candidates[6] = SE_LabelInfo(cx + op_pts[12], cy + op_pts[13]*yScale, RS_Units_Device, 0.0, st6);

    SE_RenderPointStyle* st7 = DeepClonePointStyle(st6);
    ((SE_RenderText*)st7->symbol[0])->tdef.halign() = RS_HAlignment_Left;
    UpdateStyleBounds(st7, se_renderer);
    candidates[7] = SE_LabelInfo(cx + op_pts[14], cy + op_pts[15]*yScale, RS_Units_Device, 0.0, st7);

    se_renderer->ProcessLabelGroup(candidates, 8, RS_OverpostType_FirstFit, true, NULL);
}


void SE_PositioningAlgorithms::PathLabels(SE_Renderer*    se_renderer,
                                          LineBuffer*     geometry,
                                          SE_Matrix&      /*xform*/,
                                          SE_Style*       /*style*/,
                                          SE_RenderStyle* rstyle,
                                          double          /*mm2px*/)
{
    //This placement algorithm implements MapGuide path labels -- periodic text label along
    //a linestring or multi line string feature, with stitching of adjacent features that have the
    //same label

    //assume that a single text was used in the SymbolDefinition that requests this positioning algorithm
    SE_RenderText* rt = (SE_RenderText*)rstyle->symbol[0];

    RS_LabelInfo info(0.0, 0.0, 0.0, 0.0, RS_Units_Device, rt->tdef, true);

    //TODO: get rid of this dynamic_cast once we fix the class hierarchy
    Renderer* renderer = dynamic_cast<Renderer*>(se_renderer);
    renderer->ProcessLabelGroup(&info, 1, rt->content, RS_OverpostType_AllFit, true, geometry);
}


void SE_PositioningAlgorithms::MultipleHighwaysShields(SE_Renderer*    renderer,
                                                       LineBuffer*     geometry,
                                                       SE_Matrix&      /*xform*/,
                                                       SE_Style*       /*seStyle*/,
                                                       SE_RenderStyle* rstyle,
                                                       double          mm2px,
                                                       RS_FeatureReader* featureReader,
                                                       SE_SymbolManager* symbolManager)
{
    // highway info format:  countryCode|type1|num1|type2|num2|type3|num3|...
    // example:              US|2|101|3|1
    StringOfTokens highwayInfo = StringOfTokens(featureReader->GetString(L"Url"), L"|");

    int shieldCount = (highwayInfo.getTokenCount() - 1) / 2;
    if (shieldCount < 1)
        return;

    SE_RenderLineStyle* rlStyle = (SE_RenderLineStyle*)rstyle;
    double startOffset = rlStyle->startOffset;
    double increment = rlStyle->repeat;

    // the endOffset is used in this context as the increment between multiple shields in one group
//  double incrementS = 10.0 * mm2px;
    double incrementS = rlStyle->endOffset;

    // calc the overall length of this geometry
    double totalLen = 0.0;
    for (int i = 0; i < geometry->cntr_count(); i++)
    {
        int pt = geometry->contour_start_point(i);
        int last = geometry->contour_end_point(i);
        while (pt < last)
        {
            // transform the point to screen space
            double  cx1, cy1, cx2, cy2;
            renderer->WorldToScreenPoint(geometry->x_coord(pt), geometry->y_coord(pt), cx1, cy1);
            pt++;
            renderer->WorldToScreenPoint(geometry->x_coord(pt), geometry->y_coord(pt), cx2, cy2);

            // calc length
            double dx = cx2 - cx1;
            double dy = cy2 - cy1;
            totalLen += sqrt(dx*dx + dy*dy);
        }
    }

    if (startOffset >= 0.0)
    {
        // calc optimal start offset (with rlStyle->startOffset taken as a minimum)
        // to co-locate shield groups placed on two-line highways where the two
        // parallel lines are processed from opposit ends.
        // this avoids a problem with perceived irregular placement when overposting
        // removes just some of the duplicate shields

        double shieldGroupLen = (shieldCount - 1) * incrementS;

        // length in excess of the required length to place one group with startOffset on each side
        double availLen = totalLen - (shieldGroupLen + 2.0 * startOffset);

        if (availLen < 0.0)
        {
            // there is no room to 'properly' place even one group, nothing to do but cry about it
            return;
        }

        int numAdditionalGroups = (int) (availLen / (shieldGroupLen + increment));

        double additionalOffset = (availLen - numAdditionalGroups * (shieldGroupLen + increment)) / 2;

        startOffset += additionalOffset;
    }
    else
    {
        // negative startOffset value disables the optimization
        // use absolute value as the offset
        startOffset = -startOffset;
    }

    SE_RenderPrimitiveList * symbolVectors = new SE_RenderPrimitiveList[shieldCount];

    std::wstring countryCode = highwayInfo.getFirstToken();

    int shieldIndex;
    for (shieldIndex = 0; shieldIndex < shieldCount; shieldIndex++)
    {
        std::wstring shieldType = highwayInfo.getNextToken();
        std::wstring highwayNum = highwayInfo.getNextToken();

        // first the shield graphic

        SE_RenderRaster* rr = new SE_RenderRaster();

        std::wstring imgName = HIGWAY_SHIELD_SYMBOLS_PREFIX + countryCode + L"_" + shieldType + L".png";

        symbolManager->GetImageData(HIGWAY_SHIELD_SYMBOLS_RESOURCE.c_str(), imgName.c_str(), rr->imageData);

        if (rr->imageData.size == 0)
        {
            // could not find the image or resource

            // we could fall back and try to pick up the image from a disk file like this:
            //  std::wstring imgPathName = HIGWAY_SHIELD_SYMBOLS_LOCATION + imgName;
            //  rr->pngPtr = symbolManager->GetImageData(L"", imgPathName.c_str(), rr->pngSize);
            // but let's not do that unless really necessary

            // cannot just leave this shield empty, that causes exceptions later, so bail out
            // TODO: find a better way to handle this condition
            return;
        }

        rr->position[0] = 0.0;
        rr->position[1] = 0.0;
        rr->extent[0] = 20.0;
        rr->extent[1] = 20.0;
        rr->angleRad = 0.0;

        if (highwayNum.length() == 1)
            rr->extent[0] = ((shieldType == L"3")? 25.0 : 20.0);
        else if (highwayNum.length() == 2)
            rr->extent[0] = 25.0;
        else
            rr->extent[0] = 30.0;

        double w = 0.5 * rr->extent[0];
        double h = 0.5 * rr->extent[1];

        rr->bounds[0].x = -w;
        rr->bounds[0].y = -h;
        rr->bounds[1].x =  w;
        rr->bounds[1].y = -h;
        rr->bounds[2].x =  w;
        rr->bounds[2].y =  h;
        rr->bounds[3].x = -w;
        rr->bounds[3].y =  h;

        // the shield graphic is ready
        symbolVectors[shieldIndex].push_back(rr);

        // now symbol for the highway number
        SE_RenderText* rt = new SE_RenderText();

        rt->content = highwayNum;
        rt->position[0] = 0.0;
        rt->position[1] = 0.0;
        rt->tdef.font().name() = L"Arial";
        rt->tdef.font().height() = 10.0*0.001 / mm2px; // convert mm to meters
        rt->tdef.rotation() = 0.0;

        rt->tdef.halign() = RS_HAlignment_Center;
        rt->tdef.valign() = RS_VAlignment_Half;

        if (shieldType == L"1")
        {
            rt->tdef.textcolor() = RS_Color(255, 255, 255, 255);
        }
        else
        {
            rt->tdef.textcolor() = RS_Color(0, 0, 0, 255);
        }

        rt->tdef.textbg() = RS_TextBackground_None;
        rt->tdef.font().style() = RS_FontStyle_Bold;

        // the number graphic is ready
        symbolVectors[shieldIndex].push_back(rt);
    }

    SE_Matrix symxf;
    shieldIndex = 0;

    // init position along the whole geometry to the start offset
    double drawpos = startOffset;

    for (int j=0; j<geometry->cntr_count(); j++)
    {
        // current polyline
        int pt = geometry->contour_start_point(j);
        int last = geometry->contour_end_point(j);

        while (pt < last)
        {
            symxf.setIdentity();

            // current line segment

            // transform the point to screen space
            double  cx1, cy1, cx2, cy2;
            renderer->WorldToScreenPoint(geometry->x_coord(pt), geometry->y_coord(pt), cx1, cy1);
            pt++;
            renderer->WorldToScreenPoint(geometry->x_coord(pt), geometry->y_coord(pt), cx2, cy2);

            // calc length
            double dx = cx2 - cx1;
            double dy = cy2 - cy1;
            double len = sqrt(dx*dx + dy*dy);

            // check if completely skipping current segment since it is smaller than the increment
            if (drawpos < len)
            {
                double invlen = 1.0 / len;
                double dx_fact = dx * invlen;
                double dy_fact = dy * invlen;

                double tx = cx1 + dx_fact * drawpos;
                double ty = cy1 + dy_fact * drawpos;
                symxf.translate(tx, ty);

                double dx_incr = dx_fact * increment;      // x/y incr between groups of shields
                double dy_incr = dy_fact * increment;

                double dx_incS = dx_fact * incrementS;     // x/y incr between shields in a group
                double dy_incS = dy_fact * incrementS;

                // follow the segment and place the shield symbols alternated via shieldIndex
                while (drawpos < len)
                {
                    if (rlStyle->drawLast)
                    {
                        rlStyle->symbol = symbolVectors[shieldIndex];
                        memcpy(rlStyle->bounds, symbolVectors[shieldIndex].front()->bounds, sizeof(rlStyle->bounds));
                        SE_RenderStyle* clonedStyle = renderer->CloneRenderStyle(rlStyle);

                        SE_LabelInfo info(symxf.x2, symxf.y2, RS_Units_Device, 0, clonedStyle);

                        renderer->ProcessLabelGroup(&info, 1, RS_OverpostType_AllFit, rlStyle->addToExclusionRegions, geometry);
                    }
                    else
                    {
                        renderer->DrawSymbol(symbolVectors[shieldIndex], symxf, 0.0);

                        // TODO: if this is ever needed ...
//                      if (rlStyle->addToExclusionRegions)
//                          renderer->AddExclusionRegion(style, symxf, 0.0);
                    }

                    // move on to the next shield, if beyond the last one go back to the first
                    shieldIndex++;
                    if (shieldIndex < shieldCount)
                    {
                        symxf.translate(dx_incS, dy_incS);
                        drawpos += incrementS;
                    }
                    else
                    {
                        // finished with one group
                        // go back to the first symbol and advance using the full increment

                        shieldIndex = 0;
                        symxf.translate(dx_incr, dy_incr);
                        drawpos += increment;
                    }
                }
            }

            drawpos -= len;
        }
    }

    // cleanup time

    // the rlStyle->symbol has been assigned various values from the 'symbolVectors',
    // 'un-assign' it, so that we can clean them up
    rlStyle->symbol.clear();

    for (shieldIndex = 0; shieldIndex < shieldCount; shieldIndex++)
    {
        for (SE_RenderPrimitiveList::iterator iter = symbolVectors[shieldIndex].begin();
             iter != symbolVectors[shieldIndex].end();
             iter++)
        {
            // necessary since destructor of SE_RenderPrimitive is not virtual
            switch ((*iter)->type)
            {
                case SE_RenderPolylinePrimitive: delete (SE_RenderPolyline*)(*iter);break;
                case SE_RenderPolygonPrimitive: delete (SE_RenderPolygon*)(*iter);break;
                case SE_RenderRasterPrimitive: delete (SE_RenderRaster*)(*iter);break;
                case SE_RenderTextPrimitive: delete (SE_RenderText*)(*iter);break;
                default: throw; //means there is a bug
            }
        }
    }

    delete [] symbolVectors;
}


int StringOfTokens::getTokenCount()
{
    if (m_tokenstring.empty())
    {
        return 0;
    }

    std::wstring::size_type curPos = 0;
    std::wstring::size_type delimPos;
    int count = 1;

    delimPos = m_tokenstring.find(m_delimiter, curPos);

    while (std::wstring::npos != delimPos)
    {
        curPos = delimPos + 1;
        count++;

        delimPos = m_tokenstring.find(m_delimiter, curPos);
    }

    return count;
}


std::wstring StringOfTokens::getFirstToken()
{
    if (m_tokenstring.empty())
    {
        return L"";
    }

    m_currentPos = 0;

    std::wstring::size_type delimPos1 = m_tokenstring.find(m_delimiter);

    if (std::wstring::npos == delimPos1)
    {
        m_currentPos = m_tokenstring.length();
        return m_tokenstring;
    }
    else
    {
        m_currentPos = delimPos1+1;
        return m_tokenstring.substr(0, delimPos1);
    }
}


std::wstring StringOfTokens::getNextToken()
{
    if (m_tokenstring.empty() || m_currentPos >= m_tokenstring.length())
    {
        return L"";
    }

    std::wstring::size_type curPos = m_currentPos;
    std::wstring::size_type delimPos = m_tokenstring.find(m_delimiter, m_currentPos);

    if (std::wstring::npos == delimPos)
    {
        m_currentPos = m_tokenstring.length();
        return m_tokenstring.substr(curPos);
    }
    else
    {
        m_currentPos = delimPos+1;
        return m_tokenstring.substr(curPos, delimPos - curPos);
    }
}
