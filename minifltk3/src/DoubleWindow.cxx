//
// "$Id: DoubleWindow.cxx 10155 2014-05-21 14:12:04Z manolo $"
//
// Double-buffered window code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2012 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
/** \file
 DoubleWindow implementation.
 */

#include <config.h>
#include "run.h"
#include "DoubleWindow.h"
#include "Printer.h"
#include "x.h"
#include "draw.h"

// On systems that support double buffering "naturally" the base
// fltk3::Window class will probably do double-buffer and this subclass
// does nothing.

#if USE_XDBE

#include <X11/extensions/Xdbe.h>

static int use_xdbe;

static int can_xdbe()
{
	static int tried;
	if (!tried) {
		tried = 1;
		int event_base, error_base;
		if (!XdbeQueryExtension(fl_display, &event_base, &error_base)) return 0;
		Drawable root = RootWindow(fl_display,fl_screen);
		int numscreens = 1;
		XdbeScreenVisualInfo *a = XdbeGetVisualInfo(fl_display,&root,&numscreens);
		if (!a) return 0;
		for (int j = 0; j < a->count; j++) {
			if (a->visinfo[j].visual == fl_visual->visualid
			    /*&& a->visinfo[j].perflevel > 0*/) {
				use_xdbe = 1;
				break;
			}
		}
		XdbeFreeVisualInfo(a);
	}
	return use_xdbe;
}
#endif


fltk3::DoubleWindow::DoubleWindow(int W, int H, const char *l) : fltk3::Window(W,H,l), force_doublebuffering_(0)
{
	type(fltk3::DOUBLE_WINDOW);
}


fltk3::DoubleWindow::DoubleWindow(int X, int Y, int W, int H, const char *l) : fltk3::Window(X,Y,W,H,l), force_doublebuffering_(0)
{
	type(fltk3::DOUBLE_WINDOW);
}


void fltk3::DoubleWindow::show()
{
	//fltk3::
		Window::show();
}


/** \addtogroup fl_drawings
 @{
 */
/** Copy a rectangular area of the given offscreen buffer into the current drawing destination.
 \param x,y	position where to draw the copied rectangle
 \param w,h	size of the copied rectangle
 \param pixmap  offscreen buffer containing the rectangle to copy
 \param srcx,srcy origin in offscreen buffer of rectangle to copy
 */
#if FLTK_ABI_VERSION >= 10301
void fl_copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy)
{
	fltk3::graphics_driver->copy_offscreen(x, y, w, h, pixmap, srcx, srcy);
}
#else
void fl_copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy)
{
	/*
#ifdef WIN32
	if (fltk3::graphics_driver->class_name() == fltk3::GDIGraphicsDriver::class_id || fltk3::graphics_driver->class_name() == fltk3::GDIPrinterGraphicsDriver::class_id) {
#else
	if (fltk3::graphics_driver->class_name() == fltk3::DisplayDevice::display_device()->driver()->class_name()) {
#endif
#ifdef USE_X11
		((fltk3::XlibGraphicsDriver*)fltk3::graphics_driver)->copy_offscreen(x, y, w, h, pixmap, srcx, srcy);
#elif defined(WIN32)
		((fltk3::GDIGraphicsDriver*)fltk3::graphics_driver)->copy_offscreen(x, y, w, h, pixmap, srcx, srcy);
#elif defined(__APPLE__)
		((fltk3::QuartzGraphicsDriver*)fltk3::graphics_driver)->copy_offscreen(x, y, w, h, pixmap, srcx, srcy);
#endif
	} else { // when copy is not to the display
	*/
		fltk3::graphics_driver->copy_offscreen(x, y, w, h, pixmap, srcx, srcy);
	//}
}
#endif // FLTK_ABI_VERSION
/** @} */

/** see fl_copy_offscreen() */
void fltk3::GraphicsDriver::copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy)
{
	fl_begin_offscreen(pixmap);
	uchar *img = fltk3::read_image(NULL, srcx, srcy, w, h, 0);
	fl_end_offscreen();
	fltk3::draw_image(img, x, y, w, h, 3, 0);
	delete[] img;
}

#if defined(USE_X11)

void fltk3::XlibGraphicsDriver::copy_offscreen(int x, int y, int w, int h, fltk3::Offscreen pixmap, int srcx, int srcy)
{
	XCopyArea(fl_display, pixmap, fl_window, fl_gc, srcx, srcy, w, h, x, y);
}


// maybe someone feels inclined to implement alpha blending on X11?
char fltk3::XlibGraphicsDriver::can_do_alpha_blending()
{
	return 0;
}
#elif defined(WIN32)

// Code used to switch output to an off-screen window.  See macros in
// win32.H which save the old state in local variables.

typedef struct {
	BYTE a;
	BYTE b;
	BYTE c;
	BYTE d;
} FL_BLENDFUNCTION;
typedef BOOL (WINAPI* fl_alpha_blend_func)
(HDC,int,int,int,int,HDC,int,int,int,int,FL_BLENDFUNCTION);
static fl_alpha_blend_func fl_alpha_blend = NULL;
static FL_BLENDFUNCTION blendfunc = { 0, 0, 255, 1};

/*
 * This function checks if the version of MSWindows that we
 * curently run on supports alpha blending for bitmap transfers
 * and finds the required function if so.
 */
char fltk3::GDIGraphicsDriver::can_do_alpha_blending()
{
	static char been_here = 0;
	static char can_do = 0;
	// do this test only once
	if (been_here) return can_do;
	been_here = 1;
	// load the library that implements alpha blending
	HMODULE hMod = LoadLibrary("MSIMG32.DLL");
	// give up if that doesn't exist (Win95?)
	if (!hMod) return 0;
	// now find the blending function inside that dll
	fl_alpha_blend = (fl_alpha_blend_func)GetProcAddress(hMod, "AlphaBlend");
	// give up if we can't find it (Win95)
	if (!fl_alpha_blend) return 0;
	// we have the call, but does our display support alpha blending?
	// get the desktop's device context
	HDC dc = GetDC(0L);
	if (!dc) return 0;
	// check the device capabilities flags. However GetDeviceCaps
	// does not return anything useful, so we have to do it manually:

	HBITMAP bm = CreateCompatibleBitmap(dc, 1, 1);
	HDC new_gc = CreateCompatibleDC(dc);
	int save = SaveDC(new_gc);
	SelectObject(new_gc, bm);
	/*COLORREF set = */
	SetPixel(new_gc, 0, 0, 0x01010101);
	BOOL alpha_ok = fl_alpha_blend(dc, 0, 0, 1, 1, new_gc, 0, 0, 1, 1, blendfunc);
	RestoreDC(new_gc, save);
	DeleteDC(new_gc);
	DeleteObject(bm);
	ReleaseDC(0L, dc);

	if (alpha_ok) can_do = 1;
	return can_do;
}

HDC fl_makeDC(HBITMAP bitmap)
{
	HDC new_gc = CreateCompatibleDC(fl_gc);
	SetTextAlign(new_gc, TA_BASELINE|TA_LEFT);
	SetBkMode(new_gc, TRANSPARENT);
#if USE_COLORMAP
	if (fl_palette) SelectPalette(new_gc, fl_palette, FALSE);
#endif
	SelectObject(new_gc, bitmap);
	return new_gc;
}

void fltk3::GDIGraphicsDriver::copy_offscreen(int x,int y,int w,int h,HBITMAP bitmap,int srcx,int srcy)
{
	HDC new_gc = CreateCompatibleDC(fl_gc);
	int save = SaveDC(new_gc);
	SelectObject(new_gc, bitmap);
	BitBlt(fl_gc, x, y, w, h, new_gc, srcx, srcy, SRCCOPY);
	RestoreDC(new_gc, save);
	DeleteDC(new_gc);
}

void fltk3::GDIGraphicsDriver::copy_offscreen_with_alpha(int x,int y,int w,int h,HBITMAP bitmap,int srcx,int srcy)
{
	HDC new_gc = CreateCompatibleDC(fl_gc);
	int save = SaveDC(new_gc);
	SelectObject(new_gc, bitmap);
	can_do_alpha_blending(); // make sure this is called
	BOOL alpha_ok = 0;
	// first try to alpha blend
	// if to printer, always try alpha_blend
	// if ( (fltk3::graphics_driver->class_name() == fltk3::GDIPrinterGraphicsDriver::class_id && fl_alpha_blend) || (fltk3::SurfaceDevice::surface() == fltk3::DisplayDevice::display_device() && fl_can_do_alpha_blending() ) ) {
	if ( fl_alpha_blend || (fltk3::SurfaceDevice::surface() == fltk3::DisplayDevice::display_device() && can_do_alpha_blending() ) ) {
		alpha_ok = fl_alpha_blend(fl_gc, x, y, w, h, new_gc, srcx, srcy, w, h, blendfunc);
	}
	// if that failed (it shouldn't), still copy the bitmap over, but now alpha is 1
	if (!alpha_ok) {
		BitBlt(fl_gc, x, y, w, h, new_gc, srcx, srcy, SRCCOPY);
	}
	RestoreDC(new_gc, save);
	DeleteDC(new_gc);
}

extern void fltk3::restore_clip();

#elif defined(__APPLE_QUARTZ__) || defined(FLTK3_DOXYGEN)

char fltk3::QuartzGraphicsDriver::can_do_alpha_blending()
{
	return 1;
}

#if ! defined(FLTK3_DOXYGEN)
fltk3::Offscreen fltk3::QuartzGraphicsDriver::create_offscreen_with_alpha(int w, int h)
{
	void *data = calloc(w*h,4);
	CGColorSpaceRef lut = CGColorSpaceCreateDeviceRGB();
	CGContextRef ctx = CGBitmapContextCreate(
	                           data, w, h, 8, w*4, lut, kCGImageAlphaPremultipliedLast);
	CGColorSpaceRelease(lut);
	return (fltk3::Offscreen)ctx;
}
#endif

/** \addtogroup fl_drawings
 @{
 */

/**
  Creation of an offscreen graphics buffer.
 \param w,h     width and height in pixels of the buffer.
 \return    the created graphics buffer.
 */
fltk3::Offscreen fl_create_offscreen(int w, int h)
{
	void *data = calloc(w*h,4);
	CGColorSpaceRef lut = CGColorSpaceCreateDeviceRGB();
	CGContextRef ctx = CGBitmapContextCreate(
	                           data, w, h, 8, w*4, lut, kCGImageAlphaNoneSkipLast);
	CGColorSpaceRelease(lut);
	return (fltk3::Offscreen)ctx;
}

static void bmProviderRelease (void *src, const void *data, size_t size)
{
	CFIndex count = CFGetRetainCount(src);
	CFRelease(src);
	if(count == 1) free((void*)data);
}

void fltk3::QuartzGraphicsDriver::copy_offscreen(int x,int y,int w,int h,fltk3::Offscreen osrc,int srcx,int srcy)
{
	CGContextRef src = (CGContextRef)osrc;
	void *data = CGBitmapContextGetData(src);
	int sw = CGBitmapContextGetWidth(src);
	int sh = CGBitmapContextGetHeight(src);
	CGImageAlphaInfo alpha = CGBitmapContextGetAlphaInfo(src);
	CGColorSpaceRef lut = CGColorSpaceCreateDeviceRGB();
	// when output goes to a Quartz printercontext, release of the bitmap must be
	// delayed after the end of the print page
	CFRetain(src);
	CGDataProviderRef src_bytes = CGDataProviderCreateWithData( src, data, sw*sh*4, bmProviderRelease);
	CGImageRef img = CGImageCreate( sw, sh, 8, 4*8, 4*sw, lut, alpha,
	                                src_bytes, 0L, false, kCGRenderingIntentDefault);
	// fl_push_clip();
	CGRect rect = { { (CGFloat)x, (CGFloat)y }, { (CGFloat)w, (CGFloat)h } };
	Fl_X::q_begin_image(rect, srcx, srcy, sw, sh);
	CGContextDrawImage(fl_gc, rect, img);
	Fl_X::q_end_image();
	CGImageRelease(img);
	CGColorSpaceRelease(lut);
	CGDataProviderRelease(src_bytes);
}

/**  Deletion of an offscreen graphics buffer.
 \param ctx     the buffer to be deleted.
 */
void fl_delete_offscreen(fltk3::Offscreen ctx)
{
	if (!ctx) return;
	void *data = CGBitmapContextGetData((CGContextRef)ctx);
	CFIndex count = CFGetRetainCount(ctx);
	CGContextRelease((CGContextRef)ctx);
	if(count == 1) free(data);
}

const int stack_max = 16;
static int stack_ix = 0;
static CGContextRef stack_gc[stack_max];
static Window stack_window[stack_max];
static fltk3::SurfaceDevice *_ss;

/**  Send all subsequent drawing commands to this offscreen buffer.
 \param ctx     the offscreen buffer.
 */
void fl_begin_offscreen(fltk3::Offscreen ctx)
{
	_ss = fltk3::SurfaceDevice::surface();
	fltk3::DisplayDevice::display_device()->set_current();
	if (stack_ix<stack_max) {
		stack_gc[stack_ix] = fl_gc;
		stack_window[stack_ix] = fl_window;
	} else
		fprintf(stderr, "FLTK CGContext Stack overflow error\n");
	stack_ix++;

	fl_gc = (CGContextRef)ctx;
	fl_window = 0;
	CGContextSaveGState(fl_gc);
	fltk3::push_no_clip();
}

/** Quit sending drawing commands to the current offscreen buffer.
 */
void fl_end_offscreen()
{
	Fl_X::q_release_context();
	fltk3::pop_clip();
	if (stack_ix>0)
		stack_ix--;
	else
		fprintf(stderr, "FLTK CGContext Stack underflow error\n");
	if (stack_ix<stack_max) {
		fl_gc = stack_gc[stack_ix];
		fl_window = stack_window[stack_ix];
	}
	_ss->set_current();
}

/** @} */

namespace fltk3
{
	extern void restore_clip();
}

#else
# error unsupported platform
#endif

/**
  Forces the window to be redrawn.
*/
void fltk3::DoubleWindow::flush()
{
	flush(0);
}

/**
  Forces the window to be redrawn.
  \param[in] eraseoverlay non-zero to erase overlay, zero to ignore

  Fl_Overlay_Window relies on flush(1) copying the back buffer to the
  front everywhere, even if damage() == 0, thus erasing the overlay,
  and leaving the clip region set to the entire window.
*/
void fltk3::DoubleWindow::flush(int eraseoverlay)
{
	if (!shown()) return;
	make_current(); // make sure fl_gc is non-zero
	Fl_X *myi = Fl_X::i(this);
	if (!myi) return; // window not yet created
	if (!myi->other_xid) {
#if USE_XDBE
		if (can_xdbe()) {
			myi->other_xid = XdbeAllocateBackBufferName(fl_display, fl_xid(this), XdbeCopied);
			myi->backbuffer_bad = 1;
		} else
#endif
#if defined(USE_X11) || defined(WIN32)
			myi->other_xid = fl_create_offscreen(w(), h());
		clear_damage(fltk3::DAMAGE_ALL);
#elif defined(__APPLE_QUARTZ__)
			if (force_doublebuffering_) {
				myi->other_xid = fl_create_offscreen(w(), h());
				clear_damage(fltk3::DAMAGE_ALL);
			}
#else
# error unsupported platform
#endif
	}
#if USE_XDBE
	if (use_xdbe) {
		if (myi->backbuffer_bad || eraseoverlay) {
			// Make sure we do a complete redraw...
			if (myi->region) {
				XDestroyRegion(myi->region);
				myi->region = 0;
			}
			clear_damage(fltk3::DAMAGE_ALL);
			myi->backbuffer_bad = 0;
		}

		// Redraw as needed...
		if (damage()) {
			fltk3::clip_region(myi->region);
			myi->region = 0;
			fl_window = myi->other_xid;
			draw();
			fl_window = myi->xid;
		}

		// Copy contents of back buffer to window...
		XdbeSwapInfo s;
		s.swap_window = fl_xid(this);
		s.swap_action = XdbeCopied;
		XdbeSwapBuffers(fl_display, &s, 1);
		return;
	} else
#endif
		if (damage() & ~fltk3::DAMAGE_EXPOSE) {
			fltk3::clip_region(myi->region);
			myi->region = 0;
#ifdef WIN32
			HDC _sgc = fl_gc;
			fl_gc = fl_makeDC(myi->other_xid);
			int save = SaveDC(fl_gc);
			fltk3::restore_clip(); // duplicate region into new gc
			draw();
			RestoreDC(fl_gc, save);
			DeleteDC(fl_gc);
			fl_gc = _sgc;
			//# if defined(FLTK_USE_CAIRO)
			//if Fl::cairo_autolink_context() Fl::cairo_make_current(this); // capture gc changes automatically to update the cairo context adequately
			//# endif
#elif defined(__APPLE__)
			if ( myi->other_xid ) {
				fl_begin_offscreen( myi->other_xid );
				fltk3::clip_region( 0 );
				draw();
				fl_end_offscreen();
			} else {
				draw();
			}
#else // X:
			fl_window = myi->other_xid;
			draw();
			fl_window = myi->xid;
#endif
		}
		if (eraseoverlay) fltk3::clip_region(0);
	// on Irix (at least) it is faster to reduce the area copied to
	// the current clip region:
	int X,Y,W,H;
	fltk3::clip_box(0,0,w(),h(),X,Y,W,H);
	if (myi->other_xid) fl_copy_offscreen(X, Y, W, H, myi->other_xid, X, Y);
}

void fltk3::DoubleWindow::resize(int X,int Y,int W,int H)
{
	int ow = w();
	int oh = h();
	//fltk3::
		Window::resize(X,Y,W,H);
#if USE_XDBE
	if (use_xdbe) {
		Fl_X* myi = Fl_X::i(this);
		if (myi && myi->other_xid && (ow < w() || oh < h())) {
			// STR #2152: Deallocate the back buffer to force creation of a new one.
			XdbeDeallocateBackBufferName(fl_display,myi->other_xid);
			myi->other_xid = 0;
		}
		return;
	}
#endif
	Fl_X* myi = Fl_X::i(this);
	if (myi && myi->other_xid && (ow != w() || oh != h())) {
		fl_delete_offscreen(myi->other_xid);
		myi->other_xid = 0;
	}
}

void fltk3::DoubleWindow::hide()
{
	Fl_X* myi = Fl_X::i(this);
	if (myi && myi->other_xid) {
#if USE_XDBE
		if (!use_xdbe)
#endif
			fl_delete_offscreen(myi->other_xid);
	}
	//fltk3::
		Window::hide();
}

/**
  The destructor <I>also deletes all the children</I>. This allows a
  whole tree to be deleted at once, without having to keep a pointer to
  all the children in the user code.
*/
fltk3::DoubleWindow::~DoubleWindow()
{
	hide();
}


//
// End of "$Id: Fl_DoubleWindow.cxx 10155 2014-05-21 14:12:04Z manolo $".
//
