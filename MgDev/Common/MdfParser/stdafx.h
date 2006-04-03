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

#ifndef _STDAFX_H
#define _STDAFX_H

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers.

// Windows Header Files:
#include <windows.h>

// for memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#else

#define _ASSERT(x)

#endif //_WIN32

// Xerces includes
#include "xercesc/util/XMLString.hpp"

// Misc Includes
#include <iostream>
#include <fstream>

// MdfModel includes
#include "MdfModel.h"

// MdfParser includes
#include "MdfParser.h"
#include "IOUtil.h"

#endif
