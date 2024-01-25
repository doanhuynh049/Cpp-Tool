/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/toption.h#1 $
 */

#ifndef _T_OPTION_H_
#define _T_OPTION_H_

#include <rtworks/ipc.h>
#include <rtworks/tex.h>
#include <rtworks/types.h>
#include <rtworks/tcallbck.h>

namespace SmartSockets {

  /**
   * Inherits from Exception so users can trap Option related
   * exceptions. Offers no additional functionality.
   *
   * @see Exception, Option
   */
  class T_IPCX_API OptionException : public Exception {

  public:
      /**
       * Constructor.
       */
      OptionException () {}

      /**
       * Destructor.
       */
      virtual ~OptionException () throw() {}
  };

  class T_IPCX_API Option;

  /**
   * Base class for all option change callback methods used in 
   * Option::optionChangeCbCreate().
   */
  class T_IPCX_API OptionChangeCallBack
  {
    public:

     /**
      * Signature for the method implementing the OptionChangeCallBack 
      * @param callBack pointer to a Callback that allows one to get arguments,
      * change the priority, or unregister (destroy) this callback.
      * @param newVal  the new value assigned to this option; use 
      * Option::getType()  to cast this appropriately.
      * @param opt option that was just changed (with the new value set).
      */
      typedef void (OptionChangeCallBack::* Handler) (
        CallBack < OptionChangeCallBack > * callBack,
        void* newVal,
        Option & opt);

     /**
      * Default implementation of the callback method.
      * @param callBack pointer to a Callback that allows one to get arguments,
      * change the priority, or unregister (destroy) this callback.
      * @param newVal  the new value assigned to this option; use
      * Option::getType()  to cast this appropriately.
      * @param opt option that was just changed (with the new value set).
      */
      virtual void onChange (
        CallBack < OptionChangeCallBack > * callBack,
        void* newVal,
        Option& opt);
  };


  /**
   *  Implements all the TutOption methods.
   */
  class T_IPCX_API Option
  {

  public:
    /// constructor compatible with the C API.
    Option (T_OPTION opt) throw (OptionException);

    /// constructor that looks up an option by name.
    /// @param name name of a previously created user option, or of a standard
    /// option.
    /// @throw OptionException if the name passed in doesn't match any options.
    Option (const char* name) throw (OptionException);

   /**
    * Creates a user-defined SmartSockets option. 
    * Matches a Call to TutOptionCreate().
    * @param name option name
    * @param optType: \n
    *  - T_OPT_TYPE_BOOLEAN\n
    *  - T_OPT_TYPE_ENUM\n
    *  - T_OPT_TYPE_NUMERIC\n
    *  - T_OPT_TYPE_STRING\n
    *  - T_OPT_TYPE_LIST_ENUM\n
    *  - T_OPT_TYPE_LIST_NUMERIC\n
    *  - T_OPT_TYPE_LIST_STRING\n
    *
    * @throw OptionException if option already exists
    */
    static Option create(const char* name,T_OPT_TYPE optType ) 
          throw (OptionException);

    ///destructor
    virtual ~Option () throw() {}
  
    ///destroys a property
    static void destroyOption (const char* name) throw (OptionException);
  
    ///gets a bool property
    bool getBool () const throw (OptionException);
  
    ///sets a bool property
    void setBool (bool value) throw (OptionException);
  
    ///gets a enum property
    const char *getEnum () const throw (OptionException);
  
    ///sets a enum property
    void setEnum (const char * value) throw (OptionException);
  
    ///gets a enum list property
    T_STR_LIST getEnumList() const throw (OptionException);
  
    ///sets a enum list property
    void setEnumList (T_STR_LIST value) throw (OptionException);

    ///gets a numeric property
    real8 getReal8 () const throw (OptionException);
  
    ///sets a numeric property
    void setReal8 (real8 value) throw (OptionException);

    ///gets a numeric property
    int4 getInt4 () const throw (OptionException);
  
    ///sets a numeric property
    void setInt4 (int4 value) throw (OptionException);

    ///gets a string property
    const char * getString () const throw (OptionException);
  
    ///sets a string property
    void setString (const char * value) throw (OptionException);
  
    /**
     * Gets a string list property.  The following example shows how to use
     * the return value:\n
     *      Option serverNames("server_names");\n
     *      T_STR_LIST servNamesVal = serverNames.getStringList();\n
     *      // Dump the string values \n
     *      for (T_STR_LIST node = str_list_val; \n
     *           node != NULL; \n
     *           node = node->next) \n
     *      {\n
     *        TutOut("%s\n", node->string);\n
     *      }\n
     */
    T_STR_LIST getStringList () const throw (OptionException);
  
    ///sets a string list property
    void setStringList (T_STR_LIST value) throw (OptionException);
 
    /**
     * Gets the type of a property.
     * @return  (one of T_OPT_TYPE_BOOLEAN,T_OPT_TYPE_ENUM,T_OPT_TYPE_NUMERIC
     * T_OPT_TYPE_STRING,T_OPT_TYPE_LIST_ENUM,T_OPT_TYPE_LIST_NUMERIC,
     * T_OPT_TYPE_LIST_STRING)
     */
    T_OPT_TYPE getType () const throw (OptionException);
  
    ///???is the property known?
    bool isKnown () const throw (OptionException);
  
    ///???sets whether the property is known?
    void setUnKnown () throw (OptionException);

    ///is the property read only?
    bool getReadOnly () const throw (OptionException);
  
    ///sets whether the property is read only
    void setReadOnly (bool is_read_only) throw (OptionException);
  
    ///Determines whether or not a property is required.
    bool getRequired () const throw (OptionException);
  
    ///sets whether the property is required
    void setRequired (bool is_required) throw (OptionException);

    ///gets the name of the option.
    const char* getName () throw (OptionException);

    ///Adds a legal value to enumerated option
    void legValAdd (const char * value) throw (OptionException);
  
    ///creates an option change callback
    CallBack <OptionChangeCallBack> * optionChangeCbCreate(OptionChangeCallBack * impl,
                                                           void* arg = NULL)
                          throw (OptionException);

  private:

    T_OPTION _opt;

  protected:
    ///delegator method called when callback is triggered
    static void T_ENTRY1 optionChangeCbDelegator(T_OPTION,
                                                 T_OPTION_CHANGE_CB_DATA,
                                                 T_CB_ARG);
  };
  
} //namespace SmartSockets

#endif //_T_OPTION_H_
