/******************************************************************************
 *
 * Copyright (C) 1993-2006 by Autodesk, Inc.
 *
 * Permission to use, copy, modify, and distribute this software in
 * object code form for any purpose and without fee is hereby granted, 
 * provided that the above copyright notice appears in all copies and 
 * that both that copyright notice and the limited warranty and
 * restricted rights notice below appear in all supporting 
 * documentation.
 *
 * AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
 * AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
 * DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
 * UNINTERRUPTED OR ERROR FREE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject to 
 * restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable. 
 *
 * DESCRIPTION: iis7_action
 */
#ifndef __IIS7_ACTION_H__
#define __IIS7_ACTION_H__

#include "windows.h"
#include "MSIQuery.h"
#include "tchar.h"

UINT __stdcall CreateAPPMapGuideWithIIS7(MSIHANDLE hMSI);
UINT __stdcall CreateVDirPhpAgentWithIIS7(MSIHANDLE hMSI);
UINT __stdcall CreateAPPNetAgentWithIIS7(MSIHANDLE hMSI);
UINT __stdcall DeleteAPPMapGuideWithIIS7(MSIHANDLE hMSI);

#endif

#define MG_APP_POOL L"MapGuideAppPool"
#define MG_VDIR L"mapguide"
#define MG_WEBSITE L"Default Web Site"

#define MSI_PROP_INSTALLDIR L"WEBEXTENSIONSLOCATION"
#define MG_DEFAULT_WEB_PATH L"C:\\Program Files\\OSGeo\\MapGuide Web\\"