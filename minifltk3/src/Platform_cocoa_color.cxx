//
// "$Id: cocoa_color.cxx 9153 2011-10-26 12:56:06Z manolo $"
//
// MacOS color functions for the Fast Light Tool Kit (FLTK).
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

// The fltk "colormap".  This allows ui colors to be stored in 8-bit
// locations, and provides a level of indirection so that global color
// changes can be made.  Not to be confused with the X colormap, which
// I try to hide completely.

// matt: Neither Quartz nor Quickdraw support colormaps in this implementation
// matt: Quartz support done

#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_MAC

#include <config.h>
#include "run.h"
#include "x.h"
#include "draw.h"

static unsigned fl_cmap[256] = {
#include "cmap.h" // this is a file produced by "cmap.cxx":
};

void fltk3::QuartzGraphicsDriver::color(fltk3::Color i)
{
	fltk3::GraphicsDriver::color(i);
	int index;
	uchar r, g, b;
	if (i & 0xFFFFFF00) {
		// translate rgb colors into color index
		r = i>>24;
		g = i>>16;
		b = i>> 8;
	} else {
		// translate index into rgb:
		index = i;
		unsigned c = fl_cmap[i];
		r = c>>24;
		g = c>>16;
		b = c>> 8;
	}
	if (!fl_gc) return; // no context yet? We will assign the color later.
	float fr = r/255.0f;
	float fg = g/255.0f;
	float fb = b/255.0f;
	CGContextSetRGBFillColor(fl_gc, fr, fg, fb, 1.0f);
	CGContextSetRGBStrokeColor(fl_gc, fr, fg, fb, 1.0f);
}

void fltk3::QuartzGraphicsDriver::color(uchar r, uchar g, uchar b)
{
	fltk3::GraphicsDriver::color( fltk3::rgb_color(r, g, b) );
	float fr = r/255.0f;
	float fg = g/255.0f;
	float fb = b/255.0f;
	CGContextSetRGBFillColor(fl_gc, fr, fg, fb, 1.0f);
	CGContextSetRGBStrokeColor(fl_gc, fr, fg, fb, 1.0f);
}

void fltk3::set_color(fltk3::Color i, unsigned c)
{
	if (fl_cmap[i] != c) {
		fl_cmap[i] = c;
	}
}

#endif // #if TARGET_OS_MAC
#endif // #ifdef __APPLE__

//
// End of "$Id: cocoa_color.cxx 9153 2011-10-26 12:56:06Z manolo $".
//
