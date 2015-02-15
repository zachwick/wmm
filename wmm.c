/**
 * (W)indow (M)anager (M)inus
 *    Based off of TinyWM and TinyWM-XCB
 *
 * Copyright 2013 ZachWick <zach@zachwick.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

	// Connect the X11 server and check for connection errors
	display = xcb_connect(NULL,NULL);
	if (xcb_connection_has_error(display)) return 1;

	// Set screen and root to be what their names suggest
	screen = xcb_setup_roots_iterator(xcb_get_setup(display)).data;
	root = screen->root;

	xcb_grab_key(display, 1, root, XCB_MOD_MASK_CONTROL, XCB_NO_SYMBOL,
	             XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
  
	xcb_grab_button(display, 0, root, XCB_EVENT_MASK_BUTTON_PRESS |
	                XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
	                XCB_GRAB_MODE_ASYNC, root, XCB_NONE, 3, XCB_MOD_MASK_CONTROL);
  
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
		case XCB_BUTTON_RELEASE:
			{
				xcb_ungrab_pointer(display, XCB_CURRENT_TIME);
				xcb_flush(display);
				break;
			}
		}
	}
	return 0;
}
