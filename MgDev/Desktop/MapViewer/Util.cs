﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using System.Drawing;

namespace OSGeo.MapGuide.Viewer
{
    internal static class Util
    {
        public static string MakeWktCircle(double x, double y, double r)
        {
            return "CURVEPOLYGON ((" + (x - r).ToString(CultureInfo.InvariantCulture) + " " + y.ToString(CultureInfo.InvariantCulture) + " (CIRCULARARCSEGMENT (" + x.ToString(CultureInfo.InvariantCulture) + " " + (y - r).ToString(CultureInfo.InvariantCulture) + ", " + (x + r).ToString(CultureInfo.InvariantCulture) + " " + y.ToString(CultureInfo.InvariantCulture) + "), CIRCULARARCSEGMENT (" + x.ToString(CultureInfo.InvariantCulture) + " " + (y + r).ToString(CultureInfo.InvariantCulture) + ", " + (x - r).ToString(CultureInfo.InvariantCulture) + " " + y.ToString(CultureInfo.InvariantCulture) + "))))"; //NOXLATE
        }

        public static string MakeWktPolygon(double x1, double y1, double x2, double y2)
        {
            string x1str = x1.ToString(CultureInfo.InvariantCulture);
            string y1str = y1.ToString(CultureInfo.InvariantCulture);
            string x2str = x2.ToString(CultureInfo.InvariantCulture);
            string y2str = y2.ToString(CultureInfo.InvariantCulture);
            return "POLYGON((" + x1str + " " + y1str + ", " + x2str + " " + y1str + ", " + x2str + " " + y2str + ", " + x1str + " " + y2str + ", " + x1str + " " + y1str + "))"; //NOXLATE
        }

        public static string ToHtmlColor(Color color)
        {
            return String.Format("{0:X2}{1:X2}{2:X2}", color.R, color.G, color.B); //NOXLATE
        }

        public static string ToHtmlColorWithAlpha(Color color)
        {
            return String.Format("{0:X2}{1:X2}{2:X2}{3:X2}", color.A, color.R, color.G, color.B); //NOXLATE
        }

        public static Color FromHtmlColor(string html)
        {
            int rgb = int.Parse(html, System.Globalization.NumberStyles.HexNumber);
            return Color.FromArgb(rgb);
        }

        public static MgLayerBase FindLayer(MgLayerCollection layers, String layerName)
        {
            MgLayerBase layer = null;
            int i = 0;
            for (i = 0; i < layers.GetCount(); i++)
            {
                MgLayerBase layer1 = layers.GetItem(i);

                if (layer1.GetName() == layerName)
                {
                    layer = layer1;
                    break;
                }
            }
            return layer;
        }

        public static string GetMapSrs(MgMapBase map)
        {
            try
            {
                String srs = map.GetMapSRS();
                if (srs != string.Empty)
                    return srs;
            }
            catch (MgException e)
            {
            }

            //No SRS, set to ArbitrayXY meters
            //
            return "LOCALCS[\"Non-Earth (Meter)\",LOCAL_DATUM[\"Local Datum\",0],UNIT[\"Meter\", 1],AXIS[\"X\",EAST],AXIS[\"Y\",NORTH]]"; //NOXLATE
        }

        public static void ClearDataSource(MgFeatureService featSvc, MgResourceIdentifier fsId, String featureName)
        {
            MgDeleteFeatures deleteCmd = new MgDeleteFeatures(featureName, "ID >= 0"); //NOXLATE
            MgFeatureCommandCollection commands = new MgFeatureCommandCollection();
            commands.Add(deleteCmd);
            featSvc.UpdateFeatures(fsId, commands, false);
        }

        public static void ReleaseReader(MgPropertyCollection res, MgFeatureCommandCollection commands)
        {
            if (res == null)
                return;

            for (int i = 0; i < res.GetCount(); i++)
            {
                MgFeatureCommand cmd = commands.GetItem(i);
                if (cmd is MgInsertFeatures)
                {
                    MgFeatureProperty resProp = res.GetItem(i) as MgFeatureProperty;
                    if (resProp != null)
                    {
                        MgFeatureReader reader = resProp.GetValue() as MgFeatureReader;
                        if (reader == null)
                            return;
                        reader.Close();
                    }
                }
            }
        }

        public static void AddFeatureToCollection(MgBatchPropertyCollection propCollection, MgAgfReaderWriter agfRW, int featureId, MgGeometry featureGeom)
        {
            MgPropertyCollection bufferProps = new MgPropertyCollection();
            MgInt32Property idProp = new MgInt32Property("ID", featureId); //NOXLATE
            bufferProps.Add(idProp);
            MgByteReader geomReader = agfRW.Write(featureGeom);
            MgGeometryProperty geomProp = new MgGeometryProperty("GEOM", geomReader); //NOXLATE
            bufferProps.Add(geomProp);
            propCollection.Add(bufferProps);
        }

        public static String GetFeaturePropertyValue(MgReader featureReader, String propName)
        {
            String value = string.Empty;
            int propertyType = featureReader.GetPropertyType(propName);
            switch (propertyType)
            {
                case MgPropertyType.Boolean:
                    value = featureReader.GetBoolean(propName).ToString();
                    break;
                case MgPropertyType.Byte:
                    value = featureReader.GetByte(propName).ToString();
                    break;
                case MgPropertyType.Single:
                    value = featureReader.GetSingle(propName).ToString();
                    break;
                case MgPropertyType.Double:
                    value = featureReader.GetDouble(propName).ToString();
                    break;
                case MgPropertyType.Int16:
                    value = featureReader.GetInt16(propName).ToString();
                    break;
                case MgPropertyType.Int32:
                    value = featureReader.GetInt32(propName).ToString();
                    break;
                case MgPropertyType.Int64:
                    value = featureReader.GetInt64(propName).ToString();
                    break;
                case MgPropertyType.String:
                    value = featureReader.GetString(propName);
                    break;
                case MgPropertyType.DateTime:
                case MgPropertyType.Null:
                case MgPropertyType.Blob:
                case MgPropertyType.Clob:
                case MgPropertyType.Feature:
                case MgPropertyType.Geometry:
                case MgPropertyType.Raster:
                    value = Strings.StrUnsupportedDataType; //NOXLATE
                    break;
            }
            return value;
        }

        public static String MakeUniqueLayerName(MgMapBase map, String layerName, String themeName)
        {
            String desiredName = "_" + layerName + themeName; //NOXLATE
            String uniqueName = desiredName;
            int index = 1;

            var layers = map.GetLayers();
            while (layers.Contains(uniqueName))
            {
                uniqueName = desiredName + index.ToString();
                index++;
            }
            return uniqueName;
        }

        public static String InterpolateColor(double portion, String startColor, String endColor, int percentTransparent)
        {
            int alpha = (int)(255 * (100.0 - percentTransparent) / 100.0);
            String result = string.Empty;
            if (startColor.Equals(endColor))
            {
                result = String.Format("{0:X2}{1}", alpha, startColor); //NOXLATE
            }
            else
            {
                int red = CalculateRGB(portion, startColor.Substring(0, 2), endColor.Substring(0, 2));
                int green = CalculateRGB(portion, startColor.Substring(2, 2), endColor.Substring(2, 2));
                int blue = CalculateRGB(portion, startColor.Substring(4, 2), endColor.Substring(4, 2));
                result = String.Format("{0:X2}{1:X2}{2:X2}{3:X2}", alpha, red, green, blue); //NOXLATE
            }
            return result;
        }

        public static int CalculateRGB(double portion, String startRGB, String endRGB)
        {
            double result = Int32.Parse(startRGB, NumberStyles.HexNumber) + portion * (Int32.Parse(endRGB, NumberStyles.HexNumber) - Int32.Parse(startRGB, NumberStyles.HexNumber));
            return (int)result;
        }
    }
}
