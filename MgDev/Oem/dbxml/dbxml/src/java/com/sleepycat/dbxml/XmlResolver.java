/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.39
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sleepycat.dbxml;

public class XmlResolver {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected XmlResolver(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(XmlResolver obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  public void delete() /* no exception */ {
    if(swigCPtr != 0 && swigCMemOwn) {
	swigCMemOwn = false;
	dbxml_javaJNI.delete_XmlResolver(swigCPtr);
    }
    swigCPtr = 0;
 }

  protected void swigDirectorDisconnect() {
    swigCMemOwn = false;
    delete();
  }

  public void swigReleaseOwnership() throws XmlException {
    swigCMemOwn = false;
    dbxml_javaJNI.XmlResolver_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() throws XmlException {
    swigCMemOwn = true;
    dbxml_javaJNI.XmlResolver_change_ownership(this, swigCPtr, true);
  }

  protected XmlResolver() {
    this(dbxml_javaJNI.new_XmlResolver(), true);
    dbxml_javaJNI.XmlResolver_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public boolean resolveDocument(XmlTransaction txn, XmlManager mgr, String uri, XmlValue reslt) throws XmlException {
    return (getClass() == XmlResolver.class) ? dbxml_javaJNI.XmlResolver_resolveDocument(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, uri, reslt) : dbxml_javaJNI.XmlResolver_resolveDocumentSwigExplicitXmlResolver(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, uri, reslt);
  }

  public boolean resolveCollection(XmlTransaction txn, XmlManager mgr, String uri, XmlResults reslt) throws XmlException {
    return (getClass() == XmlResolver.class) ? dbxml_javaJNI.XmlResolver_resolveCollection(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, uri, XmlResults.getCPtr(reslt), reslt) : dbxml_javaJNI.XmlResolver_resolveCollectionSwigExplicitXmlResolver(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, uri, XmlResults.getCPtr(reslt), reslt);
  }

  public XmlInputStream resolveSchema(XmlTransaction txn, XmlManager mgr, String schemaLocation, String nameSpace) throws XmlException {
    long cPtr = (getClass() == XmlResolver.class) ? dbxml_javaJNI.XmlResolver_resolveSchema(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, schemaLocation, nameSpace) : dbxml_javaJNI.XmlResolver_resolveSchemaSwigExplicitXmlResolver(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, schemaLocation, nameSpace);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, true);
  }

  public XmlInputStream resolveEntity(XmlTransaction txn, XmlManager mgr, String systemId, String publicId) throws XmlException {
    long cPtr = (getClass() == XmlResolver.class) ? dbxml_javaJNI.XmlResolver_resolveEntity(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, systemId, publicId) : dbxml_javaJNI.XmlResolver_resolveEntitySwigExplicitXmlResolver(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, systemId, publicId);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, true);
  }

  public boolean resolveModuleLocation(XmlTransaction txn, XmlManager mgr, String nameSpace, XmlResults reslt) throws XmlException {
    return (getClass() == XmlResolver.class) ? dbxml_javaJNI.XmlResolver_resolveModuleLocation(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, nameSpace, XmlResults.getCPtr(reslt), reslt) : dbxml_javaJNI.XmlResolver_resolveModuleLocationSwigExplicitXmlResolver(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, nameSpace, XmlResults.getCPtr(reslt), reslt);
  }

  public XmlInputStream resolveModule(XmlTransaction txn, XmlManager mgr, String moduleLocation, String nameSpace) throws XmlException {
    long cPtr = (getClass() == XmlResolver.class) ? dbxml_javaJNI.XmlResolver_resolveModule(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, moduleLocation, nameSpace) : dbxml_javaJNI.XmlResolver_resolveModuleSwigExplicitXmlResolver(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, moduleLocation, nameSpace);
    return (cPtr == 0) ? null : new XmlInputStream(cPtr, true);
  }

  public XmlExternalFunctionBase resolveExternalFunction(XmlTransaction txn, XmlManager mgr, String uri, String name, int numberOfArgs) throws XmlException {
    long cPtr = (getClass() == XmlResolver.class) ? dbxml_javaJNI.XmlResolver_resolveExternalFunction(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, uri, name, numberOfArgs) : dbxml_javaJNI.XmlResolver_resolveExternalFunctionSwigExplicitXmlResolver(swigCPtr, this, XmlTransaction.getCPtr(txn), txn, XmlManager.getCPtr(mgr), mgr, uri, name, numberOfArgs);
    return (cPtr == 0) ? null : new XmlExternalFunctionBase(cPtr, true);
  }

}
