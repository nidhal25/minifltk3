// "$Id: NativeFileChooser.cxx 9013 2011-08-25 22:07:10Z matt $"
//
// FLTK native OS file chooser widget
//
// Copyright 1998-2010 by Bill Spitzak and others.
// Copyright 2004 Greg Ercolano.
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
// Please report all bugs and problems to:
//
//     http://www.fltk.org/str.php
//

// Use Windows' chooser
#ifdef WIN32
#include "Platform_win32_NativeFileChooser.cxx"
#endif

// Use Apple's chooser
#ifdef __APPLE__
#include "NativeFileChooser.h"
#endif

// All else falls back to FLTK's own chooser
#if ! defined(__APPLE__) && !defined(WIN32)
#include "Platform_x11_NativeFileChooser.cxx"
#endif

const char *fltk3::NativeFileChooser::file_exists_message = "File exists. Are you sure you want to overwrite?";

//
// End of "$Id: NativeFileChooser.cxx 9013 2011-08-25 22:07:10Z matt $".
//
