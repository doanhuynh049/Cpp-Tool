/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/rtworks/txml.h#1 $
 */

#ifndef _T_XML_HH_
#define _T_XML_HH_

#include <rtworks/ipc.h>
#include <rtworks/types.h>
#include <rtworks/tex.h>

namespace SmartSockets {

  /**
   * Inherits from Exception so users can trap XML related
   * exceptions. Offers no additional functionality.
   *
   * @see Exception, XML 
   */
  class T_IPCX_API XMLException : public Exception {

  public:
      /**
       * Constructor that simply wraps the Exception constructor.
       */
			XMLException () {}
  
      /**
       * Destructor.
       */
      virtual ~XMLException () throw() {}
  };
  
  /**
   * XML implements all the TutXml methods for handling T_XML objects.
   */
  class T_IPCX_API XML
  {

    T_XML _xml;

  public:
      /**
       * Constructs an XML object containing an %XML string.
       * @param xml is a const string containing the %XML.
       */
      XML (const char * xml) throw (XMLException);

      /**
       * Constructs an XML object from the C structure.
       */
      XML (T_XML xml) throw (XMLException):
      _xml(xml)
      {
        if (_xml == NULL)
        {
          TutErrNumSet(T_ERR_NULL_PTR);
          throw XMLException();
        }
      }


      
      /**
       * Destructor
       */
      virtual ~XML () throw();
  
      /**
       * Typecast operator for compatibility with the C API.
       */
      operator T_XML() const throw() {return _xml;}

      /**
       * Creates an XML object from a static buffer.
       * It does not clone the data which must remain
       * valid for the lifetime of the newly created XML object.
       * Maps to TutXmlCreateStatic.
       * @param xml is the static buffer.
       * @returns the newly created XML object.
       */
      static T_XML createFromStaticBuf (const char * xml)
												throw (XMLException);
  
      /**
       * Clones this XML object. Maps to TutXmlClone.
       * @returns a copy of this XML object.
       */
      T_XML clone () const throw (XMLException);

      /**
       * Gets the XML string associated with this XML object.
       * @returns the %XML string.
       */
      const char * getString () const throw (XMLException);

      /**
       * Sets the %XML string associated with this XML object.
       * @param xml is the new %XML string.
       */
      void setString (const char * xml) throw (XMLException);
  };

} //namespace SmartSockets

#endif //_T_XML_HH_
