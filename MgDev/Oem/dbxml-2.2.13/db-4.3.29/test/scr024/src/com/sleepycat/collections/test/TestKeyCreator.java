/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2002-2004
 *	Sleepycat Software.  All rights reserved.
 *
 * $Id: TestKeyCreator.java,v 1.2 2004/06/04 18:26:02 mark Exp $
 */

package com.sleepycat.collections.test;

import com.sleepycat.bind.RecordNumberBinding;
import com.sleepycat.db.DatabaseEntry;
import com.sleepycat.db.DatabaseException;
import com.sleepycat.db.SecondaryDatabase;
import com.sleepycat.db.SecondaryKeyCreator;

/**
 * Unused until secondaries are available.
 * @author Mark Hayes
 */
class TestKeyCreator implements SecondaryKeyCreator {

    private boolean isRecNum;

    TestKeyCreator(boolean isRecNum) {

        this.isRecNum = isRecNum;
    }

    public boolean createSecondaryKey(SecondaryDatabase db,
                                      DatabaseEntry primaryKeyData,
                                      DatabaseEntry valueData,
                                      DatabaseEntry indexKeyData)
        throws DatabaseException {

        if (valueData.getSize() == 0) {
            return false;
        }
        if (valueData.getSize() != 1) {
            throw new IllegalStateException();
        }
        byte val = valueData.getData()[valueData.getOffset()];
        if (val == 0) {
            return false; // fixed-len pad value
        }
        val -= 100;
        if (isRecNum) {
            RecordNumberBinding.recordNumberToEntry(val, indexKeyData);
        } else {
            indexKeyData.setData(new byte[] { val }, 0, 1);
        }
        return true;
    }

    public void clearIndexKey(DatabaseEntry valueData) {

        throw new RuntimeException("not supported");
    }
}
