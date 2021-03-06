//
// "$Id: Choice.cxx 9638 2012-07-24 04:41:38Z matt $"
//
// Choice widget for the Fast Light Tool Kit (FLTK).
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
#include "Choice.h"
#include "draw.h"
#include "flstring.h"

// Emulates the Forms choice widget.  This is almost exactly the same
// as an fltk3::MenuButton.  The only difference is the appearance of the
// button: it draws the text of the current pick and a down-arrow.

void fltk3::Choice::draw()
{
	int dx = fltk3::box_dx(fltk3::DOWN_BOX);
	int dy = fltk3::box_dy(fltk3::DOWN_BOX);
	int H = h() - 2 * dy;
	int W = (H > 20) ? 20 : H;
	int X = w() - W - dx;
	int Y = dy;
	int w1 = (W - 4) / 3;
	if (w1 < 1) w1 = 1;
	int x1 = X + (W - 2 * w1 - 1) / 2;
	int y1 = Y + (H - w1 - 1) / 2;

	if (fltk3::scheme()) {
		draw_box(fltk3::UP_BOX, color());

		fltk3::color(active_r() ? labelcolor() : fltk3::inactive(labelcolor()));
		if (!strcmp(fltk3::scheme(), "plastic")) {
			// Show larger up/down arrows...
			fltk3::polygon(x1, y1 + 3, x1 + w1, y1 + w1 + 3, x1 + 2 * w1, y1 + 3);
			fltk3::polygon(x1, y1 + 1, x1 + w1, y1 - w1 + 1, x1 + 2 * w1, y1 + 1);
		} else {
			// Show smaller up/down arrows with a divider...
			x1 = w() - 13 - dx;
			y1 = h() / 2;
			fltk3::polygon(x1, y1 - 2, x1 + 3, y1 - 5, x1 + 6, y1 - 2);
			fltk3::polygon(x1, y1 + 2, x1 + 3, y1 + 5, x1 + 6, y1 + 2);

			fltk3::color(fltk3::darker(color()));
			fltk3::yxline(x1 - 7, y1 - 8, y1 + 8);

			fltk3::color(fltk3::lighter(color()));
			fltk3::yxline(x1 - 6, y1 - 8, y1 + 8);
		}
	} else {
		if (fltk3::contrast(textcolor(), fltk3::BACKGROUND2_COLOR) == textcolor()) {
			fltk3::draw_box(fltk3::DOWN_BOX, 0, 0, X, h(), fltk3::BACKGROUND2_COLOR, Box::TIE_RIGHT|Box::TIE_HIDDEN);
		} else {
			fltk3::draw_box(fltk3::DOWN_BOX, 0, 0, X, h(), fltk3::lighter(color()), Box::TIE_RIGHT|Box::TIE_HIDDEN);
		}
		fltk3::draw_box(fltk3::UP_BOX,X,0,w()-X,h(),color(),Box::TIE_LEFT|Box::TIE_HIDDEN);

		fltk3::color(active_r() ? labelcolor() : fltk3::inactive(labelcolor()));
		fltk3::polygon(x1, y1, x1 + w1, y1 + w1, x1 + 2 * w1, y1);
	}

	W += 2 * dx;

	if (mvalue()) {
		fltk3::MenuItem m = *mvalue();
		if (active_r()) m.activate();
		else m.deactivate();

		// ERCO
		int xx = dx, yy = dy + 1, ww = w() - W, hh = H - 2;

		fltk3::push_clip(xx, yy, ww, hh);

		if ( fltk3::scheme()) {
			fltk3::Label l;
			l.label(m.text);
			l.image(0);
			l.deimage(0);
			l.labeltype( m.labeltype() );
			l.labelfont( m.labelsize() || m.labelfont() ? m.labelfont() : textfont() );
			l.labelsize( m.labelsize() ? m.labelsize() : textsize() );
			l.labelcolor( m.labelcolor() ? m.labelcolor() : textcolor() );
			if (!m.active()) l.labelcolor( fltk3::inactive((fltk3::Color)l.labelcolor()) );
			fltk3::draw_shortcut = 2; // hack value to make '&' disappear
			l.draw(xx+3, yy, ww>6 ? ww-6 : 0, hh, fltk3::ALIGN_LEFT);
			fltk3::draw_shortcut = 0;
			if ( fltk3::focus() == this ) draw_focus(box(), xx, yy, ww, hh);
		} else {
			fltk3::draw_shortcut = 2; // hack value to make '&' disappear
			m.draw(xx, yy, ww, hh, this, fltk3::focus() == this);
			fltk3::draw_shortcut = 0;
		}

		fltk3::pop_clip();
	}

	draw_label();
}

/**
  Create a new fltk3::Choice widget using the given position, size and label string.
  The default boxtype is \c fltk3::UP_BOX.

  The constructor sets menu() to NULL.
  See fltk3::Menu_ for the methods to set or change the menu.

  \param[in] X, Y, W, H position and size of the widget
  \param[in] L widget label, default is no label
 */
fltk3::Choice::Choice(int X, int Y, int W, int H, const char *L)
	: fltk3::Menu_(X,Y,W,H,L)
{
	align(fltk3::ALIGN_LEFT);
	when(fltk3::WHEN_RELEASE);
	textfont(fltk3::HELVETICA);
	box(fltk3::FLAT_BOX);
	down_box(fltk3::BORDER_BOX);
}

/**
  Sets the currently selected value using a pointer to menu item.
  Changing the selected value causes a redraw().
  \param[in] v pointer to menu item in the menu item array.
  \returns non-zero if the new value is different to the old one.
 */
int fltk3::Choice::value(const fltk3::MenuItem *v)
{
	if (!Menu_::value(v)) return 0;
	redraw();
	return 1;
}

/**
  Sets the currently selected value using the index into the menu item array.
  Changing the selected value causes a redraw().
  \param[in] v index of value in the menu item array.
  \returns non-zero if the new value is different to the old one.
 */
int fltk3::Choice::value(int v)
{
	if (v == -1) return value((const fltk3::MenuItem *)0);
	if (v < 0 || v >= (size() - 1)) return 0;
	if (!Menu_::value(v)) return 0;
	redraw();
	return 1;
}

int fltk3::Choice::handle(int e)
{
	if (!menu() || !menu()->text) return 0;
	const fltk3::MenuItem* v;
	switch (e) {
	case fltk3::ENTER:
	case fltk3::LEAVE:
		return 1;

	case fltk3::KEYBOARD:
		if (fltk3::event_key() != ' ' ||
		    (fltk3::event_state() & (fltk3::SHIFT | fltk3::CTRL | fltk3::ALT | fltk3::META))) return 0;
	case fltk3::PUSH:
		if (fltk3::visible_focus()) fltk3::focus(this);
J1:
		if (fltk3::scheme() || fltk3::contrast(textcolor(), fltk3::BACKGROUND2_COLOR) != textcolor()) {
			v = menu()->pulldown(dx_window(), dy_window(), w(), h(), mvalue(), this);
		} else {
			// In order to preserve the old look-n-feel of "white" menus,
			// temporarily override the color() of this widget...
			fltk3::Color c = color();
			color(fltk3::BACKGROUND2_COLOR);
			v = menu()->pulldown(dx_window(), dy_window(), w(), h(), mvalue(), this);
			color(c);
		}
		if (!v || v->submenu()) return 1;
		if (v != mvalue()) redraw();
		picked(v);
		return 1;
	case fltk3::SHORTCUT:
		if (Widget::test_shortcut()) goto J1;
		v = menu()->test_shortcut();
		if (!v) return 0;
		if (v != mvalue()) redraw();
		picked(v);
		return 1;
	case fltk3::FOCUS:
	case fltk3::UNFOCUS:
		if (fltk3::visible_focus()) {
			redraw();
			return 1;
		} else return 0;
	default:
		return 0;
	}
}

//
// End of "$Id: Choice.cxx 9638 2012-07-24 04:41:38Z matt $".
//
