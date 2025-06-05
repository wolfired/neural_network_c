#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <MiniFB.h>
#include <lodepng.h>

#include "bresenham.h"

const unsigned int WID = 800;
const unsigned int HEI = 600;

static uint32_t* buffer = NULL;

void shade(int32_t x, int32_t y) {
    buffer[y * WID + x] = MFB_RGB(0, 0, 0);
}

static int latest_x         = -1;
static int latest_y         = -1;
static int is_right_clicked = 0;

void mouse_click(struct mfb_window* window, mfb_mouse_button button, mfb_key_mod mod, bool isPressed) {
    if(MOUSE_LEFT == button) {
        if(!isPressed) { return; }

        if(-1 == latest_x || -1 == latest_y) {
            latest_x = mfb_get_mouse_x(window);
            latest_y = mfb_get_mouse_y(window);
        } else {
            bresenham_line(latest_x, latest_y, mfb_get_mouse_x(window), mfb_get_mouse_y(window), shade);

            latest_x = -1;
            latest_y = -1;
        }
    } else if(MOUSE_RIGHT == button) {
        if(isPressed) {
            is_right_clicked = 1;
            shade(mfb_get_mouse_x(window), mfb_get_mouse_y(window));
        } else {
            is_right_clicked = 0;
        }
    }
}

int d = 0;

void mouse_move(struct mfb_window* window, int x, int y) {
    if(0 == is_right_clicked) { return; }
    shade(x, y);
}

void keyboard_click(struct mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed) {
    if(!isPressed) { return; }
    switch(key) {
    case KB_KEY_W: d = 1; break;
    case KB_KEY_S: d = 0; break;
    case KB_KEY_A: d = 3; break;
    case KB_KEY_D: d = 2; break;
    default: break;
    }
}

unsigned char* img   = NULL;
unsigned int   img_w = 0;
unsigned int   img_h = 0;

void play_frame(int d, int f, int tx, int ty) {
    int sx = f * 32;
    int sy = d * 32;
    for(size_t j = sy; j < sy + 32; ++j) {
        for(size_t i = sx; i < sx + 32; ++i) {
            int offset = (j * img_w + i) * 4;

            if(255 == img[offset + 3]) { buffer[(ty + j - sy) * WID + tx + i - sx] = MFB_ARGB(img[offset + 3], img[offset + 0], img[offset + 1], img[offset + 2]); }
        }
    }
}

int main(int argc, char** argv) {
    lodepng_decode32_file(&img, &img_w, &img_h, "E:\\Desktop\\4.png");

    printf("%dx%d\n", img_w, img_h);

    struct mfb_window* window = mfb_open_ex("sky", WID, HEI, 0);
    struct mfb_timer*  timer  = mfb_timer_create();

    if(NULL == window) { return EXIT_FAILURE; }

    mfb_set_mouse_button_callback(window, mouse_click);
    mfb_set_mouse_move_callback(window, mouse_move);
    mfb_set_keyboard_callback(window, keyboard_click);

    buffer = (uint32_t*)malloc(WID * HEI * sizeof(uint32_t));

    double t  = 0;
    int    f  = 0;
    double de = 1.0 / 4.0;

    mfb_update_state state;
    do {
        for(size_t i = 0; i < WID * HEI; i++) { buffer[i] = MFB_RGB(255, 255, 255); }

        bresenham_line(0, 0, WID - 1, HEI - 1, shade);
        bresenham_circ(100, 100, 10, shade);

        t += mfb_timer_delta(timer);
        if(de <= t) {
            t -= de;
            ++f;
        }
        play_frame(d, f % 3, 0, 0);

        state = mfb_update_ex(window, buffer, WID, HEI);
        if(state != STATE_OK) {
            mfb_close(window);
            window = NULL;
            break;
        }
    } while(mfb_wait_sync(window));

    free(img);

    mfb_timer_destroy(timer);

    return EXIT_SUCCESS;
}
