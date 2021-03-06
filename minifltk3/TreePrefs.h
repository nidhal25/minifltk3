//
// "$Id: TreePrefs.h 9560 2012-05-30 21:04:33Z matt $"
//

#ifndef FLtk3_TREE_PREFS_H
#define FLtk3_TREE_PREFS_H

namespace fltk3
{

//////////////////////
// fltk3/TreePrefs.h
//////////////////////
//
// fltk3::Tree -- This file is part of the fltk3::Tree widget for FLTK
// Copyright (C) 2009-2010 by Greg Ercolano.
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

///
/// \file
/// \brief This file contains the definitions for fltk3::Tree's preferences.
///
/// \code
///                  fltk3::TreePrefs
///                       :
///                  .....:.......
///                  :           :
///               fltk3::Tree        :
///                  |_____ fltk3::TreeItem
///
/// \endcode
///

/// \class fltk3::TreePrefs
/// \brief Tree widget's preferences.

/// \enum fltk3::TreeSort
/// Sort order options for items added to the tree
///
enum TreeSort {
	TREE_SORT_NONE=0,	///< No sorting; items are added in the order defined (default).
	TREE_SORT_ASCENDING=1,	///< Add items in ascending sort order.
	TREE_SORT_DESCENDING=2	///< Add items in descending sort order.
};

/// \enum fltk3::TreeConnector
/// Defines the style of connection lines between items.
///
enum TreeConnector {
	TREE_CONNECTOR_NONE=0,	///< Use no lines connecting items
	TREE_CONNECTOR_DOTTED=1,	///< Use dotted lines connecting items (default)
	TREE_CONNECTOR_SOLID=2	///< Use solid lines connecting items
};

/// \enum fltk3::TreeSelect
/// Tree selection style.
///
enum TreeSelect {
	TREE_SELECT_NONE=0,	///< Nothing selected when items are clicked
	TREE_SELECT_SINGLE=1,	///< Single item selected when item is clicked (default)
	TREE_SELECT_MULTI=2	///< Multiple items can be selected by clicking with
	///< SHIFT or CTRL or mouse drags.
};

/// \class fltk3::TreePrefs
///
/// \brief fltk3::Tree's Preferences class.
///
/// This class manages the fltk3::Tree's defaults.
/// You should probably be using the methods in fltk3::Tree
/// instead of trying to accessing tree's preferences settings directly.
///
class FLTK3_EXPORT TreePrefs
{
	fltk3::Font _labelfont;			// label's font face
	fltk3::Fontsize _labelsize;		// label's font size
	int _margintop;			// --
	int _marginleft;			//   |- tree's margins
	//int _marginright;			//   |
	//int _marginbottom;			// --
	int _openchild_marginbottom;		// extra space below an open child tree
	int _usericonmarginleft;		// space to left of user icon (if any)
	int _labelmarginleft;			// space to left of label
	int _connectorwidth;			// connector width (right of open/close icon)
	int _linespacing;			// vertical space between lines
	// Colors
	fltk3::Color _labelfgcolor;		// label's foreground color
	fltk3::Color _labelbgcolor;		// background color
	fltk3::Color _connectorcolor;		// connector dotted line color
	fltk3::TreeConnector _connectorstyle;	// connector line style
	fltk3::Image *_openimage;			// the 'open' icon [+]
	fltk3::Image *_closeimage;		// the 'close' icon [-]
	fltk3::Image *_userimage;			// user's own icon
	char _showcollapse;			// 1=show collapse icons, 0=don't
	char _showroot;			// show the root item as part of the tree
	fltk3::TreeSort   _sortorder;		// none, ascening, descending, etc.
	fltk3::Box*     _selectbox;		// selection box type
	fltk3::TreeSelect _selectmode;		// selection mode
public:
	TreePrefs();

	////////////////////////////
	// Labels
	////////////////////////////
	/// Return the label's font.
	inline fltk3::Font labelfont() const {
		return(_labelfont);
	}
	/// Set the label's font to \p val.
	inline void labelfont(fltk3::Font val) {
		_labelfont = val;
	}
	/// Return the label's size in pixels.
	inline fltk3::Fontsize labelsize() const {
		return(_labelsize);
	}
	/// Set the label's size in pixels to \p val.
	inline void labelsize(fltk3::Fontsize val) {
		_labelsize = val;
	}

	////////////////////////////
	// Margins
	////////////////////////////
	/// Get the left margin's value in pixels
	inline int marginleft() const {
		return(_marginleft);
	}
	/// Set the left margin's value in pixels
	inline void marginleft(int val) {
		_marginleft = val;
	}
	/// Get the top margin's value in pixels
	inline int margintop() const {
		return(_margintop);
	}
	/// Set the top margin's value in pixels
	inline void margintop(int val) {
		_margintop = val;
	}
	/// Get the margin below an open child in pixels
	inline int openchild_marginbottom() const {
		return(_openchild_marginbottom);
	}
	/// Set the margin below an open child in pixels
	inline void openchild_marginbottom(int val) {
		_openchild_marginbottom = val;
	}

	/****** NOT IMPLEMENTED
	 inline int marginright() const {
	 return(_marginright);
	 }
	 inline void marginright(int val) {
	 _marginright = val;
	 }
	 inline int marginbottom() const {
	 return(_marginbottom);
	 }
	 inline void marginbottom(int val) {
	 _marginbottom = val;
	 }
	 *******/

	/// Get the user icon's left margin value in pixels
	inline int usericonmarginleft() const {
		return(_usericonmarginleft);
	}
	/// Set the user icon's left margin value in pixels
	inline void usericonmarginleft(int val) {
		_usericonmarginleft = val;
	}
	/// Get the label's left margin value in pixels
	inline int labelmarginleft() const {
		return(_labelmarginleft);
	}
	/// Set the label's left margin value in pixels
	inline void labelmarginleft(int val) {
		_labelmarginleft = val;
	}
	/// Get the line spacing value in pixels
	inline int linespacing() const {
		return(_linespacing);
	}
	/// Set the line spacing value in pixels
	inline void linespacing(int val) {
		_linespacing = val;
	}

	////////////////////////////
	// Colors and Styles
	////////////////////////////
	/// Get the default label foreground color
	inline fltk3::Color labelfgcolor() const {
		return(_labelfgcolor);
	}
	/// Set the default label foreground color
	inline void labelfgcolor(fltk3::Color val) {
		_labelfgcolor = val;
	}
	/// Get the default label background color
	inline fltk3::Color labelbgcolor() const {
		return(_labelbgcolor);
	}
	/// Set the default label background color
	inline void labelbgcolor(fltk3::Color val) {
		_labelbgcolor = val;
	}
	/// Get the connector color used for tree connection lines.
	inline fltk3::Color connectorcolor() const {
		return(_connectorcolor);
	}
	/// Set the connector color used for tree connection lines.
	inline void connectorcolor(fltk3::Color val) {
		_connectorcolor = val;
	}
	/// Get the connector style.
	inline fltk3::TreeConnector connectorstyle() const {
		return(_connectorstyle);
	}
	/// Set the connector style.
	inline void connectorstyle(fltk3::TreeConnector val) {
		_connectorstyle = val;
	}
	/// Set the connector style [integer].
	inline void connectorstyle(int val) {
		_connectorstyle = fltk3::TreeConnector(val);
	}
	/// Get the tree connection line's width.
	inline int connectorwidth() const {
		return(_connectorwidth);
	}
	/// Set the tree connection line's width.
	inline void connectorwidth(int val) {
		_connectorwidth = val;
	}

	////////////////////////////
	// Icons
	////////////////////////////
	/// Get the current default 'open' icon.
	///     Returns the fltk3::Image* of the icon, or 0 if none.
	///
	inline fltk3::Image *openicon() const {
		return(_openimage);
	}
	void openicon(fltk3::Image *val);
	/// Gets the default 'close' icon
	///     Returns the fltk3::Image* of the icon, or 0 if none.
	///
	inline fltk3::Image *closeicon() const {
		return(_closeimage);
	}
	void closeicon(fltk3::Image *val);
	/// Gets the default 'user icon' (default is 0)
	inline fltk3::Image *usericon() const {
		return(_userimage);
	}
	/// Sets the default 'user icon'
	///     Returns the fltk3::Image* of the icon, or 0 if none (default).
	///
	inline void usericon(fltk3::Image *val) {
		_userimage = val;
	}

	////////////////////////////
	// Options
	////////////////////////////
	/// Returns 1 if the collapse icon is enabled, 0 if not.
	inline char showcollapse() const {
		return(_showcollapse);
	}
	/// Set if we should show the collapse icon or not.
	/// If collapse icons are disabled, the user will not be able
	/// to interactively collapse items in the tree, unless the application
	/// provides some other means via open() and close().
	///
	/// \param[in] val 1: shows collapse icons (default),\n
	///                0: hides collapse icons.
	///
	inline void showcollapse(int val) {
		_showcollapse = val;
	}
	/// Get the default sort order value
	inline fltk3::TreeSort sortorder() const {
		return(_sortorder);
	}
	/// Set the default sort order value.
	///     Defines the order new items appear when add()ed to the tree.
	///     See fltk3::TreeSort for possible values.
	///
	inline void sortorder(fltk3::TreeSort val) {
		_sortorder = val;
	}
	/// Get the default selection box's box drawing style as an fltk3::Boxtype.
	inline fltk3::Box* selectbox() const {
		return(_selectbox);
	}
	/// Set the default selection box's box drawing style to \p val.
	inline void selectbox(fltk3::Box* val) {
		_selectbox = val;
	}
	/// Returns 1 if the root item is to be shown, or 0 if not.
	inline int showroot() const {
		return(int(_showroot));
	}
	/// Set if the root item should be shown or not.
	/// \param[in] val 1 -- show the root item (default)\n
	///                0 -- hide the root item.
	///
	inline void showroot(int val) {
		_showroot = char(val);
	}
	/// Get the selection mode used for the tree
	inline fltk3::TreeSelect selectmode() const {
		return(_selectmode);
	}
	/// Set the selection mode used for the tree to \p val.
	///     This affects how items in the tree are selected
	///     when clicked on and dragged over by the mouse.
	///     See fltk3::TreeSelect for possible values.
	///
	inline void selectmode(fltk3::TreeSelect val) {
		_selectmode = val;
	}
};

}

#endif /*fltk3::TREE_PREFS_H*/

//
// End of "$Id: TreePrefs.h 9560 2012-05-30 21:04:33Z matt $".
//
