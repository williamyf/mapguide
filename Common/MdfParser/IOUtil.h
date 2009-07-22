//
//  Copyright (C) 2004-2009 by Autodesk, Inc.
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

#ifndef _IOUTIL_H
#define _IOUTIL_H

#include "MdfModel.h"
#include "MdfParser.h"
#include <sstream>

using namespace XERCES_CPP_NAMESPACE;
using namespace MDFMODEL_NAMESPACE;

BEGIN_NAMESPACE_MDFPARSER

    // TODO: The streams below should really be changed to std::w* versions to
    // output unicode strings but MapGuide explicitly supports UTF-8 characters in the
    // output for some reason; If we have time, we should come back to this and fix
    // it.  -- Reva
    typedef std::ostream MdfStream;
    typedef std::ostringstream MdfStringStream;

    MDFPARSER_API std::wstring toMdfString(const char* str);
    MDFPARSER_API std::string EncodeString(const std::wstring& str);
    MDFPARSER_API std::string toCString(const std::wstring& str);

    MDFPARSER_API bool wstrToBool(const wchar_t* ch);
    MDFPARSER_API double wstrToDouble(const wchar_t* ch);
    MDFPARSER_API int wstrToInt(const wchar_t* ch);

    MDFPARSER_API bool wstrToBool(const MdfString& str, bool& result);
    MDFPARSER_API bool wstrToDouble(const MdfString& str, double& result);
    MDFPARSER_API bool wstrToInt(const MdfString& str, int& result);

    MDFPARSER_API std::string IntToStr(int i);
    MDFPARSER_API std::string DoubleToStr(double d);
    MDFPARSER_API std::string BoolToStr(bool b);

    MDFPARSER_API std::string tab();

    MDFPARSER_API int gettabcount();
    MDFPARSER_API void inctab();
    MDFPARSER_API void dectab();
    MDFPARSER_API void zerotab();
    MDFPARSER_API void disableTabs();

    MDFPARSER_API std::string startStr(const std::string& elementName);
    MDFPARSER_API std::string endStr(const std::string& elementName);

END_NAMESPACE_MDFPARSER
#endif // _IOUTIL_H
