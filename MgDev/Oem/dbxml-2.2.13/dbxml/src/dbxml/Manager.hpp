//
// See the file LICENSE for redistribution information.
//
// Copyright (c) 2002-2005
//	Sleepycat Software.  All rights reserved.
//
// $Id: Manager.hpp,v 1.14 2005/08/31 17:14:20 gmf Exp $
//

#ifndef __MANAGER_HPP
#define __MANAGER_HPP

#include <string>
#include <map>

#include <xercesc/util/XMLUri.hpp>

#include "dbxml/XmlDocument.hpp"
#include "ReferenceCounted.hpp"
#include "TransactedContainer.hpp"
#include "Globals.hpp"

class DbEnv;
//class XERCES_CPP_NAMESPACE_QUALIFIER DatatypeValidatorFactory;
class DatatypeValidatorFactory;

namespace DbXml
{

class Container;
class TransactedContainer;
class Transaction;
class ResolverStore;

class Manager : public Log, public ReferenceCountedProtected
{
public:
	/**
	 * It's assumed that the DbEnv is already open. If adopted, the DbEnv
	 * will be both closed and deleted by the XmlDocument when it is
	 * destroyed.
	 */
	Manager(DbEnv *dbEnv, u_int32_t flags = 0);
	Manager(u_int32_t flags);
	~Manager();

	//
	// Resolver methods
	//

	ResolverStore &getResolverStore() {
		return *resolvers_;
	}

	//
	// Container
	//

	void setDefaultContainerFlags(u_int32_t flags) {
		lock(); defaultContainerFlags_ = flags; unlock();
	}
	u_int32_t getDefaultContainerFlags() { return defaultContainerFlags_; }

	void setDefaultPageSize(u_int32_t pageSize) {
		lock(); defaultPageSize_ = pageSize; unlock();
	}
	u_int32_t getDefaultPageSize() { return defaultPageSize_; }

	void setDefaultSequenceIncrement(u_int32_t incr) {
		lock(); defaultSequenceIncr_ = incr; unlock();
	}
	u_int32_t getDefaultSequenceIncrement() { return defaultSequenceIncr_; }

	XmlContainer::ContainerType getDefaultContainerType() {
		return defaultContainerType_;
	}
	void setDefaultContainerType(XmlContainer::ContainerType type) {
		lock(); defaultContainerType_ = type; unlock();
	}

	TransactedContainer *openContainer(const std::string &name,
					   Transaction *txn, u_int32_t flags,
					   XmlContainer::ContainerType type,
					   int mode,
					   bool doVersionCheck = true);
	TransactedContainer *getOpenContainer(const std::string &name);

	void removeContainer(Transaction *txn, const std::string &name);
	void renameContainer(Transaction *txn, const std::string &oldName,
			     const std::string &newName);
	DbEnv *getDbEnv() { return dbEnv_; }

	const std::string &getHome() const { return dbHome_; }

	bool isTransactedEnv() const { return ((dbEnvOpenFlags_ & DB_INIT_TXN) != 0); }
	bool isLockingEnv() const { return ((dbEnvOpenFlags_ & DB_INIT_LOCK) != 0); }

	bool allowAutoOpen() const { return autoOpen_; }

	//
	// Logging
	//
	virtual void log(ImplLogCategory c, ImplLogLevel l,
			 const std::ostringstream &s) const;
	virtual void log(ImplLogCategory c, ImplLogLevel l,
			 const std::string &s) const;
	virtual void checkFlags(const FlagInfo *flag_info,
				const char *function,
				u_int32_t flags, u_int32_t mask) const;

	// Datatype validator factory ("private" / friend to QueryContext)
	XERCES_CPP_NAMESPACE_QUALIFIER DatatypeValidatorFactory
	&getDatatypeValidatorFactory() const;
	//
	// Is the current host big-endian?  DB XML's "native"
	// format is little-endian, as of release 2.0
	//
	static bool isBigendian() { return Globals::isBigendian_; }

private:
	Manager(const Manager &o);
	Manager &operator=(const Manager &o);
	void initialize(DbEnv *env);
	void terminate();

	class ContainerStore {
	public:
		ContainerStore();
		~ContainerStore();

		int closeContainer(TransactedContainer *container,
				   u_int32_t flags);
		TransactedContainer *findContainer(
			Manager &db,
			const std::string &name,
			Transaction *txn,
			u_int32_t flags,
			XmlContainer::ContainerType type,
			u_int32_t pagesize, u_int32_t seqIncr, int mode,
			bool doVersionCheck);

		TransactedContainer *findOpenContainer(
			const std::string &name);

		void releaseRegisteredContainers();
		bool addAlias(const std::string &alias,
			      TransactedContainer *container);
		bool removeAlias(const std::string &alias,
				 TransactedContainer *container);
	private:
		typedef std::map<std::string, TransactedContainer *> Map;
		void *mutex_;
		Map store_;
	};

	DbEnv *dbEnv_;
	bool dbEnvAdopted_;
	u_int32_t dbEnvOpenFlags_;
	bool autoOpen_;
	std::string dbHome_;
	u_int32_t defaultContainerFlags_;
	u_int32_t defaultPageSize_;
	u_int32_t defaultSequenceIncr_;
	XmlContainer::ContainerType defaultContainerType_;
	ContainerStore openContainers_;
	ResolverStore *resolvers_;

	friend class XmlContainer;
	friend class Container;
	friend class TransactedContainer;
};

}

#endif
