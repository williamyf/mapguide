package org.osgeo.mapguide.test.mapguide.operations.admin;

import java.util.*;
import com.almworks.sqlite4java.*;
import org.osgeo.mapguide.*;
import org.osgeo.mapguide.test.*;
import org.osgeo.mapguide.test.common.*;

public class Offline extends ServerAdminOperationExecutor
{
    public Offline(MgServerAdmin admin, String unitTestVm)
    {
        super("Offline", admin, unitTestVm);
    }

    public TestResult Execute(int paramSetId)
    {
        try
        {
            _serverAdmin.takeOffline();
            return TestResult.Boolean(_serverAdmin.isOnline());
        }
        catch (MgException ex)
        {
            return TestResult.FromMgException(ex);
        }
    }
}