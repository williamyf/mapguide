/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.39
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.sleepycat.dbxml;

public class XmlIndexLookup {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected XmlIndexLookup(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(XmlIndexLookup obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  public void delete() /* no exception */ {
    if(swigCPtr != 0 && swigCMemOwn) {
	swigCMemOwn = false;
	dbxml_javaJNI.delete_XmlIndexLookup(swigCPtr);
    }
    swigCPtr = 0;
 }

    protected XmlContainer container;
    protected XmlValue lowBoundValue = new XmlValue();
    protected XmlValue highBoundValue = new XmlValue();
	
    public XmlIndexLookup(XmlIndexLookup o) throws XmlException {
	this(dbxml_javaJNI.XmlIndexLookup_copy(XmlIndexLookup.getCPtr(o), o), true);
	lowBoundValue = o.lowBoundValue;
	highBoundValue = o.highBoundValue;
    }
	
    public XmlContainer getContainer() 
	throws XmlException {
	return container;
    }
    public void setContainer(XmlContainer container)
	throws XmlException {
	this.container = container;
	setContainerInternal(container);
    }
    public XmlResults execute(XmlQueryContext context,
			      XmlDocumentConfig config)
	throws XmlException {
	int flags = (config == null) ? 0 : config.makeFlags();
	return execute(context, flags);
    }
    public XmlResults execute(XmlTransaction txn,
			      XmlQueryContext context,
			      XmlDocumentConfig config)
	throws XmlException {
	int flags = (config == null) ? 0 : config.makeFlags();
	return execute(txn, context, flags);
    }
    public XmlValue getLowBoundValue()
	throws XmlException {
	return lowBoundValue;
    }
    public XmlValue getHighBoundValue()
	throws XmlException {
	return highBoundValue;
    }
    public void setHighBound(XmlValue value, int op) throws XmlException {
	setHighBoundInternal(value,op);
	highBoundValue = value;
    }
    public void setLowBound(XmlValue value, int op) throws XmlException {
	setLowBoundInternal(value,op);
	lowBoundValue = value;
    }
    
  public boolean isNull() throws XmlException {
    return dbxml_javaJNI.XmlIndexLookup_isNull(swigCPtr, this);
  }

  public String getIndex() throws XmlException {
    return dbxml_javaJNI.XmlIndexLookup_getIndex(swigCPtr, this);
  }

  public void setIndex(String index) throws XmlException {
    dbxml_javaJNI.XmlIndexLookup_setIndex(swigCPtr, this, index);
  }

  public String getNodeURI() throws XmlException {
    return dbxml_javaJNI.XmlIndexLookup_getNodeURI(swigCPtr, this);
  }

  public String getNodeName() throws XmlException {
    return dbxml_javaJNI.XmlIndexLookup_getNodeName(swigCPtr, this);
  }

  public void setNode(String uri, String name) throws XmlException {
    dbxml_javaJNI.XmlIndexLookup_setNode(swigCPtr, this, uri, name);
  }

  public String getParentURI() throws XmlException {
    return dbxml_javaJNI.XmlIndexLookup_getParentURI(swigCPtr, this);
  }

  public String getParentName() throws XmlException {
    return dbxml_javaJNI.XmlIndexLookup_getParentName(swigCPtr, this);
  }

  public void setParent(String uri, String name) throws XmlException {
    dbxml_javaJNI.XmlIndexLookup_setParent(swigCPtr, this, uri, name);
  }

  public int getLowBoundOperation() throws XmlException {
    return dbxml_javaJNI.XmlIndexLookup_getLowBoundOperation(swigCPtr, this);
  }

  protected void setLowBoundInternal(XmlValue value, int op) {
    dbxml_javaJNI.XmlIndexLookup_setLowBoundInternal(swigCPtr, this, value, op);
  }

  public int getHighBoundOperation() throws XmlException {
    return dbxml_javaJNI.XmlIndexLookup_getHighBoundOperation(swigCPtr, this);
  }

  protected void setHighBoundInternal(XmlValue value, int op) {
    dbxml_javaJNI.XmlIndexLookup_setHighBoundInternal(swigCPtr, this, value, op);
  }

  public void setContainerInternal(XmlContainer container) throws XmlException {
    dbxml_javaJNI.XmlIndexLookup_setContainerInternal(swigCPtr, this, XmlContainer.getCPtr(container), container);
  }

  protected XmlResults execute(XmlQueryContext context, int flags) {
    long cPtr = dbxml_javaJNI.XmlIndexLookup_execute__SWIG_0(swigCPtr, this,  context.getManagerPtr(), context.getDebugListenerCPtr(), context.evaluationType, context.queryInterruptSec, context.pack(), context , flags);
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  public XmlResults execute(XmlQueryContext context) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlIndexLookup_execute__SWIG_1(swigCPtr, this,  context.getManagerPtr(), context.getDebugListenerCPtr(), context.evaluationType, context.queryInterruptSec, context.pack(), context );
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  protected XmlResults execute(XmlTransaction txn, XmlQueryContext context, int flags) {
    long cPtr = dbxml_javaJNI.XmlIndexLookup_execute__SWIG_2(swigCPtr, this, XmlTransaction.getCPtr(txn), txn,  context.getManagerPtr(), context.getDebugListenerCPtr(), context.evaluationType, context.queryInterruptSec, context.pack(), context , flags);
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  public XmlResults execute(XmlTransaction txn, XmlQueryContext context) throws XmlException {
    long cPtr = dbxml_javaJNI.XmlIndexLookup_execute__SWIG_3(swigCPtr, this, XmlTransaction.getCPtr(txn), txn,  context.getManagerPtr(), context.getDebugListenerCPtr(), context.evaluationType, context.queryInterruptSec, context.pack(), context );
    return (cPtr == 0) ? null : new XmlResults(cPtr, true);
  }

  protected static XmlIndexLookup copy(XmlIndexLookup o) {
    long cPtr = dbxml_javaJNI.XmlIndexLookup_copy(XmlIndexLookup.getCPtr(o), o);
    return (cPtr == 0) ? null : new XmlIndexLookup(cPtr, true);
  }

  public final static int NONE = 0;
  public final static int EQ = NONE + 1;
  public final static int GT = EQ + 1;
  public final static int GTE = GT + 1;
  public final static int LT = GTE + 1;
  public final static int LTE = LT + 1;

}
