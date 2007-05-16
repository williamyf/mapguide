/**
\addtogroup ProviderCapabilities
<h2>Provider Capabilities</h2>
<p>
  The MgFeatureService API delegates much of its functionality
  to FDO providers. The providers implement the storage and
  retrieval of feature data from a variety of RDBMS and
  file-based datastore technologies.
</p>
<p>
  A set of provider capabilities has been defined, and each
  provider has been characterized according to what
  capabilities it supports. For example, the FDO Provider for
  Oracle supports the creation, description, and destruction of
  a schema definition, but the FDO Provider for ArcSDE supports
  only the description of a schema definition.
</p>
<p>
  The capabilities are grouped in the following categories:
  \li  Connection
  \li  Schema
  \li  Commands
  \li  Expressions
  \li  Filters
  \li  Raster
  \li  Topology
  The MgFeatureService::GetCapabilities method returns an XML
  representation of a provider's capabilities. See \link FdoProviderCapabilities_schema FdoProviderCapabilities \endlink
  for the definition of the XML representation.
</p>
<p>
  The capability characterization can be used to execute code
  conditionally depending on the provider being used and what
  capability is being executed. At the end of this section is a
  PHP class which parses the XML and implements a set of
  boolean functions. These functions can be used to execute
  code conditional on provider capability. Example output from
  this class follows here. The code for the class is after the
  example.
</p>
<h2>Capabilities for Autodesk.Oracle.2.0</h2>
<pre>
Connection
    ThreadCapability
        supportsSingleThreaded(): false
        supportsPerConnectionThreaded(): true
        supportsPerCommandThreaded(): false
        supportsMultiThreaded(): false
    SpatialContextExtent
        Type
            supportsDynamic(): false
            supportsStatic(): true
    supportsLocking(): true
    supportsConnectionTimeout(): false
    supportsTransactions(): true
    supportsLongTransactions(): true
    supportsSQL(): true
    supportsConfiguration(): false
Schema
    Class
        Type
            supportsClass(): true
            supportsFeatureClass(): true
    Data
        Type
            supportsBoolean(): true
            supportsByte(): true
            supportsDateTime(): true
            supportsDecimal(): true
            supportsDouble(): true
            supportsInt16(): true
            supportsInt32(): true
            supportsInt64(): true
            supportsSingle(): true
            supportsString(): true
            supportsBLOB(): true
            supportsCLOB(): true
            supportsUniqueID(): false
    supportsInheritance(): true
    supportsMultipleSchemas(): true
    supportsObjectProperties(): true
    supportsAssociationProperties(): true
    supportsSchemaOverrides(): true
    supportsNetworkModel(): false
    supportsAutoIdGeneration(): true
    supportsDataStoreScopeUniqueIdGeneration(): true
    supportsSchemaModification(): true
Command
    SupportedCommands
        Name
            supportsSelect(): true
            supportsInsert(): true
            supportsDelete(): true
            supportsUpdate(): true
            supportsSelectAggregates(): true
            supportsDescribeSchema(): true
            supportsDescribeSchemaMapping(): false
            supportsDestroySchema(): true
            supportsApplySchema(): true
            supportsActivateSpatialContext(): true
            supportsCreateSpatialContext(): true
            supportsDestroySpatialContext(): true
            supportsDestroySpatialContext(): true
            supportsCreateMeasureUnit(): false
            supportsDestroyMeasureUnit()t: false
            supportsGetMeasureUnits(): false
            supportsSQLCommand(): true
            supportsAcquireLock(): true
            supportsGetLockInfo(): true
            supportsGetLockedObjects(): true
            supportsGetLockOwners(): true
            supportsReleaseLock(): true
            supportsActivateLongTransaction(): true
            supportsCommitLongTransaction(): true
            supportsCreateLongTransaction(): true
            supportsGetLongTransactions(): true
            supportsFreezeLongTransaction(): false
            supportsRollbackLongTransaction(): true
            supportsActivateLongTransactionCheckpoint(): false
            supportsCreateLongTransactionCheckpoint(): false
            supportsGetLongTransactionCheckpoints(): false
            supportsRollbackLongTransactionCheckpoint(): false
            supportsChangeLongTransactionPrivileges(): false
            supportsGetLongTransactionPrivileges(): false
            supportsChangeLongTransactionSet(): false
            supportsGetLongTransactionsInSet(): false
            supportsFirstProviderCommand(): false
            supportsDeactivateLongTransaction(): true
    supportsParameters(): false
    supportsCommandTimeout(): false
    supportsSelectExpressions(): true
    supportsSelectFunctions(): true
    supportsSelectDistinct(): true
    supportsSelectOrdering(): true
    supportsSelectGrouping(): true
Filter
    Condition
        Type
            supportsComparison(): true
            supportsLike(): true
            supportsIn(): true
            supportsNull(): true
            supportsSpatial(): true
            supportsDistance(): true
    Spatial
        Operation
            supportsContains(): false
            supportsCrosses(): false
            supportsDisjoint(): false
            supportsEquals(): false
            supportsIntersects(): true
            supportsOverlaps()s: false
            supportsTouches(): false
            supportsSpatialWithin(): false
            supportsCoveredBy(): true
            supportsInside(): true
            supportsEnvelopeIntersects(): true
    Distance
        Operation
            supportsDistanceWithin(): true
            supportsBeyond(): false
    supportsGeodesicDistance(): false
    supportsNonLiteralGeometricOperations(): false
Expression
    Type
        Name
            supportsBasic(): true
            supportsFunction(): true
            supportsParameter(): true
    FunctionDefinitionCollection
        FunctionDefinition
            Name
                supportsAvg(): true
                supportsCeil(): true
                supportsCLIP(): false
                supportsConcat(): true
                supportsCount(): true
                supportsFloor(): true
                supportsLower(): true
                supportsMax(): true
                supportsMin(): true
                supportsMOSAIC(): false
                supportsStdDev(): false
                supportsSum(): true
                supportsUpper(): true
Raster
    supportsRaster(): false
    supportsStitching(): false
    supportsSubsampling(): false
Topology
    supportsTopology(): false
    supportsTopologicalHierarchy(): false
    supportsBreaksCurveCrossingsAutomatically(): false
    supportsActivatesTopologyByArea(): false
    supportsConstrainsFeatureMovements(): false
Geometry
    Type
        supportsPoint(): true
        supportsMultiPoint(): true
        supportsLineString(): true
        supportsMultiLineString(): true
        supportsPolygon(): true
        supportsMultiPolygon(): true
        supportsCurveString(): true
        supportsMultiCurveString(): true
        supportsCurvePolygon(): true
        supportsMultiCurvePolygon(): true
        supportsMultiGeometry(): false
    Components
        Type
            supportsLinearRing(): true
            supportsLinearSegment(): true
            supportsArcSegment(): true
            supportsCurveRing(): true
    dimensionality(): 3
</pre>
<h2> Class ProviderCapability</h2>
<p>
  The constructor takes a provider name, a file handle, and an
  MgByteReader containing the provider capabilities. The
  provider name is obtained from \link MgFeatureService::GetFeatureProviders MgFeatureService::GetFeatureProviders\endlink.
  The provider capabilities is obtained from 
  \link MgFeatureService::GetCapabilities MgFeatureService::GetCapabilities\endlink.
  The file handle is for logging.
</p>
<pre>
class ProviderCapabilities {
   var $logFileHandle;
   var $xpath;
   var $providerName;

   function ProviderCapabilities( $providerName, $logFileHandle, $byteReader ) {
      try {
     $this-&gt;providerName = $providerName;
     $this-&gt;logFileHandle = $logFileHandle;
     $doc = new DOMDocument();
     $byteSink = new MgByteSink($byteReader);
     $filePath = &quot;temp_byte_reader_file.$providerName&quot;;
//     fwrite($this-&gt;logFileHandle, &quot;file path is \\&quot;$filePath\\&quot;\\n&quot;);
     $byteSink-&gt;ToFile($filePath);
     $doc-&gt;load($filePath);
//     unlink($filePath);
     $this-&gt;xpath = new DOMXPath($doc);
     $this-&gt;queryConnection();
     $this-&gt;querySchema();
     $this-&gt;queryCommand();
     $this-&gt;queryFilter();
     $this-&gt;queryExpression();
     $this-&gt;queryRaster();
     $this-&gt;queryTopology();
     $this-&gt;queryGeometry();
      } catch (Exception $e) {
     $msg = $e-&gt;GetMessage();
     fwrite($logFileHandle, &quot;ProviderCapabilities PHP Exception: $msg\\n&quot;);
      }
   }

   function prtBool($bool) {
      if ($bool) {
     return &quot;true&quot;;
      } else {
     return &quot;false&quot;;
      }
   }

   function queryBooleanElement($queryExpr, &amp;$reference) {
      $domNodeList = $this-&gt;xpath-&gt;query($queryExpr);
      if ($domNodeList-&gt;length == 0) return;
      $domNode = $domNodeList-&gt;item(0);
      $value = $domNode-&gt;nodeValue;
      switch($value) {
     case &quot;true&quot; : $reference = TRUE; break;
     case &quot;false&quot; : $reference = FALSE; break;
     default : fwrite($this-&gt;logFileHandle, &quot;$queryExpr: unknown value: $value\\n&quot;);
      }
   }

/*
   var $supports = FALSE;
   function supports() {
      return $this-&gt;supports;
   }
\*/

   // CONNECTION CAPABILITIES
   // thread capability

   var $supportsSingleThreaded = FALSE;
   function supportsSingleThreaded() {
      return $this-&gt;supportsSingleThreaded;
   }

   var $supportsPerConnectionThreaded = FALSE;
   function supportsPerConnectionThreaded() {
      return $this-&gt;supportsPerConnectionThreaded;
   }

   var $supportsPerCommandThreaded = FALSE;
   function supportsPerCommandThreaded() {
      return $this-&gt;supportsPerCommandThreaded;
   }

   var $supportsMultiThreaded = FALSE;
   function supportsMultiThreaded() {
      return $this-&gt;supportsMultiThreaded;
   }

   // spatial context extent type

   var $supportsDynamic = FALSE;
   function supportsDynamic() {
      return $this-&gt;supportsDynamic;
   }

   var $supportsStatic = FALSE;
   function supportsStatic() {
      return $this-&gt;supportsStatic;
   }

   // rest of connection capabilities
   var $supportsLocking = FALSE;
   function supportsLocking() {
      return $this-&gt;supportsLocking;
   }

   var $supportsConnectionTimeout = FALSE;
   function supportsConnectionTimeout() {
      return $this-&gt;supportsConnectionTimeout;
   }

   var $supportsTransactions = FALSE;
   function supportsTransactions() {
      return $this-&gt;supportsTransactions;
   }

   var $supportsLongTransactions = FALSE;
   function supportsLongTransactions() {
      return $this-&gt;supportsLongTransactions;
   }

   var $supportsSQL = FALSE;
   function supportsSQL() {
      return $this-&gt;supportsSQL;
   }

   var $supportsConfiguration = FALSE;
   function supportsConfiguration() {
      return $this-&gt;supportsConfiguration;
   }

   function queryConnection() {
      $this-&gt;queryThreadCapability();
      $this-&gt;querySpatialContextExtentType();
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Connection/SupportsLocking&quot;, $this-&gt;supportsLocking);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Connection/SupportsTimeout&quot;, $this-&gt;supportsConnectionTimeout);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Connection/SupportsTransactions&quot;, $this-&gt;supportsTransactions);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Connection/SupportsLongTransactions&quot;, $this-&gt;supportsLongTransactions);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Connection/SupportsSQL&quot;, $this-&gt;supportsSQL);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Connection/SupportsConfiguration&quot;, $this-&gt;supportsConfiguration);
   }

   function queryThreadCapability() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Connection/ThreadCapability&quot;);
      if ($domNodeList-&gt;length == 0) return;
      $domNode = $domNodeList-&gt;item(0);
      $value = $domNode-&gt;nodeValue;
      switch($value) {
     case &quot;PerConnectionThreaded&quot; : $this-&gt;supportsPerConnectionThreaded = TRUE; break;
     case &quot;SingleThreaded&quot; : $this-&gt;supportsSingleThreaded = TRUE; break;
     case &quot;PerCommandThreaded&quot; : $this-&gt;supportsPerCommandThreaded = TRUE; break;
     case &quot;MultiThreaded&quot; : $this-&gt;supportsMultiThreaded = TRUE; break;
     default : fwrite($this-&gt;logFileHandle, &quot;unknown thread type: $value\\n&quot;);
      }
   }

   function querySpatialContextExtentType() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Connection/SpatialContextExtent/Type&quot;);
      if ($domNodeList-&gt;length == 0) return;
      $domNode = $domNodeList-&gt;item(0);
      $value = $domNode-&gt;nodeValue;
      switch($value) {
     case &quot;Static&quot; : $this-&gt;supportsStatic = TRUE; break;
     case &quot;Dynamic&quot; : $this-&gt;supportsDynamic = TRUE; break;
     default : fwrite($this-&gt;logFileHandle, &quot;unknown spatial context extent type: $value\\n&quot;);
      }
   }

   // SCHEMA
   // class type
   var $supportsFeatureClass = FALSE;
   function supportsFeatureClass() {
      return $this-&gt;supportsFeatureClass;
   }

   var $supportsClass = FALSE;
   function supportsClass() {
      return $this-&gt;supportsClass;
   }

   // data types
   var $supportsBoolean = FALSE;
   function supportsBoolean() {
      return $this-&gt;supportsBoolean;
   }

   var $supportsByte = FALSE;
   function supportsByte() {
      return $this-&gt;supportsByte;
   }

   var $supportsDateTime = FALSE;
   function supportsDateTime() {
      return $this-&gt;supportsDateTime;
   }

   var $supportsDecimal = FALSE;
   function supportsDecimal() {
      return $this-&gt;supportsDecimal;
   }

   var $supportsDouble = FALSE;
   function supportsDouble() {
      return $this-&gt;supportsDouble;
   }

   var $supportsInt16 = FALSE;
   function supportsInt16() {
      return $this-&gt;supportsInt16;
   }

   var $supportsInt32 = FALSE;
   function supportsInt32() {
      return $this-&gt;supportsInt32;
   }

   var $supportsInt64 = FALSE;
   function supportsInt64() {
      return $this-&gt;supportsInt64;
   }

   var $supportsSingle = FALSE;
   function supportsSingle() {
      return $this-&gt;supportsSingle;
   }

   var $supportsString = FALSE;
   function supportsString() {
      return $this-&gt;supportsString;
   }

   var $supportsBLOB = FALSE;
   function supportsBLOB() {
      return $this-&gt;supportsBLOB;
   }

   var $supportsCLOB = FALSE;
   function supportsCLOB() {
      return $this-&gt;supportsCLOB;
   }

   var $supportsUniqueID = FALSE;
   function supportsUniqueID() {
      return $this-&gt;supportsUniqueID;
   }

   // rest of schema

   var $supportsInheritance = FALSE;
   function supportsInheritance() {
      return $this-&gt;supportsInheritance;
   }

   var $supportsMultipleSchemas = FALSE;
   function supportsMultipleSchemas() {
      return $this-&gt;supportsMultipleSchemas;
   }

   var $supportsObjectProperties = FALSE;
   function supportsObjectProperties() {
      return $this-&gt;supportsObjectProperties;
   }

   var $supportsAssociationProperties = FALSE;
   function supportsAssociationProperties() {
      return $this-&gt;supportsAssociationProperties;
   }

   var $supportsSchemaOverrides = FALSE;
   function supportsSchemaOverrides() {
      return $this-&gt;supportsSchemaOverrides;
   }

   var $supportsNetworkModel = FALSE;
   function supportsNetworkModel() {
      return $this-&gt;supportsNetworkModel;
   }

   var $supportsAutoIdGeneration = FALSE;
   function supportsAutoIdGeneration() {
      return $this-&gt;supportsAutoIdGeneration;
   }

   var $supportsDataStoreScopeUniqueIdGeneration = FALSE;
   function supportsDataStoreScopeUniqueIdGeneration() {
      return $this-&gt;supportsDataStoreScopeUniqueIdGeneration;
   }

   var $supportsSchemaModification = FALSE;
   function supportsSchemaModification() {
      return $this-&gt;supportsSchemaModification;
   }

   function querySchema() {
      $this-&gt;queryClassTypes();
      $this-&gt;queryDataTypes();
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsInheritance&quot;, $this-&gt;supportsInheritance);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsMultipleSchemas&quot;, $this-&gt;supportsMultipleSchemas);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsObjectProperties&quot;, $this-&gt;supportsObjectProperties);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsAssociationProperties&quot;, $this-&gt;supportsAssociationProperties);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsSchemaOverrides&quot;, $this-&gt;supportsSchemaOverrides);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsNetworkModel&quot;, $this-&gt;supportsNetworkModel);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsAutoIdGeneration&quot;, $this-&gt;supportsAutoIdGeneration);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsDataStoreScopeUniqueIdGeneration&quot;, $this-&gt;supportsDataStoreScopeUniqueIdGeneration);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Schema/SupportsSchemaModification&quot;, $this-&gt;supportsSchemaModification);
   }

   function queryClassTypes() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Schema/Class/Type&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Class&quot; : $this-&gt;supportsClass = TRUE; break;
        case &quot;FeatureClass&quot; : $this-&gt;supportsFeatureClass = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown schema class type: $value\\n&quot;);
     }
      }
   }

   function queryDataTypes() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Schema/Data/Type&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Boolean&quot; : $this-&gt;supportsBoolean = TRUE; break;
        case &quot;Byte&quot; : $this-&gt;supportsByte = TRUE; break;
        case &quot;DateTime&quot; : $this-&gt;supportsDateTime = TRUE; break;
        case &quot;Decimal&quot; : $this-&gt;supportsDecimal = TRUE; break;
        case &quot;Double&quot; : $this-&gt;supportsDouble = TRUE; break;
        case &quot;Int16&quot; : $this-&gt;supportsInt16 = TRUE; break;
        case &quot;Int32&quot; : $this-&gt;supportsInt32 = TRUE; break;
        case &quot;Int64&quot; : $this-&gt;supportsInt64 = TRUE; break;
        case &quot;Single&quot; : $this-&gt;supportsSingle = TRUE; break;
        case &quot;String&quot; : $this-&gt;supportsString = TRUE; break;
        case &quot;CLOB&quot; : $this-&gt;supportsCLOB = TRUE; break;
        case &quot;BLOB&quot; : $this-&gt;supportsBLOB = TRUE; break;
        case &quot;UniqueID&quot; : $this-&gt;supportsUniqueID = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown data type: $value\\n&quot;);
     }
      }
   }

   // COMMAND

   var $supportsSelect = FALSE;
   function supportsSelect() {
      return $this-&gt;supportsSelect;
   }

   var $supportsSelectAggregates = FALSE;
   function supportsSelectAggregates() {
      return $this-&gt;supportsSelectAggregates;
   }

   var $supportsInsert = FALSE;
   function supportsInsert() {
      return $this-&gt;supportsInsert;
   }

   var $supportsDelete = FALSE;
   function supportsDelete() {
      return $this-&gt;supportsDelete;
   }

   var $supportsUpdate = FALSE;
   function supportsUpdate() {
      return $this-&gt;supportsUpdate;
   }

   var $supportsDescribeSchema = FALSE;
   function supportsDescribeSchema() {
      return $this-&gt;supportsDescribeSchema;
   }

   var $supportsDescribeSchemaMapping = FALSE;
   function supportsDescribeSchemaMapping() {
      return $this-&gt;supportsDescribeSchemaMapping;
   }

   var $supportsApplySchema = FALSE;
   function supportsApplySchema() {
      return $this-&gt;supportsApplySchema;
   }

   var $supportsDestroySchema = FALSE;
   function supportsDestroySchema() {
      return $this-&gt;supportsDestroySchema;
   }

   var $supportsActivateSpatialContext = FALSE;
   function supportsActivateSpatialContext() {
      return $this-&gt;supportsActivateSpatialContext;
   }
   var $supportsCreateSpatialContext = FALSE;
   function supportsCreateSpatialContext() {
      return $this-&gt;supportsCreateSpatialContext;
   }

   var $supportsDestroySpatialContext = FALSE;
   function supportsDestroySpatialContext() {
      return $this-&gt;supportsDestroySpatialContext;
   }

   var $supportsGetSpatialContexts = FALSE;
   function supportsGetSpatialContexts() {
      return $this-&gt;supportsGetSpatialContexts;
   }

   var $supportsCreateMeasureUnit = FALSE;
   function supportsCreateMeasureUnit() {
      return $this-&gt;supportsCreateMeasureUnit;
   }

   var $supportsDestroyMeasureUnit = FALSE;
   function supportsDestroyMeasureUnit() {
      return $this-&gt;supportsDestroyMeasureUnit;
   }

   var $supportsGetMeasureUnits = FALSE;
   function supportsGetMeasureUnits() {
      return $this-&gt;supportsGetMeasureUnits;
   }
   
   var $supportsSQLCommand = FALSE;
   function supportsSQLCommand() {
      return $this-&gt;supportsSQLCommand;
   }

   var $supportsAcquireLock = FALSE;
   function supportsAcquireLock() {
      return $this-&gt;supportsAcquireLock;
   }

   var $supportsGetLockInfo = FALSE;
   function supportsGetLockInfo() {
      return $this-&gt;supportsGetLockInfo;
   }

   var $supportsGetLockedObjects = FALSE;
   function supportsGetLockedObjects() {
      return $this-&gt;supportsGetLockedObjects;
   }

   var $supportsGetLockOwners = FALSE;
   function supportsGetLockOwners() {
      return $this-&gt;supportsGetLockOwners;
   }

   var $supportsReleaseLock = FALSE;
   function supportsReleaseLock() {
      return $this-&gt;supportsReleaseLock;
   }

   var $supportsActivateLongTransaction = FALSE;
   function supportsActivateLongTransaction() {
      return $this-&gt;supportsActivateLongTransaction;
   }

   var $supportsCommitLongTransaction = FALSE;
   function supportsCommitLongTransaction() {
      return $this-&gt;supportsCommitLongTransaction;
   }

   var $supportsCreateLongTransaction = FALSE;
   function supportsCreateLongTransaction() {
      return $this-&gt;supportsCreateLongTransaction;
   }

   var $supportsGetLongTransactions = FALSE;
   function supportsGetLongTransactions() {
      return $this-&gt;supportsGetLongTransactions;
   }

   var $supportsFreezeLongTransaction = FALSE;
   function supportsFreezeLongTransaction() {
      return $this-&gt;supportsFreezeLongTransaction;
   }

   var $supportsRollbackLongTransaction = FALSE;
   function supportsRollbackLongTransaction() {
      return $this-&gt;supportsRollbackLongTransaction;
   }

   var $supportsActivateLongTransactionCheckpoint = FALSE;
   function supportsActivateLongTransactionCheckpoint() {
      return $this-&gt;supportsActivateLongTransactionCheckpoint;
   }

   var $supportsCreateLongTransactionCheckpoint = FALSE;
   function supportsCreateLongTransactionCheckpoint() {
      return $this-&gt;supportsCreateLongTransactionCheckpoint;
   }

   var $supportsGetLongTransactionCheckpoints = FALSE;
   function supportsGetLongTransactionCheckpoints() {
      return $this-&gt;supportsGetLongTransactionCheckpoints;
   }

   var $supportsRollbackLongTransactionCheckpoint = FALSE;
   function supportsRollbackLongTransactionCheckpoint() {
      return $this-&gt;supportsRollbackLongTransactionCheckpoint;
   }

   var $supportsChangeLongTransactionPrivileges = FALSE;
   function supportsChangeLongTransactionPrivileges() {
      return $this-&gt;supportsChangeLongTransactionPrivileges;
   }

   var $supportsGetLongTransactionPrivileges = FALSE;
   function supportsGetLongTransactionPrivileges() {
      return $this-&gt;supportsGetLongTransactionPrivileges;
   }

   var $supportsChangeLongTransactionSet = FALSE;
   function supportsChangeLongTransactionSet() {
      return $this-&gt;supportsChangeLongTransactionSet;
   }

   var $supportsGetLongTransactionsInSet = FALSE;
   function supportsGetLongTransactionsInSet() {
      return $this-&gt;supportsGetLongTransactionsInSet;
   }

   var $supportsFirstProviderCommand = FALSE;
   function supportsFirstProviderCommand() {
      return $this-&gt;supportsFirstProviderCommand;
   }

   var $supportsDeactivateLongTransaction = FALSE;
   function supportsDeactivateLongTransaction() {
      return $this-&gt;supportsDeactivateLongTransaction;
   }

   // rest of Commands

   var $supportsParameters = FALSE;
   function supportsParameters() {
      return $this-&gt;supportsParameters;
   }

   var $supportsCommandTimeout = FALSE;
   function supportsCommandTimeout() {
      return $this-&gt;supportsCommandTimeout;
   }

   var $supportsSelectExpressions = FALSE;
   function supportsSelectExpressions() {
      return $this-&gt;supportsSelectExpressions;
   }

   var $supportsSelectFunctions = FALSE;
   function supportsSelectFunctions() {
      return $this-&gt;supportsSelectFunctions;
   }

   var $supportsSelectDistinct = FALSE;
   function supportsSelectDistinct() {
      return $this-&gt;supportsSelectDistinct;
   }

   var $supportsSelectOrdering = FALSE;
   function supportsSelectOrdering() {
      return $this-&gt;supportsSelectOrdering;
   }

   var $supportsSelectGrouping = FALSE;
   function supportsSelectGrouping() {
      return $this-&gt;supportsSelectGrouping;
   }

   function queryCommand() {
      $this-&gt;queryCommands();
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Command/SupportsParameters&quot;, $this-&gt;supportsParameters);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Command/SupportsTimeout&quot;, $this-&gt;supportsCommandTimeout);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Command/SupportsSelectExpressions&quot;, $this-&gt;supportsSelectExpressions);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Command/SupportsSelectFunctions&quot;, $this-&gt;supportsSelectFunctions);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Command/SupportsSelectDistinct&quot;, $this-&gt;supportsSelectDistinct);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Command/SupportsSelectOrdering&quot;, $this-&gt;supportsSelectOrdering);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Command/SupportsSelectGrouping&quot;, $this-&gt;supportsSelectGrouping);
   }

   function queryCommands() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Command/SupportedCommands/Name&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Insert&quot; : $this-&gt;supportsInsert = TRUE; break;
        case &quot;Delete&quot; : $this-&gt;supportsDelete = TRUE; break;
        case &quot;Update&quot; : $this-&gt;supportsUpdate = TRUE; break;
        case &quot;Select&quot; : $this-&gt;supportsSelect = TRUE; break;
        case &quot;SelectAggregates&quot; : $this-&gt;supportsSelectAggregates = TRUE; break;
        case &quot;DescribeSchema&quot; : $this-&gt;supportsDescribeSchema = TRUE; break;
        case &quot;DescribeSchemaMapping&quot; : $this-&gt;supportsDescribeSchemaMapping = TRUE; break;
        case &quot;DestroySchema&quot; : $this-&gt;supportsDestroySchema = TRUE; break;
        case &quot;ApplySchema&quot; : $this-&gt;supportsApplySchema = TRUE; break;
        case &quot;SQLCommand&quot; : $this-&gt;supportsSQLCommand = TRUE; break;
        case &quot;ActivateSpatialContext&quot; : $this-&gt;supportsActivateSpatialContext = TRUE; break;
        case &quot;CreateSpatialContext&quot; : $this-&gt;supportsCreateSpatialContext = TRUE; break;
        case &quot;DestroySpatialContext&quot; : $this-&gt;supportsDestroySpatialContext = TRUE; break;
        case &quot;GetSpatialContexts&quot; : $this-&gt;supportsGetSpatialContexts = TRUE; break;
        case &quot;CreateMeasureUnit&quot; : $this-&gt;supportsCreateMeasureUnit = TRUE; break;
        case &quot;DestroyMeasureUnit&quot; : $this-&gt;supportsDestroyMeasureUnit = TRUE; break;
        case &quot;GetMeasureUnits&quot; : $this-&gt;supportsGetMeasureUnits = TRUE; break;
        case &quot;AcquireLock&quot; : $this-&gt;supportsAcquireLock = TRUE; break;
        case &quot;GetLockInfo&quot; : $this-&gt;supportsGetLockInfo = TRUE; break;
        case &quot;GetLockedObjects&quot; : $this-&gt;supportsGetLockedObjects = TRUE; break;
        case &quot;GetLockOwners&quot; : $this-&gt;supportsGetLockOwners = TRUE; break;
        case &quot;ReleaseLock&quot; : $this-&gt;supportsReleaseLock = TRUE; break;
        case &quot;ActivateLongTransaction&quot; : $this-&gt;supportsActivateLongTransaction = TRUE; break;
        case &quot;CommitLongTransaction&quot; : $this-&gt;supportsCommitLongTransaction = TRUE; break;
        case &quot;CreateLongTransaction&quot; : $this-&gt;supportsCreateLongTransaction = TRUE; break;
        case &quot;GetLongTransactions&quot; : $this-&gt;supportsGetLongTransactions = TRUE; break;
        case &quot;FreezeLongTransaction&quot; : $this-&gt;supportsFreezeLongTransaction = TRUE; break;
        case &quot;RollbackLongTransaction&quot; : $this-&gt;supportsRollbackLongTransaction = TRUE; break;
        case &quot;ActivateLongTransactionCheckpoint&quot; : $this-&gt;supportsActivateLongTransactionCheckpoint = TRUE; break;
        case &quot;CreateLongTransactionCheckpoint&quot; : $this-&gt;supportsCreateLongTransactionCheckpoint = TRUE; break;
        case &quot;GetLongTransactionCheckpoints&quot; : $this-&gt;supportsGetLongTransactionCheckpoints = TRUE; break;
        case &quot;RollbackLongTransactionCheckpoint&quot; : $this-&gt;supportsRollbackLongTransactionCheckpoint = TRUE; break;
        case &quot;ChangeLongTransactionPrivileges&quot; : $this-&gt;supportsChangeLongTransactionPrivileges = TRUE; break;
        case &quot;GetLongTransactionPrivileges&quot; : $this-&gt;supportsGetLongTransactionPrivileges = TRUE; break;
        case &quot;ChangeLongTransactionSet&quot; : $this-&gt;supportsChangeLongTransactionSet = TRUE; break;
        case &quot;GetLongTransactionsInSet&quot; : $this-&gt;supportsGetLongTransactionsInSet = TRUE; break;
        case &quot;FirstProviderCommand&quot; : $this-&gt;supportsFirstProviderCommand = TRUE; break;
        case &quot;DeactivateLongTransaction&quot; : $this-&gt;supportsDeactivateLongTransaction = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown command type: $value\\n&quot;);
     }
      }
   }

   // FILTER

   // Condition types

   var $supportsComparison = FALSE;
   function supportsComparison() {
      return $this-&gt;supportsComparison;
   }

   var $supportsLike = FALSE;
   function supportsLike() {
      return $this-&gt;supportsLike;
   }

   var $supportsIn = FALSE;
   function supportsIn() {
      return $this-&gt;supportsIn;
   }

   var $supportsNull = FALSE;
   function supportsNull() {
      return $this-&gt;supportsNull;
   }

   var $supportsSpatial = FALSE;
   function supportsSpatial() {
      return $this-&gt;supportsSpatial;
   }

   var $supportsDistance = FALSE;
   function supportsDistance() {
      return $this-&gt;supportsDistance;
   }

   // Spatial types

   var $supportsContains = FALSE;
   function supportsContains() {
      return $this-&gt;supportsContains;
   }

   var $supportsCrosses = FALSE;
   function supportsCrosses() {
      return $this-&gt;supportsCrosses;
   }

   var $supportsDisjoint = FALSE;
   function supportsDisjoint() {
      return $this-&gt;supportsDisjoint;
   }

   var $supportsEquals = FALSE;
   function supportsEquals() {
      return $this-&gt;supportsEquals;
   }

   var $supportsIntersects = FALSE;
   function supportsIntersects() {
      return $this-&gt;supportsIntersects;
   }

   var $supportsOverlaps = FALSE;
   function supportsOverlaps() {
      return $this-&gt;supportsOverlaps;
   }

   var $supportsTouches = FALSE;
   function supportsTouches() {
      return $this-&gt;supportsTouches;
   }

   var $supportsSpatialWithin = FALSE;
   function supportsSpatialWithin() {
      return $this-&gt;supportsSpatialWithin;
   }

   var $supportsCoveredBy = FALSE;
   function supportsCoveredBy() {
      return $this-&gt;supportsCoveredBy;
   }

   var $supportsInside = FALSE;
   function supportsInside() {
      return $this-&gt;supportsInside;
   }

   var $supportsEnvelopeIntersects = FALSE;
   function supportsEnvelopeIntersects() {
      return $this-&gt;supportsEnvelopeIntersects;
   }

   // Distance types

   var $supportsBeyond = FALSE;
   function supportsBeyond() {
      return $this-&gt;supportsBeyond;
   }

   var $supportsDistanceWithin = FALSE;
   function supportsDistanceWithin() {
      return $this-&gt;supportsDistanceWithin;
   }

   // rest of Filter

   var $supportsGeodesicDistance = FALSE;
   function supportsGeodesicDistance() {
      return $this-&gt;supportsGeodesicDistance;
   }

   var $supportsNonLiteralGeometricOperations = FALSE;
   function supportsNonLiteralGeometricOperations() {
      return $this-&gt;supportsNonLiteralGeometricOperations;
   }

   function queryFilter() {
      $this-&gt;queryConditionType();
      $this-&gt;querySpatialOperations();
      $this-&gt;queryDistanceOperations();
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Filter/SupportsGeodesicDistance&quot;, $this-&gt;supportsGeodesicDistance);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Filter/SupportsNonLiteralGeometricOperations&quot;, $this-&gt;supportsNonLiteralGeometricOperations);
   }

   function queryConditionType() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Filter/Condition/Type&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Comparison&quot; : $this-&gt;supportsComparison = TRUE; break;
        case &quot;Like&quot; : $this-&gt;supportsLike = TRUE; break;
        case &quot;In&quot; : $this-&gt;supportsIn = TRUE; break;
        case &quot;Null&quot; : $this-&gt;supportsNull = TRUE; break;
        case &quot;Spatial&quot; : $this-&gt;supportsSpatial = TRUE; break;
        case &quot;Distance&quot; : $this-&gt;supportsDistance = TRUE; break;
; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown filter condition type: $value\\n&quot;);
     }
      }
   }

   function querySpatialOperations() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Filter/Spatial/Operation&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Contains&quot; : $this-&gt;supportsContains = TRUE; break;
        case &quot;Crosses&quot; : $this-&gt;supportsCrosses = TRUE; break;
        case &quot;Disjoint&quot; : $this-&gt;supportsDisjoint = TRUE; break;
        case &quot;Equals&quot; : $this-&gt;supportsEquals = TRUE; break;
        case &quot;Intersects&quot; : $this-&gt;supportsIntersects = TRUE; break;
        case &quot;Overlaps&quot; : $this-&gt;supportsOverlaps = TRUE; break;
        case &quot;Touches&quot; : $this-&gt;supportsTouches = TRUE; break;
        case &quot;Within&quot; : $this-&gt;supportsSpatialWithin = TRUE; break;
        case &quot;CoveredBy&quot; : $this-&gt;supportsCoveredBy = TRUE; break;
        case &quot;Inside&quot; : $this-&gt;supportsInside = TRUE; break;
        case &quot;EnvelopeIntersects&quot; : $this-&gt;supportsEnvelopeIntersects = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown filter spatial operation: $value\\n&quot;);
     }
      }
   }

   function queryDistanceOperations() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Filter/Distance/Operation&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Within&quot; : $this-&gt;supportsDistanceWithin = TRUE; break;
        case &quot;Beyond&quot; : $this-&gt;supportsBeyond = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown filter distance operation: $value\\n&quot;);
     }
      }
   }


   // EXPRESSION

   // types

   var $supportsBasic = FALSE;
   function supportsBasic() {
      return $this-&gt;supportsBasic;
   }

   var $supportsFunction = FALSE;
   function supportsFunction() {
      return $this-&gt;supportsFunction;
   }

   var $supportsParameter = FALSE;
   function supportsParameter() {
      return $this-&gt;supportsParameter;
   }

   var $supportsAvg = FALSE;
   function supportsAvg() {
      return $this-&gt;supportsAvg;
   }

   var $supportsCeil = FALSE;
   function supportsCeil() {
      return $this-&gt;supportsCeil;
   }

   var $supportsCLIP = FALSE;
   function supportsCLIP() {
      return $this-&gt;supportsCLIP;
   }

   var $supportsConcat = FALSE;
   function supportsConcat() {
      return $this-&gt;supportsConcat;
   }

   var $supportsCount = FALSE;
   function supportsCount() {
      return $this-&gt;supportsCount;
   }

   var $supportsFloor = FALSE;
   function supportsFloor() {
      return $this-&gt;supportsFloor;
   }

   var $supportsLower = FALSE;
   function supportsLower() {
      return $this-&gt;supportsLower;
   }

   var $supportsMax = FALSE;
   function supportsMax() {
      return $this-&gt;supportsMax;
   }

   var $supportsMin = FALSE;
   function supportsMin() {
      return $this-&gt;supportsMin;
   }

   var $supportsMOSAIC = FALSE;
   function supportsMOSAIC() {
      return $this-&gt;supportsMOSAIC;
   }

   var $supportsStdDev = FALSE;
   function supportsStdDev() {
      return $this-&gt;supportsStdDev;
   }

   var $supportsSum = FALSE;
   function supportsSum() {
      return $this-&gt;supportsSum;
   }

   var $supportsUpper = FALSE;
   function supportsUpper() {
      return $this-&gt;supportsUpper;
   }

   function queryExpression() {
      $this-&gt;queryExpressionType();
      $this-&gt;queryFunctionDefinitions();
   }

   function queryExpressionType() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Expression/Type/Name&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Basic&quot; : $this-&gt;supportsBasic = TRUE; break;
        case &quot;Function&quot; : $this-&gt;supportsFunction = TRUE; break;
        case &quot;Parameter&quot; : $this-&gt;supportsParameter = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown filter distance operation: $value\\n&quot;);
     }
      }
   }

   function queryFunctionDefinitions() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Expression/FunctionDefinitionCollection/FunctionDefinition/Name&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Avg&quot; : $this-&gt;supportsAvg = TRUE; break;
        case &quot;Ceil&quot; : $this-&gt;supportsCeil = TRUE; break;
        case &quot;CLIP&quot; : $this-&gt;supportsCLIP = TRUE; break;
        case &quot;Concat&quot; : $this-&gt;supportsConcat = TRUE; break;
        case &quot;Count&quot; : $this-&gt;supportsCount = TRUE; break;
        case &quot;Floor&quot; : $this-&gt;supportsFloor = TRUE; break;
        case &quot;Lower&quot; : $this-&gt;supportsLower = TRUE; break;
        case &quot;Max&quot; : $this-&gt;supportsMax = TRUE; break;
        case &quot;Min&quot; : $this-&gt;supportsMin = TRUE; break;
        case &quot;MOSAIC&quot; : $this-&gt;supportsMOSAIC = TRUE; break;
        case &quot;StdDev&quot; : $this-&gt;supportsStdDev = TRUE; break;
        case &quot;Sum&quot; : $this-&gt;supportsSum = TRUE; break;
        case &quot;Upper&quot; : $this-&gt;supportsUpper = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown expression function name: $value\\n&quot;);
     }
      }
   }

   // RASTER

   var $supportsRaster = FALSE;
   function supportsRaster() {
      return $this-&gt;supportsRaster;
   }

   var $supportsStitching = FALSE;
   function supportsStitching() {
      return $this-&gt;supportsStitching;
   }

   var $supportsSubsampling = FALSE;
   function supportsSubsampling() {
      return $this-&gt;supportsSubsampling;
   }

   function queryRaster() {
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Raster/SupportsRaster&quot;, $this-&gt;supportsRaster);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Raster/SupportsStitching&quot;, $this-&gt;supportsStitching);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Raster/SupportsSubsampling&quot;, $this-&gt;supportsSubsampling);
   }

   // TOPOLOGY

   var $supportsTopology = FALSE;
   function supportsTopology() {
      return $this-&gt;supportsTopology;
   }

   var $supportsTopologicalHierarchy = FALSE;
   function supportsTopologicalHierarchy() {
      return $this-&gt;supportsTopologicalHierarchy;
   }

   var $supportsBreaksCurveCrossingsAutomatically = FALSE;
   function supportsBreaksCurveCrossingsAutomatically() {
      return $this-&gt;supportsBreaksCurveCrossingsAutomatically;
   }

   var $supportsActivatesTopologyByArea = FALSE;
   function supportsActivatesTopologyByArea() {
      return $this-&gt;supportsActivatesTopologyByArea;
   }

   var $supportsConstrainsFeatureMovements = FALSE;
   function supportsConstrainsFeatureMovements() {
      return $this-&gt;supportsConstrainsFeatureMovements;
   }

   function queryTopology() {
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Topology/SupportsTopology&quot;, $this-&gt;supportsTopology);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Topology/SupportsTopologicalHierarchy&quot;, $this-&gt;supportsTopologicalHierarchy);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Topology/BreaksCurveCrossingsAutomatically&quot;, $this-&gt;supportsBreaksCurveCrossingsAutomatically);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Topology/ActivatesTopologyByArea&quot;, $this-&gt;supportsActivatesTopologyByArea);
      $this-&gt;queryBooleanElement(&quot;//FeatureProviderCapabilities/Topology/ConstrainsFeatureMovements&quot;, $this-&gt;supportsConstrainsFeatureMovements);
   }

   // GEOMETRY

   var $supportsPoint = FALSE;
   function supportsPoint() {
      return $this-&gt;supportsPoint;
   }

   var $supportsMultiPoint = FALSE;
   function supportsMultiPoint() {
      return $this-&gt;supportsMultiPoint;
   }

   var $supportsLineString = FALSE;
   function supportsLineString() {
      return $this-&gt;supportsLineString;
   }

   var $supportsMultiLineString = FALSE;
   function supportsMultiLineString() {
      return $this-&gt;supportsMultiLineString;
   }

   var $supportsPolygon = FALSE;
   function supportsPolygon() {
      return $this-&gt;supportsPolygon;
   }

   var $supportsMultiPolygon = FALSE;
   function supportsMultiPolygon() {
      return $this-&gt;supportsMultiPolygon;
   }

   var $supportsCurveString = FALSE;
   function supportsCurveString() {
      return $this-&gt;supportsCurveString;
   }

   var $supportsMultiCurveString = FALSE;
   function supportsMultiCurveString() {
      return $this-&gt;supportsMultiCurveString;
   }

   var $supportsCurvePolygon = FALSE;
   function supportsCurvePolygon() {
      return $this-&gt;supportsCurvePolygon;
   }

   var $supportsMultiCurvePolygon = FALSE;
   function supportsMultiCurvePolygon() {
      return $this-&gt;supportsMultiCurvePolygon;
   }

   var $supportsMultiGeometry = FALSE;
   function supportsMultiGeometry() {
      return $this-&gt;supportsMultiGeometry;
   }

   var $supportsLinearRing = FALSE;
   function supportsLinearRing() {
      return $this-&gt;supportsLinearRing;
   }
   var $supportsLinearSegment = FALSE;
   function supportsLinearSegment() {
      return $this-&gt;supportsLinearSegment;
   }
   var $supportsArcSegment = FALSE;
   function supportsArcSegment() {
      return $this-&gt;supportsArcSegment;
   }
   var $supportsCurveRing = FALSE;
   function supportsCurveRing() {
      return $this-&gt;supportsCurveRing;
   }

   var $dimensionality = -1;
   function getDimensionality() {
      return $this-&gt;dimensionality;
   }

   function queryGeometry() {
      $this-&gt;queryGeometries();
      $this-&gt;queryComponents();
      $this-&gt;queryDimensionality();
   }

   function queryGeometries() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Geometry/Type&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;Point&quot; : $this-&gt;supportsPoint = TRUE; break;
        case &quot;MultiPoint&quot; : $this-&gt;supportsMultiPoint = TRUE; break;
        case &quot;LineString&quot; : $this-&gt;supportsLineString = TRUE; break;
        case &quot;MultiLineString&quot; : $this-&gt;supportsMultiLineString = TRUE; break;
        case &quot;Polygon&quot; : $this-&gt;supportsPolygon = TRUE; break;
        case &quot;MultiPolygon&quot; : $this-&gt;supportsMultiPolygon = TRUE; break;
        case &quot;CurveString&quot; : $this-&gt;supportsCurveString = TRUE; break;
        case &quot;MultiCurveString&quot; : $this-&gt;supportsMultiCurveString = TRUE; break;
        case &quot;CurvePolygon&quot; : $this-&gt;supportsCurvePolygon = TRUE; break;
        case &quot;MultiCurvePolygon&quot; : $this-&gt;supportsMultiCurvePolygon = TRUE; break;
        case &quot;MultiGeometry&quot; : $this-&gt;supportsMultiGeometry = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown geometry type: $value\\n&quot;);
     }
      }
   }

   function queryComponents() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Geometry/Components/Type&quot;);
      $count = $domNodeList-&gt;length;
      for($i=0; $i&lt;$count; $i++) {
     $domNode = $domNodeList-&gt;item($i);
     $value = $domNode-&gt;nodeValue;
     switch($value) {
        case &quot;LinearRing&quot; : $this-&gt;supportsLinearRing = TRUE; break;
        case &quot;LinearSegment&quot; : $this-&gt;supportsLinearSegment = TRUE; break;
        case &quot;ArcSegment&quot; : $this-&gt;supportsArcSegment = TRUE; break;
        case &quot;CurveRing&quot; : $this-&gt;supportsCurveRing = TRUE; break;
        default : fwrite($this-&gt;logFileHandle, &quot;unknown geometry component type: $value\\n&quot;);
     }
      }
   }

   function queryDimensionality() {
      $domNodeList = $this-&gt;xpath-&gt;query(&quot;//FeatureProviderCapabilities/Geometry/Dimensionality&quot;);
      if ($domNodeList-&gt;length == 0) return;
      $domNode = $domNodeList-&gt;item(0);
      $value = $domNode-&gt;nodeValue;
      $this-&gt;dimensionality = $value;
   }

   function logCapabilities() {
      fwrite($this-&gt;logFileHandle, &quot;Capabilities for $this-&gt;providerName\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;Connection\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\tThreadCapability\\n&quot;);
      $logEntry = &quot;\\t\\tsupportsSingleThreaded(): &quot; . $this-&gt;prtBool($this-&gt;supportsSingleThreaded()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsPerConnectionThreaded(): &quot; . $this-&gt;prtBool($this-&gt;supportsPerConnectionThreaded()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsPerCommandThreaded(): &quot; . $this-&gt;prtBool($this-&gt;supportsPerCommandThreaded()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsMultiThreaded(): &quot; . $this-&gt;prtBool($this-&gt;supportsMultiThreaded()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;\\tSpatialContextExtent\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tType\\n&quot;);
      $logEntry = &quot;\\t\\t\\tsupportsDynamic(): &quot; . $this-&gt;prtBool($this-&gt;supportsDynamic()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsStatic(): &quot; . $this-&gt;prtBool($this-&gt;supportsStatic()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsLocking(): &quot; . $this-&gt;prtBool($this-&gt;supportsLocking()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsConnectionTimeout(): &quot; . $this-&gt;prtBool($this-&gt;supportsConnectionTimeout()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsTransactions(): &quot; . $this-&gt;prtBool($this-&gt;supportsTransactions()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsLongTransactions(): &quot; . $this-&gt;prtBool($this-&gt;supportsLongTransactions()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSQL(): &quot; . $this-&gt;prtBool($this-&gt;supportsSQL()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsConfiguration(): &quot; . $this-&gt;prtBool($this-&gt;supportsConfiguration()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;Schema\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\tClass\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tType\\n&quot;);
     $logEntry = &quot;\\t\\t\\tsupportsClass(): &quot; . $this-&gt;prtBool($this-&gt;supportsClass()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsFeatureClass(): &quot; . $this-&gt;prtBool($this-&gt;supportsFeatureClass()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;\\tData\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tType\\n&quot;);
      $logEntry = &quot;\\t\\t\\tsupportsBoolean(): &quot; . $this-&gt;prtBool($this-&gt;supportsBoolean()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsByte(): &quot; . $this-&gt;prtBool($this-&gt;supportsByte()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDateTime(): &quot; . $this-&gt;prtBool($this-&gt;supportsDateTime()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDecimal(): &quot; . $this-&gt;prtBool($this-&gt;supportsDecimal()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDouble(): &quot; . $this-&gt;prtBool($this-&gt;supportsDouble()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsInt16(): &quot; . $this-&gt;prtBool($this-&gt;supportsInt16()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsInt32(): &quot; . $this-&gt;prtBool($this-&gt;supportsInt32()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsInt64(): &quot; . $this-&gt;prtBool($this-&gt;supportsInt64()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsSingle(): &quot; . $this-&gt;prtBool($this-&gt;supportsSingle()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsString(): &quot; . $this-&gt;prtBool($this-&gt;supportsString()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsBLOB(): &quot; . $this-&gt;prtBool($this-&gt;supportsBLOB()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCLOB(): &quot; . $this-&gt;prtBool($this-&gt;supportsCLOB()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsUniqueID(): &quot; . $this-&gt;prtBool($this-&gt;supportsUniqueID()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsInheritance(): &quot; . $this-&gt;prtBool($this-&gt;supportsInheritance()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsMultipleSchemas(): &quot; . $this-&gt;prtBool($this-&gt;supportsMultipleSchemas()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsObjectProperties(): &quot; . $this-&gt;prtBool($this-&gt;supportsObjectProperties()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsAssociationProperties(): &quot; . $this-&gt;prtBool($this-&gt;supportsAssociationProperties()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSchemaOverrides(): &quot; . $this-&gt;prtBool($this-&gt;supportsSchemaOverrides()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsNetworkModel(): &quot; . $this-&gt;prtBool($this-&gt;supportsNetworkModel()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsAutoIdGeneration(): &quot; . $this-&gt;prtBool($this-&gt;supportsAutoIdGeneration()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsDataStoreScopeUniqueIdGeneration(): &quot; . $this-&gt;prtBool($this-&gt;supportsDataStoreScopeUniqueIdGeneration()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSchemaModification(): &quot; . $this-&gt;prtBool($this-&gt;supportsSchemaModification()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;Command\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\tSupportedCommands\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tName\\n&quot;);
      $logEntry = &quot;\\t\\t\\tsupportsSelect(): &quot; . $this-&gt;prtBool($this-&gt;supportsSelect()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsInsert(): &quot; . $this-&gt;prtBool($this-&gt;supportsInsert()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDelete(): &quot; . $this-&gt;prtBool($this-&gt;supportsDelete()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsUpdate(): &quot; . $this-&gt;prtBool($this-&gt;supportsUpdate()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsSelectAggregates(): &quot; . $this-&gt;prtBool($this-&gt;supportsSelectAggregates()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDescribeSchema(): &quot; . $this-&gt;prtBool($this-&gt;supportsDescribeSchema()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDescribeSchemaMapping(): &quot; . $this-&gt;prtBool($this-&gt;supportsDescribeSchemaMapping()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDestroySchema(): &quot; . $this-&gt;prtBool($this-&gt;supportsDestroySchema()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsApplySchema(): &quot; . $this-&gt;prtBool($this-&gt;supportsApplySchema()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsActivateSpatialContext(): &quot; . $this-&gt;prtBool($this-&gt;supportsActivateSpatialContext()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCreateSpatialContext(): &quot; . $this-&gt;prtBool($this-&gt;supportsCreateSpatialContext()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDestroySpatialContext(): &quot; . $this-&gt;prtBool($this-&gt;supportsDestroySpatialContext()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDestroySpatialContext(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetSpatialContexts) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCreateMeasureUnit(): &quot; . $this-&gt;prtBool($this-&gt;supportsCreateMeasureUnit()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDestroyMeasureUnit()t: &quot; . $this-&gt;prtBool($this-&gt;supportsDestroyMeasureUnit()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsGetMeasureUnits(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetMeasureUnits()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsSQLCommand(): &quot; . $this-&gt;prtBool($this-&gt;supportsSQLCommand()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsAcquireLock(): &quot; . $this-&gt;prtBool($this-&gt;supportsAcquireLock()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsGetLockInfo(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetLockInfo()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsGetLockedObjects(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetLockedObjects()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsGetLockOwners(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetLockOwners()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsReleaseLock(): &quot; . $this-&gt;prtBool($this-&gt;supportsReleaseLock()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsActivateLongTransaction(): &quot; . $this-&gt;prtBool($this-&gt;supportsActivateLongTransaction()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCommitLongTransaction(): &quot; . $this-&gt;prtBool($this-&gt;supportsCommitLongTransaction()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCreateLongTransaction(): &quot; . $this-&gt;prtBool($this-&gt;supportsCreateLongTransaction()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsGetLongTransactions(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetLongTransactions()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsFreezeLongTransaction(): &quot; . $this-&gt;prtBool($this-&gt;supportsFreezeLongTransaction()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsRollbackLongTransaction(): &quot; . $this-&gt;prtBool($this-&gt;supportsRollbackLongTransaction()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsActivateLongTransactionCheckpoint(): &quot; . $this-&gt;prtBool($this-&gt;supportsActivateLongTransactionCheckpoint()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCreateLongTransactionCheckpoint(): &quot; . $this-&gt;prtBool($this-&gt;supportsCreateLongTransactionCheckpoint()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsGetLongTransactionCheckpoints(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetLongTransactionCheckpoints()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsRollbackLongTransactionCheckpoint(): &quot; . $this-&gt;prtBool($this-&gt;supportsRollbackLongTransactionCheckpoint()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsChangeLongTransactionPrivileges(): &quot; . $this-&gt;prtBool($this-&gt;supportsChangeLongTransactionPrivileges()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsGetLongTransactionPrivileges(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetLongTransactionPrivileges()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsChangeLongTransactionSet(): &quot; . $this-&gt;prtBool($this-&gt;supportsChangeLongTransactionSet()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsGetLongTransactionsInSet(): &quot; . $this-&gt;prtBool($this-&gt;supportsGetLongTransactionsInSet()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsFirstProviderCommand(): &quot; . $this-&gt;prtBool($this-&gt;supportsFirstProviderCommand()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDeactivateLongTransaction(): &quot; . $this-&gt;prtBool($this-&gt;supportsDeactivateLongTransaction()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsParameters(): &quot; . $this-&gt;prtBool($this-&gt;supportsParameters()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsCommandTimeout(): &quot; . $this-&gt;prtBool($this-&gt;supportsCommandTimeout()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSelectExpressions(): &quot; . $this-&gt;prtBool($this-&gt;supportsSelectExpressions()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSelectFunctions(): &quot; . $this-&gt;prtBool($this-&gt;supportsSelectFunctions()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSelectDistinct(): &quot; . $this-&gt;prtBool($this-&gt;supportsSelectDistinct()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSelectOrdering(): &quot; . $this-&gt;prtBool($this-&gt;supportsSelectOrdering()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSelectGrouping(): &quot; . $this-&gt;prtBool($this-&gt;supportsSelectGrouping()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;Filter\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\tCondition\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tType\\n&quot;);
      $logEntry = &quot;\\t\\t\\tsupportsComparison(): &quot; . $this-&gt;prtBool($this-&gt;supportsComparison()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsLike(): &quot; . $this-&gt;prtBool($this-&gt;supportsLike()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsIn(): &quot; . $this-&gt;prtBool($this-&gt;supportsIn()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsNull(): &quot; . $this-&gt;prtBool($this-&gt;supportsNull()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsSpatial(): &quot; . $this-&gt;prtBool($this-&gt;supportsSpatial()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDistance(): &quot; . $this-&gt;prtBool($this-&gt;supportsDistance()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;\\tSpatial\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tOperation\\n&quot;);
      $logEntry = &quot;\\t\\t\\tsupportsContains(): &quot; . $this-&gt;prtBool($this-&gt;supportsContains()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCrosses(): &quot; . $this-&gt;prtBool($this-&gt;supportsCrosses()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsDisjoint(): &quot; . $this-&gt;prtBool($this-&gt;supportsDisjoint()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsEquals(): &quot; . $this-&gt;prtBool($this-&gt;supportsEquals()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsIntersects(): &quot; . $this-&gt;prtBool($this-&gt;supportsIntersects()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsOverlaps()s: &quot; . $this-&gt;prtBool($this-&gt;supportsOverlaps()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsTouches(): &quot; . $this-&gt;prtBool($this-&gt;supportsTouches()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsSpatialWithin(): &quot; . $this-&gt;prtBool($this-&gt;supportsSpatialWithin()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCoveredBy(): &quot; . $this-&gt;prtBool($this-&gt;supportsCoveredBy()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsInside(): &quot; . $this-&gt;prtBool($this-&gt;supportsInside()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsEnvelopeIntersects(): &quot; . $this-&gt;prtBool($this-&gt;supportsEnvelopeIntersects()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;\\tDistance\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tOperation\\n&quot;);
      $logEntry = &quot;\\t\\t\\tsupportsDistanceWithin(): &quot; . $this-&gt;prtBool($this-&gt;supportsDistanceWithin()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsBeyond(): &quot; . $this-&gt;prtBool($this-&gt;supportsBeyond()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsGeodesicDistance(): &quot; . $this-&gt;prtBool($this-&gt;supportsGeodesicDistance()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsNonLiteralGeometricOperations(): &quot; . $this-&gt;prtBool($this-&gt;supportsNonLiteralGeometricOperations()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;Expression\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\tType\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tName\\n&quot;);
      $logEntry = &quot;\\t\\t\\tsupportsBasic(): &quot; . $this-&gt;prtBool($this-&gt;supportsBasic()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsFunction(): &quot; . $this-&gt;prtBool($this-&gt;supportsFunction()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsParameter(): &quot; . $this-&gt;prtBool($this-&gt;supportsParameter()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;\\tFunctionDefinitionCollection\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tFunctionDefinition\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\t\\tName\\n&quot;);
      $logEntry = &quot;\\t\\t\\t\\tsupportsAvg(): &quot; . $this-&gt;prtBool($this-&gt;supportsAvg()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsCeil(): &quot; . $this-&gt;prtBool($this-&gt;supportsCeil()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsCLIP(): &quot; . $this-&gt;prtBool($this-&gt;supportsCLIP()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsConcat(): &quot; . $this-&gt;prtBool($this-&gt;supportsConcat()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsCount(): &quot; . $this-&gt;prtBool($this-&gt;supportsCount()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsFloor(): &quot; . $this-&gt;prtBool($this-&gt;supportsFloor()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsLower(): &quot; . $this-&gt;prtBool($this-&gt;supportsLower()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsMax(): &quot; . $this-&gt;prtBool($this-&gt;supportsMax()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsMin(): &quot; . $this-&gt;prtBool($this-&gt;supportsMin()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsMOSAIC(): &quot; . $this-&gt;prtBool($this-&gt;supportsMOSAIC()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsStdDev(): &quot; . $this-&gt;prtBool($this-&gt;supportsStdDev()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsSum(): &quot; . $this-&gt;prtBool($this-&gt;supportsSum()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\t\\tsupportsUpper(): &quot; . $this-&gt;prtBool($this-&gt;supportsUpper()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;Raster\\n&quot;);
      $logEntry = &quot;\\tsupportsRaster(): &quot; . $this-&gt;prtBool($this-&gt;supportsRaster()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsStitching(): &quot; . $this-&gt;prtBool($this-&gt;supportsStitching()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsSubsampling(): &quot; . $this-&gt;prtBool($this-&gt;supportsSubsampling()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;Topology\\n&quot;);
      $logEntry = &quot;\\tsupportsTopology(): &quot; . $this-&gt;prtBool($this-&gt;supportsTopology()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsTopologicalHierarchy(): &quot; . $this-&gt;prtBool($this-&gt;supportsTopologicalHierarchy()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsBreaksCurveCrossingsAutomatically(): &quot; . $this-&gt;prtBool($this-&gt;supportsBreaksCurveCrossingsAutomatically()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsActivatesTopologyByArea(): &quot; . $this-&gt;prtBool($this-&gt;supportsActivatesTopologyByArea()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tsupportsConstrainsFeatureMovements(): &quot; . $this-&gt;prtBool($this-&gt;supportsConstrainsFeatureMovements()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;Geometry\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\tType\\n&quot;);
      $logEntry = &quot;\\t\\tsupportsPoint(): &quot; . $this-&gt;prtBool($this-&gt;supportsPoint()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsMultiPoint(): &quot; . $this-&gt;prtBool($this-&gt;supportsMultiPoint()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsLineString(): &quot; . $this-&gt;prtBool($this-&gt;supportsLineString()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsMultiLineString(): &quot; . $this-&gt;prtBool($this-&gt;supportsMultiLineString()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsPolygon(): &quot; . $this-&gt;prtBool($this-&gt;supportsPolygon()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsMultiPolygon(): &quot; . $this-&gt;prtBool($this-&gt;supportsMultiPolygon()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsCurveString(): &quot; . $this-&gt;prtBool($this-&gt;supportsCurveString()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsMultiCurveString(): &quot; . $this-&gt;prtBool($this-&gt;supportsMultiCurveString()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsCurvePolygon(): &quot; . $this-&gt;prtBool($this-&gt;supportsCurvePolygon()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsMultiCurvePolygon(): &quot; . $this-&gt;prtBool($this-&gt;supportsMultiCurvePolygon()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\tsupportsMultiGeometry(): &quot; . $this-&gt;prtBool($this-&gt;supportsMultiGeometry()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      fwrite($this-&gt;logFileHandle, &quot;\\tComponents\\n&quot;);
      fwrite($this-&gt;logFileHandle, &quot;\\t\\tType\\n&quot;);
      $logEntry = &quot;\\t\\t\\tsupportsLinearRing(): &quot; . $this-&gt;prtBool($this-&gt;supportsLinearRing()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsLinearSegment(): &quot; . $this-&gt;prtBool($this-&gt;supportsLinearSegment()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsArcSegment(): &quot; . $this-&gt;prtBool($this-&gt;supportsArcSegment()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\t\\t\\tsupportsCurveRing(): &quot; . $this-&gt;prtBool($this-&gt;supportsCurveRing()) . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
      $logEntry = &quot;\\tdimensionality(): &quot; . $this-&gt;getDimensionality() . &quot;\\n&quot;;
      fwrite($this-&gt;logFileHandle, $logEntry);
   }
}
</pre>
**/ 