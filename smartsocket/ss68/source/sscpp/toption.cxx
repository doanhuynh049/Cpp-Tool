/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/toption.cxx#1 $
 */

#include "includes.h"
#include <rtworks/toption.h>
#include <rtworks/tutil.h>

namespace SmartSockets {

Option::Option (T_OPTION opt) throw (OptionException)
{
  if (opt == NULL)
  { 
    TutErrNumSet(T_ERR_NULL_PTR);
    throw OptionException();
  }
  _opt = opt;
}


/// Matches a Call to TutOptionLookup(); throws an exception if _opt doesn't exist
Option::Option (const char* name) throw (OptionException)
{
  _opt = TutOptionLookup(const_cast<char *> (name));
  if (_opt == NULL)
    throw OptionException();
}

/**
* Creates a user-defined SmartSockets _opt. 
* Matches a Call to TutOptionCreate().
* @param name _opt name
* @param optType (one of T_OPT_TYPE_BOOLEAN,T_OPT_TYPE_ENUM,T_OPT_TYPE_NUMERIC
* T_OPT_TYPE_STRING,T_OPT_TYPE_LIST_ENUM,T_OPT_TYPE_LIST_NUMERIC,
* T_OPT_TYPE_LIST_STRING).
* @throw OptionException if _opt already exists
*/
Option Option::create (const char* name,T_OPT_TYPE optType ) 
   throw (OptionException)
{
  T_OPTION opt = TutOptionCreate(const_cast<char *>(name),optType);
  if (opt == NULL)
    throw OptionException();
  return Option(opt);
}


///destroys a property
void Option::destroyOption (const char * name) throw (OptionException)
{
  T_OPTION opt = TutOptionLookup(const_cast<char *>(name));
  if (opt == NULL)
    throw OptionException();

  if (!TutOptionDestroy(opt))
    throw OptionException();
}

///gets a bool property
bool Option::getBool () const throw (OptionException)
{
  T_BOOL bool_ret_value = false;

  bool result = TutOptionGetBool(_opt, &bool_ret_value);
  if (false == result)
	throw OptionException();

  return (bool_ret_value?true:false);
}

///sets a bool property
void Option::setBool (bool value) throw (OptionException)
{
  bool result = TutOptionSetBool(_opt, value);
	if (false == result)
		throw OptionException();
}

///gets a enum property
const char * Option::getEnum () const throw (OptionException)
{
  char * enum_return = (char *)"";  // initialize variable 

  bool result = TutOptionGetEnum(_opt, &enum_return);
	if (false == result)
		throw OptionException();
  
  return static_cast <const char*> (enum_return);
}

///sets a enum property
void Option::setEnum (const char * name) throw (OptionException)
{
  bool result = TutOptionSetEnum(_opt, const_cast<char *> (name));
	if (false == result)
		throw OptionException();
}

///gets a enum list property
T_STR_LIST Option::getEnumList () const throw (OptionException)
{
  T_STR_LIST enum_list_return;

  bool result = TutOptionGetEnumList(_opt, &enum_list_return);
	if (false == result)
		throw OptionException();

  return enum_list_return;
}

///sets a enum list property
void Option::setEnumList (T_STR_LIST value)
												 throw (OptionException)
{
  bool result = TutOptionSetEnumList(_opt, value);
	if (false == result)
		throw OptionException();
}

///gets a numeric property
real8 Option::getReal8 () const throw (OptionException)
{
  real8 val_return;
  bool result = TutOptionGetNum(_opt, &val_return);
	if (false == result)
		throw OptionException();

  return val_return;
}

///sets a numeric property
void Option::setReal8 (real8 value) throw (OptionException)
{
  bool result = TutOptionSetNum(_opt, value);
	if (false == result)
		throw OptionException();
}

///gets a numeric property
int4 Option::getInt4 () const throw (OptionException)
{
  real8 val_return = 0;
  bool result = TutOptionGetNum(_opt, &val_return);
	if (false == result)
		throw OptionException();

  return static_cast<int4>(val_return);
}

///sets a numeric property
void Option::setInt4 (int4 value) throw (OptionException)
{
  bool result = TutOptionSetNum(_opt, static_cast<real8>(value));
	if (false == result)
		throw OptionException();
}

///gets a string property
const char * Option::getString () const throw (OptionException)
{
  char * val_return = (char *) "";
  bool result = TutOptionGetStr(_opt, &val_return);
	if (false == result)
		throw OptionException();

  return static_cast<const char *> (val_return);
}

///sets a string property
void Option::setString (const char * value) throw (OptionException)
{
  bool result = TutOptionSetStr(_opt, const_cast<char *>(value));
	if (false == result)
		throw OptionException();
}

///gets a string list property
T_STR_LIST Option::getStringList () const 
												throw (OptionException)
{
  T_STR_LIST val_return;
  bool result = TutOptionGetStrList(_opt, &val_return);
	if (false == result)
		throw OptionException();

  return val_return;
}

///sets a string list property
void Option::setStringList (T_STR_LIST value) 
												throw (OptionException)
{
  bool result = TutOptionSetStrList(_opt, value);
	if (false == result)
		throw OptionException();
}

///gets the type of a property
T_OPT_TYPE Option::getType () const throw (OptionException)
{
  T_OPT_TYPE type_return = T_OPT_TYPE_STRING;
  bool result = TutOptionGetType(_opt, &type_return);
	if (false == result)
		throw OptionException();

  return type_return;
}

///is the property known?
bool Option::isKnown () const throw (OptionException)
{
  T_BOOL retVal = FALSE;
  if (!TutOptionGetKnown(_opt,&retVal))
    throw OptionException();

  return (retVal?true:false);
}

///sets whether the property is known?
void Option::setUnKnown () throw (OptionException)
{
  bool result = TutOptionSetUnknown(_opt);
  if (false == result)
  	throw OptionException();
}

///is the property read only?
bool Option::getReadOnly () const throw (OptionException)
{
  T_BOOL read_only_return;
  bool result = TutOptionGetReadOnly(_opt, &read_only_return);
  if (false == result)
  	throw OptionException();

  return (read_only_return?true:false);
}

///sets whether the property is read only
void Option::setReadOnly (bool is_read_only) 
												throw (OptionException)
{
  bool result = TutOptionSetProtected(_opt, is_read_only);
  if (false == result)
  	throw OptionException();
}

///is the property required?
bool Option::getRequired () const throw (OptionException)
{
  T_BOOL required_return;
  bool result = TutOptionGetRequired(_opt, &required_return);
  if (false == result)
   	throw OptionException();
  
  return (required_return?true:false);
}

///sets whether the property is required
void Option::setRequired (bool is_required) 
												throw (OptionException)
{
  bool result = TutOptionSetRequired(_opt, is_required);
  if (false == result)
   	throw OptionException();
}
  
const char* Option::getName () throw (OptionException)
{
  char * retVal = TutOptionGetName(_opt);
  if (NULL == retVal)
    throw OptionException();
  
  return static_cast<const char*> (retVal);
}

///Adds a legal value to enumerated option
void Option::legValAdd (const char * value) throw (OptionException)
{
  bool result = TutOptionLegValAdd(_opt, const_cast<char *>(value));
	if (false == result)
		throw OptionException();
}


CallBack<OptionChangeCallBack> * Option::optionChangeCbCreate(
       OptionChangeCallBack * implementor,
       void* arg)
                                                        throw (OptionException)
{

  OptionChangeCallBack::Handler handler = &OptionChangeCallBack::onChange;
  //Create a handle to the C++ CallBack handle
  CallBack <OptionChangeCallBack> * cb =
                    new CallBack <OptionChangeCallBack> (implementor, handler);

  T_CB tcb = TutOptionChangeCbCreate(this->_opt,
                                     this->optionChangeCbDelegator,
                                     cb);

  // Throw exception of the creation failed.
  if (!tcb) {
    delete cb;
    throw OptionException ();
  }

  cb->setTCB (tcb);
  cb->setArgument(arg);

  return cb;
}


void T_ENTRY1 Option::optionChangeCbDelegator(T_OPTION opt,
                                              T_OPTION_CHANGE_CB_DATA data,
                                              T_CB_ARG arg) 
{
  CallBack <OptionChangeCallBack> * callBack = 
                        static_cast <CallBack <OptionChangeCallBack> *> (arg);

  Option option = (Option) opt;

  ((callBack->m_implementor)->*(callBack->m_handler)) (callBack, data->value,
                                                       option);
}


void OptionChangeCallBack::onChange(CallBack<OptionChangeCallBack> * callBack,
                                    void * newVal,
                                    Option & opt) 
{
   callBack->print((T_OUT_FUNC)TutOut);
   Utilities::out("%d\n", sizeof(newVal));
   opt.getType();
}
} //namespace SmartSockets
