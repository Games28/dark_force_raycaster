#include "Map.h"

void Map::initMap()
{
   
    // Initialise nMap as a 2d array of ints, having the same size as sMap, and containing the height per cell.
    // NOTE - if MULTIPLE_LEVELS is false, the nMap will contain only 0 and 1 values
    sMap.append("............###.................");
    sMap.append(".*#########################....#");
    sMap.append(".#............................##");
    sMap.append(".#..............Q.H.T.#.......@.");
    sMap.append(".#............................@.");
    sMap.append(".#............................@.");
    sMap.append(".#...................Q........@.");
    sMap.append(".#...................H........@.");
    sMap.append(".#...................T........@.");
    sMap.append(".#...................#........@.");
    sMap.append(".#...................T........@.");
    sMap.append(".#.......*#.#*.......H........@.");
    sMap.append(".#...@...#...#.......Q........#.");
    sMap.append(".#.......#...#................@.");
    sMap.append(".#...*....@@@.................#.");
    sMap.append(".#............................@.");
    sMap.append(".#...-..........1.............#.");
    sMap.append(".#...............2............@.");
    sMap.append(".#...+............3...........#.");
    sMap.append(".#.................4..........@.");
    sMap.append(".#...=..............5.........#.");
    sMap.append(".#.........1234......6........@.");
    sMap.append(".#............5.......7.......#.");
    sMap.append(".#.........9876........8......@.");
    sMap.append(".#......................9.....@.");
    sMap.append(".#.......................#....@.");
    sMap.append("..............................@.");
    sMap.append("..#@*-+++===#@*.*@#===+++---***.");
    sMap.append("..............#.#...............");
    sMap.append("................................");
    sMap.append("................................");
    sMap.append("................................");

    // Initialise fMap as a 2d array of floats, having the same size as sMap, and containing the height per cell.
    // NOTE - if MULTIPLE_LEVELS is false, the fMap will contain no values > 1
    fMap = new float[nMapX * nMapY];
    for (int y = 0; y < nMapY; y++) {
        for (int x = 0; x < nMapX; x++) {
            switch (sMap[y * nMapX + x]) {
            case GRND_FLOOR: fMap[y * nMapX + x] = 0.0f; break;
            case FRST_FLOOR: fMap[y * nMapX + x] = 1.0f; break;
            case SCND_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 2.0f : 1.0f; break;
            case THRD_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 3.0f : 1.0f; break;
            case FRTH_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 4.0f : 1.0f; break;
            case FFTH_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 5.0f : 1.0f; break;
            case SXTH_FLOOR: fMap[y * nMapX + x] = MULTIPLE_LEVELS ? 6.0f : 1.0f; break;

            case FLOOR_1QRTR: fMap[y * nMapX + x] = 0.25f; break;
            case FLOOR_HALVE: fMap[y * nMapX + x] = 0.50f; break;
            case FLOOR_3QRTR: fMap[y * nMapX + x] = 0.75f; break;

            case         '1': fMap[y * nMapX + x] = 0.10f; break;
            case         '2': fMap[y * nMapX + x] = 0.20f; break;
            case         '3': fMap[y * nMapX + x] = 0.30f; break;
            case         '4': fMap[y * nMapX + x] = 0.40f; break;
            case         '5': fMap[y * nMapX + x] = 0.50f; break;
            case         '6': fMap[y * nMapX + x] = 0.60f; break;
            case         '7': fMap[y * nMapX + x] = 0.70f; break;
            case         '8': fMap[y * nMapX + x] = 0.80f; break;
            case         '9': fMap[y * nMapX + x] = 0.90f; break;
            }
        }
    }

    fMaxDistance = sqrt(nMapX * nMapX + nMapY * nMapY);
}
