#ifndef DRASGON_PINS_H
#define DRASGON_PINS_H

// Alive LED
static const int dpin_alive_led = 13;

// Servos
static const int dpin_look_servo = 14;
static const int dpin_leye_servo = 15;
static const int dpin_reye_servo = 16;
static const int dpin_lips_servo = 17;

//================ LOOK

static const int sv_look_min = 68;  // right (burst only)
static const int sv_look_mid = 93;
static const int sv_look_max = 120; // left (burst only)
// max/min steady state
static const int sv_look_min_ss = 82;  // right
static const int sv_look_max_ss = 110; // left

static const int sv_look_mid_from_r = 97;
static const int sv_look_mid_from_l = 89;

// back off by this amount to avoid jitter
static const int sv_look_backoff = 4;

//================ LEYE

static const int sv_leye_min = 67; // open
static const int sv_leye_mid = 101;
static const int sv_leye_max = 168; // closed

static const int sv_leye_wide_open = 67;
static const int sv_leye_droopy = 81;
static const int sv_leye_guarded = 101;
static const int sv_leye_angry = 115;

static const int sv_leye_closed = 168;

//================ REYE

static const int sv_reye_min = 71; // closed
static const int sv_reye_mid = 139;
static const int sv_reye_max = 171; // open

static const int sv_reye_wide_open = 170;
static const int sv_reye_droopy = 156;
static const int sv_reye_guarded = 139;
static const int sv_reye_angry = 123;

static const int sv_reye_closed = 71;

//================ LIPS

static const int sv_lips_min = 59;    // open
static const int sv_lips_mid = 160;
static const int sv_lips_max = 178;  // closed

static const int sv_lips_soft_max = 162;
static const int sv_lips_back_teeth_closed = 162;
static const int sv_lips_back_teeth_open = 136;

static const int sv_lips_nose_closed = 136;
static const int sv_lips_nose_open = 63;




// eye LEDS
static const int dpin_eye_red = 21;
static const int apin_eye_red = 7;
static const int dpin_eye_green = 22;
static const int apin_eye_green = 8;
static const int dpin_eye_blue = 23;
static const int apin_eye_blue = 9;


// colors C(R,G,B)  RGB are 0...255
#define COLOR_ANGRY_RED         C(101,0,0)
#define COLOR_ICE               C(207,0,236)
#define COLOR_MAGENTA           C(207,0,22)
#define COLOR_ICE1              C(112,0,200)
#define COLOR_ICE_BLUE          C(44,0,200)
#define COLOR_ICE_VERY_BLUE     C(0,0,200)
#define COLOR_GREEN             C(0,200,0)
#define COLOR_GOLD              C(255,119,0)
#define COLOR_GOLD_ORANGE       C(255,42,0)
#define COLOR_SICKLY            C(255,219,0)
#define COLOR_WHITE             C(255,219,103)


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
