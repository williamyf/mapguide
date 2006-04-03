//
// See the file LICENSE for redistribution information.
//
// Copyright (c) 2002-2005
//	Sleepycat Software.  All rights reserved.
//
// $Id: Cursor.hpp,v 1.41 2005/11/22 16:39:48 jsnelson Exp $
//

#ifndef __CURSOR_HPP
#define	__CURSOR_HPP

#include "DbWrapper.hpp"
#include "ScopedDbt.hpp"

#define DB_CURSOR_CREATE_MASK(flags) ((flags) & ~(DB_RMW))
#define DB_CURSOR_GET_MASK(flags) ((flags) & ~(DB_DEGREE_2))

namespace DbXml
{

typedef enum CursorType { CURSOR_READ, CURSOR_WRITE } CursorType;

/// Provides exception safety for a Berkeley Db Cursor.
class Cursor
{
public:
	Cursor(DbWrapper &db, Transaction *txn, CursorType type,
		u_int32_t flags = 0);
	~Cursor()
	{
		if (dbc_ && (error_ == 0))
			dbc_->close();
	}
	void close()
	{
		if (dbc_ && (error_ == 0)) {
			dbc_->close();
			dbc_ = 0;
		}
	}
	int error() const
	{
		return error_;
	}
	int get(Dbt *key, Dbt *data, u_int32_t flags)
	{
		try {
			return dbc_->get(key, data, flags);
		}
		catch(DbException &de) {
			return de.get_errno();
		}
	}
	int put(Dbt *key, Dbt *data, u_int32_t flags)
	{
		try {
			return dbc_->put(key, data, flags);
		}
		catch(DbException &de) {
			return de.get_errno();
		}
	}
	int del(u_int32_t flags)
	{
		try {
			return dbc_->del(flags);
		}
		catch(DbException &de) {
			return de.get_errno();
		}
	}
private:
	// no need for copy and assignment
	Cursor(const Cursor&);
	Cursor &operator = (const Cursor &);

	Dbc *dbc_;
	int error_;
};

// Wraps a Cursor for a DocumentDatabase
// Virtual behavior is implemented by specific DocumentDatabase impl.
// Construction is via DocumentDatabase::createDocumentCursor()
class DocumentCursor
{
public:
	virtual ~DocumentCursor() {}
	virtual int first(ID &id) = 0; // done when id == 0
	virtual int next(ID &id) = 0; // done when id == 0
};

/// Wraps a smart Cursor, for a Secondary (Index) Database.
class IndexCursor
{
public:
	IndexCursor(IndexDatabase &db, Transaction *txn, bool initBulk = true);
	virtual ~IndexCursor();

	int error() const
	{
		return cursor_.error();
	}

	virtual int first(IndexEntry &ie) = 0;
	virtual int next(IndexEntry &ie) = 0;

protected:
	int nextEntries(u_int32_t flags);
	int prevEntry();
	// find the last matching prefix for specified key
	int findLastPrefix(Dbt &key, IndexEntry *ie);

	Cursor cursor_;
	DbtOut key_;
	Dbt bulk_;
	Dbt tmpKey_;
	Dbt data_;
	bool done_;

private:
	// no need for copy and assignment
	IndexCursor(const IndexCursor&);
	IndexCursor & operator = (const IndexCursor &);
};

class InequalityIndexCursor : public IndexCursor
{
public:
	InequalityIndexCursor(IndexDatabase &db, Transaction *txn,
			      DbWrapper::Operation operation, const Key *k1,
			      const Syntax *syntax);
	InequalityIndexCursor(IndexDatabase &db, Transaction *txn,
			      DbWrapper::Operation gto, const Key *k1,
			      DbWrapper::Operation lto, const Key *k2,
			      const Syntax *syntax);

	int first(IndexEntry &ie);
	int next(IndexEntry &ie);

private:
	const Syntax *syntax_; // The syntax for the cursor
	DbWrapper::Operation operation_; // Operation passed through the ctor
	// Ranges: The upper bound operation. GTX|GTE
	DbWrapper::Operation greaterThanOperation_;
	// Ranges: The lower bound operation. LTX|LTE
	DbWrapper::Operation lessThanOperation_; 
	DbtOut key2_; // Range: The end of the range.

	DbMultipleKeyDataIterator it_;
};

class PrefixIndexCursor : public IndexCursor
{
public:
	PrefixIndexCursor(IndexDatabase &db, Transaction *txn,
			  const Key *k1);

	int first(IndexEntry &ie);
	int next(IndexEntry &ie);

private:
	DbMultipleKeyDataIterator it_;
};

class EqualsIndexCursor : public IndexCursor
{
public:
	EqualsIndexCursor(IndexDatabase &db, Transaction *txn,
			  const Key *k1);

	int first(IndexEntry &ie);
	int next(IndexEntry &ie);

private:
	DbMultipleDataIterator it_;
};

// reverse prefix cursor
class ReversePrefixIndexCursor : public IndexCursor
{
public:
	ReversePrefixIndexCursor(IndexDatabase &db, Transaction *txn,
				 const Key *k1);

	int first(IndexEntry &ie);
	int next(IndexEntry &ie);
};

// reverse sort inequality cursor.
class ReverseInequalityIndexCursor : public IndexCursor
{
public:
	ReverseInequalityIndexCursor(
		IndexDatabase &db, Transaction *txn,
		DbWrapper::Operation operation, const Key *k1,
		const Syntax *syntax);
	ReverseInequalityIndexCursor(
		IndexDatabase &db, Transaction *txn,
		DbWrapper::Operation gto, const Key *k1,
		DbWrapper::Operation lto, const Key *k2,
		const Syntax *syntax);

	int first(IndexEntry &ie);
	int next(IndexEntry &ie);

private:
	int compare(IndexEntry &ie);
	// find the last matching value
	int findLastValue(Dbt &key, bool excl);
	
	const Syntax *syntax_; // The syntax for the cursor
	DbWrapper::Operation operation_; // Operation passed through the ctor
	// Ranges: The upper bound operation. GTX|GTE
	DbWrapper::Operation greaterThanOperation_;
	// Ranges: The lower bound operation. LTX|LTE
	DbWrapper::Operation lessThanOperation_; 
	DbtOut key2_; // Range: The end of the range.
};

}

#endif

