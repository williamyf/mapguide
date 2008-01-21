/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.29
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sleepycat.dbxml;

import com.sleepycat.db.*;
import com.sleepycat.db.internal.DbEnv;
import com.sleepycat.db.internal.DbConstants;

public class XmlManager {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected XmlManager(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(XmlManager obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public void delete() /* no exception */ {
    closeInternal();
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      dbxml_javaJNI.delete_XmlManager(swigCPtr);
    }
    swigCPtr = 0;
}


    private Environment dbenv;
    private XmlContainerConfig defaultConfig;
    private boolean threaded = true; // default on if no Environment
    private boolean adopted = false;
    private XmlManagerConfig config = null;

    public XmlManager(final Environment dbenv,
		      XmlManagerConfig config)
	throws XmlException {
	this(dbxml_javaJNI.new_XmlManager(XmlHelper.getDbEnv(dbenv),
					  ((config == null ) ? 0 :
					   config.makeFlags())),true);
	this.dbenv = dbenv;
	this.config = config;
	if ((config != null) && (config.getAdoptEnvironment()))
	    this.adopted = true;
	try {
	    threaded =
		((XmlHelper.getDbEnv(dbenv).get_open_flags() &
		  DbConstants.DB_THREAD) != 0);
	} catch (DatabaseException de) {
	    throw new XmlException(XmlException.DATABASE_ERROR,
				   de.toString(), de,
				   de.getErrno(), 0, 0);
	}
    }

    public XmlManager(XmlManagerConfig config)
	throws XmlException, java.io.FileNotFoundException {
	DbEnv newEnv = null;
	try {
	    newEnv = new DbEnv(0);
	    newEnv.set_cachesize(64 * 1024 * 1024, 1); // 64MB cache
	    newEnv.set_errpfx("BDB XML");
	    newEnv.open(null, DbConstants.DB_PRIVATE|
			DbConstants.DB_CREATE|DbConstants.DB_INIT_MPOOL|
			DbConstants.DB_THREAD, 0);
	    this.dbenv = XmlHelper.makeNewEnvironment(newEnv);
	} catch (DatabaseException de) {
	    throw new XmlException(XmlException.DATABASE_ERROR,
				   de.toString(), de,
				   de.getErrno(), 0, 0);
	}
	this.adopted = true;
	this.config = config;
	// do what the SWIG-generated ctor does, in order to
	// create a C++ XmlManager object.  Add ADOPT flag,
	// since the DbEnv was internally constructed.
	int flags = (config == null) ? 0 : config.makeFlags();
	swigCPtr = dbxml_javaJNI.new_XmlManager(newEnv, flags);
	swigCMemOwn = true;
    }

    public XmlManager()
	throws XmlException, java.io.FileNotFoundException {
	this(null);
    }

    public Environment getEnvironment() throws XmlException {
	return dbenv;
    }

    public XmlManagerConfig getManagerConfig() throws XmlException {
	return config;
    }

    public void setDefaultContainerConfig(XmlContainerConfig config) throws XmlException {
	this.defaultConfig = config;
    }

    public XmlContainerConfig getDefaultContainerConfig() throws XmlException {
	return defaultConfig;
    }

	public XmlInputStream createInputStream(java.io.InputStream is) throws XmlException {
		return new NativeXmlInputStream(is);
	}

    public XmlContainer createContainer(String name)
	throws XmlException {
	return createContainer(name, defaultConfig);
    }

    public XmlContainer createContainer(XmlTransaction txn, String name)
	throws XmlException {
	return createContainer(txn, name, defaultConfig);
    }

    public XmlContainer openContainer(String name)
	throws XmlException {
	return openContainer(name, defaultConfig);
    }

    public XmlContainer openContainer(XmlTransaction txn, String name)
	throws XmlException {
	return openContainer(txn, name, defaultConfig);
    }

    public XmlContainer createContainer(String name,
					XmlContainerConfig config)
	throws XmlException {
	int containerType;
	int flags = 0;
	if (config != null) {
	    flags = config.makeFlags(threaded);
	    containerType = (config.getNodeContainer()) ?
		XmlContainer.NodeContainer : XmlContainer.WholedocContainer;
	} else {
	    containerType = getDefaultContainerType();
	}
	XmlContainer cont = createContainer(name, flags, containerType, 0);
	cont.setContainerConfig(config);
	cont.setManager(this);
	return cont;
    }

    public XmlContainer createContainer(XmlTransaction txn,
					String name,
					XmlContainerConfig config)
	throws XmlException {
	int flags = 0;
	int containerType;
	if (config != null) {
	    flags = config.makeFlags(threaded);
	    containerType = (config.getNodeContainer()) ?
		XmlContainer.NodeContainer : XmlContainer.WholedocContainer;
	} else {
	    containerType = getDefaultContainerType();
	}
	XmlContainer cont = createContainer(txn, name, flags, containerType, 0);
	cont.setContainerConfig(config);
	cont.setManager(this);
	return cont;
    }

    public XmlContainer openContainer(String name,
				      XmlContainerConfig config)
	throws XmlException {
	int flags = 0;
	int containerType;
	if (config != null) {
	    flags = config.makeFlags(threaded);
	    containerType = (config.getNodeContainer()) ?
		XmlContainer.NodeContainer : XmlContainer.WholedocContainer;
	} else {
	    containerType = getDefaultContainerType();
	}

	XmlContainer cont = openContainer(name, flags, containerType, 0);
	cont.setContainerConfig(config);
	cont.setManager(this);
	return cont;
    }

    public XmlContainer openContainer(XmlTransaction txn,
				      String name,
				      XmlContainerConfig config)
	throws XmlException {
	int flags = 0;
	int containerType;
	if (config != null) {
	    flags = config.makeFlags(threaded);
	    containerType = (config.getNodeContainer()) ?
		XmlContainer.NodeContainer : XmlContainer.WholedocContainer;
	} else {
	    containerType = getDefaultContainerType();
	}

	XmlContainer cont = openContainer(txn, name, flags, containerType, 0);
	cont.setContainerConfig(config);
	cont.setManager(this);
	return cont;
    }

    public XmlTransaction createTransaction(com.sleepycat.db.Transaction toUse)
	throws XmlException {
	XmlTransaction txn = createTransaction(XmlHelper.getDbTxn(toUse));
	txn.setTransaction(toUse);
	return txn;
    }

    public XmlTransaction createTransaction(final Transaction parent,
					    TransactionConfig config)
	throws XmlException {
	Transaction newTxn = null;
	try {
	    newTxn = dbenv.beginTransaction(parent, config);
	} catch (DatabaseException de) {
	    throw new XmlException(XmlException.DATABASE_ERROR,
				   de.toString(), de,
				   de.getErrno(), 0, 0);
	}
	return createTransaction(newTxn);
    }

    public XmlTransaction createTransaction()
	throws XmlException {
	return createTransaction(null, null);
    }

    public XmlResults query(String query,
			    XmlQueryContext context,
			    XmlDocumentConfig config)
	throws XmlException {
	int flags = (config == null) ? 0 : config.makeFlags();
	return query(query, context, flags);
    }

    public XmlResults query(XmlTransaction txn,
			    String query,
			    XmlQueryContext context,
			    XmlDocumentConfig config)
	throws XmlException {
	int flags = (config == null) ? 0 : config.makeFlags();
	return query(txn, query, context, flags);
    }

    public void reindexContainer(String name,
				 XmlUpdateContext context,
				 XmlContainerConfig config)
	throws XmlException {
	int flags = 0;
	if (config != null)
	    flags = config.makeFlags(threaded);
	reindexContainer(name, context, flags);
    }

    public void reindexContainer(XmlTransaction txn,
				 String name,
				 XmlUpdateContext context,
				 XmlContainerConfig config)
	throws XmlException {
	int flags = 0;
	if (config != null)
	    flags = config.makeFlags(threaded);
	reindexContainer(txn, name, context, flags);
    }

    public void verifyContainer(String name,
				String filename,
				VerifyConfig config)
	throws XmlException {
	int flags = 0;
	if (config.getAggressive())
	    flags |= DbConstants.DB_AGGRESSIVE;
	if (config.getSalvage())
	    flags |= DbConstants.DB_SALVAGE;
	if (config.getPrintable())
	    flags |= DbConstants.DB_PRINTABLE;
	if (config.getOrderCheckOnly())
	    flags |= DbConstants.DB_ORDERCHKONLY;
	if (config.getNoOrderCheck())
	    flags |= DbConstants.DB_NOORDERCHK;
	verifyContainer(name, filename, flags);
    }

    protected void closeInternal() {
	if (adopted) {
	    try {
		dbenv.close();
	    } catch (DatabaseException de) {}
	}
	adopted = false;
	this.dbenv = null;
	this.config = null;
    }

    public void close() throws XmlException {
	delete();
    }

    public final static int LEVEL_NONE = dbxml_java.LEVEL_NONE;
    public final static int LEVEL_DEBUG = dbxml_java.LEVEL_DEBUG;
    public final static int LEVEL_INFO = dbxml_java.LEVEL_INFO;
    public final static int LEVEL_WARNING = dbxml_java.LEVEL_WARNING;
    public final static int LEVEL_ERROR = dbxml_java.LEVEL_ERROR;
    public final static int LEVEL_ALL = dbxml_java.LEVEL_ALL;
    public final static int CATEGORY_NONE = dbxml_java.CATEGORY_NONE;
    public final static int CATEGORY_INDEXER = dbxml_java.CATEGORY_INDEXER;
    public final static int CATEGORY_QUERY = dbxml_java.CATEGORY_QUERY;
    public final static int CATEGORY_OPTIMIZER = dbxml_java.CATEGORY_OPTIMIZER;
    public final static int CATEGORY_DICTIONARY = dbxml_java.CATEGORY_DICTIONARY;
    public final static int CATEGORY_CONTAINER = dbxml_java.CATEGORY_CONTAINER;
    public final static int CATEGORY_NODESTORE = dbxml_java.CATEGORY_NODESTORE;
    public final static int CATEGORY_MANAGER = dbxml_java.CATEGORY_MANAGER;
    public final static int CATEGORY_ALL = dbxml_java.CATEGORY_ALL;
    public final static String metaDataNamespace_uri = dbxml_javaConstants.metaDataNamespace_uri;
    public final static String metaDataNamespace_prefix = dbxml_javaConstants.metaDataNamespace_prefix;
    public final static String metaDataName_name = dbxml_javaConstants.metaDataName_name;
    public final static String metaDataName_root = dbxml_javaConstants.metaDataName_root;
    
  public XmlManager(com.sleepycat.db.internal.DbEnv dbEnv, int flags) throws XmlException {
    this(dbxml_javaJNI.new_XmlManager(dbEnv, flags), true);
  }

  public void setDefaultPageSize(int pageSize) throws XmlException {
    dbxml_javaJNI.XmlManager_setDefaultPageSize(swigCPtr, pageSize);
  }

  public int getDefaultPageSize() throws XmlException {
    return dbxml_javaJNI.XmlManager_getDefaultPageSize(swigCPtr);
  }

  public void setDefaultSequenceIncrement(int incr) throws XmlException {
    dbxml_javaJNI.XmlManager_setDefaultSequenceIncrement(swigCPtr, incr);
  }

  public int getDefaultSequenceIncrement() throws XmlException {
    return dbxml_javaJNI.XmlManager_getDefaultSequenceIncrement(swigCPtr);
  }

  public void setDefaultContainerType(int type) throws XmlException {
    dbxml_javaJNI.XmlManager_setDefaultContainerType(swigCPtr, type);
  }

  public int getDefaultContainerType() throws XmlException {
    return dbxml_javaJNI.XmlManager_getDefaultContainerType(swigCPtr);
  }

  public String getHome() throws XmlException {
    return dbxml_javaJNI.XmlManager_getHome(swigCPtr);
  }

  public void registerResolver(XmlResolver resolver) throws XmlException {
    dbxml_javaJNI.XmlManager_registerResolver(swigCPtr, XmlResolver.getCPtr(resolver));
  }

  public int getImplicitTimezone() throws XmlException {
    return dbxml_javaJNI.XmlManager_getImplicitTimezone(swigCPtr);
  }

  public void setImplicitTimezone(int tz) throws XmlException {
    dbxml_javaJNI.XmlManager_setImplicitTimezone(swigCPtr, tz);
  }

  public int existsContainer(String name) throws XmlException {
    return dbxml_javaJNI.XmlManager_existsContainer(swigCPtr, name);
  }

  public void removeContainer(String name) throws XmlException {
    dbxml_javaJNI.XmlManager_removeContainer__SWIG_0(swigCPtr, name);
  }

  public void removeContainer(XmlTransaction txn, String name) throws XmlException {
    dbxml_javaJNI.XmlManager_removeContainer__SWIG_1(swigCPtr, XmlTransaction.getCPtr(txn), name);
  }

  public void renameContainer(String oldName, String newName) throws XmlException {
    dbxml_javaJNI.XmlManager_renameContainer__SWIG_0(swigCPtr, oldName, newName);
  }

  public void renameContainer(XmlTransaction txn, String oldName, String newName) throws XmlException {
    dbxml_javaJNI.XmlManager_renameContainer__SWIG_1(swigCPtr, XmlTransaction.getCPtr(txn), oldName, newName);
  }

  public void upgradeContainer(String name, XmlUpdateContext uc) throws XmlException {
    dbxml_javaJNI.XmlManager_upgradeContainer(swigCPtr, name, XmlUpdateContext.getCPtr(uc));
  }

  public void compactContainer(String name, XmlUpdateContext uc, int flags) throws XmlException {
    dbxml_javaJNI.XmlManager_compactContainer__SWIG_0(swigCPtr, name, XmlUpdateContext.getCPtr(uc), flags);
  }

  public void compactContainer(String name, XmlUpdateContext uc) throws XmlException {
    dbxml_javaJNI.XmlManager_compactContainer__SWIG_1(swigCPtr, name, XmlUpdateContext.getCPtr(uc));
  }

  public void compactContainer(XmlTransaction txn, String name, XmlUpdateContext uc, int flags) throws XmlException {
    dbxml_javaJNI.XmlManager_compactContainer__SWIG_2(swigCPtr, XmlTransaction.getCPtr(txn), name, XmlUpdateContext.getCPtr(uc), flags);
  }

  public void compactContainer(XmlTransaction txn, String name, XmlUpdateContext uc) throws XmlException {
    dbxml_javaJNI.XmlManager_compactContainer__SWIG_3(swigCPtr, XmlTransaction.getCPtr(txn), name, XmlUpdateContext.getCPtr(uc));
  }

  public void truncateContainer(String name, XmlUpdateContext uc, int flags) throws XmlException {
    dbxml_javaJNI.XmlManager_truncateContainer__SWIG_0(swigCPtr, name, XmlUpdateContext.getCPtr(uc), flags);
  }

  public void truncateContainer(String name, XmlUpdateContext uc) throws XmlException {
    dbxml_javaJNI.XmlManager_truncateContainer__SWIG_1(swigCPtr, name, XmlUpdateContext.getCPtr(uc));
  }

  public void truncateContainer(XmlTransaction txn, String name, XmlUpdateContext uc, int flags) throws XmlException {
    dbxml_javaJNI.XmlManager_truncateContainer__SWIG_2(swigCPtr, XmlTransaction.getCPtr(txn), name, XmlUpdateContext.getCPtr(uc), flags);
  }

  public void truncateContainer(XmlTransaction txn, String name, XmlUpdateContext uc) throws XmlException {
    dbxml_javaJNI.XmlManager_truncateContainer__SWIG_3(swigCPtr, XmlTransaction.getCPtr(txn), name, XmlUpdateContext.getCPtr(uc));
  }

  protected void reindexContainer(String name, XmlUpdateContext uc, int flags) {
    dbxml_javaJNI.XmlManager_reindexContainer__SWIG_0(swigCPtr, name, XmlUpdateContext.getCPtr(uc), flags);
  }

  protected void reindexContainer(String name, XmlUpdateContext uc) {
    dbxml_javaJNI.XmlManager_reindexContainer__SWIG_1(swigCPtr, name, XmlUpdateContext.getCPtr(uc));
  }

  protected void reindexContainer(XmlTransaction txn, String name, XmlUpdateContext uc, int flags) {
    dbxml_javaJNI.XmlManager_reindexContainer__SWIG_2(swigCPtr, XmlTransaction.getCPtr(txn), name, XmlUpdateContext.getCPtr(uc), flags);
  }

  protected void reindexContainer(XmlTransaction txn, String name, XmlUpdateContext uc) {
    dbxml_javaJNI.XmlManager_reindexContainer__SWIG_3(swigCPtr, XmlTransaction.getCPtr(txn), name, XmlUpdateContext.getCPtr(uc));
  }

  public XmlInputStream createStdInInputStream() throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createStdInInputStream(swigCPtr);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, false);
  }

  public XmlInputStream createLocalFileInputStream(String filename) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createLocalFileInputStream(swigCPtr, filename);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, false);
  }

  public XmlInputStream createMemBufInputStream(String bytes, long count, String id, boolean adopt) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createMemBufInputStream__SWIG_0(swigCPtr, bytes, count, id, adopt);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, false);
  }

  public XmlInputStream createMemBufInputStream(String bytes, long count, String id) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createMemBufInputStream__SWIG_1(swigCPtr, bytes, count, id);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, false);
  }

  public XmlInputStream createMemBufInputStream(String bytes, long count, boolean copyBuffer) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createMemBufInputStream__SWIG_2(swigCPtr, bytes, count, copyBuffer);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, false);
  }

  public XmlInputStream createURLInputStream(String baseId, String systemId, String publicId) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createURLInputStream__SWIG_0(swigCPtr, baseId, systemId, publicId);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, false);
  }

  public XmlInputStream createURLInputStream(String baseId, String systemId) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createURLInputStream__SWIG_1(swigCPtr, baseId, systemId);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, false);
  }

  protected XmlContainer createContainer(String name, int flags, int type, int mode) {
    long cPtr = dbxml_javaJNI.XmlManager_createContainer__SWIG_0(swigCPtr, name, flags, type, mode);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer createContainer(String name, int flags, int type) {
    long cPtr = dbxml_javaJNI.XmlManager_createContainer__SWIG_1(swigCPtr, name, flags, type);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer createContainer(String name, int flags) {
    long cPtr = dbxml_javaJNI.XmlManager_createContainer__SWIG_2(swigCPtr, name, flags);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer createContainer(XmlTransaction txn, String name, int flags, int type, int mode) {
    long cPtr = dbxml_javaJNI.XmlManager_createContainer__SWIG_3(swigCPtr, XmlTransaction.getCPtr(txn), name, flags, type, mode);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer createContainer(XmlTransaction txn, String name, int flags, int type) {
    long cPtr = dbxml_javaJNI.XmlManager_createContainer__SWIG_4(swigCPtr, XmlTransaction.getCPtr(txn), name, flags, type);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer createContainer(XmlTransaction txn, String name, int flags) {
    long cPtr = dbxml_javaJNI.XmlManager_createContainer__SWIG_5(swigCPtr, XmlTransaction.getCPtr(txn), name, flags);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer openContainer(String name, int flags) {
    long cPtr = dbxml_javaJNI.XmlManager_openContainer__SWIG_0(swigCPtr, name, flags);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer openContainer(XmlTransaction txn, String name, int flags) {
    long cPtr = dbxml_javaJNI.XmlManager_openContainer__SWIG_1(swigCPtr, XmlTransaction.getCPtr(txn), name, flags);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer openContainer(String name, int flags, int type, int mode) {
    long cPtr = dbxml_javaJNI.XmlManager_openContainer__SWIG_2(swigCPtr, name, flags, type, mode);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer openContainer(String name, int flags, int type) {
    long cPtr = dbxml_javaJNI.XmlManager_openContainer__SWIG_3(swigCPtr, name, flags, type);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer openContainer(XmlTransaction txn, String name, int flags, int type, int mode) {
    long cPtr = dbxml_javaJNI.XmlManager_openContainer__SWIG_4(swigCPtr, XmlTransaction.getCPtr(txn), name, flags, type, mode);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  protected XmlContainer openContainer(XmlTransaction txn, String name, int flags, int type) {
    long cPtr = dbxml_javaJNI.XmlManager_openContainer__SWIG_5(swigCPtr, XmlTransaction.getCPtr(txn), name, flags, type);
    return (cPtr == 0) ? null : new XmlContainer(cPtr, true);
  }

  public void dumpContainer(String name, String filename) throws XmlException {
    dbxml_javaJNI.XmlManager_dumpContainer(swigCPtr, name, filename);
  }

  public void loadContainer(String name, String filename, XmlUpdateContext uc) throws XmlException {
    dbxml_javaJNI.XmlManager_loadContainer(swigCPtr, name, filename, XmlUpdateContext.getCPtr(uc));
  }

  protected void verifyContainer(String name, String filename, int flags) {
    dbxml_javaJNI.XmlManager_verifyContainer__SWIG_0(swigCPtr, name, filename, flags);
  }

  public void verifyContainer(String name, String filename) throws XmlException {
    dbxml_javaJNI.XmlManager_verifyContainer__SWIG_1(swigCPtr, name, filename);
  }

  public XmlDocument createDocument() throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createDocument(swigCPtr);
    return (cPtr == 0) ? null : new XmlDocument(cPtr, true);
  }

  public XmlQueryContext createQueryContext(int rt, int et) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createQueryContext__SWIG_0(swigCPtr, rt, et);
    return (cPtr == 0) ? null : new XmlQueryContext(cPtr, true);
  }

  public XmlQueryContext createQueryContext() throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createQueryContext__SWIG_1(swigCPtr);
    return (cPtr == 0) ? null : new XmlQueryContext(cPtr, true);
  }

  public XmlQueryContext createQueryContext(int rt) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createQueryContext__SWIG_2(swigCPtr, rt);
    return (cPtr == 0) ? null : new XmlQueryContext(cPtr, true);
  }

  public XmlUpdateContext createUpdateContext() throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createUpdateContext(swigCPtr);
    return (cPtr == 0) ? null : new XmlUpdateContext(cPtr, true);
  }

  public XmlQueryExpression prepare(String query, XmlQueryContext context) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_prepare__SWIG_0(swigCPtr, query, XmlQueryContext.getCPtr(context));
    return (cPtr == 0) ? null : new XmlQueryExpression(cPtr, true);
  }

  public XmlQueryExpression prepare(XmlTransaction txn, String query, XmlQueryContext context) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_prepare__SWIG_1(swigCPtr, XmlTransaction.getCPtr(txn), query, XmlQueryContext.getCPtr(context));
    return (cPtr == 0) ? null : new XmlQueryExpression(cPtr, true);
  }

  protected XmlResults query(String query, XmlQueryContext context, int flags) {
    long cPtr = dbxml_javaJNI.XmlManager_query__SWIG_0(swigCPtr, query, XmlQueryContext.getCPtr(context), flags);
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  protected XmlResults query(XmlTransaction txn, String query, XmlQueryContext context, int flags) {
    long cPtr = dbxml_javaJNI.XmlManager_query__SWIG_1(swigCPtr, XmlTransaction.getCPtr(txn), query, XmlQueryContext.getCPtr(context), flags);
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  public XmlResults query(String query, XmlQueryContext context) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_query__SWIG_2(swigCPtr, query, XmlQueryContext.getCPtr(context));
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  public XmlResults query(XmlTransaction txn, String query, XmlQueryContext context) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_query__SWIG_3(swigCPtr, XmlTransaction.getCPtr(txn), query, XmlQueryContext.getCPtr(context));
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  public XmlResults createResults() throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createResults(swigCPtr);
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  public XmlModify createModify() throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createModify(swigCPtr);
    return (cPtr == 0) ? null : new XmlModify(cPtr, true);
  }

  public XmlTransaction createTransaction(com.sleepycat.db.internal.DbTxn toAdopt) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createTransaction(swigCPtr, toAdopt);
    return (cPtr == 0) ? null : new XmlTransaction(cPtr, true);
  }

  public XmlIndexLookup createIndexLookup(XmlContainer cont, String uri, String name, String index, XmlValue value, int op) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createIndexLookup__SWIG_0(swigCPtr, XmlContainer.getCPtr(cont), uri, name, index, XmlValue.getCPtr(value), op);
    return (cPtr == 0) ? null : new XmlIndexLookup(cPtr, true);
  }

  public XmlIndexLookup createIndexLookup(XmlContainer cont, String uri, String name, String index, XmlValue value) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createIndexLookup__SWIG_1(swigCPtr, XmlContainer.getCPtr(cont), uri, name, index, XmlValue.getCPtr(value));
    return (cPtr == 0) ? null : new XmlIndexLookup(cPtr, true);
  }

  public XmlIndexLookup createIndexLookup(XmlContainer cont, String uri, String name, String index) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlManager_createIndexLookup__SWIG_2(swigCPtr, XmlContainer.getCPtr(cont), uri, name, index);
    return (cPtr == 0) ? null : new XmlIndexLookup(cPtr, true);
  }

  public static void setLogLevel(int level, boolean enabled) throws XmlException {
    dbxml_javaJNI.XmlManager_setLogLevel(level, enabled);
  }

  public static void setLogCategory(int category, boolean enabled) throws XmlException {
    dbxml_javaJNI.XmlManager_setLogCategory(category, enabled);
  }

  public static int get_version_major() {
    return dbxml_javaJNI.XmlManager_get_version_major();
  }

  public static int get_version_minor() {
    return dbxml_javaJNI.XmlManager_get_version_minor();
  }

  public static int get_version_patch() {
    return dbxml_javaJNI.XmlManager_get_version_patch();
  }

  public static String get_version_string() {
    return dbxml_javaJNI.XmlManager_get_version_string();
  }

}
