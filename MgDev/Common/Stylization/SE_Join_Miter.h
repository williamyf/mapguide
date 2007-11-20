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

#ifndef SE_JOIN_MITER_H
#define SE_JOIN_MITER_H

#include "SE_Join.h"

template<class USER_DATA> class SE_Join_Miter : public SE_Join<USER_DATA>
{
protected:
/* Using declarations to make the linux build happy */
using SE_Join<USER_DATA>::m_width;
using SE_Join<USER_DATA>::m_join_ext;
using SE_Join<USER_DATA>::m_lead;
using SE_Join<USER_DATA>::m_tail;
using SE_Join<USER_DATA>::m_lead_nml;
using SE_Join<USER_DATA>::m_tail_nml;
using SE_Join<USER_DATA>::m_lxt;
using SE_Join<USER_DATA>::m_colinear;

public:
    SE_Join_Miter( SE_RenderLineStyle* style );

    virtual void Construct( const SE_SegmentInfo& lead,
                            const SE_SegmentInfo& tail,
                            double& tolerance );
    virtual void Transform( SE_JoinTransform<USER_DATA>& joins );

protected:
    double m_sin_a;         /* The sine of the angle between the two segments */
    double m_cos_a;         /* The cosine of the angle between the two segments */
    double m_tan_ha;        /* The tangent of half the angle between the two segments */
    double m_sin_ha;        /* The sine of half the angle between the two segments */
    double m_cos_ha;        /* The cosine of half the angle between the two segments */

    double m_miter;         /* The distance from the inside of the join to the vertex (or the vertex to
                             * the end of the miter) */
    bool m_clockwise;
};

// Function Implementations

template<class USER_DATA>
SE_Join_Miter<USER_DATA>::SE_Join_Miter( SE_RenderLineStyle* style )
    : SE_Join<USER_DATA>(style)
{
}


template<class USER_DATA>
void SE_Join_Miter<USER_DATA>::Construct( const SE_SegmentInfo& lead,
                                          const SE_SegmentInfo& tail,
                                          double& tolerance )
{
    SE_Join<USER_DATA>::Construct(lead, tail, tolerance);

    if (m_colinear)
        return;

    /* We default conceptually to y-down */
    m_clockwise = m_lxt > 0;

    m_cos_a = -m_lead_nml.dot(m_tail_nml);
    m_sin_a = fabs(m_lxt);
    m_tan_ha = (1 - m_cos_a) / m_sin_a; /* Half-angle identity for tangent */
    m_cos_ha = sqrt((1 + m_cos_a) * 0.5); /* Half-angle identity for cosine */
    m_sin_ha = m_tan_ha * m_cos_ha;

    m_width = m_join_ext / m_tan_ha;
    m_miter = m_join_ext / m_sin_ha;
}


template<class USER_DATA>
void SE_Join_Miter<USER_DATA>::Transform(SE_JoinTransform<USER_DATA>& joins)
{
    /* Calculate the correct position in the case of closed contours */
    bool open = m_tail->vertpos >= m_lead->vertpos;

    /* The start and end of the line will have the same lead/tail segments, so we determine which
     * one is active by checking against the last vertex position */
    bool ending = joins.LastPosition() < m_lead->vertpos + m_lead->nextlen;
    double position =  !open && ending ? m_lead->vertpos + m_lead->nextlen : m_tail->vertpos;

    if (m_colinear)
    {
        if (!open)
        {
            joins.StartJoin(false);
            SE_Tuple cw_nml = SE_Tuple(-m_tail->next.y, m_tail->next.x) * (m_join_ext / m_tail->nextlen);
            joins.AddVertex( *m_tail->vertex + cw_nml,
                             *m_tail->vertex,
                             *m_tail->vertex - cw_nml,
                             position );
        }
        return;
    }

    joins.StartJoin(m_clockwise);

    SE_Tuple v_out = (m_lead_nml - m_tail_nml).normalize() * m_miter;

    joins.AddVertex( *m_tail->vertex + v_out,
                     *m_tail->vertex,
                     *m_tail->vertex - v_out,
                     position );
}

#endif // SE_JOIN_MITER_H
