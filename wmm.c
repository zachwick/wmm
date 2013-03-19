/**
 * (W)indow (M)anager (M)inus
 * Copyright 2013 ZachWick <zach@zachwick.com>
 * Based off of TinyWM and TinyWM-XCB
 *
**/

#include <xcb/xcb.h>

int
main(int argc, char **argv) {
  uint32_t values[5];
  
  xcb_connection_t *display;
  xcb_screen_t *screen;
  xcb_drawable_t window;
  xcb_drawable_t root;

  xcb_generic_event_t *event;
  xcb_get_geometry_reply_t *geometry;

  // Connect the X11 server and check for connection errors
  display = xcb_connect(NULL,NULL);
  if (xcb_connection_has_error(display)) return 1;

  // Set screen and root to be what their names suggest
  screen = xcb_setup_roots_iterator(xcb_get_setup(display)).data;
  root = screen->root;

  xcb_grab_key(display, 1, root, XCB_MOD_MASK_2, XCB_NO_SYMBOL,
	       XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
  
  xcb_grab_button(display, 0, root, XCB_EVENT_MASK_BUTTON_PRESS |
		  XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
		  XCB_GRAB_MODE_ASYNC, root, XCB_NONE, 1, XCB_MOD_MASK_1);
  
  xcb_grab_button(display, 0, root, XCB_EVENT_MASK_BUTTON_PRESS |
		  XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
		  XCB_GRAB_MODE_ASYNC, root, XCB_NONE, 3, XCB_MOD_MASK_1);
  
  // Flush as queued events.
  xcb_flush(display);

  for (;;) {
    // wait for events from X server
    event = xcb_wait_for_event(display);

    switch (event->response_type & ~0x80) {
    case XCB_BUTTON_PRESS:
      {
	xcb_button_press_event_t *e;
	e = (xcb_button_press_event_t *) event;
	window = e->child;
	if (3 == e->detail) { 
	  values[0] = XCB_STACK_MODE_BELOW;
	  xcb_configure_window(display, window, XCB_CONFIG_WINDOW_STACK_MODE, values);
	  
	  values[0] = screen->width_in_pixels;
	  values[1] = screen->height_in_pixels;
	  xcb_configure_window(display, window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);
	}
	xcb_flush(display);
	break;
      }
    }
  }
  return 0;
}
