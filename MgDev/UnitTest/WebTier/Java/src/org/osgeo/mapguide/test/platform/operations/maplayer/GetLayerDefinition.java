package org.osgeo.mapguide.test.platform.operations.maplayer;

import java.util.*;
import com.almworks.sqlite4java.*;
import org.osgeo.mapguide.*;
import org.osgeo.mapguide.test.*;
import org.osgeo.mapguide.test.common.*;
import org.osgeo.mapguide.test.operations.*;
import org.osgeo.mapguide.test.platform.*;

public class GetLayerDefinition extends MapLayerOperationExecutor
{
    public GetLayerDefinition(MgResourceService resSvc, String vm, IMapCreator creator)
    {
        super("GetLayerDefinition", resSvc, vm, creator);
    }

    public TestResult Execute(int paramSetId)
    {
        try
        {
            this.CreateMapFromResource(paramSetId);

            SQLiteStatement stmt = _conn.prepare(String.format("Select ParamValue from Params WHERE ParamSet=%d AND ParamName=\"LAYERNAME\"", paramSetId));
            if (stmt.step()) {
                String layerName = CommonUtility.GetStringFromStatement(stmt, "ParamValue");

                MgLayerBase layer = this.FindLayer(layerName);
                if (layer == null)
                    return TestResult.String("***Unknown Layer***");
                MgResourceIdentifier ldfId = layer.getLayerDefinition();
                return TestResult.String(ldfId.toString());
            }
            throw new RuntimeException("No layer param found for param set: " + paramSetId);
        }
        catch (MgException ex)
        {
            return TestResult.FromMgException(ex);
        }
        catch (Exception ex)
        {
            return TestResult.FromException(ex);
        }
    }
}