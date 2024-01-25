/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/burst/TipcBurstFilterFuncInterface.java#1 $
 */

package com.smartsockets.burst;

import java.io.*;
import com.smartsockets.*;

  /**
   * The user interface for filtering component msg in a burst msg
   */

public interface TipcBurstFilterFuncInterface {
  /**
   * Called when a message is disassembled and passed to user 
   * so he can look at at it and decide if he needs it or not.	
   * if the filter_func returns true then the message is processed 
   * or else destroyed.
   *
   * @param msg the message that was read and passed
   * @param arg a user-defined argument set when the filter was created
   * @return true on success, false on failure
   * @since SmartSockets 6.2
   */
  boolean filter_func(TipcMsg msg, Object arg);
}

