//
// "$Id: Object.h 9676 2012-08-18 11:23:23Z matt $"
//
// Widget header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
 fltk3::Object */

#ifndef FLTK3_OBJECT_H
#define FLTK3_OBJECT_H


#include "Export.h"


namespace fltk3
{
/**
 \brief A base class for any kind of langage wrapper around FLTK3.

 fltk3::Object is a way to inject code into the existing library at
 run-time. When used as a base class, external languages and interfaces can
 use the counterpart fltk3::Wrapper to allow for external overrides of every
 fltk3 class.

 Practical examples for this are the FLTK1 and FLTK2 emulation. It all looks
 a bit convoluted at first, but the use is straight forward and creates a true
 relationship between this library and any other frontend.
 */
class FLTK3_EXPORT Object
{
public:
	Object() {}
	virtual ~Object() {}
};

} // namespace fltk3


#endif

//
// End of "$Id: Object.h 9676 2012-08-18 11:23:23Z matt $".
//
