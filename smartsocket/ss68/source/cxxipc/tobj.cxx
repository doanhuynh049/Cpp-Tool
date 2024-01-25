/*
 * Copyright (c) 1991-2006 TIBCO Software Inc.
 * All rights reserved.
 * For more information, please contact:
 * TIBCO Software Inc., Palo Alto, California, USA
 *
 * $Id: //dev/packages/tpsi/cpsi/dev/68/src/cxxipc/tobj.cxx#1 $
 */

#include "includes.h"
#include <rtworks/tobj.hxx>

/* ========================================================================= */
/*..Tobj::Tobj -- Tobj constructor */
Tobj::Tobj() 
{ 
  _status = T_TRUE; 
}

/* ========================================================================= */
/*..Tobj::~Tobj -- Tobj destructor */
Tobj::~Tobj()
{
  // This destructor is not inlined due to compiler warnings on IRIX.
}


