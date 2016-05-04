//
//  Copyright (C) 2004-2011 by Autodesk, Inc.
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//
//  This library is distributed in the hope that it will be useful;
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not; write to the Free Software
//  Foundation; Inc.; 51 Franklin St; Fifth Floor; Boston; MA  02110-1301  USA
//

#ifndef _MGCOORDINATESYSTEMERRORCODE_H_
#define _MGCOORDINATESYSTEMERRORCODE_H_

/// \defgroup MgCoordinateSystemErrorCode MgCoordinateSystemErrorCode
/// \ingroup Coordinate_System_classes
/// \{

///////////////////////////////////////////////////////////////
/// \brief
/// Defines various error codes returned by the Coordinate System API
///
class MG_GEOMETRY_API MgCoordinateSystemErrorCode
{
PUBLISHED_API:
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Y axis azimuth specification is invalid.
    ///
    static const INT32 AZM      = 201;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Great circle azimuth value is invalid.
    ///
    static const INT32 AZMTH    = 202;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid latitude detected in definition.
    ///
    static const INT32 LAT      = 203;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Latitudes of defining points are equal.
    ///
    static const INT32 LATEQU   = 204;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid longitude detected in definition.
    ///
    static const INT32 LNG      = 205;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// First defining point may not be on the equator.
    ///
    static const INT32 LNGEQU   = 206;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid map scale value detected.
    ///
    static const INT32 MAPSCL   = 207;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard meridians are the same.
    ///
    static const INT32 MEREQU   = 208;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Defining great circle is a parallel, try Mercator.
    ///
    static const INT32 MRCAT    = 209;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid coefficient value specified.
    ///
    static const INT32 MSCOEF   = 210;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// System must be referenced to a datum or an ellipsoid.
    ///
    static const INT32 NOREF    = 211;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard paralells are not ordered correctly.
    ///
    static const INT32 NOTNRTH  = 212;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallells are not ordered correctly.
    ///
    static const INT32 NRTHLAT  = 213;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Point defining Y axis direction incorrectly specified.
    ///
    static const INT32 NRTHPNT  = 214;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid latitude value given for projection origin.
    ///
    static const INT32 ORGLAT   = 215;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid longitude value given for projection origin.
    ///
    static const INT32 ORGLNG   = 216;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallel location exceeds 90 degrees.
    ///
    static const INT32 PLL90    = 217;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallel locations are the same.
    ///
    static const INT32 PLLEQU   = 218;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallel locations not inbetween poles.
    ///
    static const INT32 PLLLRG   = 219;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Order of standard parallels incorrect.
    ///
    static const INT32 PLLREV   = 220;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallel location is zero.
    ///
    static const INT32 PLLZERO  = 221;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Poles are too far apart.
    ///
    static const INT32 POLDD    = 222;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Poles are not sufficiently separated.
    ///
    static const INT32 POLDUP   = 223;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Latitude of pole incorrectly specified.
    ///
    static const INT32 POLLAT   = 224;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Longitude of pole incorrectly specified.
    ///
    static const INT32 POLLNG   = 225;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid quad specification detected.
    ///
    static const INT32 QUAD     = 226;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid scale reduction value encountered.
    ///
    static const INT32 SCLRED   = 227;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallels are not ordered correctly.
    ///
    static const INT32 SOTHLAT  = 228;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallel is incorrectly specified.
    ///
    static const INT32 STDLAT   = 229;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard meridian specification is invalid.
    ///
    static const INT32 STDLNG   = 230;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallel specifications are invalid.
    ///
    static const INT32 STDPLL   = 231;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Southern parallel is equal to or north of northern.
    ///
    static const INT32 STDSOU   = 232;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard meridian must be east of the central meridian.
    ///
    static const INT32 STDWEST  = 233;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid unit specification detected.
    ///
    static const INT32 UNIT     = 234;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid projection key name detected.
    ///
    static const INT32 INVPRJ   = 235;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid datum name specification detected.
    ///
    static const INT32 INVDTM   = 236;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid ellipsoid name specification detected.
    ///
    static const INT32 INVELP   = 237;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Longitude range is too small.
    ///
    static const INT32 LLRNG    = 238;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Longitude range values out of order.
    ///
    static const INT32 RNGORD   = 239;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid quadrant specification given.
    ///
    static const INT32 INVQUAD  = 240;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid invalid geographic useful range detected.
    ///
    static const INT32 GEOMM    = 241;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid invalid cartesian useful range detected.
    ///
    static const INT32 CRTMM    = 242;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Standard parallels must be equidistant from poles.
    ///
    static const INT32 PLLED    = 243;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Origin latitude is not polar; use oblique form of projection.
    ///
    static const INT32 PLRLAT   = 244;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Origin latitude is polar; use polar form of projection.
    ///
    static const INT32 USEPLR   = 245;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid value given for UTM zone number.
    ///
    static const INT32 UTMZON   = 246;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid hemisphere specification (+1 = north, -1 = south).
    ///
    static const INT32 HMISPHR  = 247;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Central azimuth is 90, use the Swiss Oblique Mercator.
    ///
    static const INT32 USESW    = 248;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Eastern meridian is more than 15 degrees from the central meridian.
    ///
    static const INT32 MAX15    = 249;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid oblique pole specification.
    ///
    static const INT32 OBLQPOLE = 250;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Denominator of affine is zero.
    ///
    static const INT32 AFFZERO  = 251;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Non-earth system referenced to a datum.
    ///
    static const INT32 NRDATUM  = 252;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// OSTN97.TXT grid shift file could not be located or opened with read access.
    ///
    static const INT32 OSTN97   = 253;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Invalid region code specified for Danish System 34/45 (1=J, 2=S, 3=B).
    ///
    static const INT32 DENRGN   = 253;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Value supplied for elevated ellipsoid is outside acceptable range
    ///
    static const INT32 ELEVEL   = 254;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// OSTN02.TXT grid shift file could not be located or opened with read access.
    ///
    static const INT32 OSTN02   = 255;    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Transverse Mercator Kruger formulation requested with non-zero origin latitude.
    ///
    static const INT32 TMKRG0   = 256;    

    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// The operation succeeded
    ///
    static const INT32 Ok                   = 1000;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// The operation ran out of memory
    ///
    static const INT32 OutOfMemory          = 1001;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// An initialization operation failed 
    ///
    static const INT32 InitializationFailed = 1002;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// A conversion operation failed
    ///
    static const INT32 ConversionFailed     = 1003;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// An argument supplied to an operation was null
    ///
    static const INT32 NullArgument         = 1004;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief
    /// An argument supplied to an operation was invalid
    ///
    static const INT32 InvalidArgument      = 1005;
};
/// \}

#endif //_MGCOORDINATESYSTEMERRORCODE_H_
