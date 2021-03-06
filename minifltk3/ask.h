//
// "$Id: ask.h 9749 2012-12-11 17:18:38Z manolo $"
//
// Standard dialog header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2011 by Bill Spitzak and others.
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

// FLTK 123 complete

/** \file ask.H
 API for common dialogs.
 */


#ifndef fltk3_ask_H
#define fltk3_ask_H

#include "enumerations.h"
#include <stdarg.h>

#  ifdef __GNUC__
#    define __fl_attr(x) __attribute__ (x)
#  else
#    define __fl_attr(x)
#  endif // __GNUC__


namespace fltk3
{

class Widget;

/** Different system beeps available.
 \sa fltk3::beep(int) */
enum Beep {
	BEEP_DEFAULT = 0,  ///< Default beep.
	BEEP_MESSAGE,  ///< Message beep.
	BEEP_ERROR,  ///< Error beep.
	BEEP_QUESTION,  ///< Question beep.
	BEEP_PASSWORD,  ///< Password beep.
	BEEP_NOTIFICATION  ///< Notification beep.
};

FLTK3_EXPORT void beep(int type = BEEP_DEFAULT);
FLTK3_EXPORT void message(const char *,...) __fl_attr((__format__ (__printf__, 1, 2)));
FLTK3_EXPORT void vmessage(const char *, va_list);
FLTK3_EXPORT void alert(const char *,...) __fl_attr((__format__ (__printf__, 1, 2)));
FLTK3_EXPORT void valert(const char *, va_list);
FLTK3_EXPORT int ask(const char *,...) __fl_attr((__format__ (__printf__, 1, 2), __deprecated__));
FLTK3_EXPORT int vask(const char *, va_list);
FLTK3_EXPORT int choice(const char *q,const char *b0,const char *b1,const char *b2,...) __fl_attr((__format__ (__printf__, 1, 5)));
FLTK3_EXPORT int vchoice(const char *, const char *, const char *, const char *, va_list);
FLTK3_EXPORT const char *input(const char *label, const char *deflt = 0, ...) __fl_attr((__format__ (__printf__, 1, 3)));
FLTK3_EXPORT const char *vinput(const char *, const char *d, va_list);
FLTK3_EXPORT const char *password(const char *label, const char *deflt = 0, ...) __fl_attr((__format__ (__printf__, 1, 3)));
FLTK3_EXPORT const char *vpassword(const char *, const char *d, va_list);

FLTK3_EXPORT Widget *message_icon();
extern FLTK3_EXPORT Font message_font_;
extern FLTK3_EXPORT Fontsize message_size_;
inline void message_font(Font f, Fontsize s)
{
	message_font_ = f;
	message_size_ = s;
}

FLTK3_EXPORT void message_hotspot(int enable);
FLTK3_EXPORT int message_hotspot(void);

FLTK3_EXPORT void message_title(const char *title);
FLTK3_EXPORT void message_title_default(const char *title);

// pointers you can use to change FLTK to a foreign language:
extern FLTK3_EXPORT const char* no;
extern FLTK3_EXPORT const char* yes;
extern FLTK3_EXPORT const char* ok;
extern FLTK3_EXPORT const char* cancel;
extern FLTK3_EXPORT const char* close;

} // namespace

#endif // !fltk3::ask_H

//
// End of "$Id: ask.h 9749 2012-12-11 17:18:38Z manolo $".
//
