//
// "$Id: RadioLightButton.h 9638 2012-07-24 04:41:38Z matt $"
//
// Radio light button header file for the Fast Light Tool Kit (FLTK).
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
 fltk3::RadioLightButton widget . */

#ifndef Fltk3_Radio_Light_Button_H
#define Fltk3_Radio_Light_Button_H

#include "LightButton.h"

#include "run.h"
#include "draw.h"
#include "src/flstring.h"

//#include <fltk3/RadioLightButton.h>

namespace fltk3
{

class FLTK3_EXPORT RadioLightButton : public fltk3::LightButton
{
public:
	RadioLightButton(int X,int Y,int W,int H,const char *l=0): fltk3::LightButton(X,Y,W,H,l) {
		type(fltk3::RADIO_BUTTON);
	}
};

}

#endif

//
// End of "$Id: RadioLightButton.h 9638 2012-07-24 04:41:38Z matt $".
//
