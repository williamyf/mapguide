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

/// \ingroup Geometry_Module

//------------------------------------------------------------------------------
//
// FILE: orpplygn.h.
//
// PURPOSE: Declaration of the OrientedPolyPolygon class. The OrientedPolyPoly-
//          gon class is used to represent the output polygon generated by the
//          PlaneSweep class, and its various derivatives. It can represent a
//          poly-polygon with any number of disjoint exterior boundaries and
//          holes. Boundaries are oriented so that the polygon interior always
//          lies to the left of an edge (i.e., exterior boundaries have counter-
//          clockwise orientation, and the boundaries of holes have clockwise
//          orientation).
//
//------------------------------------------------------------------------------

#ifndef _ORRPPLYGN_H_
#define _ORRPPLYGN_H_

class OrientedPolyPolygon {
public:
    /// constructor/destructor

    OrientedPolyPolygon(int nExpectedBoundaries = 1);
    virtual ~OrientedPolyPolygon();

    OrientedPolyPolygon(const OrientedPolyPolygon &orientedPolyPolygon);
    OrientedPolyPolygon &operator=(const OrientedPolyPolygon &orientedPolyPolygon);


    /// methods to access the boundaries, and query boundary information

    int GetNBoundaries() const;
    int GetNBoundaryVertices(int boundaryIndex) const;
    int GetTotalVertices() const;
    const OpsFloatPoint *GetBoundary(int boundaryIndex) const;
    OpsFloatExtent GetExtent() const;
    void GetPolyPolygon(OpsFloatPoint vertices[], int nPolyVerts[],
        int &nPolygons) const;

    /// method to add a polygon boundary

    void AddBoundary(const OpsFloatPoint vertices[], int nVertices,
        const OpsFloatExtent &extent);

    /// method to test whether a boundary is contained in the poly-polygon

    BOOL BoundaryContained(const OpsFloatExtent &boundaryExt,
        const OpsDoublePoint &interiorPt, BOOL strictContainment) const;

private:
    int m_nBoundaries;
    int m_maxBoundaries;
    OpsFloatExtent *m_boundaryExt;
    int *m_nBoundaryVerts;
    int m_totalVertices;
    OpsFloatPoint **m_boundaries;
    OpsFloatExtent m_extentOfBoundaries;

    void Cleanup();
    void ResizeArrays();
};

#endif
