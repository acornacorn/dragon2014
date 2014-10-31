#ifndef DRASGON_PINS_H
#define DRASGON_PINS_H

// Alive LED
static const int dpin_alive_led = 13;

// Servos
static const int dpin_look_servo = 14;
static const int dpin_leye_servo = 15;
static const int dpin_reye_servo = 16;
static const int dpin_lips_servo = 17;

static const int sv_look_min = 29;
static const int sv_look_mid = 107;
static const int sv_look_max = 176;

static const int sv_leye_min = 29;
static const int sv_leye_mid = 107;
static const int sv_leye_max = 176;
static const int sv_leye_closed = 107;

static const int sv_reye_min = 29;
static const int sv_reye_mid = 107;
static const int sv_reye_max = 176;
static const int sv_reye_closed = 107;

static const int sv_lips_min = 29;
static const int sv_lips_mid = 107;
static const int sv_lips_max = 176;



// eye LEDS
static const int dpin_eye_red = 21;
static const int apin_eye_red = 7;
static const int dpin_eye_green = 22;
static const int apin_eye_green = 8;
static const int dpin_eye_blue = 23;
static const int apin_eye_blue = 9;

// buttons
static const int dpin_button0 = 12;
static const int dpin_button1 = 11;
static const int dpin_button2 = 10;
static const int dpin_button3 = 9;
static const int dpin_button4 = 8;
static const int dpin_button5 = 7;
static const int dpin_button6 = 6;
static const int dpin_button7 = 5;

#endif
