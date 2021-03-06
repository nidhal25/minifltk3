//
// "$Id: WizardGroup.h 9638 2012-07-24 04:41:38Z matt $"
//
// fltk3::WizardGroup widget definitions.
//
// Copyright 1999-2010 by Easy Software Products.
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
 fltk3::WizardGroup widget . */

//
// Include necessary header files...
//

#ifndef _Fltk3_Wizard_H_
#define _Fltk3_Wizard_H_

#include "Group.h"
#include "Window.h"
#include "draw.h"

namespace fltk3
{

/**
 This widget is based off the fltk3::TabGroup
 widget, but instead of displaying tabs it only changes "tabs" under
 program control. Its primary purpose is to support "wizards" that
 step a user through configuration or troubleshooting tasks.

 As with fltk3::TabGroup, wizard panes are composed of child (usually
 fltk3::Group) widgets. Navigation buttons must be added separately.
 */
class FLTK3_EXPORT WizardGroup : public fltk3::Group
{

protected:

	fltk3::Widget *value_;

	/**
	Draws the wizard border and visible child.
	*/
	void draw()
	{
		fltk3::Widget *kid;	// Visible child
		kid = value();
		if (damage() & fltk3::DAMAGE_ALL) {
			// Redraw everything...
			if (kid) {
				draw_box(box(), x(), y(), w(), h(), kid->color());
				draw_child(*kid);
			} else {
				draw_box(box(), x(), y(), w(), h(), color());
			}
		} else if (kid) {
			update_child(*kid);
		}
	}

public:
	/**
	The constructor creates the fltk3::WizardGroup widget at the specified
	position and size.
	\param xx, yy, ww, hh position and size
	\param l label text
	*/
	WizardGroup(int xx, int yy, int ww, int hh, const char *l=0) : fltk3::Group(xx, yy, ww, hh, l)
	{
		box(fltk3::THIN_UP_BOX);
		value_ = (fltk3::Widget *)0;
	}

	/**
	This method shows the next child of the wizard. If the last child
	is already visible, this function does nothing.
	*/
	void next()
	{
		int num_kids;
		fltk3::Widget * const *kids;

		if ((num_kids = children()) == 0) return;

		for (kids = array(); num_kids > 0; kids ++, num_kids --) {
			if ((*kids)->visible()) break;
		}

		if (num_kids > 1) value(kids[1]);
	}

	/**
	Shows the previous child.
	*/
	void prev()
	{
		int num_kids;
		fltk3::Widget * const *kids;

		if ((num_kids = children()) == 0) return;

		for (kids = array(); num_kids > 0; kids ++, num_kids --) {
			if ((*kids)->visible()) break;
		}

		if (num_kids > 0 && num_kids < children()) value(kids[-1]);
	}

	/**
	Gets the current visible child widget.
	*/
	fltk3::Widget *value()
	{
		int num_kids;
		fltk3::Widget * const *kids;
		fltk3::Widget *kid;

		if ((num_kids = children()) == 0) return ((fltk3::Widget *)0);

		for (kids = array(), kid = (fltk3::Widget *)0; num_kids > 0; kids ++, num_kids --) {
			if ((*kids)->visible()) {
				if (kid) (*kids)->hide();
				else kid = *kids;
			}
		}

		if (!kid) {
			kids --;
			kid = *kids;
			kid->show();
		}

		return (kid);
	}

	/**
	Sets the child widget that is visible.
	*/
	void value(fltk3::Widget *kid)
	{
		int num_kids;
		fltk3::Widget * const *kids;

		if ((num_kids = children()) == 0) return;

		for (kids = array(); num_kids > 0; kids ++, num_kids --) {
			if (*kids == kid) {
				if (!kid->visible()) kid->show();
			} else {
				(*kids)->hide();
			}
		}

		// This will restore the mouse pointer to the window's default cursor
		// whenever the wizard pane is changed.  Otherwise text widgets that
		// show the next pane may leave the cursor set to the I beam, etc...
		if (window()) window()->cursor(fltk3::CURSOR_DEFAULT);
	}
};

}

#endif // !_Fl_Wizard_H_

//
// End of "$Id: WizardGroup.h 9638 2012-07-24 04:41:38Z matt $".
//
