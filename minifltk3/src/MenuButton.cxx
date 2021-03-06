//
// "$Id: MenuButton.cxx 9669 2012-08-16 21:41:30Z matt $"
//
// Menu button widget for the Fast Light Tool Kit (FLTK).
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

#include "run.h"
#include "MenuButton.h"
#include "draw.h"


static fltk3::MenuButton	*pressed_menu_button_ = 0;

void fltk3::MenuButton::draw()
{
	if (!box() || type()) return;
	int H = (labelsize()-3)&-2;
	int X = w()-H-fltk3::box_dx(box())-fltk3::box_dw(box())-1;
	int Y = (h()-H)/2;
	//draw_box(pressed_menu_button_ == this ? fltk3::down(box()) : box(), color());
	draw_box(pressed_menu_button_ == this ? DOWN_BOX : box(), color());
	draw_label(fltk3::box_dx(box()), 0, X, h());
	if (fltk3::focus() == this) draw_focus();
	// ** if (box() == fltk3::FLAT_BOX) return; // for XForms compatibility
	fltk3::color(active_r() ? fltk3::DARK3 : fltk3::inactive(fltk3::DARK3));
	fltk3::line(X+H/2, Y+H, X, Y, X+H, Y);
	fltk3::color(active_r() ? fltk3::LIGHT3 : fltk3::inactive(fltk3::LIGHT3));
	fltk3::line(X+H, Y, X+H/2, Y+H);
}


/**
  Act exactly as though the user clicked the button or typed the
  shortcut key.  The menu appears, it waits for the user to pick an item,
  and if they pick one it sets value() and does the callback or
  sets changed() as described above.  The menu item is returned
  or NULL if the user dismisses the menu.
*/
const fltk3::MenuItem* fltk3::MenuButton::popup()
{
	const fltk3::MenuItem* m;
	pressed_menu_button_ = this;
	redraw();
	fltk3::WidgetTracker mb(this);
	if (!box() || type()) {
		m = menu()->popup(dx_window()+event_x(), dy_window()+event_y(), label(), mvalue(), this);
		/*
		int x, y;
		fltk3::get_mouse(x, y);
		m = menu()->popup(x+event_x(), y+event_y(), label(), mvalue(), this);
		printf("x=%d, y=%d\n", x, y);
		*/
	} else {
		m = menu()->pulldown(dx_window(), dy_window(), w(), h(), 0, this);
	}
	picked(m);
	pressed_menu_button_ = 0;
	if (mb.exists()) redraw();
	return m;
}

int fltk3::MenuButton::handle(int e)
{
	if (!menu() || !menu()->text) return 0;
	switch (e) {
	case fltk3::ENTER: /* FALLTHROUGH */
	case fltk3::LEAVE:
		return (box() && !type()) ? 1 : 0;
	case fltk3::PUSH:
		if (!box()) {
			if (fltk3::event_button() != 3) return 0;
		} else if (type()) {
			if (!(type() & (1 << (fltk3::event_button()-1)))) return 0;
		}
		if (fltk3::visible_focus()) fltk3::focus(this);
		popup();
		return 1;
	case fltk3::KEYBOARD:
		if (!box()) return 0;
		if (fltk3::event_key() == ' ' &&
		    !(fltk3::event_state() & (fltk3::SHIFT | fltk3::CTRL | fltk3::ALT | fltk3::META))) {
			popup();
			return 1;
		} else return 0;
	case fltk3::SHORTCUT:
		if (Widget::test_shortcut()) {
			popup();
			return 1;
		}
		return test_shortcut() != 0;
	case fltk3::FOCUS: /* FALLTHROUGH */
	case fltk3::UNFOCUS:
		if (box() && fltk3::visible_focus()) {
			redraw();
			return 1;
		}
	default:
		return 0;
	}
}

/**
  Creates a new fltk3::MenuButton widget using the given position,
  size, and label string. The default boxtype is fltk3::UP_BOX.
  <P>The constructor sets menu() to NULL.  See
  fltk3::Menu_ for the methods to set or change the menu.
*/
fltk3::MenuButton::MenuButton(int X,int Y,int W,int H,const char *l) : fltk3::Menu_(X,Y,W,H,l)
{
	down_box(fltk3::NO_BOX);
}

//
// End of "$Id: MenuButton.cxx 9669 2012-08-16 21:41:30Z matt $".
//
