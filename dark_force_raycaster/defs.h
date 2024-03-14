#ifndef DEFS_H
#define DEFS_H


#define PI     3.14159265f
#define TWO_PI 6.28318530f

#define STRETCHED_TEXTURING false
#define MULTIPLE_LEVELS     true
#define RENDER_CEILING      !MULTIPLE_LEVELS


// shading constants
#define RENDER_SHADED        true
#define OBJECT_INTENSITY       0.5f
#define MULTIPLIER_INTENSITY   5.0f
#define INTENSITY_SPEED        1.0f

#define SHADE_FACTOR_MIN  0.1f      // the shade factor is clamped between these two values
#define SHADE_FACTOR_MAX  1.0f

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 420

#define MAP_NUM_ROWS 14
#define MAP_NUM_COLS 20
#define MAXHEIGHT 4
#define TILE_SIZE 64

#define BLOCK_EMPTY '.'     // no block
#define BLOCK_FULL '#'     // block of height 1

// Fractional block constants - you can also set blocks of height 0.1f, 0.2f, etc by specifying '1', '2', etc resp.
#define BLOCK_1QRTR 'Q'    // block of height 1/4
#define BLOCK_HALVE 'H'    //                 2/4
#define BLOCK_3QRTR 'T'    //                 3/4

#define MINIMAP_SCALE_FACTOR 0.1   // should be 0.1

#define NUM_TEXTURES 14
#define TEXTURE_WIDTH 64
#define TEXUTRE_HEIGHT 64

#define NUM_SPRITES 5
#define FOV_ANGLE (60 * (PI / 180))

#define NUM_RAYS WINDOW_WIDTH
//640               .5775
#define DIST_TO_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2)) //1108.51

#define FPS 60
#define FRAME_TIME_LENGTH (1000 / FPS)


#define FACE_UNKNOWN  -1
#define FACE_EAST      0
#define FACE_NORTH     1
#define FACE_WEST      2
#define FACE_SOUTH     3
#define FACE_TOP       4
#define FACE_BOTTOM    5
#define FACE_NR_OF     6

#endif // #ifndef DEFS_H
