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

#ifndef _MgCoordinateXYM_H_
#define _MgCoordinateXYM_H_

/// \defgroup MgCoordinateXYM MgCoordinateXYM
/// \ingroup Geometry_Module_classes
/// \{

class MgCoordinateXYM;
template class MG_SERVICE_API Ptr<MgCoordinateXYM>;

/////////////////////////////////////////////////////////////////
/// \brief
/// MgCoordinateXYM is a concrete class derived from the abstract
/// class MgCoordinate.
///
/// \remarks
/// It has no public constructor. Instances
/// are created by calling the non-static
/// MgGeometryFactory::CreateCoordinateXY() method.
/// \n
/// The M number signifies a unit of measure. The mapping of the
/// M number to a unit of measure is determined by the user.
///
class MG_SERVICE_API MgCoordinateXYM : public MgCoordinate
{
    DECLARE_CREATE_OBJECT()
    DECLARE_CLASSNAME(MgCoordinateXYM)

PUBLISHED_API:
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
    virtual INT32 GetDimension();

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual double GetX();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual double GetX();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual double GetX();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual double GetX();

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual double GetY();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual double GetY();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual double GetY();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual double GetY();

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual double GetZ();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual double GetZ();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual double GetZ();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual double GetZ();

    /// <!-- Syntax in .Net, Java, and PHP -->
    /// \htmlinclude DotNetSyntaxTop.html
    /// virtual double GetM();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude JavaSyntaxTop.html
    /// virtual double GetM();
    /// \htmlinclude SyntaxBottom.html
    /// \htmlinclude PHPSyntaxTop.html
    /// virtual double GetM();
    /// \htmlinclude SyntaxBottom.html
    ///
    virtual double GetM();

INTERNAL_API:

    ///////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Construct a MgCoordinateXYImpl object
    ///
    MgCoordinateXYM(double x, double y, double m);

    ///////////////////////////////////////////////////////////////////////////
    /// \brief
    /// Construct an empty MgCoordinateXYM object for deserialization
    ///
    MgCoordinateXYM();

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Returns a copy of this object
    ///
    MgCoordinate* Copy();

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
    /// Test 2 coordinates for equality
    ///
    virtual bool Equals(MgCoordinate* coord);

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Convert to XML representation
    ///
    virtual void ToXml(string &str);

    //////////////////////////////////////////////////////////////////
    /// \brief
    /// Convert to AWKT representation
    ///
    virtual void ToAwkt(REFSTRING awktStr, REFSTRING coordDim, bool is2dOnly);

protected:

    //////////////////////////////////////////////
    /// \brief
    /// Dispose this object.
    ///
    virtual void Dispose();

private:
    double      m_x;
    double      m_y;
    double      m_m;

    static const INT32 m_cls_id = Geometry_CoordinateXYM;
};
/// \}

#endif // _MgCoordinateXYM_H_
