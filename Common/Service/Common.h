//
//  Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifndef MGCOMMON_H_
#define MGCOMMON_H_

class ACE_SOCK_Stream;

#include "System/ClassId.h"
#include "System/CommonDefs.h"

#include "System/Object.h"
#include "System/Ptr.h"
#include "System/Disposable.h"
#include "System/GuardDisposable.h"
#include "System/Serializable.h"
#include "System/NamedSerializable.h"
#include "System/CryptographyManager.h"

#include "Data/Collection.h"
#include "Data/StringCollection.h"
#include "Data/IntCollection.h"

#include "System/ConfigProperties.h"
#include "System/UserInformation.h"
#include "Util/Byte.h"
#include "Data/MimeType.h"
#include "Data/ByteReader.h"
#include "Data/ByteSourceImpl.h"
#include "Data/ByteSource.h"
#include "Data/ByteSink.h"

#include "Data/Color.h"
#include "Data/DateTime.h"

#include "Data/Property.h"
#include "Data/NullableProperty.h"
#include "Data/PropertyDefinition.h"
#include "Data/BlobProperty.h"
#include "Data/BooleanProperty.h"
#include "Data/ByteProperty.h"
#include "Data/ClobProperty.h"
#include "Data/DateTimeProperty.h"
#include "Data/DoubleProperty.h"
#include "Data/Int16Property.h"
#include "Data/Int32Property.h"
#include "Data/Int64Property.h"
#include "Data/SingleProperty.h"
#include "Data/StringProperty.h"
#include "Data/FeaturePropertyType.h"

#include "Data/DisposableCollection.h"
#include "Data/NamedCollection.h"
#include "Data/PropertyCollection.h"
#include "Data/StringPropertyCollection.h"
#include "Data/PropertyDefinitionCollection.h"
#include "Data/BatchPropertyCollection.h"
#include "Data/SerializableCollection.h"
#include "Data/ConfigurationSectionCollection.h"

#include "../MdfModel/UnicodeString.h"
#include "Util/IpUtil.h"
#include "Util/FileUtil.h"
#include "Util/Util.h"

#include "Exception/ExceptionDefs.h"
#include "Exception/Exception.h"
#include "Exception/ApplicationException.h"
#include "Exception/SystemException.h"
#include "Exception/ThirdPartyException.h"
#include "Exception/IoException.h"
#include "Exception/FileIoException.h"
#include "Exception/StreamIoException.h"
#include "Exception/OutOfRangeException.h"

#include "Exception/ArgumentOutOfRangeException.h"
#include "Exception/ArrayTypeMismatchException.h"
#include "Exception/AuthenticationFailedException.h"
#include "Exception/ClassNotFoundException.h"
#include "Exception/ConfigurationException.h"
#include "Exception/ConfigurationLoadFailedException.h"
#include "Exception/ConfigurationSaveFailedException.h"
#include "Exception/ConnectionFailedException.h"
#include "Exception/ConnectionNotOpenException.h"
#include "Exception/CoordinateSystemComputationFailedException.h"
#include "Exception/CoordinateSystemConversionFailedException.h"
#include "Exception/CoordinateSystemInitializationFailedException.h"
#include "Exception/CoordinateSystemMeasureFailedException.h"
#include "Exception/CoordinateSystemTransformFailedException.h"
#include "Exception/DateTimeException.h"
#include "Exception/DbException.h"
#include "Exception/DbXmlException.h"
#include "Exception/DecryptionException.h"
#include "Exception/DirectoryNotFoundException.h"
#include "Exception/DivideByZeroException.h"
#include "Exception/DomainException.h"
#include "Exception/DuplicateDirectoryException.h"
#include "Exception/DuplicateFileException.h"
#include "Exception/DuplicateGroupException.h"
#include "Exception/DuplicateNameException.h"
#include "Exception/DuplicateObjectException.h"
#include "Exception/DuplicateParameterException.h"
#include "Exception/DuplicateRepositoryException.h"
#include "Exception/DuplicateResourceDataException.h"
#include "Exception/DuplicateResourceException.h"
#include "Exception/DuplicateRoleException.h"
#include "Exception/DuplicateServerException.h"
#include "Exception/DuplicateSessionException.h"
#include "Exception/DuplicateUserException.h"
#include "Exception/DwfException.h"
#include "Exception/DwfSectionNotFoundException.h"
#include "Exception/DwfSectionResourceNotFoundException.h"
#include "Exception/EmptyFeatureSetException.h"
#include "Exception/EncryptionException.h"
#include "Exception/EndOfStreamException.h"
#include "Exception/EvaluationExpiredException.h"
#include "Exception/FdoException.h"
#include "Exception/FeatureServiceException.h"
#include "Exception/FileNotFoundException.h"
#include "Exception/GeometryException.h"
#include "Exception/GroupNotFoundException.h"
#include "Exception/IndexOutOfRangeException.h"
#include "Exception/InvalidArgumentException.h"
#include "Exception/InvalidCastException.h"
#include "Exception/InvalidCoordinateSystemException.h"
#include "Exception/InvalidCoordinateSystemTypeException.h"
#include "Exception/InvalidCoordinateSystemUnitsException.h"
#include "Exception/InvalidDwfPackageException.h"
#include "Exception/InvalidDwfSectionException.h"
#include "Exception/InvalidFeatureSourceException.h"
#include "Exception/InvalidIpAddressException.h"
#include "Exception/InvalidLicenseException.h"
#include "Exception/InvalidLogEntryException.h"
#include "Exception/InvalidMapDefinitionException.h"
#include "Exception/InvalidMimeTypeException.h"
#include "Exception/InvalidOperationException.h"
#include "Exception/InvalidOperationVersionException.h"
#include "Exception/InvalidPasswordException.h"
#include "Exception/InvalidPrintLayoutFontSizeUnitsException.h"
#include "Exception/InvalidPrintLayoutPositionUnitsException.h"
#include "Exception/InvalidPrintLayoutSizeUnitsException.h"
#include "Exception/InvalidPropertyTypeException.h"
#include "Exception/InvalidRepositoryNameException.h"
#include "Exception/InvalidRepositoryTypeException.h"
#include "Exception/InvalidResourceDataNameException.h"
#include "Exception/InvalidResourceDataTypeException.h"
#include "Exception/InvalidResourceNameException.h"
#include "Exception/InvalidResourcePathException.h"
#include "Exception/InvalidResourcePreProcessingTypeException.h"
#include "Exception/InvalidResourceTypeException.h"
#include "Exception/InvalidServerNameException.h"
#include "Exception/InvalidSerialNumberException.h"
#include "Exception/InvalidStreamHeaderException.h"
#include "Exception/LayerNotFoundException.h"
#include "Exception/LengthException.h"
#include "Exception/LicenseException.h"
#include "Exception/LicenseExpiredException.h"
#include "Exception/LogicException.h"
#include "Exception/LogOpenFailedException.h"
#include "Exception/NotFiniteNumberException.h"
#include "Exception/NotImplementedException.h"
#include "Exception/NullArgumentException.h"
#include "Exception/NullPropertyValueException.h"
#include "Exception/NullReferenceException.h"
#include "Exception/ObjectNotFoundException.h"
#include "Exception/OperationProcessingException.h"
#include "Exception/OutOfMemoryException.h"
#include "Exception/OverflowException.h"
#include "Exception/ParameterNotFoundException.h"
#include "Exception/PathTooLongException.h"
#include "Exception/PermissionDeniedException.h"
#include "Exception/PlatformNotSupportedException.h"
#include "Exception/PortNotAvailableException.h"
#include "Exception/PrintToScaleModeNotSelectedException.h"
#include "Exception/RepositoryCreationFailedException.h"
#include "Exception/RepositoryNotFoundException.h"
#include "Exception/RepositoryNotOpenException.h"
#include "Exception/RepositoryOpenFailedException.h"
#include "Exception/ResourceBusyException.h"
#include "Exception/ResourceDataNotFoundException.h"
#include "Exception/ResourceNotFoundException.h"
#include "Exception/ResourcesException.h"
#include "Exception/ResourcesLoadFailedException.h"
#include "Exception/ResourceTagNotFoundException.h"
#include "Exception/RoleNotFoundException.h"
#include "Exception/RuntimeException.h"
#include "Exception/ServerNotFoundException.h"
#include "Exception/ServerNotOnlineException.h"
#include "Exception/ServiceNotAvailableException.h"
#include "Exception/ServiceNotSupportedException.h"
#include "Exception/SessionExpiredException.h"
#include "Exception/TemporaryFileNotAvailableException.h"
#include "Exception/UnauthorizedAccessException.h"
#include "Exception/UnclassifiedException.h"
#include "Exception/UnderflowException.h"
#include "Exception/UriFormatException.h"
#include "Exception/UserNotFoundException.h"
#include "Exception/XmlException.h"
#include "Exception/XmlParserException.h"

#include "System/Warnings.h"

#include "Services/BaseService.h"
#include "Services/BaseServiceDefs.h"

#include "Geometry/GeometryCommon.h"

#include "Services/ResourceService.h"

#include "Services/DrawingService.h"
#include "Services/FeatureService.h"
#include "Services/FeatureInformation.h"
#include "Services/FeatureProperty.h"
#include "Services/ImageFormats.h"

#include "MapLayer/LayerType.h"
#include "MapLayer/LayerGroupType.h"
#include "MapLayer/Map.h"

#include "Services/MappingService.h"
#include "Services/RenderingService.h"
#include "Services/TileService.h"

#include "Services/ProxyDrawingService.h"
#include "Services/ProxyFeatureService.h"
#include "Services/ProxyMappingService.h"
#include "Services/ProxyRenderingService.h"
#include "Services/ProxyResourceService.h"
#include "Services/ProxyTileService.h"

#include "Controller/Controller.h"
#include "Controller/DwfController.h"
#include "Controller/HtmlController.h"

#include "Services/Site.h"
#include "Services/ServerAdmin.h"

#include "CoordinateSystem/CoordinateSystem.h"
#include "CoordinateSystem/CoordinateSystemFactory.h"
#include "CoordinateSystem/CoordinateSystemMeasure.h"
#include "CoordinateSystem/CoordinateSystemTransform.h"
#include "CoordinateSystem/CoordinateSystemType.h"

#endif // MGCOMMON_H_
