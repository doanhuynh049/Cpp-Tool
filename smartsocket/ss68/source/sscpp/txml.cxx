/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/sscpp/txml.cxx#1 $
 */

#include "includes.h"
#include <rtworks/txml.h>

namespace SmartSockets {

/**
 * Constructs an XML object containing an %XML string.
 * @param xml is a const string containing the %XML.
 */
XML::XML (const char * xml) throw (XMLException)
{
  _xml = TutXmlCreate(const_cast<char *> (xml));
  if (NULL == _xml) {
		throw XMLException();
  }
}

/**
 * Destructor
 */
XML::~XML () throw()
{
}

/**
 * Creates an XML object from a static buffer.
 * It does not clone the data which must remain
 * valid for the lifetime of the newly created XML object.
 * Maps to TutXmlCreateStatic.
 * @param xml is the static buffer.
 * @returns the newly created XML object.
 */
T_XML XML::createFromStaticBuf (const char * xml) throw (XMLException)
{
  return TutXmlCreateStatic(const_cast<char *> (xml));
}

/**
 * Clones this XML object. Maps to TutXmlClone.
 * @returns a copy of this XML object.
 */
T_XML XML::clone () const  throw (XMLException)
{
  return TutXmlClone(_xml);
}

/**
 * Gets the XML string associated with this XML object.
 * @returns the %XML string.
 */
const char * XML::getString () const throw (XMLException)
{
  char * str_return = (char *)"";  // initialize variable
	
  bool result = TutXmlGetStr(_xml, &str_return);
	if(false == result)
		throw XMLException();
  
  return str_return;
}

/**
 * Sets the %XML string associated with this XML object.
 * @param xml is the new %XML string.
 */
void XML::setString (const char * xml) throw (XMLException)
{
  bool result = TutXmlSetStr(_xml, const_cast<char *> (xml));
	if(false == result)
		throw XMLException();
}

} //namespace SmartSockets
