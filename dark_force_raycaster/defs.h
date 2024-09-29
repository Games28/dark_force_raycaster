#ifndef DEFS_H
#define DEFS_H


#define PI     3.14159265f
#define TWO_PI 6.28318530f

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT    600
#define PIXEL_X       1
#define PIXEL_Y       1



// colour constants
#define COL_CEIL    olc::CYAN
#define COL_FLOOR   olc::DARK_YELLOW
#define COL_WALL    olc::GREY
#define COL_TEXT    olc::MAGENTA
#define STRETCHED_TEXTURING  false
#define MULTIPLE_LEVELS true

#define SPEED_STRAFE_UP    1.0f  


#define GRND_FLOOR '.'     // no block
#define FRST_FLOOR '#'     // block of height 1
#define SCND_FLOOR '@'     //                 2
#define THRD_FLOOR '*'     //                 3
#define FRTH_FLOOR '-'     //                 4
#define FFTH_FLOOR '+'     //                 5
#define SXTH_FLOOR '='     //                 6

#define FLOOR_1QRTR 'Q'    // block of height 1/4
#define FLOOR_HALVE 'H'    //                 2/4
#define FLOOR_3QRTR 'T'    //                 3/4


// constants for speed movements - all movements are modulated with fElapsedTime
#define SPEED_ROTATE      60.0f   //                          60 degrees per second
#define SPEED_MOVE         5.0f   // forward and backward -    5 units per second
#define SPEED_STRAFE       5.0f   // left and right strafing - 5 units per second
#define SPEED_LOOKUP     200.0f

#define SIGNIFICANCE 3
#define SIG_POW10    1000


static float deg2rad(float fAngleDeg) { return fAngleDeg * PI / 180.0f; }
static float rad2deg(float fAngleRad) { return fAngleRad / PI * 180.0f; }
static float deg_mod2pi(float fAngleDeg) {
    while (fAngleDeg < 0.0f) fAngleDeg += 360.0f;
    while (fAngleDeg >= 360.0f) fAngleDeg -= 360.0f;
    return fAngleDeg;
}
static float rad_mod2pi(float fAngleRad) {
    while (fAngleRad < 0.0f) fAngleRad += 2.0f * PI;
    while (fAngleRad >= 2.0f * PI) fAngleRad -= 2.0f * PI;
    return fAngleRad;
}

static float lu_sin_array[360 * SIG_POW10];
static float lu_cos_array[360 * SIG_POW10];

static void init_lu_sin_array() {
    for (int i = 0; i < 360; i++) {
        for (int j = 0; j < SIG_POW10; j++) {
            int nIndex = i * SIG_POW10 + j;
            float fArg_deg = float(nIndex) / float(SIG_POW10);
            lu_sin_array[nIndex] = sinf(deg2rad(fArg_deg));
        }
    }
}

static void init_lu_cos_array() {
    for (int i = 0; i < 360; i++) {
        for (int j = 0; j < SIG_POW10; j++) {
            int nIndex = i * SIG_POW10 + j;
            float fArg_deg = float(nIndex) / float(SIG_POW10);
            lu_cos_array[nIndex] = cosf(deg2rad(fArg_deg));
        }
    }
}

static float lu_sin(float fDegreeAngle) {
    fDegreeAngle = deg_mod2pi(fDegreeAngle);
    int nWholeNr = int(fDegreeAngle);
    int nRemainder = int((fDegreeAngle - nWholeNr) * float(SIG_POW10));
    int nIndex = nWholeNr * SIG_POW10 + nRemainder;
    return lu_sin_array[nIndex];
}

static float lu_cos(float fDegreeAngle) {
    fDegreeAngle = deg_mod2pi(fDegreeAngle);
    int nWholeNr = int(fDegreeAngle);
    int nRemainder = int((fDegreeAngle - nWholeNr) * float(SIG_POW10));
    int nIndex = nWholeNr * SIG_POW10 + nRemainder;
    return lu_cos_array[nIndex];
}



#endif // #ifndef DEFS_H
