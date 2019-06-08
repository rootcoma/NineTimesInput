#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_event.h>
#include <xcb/xtest.h>
#include <string.h>
#include "gui.h"
#include "inputhandler.h"

#define WIN_X 0
#define WIN_Y 0
#define WIN_W 200
#define WIN_H WIN_W

int is_focused = 0;
xcb_connection_t *conn = NULL;
xcb_screen_t *screen = NULL;
xcb_rectangle_t screen_rect = {WIN_X, WIN_Y, WIN_W, WIN_H,};
xcb_drawable_t window;
xcb_gcontext_t foreground;
xcb_gcontext_t background;


int gui_lock_mouse()
{
    // xcb_generic_error_t *err = NULL;
    // xcb_grab_pointer_reply_t *grab_reply = NULL;
    // grab_reply = xcb_grab_pointer_reply(conn,
    //         xcb_grab_pointer(conn, 1, window, 0, XCB_GRAB_MODE_ASYNC,
    //         XCB_GRAB_MODE_ASYNC, window, XCB_NONE, XCB_CURRENT_TIME), &err);
    // if (err != NULL) {
    //     fprintf(stderr, "[-] Error calling grab pointer\n");
    //     free(err);
    //     return 0;
    // }

    // if (grab_reply->status != XCB_GRAB_STATUS_SUCCESS) {
    //     fprintf(stderr, "[-] Error locking mouse input\n");
    // }

    // free(grab_reply);
    return 1;
}


void gui_hide_window()
{
    //xcb_unmap_window(conn, window);
    //xcb_flush(conn);
}


void gui_show_window()
{
    //xcb_map_window(conn, window);
    //xcb_flush(conn);
}


void gui_create_window()
{
    // foreground color gc. not used
    foreground = xcb_generate_id(conn);
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t values[2] = {screen->black_pixel, 0,};
    xcb_create_gc(conn, foreground, screen->root, mask, values);
    // background color gc. set to black
    background = xcb_generate_id(conn);
    mask = XCB_GC_BACKGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    values[0] = screen->white_pixel;
    xcb_create_gc(conn, foreground, screen->root, mask, values);
    // create window to trap mouse and keyboard input
    window = xcb_generate_id(conn);
    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->white_pixel;
    values[1] =  XCB_EVENT_MASK_BUTTON_PRESS |
            XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
            XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
            XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_EXPOSURE;
    xcb_create_window(conn, XCB_COPY_FROM_PARENT,
            window, screen->root,
            WIN_X, WIN_Y,
            WIN_W, WIN_H,
            0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
            mask, values);
    
    // To make the window float with my wm, ymmv
    xcb_change_property(conn, XCB_PROP_MODE_REPLACE, window,
        XCB_ATOM_WM_TRANSIENT_FOR, XCB_ATOM_WINDOW, 32, 1, &screen->root);
    
    // TODO: hide and show window, for now always show window
    xcb_map_window(conn, window);
    xcb_flush(conn);

}


void mouse_scroll(const int up)
{
	mouse_click(up ? 5 : 4, 1);
	mouse_click(up ? 5 : 4, 0);
}


void mouse_click(const int button, const int down)
{
    if (button > 8) {
        fprintf(stderr, "[>] TODO: Implement ms btn > 8. (b:%d,%d)\n",
                button, down);
        return;
    }
    if (down > 1) {
        fprintf(stderr, "[!] Skipping click. (b:%d,%d)\n",
                button, down);
        return; // skip anything crazy for now
    }
    
    xcb_test_fake_input(conn,
            down ? XCB_BUTTON_PRESS : XCB_BUTTON_RELEASE,
            button, XCB_CURRENT_TIME, XCB_NONE, 0, 0, 0);
    xcb_flush(conn);

    //fprintf(stderr, "[*] Button click: button: %d, clickType: %d\n",
    //        button, click_type);
}


int gui_is_focused()
{
    return is_focused;
}


void mouse_move_to(const int x, const int y)
{
    // set x = curr_x + dx;
    xcb_test_fake_input(conn, XCB_MOTION_NOTIFY, 0, XCB_CURRENT_TIME, XCB_NONE,
            x, y, 0);
    xcb_flush(conn);
}


void mouse_move_relative(const int dx, const int dy)
{
    // Get current mouse X
    xcb_generic_error_t *error;
    xcb_query_pointer_reply_t *mouse_pos_reply = NULL;
    mouse_pos_reply = xcb_query_pointer_reply(conn,
            xcb_query_pointer(conn, screen->root), &error);
    if (error != NULL) {
        fprintf(stderr, "[-] Failed to query pointer\n");
        free(error);
        return;
    }

    int x = dx + mouse_pos_reply->root_x;
    int y = dy + mouse_pos_reply->root_y;

    free(mouse_pos_reply);

    // set x = curr_x + dx;
    xcb_test_fake_input(conn, XCB_MOTION_NOTIFY, 0, XCB_CURRENT_TIME, XCB_NONE,
            x, y, 0);
    xcb_flush(conn);
}


void keyboard_press(const unsigned char key_code, const int down)
{
    if (key_code == 0) {
        return;
    }
    xcb_test_fake_input(conn, down ? XCB_KEY_PRESS : XCB_KEY_RELEASE,
            key_code, XCB_CURRENT_TIME, XCB_NONE, 0, 0, 0);
    xcb_flush(conn);
}

void unhook_keyboard()
{

}


void unhook_mouse()
{
    
}


void gui_setup()
{
    conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn)) {
        fprintf(stderr, "[-] Could not open display\n");
        return;
    }
    screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
    if (screen == NULL) {
        fprintf(stderr, "[-] Could not get root screen\n");
        return;
    }
}


void gui_unlock_mouse()
{
    // xcb_ungrab_pointer(conn, XCB_CURRENT_TIME);
    // xcb_flush(conn);
}


void gui_event_loop()
{
    xcb_generic_event_t *event;
    while(1) {
        event = xcb_wait_for_event(conn);
        if (event == NULL) {
            break;
        }
        uint8_t event_type = event->response_type & ~0x80;
        switch (event_type) {
        case XCB_FOCUS_IN:
            is_focused = 1;
            //fprintf(stderr, "[*] Focus in\n");
            break;
        case XCB_FOCUS_OUT:
            is_focused = 0;
            //fprintf(stderr, "[*] Focus out\n");
            break;
        case XCB_EXPOSE:
            {
                xcb_expose_event_t *expose_event = \
                        (xcb_expose_event_t *)event;
                xcb_rectangle_t redraw_rect = {
                    expose_event->x,
                    expose_event->y,
                    expose_event->width,
                    expose_event->height,
                };
                xcb_poly_fill_rectangle(conn, window, foreground, 1,
                        &redraw_rect);
                xcb_flush(conn);
                //fprintf(stderr, "[*] Expose\n");
            }
            break;
        case XCB_KEY_PRESS:
            {
                xcb_key_press_event_t *key_press_event = \
                        (xcb_key_press_event_t *)event;
                int key_pressed = key_press_event->detail;
                register_key_stroke(key_pressed, 1);
                //fprintf(stderr, "[*] Key pressed\n");
            }
            break;
        case XCB_KEY_RELEASE:
            {
                xcb_key_release_event_t *key_release_event = \
                        (xcb_key_release_event_t *)event;
                int key_released = key_release_event->detail;
                register_key_stroke(key_released, 0);
                //fprintf(stderr, "[*] Key released\n");
            }
            break;
        case XCB_MOTION_NOTIFY:
            {
                xcb_motion_notify_event_t *motion_event = \
                        (xcb_motion_notify_event_t *)event;
                register_mouse_move(motion_event->root_x, motion_event->root_y);
                //fprintf(stderr, "[*] Pointer motion\n");
            }
            break;
        case XCB_BUTTON_PRESS:
            {
                xcb_button_press_event_t *button_press_event = \
                        (xcb_button_press_event_t *)event;
                register_mouse_click(button_press_event->detail, 1);
                //fprintf(stderr, "[*] Button pressed\n");
            }
            break;
        case XCB_BUTTON_RELEASE:
            {
                xcb_button_release_event_t *button_release_event = \
                        (xcb_button_release_event_t *)event;
                register_mouse_click(button_release_event->detail, 0);
                //fprintf(stderr, "[*] Button released\n");
            }
            break;
        //case XCB_MAPPING_NOTIFY:
        //    fprintf(stderr, "[*] Mapping Notification\n");
        //    break;
        default:
            fprintf(stderr, "[*] Default type: %d\n", event_type);
            break;
        }

        free(event);
    }
}


void gui_get_window_center(int *window_x, int *window_y)
{
    xcb_generic_error_t *err;
    xcb_get_geometry_reply_t *geo_reply = NULL;
    geo_reply = xcb_get_geometry_reply(conn,
            xcb_get_geometry(conn, window), &err);
    if (err != NULL) {
        fprintf(stderr, "[-] Failed to get window center\n");
        free(err);
        return;
    }
    if (geo_reply == NULL) {
        fprintf(stderr, "[-] xcb_get_geometry returned NULL\n");
        return;
    }

    *window_x = geo_reply->x + (geo_reply->width/2);
    *window_y = geo_reply->y + (geo_reply->height/2);

    free(geo_reply);
}


void gui_cleanup()
{
    xcb_free_gc(conn, foreground);
    xcb_free_gc(conn, background);
    xcb_disconnect(conn);
}


/* This will freeze program, I would like to revisit this later
void create_passive_hook() {
    xcb_generic_error_t *err;
    err = xcb_request_check(conn,
            xcb_grab_key_checked(conn, 1, screen->root,
                    0, // no modifier
                    96, // X_KEYCODE_f12 XXX: Configuration value here
                    XCB_GRAB_MODE_ASYNC,
                    XCB_GRAB_MODE_SYNC)
            );
    if (err != NULL) {
        fprintf(stderr, "[-] Error hooking keyboard\n");
        free(err);
        return;
    }
}
*/
