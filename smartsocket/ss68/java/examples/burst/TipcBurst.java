/*
 * Copyright (c) 1997-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/examples/ss/dev/68/src/java/burst/TipcBurst.java#1 $
 */

package com.smartsockets.burst;

import java.io.IOException;
import java.util.Vector;
import java.util.Enumeration;
import java.util.Hashtable;
import com.smartsockets.*;

/**
 * <p>
 * Provides an interface to coalesce/burst a set of msgs destined to a 
 * subject there by reducing the message header overhead.
 * Messages destined to different subjects ar automatically segregated
 * into different bursts internally.
 * </p>
 * <p>
 * 
 * A simple example:
 * <pre>
 *   // get the single, global RTserver connection instance 
 *   TipcSrv srv = TipcSvc.getSrv();
 *
 *   // create a burst of size 100, 
 *   TipcBurst burst = new TipcBurst(100);
 *
 *   // create and send a message
 *   TipcMsg msg = TipcSvc.createMsg(TipcMt.INFO);
 *   msg.setDest("/foo");
 *   msg.appendStr("hello");
 *   // burst.msgsend coalesces these message into a single burst
 *   // message, there by reducing the overhead of message headers 
 *   // of 100 msgs into one message header of one burst message.
 *   for (int i=0; i<100; i++) {
 *     burst.msgsend(msg);
 *   }
 *   burst.flush();
 *   srv.flush();
 *
 * </pre>
 * </p>
 * <p>
 * In this example, the msgsend method automatically  cooleasces 
 * the 100 msgs into one BURST msg. Only the relevant
 * header information from each of the component msgs in kept and the 
 * entire data portion of each message is kept in the burst message.
 * </p>
 * @see TipcSvc
 * @see TipcSrv
 * @see TipcMsg
 */



public class TipcBurst {

  static final int BURST_INFO_FIELD_SIZE = 3;
  static boolean initialized = false;  
  static boolean already_added = false;
  static final int BURST = 9000;


  /* This is how TipcBurst API is setup.
   * 
   * When user calls TipcBurst burst = new TipcBurst(size) the MAIN
   * BURST gets created which hold a hash table.
   *
   * When the user calls burst.msgSend(msg), the msg destination is
   * pulled out, the main burst hashtable is checked to see if the 
   * subject name exists, if it doesn't then a new internal burst 
   * (new_burst) is created and the new_burst's msg destination is 
   * set to this subject and the minimal information about the message 
   * and it fieldsare stripped out and appended to new_burst.info_field_array
   * new_burst.msg respectively.
   * When another message comes in, it's subject is checked for in 
   * the hashtable and if the subject exits it's respective burst 
   * is pulled out and the message is appended to it's own burst
   * else a new_burst is created each time a message with different
   * subject comes in.(BM 05/11/98)
   */

  /* Internal class which does all the work of creating bursts
   * on a per-subject basis and also does all the message
   * coalescing. Also for internal burst's we do not create
   * the hashtable since it would not hold anything.
   */

  class ipcBurstInternal {


    private TipcMsg  msg;
    private int      info_field_array[] = null;
    private int      info_field_size;
    private int      auto_flush_count; 
    private TipcMt   burst_mt;

    ////////////////////////////////////////////////////
    // ..ipcBurstInternal Constructor

    /**
     * Constructs a internal burst, none of the data structures are
     * initialized, this is basically provided so when called from
     * TipcBurst's constructor it can just be used to initMsgTypes
     */

    ipcBurstInternal() {

    }

    ////////////////////////////////////////////////////
    // ..initMsgTypes

    /**7
     * create the message type MT_BURST to hold the the
     * entire burst of messages
     */

    void initMsgTypes() {

      if (false == initialized) {
        burst_mt = null;
        try {
          burst_mt = TipcSvc.createMt("burst", BURST, "verbose");
        }
        catch (TipcException tipce) {
          Tut.fatal(tipce);
        }

        if (burst_mt == null) {
          initialized = false;
        }
      }
      initialized = true;
    }

    ////////////////////////////////////////////////////
    // ..ipcBurstInternal Constructor

    /** Constructor which creates the required members and sets
     * the auto auto_flush_count 
     */

    ipcBurstInternal(int auto_flush_count) throws TipcException {
      TipcSrv srv = null;
      srv = TipcSvc.getSrv();
      /*
       * init the message types
       */
      initMsgTypes();
      this.msg = null;
      TipcMt mt = TipcSvc.lookupMt(BURST);

      msg = TipcSvc.createMsg(mt);
      setAutoFlushCountInternal(srv, auto_flush_count);

      info_field_size = 0;
    }

   
    ////////////////////////////////////////////////////
    // ..ipcBurstInternal Constructor

    /** Constructor which create the required members and sets
     * the auto auto_flush_count 
     */

    ipcBurstInternal(TipcSrv srv, int auto_flush_count) throws TipcException {
      
      /*
       * init the message types
       */
      initMsgTypes();
      this.msg = null;
      TipcMt mt = TipcSvc.lookupMt(BURST);

      msg = TipcSvc.createMsg(mt);
      setAutoFlushCountInternal(srv, auto_flush_count);

      info_field_size = 0;

    }


    ////////////////////////////////////////////////////
    // ..ipcBurstAppendMsg

    boolean ipcBurstAppendMsg(TipcMsg msg) throws TipcException {
      boolean status;
      TipcMsg destMsg = null;
      destMsg = this.msg;     

      info_field_size += BURST_INFO_FIELD_SIZE;

      /* add the minimal info to burst and append msg fields
       * to burst.msg
       */

      status = ipcBurstAppendMsgHeader(msg);
      status &= ipcBurstAppendAllMsgFields(destMsg, msg);

      return status;

    }

    ////////////////////////////////////////////////////
    // ..ipcBurstAppendMsgHeader

    boolean ipcBurstAppendMsgHeader(TipcMsg msg) {

      int msg_type;
      int number_of_fields;
      int user_prop;
      boolean status;
      int burst_msg_info_start_index = 0;
      TipcMt mt;

      mt = msg.getType();
      Tut.affirm(mt != null);

      msg_type = mt.getNum();
      number_of_fields = msg.getNumFields();

      user_prop = msg.getUserProp();

      burst_msg_info_start_index =
      info_field_size - BURST_INFO_FIELD_SIZE;

      info_field_array[burst_msg_info_start_index + 0] = msg_type;
      info_field_array[burst_msg_info_start_index + 1] = number_of_fields;
      info_field_array[burst_msg_info_start_index + 2] = user_prop;

      return true;

    } 

    ////////////////////////////////////////////////////
    // ..ipcBurstAppendAllMsgFields

    boolean ipcBurstAppendAllMsgFields(TipcMsg destMsg,
                                       TipcMsg srcMsg) throws TipcException {

      int num_fields_of_src_msg;
      int counter;
      boolean status;

      num_fields_of_src_msg = srcMsg.getNumFields();
      srcMsg.setCurrent(0);

      status = ipcBurstAppendMsgFields(destMsg, srcMsg, 0,
                                       num_fields_of_src_msg);
      return status;

    }

    ////////////////////////////////////////////////////
    // ..ipcBurstAppendMsgFields

    boolean ipcBurstAppendMsgFields(TipcMsg destMsg,
                                    TipcMsg srcMsg, 
                                    int startingFieldIndex,
                                    int numOfFields) throws TipcException {

      int num_fields_of_src_msg;
      boolean status = true;
      int counter = 0;

      num_fields_of_src_msg = srcMsg.getNumFields();
      srcMsg.setCurrent(startingFieldIndex);


      for (counter=0; counter < numOfFields; counter++) {
        int fieldType = 0;
        fieldType = srcMsg.nextType();

        switch (fieldType) {
          
          case TipcMsg.FT_INVALID:
            return false;

          case TipcMsg.FT_CHAR:
            {
              char ch= '\0';
              ch = srcMsg.nextChar();
              destMsg.appendChar(ch);
            }
            break;
          case TipcMsg.FT_INT2:
            {
              short int2 = 0;
              int2 = srcMsg.nextInt2();
              destMsg.appendInt2(int2);
            }
            break;

          case TipcMsg.FT_INT4:
            {
              int int4 = 0;
              int4 = srcMsg.nextInt4();
              destMsg.appendInt4(int4);
            }
            break;

          case TipcMsg.FT_INT8:
            {
              long int8 = 0;
              int8 = srcMsg.nextInt8();
              destMsg.appendInt8(int8);
            }
            break;

          case TipcMsg.FT_REAL4:
            {
              float real4 = 0;
              real4 = srcMsg.nextReal4();
              destMsg.appendReal4(real4);
            }
            break;

          case TipcMsg.FT_REAL8:
            {
              double real8 = 0.0;
              real8 = srcMsg.nextReal8();
              destMsg.appendReal8(real8);
            }
            break;

          case TipcMsg.FT_STR:
            {
              String str= null;
              str = srcMsg.nextStr();
              destMsg.appendStr(str);
            }
            break;

          case TipcMsg.FT_MSG:
            {
              TipcMsg msg= null;
              msg = srcMsg.nextMsg();
              destMsg.appendMsg(msg);
            }
            break;

          case TipcMsg.FT_INT2_ARRAY:
            {
              short[] int2_array= null;
              int2_array = srcMsg.nextInt2Array();
              destMsg.appendInt2Array(int2_array);
            }
            break;

          case TipcMsg.FT_INT4_ARRAY:
            {
              int[] int4_array = null;
              int4_array = srcMsg.nextInt4Array();
              destMsg.appendInt4Array(int4_array);
            }
            break;

          case TipcMsg.FT_INT8_ARRAY:
            {
              long[] int8_array= null;
              int8_array = srcMsg.nextInt8Array();
              destMsg.appendInt8Array(int8_array);
            }
            break;

          case TipcMsg.FT_REAL4_ARRAY:
            {
              float[] real4_array= null;
              real4_array = srcMsg.nextReal4Array();
              destMsg.appendReal4Array(real4_array);
            }
            break;

          case TipcMsg.FT_REAL8_ARRAY:
            {
              double[] real8_array= null;
              real8_array = srcMsg.nextReal8Array();
              destMsg.appendReal8Array(real8_array);
            }
            break;

          case TipcMsg.FT_BINARY:
            {
              byte[] binary = null;
              binary = srcMsg.nextBinary();
              destMsg.appendBinary(binary);
            }
            break;

          case TipcMsg.FT_STR_ARRAY:
            {
              String[] str_array = null;
              str_array = srcMsg.nextStrArray();
              destMsg.appendStrArray(str_array);
            }
            break;

          case TipcMsg.FT_MSG_ARRAY:
            {
              TipcMsg msg_array[] = null;
              msg_array = srcMsg.nextMsgArray();
              destMsg.appendMsgArray(msg_array);
            }
            break;
          case TipcMsg.FT_GUID:
            {
              byte[] guid = null;
              guid = srcMsg.nextGuid();
              destMsg.appendGuid(guid, guid.length);
            }
            break;
          case TipcMsg.FT_GUID_ARRAY:
            {
              byte[][] guid_array = null;
              guid_array = srcMsg.nextGuidArray();
              destMsg.appendGuidArray(guid_array, 0, guid_array.length);
            }
            break;
          case TipcMsg.FT_XML:
            {
              TutXml xml = null;
              xml = srcMsg.nextXml();
              destMsg.appendXml(xml);
            }
            break;
          case TipcMsg.FT_UTF8:
            {
              String utf8_str = null;
              utf8_str = srcMsg.nextUtf8();
              destMsg.appendUtf8(utf8_str);
            }
            break;
          case TipcMsg.FT_UTF8_ARRAY:
            {
              String[] utf8_array = null;
              utf8_array = srcMsg.nextUtf8Array();
              destMsg.appendUtf8Array(utf8_array);
            }
            break;
          case TipcMsg.FT_BOOL:
            {
              boolean val = false;
              val = srcMsg.nextBool();
              destMsg.appendBool(val);
            }
            break;
          case TipcMsg.FT_BOOL_ARRAY:
            {
              boolean[] bool_array = null;
              bool_array = srcMsg.nextBoolArray();
              destMsg.appendBoolArray(bool_array);
            }
            break;
          case TipcMsg.FT_BYTE:
            {
              byte b = (byte)'\0';
              b = srcMsg.nextByte();
              destMsg.appendByte(b);
            }
            break;
          default:
            {
              Tut.warning("Unsupported Field Type");
              status = false;
            }
        } 
      } 

      return status;

    } 

    ////////////////////////////////////////////////////
    // ..setAutoFlushCountInternal

    boolean setAutoFlushCountInternal(TipcSrv srv, 
                                      int auto_flush_count) throws TipcException {


      if (auto_flush_count <=  0) {
        return false;
      }
      /* if the size is less than existing size then 
       * flush it so user would not loose any messages
       */
      if (this.info_field_array == null) {
        this.auto_flush_count = auto_flush_count;
        this.info_field_array = new int[auto_flush_count *
                                        BURST_INFO_FIELD_SIZE];
      }
      /* If the info_field_array is not null which would mean that
       * the user called it after creating the burst
       * So if the size is smaller than initially specified
       * flush the messages out or else it would result in 
       * a loss. if size is greater than initial size
       * create a temporary array and copy everything
       * uptil now to this temp array and back to info_field_array
       */

      else {
        if (auto_flush_count <  this.auto_flush_count) {

          this.flushInternal(srv);
          this.info_field_array = new int[auto_flush_count
                                          * BURST_INFO_FIELD_SIZE];
          this.auto_flush_count = auto_flush_count;
        }         

        else if (auto_flush_count > this.auto_flush_count) {
          int[] temp_array = new int[auto_flush_count *
                                     BURST_INFO_FIELD_SIZE];

          System.arraycopy(this.info_field_array, 0, temp_array, 0,
                           this.info_field_array.length);
          this.info_field_array = new int[auto_flush_count *
                                          BURST_INFO_FIELD_SIZE];
          System.arraycopy(temp_array, 0, this.info_field_array, 0,
                           temp_array.length);

          this.auto_flush_count = auto_flush_count;
        }
      } 
      return true;

    } 


    ////////////////////////////////////////////////////
    // ..getAutoFlushCountInternal

    int getAutoFlushCountInternal() {
      return this.auto_flush_count;
    }  

    ////////////////////////////////////////////////////
    // ..getCountInternal

    int getCountInternal() {
      int burst_count = 0;
      burst_count = this.info_field_size / BURST_INFO_FIELD_SIZE ;
      return burst_count;
    } 

    ////////////////////////////////////////////////////
    // ..flushInternal

    boolean flushInternal(TipcSrv srv) throws TipcException {

      boolean status;     
      
      msg.appendInt4Array(info_field_array, 0, info_field_size);

      /* send should automatically connect to RTserver */
      srv.send(this.msg);

      /* reuse messages; it is more efficient than create/destroy */
      msg.setNumFields(0);

      info_field_size = 0;

      return true;
    } 


    ////////////////////////////////////////////////////
    // ..destroyInternal

    boolean destroyInternal() {
      return true;
    } 
  } 

  /**
   * Class to hold the TipcBurstFilterFuncInterface type
   * object and Object arg in case the user wants to pass on any
   * other Object to the filter.
   */

  class filterClosure {
    TipcBurstFilterFuncInterface filter_func;
    Object arg;
    TipcSrv srv;

    filterClosure() {}
  } 


  ////////////////////////////////////////////////////

  /**
   * helper class to handle BURST MESSAGES 
   */

  class BurstProcessCb implements TipcProcessCb {
    public void process(TipcMsg msg, Object arg) {

      int[] info_field_array = null;
      int info_field_size;
      int num_of_bursted_msgs;
      String dest;
      String sender;
      int msg_fields_offset = 0;
      ipcBurstInternal burst = new ipcBurstInternal();
      boolean status;
      int counter;
      int num_field = 0;
      double sender_timestamp = 0.0;
      double arrival_timestamp = 0.0;
      boolean sender_timestamp_set = false;

      filterClosure closure = (filterClosure)arg;
      TipcBurstFilterFuncInterface test =
      (TipcBurstFilterFuncInterface)closure.filter_func;
      Object new_arg = (Object)closure.arg;
      TipcSrv srv = closure.srv;
      
      try {
        num_field = msg.getNumFields();
        
        msg.setCurrent(num_field - 1);
      }
      catch (Exception e) {
        Tut.fatal(e);
      }

      try {
        info_field_array  = msg.nextInt4Array();
      }
      catch (Exception e) {
        Tut.fatal(e);
      }
      try {
        sender_timestamp = msg.getSenderTimestamp();
        if (sender_timestamp > 0.0) {
          sender_timestamp_set = true;
          arrival_timestamp = msg.getArrivalTimestamp();
        }
      }
      catch (Exception e) {
        Tut.fatal(e);
      }

      info_field_size = info_field_array.length;

      num_of_bursted_msgs = info_field_size / BURST_INFO_FIELD_SIZE;

      dest = msg.getDest();
      Tut.affirm(dest != null);

      sender = msg.getSender();

      srv.getSubjectSubscribe(dest);

      /* set it back to zero so we can start extracting fields */
      try {
        msg.setCurrent(0);
      }
      catch (Exception sce) {
        Tut.fatal(sce);
      }
      for (counter=0; counter < num_of_bursted_msgs; counter++) {

        TipcMsg origMsg = null;
        int msg_type;
        int num_fields;
        int user_prop;

        msg_type = info_field_array[(counter * BURST_INFO_FIELD_SIZE) 
                                    + 0];
        num_fields = info_field_array[(counter * BURST_INFO_FIELD_SIZE)
                                      + 1];
        user_prop = info_field_array[(counter * BURST_INFO_FIELD_SIZE)
                                     + 2];
        /* Look up the msg type from the info_field_array and
         * create the message type
         */
        origMsg = TipcSvc.createMsg(TipcSvc.lookupMt(msg_type));

        if (origMsg == null) {
          continue;
        }

        origMsg.setDest(dest);
        origMsg.setSender(sender);
        try {
          if (true == sender_timestamp_set
              && sender_timestamp > 0.0
              && arrival_timestamp > 0.0) {
            origMsg.setSenderTimestamp(sender_timestamp);
            origMsg.setArrivalTimestamp(arrival_timestamp);
          }
          status = burst.ipcBurstAppendMsgFields(origMsg, msg,
                                                 msg_fields_offset,
                                                 num_fields);
          if (false == status) {
            return;
          }
        }
        catch (TipcException e ) {
          return;
        }
  
        origMsg.setUserProp(user_prop);

        /*
         * Now that we have pulled out num_fields from the message
         * move the offset so we can proceed to start getting from 
         * the new offset. (BM 05/17/98)
         */
        msg_fields_offset += num_fields;

        /*
         * If the user has passed a valid TipcBurstFilterFuncInterface 
         * type object which would mean that he has some kind of 
         * filtering setup so we call his filter_func and pass the 
         * message to him so he can look at it and decide if he needs
         * it or not. If he returns true then we process the message,
         * and if he returns false we destory the message.
         * If he has not filtering setup, i.e during initialization
         * if he passed init(null, null) then we process every-thing
         */

        if (test != null) {
          status = test.filter_func(origMsg, new_arg); 
          if (status == true) {
            /*
             * The user has provided a filter and he returns
             * true so process them before destroying
             */
            try {
              srv.process(origMsg);
            }
            catch (TipcException e) {
              Tut.warning(e);
            }
            try {
              origMsg.destroy();
            }
            catch (TipcException e) {
              Tut.warning(e);
            }
          }
          else {
            /*
             * The user has returned false, which would mean
             * that he has looked at the message and does 
             * not need it so go ahead and destroy it 
             */
            try {
              origMsg.destroy();
            }
            catch (TipcException e) {
              Tut.warning(e);
            }
          } 
        }
        else {
          /* The user has not provided a filter so process
           * all the messages
           */
          try {
            srv.process(origMsg);
            origMsg.destroy();
          }
          catch (TipcException e) {
            Tut.warning(e);
          }
        }
      }
    }
  }


  ///////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////


  private Hashtable ipc_burst_table = null;
  private int auto_flush_count;
  private int info_field_size;
  private TipcCb burstMsgDefaultCb = null;

  ////////////////////////////////////////////////////

  /** 
   * Default contructor, If the process in a receiver then
   * it will call this and then call burst.init(). A default burst 
   * auto_flush_count is set to 100. Just in case the process intends
   * to send messages to RTserver.
   * @since  SmartSockets 6.2         
   */

  public TipcBurst() {

    ipcBurstInternal internal_burst = new ipcBurstInternal();
    internal_burst.initMsgTypes();

    this.ipc_burst_table = new Hashtable();
    this.auto_flush_count = 100;  

  }

  ////////////////////////////////////////////////////

  /**
   * Constructor which takes an argument and sets up the necessary
   * data structures.
   * 
   * @param auto_flush_count indicating the size of the burst
   * @exception TipcBadArgumentException if negative value
   * @return the TipcBurst Object.
   * @since  SmartSockets 6.2         
   */
  public TipcBurst(int auto_flush_count) throws TipcBadArgumentException {
    /* 
     * provide the user a way to specify the size right away
     */
    if (auto_flush_count <= 0) {
      throw new TipcBadArgumentException("Invalid auto_flush_count should be greater than zero");
    }
    ipcBurstInternal internal_burst = new ipcBurstInternal();
    internal_burst.initMsgTypes();

    this.ipc_burst_table = new Hashtable();
    this.auto_flush_count = auto_flush_count;
  }


  boolean initInternal(TipcSrv srv, TipcBurstFilterFuncInterface obj, Object arg) {
    
    ipcBurstInternal burst = new ipcBurstInternal();
    burst.initMsgTypes();

    filterClosure filter_obj  = new filterClosure();
    filter_obj.filter_func = obj;
    filter_obj.arg = arg;
    filter_obj.srv  = srv;

    if (burstMsgDefaultCb == null) {
      burstMsgDefaultCb =
        srv.addProcessCb(new BurstProcessCb(),
                         TipcSvc.lookupMt(BURST),
                         filter_obj);
      if (burstMsgDefaultCb == null) {
        return false;
      }
    }
    return true;
  }

  ////////////////////////////////////////////////////
  // ..init
  /**
   * Initializes the burst by creating the required message types
   * to handle bursting and also sets up the callback to handle
   * the burst messages. This also sets up any user defined filter
   * specified.
   *
   * @param TipcBurstFilterFuncInterface An object which extends
   * the interface TipcBurstFilterFuncInterface
   * @param Object Optional param which user may want to send
   * @since  SmartSockets 6.2         
   */
  public boolean init(TipcBurstFilterFuncInterface obj, Object arg) { 
    
    TipcSrv srv = null;
    srv = TipcSvc.getSrv();

    return initInternal(srv, obj, arg);
  }  

  ////////////////////////////////////////////////////
  // ..init
  /**
   * Initializes the burst by creating the required message types
   * to handle bursting and also sets up the callback to handle
   * the burst messages. This also sets up any user defined filter
   * specified.
   *
   * @param TipcBurstFilterFuncInterface An object which extends
   * the interface TipcBurstFilterFuncInterface
   * @param Object Optional param which user may want to send
   * @since  SmartSockets 6.2         
   */
  public boolean init(TipcSrv srv, TipcBurstFilterFuncInterface obj, Object arg) { 
    
    if (null == srv) {
      return false;
    }
    return initInternal(srv, obj, arg);
  }

  ////////////////////////////////////////////////////
  // ..setAutoFlushCount
  /**
   * Sets the auto_flush_count of the burst.
   * 
   * NOTE: If this value is smaller than the initially set value
   * then the existing messages are automatically flushed(i.e a burst.flush)
   * is called which causes messages to be sent to Rtserver and then 
   * the auto_flush_count is set.
   * 
   * @param auto_flush_count size of the burst 
   * @exception TipcBadArgumentException if value is negative
   * @exception TipcException for all other errors
   * @return true on success, false on failure.
   * @since  SmartSockets 6.2         
   */
  public boolean setAutoFlushCount(int auto_flush_count)
  throws TipcBadArgumentException, TipcException {

    TipcSrv srv = null;
    srv = TipcSvc.getSrv();
    
    if (auto_flush_count <= 0) {
      throw new TipcBadArgumentException("Invalid auto_flush_count");
    }
    this.auto_flush_count = auto_flush_count;
    Enumeration l = ipc_burst_table.elements();
    /*
     * If the user calls setAutoFlushCount on the main burst
     * then we want to set each of these bursts since the main
     * burst holds references to all internally created
     * burst for each subject
     */
    while (l.hasMoreElements()) {
      internalBurstObjectInfo burst_info = 
        (internalBurstObjectInfo)l.nextElement();
      ipcBurstInternal burst = burst_info.burst;
      burst.setAutoFlushCountInternal(srv, auto_flush_count);
    } 
    return true;
  }

  ////////////////////////////////////////////////////
  // ..setAutoFlushCount
  /**
   * Sets the auto_flush_count of the burst.
   * 
   * NOTE: If this value is smaller than the initially set value
   * then the existing messages are automatically flushed(i.e a burst.flush)
   * is called which causes messages to be sent to Rtserver and then 
   * the auto_flush_count is set.
   * 
   * @param auto_flush_count size of the burst 
   * @exception TipcBadArgumentException if value is negative
   * @exception TipcException for all other errors
   * @return true on success, false on failure.
   * @since  SmartSockets 6.2         
   */
  public boolean setAutoFlushCount(TipcSrv srv, int auto_flush_count)
  throws TipcBadArgumentException, TipcException {
    if (auto_flush_count <= 0) {
      throw new TipcBadArgumentException("Invalid auto_flush_count");
    }
    this.auto_flush_count = auto_flush_count;
    Enumeration l = ipc_burst_table.elements();
    /*
     * If the user calls setAutoFlushCount on the main burst
     * then we want to set each of these bursts since the main
     * burst holds references to all internally created
     * burst for each subject
     */
    while (l.hasMoreElements()) {
      internalBurstObjectInfo burst_info = 
        (internalBurstObjectInfo)l.nextElement();
      ipcBurstInternal burst = burst_info.burst;
      burst.setAutoFlushCountInternal(srv, auto_flush_count);
    } 
    return true;
  }

  ////////////////////////////////////////////////////
  // ..getAutoFlushCount

  /**
   * Gets the auto_flush_count of the main burst
   * 
   * @return auto_flush_count of the burst
   * @since  SmartSockets 6.2      
   */
  public int getAutoFlushCount() {
    return this.auto_flush_count;
  }

  ////////////////////////////////////////////////////
  // ..getBurstCount

  /**
   * Gets the number of messages in burst at any given instance
   * based on the subject specified. 
   * 
   * @param subject_name The subject_name of which burst count is required
   * @exception TipcBadArgumentException If invalid subject name
   * or subject_name that doesn't exists is given.
   * @return count of number of messages in a burst at given instance
   
   * @since  SmartSockets 6.2   
   */
  private int getBurstCount(String subject_name) 
  throws TipcBadArgumentException {
    if (subject_name == null) {
      /* set it to something or else we get a null pointer 
       * exception
       */
      subject_name = "";
    }
    internalBurstObjectInfo burst_info = 
      (internalBurstObjectInfo)this.ipc_burst_table.get(subject_name);
    if (null != burst_info) {
      ipcBurstInternal burst = burst_info.burst;
      return burst.info_field_size / BURST_INFO_FIELD_SIZE;
    }
    else {
      throw new TipcBadArgumentException("Subject name does not exist");
    }
  }

  ////////////////////////////////////////////////////
  // ..flush

  /**
   * Sends the message to RTserver but the process has to 
   * explicitly call srv.flush to flush messages across
   * the link. 
   * 
   * @exception TipcException for all errors
   * @return true on success, false on failure.
   */
  public boolean flush() throws TipcException {
    TipcSrv srv = null;
    srv = TipcSvc.getSrv();

    Enumeration list = ipc_burst_table.elements();
    while (list.hasMoreElements()) {
      internalBurstObjectInfo new_burst_info = 
        (internalBurstObjectInfo)list.nextElement();
      ipcBurstInternal new_burst = new_burst_info.burst;
      if (new_burst.getCountInternal() > 0) {
        new_burst.flushInternal(srv);
      }
    }
    return true;
  }

  ////////////////////////////////////////////////////
  // ..flush

  /**
   * Sends the message to RTserver but the process has to 
   * explicitly call srv.flush to flush messages across
   * the link. 
   * 
   * @param TipcSrv srv object created by calling {@link TipcSvc#createSrv}
   * @exception TipcException for all errors
   * @return true on success, false on failure.
   */
  public boolean flush(TipcSrv srv) throws TipcException {

    /* should either call flushInternal i.e traverse the hashtable and
     * call flush for each one of them
     */
    Enumeration list = ipc_burst_table.elements();
    while (list.hasMoreElements()) {
      internalBurstObjectInfo new_burst_info = 
        (internalBurstObjectInfo)list.nextElement();
      ipcBurstInternal new_burst = new_burst_info.burst;
      if (new_burst.getCountInternal() > 0) {
        if (new_burst_info.srv == srv) {
          new_burst.flushInternal(srv);
        }
      }
    }
    return true;
  }

  ////////////////////////////////////////////////////
  // ..destroy
  /**
   * Destroys the internal data structures i.e all the
   * elements and keys of the hashtable
   * 
   * @return true on success, false on failure
   * @since  SmartSockets 6.2
   */
  public boolean destroy() {
    Enumeration d_list = ipc_burst_table.elements();
    while (d_list.hasMoreElements()) {
      internalBurstObjectInfo new_burst_info = 
        (internalBurstObjectInfo)d_list.nextElement();
      ipc_burst_table.remove(new_burst_info);
    }
    return true;
  }


  boolean msgSendInternal(TipcSrv srv, TipcMsg msg) throws TipcException {
    String subject_name;
    int delivery_mode;
    boolean status = false;
    
    subject_name = msg.getDest();
    /* set it nothing or else hashtable.get(subject_name) will get
     * a null pointer exception and fail, if the user forgets to 
     * set the destination
     */
    if (subject_name == null) {
      subject_name = "";
    }

    delivery_mode = msg.getDeliveryMode();

    internalBurstObjectInfo burst_info = 
      (internalBurstObjectInfo)this.ipc_burst_table.get(subject_name);

    /* This subject does not exists so it's new subject */
    if (burst_info == null) {
      ipcBurstInternal new_burst = null;
      new_burst = new ipcBurstInternal(srv, this.getAutoFlushCount());
      if (new_burst == null) {
        this.destroy();
        return false;
      }
      /* set the destination of the message right away */
      new_burst.msg.setDest(subject_name);
      new_burst.msg.setDeliveryMode(delivery_mode);

      double timestamp = msg.getSenderTimestamp();
      if (timestamp > 0.0) {
        new_burst.msg.setSenderTimestamp(timestamp);
      }

      internalBurstObjectInfo new_burst_info =
        new internalBurstObjectInfo(new_burst, srv);
      ipc_burst_table.put(subject_name, new_burst_info);
      int size_of_tab = ipc_burst_table.size();

      status = new_burst.ipcBurstAppendMsg(msg);
      /* if the auto_flush_count is reached the flush it */
      if (true == status) {
        if (new_burst.getCountInternal() >= new_burst.auto_flush_count) {
          new_burst.flushInternal(srv);
        }
      }
    }
    /* this would mean that the burst for that subject exists. 
     * So pull it out
     */
    else {
      /* get the burst out and use it */
      internalBurstObjectInfo check_burst_info =
        (internalBurstObjectInfo)this.ipc_burst_table.get(subject_name);
      int mode;
      ipcBurstInternal check_burst = check_burst_info.burst;
      mode = check_burst.msg.getDeliveryMode();

      if (mode != TipcDefs.DELIVERY_ORDERED
          && delivery_mode != TipcDefs.DELIVERY_ORDERED
          && delivery_mode > mode) {
        check_burst.msg.setDeliveryMode(delivery_mode);
      }
      else if (mode == TipcDefs.DELIVERY_ORDERED
               && delivery_mode > TipcDefs.DELIVERY_BEST_EFFORT 
               && delivery_mode <= TipcDefs.DELIVERY_ALL) {
        check_burst.msg.setDeliveryMode(delivery_mode);
      }

      double timestamp = msg.getSenderTimestamp();
      if (timestamp > 0.0) {
        msg.setSenderTimestamp(timestamp);
      }
      status = check_burst.ipcBurstAppendMsg(msg);
      if (true == status) {
        if (check_burst.getCountInternal() >=
            check_burst.auto_flush_count) {
          check_burst.flushInternal(srv);
        }
      }
    }
    return status;
  }

  ////////////////////////////////////////////////////
  // ..msgSend
  /**
   * Causes the messages to be coalesced on a per-subject basis. 
   *                                                                    
   * NOTE: When the auto_flush_count is reached burst.msgSend(msg) calls
   * flush to flush out the messages to RTserver so if you are setting
   * any options they have to be set before you call burst.msgSend(msg)
   * 
   * @param TipcMsg msg object
   * @exception TipcBadArgumentException if msg is null
   * @exception TipcException for all other errors
   * @return true on success, false on failure.
   * @since  SmartSockets 6.2   
   */
  public boolean msgSend(TipcMsg msg) throws TipcBadArgumentException, TipcException {

    if (msg == null) {
      throw new 
      TipcBadArgumentException("Null message not Acceptable");
    }
    TipcSrv srv = TipcSvc.getSrv();
    return msgSendInternal(srv, msg);
  }


  ////////////////////////////////////////////////////
  // ..msgSend
  /**
   * Causes the messages to be coalesced on a per-subject basis. The burst
   * message is sent on the given TipcSrv connection
   *                                                                    
   * NOTE: When the auto_flush_count is reached burst.msgSend(msg) calls
   * flush to flush out the messages to RTserver so if you are setting
   * any options they have to be set before you call burst.msgSend(msg)
   * 
   * @param TipcSrv srv object
   * @param TipcMsg msg object
   * @exception TipcBadArgumentException if msg is null
   * @exception TipcException for all other errors
   * @return true on success, false on failure.
   * @since  SmartSockets 6.2   
   */
  public boolean msgSend(TipcSrv srv, TipcMsg msg) throws TipcBadArgumentException, TipcException {
    if (null == msg
        || null == srv) {
      throw new 
      TipcBadArgumentException("Null message not Acceptable");
    }
    return msgSendInternal(srv, msg);
  }

    /*
   * Get the burst msg associated with this subject 
   *
   * @exception TipcException for all errors.
   * @return the burst msg associated with this subject
   * @since  SmartSockets 6.2
   */
  public TipcMsg getMsg(String subject) throws TipcException {
    internalBurstObjectInfo burst_info =
      (internalBurstObjectInfo)this.ipc_burst_table.get(subject);
    if (null == burst_info) {
      throw new TipcException("Cannot find the burst msg associated with this subject");
    }

    ipcBurstInternal burst = burst_info.burst;
    TipcMsg msg = burst.msg;
    
    return msg;
  }

  
  class internalBurstObjectInfo {
    ipcBurstInternal burst;
    TipcSrv srv;

    internalBurstObjectInfo(ipcBurstInternal internal_burst, TipcSrv srv_obj) {
      burst = internal_burst;
      srv   = srv_obj;
    }
  }

  class failureInfo {
    String process;
    int err_number;
    double err_time;
    TipcSrv srv;

    failureInfo(String proc, int errnum, double errtime, TipcSrv srv_obj) {
      process = proc;
      err_number = errnum;
      err_time = errtime;
      srv = srv_obj;
    }
  }

  class burstMsgTraverse implements TipcBurstMsgTraverseInterface {
    public boolean burstMsgTraverse(TipcMsg msg, Object arg) {
      failureInfo info = (failureInfo)arg;
      TipcMsg gmd_failure_msg = null;
      TipcSrv srv = info.srv;
      gmd_failure_msg = TipcSvc.createMsg(TipcSvc.lookupMt(TipcMt.GMD_FAILURE));

      gmd_failure_msg.appendMsg(msg);
      gmd_failure_msg.appendStr(info.process);
      gmd_failure_msg.appendInt4(info.err_number);
      gmd_failure_msg.appendReal8(info.err_time);

      if (null == srv) {
        srv = TipcSvc.getSrv();
      }
      try {
        srv.insert(gmd_failure_msg, TipcDefs.POS_PRIORITY);
      }
      catch (TipcException e) {
        return false;
      }
      return true;
    }
  }

  /**
   * Traverses through the BURST message and constructs each component msg and
   * invokes the Traversal Objects traversal function on each component msg.
   * 
   * @exception TipcBadArgumentException if the traveral object is NULL
   * @exception TipcException for all other errors
   * @return true on successful traversal through all component msgs of the
   * burst msg.
   * @since  SmartSockets 6.2   
   */

  public boolean msgTraverse(TipcMsg msg, TipcBurstMsgTraverseInterface trav_obj,
                             Object arg) throws TipcException {

    int    mt_num            = 0; 
    TipcMt mt                = null;
    int    fields            = 0;
    int    counter           = 0;
    int    msg_fields_offset = 0;
    int    burst_seq_num     = 0;
    int    delivery_mode     = 0;
    int    num_of_bursted_msgs = 0;
    int    info_field_size;
    String dest   = null;
    String sender = null;
    boolean status = true;

    if (null == trav_obj) {
      throw new TipcBadArgumentException("NULL TipcBursMsgTraverInterface object, not valid");
    }

    ipcBurstInternal burst = new ipcBurstInternal();
    mt = msg.getType();
    mt_num = mt.getNum();

    if (BURST != mt_num) {
      throw new TipcException("MSG NOT OF TYPE BURST");
    }

    fields = msg.getNumFields();
    msg.setCurrent(fields - 1);

    burst_seq_num = msg.getSeqNum();
    delivery_mode = msg.getDeliveryMode();

    int[] info_field_array = null;
    info_field_array  = msg.nextInt4Array();

    info_field_size = info_field_array.length;

    num_of_bursted_msgs = info_field_size / BURST_INFO_FIELD_SIZE;
    dest = msg.getDest();
    sender = msg.getSender();

    msg.setCurrent(0);

    for (counter=0; counter < num_of_bursted_msgs; counter++) {

      TipcMsg origMsg = null;
      int msg_type;
      int num_fields;
      int user_prop;

      msg_type = info_field_array[(counter * BURST_INFO_FIELD_SIZE)  + 0];
      num_fields = info_field_array[(counter * BURST_INFO_FIELD_SIZE) + 1];
      user_prop = info_field_array[(counter * BURST_INFO_FIELD_SIZE)  + 2];

      origMsg = TipcSvc.createMsg(TipcSvc.lookupMt(msg_type));

      if (origMsg == null) {
        continue;
      }

      origMsg.setDest(dest);
      origMsg.setSender(sender);
      origMsg.setSeqNum(burst_seq_num);
      origMsg.setDeliveryMode(delivery_mode);
      status = burst.ipcBurstAppendMsgFields(origMsg, msg,
                                             msg_fields_offset,
                                             num_fields);
      if (true == status) {
        origMsg.setUserProp(user_prop);
        msg_fields_offset += num_fields;
        if (trav_obj != null) {
          trav_obj.burstMsgTraverse(origMsg, arg); 
        }
      }
    } 
    return status;
  }

  class burstGmdFailureProcessCb implements TipcProcessCb {
    public void process(TipcMsg msg, Object arg) {
      failureInfo info = null;
      burstMsgTraverse trav_obj = new burstMsgTraverse();
      TipcSrv srv = (TipcSrv)arg;
      try {
        msg.setCurrent(0);
        TipcMsg failed_msg = msg.nextMsg();
        String  failed_process = msg.nextStr();
        int err_num = msg.nextInt4();
        double err_time = msg.nextReal8();

        TipcMt mt = failed_msg.getType();
        if (mt.getNum() == BURST) {
          
          info = new failureInfo(failed_process, err_num, err_time, srv);
          msgTraverse(failed_msg, trav_obj, info);
          msg.setCurrent(0);
        }
        else {
          msg.setCurrent(0);
          return;
        }
      }
      catch (TipcException e) {
        return;
      }
    }
  }

  /**
   * Adds a Process callback to handle GMD failures of BURST messages.
   * 
   *
   * @return the newly created callback
   * @see TipcSrv#addProcessCb 
   * @since  SmartSockets 6.2
   */
  public TipcCb addGmdFailureProcessCb() {
    burstGmdFailureProcessCb burst_cb = new burstGmdFailureProcessCb();
    TipcSrv srv = null;
    srv = TipcSvc.getSrv();
    TipcCb cb = srv.addProcessCb(burst_cb, TipcSvc.lookupMt(TipcMt.GMD_FAILURE),
                                 null);
    return cb;
  }

  /**
   * Adds a Process callback to handle GMD failures of BURST messages.
   * <b>Warning:</b> Normally one should always use 
   * {@link TipcBurst#addGmdFailureProcessCb} to register a callback to handle
   * GMD failures of BURST messages. This method should only be used when
   * multiple connections are absolutely necessary. 
   *
   * @param TipcSrv srv object created by calling {@link TipcSvc#createSrv}
   * @return the newly created callback
   * @see TipcSrv#addProcessCb 
   * @since  SmartSockets 6.2
   */
  public TipcCb addGmdFailureProcessCb(TipcSrv srv) throws TipcBadArgumentException {
    if (srv == TipcSvc.getSrv()) {
      return null;
    }
    burstGmdFailureProcessCb burst_cb = new burstGmdFailureProcessCb();
    TipcCb cb = srv.addProcessCb(burst_cb, TipcSvc.lookupMt(TipcMt.GMD_FAILURE),
                                 srv);
    return cb;
  }
}
