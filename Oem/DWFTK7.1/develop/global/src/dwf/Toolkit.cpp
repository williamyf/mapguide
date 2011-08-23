//
//  Copyright (C) 2000-2011 by Autodesk, Inc.
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


#include "dwf/Toolkit.h"


//
//
// Shared module memory control
//
//
#ifndef DWFTK_STATIC

void* DWFToolkit::_DWFToolkitMemory::operator new( size_t s )
{
    return ::operator new( s );
}

void* DWFToolkit::_DWFToolkitMemory::operator new( size_t s, void* )
{
    return ::operator new( s );
}

void DWFToolkit::_DWFToolkitMemory::operator delete( void* p )
{
    ::operator delete( p );
}

void DWFToolkit::_DWFToolkitMemory::operator delete( void* p, void* )
{
    ::operator delete( p );
}

#endif