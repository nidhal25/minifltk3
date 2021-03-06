//
// "$Id: print_panel.cxx 9559 2012-05-30 10:14:51Z dejan $"
//
// Print panel for the Fast Light Tool Kit (FLTK).
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

//
// This file is "work in progress".  The main parts have been copied
// from fluid's print_panel{.fl|.h|.cxx} and hand-edited to produce
// a working version w/o global variables.  The intention is to move
// all static variables into an own class, and to name this class
// Fl_Printer_Chooser or similar...
//
// Todo:
//
//   -	Currently preferences can't be saved, and there are options that
//	are not yet used for printing.
//   -	This file can only be used as an include file in Fl_PS_Printer.cxx
//   -	The use of static variables should be avoided.
//   -	Probably much more ...
//

#include "Platform_x11_print_panel.h"
#include <stdio.h>
#include <stdlib.h>
#include "flstring.h"
#include "Preferences.h"
#include "IntInput.h"

static fltk3::Preferences print_prefs(fltk3::Preferences::USER, "fltk.org", "printers");
static fltk3::DoubleWindow *print_panel=(fltk3::DoubleWindow *)0;
static fltk3::Group *print_panel_controls=(fltk3::Group *)0;
static fltk3::Choice *print_choice=(fltk3::Choice *)0;
static fltk3::Button *print_properties=(fltk3::Button *)0;
static fltk3::Widget *print_status=(fltk3::Widget *)0;
static fltk3::RoundButton *print_all=(fltk3::RoundButton *)0;
static fltk3::RoundButton *print_pages=(fltk3::RoundButton *)0;
static fltk3::RoundButton *print_selection=(fltk3::RoundButton *)0;
static fltk3::CheckButton *print_collate_button=(fltk3::CheckButton *)0;
static fltk3::Group *print_collate_group[2]={(fltk3::Group *)0};
static fltk3::Progress *print_progress=(fltk3::Progress *)0;
static fltk3::DoubleWindow *print_properties_panel=(fltk3::DoubleWindow *)0;
static fltk3::Choice *print_page_size=(fltk3::Choice *)0;
static fltk3::IntInput *print_from=(fltk3::IntInput *)0;
static fltk3::IntInput *print_to=(fltk3::IntInput *)0;
static fltk3::Spinner *print_copies=(fltk3::Spinner *)0;

static int print_start = 0;	// 1 if print_okay has been clicked

static void cb_print_choice(fltk3::Choice*, void*) {
  print_update_status();
}

static void cb_print_properties(fltk3::Button*, void*) {
  print_properties_panel->show();
}

static void cb_print_all(fltk3::RoundButton*, void*) {
  print_from->deactivate();
  print_to->deactivate();
}

static void cb_print_pages(fltk3::RoundButton*, void*) {
  print_from->activate();
  print_to->activate();
}

static void cb_print_selection(fltk3::RoundButton*, void*) {
  print_from->deactivate();
  print_to->deactivate();
}

static void cb_print_copies(fltk3::Spinner*, void*) {
  if (print_copies->value() == 1) {
    print_collate_button->deactivate();
    print_collate_group[0]->deactivate();
    print_collate_group[1]->deactivate();
  } else {
/*    print_collate_button->activate(); // TODO: manage collate options
    print_collate_group[0]->activate();
    print_collate_group[1]->activate(); */
  };
}

static void cb_print_collate_button(fltk3::CheckButton*, void*) {
  int i = print_collate_button->value() != 0;
  print_collate_group[i]->show();
  print_collate_group[1 - i]->hide();
}

static void cb_Cancel(fltk3::Button*, void*) {
  print_start = 0;
  print_panel->hide();
}

static void cb_print_properties_panel(fltk3::DoubleWindow*, void*) {
  print_properties_panel->hide();
  print_update_status();
}

static fltk3::MenuItem menu_print_page_size[] = {
  {"Letter", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {"A4", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {"Legal", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {"Executive", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {"A3", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {"A5", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {"B5", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {"Com10", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {"DL", 0,  0, 0, 0, fltk3::NORMAL_LABEL, 0, 14, 0},
  {0,0,0,0,0,0,0,0,0}
};

#include "Pixmap.h"
#include "Printer.h"
static const char *idata_print_color[] = {
"24 24 17 1",
" \tc None",
".\tc #FFFF00",
"+\tc #C8FF00",
"@\tc #00FF00",
"#\tc #FFC800",
"$\tc #FF0000",
"%\tc #00FFFF",
"&\tc #000000",
"*\tc #FF00FF",
"=\tc #00FFC8",
"-\tc #FF00C8",
";\tc #00C800",
">\tc #C80000",
",\tc #0000C8",
"\'\tc #0000FF",
")\tc #00C8FF",
"!\tc #C800FF",
"         ......         ",
"       ..........       ",
"      ............      ",
"     ..............     ",
"     ..............     ",
"    ................    ",
"    ................    ",
"    ................    ",
"    +@@@@@@+#$$$$$$#    ",
"   %@@@@@@@&&$$$$$$$*   ",
"  %%@@@@@@&&&&$$$$$$**  ",
" %%%=@@@@&&&&&&$$$$-*** ",
" %%%%@@@;&&&&&&>$$$**** ",
"%%%%%%@@&&&&&&&&$$******",
"%%%%%%%@&&&&&&&&$*******",
"%%%%%%%%,&&&&&&,********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
" %%%%%%%)\'\'\'\'\'\'!******* ",
" %%%%%%%%\'\'\'\'\'\'******** ",
"  %%%%%%%%\'\'\'\'********  ",
"   %%%%%%%%\'\'********   ",
"     %%%%%%  ******     "
};
static fltk3::Pixmap image_print_color(idata_print_color);

static const char *idata_print_gray[] = {
"24 24 17 1",
" \tc None",
".\tc #E3E3E3",
"+\tc #D2D2D2",
"@\tc #969696",
"#\tc #C2C2C2",
"$\tc #4C4C4C",
"%\tc #B2B2B2",
"&\tc #000000",
"*\tc #696969",
"=\tc #ACACAC",
"-\tc #626262",
";\tc #767676",
">\tc #3C3C3C",
",\tc #161616",
"\'\tc #1C1C1C",
")\tc #929292",
"!\tc #585858",
"         ......         ",
"       ..........       ",
"      ............      ",
"     ..............     ",
"     ..............     ",
"    ................    ",
"    ................    ",
"    ................    ",
"    +@@@@@@+#$$$$$$#    ",
"   %@@@@@@@&&$$$$$$$*   ",
"  %%@@@@@@&&&&$$$$$$**  ",
" %%%=@@@@&&&&&&$$$$-*** ",
" %%%%@@@;&&&&&&>$$$**** ",
"%%%%%%@@&&&&&&&&$$******",
"%%%%%%%@&&&&&&&&$*******",
"%%%%%%%%,&&&&&&,********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
"%%%%%%%%\'\'\'\'\'\'\'\'********",
" %%%%%%%)\'\'\'\'\'\'!******* ",
" %%%%%%%%\'\'\'\'\'\'******** ",
"  %%%%%%%%\'\'\'\'********  ",
"   %%%%%%%%\'\'********   ",
"     %%%%%%  ******     "
};
static fltk3::Pixmap image_print_gray(idata_print_gray);

static fltk3::Button *print_output_mode[4]={(fltk3::Button *)0};

static void cb_Save(fltk3::ReturnButton*, void*) {
  print_properties_panel->hide();

  char name[1024];
  int val;
  const char *printer = (const char *)print_choice->menu()[print_choice->value()].user_data();

  snprintf(name, sizeof(name), "%s/page_size", printer == NULL ? "" : printer);
  print_prefs.set(name, print_page_size->value());

  snprintf(name, sizeof(name), "%s/output_mode", printer == NULL ? "" : printer);
  for (val = 0; val < 4; val ++) {
    if (print_output_mode[val]->value()) break;
  }
  print_prefs.set(name, val);
}

static void cb_Cancel1(fltk3::Button*, void*) {
  print_properties_panel->hide();
  print_update_status();
}

static void cb_Use(fltk3::Button*, void*) {
  print_properties_panel->hide();
}

fltk3::DoubleWindow* make_print_panel() {
  { print_panel = new fltk3::DoubleWindow(465, 235, fltk3::Printer::dialog_title);
    { print_panel_controls = new fltk3::Group(10, 10, 447, 216);
      { print_choice = new fltk3::Choice(123, 0, 181, 25, fltk3::Printer::dialog_printer);
        print_choice->down_box(fltk3::BORDER_BOX);
        print_choice->labelfont(1);
        print_choice->callback((fltk3::Callback*)cb_print_choice);
        print_choice->when(fltk3::WHEN_CHANGED);
      } // fltk3::Choice* print_choice
      { print_properties = new fltk3::Button(304, 0, 115, 25, fltk3::Printer::dialog_properties);
        print_properties->callback((fltk3::Callback*)cb_print_properties);
      } // fltk3::Button* print_properties
      { print_status = new fltk3::Widget(0, 31, print_panel_controls->w(), 17, "printer/job status");
        print_status->align(fltk3::Align(fltk3::ALIGN_CLIP|fltk3::ALIGN_INSIDE|fltk3::ALIGN_LEFT));
      } // fltk3::Widget* print_status
      { fltk3::Group* o = new fltk3::Group(0, 76, 227, 105, fltk3::Printer::dialog_range);
        o->box(fltk3::THIN_DOWN_BOX);
        o->labelfont(1);
        o->align(fltk3::Align(fltk3::ALIGN_TOP_LEFT));
        { print_all = new fltk3::RoundButton(10, 10, 38, 25, fltk3::Printer::dialog_all);
          print_all->type(102);
          print_all->down_box(fltk3::ROUND_DOWN_BOX);
          print_all->value(1);
          print_all->callback((fltk3::Callback*)cb_print_all);
        } // fltk3::RoundButton* print_all
        { print_pages = new fltk3::RoundButton(10, 40, 64, 25, fltk3::Printer::dialog_pages);
          print_pages->type(102);
          print_pages->down_box(fltk3::ROUND_DOWN_BOX);
          print_pages->callback((fltk3::Callback*)cb_print_pages);
        } // fltk3::RoundButton* print_pages
        { print_selection = new fltk3::RoundButton(10, 70, 82, 25, "Selection");
          print_selection->type(102);
          print_selection->down_box(fltk3::ROUND_DOWN_BOX);
          print_selection->callback((fltk3::Callback*)cb_print_selection);
        } // fltk3::RoundButton* print_selection
        { print_from = new fltk3::IntInput(126, 40, 28, 25, fltk3::Printer::dialog_from);
          print_from->type(2);
          print_from->textfont(4);
          print_from->deactivate();
        } // fltk3::IntInput* print_from
        { print_to = new fltk3::IntInput(189, 40, 28, 25, fltk3::Printer::dialog_to);
          print_to->type(2);
          print_to->textfont(4);
          print_to->deactivate();
        } // fltk3::IntInput* print_to
        o->end();
      } // fltk3::Group* o
      { fltk3::Group* o = new fltk3::Group(237, 76, 210, 105, fltk3::Printer::dialog_copies);
        o->box(fltk3::THIN_DOWN_BOX);
        o->labelfont(1);
        o->align(fltk3::Align(fltk3::ALIGN_TOP_LEFT));
        { print_copies = new fltk3::Spinner(74, 10, 45, 25, fltk3::Printer::dialog_copyNo);
          print_copies->callback((fltk3::Callback*)cb_print_copies);
          print_copies->when(fltk3::WHEN_CHANGED);
        } // fltk3::Spinner* print_copies
        { print_collate_button = new fltk3::CheckButton(129, 10, 64, 25, "Collate");
          print_collate_button->down_box(fltk3::DOWN_BOX);
          print_collate_button->callback((fltk3::Callback*)cb_print_collate_button);
          print_collate_button->when(fltk3::WHEN_CHANGED);
          print_collate_button->deactivate();
        } // fltk3::CheckButton* print_collate_button
        { print_collate_group[0] = new fltk3::Group(10, 45, 191, 50);
          print_collate_group[0]->deactivate();
          { fltk3::Widget* o = new fltk3::Widget(30, 10, 30, 40, "1");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(15, 5, 30, 40, "1");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(0, 0, 30, 40, "1");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(95, 10, 30, 40, "2");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(80, 5, 30, 40, "2");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(65, 0, 30, 40, "2");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(160, 10, 30, 40, "3");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(145, 5, 30, 40, "3");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(130, 0, 30, 40, "3");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
            o->deactivate();
          } // fltk3::Widget* o
          print_collate_group[0]->end();
        } // fltk3::Group* print_collate_group[0]
        { print_collate_group[1] = new fltk3::Group(10, 45, 191, 50);
          print_collate_group[1]->hide();
          print_collate_group[1]->deactivate();
          { fltk3::Widget* o = new fltk3::Widget(30, 10, 30, 40, "3");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(15, 5, 30, 40, "2");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(0, 0, 30, 40, "1");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(95, 10, 30, 40, "3");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(80, 5, 30, 40, "2");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(65, 0, 30, 40, "1");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(160, 10, 30, 40, "3");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(145, 5, 30, 40, "2");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          { fltk3::Widget* o = new fltk3::Widget(130, 0, 30, 40, "1");
            o->box(fltk3::BORDER_BOX);
            o->color(fltk3::BACKGROUND2_COLOR);
            o->labelsize(11);
            o->align(fltk3::Align(fltk3::ALIGN_BOTTOM_RIGHT|fltk3::ALIGN_INSIDE));
          } // fltk3::Widget* o
          print_collate_group[1]->end();
        } // fltk3::Group* print_collate_group[1]
        o->end();
      } // fltk3::Group* o
      { fltk3::ReturnButton* o = new fltk3::ReturnButton(269, 190, 100, 25, fltk3::Printer::dialog_print_button);
        o->callback((fltk3::Callback*)print_cb);
      } // fltk3::ReturnButton* o
      { fltk3::Button* o = new fltk3::Button(379, 190, 68, 25, fltk3::Printer::dialog_cancel_button);
        o->callback((fltk3::Callback*)cb_Cancel);
      } // fltk3::Button* o
      print_panel_controls->end();
    } // fltk3::Group* print_panel_controls
    { print_progress = new fltk3::Progress(10, 203, 289, 21);
      print_progress->selection_color((fltk3::Color)4);
      print_progress->hide();
    } // fltk3::Progress* print_progress
    print_panel->set_modal();
    print_panel->end();
  } // fltk3::DoubleWindow* print_panel
  { print_properties_panel = new fltk3::DoubleWindow(290, 130, fltk3::Printer::property_title);
    print_properties_panel->callback((fltk3::Callback*)cb_print_properties_panel);
    { print_page_size = new fltk3::Choice(150, 10, 80, 25, fltk3::Printer::property_pagesize);
      print_page_size->down_box(fltk3::BORDER_BOX);
      print_page_size->labelfont(fltk3::HELVETICA);
      print_page_size->menu(menu_print_page_size);
    } // fltk3::Choice* print_page_size
    { fltk3::Group* o = new fltk3::Group(110, 45, 170, 40, fltk3::Printer::property_mode);
      o->labelfont(fltk3::HELVETICA);
      o->align(fltk3::Align(fltk3::ALIGN_LEFT));
      { print_output_mode[0] = new fltk3::Button(0, 0, 30, 40);
        print_output_mode[0]->type(102);
        print_output_mode[0]->box(fltk3::BORDER_BOX);
        print_output_mode[0]->down_box(fltk3::BORDER_BOX);
        print_output_mode[0]->value(1);
        print_output_mode[0]->color(fltk3::BACKGROUND2_COLOR);
        print_output_mode[0]->selection_color(fltk3::FOREGROUND_COLOR);
        print_output_mode[0]->image(image_print_color);
      } // fltk3::Button* print_output_mode[0]
      { print_output_mode[1] = new fltk3::Button(40, 5, 40, 30);
        print_output_mode[1]->type(102);
        print_output_mode[1]->box(fltk3::BORDER_BOX);
        print_output_mode[1]->down_box(fltk3::BORDER_BOX);
        print_output_mode[1]->color(fltk3::BACKGROUND2_COLOR);
        print_output_mode[1]->selection_color(fltk3::FOREGROUND_COLOR);
        print_output_mode[1]->image(image_print_color);
      } // fltk3::Button* print_output_mode[1]
      { print_output_mode[2] = new fltk3::Button(90, 0, 30, 40);
        print_output_mode[2]->type(102);
        print_output_mode[2]->box(fltk3::BORDER_BOX);
        print_output_mode[2]->down_box(fltk3::BORDER_BOX);
        print_output_mode[2]->color(fltk3::BACKGROUND2_COLOR);
        print_output_mode[2]->selection_color(fltk3::FOREGROUND_COLOR);
        print_output_mode[2]->image(image_print_gray);
      } // fltk3::Button* print_output_mode[2]
      { print_output_mode[3] = new fltk3::Button(130, 5, 40, 30);
        print_output_mode[3]->type(102);
        print_output_mode[3]->box(fltk3::BORDER_BOX);
        print_output_mode[3]->down_box(fltk3::BORDER_BOX);
        print_output_mode[3]->color(fltk3::BACKGROUND2_COLOR);
        print_output_mode[3]->selection_color(fltk3::FOREGROUND_COLOR);
        print_output_mode[3]->image(image_print_gray);
      } // fltk3::Button* print_output_mode[3]
      o->end();
    } // fltk3::Group* o
    { fltk3::ReturnButton* o = new fltk3::ReturnButton(93, 95, 99, 25, fltk3::Printer::property_save);
      o->callback((fltk3::Callback*)cb_Save);
    } // fltk3::ReturnButton* o
    { fltk3::Button* o = new fltk3::Button(202, 95, 78, 25, fltk3::Printer::property_cancel);
      o->callback((fltk3::Callback*)cb_Cancel1);
    } // fltk3::Button* o
    { fltk3::Button* o = new fltk3::Button(10, 95, 73, 25, fltk3::Printer::property_use);
      o->callback((fltk3::Callback*)cb_Use);
    } // fltk3::Button* o
    print_properties_panel->set_modal();
    print_properties_panel->end();
  } // fltk3::DoubleWindow* print_properties_panel
  return print_properties_panel;
}

void print_cb(fltk3::ReturnButton *, void *) {
  print_start = 1;
  print_panel->hide();
}

void print_load() {
  FILE *lpstat;
  char line[1024], name[1024], *nptr, qname[2048], *qptr, defname[1024];
  int i;

  if (print_choice->size() > 1) {
    for (i = 1; print_choice->text(i); i ++) {
      free(print_choice->menu()[i].user_data());
    }
  }

  print_choice->clear();
  print_choice->add(fltk3::Printer::dialog_print_to_file, (unsigned)0, 0, 0, fltk3::MENU_DIVIDER);
  print_choice->value(0);
  
  print_start = 0;

  defname[0] = '\0';

  if ((lpstat = popen("LC_MESSAGES=C LANG=C lpstat -p -d", "r")) != NULL) {
    while (fgets(line, sizeof(line), lpstat)) {
      if (!strncmp(line, "printer ", 8) &&
          sscanf(line + 8, "%s", name) == 1) {
        for (nptr = name, qptr = qname; *nptr; *qptr++ = *nptr++) {
          if (*nptr == '/') *qptr++ = '\\';
        }
        *qptr = '\0';

        print_choice->add(qname, (unsigned)0, 0, (void *)strdup(name), 0);
      } else if (!strncmp(line, "system default destination: ", 28)) {
        if (sscanf(line + 28, "%s", defname) != 1) defname[0] = '\0';
      }
    }
    pclose(lpstat);
  }

  if (defname[0]) {
    for (i = 1; print_choice->text(i); i ++) {
      if (!strcmp((char *)print_choice->menu()[i].user_data(), defname)) {
        print_choice->value(i);
        break;
      }
    }
  } else if (print_choice->size() > 2) print_choice->value(1);

  print_update_status();

} // print_load()

void print_update_status() {
  FILE *lpstat;
  char command[1024];
  static char status[1024];
  const char *printer = (const char *)print_choice->menu()[print_choice->value()].user_data();

  if (print_choice->value()) {
    snprintf(command, sizeof(command), "lpstat -p '%s'", printer);
    if ((lpstat = popen(command, "r")) != NULL) {
      if (fgets(status, sizeof(status), lpstat)==0) { /* ignore */ }
      pclose(lpstat);
    } else strcpy(status, "printer status unavailable");
  } else status[0] = '\0';

  print_status->label(status);

  char name[1024];
  int val;

  snprintf(name, sizeof(name), "%s/page_size", printer == NULL ? "" : printer);
  print_prefs.get(name, val, 1);
  print_page_size->value(val);

  snprintf(name, sizeof(name), "%s/output_mode", printer == NULL ? "" : printer);
  print_prefs.get(name, val, 0);
  print_output_mode[val]->setonly();
}

//
// End of "$Id: print_panel.cxx 9559 2012-05-30 10:14:51Z dejan $".
//
