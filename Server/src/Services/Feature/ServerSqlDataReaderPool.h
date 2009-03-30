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

#ifndef _MG_SERVER_SQL_DATA_READER_POOL_H_
#define _MG_SERVER_SQL_DATA_READER_POOL_H_

class MgServerSqlProcessor;

class MgServerSqlDataReaderPool;
template class Ptr<MgServerSqlDataReaderPool>;

typedef std::map<STRING, MgServerSqlProcessor*> SqlProcessorCollection;
typedef std::pair<STRING, MgServerSqlProcessor*> SqlProcessorCacheEntry_Pair;

class MgServerSqlDataReaderPool : public MgGuardDisposable
{
    DECLARE_CLASSNAME(MgServerSqlDataReaderPool)

public:
    /// <summary>
    /// Get pointer to a process-wide MgServerSqlDataReaderPool.
    /// </summary>
    static MgServerSqlDataReaderPool* GetInstance();

    STRING Add(MgServerSqlProcessor* processor);

    bool Remove(STRING sqlReader);

    MgServerSqlProcessor* GetProcessor(STRING sqlReader);
    STRING GetReaderId(MgServerSqlProcessor* processor);

protected:

    /// <summary>
    /// Self Destructor
    /// </summary>

    virtual void Dispose();

    /// <summary>
    /// Destructor
    /// </summary>

    virtual ~MgServerSqlDataReaderPool();

    MgServerSqlDataReaderPool();

private:

    /// Pointer to a process-wide singleton.
    static Ptr<MgServerSqlDataReaderPool> m_drPool;

    SqlProcessorCollection m_drCollection;

    ACE_Recursive_Thread_Mutex m_mutex;
};

#endif
