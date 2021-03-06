//
// "$Id: set_font.cxx 9011 2011-08-25 21:50:59Z matt $"
//
// Font utilities for the Fast Light Tool Kit (FLTK).
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

// Add a font to the internal table.
// Also see fl_set_fonts.cxx which adds all possible fonts.

#include "run.h"
#include "x.h"
#include "draw.h"
#include "flstring.h"
#include "font.h"
#include <stdlib.h>

static int table_size;
/**
  Changes a face.  The string pointer is simply stored,
  the string is not copied, so the string must be in static memory.
*/
void fltk3::set_font(fltk3::Font fnum, const char* name)
{
	while (fnum >= table_size) {
		int i = table_size;
		if (!i) {	// don't realloc the built-in table
			table_size = 2*fltk3::FREE_FONT;
			i = fltk3::FREE_FONT;
			Fl_Fontdesc* t = (Fl_Fontdesc*)malloc(table_size*sizeof(Fl_Fontdesc));
			memcpy(t, fltk3::fonts, fltk3::FREE_FONT*sizeof(Fl_Fontdesc));
			fltk3::fonts = t;
		} else {
			table_size = 2*table_size;
			fltk3::fonts=(Fl_Fontdesc*)realloc(fltk3::fonts, table_size*sizeof(Fl_Fontdesc));
		}
		for (; i < table_size; i++) {
			fltk3::fonts[i].fontname[0] = 0;
			fltk3::fonts[i].name = 0;
#if !defined(WIN32) && !defined(__APPLE__)
			fltk3::fonts[i].xlist = 0;
			fltk3::fonts[i].n = 0;
#endif // !WIN32 && !__APPLE__
		}
	}
	Fl_Fontdesc* s = fltk3::fonts+fnum;
	if (s->name) {
		if (!strcmp(s->name, name)) {
			s->name = name;
			return;
		}
#if !defined(WIN32) && !defined(__APPLE__)
		if (s->xlist && s->n >= 0) XFreeFontNames(s->xlist);
#endif
		for (Fl_Font_Descriptor* f = s->first; f;) {
			Fl_Font_Descriptor* n = f->next;
			delete f;
			f = n;
		}
		s->first = 0;
	}
	s->name = name;
	s->fontname[0] = 0;
#if !defined(WIN32) && !defined(__APPLE__)
	s->xlist = 0;
#endif
	s->first = 0;
	fltk3::font(-1, 0);
}
/** Copies one face to another. */
void fltk3::set_font(fltk3::Font fnum, fltk3::Font from)
{
	fltk3::set_font(fnum, get_font(from));
}
/**
    Gets the string for this face.  This string is different for each
    face. Under X this value is passed to XListFonts to get all the sizes
    of this face.
*/
const char* fltk3::get_font(fltk3::Font fnum)
{
	return fltk3::fonts[fnum].name;
}

//
// End of "$Id: set_font.cxx 9011 2011-08-25 21:50:59Z matt $".
//
