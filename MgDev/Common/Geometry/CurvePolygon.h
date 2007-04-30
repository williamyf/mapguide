//
//  Copyright (C) 2004-2007 by Autodesk, Inc.
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

#ifndef _MGCURVEPOLYGON_H_
#define _MGCURVEPOLYGON_H_

/// \defgroup MgCurvePolygon MgCurvePolygon
/// \ingroup Geometry_Module_classes
/// \{

class MgCurvePolygon;
template class MG_GEOMETRY_API Ptr<MgCurvePolygon>;

//////////////////////////////////////////////////////////////////
/// \brief
/// An MgCurvePolygon is a region, defined by 1 exterior boundary
/// and 0 or more interior boundaries.
///
/// \remarks
/// Each interior boundary
/// defines a hole in the polygon. An interior or exterior
/// boundary is defined as an MgCurveRing composed of one or more
/// curve segments. The curve segments are connected to one
/// another so that the end point of each segment is the start
/// point of the next segment in the segment list. The end point
/// of the last segment in the ring is the start point of the
/// first segment.
///
/// An object of this type is constructed by calling the
/// non-static MgGeometryFactory::CreateCurvePolygon() method
/// and, once constructed, is immutable.
/// <!-- Example (PHP) -->
/// \htmlinclude PHPExampleTop.html
/// The following code shows the construction of a circle, which
/// has a circular hole cut out of the middle of it.
///
/// \code
/// $geometryFactory = new MgGeometryFactory();
/// $curveSegmentCollection = new MgCurveSegmentCollection();
/// $curveRingCollection = new MgCurveRingCollection();
///
/// // create first curve segment for the external boundary
/// $startCoordinate = $geometryFactory->CreateCoordinateXY(0,2);
/// $controlCoordinate =
/// $geometryFactory->CreateCoordinateXY(2,4);
/// $endCoordinate = $geometryFactory->CreateCoordinateXY(4,2);
/// $arcSegment =
/// $geometryFactory->CreateArcSegment($startCoordinate,
/// $endCoordinate, $controlCoordinate);
/// $index = $curveSegmentCollection->Add($arcSegment);
///
/// // create second curve segment for the external boundary
/// $startCoordinate = $geometryFactory->CreateCoordinateXY(4,2);
/// $controlCoordinate =
/// $geometryFactory->CreateCoordinateXY(2,0);
/// $endCoordinate = $geometryFactory->CreateCoordinateXY(0,2);
/// $arcSegment =
/// $geometryFactory->CreateArcSegment($startCoordinate,
/// $endCoordinate, $controlCoordinate);
/// $index = $curveSegmentCollection->Add($arcSegment);
///
/// // create curve ring for the external boundary
/// $exteriorCurveRing =
/// $geometryFactory->CreateCurveRing($curveSegmentCollection);
///
/// // clear the collection before adding the next curve segment
/// $curveSegmentCollection->Clear();
///
/// // create first curve segment for an internal boundary
/// $startCoordinate = $geometryFactory->CreateCoordinateXY(1,2);
/// $controlCoordinate =
/// $geometryFactory->CreateCoordinateXY(2,3);
/// $endCoordinate = $geometryFactory->CreateCoordinateXY(3,2);
/// $arcSegment =
/// $geometryFactory->CreateArcSegment($startCoordinate,
/// $endCoordinate, $controlCoordinate);
/// $index = $curveSegmentCollection->Add($arcSegment);
///
/// // create second curve segment for an internal boundary
/// $startCoordinate = $geometryFactory->CreateCoordinateXY(3,2);
/// $controlCoordinate =
/// $geometryFactory->CreateCoordinateXY(2,1);
/// $endCoordinate = $geometryFactory->CreateCoordinateXY(1,2);
/// $arcSegment =
/// $geometryFactory->CreateArcSegment($startCoordinate,
/// $endCoordinate, $controlCoordinate);
/// $index = $curveSegmentCollection->Add($arcSegment);
///
/// // create curve ring for an internal boundary
/// $interiorCurveRing =
/// $geometryFactory->CreateCurveRing($curveSegmentCollection);
/// $index = $curveRingCollection->Add($interiorCurveRing);
///
/// // create curve polygon
/// $curvePolygon =
/// $geometryFactory->CreateCurvePolygon($exteriorCurveRing,
/// $curveRingCollection);
///
/// // print out the Agf Text string for the geometry
/// $curvePolygonAgfText =
/// $wktReaderWriter->Write($curvePolygon);
/// echo "AGF Text representation of CurvePolygon:
/// $curvePolygonAgfTextn";
/// \endcode
/// \htmlinclude ExampleBottom.html
class MG_GEOMETRY_API MgCurvePolygon : public MgRegion
{
    DECLARE_CREATE_OBJECT()
    DECLARE_CLASSNAME(MgCurvePolygon)

PUBLISHED_API:
    /////////////////////////////////////////////////////////////////
    /// \brief
    /// Gets the exterior ring of the polygon.
    ///
    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual MgCurveRing GetExteriorRing();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual MgCurveRing GetExteriorRing();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual MgCurveRing GetExteriorRing();
    /// \htmlinclude SyntaxBottom.html
    ///
    /// \return
    /// An MgCurveRing that defines the exterior ring of the polygon.
    ///
    virtual MgCurveRing* GetExteriorRing();  /// __get

    ///////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Gets the number of interior rings in this polygon.
    ///
    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual int GetInteriorRingCount();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual int GetInteriorRingCount();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual int GetInteriorRingCount();
    /// \htmlinclude SyntaxBottom.html
    ///
    /// \return
    /// The number of interior rings.
    ///
    virtual INT32 GetInteriorRingCount();  /// __get

    /////////////////////////////////////////////////////////////////
    /// \brief
    /// Gets the interior ring at the specified index. The indexing
    /// starts at zero.
    ///
    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual MgCurveRing GetInteriorRing(int index);
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual MgCurveRing GetInteriorRing(int index);
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual MgCurveRing GetInteriorRing(int index);
    /// \htmlinclude SyntaxBottom.html
    ///
    /// \param index (int)
    /// An integer specifying the location of the
    /// interior ring in the collection.
    ///
    /// \return
    /// The MgCurveRing at the specified index.
    ///
    virtual MgCurveRing* GetInteriorRing(INT32 index);  

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual int GetGeometryType();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual int GetGeometryType();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual int GetGeometryType();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual INT32 GetGeometryType();  /// __get, __inherited

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual int GetDimension();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual int GetDimension();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual int GetDimension();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual INT32 GetDimension();  /// __get, __inherited

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual bool IsEmpty();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual boolean IsEmpty();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual bool IsEmpty();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual bool IsEmpty();

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual bool IsClosed();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual boolean IsClosed();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual bool IsClosed();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual bool IsClosed();

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual MgGeometricEntity Copy();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual MgGeometricEntity Copy();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual MgGeometricEntity Copy();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual MgGeometricEntity* Copy();

    ///////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Returns a transformed copy of this geometric entity.
    ///
    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual MgGeometricEntity Transform(MgTransform transform);
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual MgGeometricEntity Transform(MgTransform transform);
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual MgGeometricEntity Transform(MgTransform transform);
    /// \htmlinclude SyntaxBottom.html
    ///
    /// \param transform (MgTransform)
    /// The MgTransform to use in constructing a copy of this geometric entity.
    ///
    /// \return
    /// An MgGeometricEntity that is a copy of this one with each coordinate
    /// transformed.
    ///
    virtual MgGeometricEntity* Transform(MgTransform* transform);

INTERNAL_API:

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Construct a MgCurvePolygon object
    ///
    MgCurvePolygon(MgCurveRing* outerRing, MgCurveRingCollection* innerRings);

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Construct an empty MgCurvePolygon object for deserialization
    ///
    MgCurvePolygon();

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Serialize data to TCP/IP stream
    ///
    /// \param stream
    /// Stream
    ///
    virtual void Serialize(MgStream* stream);

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Deserialize data from TCP/IP stream
    ///
    /// \param stream
    /// Stream
    ///
    virtual void Deserialize(MgStream* stream);

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Get the unique identifier for the class
    ///
    /// \return
    /// Class Identifider.
    ///
    virtual INT32 GetClassId();

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Convert to AWKT representation
    ///
    virtual void ToAwkt(REFSTRING awktStr, REFSTRING coordDim, bool is2dOnly);

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Convert to AWKT representation
    ///
    virtual STRING ToAwkt(bool is2dOnly);

    /////////////////////////////////////////////////////////////////
    /// \brief
    /// Returns an iterator over the coordinates included in this
    /// geometric entity.
    ///
    /// \return
    /// The coordinates of this geometric entity
    ///
    virtual MgCoordinateIterator* GetCoordinates();

protected:

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Calculate the envelope for this geometry
    ///
    /// \return
    /// Class Identifider.
    ///
    virtual MgEnvelope* ComputeEnvelope();

    //////////////////////////////////////////////
    /// \brief
    /// Dispose this object.
    ///
    virtual void Dispose();

CLASS_ID:
    static const INT32 m_cls_id = Geometry_CurvePolygon;

private:
    Ptr<MgCurveRing>            m_outerRing;
    Ptr<MgCurveRingCollection>  m_innerRings;
};
/// \}

#endif //_MGCURVEPOLYGON_H_
