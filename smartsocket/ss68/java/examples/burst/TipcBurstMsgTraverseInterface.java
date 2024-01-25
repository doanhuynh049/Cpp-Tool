/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/burst/TipcBurstMsgTraverseInterface.java#1 $
 */

package com.smartsockets.burst;

import java.io.*;
import com.smartsockets.*;

  /**
   * The user interface that gets invoked when the burst msg in traversed.
   */
public interface TipcBurstMsgTraverseInterface {
  /**
   * Called when a burst message is traversed. When the user invokes
   * TipcBurst.msgTraverse he passes in an object of type 
   * TipcBursMsgTraverseInterface. For Each component message in the Burst 
   * Message the traversal function is invoked. 
   *
   * @param msg the component message of the Burst message 
   * @param arg a user-defined argument set when msgTraverse is called 
   * @return true on success, false on failure
   * @since SmartSockets 6.2
   */
  boolean burstMsgTraverse(TipcMsg msg, Object arg);
}

