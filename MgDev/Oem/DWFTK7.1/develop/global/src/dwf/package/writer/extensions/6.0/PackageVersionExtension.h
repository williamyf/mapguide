//
//  Copyright (c) 1996-2005 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#ifndef _DWFTK_PACKAGE_VERSION_EXTENSION_6_0_H
#define _DWFTK_PACKAGE_VERSION_EXTENSION_6_0_H


///
///\file        dwf/package/writer/extensions/6.0/PackageVersionExtension.h
///\brief       This file contains the DWFPackageVersionExtension class declaration for <b>DWF 6.01 File Format</b>.
///

#ifndef DWFTK_READ_ONLY

#include "dwf/Version.h"
#include "dwf/package/writer/PackageVersionExtension.h"
namespace DWFToolkit
{

///
///\ingroup     dwfpackage
///
///\class       DWFPackageVersion60Extension   dwf/package/writer/PackageVersionExtension.h     "dwf/package/writer/PackageVersionExtension.h"
///\brief       This is the <b>DWF 6.0 File Format</b> plug-in for the DWFPackageWriter.
///\since       7.1.0
///
///             The current DWF Format version is 6.11. This version extension allows
///             DWF 6.0 files to be created. DWF 6.0 files can have ePlot sections only.
///             
///
class DWFPackageVersion60Extension : public DWFPackageVersionExtension
{

public:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    _DWFTK_API
    DWFPackageVersion60Extension()
        throw();

    ///
    ///         Destructor
    /// 
    ///\throw   None
    /// 
    _DWFTK_API
    virtual ~DWFPackageVersion60Extension()
        throw();

    ///
    ///\copydoc DWFPackageVersionExtension::major()
    ///
    _DWFTK_API
    unsigned short major()
        throw()
    {
        return _DWF_FORMAT_VERSION_PACKAGE_MAJOR;
    }

    ///
    ///\copydoc DWFPackageVersionExtension::minor()
    ///
    _DWFTK_API
    unsigned short minor()
        throw()
    {
        return _DWF_FORMAT_VERSION_PACKAGE_MINOR;
    }

    ///
    ///\copydoc DWFPackageVersionExtension::addSection()
    ///
    _DWFTK_API
    virtual bool addSection( DWFSection*   pSection,
                             DWFInterface* pInterface )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageVersionExtension::addGlobalSection()
    ///
    _DWFTK_API
    virtual bool addGlobalSection( DWFGlobalSection* pSection )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageVersionExtension::write()
    ///
    _DWFTK_API
    virtual bool write( const DWFString& zSourceProductVendor,
                        const DWFString& zSourceProductName,
                        const DWFString& zSourceProductVersion,
                        const DWFString& zDWFProductVendor,
                        const DWFString& zDWFProductVersion,
                        DWFZipFileDescriptor::teFileMode eCompressionMode )
        throw( DWFException );


    ///
    ///\copydoc DWFPackageVersionExtension::prewriteManifest()
    ///
    _DWFTK_API
    virtual void prewriteManifest( DWFPackageWriter&            rPackageWriter, 
                                   DWFPackageFileDescriptor&    rPackageDescriptor,
                                   DWFXMLSerializer&            rXMLSerializer,
                                   const DWFString&             rPackagePassword )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageVersionExtension::postwriteManifest()
    ///
    _DWFTK_API
    virtual void postwriteManifest( DWFPackageWriter&            rPackageWriter, 
                                    DWFPackageFileDescriptor&    rPackageDescriptor,
                                    DWFXMLSerializer&            rXMLSerializer,
                                    const DWFString&             rPackagePassword )
        throw( DWFException );

    ///
    ///\copydoc DWFPackageVersionExtension::postwriteSections()
    ///
    _DWFTK_API
    virtual void postwriteSections( DWFPackageWriter&            rPackageWriter, 
                                    DWFPackageFileDescriptor&    rPackageDescriptor,
                                    DWFXMLSerializer&            rXMLSerializer,
                                    const DWFString&             rPackagePassword )
        throw( DWFException );
};

}


#endif
#endif

