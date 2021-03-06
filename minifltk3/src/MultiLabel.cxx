//
// "$Id: MultiLabel.cxx 9017 2011-08-27 22:50:53Z matt $"
//
// Multi-label widget for the Fast Light Tool Kit (FLTK).
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

// Allows two labels to be used on a widget (by having one of them
// be one of these it allows an infinte number!)

#include "run.h"
#include "Widget.h"
#include "MenuItem.h"
#include "MultiLabel.h"

void fl_multi_labeltype(const fltk3::Label* o, int x, int y, int w, int h, fltk3::Align a)
{
	fltk3::MultiLabel* b = (fltk3::MultiLabel*)(o->label());
	fltk3::Label local = *o;
	local.label(b->labela);
	local.labeltype(b->typea);
	int W = w;
	int H = h;
	local.measure(W, H);
	local.draw(x,y,w,h,a);
	if (a & fltk3::ALIGN_BOTTOM) h -= H;
	else if (a & fltk3::ALIGN_TOP) {
		y += H;
		h -= H;
	} else if (a & fltk3::ALIGN_RIGHT) w -= W;
	else if (a & fltk3::ALIGN_LEFT) {
		x += W;
		w -= W;
	} else {
		int d = (h+H)/2;
		y += d;
		h -= d;
	}
	local.label(b->labelb);
	local.labeltype(b->typeb);
	local.draw(x,y,w,h,a);
}

// measurement is only correct for left-to-right appending...
void fl_multi_measure(const fltk3::Label* o, int& w, int& h)
{
	fltk3::MultiLabel* b = (fltk3::MultiLabel*)(o->label());
	fltk3::Label local = *o;
	local.label(b->labela);
	local.labeltype(b->typea);
	local.measure(w,h);
	local.label(b->labelb);
	local.labeltype(b->typeb);
	int W = 0;
	int H = 0;
	local.measure(W,H);
	w += W;
	if (H>h) h = H;
}

void fltk3::MultiLabel::label(fltk3::Widget* o)
{
	o->label(fltk3::MULTI_LABEL, (const char*)this);
}

void fltk3::MultiLabel::label(fltk3::MenuItem* o)
{
	o->label(fltk3::MULTI_LABEL, (const char*)this);
}

//
// End of "$Id: MultiLabel.cxx 9017 2011-08-27 22:50:53Z matt $".
//
