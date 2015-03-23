package org.osgeo.mapguide.test.mapguide.operations.weblayout;

import java.util.*;
import com.almworks.sqlite4java.*;
import org.osgeo.mapguide.*;
import org.osgeo.mapguide.test.*;
import org.osgeo.mapguide.test.common.*;
import org.osgeo.mapguide.test.operations.*;

public class WL_GetMapDefinition extends WebLayoutOperationExecutor
{
    public WL_GetMapDefinition(IWebLayoutCreator layout, String unitTestVm)
    {
        super("WL_GetMapDefinition", layout, unitTestVm);
    }

    public TestResult Execute(int paramSetId)
    {
        try
        {
            CreateWebLayoutFromResource(paramSetId);
            return TestResult.String(_wl.getMapDefinition());
        }
        catch (MgException ex)
        {
            return TestResult.FromMgException(ex);
        }
    }
}