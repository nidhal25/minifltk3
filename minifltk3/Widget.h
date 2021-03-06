//
// "$Id: Widget.h 9794 2013-01-14 17:23:42Z manolo $"
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

/** \file
 fltk3::Widget, fltk3::Label classes . */

#ifndef Fltk3_Widget_H
#define Fltk3_Widget_H

#include "enumerations.h"
#include "Rectangle.h"
#include <stdlib.h>
#include "src/flstring.h"

/**
 \todo	typedef's fl_intptr_t and fl_uintptr_t should be documented.
 */
#ifdef _WIN64
#ifdef __GNUC__
#include <stdint.h>
#else
#include <stddef.h>  // M$VC
#endif
typedef intptr_t fl_intptr_t;
typedef uintptr_t fl_uintptr_t;
#else
typedef long fl_intptr_t;
typedef unsigned long fl_uintptr_t;
#endif

namespace fltk3
{
class Widget;
class Group;
class Window;
class ShapedWindow;
class Image;
class GLWindow;
//class Style;
class Box;

/** Default callback type definition for all fltk widgets (by far the most used) */
typedef void (Callback)(Widget*, void*);
/** Default callback type pointer definition for all fltk widgets */
typedef Callback* CallbackPtr; // needed for BORLAND
/** One parameter callback type definition passing only the widget */
typedef void (Callback0)(Widget*);
/** Callback type definition passing the widget and a long data value */
typedef void (Callback1)(Widget*, long);

/** This struct stores all information for a text or mixed graphics label.

 \todo For FLTK 1.3, the fltk3::Label type will become a widget by itself. That way
 we will be avoiding a lot of code duplication by handling labels in
 a similar fashion to widgets containing text. We also provide an easy
 interface for very complex labels, containing html or vector graphics.
 */
class FLTK3_EXPORT Label : public Rectangle
{
private:
	Box *box_, *down_box_;

	Color labelcolor_;
	unsigned int labelcolor_set_:1;

	Font labelfont_;
	unsigned int labelfont_set_:1;

	Fontsize labelsize_;
	unsigned int labelsize_set_:1;

	Color textcolor_;
	unsigned int textcolor_set_:1;

	Fontsize textsize_;
	unsigned int textsize_set_:1;

	Labeltype labeltype_; // FIXME: does this belong here?
	unsigned int labeltype_set_:1;

	Font textfont_;
	unsigned int textfont_set_:1;

	Align align_;
	unsigned int align_set_:1;

	Color color_;
	Color color2_;
	unsigned int color_set_:1;
	unsigned int color2_set_:1;

protected:
	/** label text */
	const char* labeltext_;
	/** various flags for the label and derived classes */
	unsigned int flags_;
	/** optional image for an active label */
	fltk3::Image* image_;
	/** optional image for a deactivated label */
	fltk3::Image* deimage_;
	/** optional image when dragging */
	fltk3::Image* dragimage_;

	void ParamInit()
	{
		labelfont_ = fltk3::HELVETICA;
		labelsize_ = fltk3::NORMAL_SIZE;
		labeltype_ = fltk3::NORMAL_LABEL;
		labelcolor_ = fltk3::FOREGROUND_COLOR;
		//labelcolor_ = fltk3::WHITE;
		align_ = fltk3::ALIGN_CENTER;
		textfont_ = fltk3::HELVETICA;
		textsize_ = fltk3::NORMAL_SIZE;
		textcolor_ = fltk3::FOREGROUND_COLOR;
		color_ = fltk3::GRAY;
		//color_ = fltk3::BLACK;
		color2_ = fltk3::GRAY;
		//color2_ = fltk3::BLACK;
		box_ = fltk3::NO_BOX;
		down_box_ = fltk3::NO_BOX;
		labelfont_set_ = 1;
		labelsize_set_ = 1;
		labeltype_set_ = 1;
		labelcolor_set_ = 1;
		align_set_ = 1;
		textfont_set_ = 1;
		textsize_set_ = 1;
		textcolor_set_ = 1;
		color_set_ = 1;
		color2_set_ = 1;
	}

public:
	Label(int X, int Y, int W, int H, const char* L=0L) : fltk3::Rectangle(X, Y, W, H), labeltext_(L), flags_(0), image_(0), deimage_(0), dragimage_(0) {ParamInit();}

	Label(const fltk3::Label &s)
		: fltk3::Rectangle(s),
		  labeltext_(0L),
		  flags_(s.flags_),
		  image_(s.image_),
		  deimage_(s.deimage_) {
		ParamInit();
		if (s.labeltext_) {
			if (s.flags() & COPIED_LABEL) {
				copy_label(s.labeltext_);
			} else {
				labeltext_ = s.labeltext_;
			}
		}
	}

	Label(): fltk3::Rectangle(), /*style_(&fltk3::default_style),*/ flags_(0), image_(0), deimage_(0) {ParamInit();}

	~Label() {
		if (flags() & COPIED_LABEL) free((void *)(labeltext_));
		//if (style_ && style_->is_private())
		//  delete style_;
	}

	/** Draws the label aligned to the given box */
	void draw(int,int,int,int, fltk3::Align) const ;

	void measure(int &w, int &h) const ;

	/** Gets the current label text.
	 \return a pointer to the current label text
	 \see label(const char *), copy_label(const char *)
	 */
	const char* label() const {
		return labeltext_;
	}

	/** Sets the current label pointer.

	 The label is shown somewhere on or next to the widget. The passed pointer
	 is stored unchanged in the widget (the string is \em not copied), so if
	 you need to set the label to a formatted value, make sure the buffer is
	 static, global, or allocated. The copy_label() method can be used
	 to make a copy of the label string automatically.
	 \param[in] text pointer to new label text
	 \see copy_label()
	 */
	void label(const char* text) {
		if (flags() & COPIED_LABEL) {
			// reassigning a copied label remains the same copied label
			if (labeltext_ == text) return;
			free((void *)(labeltext_));
			clear_flag(COPIED_LABEL);
		}
		labeltext_ = text;
		redraw_label();
	}

	/** Sets the current label.
	 Unlike label(), this method allocates a copy of the label
	 string instead of using the original string pointer.

	 The internal copy will automatically be freed whenever you assign
	 a new label or when the widget is destroyed.

	 \param[in] new_label the new label text
	 \see label()
	 */
	void copy_label(const char *new_label) {
		// reassigning a copied label remains the same copied label
		if ((flags() & COPIED_LABEL) && (labeltext_ == new_label)) return;
		if (new_label) {
			label(strdup(new_label));
			set_flag(COPIED_LABEL);
		} else {
			label(0);
		}
	}

	/** Shortcut to set the label text and type in one call.
	 \see label(const char *), labeltype(fltk3::Labeltype)
	 */
	void label(fltk3::Labeltype a, const char* b) {
		labeltype(a);
		labeltext_ = b;
	}

	/** Gets the label color.
	 The default color is fltk3::FOREGROUND_COLOR.
	 \return the current label color
	 */
	fltk3::Color labelcolor() const {
		return labelcolor_;
		//return style()->labelcolor();
	}

	/** Sets the label color.
	 The default color is fltk3::FOREGROUND_COLOR.
	 \param[in] c the new label color
	 */
	void labelcolor(fltk3::Color c) {
		labelcolor_=c;
		labelcolor_set_ = 1;
		//private_style()->labelcolor(c);
	}

	/** Gets the font to use.
	 Fonts are identified by indexes into a table. The default value
	 uses a Helvetica typeface (Arial for Microsoft&reg; Windows&reg;).
	 The function fltk3::set_font() can define new typefaces.
	 \return current font used by the label
	 \see fltk3::Font
	 */
	fltk3::Font labelfont() const {
		return labelfont_;
		//return style()->labelfont();
	}

	/** Sets the font to use.
	 Fonts are identified by indexes into a table. The default value
	 uses a Helvetica typeface (Arial for Microsoft&reg; Windows&reg;).
	 The function fltk3::set_font() can define new typefaces.
	 \param[in] f the new font for the label
	 \see fltk3::Font
	 */
	void labelfont(fltk3::Font f) {
		labelfont_=f;
		labelfont_set_ = 1;
		//private_style()->labelfont(f);
	}

	/** Gets the font size in pixels.
	 The default size is 14 pixels.
	 \return the current font size
	 */
	fltk3::Fontsize labelsize() const {
		return labelsize_;
		//return style()->labelsize();
	}

	/** Sets the font size in pixels.
	 \param[in] pix the new font size
	 \see fltk3::Fontsize labelsize()
	 */
	void labelsize(fltk3::Fontsize pix) {
		labelsize_=pix;
		labelsize_set_ = 1;
		//private_style()->labelsize(pix);
	}

	/** Gets the label type.
	 \return the current label type.
	 \see fltk3::Labeltype
	 */
	fltk3::Labeltype labeltype() const {
		return labeltype_;
		//return style()->labeltype();
	}

	/** Sets the label type.
	 The label type identifies the function that draws the label of the widget.
	 This is generally used for special effects such as embossing or for using
	 the label() pointer as another form of data such as an icon. The value
	 fltk3::NORMAL_LABEL prints the label as plain text.
	 \param[in] a new label type
	 \see fltk3::Labeltype
	 */
	void labeltype(Labeltype a) {
		labeltype_ = a;
		labeltype_set_ = 1;
		//private_style()->labeltype(a);
	}

	/** Gets the label alignment.
	 \return label alignment
	 \see label(), align(fltk3::Align), fltk3::Align
	 */
	Align align() const {
		return align_;
		//return style()->align();
	}

	/** Sets the label alignment.
	 This controls how the label is displayed next to or inside the widget.
	 The default value is fltk3::ALIGN_CENTER, which centers the label inside
	 the widget.
	 \param[in] alignment new label alignment
	 \see align(), fltk3::Align
	 */
	void align(Align alignment) {
		align_=alignment;
		align_set_ = 1;
		//private_style()->align(alignment);
	}

	/** Gets the box type of the widget.
	 \return the current box type
	 \see box(fltk3::Boxtype), fltk3::Boxtype
	 */
	Box* box() const { return box_; }

	/** Sets the box type for the widget.
	 This identifies a routine that draws the background of the widget.
	 See fltk3::Boxtype for the available types. The default depends on the
	 widget, but is usually fltk3::NO_BOX or fltk3::UP_BOX.
	 \param[in] new_box the new box type
	 \see box(), fltk3::Boxtype
	 */
	void box(Box* b) { box_= b; }

	Box *down_box() const { return down_box_; }
	void down_box(Box* b) { down_box_ = b; }

	/** Gets the background color of the widget.
	 \return current background color
	 \see color(fltk3::Color), color(fltk3::Color, fltk3::Color)
	 */
	Color color() const {
		return color_;
		//return style()->color();
	}

	/** Sets the background color of the widget.
	 The color is passed to the box routine. The color is either an index into
	 an internal table of RGB colors or an RGB color value generated using
	 fltk3::rgb_color().

	 The default for most widgets is fltk3::BACKGROUND_COLOR. Use fltk3::set_color()
	 to redefine colors in the color map.
	 \param[in] bg background color
	 \see color(), color(fltk3::Color, fltk3::Color), selection_color(fltk3::Color)
	 */
	void color(Color bg) {
		color_ = bg;
		color_set_ = 1;
		//private_style()->color(bg);
	}

	/** Gets the selection color.
	 \return the current selection color
	 \see selection_color(fltk3::Color), color(fltk3::Color, fltk3::Color)
	 */
	fltk3::Color selection_color() const {
		return color2_;
		//return style()->color2();
	}

	/** Sets the selection color.
	 The selection color is defined for Forms compatibility and is usually
	 used to color the widget when it is selected, although some widgets
	 use this color for other purposes. You can set both colors at once
	 with color(fltk3::Color bg, fltk3::Color sel).
	 \param[in] a the new selection color
	 \see selection_color(), color(fltk3::Color, fltk3::Color)
	 */
	void selection_color(fltk3::Color a) {
		color2_ = a;
		color2_set_ = 1;
		//private_style()->color2(a);
	}

	/** Sets the background and selection color of the widget.

	 The two color form sets both the background and selection colors.
	 \param[in] bg background color
	 \param[in] sel selection color
	 \see color(unsigned), selection_color(unsigned)
	 */
	void color(fltk3::Color bg, fltk3::Color sel) {
		color_=bg;
		color_set_ = 1;
		//private_style()->color(bg);
		color2_ = sel;
		color2_set_ = 1;
		//private_style()->color2(sel);
	}

	/** Gets the widget flags mask */
	unsigned int flags() const {
		return flags_;
	}

	/** Sets a flag in the flags mask */
	void set_flag(unsigned int c) {
		flags_ |= c;
	}

	/** Clears a flag in the flags mask */
	void clear_flag(unsigned int c) {
		flags_ &= ~c;
	}

	/** flags possible values enumeration. */
	enum {
		INACTIVE        = 1<<0,   ///< the widget can't receive focus, and is disabled but potentially visible
		SHORTCUT_LABEL  = 1<<6,   ///< the label contains a shortcut we need to draw
		COPIED_LABEL    = 1<<10   ///< the widget label is internally copied, its destruction is handled by the widget
	};

	/** Schedules the drawing of the label.
	 Marks the widget or the parent as needing a redraw for the label area
	 of a widget.
	 */
	virtual void redraw_label() { }

	/** Gets the font of the text in the input field.
	 \return the current fltk3::Font index */
	fltk3::Font textfont() const {
		return textfont_;
		//return style()->textfont();
	}

	/** Sets the font of the text in the input field.
	 The text font defaults to \c fltk3::HELVETICA.
	 \param [in] s the new text font */
	void textfont(fltk3::Font s) {
		textfont_ = s;
		textfont_set_ = 1;
		//private_style()->textfont(s);
	}

	/** Gets the size of the text in the input field.
	 \return the text height in pixels */
	fltk3::Fontsize textsize() const {
		return textsize_;
		//return style()->textsize();
	}

	/** Sets the size of the text in the input field.
	 The text height defaults to \c fltk3::NORMAL_SIZE.
	 \param [in] s the new font height in pixel units */
	void textsize(fltk3::Fontsize s) {
		textsize_ = s;
		textsize_set_ = 1;
		//private_style()->textsize(s);
	}

	/** Gets the color of the text in the input field.
	 \return the text color
	 \see textcolor(fltk3::Color) */
	fltk3::Color textcolor() const {
		return textcolor_;
		//return style()->textcolor();
	}

	/** Sets the color of the text in the input field.
	 The text color defaults to \c fltk3::FOREGROUND_COLOR.
	 \param [in] n new text color
	 \see textcolor() */
	void textcolor(fltk3::Color n) {
		textcolor_ = n;
		textcolor_set_ = 1;
		//private_style()->textcolor(n);
	}

	/** Gets the image that is used as part of the widget label.
	 This image is used when drawing the widget in the active state.
	 \return the current image
	 */
	fltk3::Image* image() {
		return image_;
	}
	const fltk3::Image* image() const {
		return image_;
	}

	/** Sets the image to use as part of the widget label.
	 This image is used when drawing the widget in the active state.
	 \param[in] img the new image for the label
	 */
	void image(fltk3::Image* img) {
		image_ = img;
	}

	/** Sets the image to use as part of the widget label.
	 This image is used when drawing the widget in the active state.
	 \param[in] img the new image for the label
	 */
	void image(fltk3::Image& img) {
		image_=&img;
	}

	/** Gets the image that is used as part of the widget label.
	 This image is used when drawing the widget in the inactive state.
	 \return the current image for the deactivated widget
	 */
	fltk3::Image* deimage() {
		return deimage_;
	}
	const fltk3::Image* deimage() const {
		return deimage_;
	}

	/** Sets the image to use as part of the widget label.
	 This image is used when drawing the widget in the inactive state.
	 \param[in] img the new image for the deactivated widget
	 */
	void deimage(fltk3::Image* img) {
		deimage_=img;
	}

	/** Sets the image to use as part of the widget label.
	 This image is used when drawing the widget in the inactive state.
	 \param[in] img the new image for the deactivated widget
	 */
	void deimage(fltk3::Image& img) {
		deimage_=&img;
	}

};


/** fltk3::Widget is the base class for all widgets in FLTK.

 You can't create one of these because the constructor is not public.
 However you can subclass it.

 All "property" accessing methods, such as color(), parent(), or argument()
 are implemented as trivial inline functions and thus are as fast and small
 as accessing fields in a structure. Unless otherwise noted, the property
 setting methods such as color(n) or label(s) are also trivial inline
 functions, even if they change the widget's appearance. It is up to the
 user code to call redraw() after these.
 */
class FLTK3_EXPORT Widget : public Label
{
	friend class Group;

private:

	fltk3::Group* parent_;
	fltk3::Callback* callback_;
	union {
		void* user_data_;
		long long_value_;
	};
	uchar type_;
	uchar damage_;
	uchar when_;
	const char *tooltip_;

	/** unimplemented copy ctor */
	Widget(const Widget &);
	/** unimplemented assignment operator */
	Widget& operator=(const Widget &);

protected:

	/** flags possible values enumeration.
	 See activate(), output(), visible(), changed(), set_visible_focus()
	 */
	enum {
		INVISIBLE       = 1<<1,   ///< the widget is not drawn, but can receive a few special events
		OUTPUT          = 1<<2,   ///< for output only
		NOBORDER        = 1<<3,   ///< don't draw a decoration (fltk3::Window)
		FORCE_POSITION  = 1<<4,   ///< don't let the window manager position the window (fltk3::Window)
		NON_MODAL       = 1<<5,   ///< this is a hovering toolbar window (fltk3::Window)
		CHANGED         = 1<<7,   ///< the widget value changed
		OVERRIDE        = 1<<8,   ///< position window on top (fltk3::Window)
		VISIBLE_FOCUS   = 1<<9,   ///< accepts keyboard focus navigation if the widget can have the focus
		CLIP_CHILDREN   = 1<<11,  ///< all drawing within this widget will be clipped (fltk3::Group)
		MENU_WINDOW     = 1<<12,  ///< a temporary popup window, dismissed by clicking outside (fltk3::Window)
		TOOLTIP_WINDOW  = 1<<13,  ///< a temporary popup, transparent to events, and dismissed easily (fltk3::Window)
		MODAL           = 1<<14,  ///< a window blocking input to all other winows (fltk3::Window)
		NO_OVERLAY      = 1<<15,  ///< window not using a hardware overlay plane (fltk3::MenuWindow)
		COPIED_TOOLTIP  = 1<<17,  ///< the widget tooltip is internally copied, its destruction is handled by the widget
		FULLSCREEN      = 1<<18,  ///< a fullscreen window (Fl_Window)
		MAC_USE_ACCENTS_MENU = 1<<19, ///< On the Mac OS platform, pressing and holding a key on the keyboard opens an accented-character menu window (Fl_Input_, Fl_Text_Editor)
		// (space for more flags)
		USERFLAG3       = 1<<29,  ///< reserved for 3rd party extensions
		USERFLAG2       = 1<<30,  ///< reserved for 3rd party extensions
		USERFLAG1       = 1<<31   ///< reserved for 3rd party extensions
	};

	/** Draws the widget box according its box style */
	void draw_box() const
	{
		if (box()) {
			draw_box(box(), 0, 0, w_, h_, color());
		}
		draw_backdrop();
	}
	/* Draws a box of type t, of color c at the widget's position and size. */
	void draw_box(fltk3::Box* t, Color c) const { draw_box(t, 0, 0, w_, h_, c); }
	/** Draws a box of type t, of color c at the position X,Y and size W,H. */
	void draw_box(fltk3::Box* t, int X, int Y, int W, int H, fltk3::Color c) const;
	/** If fltk3::ALIGN_IMAGE_BACKDROP is set, the image or deimage will be drawn */
	void draw_backdrop() const;
	void draw_focus();
	void draw_focus(Box* t, int x,int y,int w,int h) const;
	void draw_label() const;
	void draw_label(int, int, int, int) const;

public:

	/** Creates a widget at the given position and size.

	 The fltk3::Widget is a protected constructor, but all derived widgets have a
	 matching public constructor. It takes a value for x(), y(), w(), h(), and
	 an optional value for label().

	 \param[in] x, y the position of the widget relative to the enclosing window
	 \param[in] w, h size of the widget in pixels
	 \param[in] label optional text for the widget label
	 */
	Widget(int x, int y, int w, int h, const char *label=0L);

	Widget(fltk3::Box* b, int x, int y, int w, int h, const char *label=0L);

	/** Destroys the widget.
	 Destroying single widgets is not very common. You almost always want to
	 destroy the parent group instead, which will destroy all of the child widgets
	 and groups in that group.

	 \since FLTK 1.3, the widget's destructor removes the widget from its parent
	 group, if it is member of a group.
	 */
	virtual ~Widget();

	/** Draws the widget.
	 Never call this function directly. FLTK will schedule redrawing whenever
	 needed. If your widget must be redrawn as soon as possible, call redraw()
	 instead.

	 Override this function to draw your own widgets.

	 If you ever need to call another widget's draw method <I>from within your
	 own draw() method</I>, e.g. for an embedded scrollbar, you can do it
	 (because draw() is virtual) like this:

	 \code
	 fltk3::Widget *s = &scroll;		// scroll is an embedded fltk3::Scrollbar
	 s->draw();			// calls fltk3::Scrollbar::draw()
	 \endcode
	 */
	virtual void draw();

	/** Handles the specified event.
	 You normally don't call this method directly, but instead let FLTK do
	 it when the user interacts with the widget.

	 When implemented in a widget, this function must return 0 if the
	 widget does not use the event or 1 otherwise.

	 Most of the time, you want to call the inherited handle() method in
	 your overridden method so that you don't short-circuit events that you
	 don't handle. In this last case you should return the callee retval.

	 \param[in] event the kind of event received
	 \retval 0 if the event was not used or understood
	 \retval 1 if the event was used and can be deleted
	 \see fltk3::Event
	 */
	virtual int handle(int event);

	int send(int event);

	/** Returns a pointer to the parent widget.
	 Usually this is a fltk3::Group or fltk3::Window.
	 \retval NULL if the widget has no parent
	 \see fltk3::Group::add(fltk3::Widget*)
	 */
	fltk3::Group* parent() const {
		return parent_;
	}

	/** Internal use only - "for hacks only".

	 It is \em \b STRONGLY recommended not to use this method, because it
	 short-circuits fltk3::Group's normal widget adding and removing methods,
	 if the widget is already a child widget of another fltk3::Group.

	 Use fltk3::Group::add(fltk3::Widget*) and/or fltk3::Group::remove(fltk3::Widget*) instead.
	 */
	void parent(fltk3::Group* p) {
		parent_ = p;       // for hacks only, use fltk3::Group::add()
	}

	/** Gets the widget type.
	 Returns the widget type value, which is used for Forms compatibility
	 and to simulate RTTI.

	 \todo Explain "simulate RTTI" (currently only used to decide if a widget
	 is a window, i.e. type()>=fltk3::WINDOW ?). Is type() really used in a way
	 that ensures "Forms compatibility" ?
	 */
	uchar type() const {
		return type_;
	}

	/** Sets the widget type.
	 This is used for Forms compatibility.
	 */
	void type(uchar t) {
		type_ = t;
	}

	/** Changes the size or position of the widget.

	 This is a virtual function so that the widget may implement its
	 own handling of resizing. The default version does \e not
	 call the redraw() method, but instead relies on the parent widget
	 to do so because the parent may know a faster way to update the
	 display, such as scrolling from the old position.

	 Some window managers under X11 call resize() a lot more often
	 than needed. Please verify that the position or size of a widget
	 did actually change before doing any extensive calculations.

	 position(X, Y) is a shortcut for resize(X, Y, w(), h()),
	 and size(W, H) is a shortcut for resize(x(), y(), W, H).

	 \param[in] x, y new position relative to the parent window
	 \param[in] w, h new size
	 \see position(int,int), size(int,int)
	 */
	virtual void resize(int x, int y, int w, int h);

	/** Internal use only. */
	int damage_resize(int,int,int,int);

	/** Repositions the window or widget.

	 position(X, Y) is a shortcut for resize(X, Y, w(), h()).

	 \param[in] X, Y new position relative to the parent window
	 \see resize(int,int,int,int), size(int,int)
	 */
	void position(int X,int Y) {
		resize(X,Y,w_,h_);
	}

	/** Changes the size of the widget.

	 size(W, H) is a shortcut for resize(x(), y(), W, H).

	 \param[in] W, H new size
	 \see position(int,int), resize(int,int,int,int)
	 */
	void size(int W,int H) {
		resize(x_,y_,W,H);
	}

	/** Gets the current tooltip text.
	 \return a pointer to the tooltip text or NULL
	 \see tooltip(const char*), copy_tooltip(const char*)
	 */
	const char *tooltip() const {
		return tooltip_;
	}

	void tooltip(const char *text);		// see fltk3::Tooltip
	void copy_tooltip(const char *text);		// see fltk3::Tooltip

	/** Gets the current callback function for the widget.
	 Each widget has a single callback.
	 \return current callback
	 */
	CallbackPtr callback() const {
		return callback_;
	}

	/** Sets the current callback function for the widget.
	 Each widget has a single callback.
	 \param[in] cb new callback
	 \param[in] p user data
	 */
	void callback(fltk3::Callback* cb, void* p) {
		callback_=cb;
		user_data_=p;
	}

	/** Sets the current callback function for the widget.
	 Each widget has a single callback.
	 \param[in] cb new callback
	 */
	void callback(fltk3::Callback* cb) {
		callback_=cb;
	}

	/** Sets the current callback function for the widget.
	 Each widget has a single callback.
	 \param[in] cb new callback
	 */
	void callback(fltk3::Callback0*cb) {
		callback_=(fltk3::Callback*)cb;
	}

	/** Sets the current callback function for the widget.
	 Each widget has a single callback.
	 \param[in] cb new callback
	 \param[in] p user data
	 */
	void callback(fltk3::Callback1*cb, long p=0) {
		callback_=(fltk3::Callback*)cb;
		long_value_=p;
	}

	/** Gets the user data for this widget.
	 Gets the current user data (void *) argument that is passed to the callback function.
	 \return user data as a pointer
	 */
	void* user_data() const {
		return user_data_;
	}

	/** Sets the user data for this widget.
	 Sets the new user data (void *) argument that is passed to the callback function.
	 \param[in] v new user data
	 */
	void user_data(void* v) {
		user_data_ = v;
	}

	/** Gets the current user data (long) argument that is passed to the callback function.
	 */
	long argument() const {
		return long_value_;
	}

	/** Sets the current user data (long) argument that is passed to the callback function.
	 */
	void argument(long v) {
		long_value_ = v;
	}

	/** Returns the conditions under which the callback is called.

	 You can set the flags with when(uchar), the default value is
	 fltk3::WHEN_RELEASE.

	 \return set of flags
	 \see when(uchar)
	 */
	fltk3::When when() const {
		return (fltk3::When)when_;
	}

	/** Sets the flags used to decide when a callback is called.

	 This controls when callbacks are done. The following values are useful,
	 the default value is fltk3::WHEN_RELEASE:

	 \li 0: The callback is not done, but changed() is turned on.
	 \li fltk3::WHEN_CHANGED: The callback is done each time the text is
	 changed by the user.
	 \li fltk3::WHEN_RELEASE: The callback will be done when this widget loses
	 the focus, including when the window is unmapped. This is a useful
	 value for text fields in a panel where doing the callback on every
	 change is wasteful. However the callback will also happen if the
	 mouse is moved out of the window, which means it should not do
	 anything visible (like pop up an error message).
	 You might do better setting this to zero, and scanning all the
	 items for changed() when the OK button on a panel is pressed.
	 \li fltk3::WHEN_ENTER_KEY: If the user types the Enter key, the entire
	 text is selected, and the callback is done if the text has changed.
	 Normally the Enter key will navigate to the next field (or insert
	 a newline for a fltk3::MultilineInput) - this changes the behavior.
	 \li fltk3::WHEN_ENTER_KEY|fltk3::WHEN_NOT_CHANGED: The Enter key will do the
	 callback even if the text has not changed. Useful for command fields.
	 fltk3::Widget::when() is a set of bitflags used by subclasses of
	 fltk3::Widget to decide when to do the callback.

	 If the value is zero then the callback is never done. Other values
	 are described  in the individual widgets. This field is in the base
	 class so that you can scan a panel and do_callback() on all the ones
	 that don't do their own callbacks in response to an "OK" button.
	 \param[in] i set of flags
	 */
	void when(uchar i) {
		when_ = i;
	}

	/** Returns whether a widget is visible.
	 \retval 0 if the widget is not drawn and hence invisible.
	 \see show(), hide(), visible_r()
	 */
	unsigned int visible() const {
		return !(flags_&INVISIBLE);
	}

	/** Returns whether a widget and all its parents are visible.
	 \retval 0 if the widget or any of its parents are invisible.
	 \see show(), hide(), visible()
	 */
	int visible_r() const;

	/** Makes a widget visible.

	 An invisible widget never gets redrawn and does not get keyboard
	 or mouse events, but can receive a few other events like fltk3::SHOW.

	 The visible() method returns true if the widget is set to be
	 visible. The visible_r() method returns true if the widget and
	 all of its parents are visible. A widget is only visible if
	 visible() is true on it <I>and all of its parents</I>.

	 Changing it will send fltk3::SHOW or fltk3::HIDE events to the widget.
	 <I>Do not change it if the parent is not visible, as this
	 will send false fltk3::SHOW or fltk3::HIDE events to the widget</I>.
	 redraw() is called if necessary on this or the parent.

	 \see hide(), visible(), visible_r()
	 */
	virtual void show();

	/** Makes a widget invisible.
	 \see show(), visible(), visible_r()
	 */
	virtual void hide();

	/** Makes the widget visible.
	 You must still redraw the parent widget to see a change in the
	 window. Normally you want to use the show() method instead.
	 */
	void set_visible() {
		flags_ &= ~INVISIBLE;
	}

	/** Hides the widget.
	 You must still redraw the parent to see a change in the window.
	 Normally you want to use the hide() method instead.
	 */
	void clear_visible() {
		flags_ |= INVISIBLE;
	}

	/** Returns whether the widget is active.
	 \retval 0 if the widget is inactive
	 \see active_r(), activate(), deactivate()
	 */
	unsigned int active() const {
		return !(flags_&INACTIVE);
	}

	/** Returns whether the widget and all of its parents are active.
	 \retval 0 if this or any of the parent widgets are inactive
	 \see active(), activate(), deactivate()
	 */
	int active_r() const;

	/** Activates the widget.
	 Changing this value will send fltk3::ACTIVATE to the widget if
	 active_r() is true.
	 \see active(), active_r(), deactivate()
	 */
	void activate();

	/** Deactivates the widget.
	 Inactive widgets will be drawn "grayed out", e.g. with less contrast
	 than the active widget. Inactive widgets will not receive any keyboard
	 or mouse button events. Other events (including fltk3::ENTER, fltk3::MOVE,
	 fltk3::LEAVE, fltk3::SHORTCUT, and others) will still be sent. A widget is
	 only active if active() is true on it <I>and all of its parents</I>.

	 Changing this value will send fltk3::DEACTIVATE to the widget if
	 active_r() is true.

	 Currently you cannot deactivate fltk3::Window widgets.

	 \see activate(), active(), active_r()
	 */
	void deactivate();

	/** Returns if a widget is used for output only.
	 output() means the same as !active() except it does not change how the
	 widget is drawn. The widget will not receive any events. This is useful
	 for making scrollbars or buttons that work as displays rather than input
	 devices.
	 \retval 0 if the widget is used for input and output
	 \see set_output(), clear_output()
	 */
	unsigned int output() const {
		return (flags_&OUTPUT);
	}

	/** Sets a widget to output only.
	 \see output(), clear_output()
	 */
	void set_output() {
		flags_ |= OUTPUT;
	}

	/** Sets a widget to accept input.
	 \see set_output(), output()
	 */
	void clear_output() {
		flags_ &= ~OUTPUT;
	}

	/** Returns if the widget is able to take events.
	 This is the same as (active() && !output() && visible())
	 but is faster.
	 \retval 0 if the widget takes no events
	 */
	unsigned int takesevents() const {
		return !(flags_&(INACTIVE|INVISIBLE|OUTPUT));
	}

	/**
	 Checks if the widget value changed since the last callback.

	 "Changed" is a flag that is turned on when the user changes the value
	 stored in the widget. This is only used by subclasses of fltk3::Widget that
	 store values, but is in the base class so it is easier to scan all the
	 widgets in a panel and do_callback() on the changed ones in response
	 to an "OK" button.

	 Most widgets turn this flag off when they do the callback, and when
	 the program sets the stored value.

	 \retval 0 if the value did not change
	 \see set_changed(), clear_changed()
	 */
	unsigned int changed() const {
		return flags_&CHANGED;
	}

	/** Marks the value of the widget as changed.
	 \see changed(), clear_changed()
	 */
	void set_changed() {
		flags_ |= CHANGED;
	}

	/** Marks the value of the widget as unchanged.
	 \see changed(), set_changed()
	 */
	void clear_changed() {
		flags_ &= ~CHANGED;
	}

	/** Gives the widget the keyboard focus.
	 Tries to make this widget be the fltk3::focus() widget, by first sending
	 it an fltk3::FOCUS event, and if it returns non-zero, setting
	 fltk3::focus() to this widget. You should use this method to
	 assign the focus to a widget.
	 \return true if the widget accepted the focus.
	 */
	int take_focus();

	/** Enables keyboard focus navigation with this widget.
	 Note, however, that this will not necessarily mean that the widget
	 will accept focus, but for widgets that can accept focus, this method
	 enables it if it has been disabled.
	 \see visible_focus(), clear_visible_focus(), visible_focus(int)
	 */
	void set_visible_focus() {
		flags_ |= VISIBLE_FOCUS;
	}

	/** Disables keyboard focus navigation with this widget.
	 Normally, all widgets participate in keyboard focus navigation.
	 \see set_visible_focus(), visible_focus(), visible_focus(int)
	 */
	void clear_visible_focus() {
		flags_ &= ~VISIBLE_FOCUS;
	}

	/** Modifies keyboard focus navigation.
	 \param[in] v set or clear visible focus
	 \see set_visible_focus(), clear_visible_focus(), visible_focus()
	 */
	void visible_focus(int v) {
		if (v) set_visible_focus();
		else clear_visible_focus();
	}

	/** Checks whether this widget has a visible focus.
	 \retval 0 if this widget has no visible focus.
	 \see visible_focus(int), set_visible_focus(), clear_visible_focus()
	 */
	unsigned int visible_focus() {
		return flags_ & VISIBLE_FOCUS;
	}

	/** Sets the default callback for all widgets.
	 Sets the default callback, which puts a pointer to the widget on the queue
	 returned by fltk3::readqueue(). You may want to call this from your own callback.
	 \param[in] cb the new callback
	 \param[in] d user data associated with that callback
	 \see callback(), do_callback(), fltk3::readqueue()
	 */
	static void default_callback(fltk3::Widget *cb, void *d);

	/** Calls the widget callback.
	 Causes a widget to invoke its callback function with default arguments.
	 \see callback()
	 */
	void do_callback() {
		do_callback(this,user_data_);
	}

	//Causes a widget to invoke its callback function with arbitrary arguments.
	// Documentation and implementation in fltk3::Widget.cxx
	void do_callback(fltk3::Widget* o,long arg);

	// Causes a widget to invoke its callback function with arbitrary arguments.
	// Documentation and implementation in fltk3::Widget.cxx
	void do_callback(fltk3::Widget* o,void* arg=0);

	/* Internal use only. */
	int test_shortcut();
	void _set_fullscreen() {
		flags_ |= FULLSCREEN;
	}
	void _clear_fullscreen() {
		flags_ &= ~FULLSCREEN;
	}
	/* Internal use only. */
	static unsigned int label_shortcut(const char *t);
	/* Internal use only. */
	static int test_shortcut(const char*, const bool require_alt = false);

	/** Checks if w is a child of this widget.
	 \param[in] w potential child widget
	 \return Returns 1 if \p w is a child of this widget, or is
	 equal to this widget. Returns 0 if \p w is NULL.
	 */
	int contains(const fltk3::Widget *w) const ;

	/** Checks if this widget is a child of w.
	 Returns 1 if this widget is a child of \p w, or is
	 equal to \p w. Returns 0 if \p w is NULL.
	 \param[in] w the possible parent widget.
	 \see contains()
	 */
	int inside(const fltk3::Widget* w) const {
		return w ? w->contains(this) : 0;
	}

	/** Schedules the drawing of the widget.
	 Marks the widget as needing its draw() routine called.
	 */
	void redraw();

	/** Schedules the drawing of the label.
	 Marks the widget or the parent as needing a redraw for the label area
	 of a widget.
	 */
	virtual void redraw_label();

	/** Returns non-zero if draw() needs to be called.
	 The damage value is actually a bit field that the widget
	 subclass can use to figure out what parts to draw.
	 \return a bitmap of flags describing the kind of damage to the widget
	 \see damage(uchar), clear_damage(), set_damage(uchar)
	 */
	fltk3::Damage damage() const {
		return (fltk3::Damage)damage_;
	}

	/** Clears all damage flags.
	 Damage flags are cleared when parts of the widget drawing is repaired.

	 \see damage(uchar), damage(), set_damage(uchar)
	 */
	void clear_damage(uchar c = 0) {
		damage_ = c;
	}

	/** Sets the damage flags.

	 The argument \p c specifies the bits that are set
	 after the call. This call will not schedule the wiget for redraw!

	 \param[in] c new bitmask of damage flags (default: 0)
	 \see damage(uchar), damage()
	 */
	void set_damage(uchar c = 0) {
		damage_ = c;
	}

	/** Sets the damage bits for the widget.
	 Setting damage bits will schedule the widget for the next redraw.
	 \param[in] c bitmask of flags to set
	 \see damage(), clear_damage(uchar)
	 */
	void damage(uchar c);

	/** Sets the damage bits for an area inside the widget.
	 Setting damage bits will schedule the widget for the next redraw.
	 \param[in] c bitmask of flags to set
	 \param[in] x, y, w, h size of damaged area
	 \see damage(), clear_damage(uchar)
	 */
	void damage(uchar c, int x, int y, int w, int h);

	void draw_label(int, int, int, int, fltk3::Align) const;

	/** Sets width ww and height hh accordingly with the label size.
	 Labels with images will return w() and h() of the image.
	 */
	void measure_label(int& ww, int& hh) const {
		Label::measure(ww, hh);
	}

	/** Returns a pointer to the primary fltk3::Window widget.
	 \retval  NULL if no window is associated with this widget.
	 \note for an fltk3::Window widget, this returns its <I>parent</I> window
	 (if any), not <I>this</I> window.
	 */
	fltk3::Window* window() const;

	int dx_window();
	int dy_window();

	/** Returns an fltk3::Group pointer if this widget is an fltk3::Group.

	 Use this method if you have a widget (pointer) and need to
	 know whether this widget is derived from fltk3::Group. If it returns
	 non-NULL, then the widget in question is derived from fltk3::Group,
	 and you can use the returned pointer to access its children
	 or other fltk3::Group-specific methods.

	 Example:
	 \code
	 void my_callback (fltk3::Widget *w, void *) {
	 fltk3::Group *g = w->as_group();
	 if (g)
	 printf ("This group has %d children\n",g->children());
	 else
	 printf ("This widget is not a group!\n");
	 }
	 \endcode

	 \retval NULL if this widget is not derived from fltk3::Group.
	 \note This method is provided to avoid dynamic_cast.
	 \see fltk3::Widget::as_window(), fltk3::Widget::as_gl_window()
	 */
	virtual fltk3::Group* as_group() {
		return 0;
	}

	/** Returns an fltk3::Window pointer if this widget is an fltk3::Window.

	 Use this method if you have a widget (pointer) and need to
	 know whether this widget is derived from fltk3::Window. If it returns
	 non-NULL, then the widget in question is derived from fltk3::Window,
	 and you can use the returned pointer to access its children
	 or other fltk3::Window-specific methods.

	 \retval NULL if this widget is not derived from fltk3::Window.
	 \note This method is provided to avoid dynamic_cast.
	 \see fltk3::Widget::as_group(), fltk3::Widget::as_gl_window()
	 */
	virtual fltk3::Window* as_window() {
		return 0;
	}

	/** Returns an fltk3::GLWindow pointer if this widget is an fltk3::GLWindow.

	 Use this method if you have a widget (pointer) and need to
	 know whether this widget is derived from fltk3::GLWindow. If it returns
	 non-NULL, then the widget in question is derived from fltk3::GLWindow.

	 \retval NULL if this widget is not derived from fltk3::GLWindow.
	 \note This method is provided to avoid dynamic_cast.
	 \see fltk3::Widget::as_group(), fltk3::Widget::as_window()
	 */
	virtual fltk3::GLWindow* as_gl_window() {
		return 0;
	}

	/** Returns an fltk3::ShapedWindow pointer if this widget is an fltk3::ShapedWindow.

	 Use this method if you have a widget (pointer) and need to
	 know whether this widget is derived from fltk3::ShapedWindow. If it returns
	 non-NULL, then the widget in question is derived from fltk3::ShapedWindow.

	 \retval NULL if this widget is not derived from fltk3::ShapedWindow.
	 \note This method is provided to avoid dynamic_cast.
	 \see fltk3::Widget::as_group(), fltk3::Widget::as_window()
	 */
	virtual fltk3::ShapedWindow* as_shaped_window() {
		return 0;
	}

	/** Returns non zero if MAC_USE_ACCENTS_MENU flag is set, 0 otherwise.
	 */
	int use_accents_menu() {
		return flags() & MAC_USE_ACCENTS_MENU;
	}

	unsigned char EnableScheme() { return 1; }
};


/**
 Reserved type numbers (necessary for my cheapo RTTI) start here.
 Grep the header files for "RESERVED_TYPE" to find the next available
 number.
 */
const uchar RESERVED_TYPE = 100;
}

#endif

//
// End of "$Id: Widget.h 9794 2013-01-14 17:23:42Z manolo $".
//
