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

#ifndef MGUNMANAGEDDATAMANAGER_H_
#define MGUNMANAGEDDATAMANAGER_H_

class MG_SERVER_MANAGER_API MgUnmanagedDataManager : public MgGuardDisposable
{
    DECLARE_CLASSNAME(MgUnmanagedDataManager)

/// Constructors/Destructor

public:

    virtual ~MgUnmanagedDataManager();

private:

    MgUnmanagedDataManager();

    // Unimplemented copy constructor and assignment operator.

    MgUnmanagedDataManager(const MgUnmanagedDataManager&);
    MgUnmanagedDataManager& operator=(const MgUnmanagedDataManager&);

    static bool ParsePath(CREFSTRING path, REFSTRING mappingName, REFSTRING subpath);
    void ParseFilter(CREFSTRING filter, MgStringCollection* filters);
    bool FilterFile(CREFSTRING file, const MgStringCollection* filters);

    void GetFilesAndFolders(string& list, CREFSTRING mappingName, CREFSTRING rootdir, CREFSTRING subdir, const MgStringCollection* filters, bool storeFolders, bool storeFiles, bool recursive);

    void AddFolder(string& list, CREFSTRING mappingName, CREFSTRING subdir, CREFSTRING entryName, INT32 numFolders, INT32 numFiles, MgDateTime createdDate, MgDateTime modifiedDate);
    void AddFile(string& list, CREFSTRING mappingName, CREFSTRING subdir, CREFSTRING entryName, INT64 fileSize, MgDateTime createdDate, MgDateTime modifiedDate);

    void GetNumberOfFilesAndSubfolders(CREFSTRING dirpath, INT32& numFolders, INT32& numFiles);

    STRING FormatMappingName(CREFSTRING name);
    STRING FormatSubdir(CREFSTRING subdir);

/// Methods

public:

    static MgUnmanagedDataManager* GetInstance();

    static int SubstituteDataPathAliases(REFSTRING data);
    static int SubstituteDataPathAliases(string& data);

    virtual void Dispose();

    void Initialize();

    void RefreshUnmanagedDataMappings();
    MgPropertyCollection* GetUnmanagedDataMappings();

    MgByteReader* EnumerateUnmanagedData(CREFSTRING path, bool recursive, CREFSTRING type, CREFSTRING filter);

/// Data Members

private:

    static Ptr<MgUnmanagedDataManager> sm_unmanagedDataManager;

    static const STRING SquareBracketBegin;
    static const wchar_t SquareBracketCharBegin;
    static const STRING SquareBracketEnd;
    static const wchar_t SquareBracketCharEnd;

    ACE_Recursive_Thread_Mutex m_mutex;
    Ptr<MgPropertyCollection> m_unmanagedDataMappings;
};

#endif
