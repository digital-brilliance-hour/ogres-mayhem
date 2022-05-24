
//#include "data/scripts/defines.c"
//#import "data/scripts/lib.c"

#define T_UNWALKABLE_WALL 1000
#define T_NOCOL_WALL 2000
#define T_EMPTY_WALL 3000

// Tra l'altro il muro sulle ordinate si vede se x == 0 e z > oppure < 0...
// Guarda a 360° (le 8 direzioni) rispetto al player se c'è un muro di altitudine definita dall'utente
int checksplattablewall(void player, int distx, int distz, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float altitude = 0, rounded = 0, margin = 0;
/*
 * f:  1,0
 * d/f:1,1
 * d:  0,1
 * d/b:-1,1
 * b:  -1,0
 * u/b:-1,-1
 * u:0,-1
 * u/f:1,-1
 */
 // axis 0 == 4 axis, 1 == horz axis, 2 == vert axis

    altitude = checkwall(x,z);
    if (altitude > 0) {
        rounded = altitude%(altitude*altitude); // arrotondiamo
        margin = altitude-rounded;
        if ( margin >= 0.000900 && margin <= 0.001 ) return 1;
    }

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search veryical only

            altitude = checkwall(x+(i*distx),z+(j*distz));
            if (altitude > 0) {
                    rounded = altitude%(altitude*altitude); // arrotondiamo
                    margin = altitude-rounded;
                    if ( margin >= 0.000900 && margin <= 0.001 ) return 1;
                    else continue;
            } else continue;
        }
    }

    return 0;
}

// Guarda a 360° (le 8 direzioni) rispetto al player se c'è un muro di altitudine definita dall'utente
int checksplattablewall_with_param(void player, int distx, int distz, float altitude, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");

/*
 * f:  1,0
 * d/f:1,1
 * d:  0,1
 * d/b:-1,1
 * b:  -1,0
 * u/b:-1,-1
 * u:0,-1
 * u/f:1,-1
 */
 // axis 0 == 4 axis, 1 == horz axis, 2 == vert axis

    if ( checkwall(x,z) == altitude ) return 1;

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search veryical only

            if ( checkwall(x+(i*distx),z+(j*distz)) == altitude ) return 1;
            else continue;
        }
    }

    return 0;
}

int checkholearound(void player, int distx, int distz, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");

/*
 * f:  1,0
 * d/f:1,1
 * d:  0,1
 * d/b:-1,1
 * b:  -1,0
 * u/b:-1,-1
 * u:0,-1
 * u/f:1,-1
 */
 // axis 0 == 4 axis, 1 == horz axis, 2 == vert axis

    if ( checkhole(x,z) == 1 ) return 1;

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search veryical only

            if ( checkhole(x+(i*distx),z+(j*distz)) == 1 ) return 1;
            else continue;
        }
    }

    return 0;
}

float get_wall_on_by_coords(float x, float z, float a, float base) {
    float wall = checkwall(x,z);

    if ( base == wall ) return wall;
    else return 0;
}

float get_wall_on(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");

    return get_wall_on_by_coords(x,z,a,base);
}

int is_a_wall_below(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float wall = checkwall(x,z);

    if ( wall > 0 && base == wall && a <= base ) return 1;
    else return 0;
}

int is_a_wall_below_by_coords(float x, float z, float a, float base) {
    float wall = checkwall(x,z);

    if ( wall > 0 && base == wall && a <= base ) return 1;
    else return 0;
}

int is_on_wall(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float wall = checkwall(x,z);

    if ( wall > 0 && base > 0 ) return 1; //  && a <= base
    else return 0;
}

int is_on_wall_by_coords(float x, float z, float a, float base) {
    float wall = checkwall(x,z);

    if ( wall > 0 && base > 0 ) return 1; //  && a <= base
    else return 0;
}

int there_is_a_wall(float x, float z, float a) {
    float wall_height = checkwall(x,z);

    if ( wall_height > 0 && wall_height == a ) return 1;
    else return 0;
}

float checkwallbelow(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float wall = checkwall(x,z);

    return wall;
}

float checkwallbelow_base(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float base = getentityproperty(player, "base");
    float wall = checkwall(x,z);

    if ( wall > base ) return wall;
    else return 0;
}

float checkwallbelow_a(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float wall = checkwall(x,z);

    if ( wall > a ) return wall;
    else return 0;
}

// Guarda a 360° (le 8 direzioni) rispetto al player se c'è un muro di altitudine definita dall'utente
float checkwallaround(void player, float distx, float distz, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");

/*
 * f:  1,0
 * d/f:1,1
 * d:  0,1
 * d/b:-1,1
 * b:  -1,0
 * u/b:-1,-1
 * u:0,-1
 * u/f:1,-1
 */
 // axis 0 == 4 axis, 1 == horz axis, 2 == vert axis

    if ( checkwall(x,z) > 0 ) return 1;

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search veryical only

            if ( checkwall(x+(i*distx),z+(j*distz)) > 0 ) return 1;
            else continue;
        }
    }

    return 0;
}

float checkwallaround_in_a(void player, float distx, float distz, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");

/*
 * f:  1,0
 * d/f:1,1
 * d:  0,1
 * d/b:-1,1
 * b:  -1,0
 * u/b:-1,-1
 * u:0,-1
 * u/f:1,-1
 */
 // axis 0 == 4 axis, 1 == horz axis, 2 == vert axis

    if ( checkwall(x,z) >= a ) return 1;

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search veryical only

            if ( checkwall(x+(i*distx),z+(j*distz)) >= a ) return 1;
            else continue;
        }
    }

    return 0;
}

float getwallaround(void player, float distx, float distz, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float altitude;

/*
 * f:  1,0
 * d/f:1,1
 * d:  0,1
 * d/b:-1,1
 * b:  -1,0
 * u/b:-1,-1
 * u:0,-1
 * u/f:1,-1
 */
 // axis 0 == 4 axis, 1 == horz axis, 2 == vert axis

    altitude = checkwall(x,z);
    if ( altitude > 0 ) return altitude;

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search veryical only

            altitude = checkwall(x+(i*distx),z+(j*distz));

            if ( altitude > 0 ) return altitude;
            else continue;
        }
    }

    return 0;
}

float getwallaround_in_a(void player, float distx, float distz, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float altitude;

/*
 * f:  1,0
 * d/f:1,1
 * d:  0,1
 * d/b:-1,1
 * b:  -1,0
 * u/b:-1,-1
 * u:0,-1
 * u/f:1,-1
 */
 // axis 0 == 4 axis, 1 == horz axis, 2 == vert axis

    altitude = checkwall(x,z);
    if ( altitude >= a ) return altitude;

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search veryical only

            altitude = checkwall(x+(i*distx),z+(j*distz));

            if ( altitude >= a ) return altitude;
            else continue;
        }
    }

    return 0;
}

int is_no_collision_wall(float alt) {
    if ( alt == T_NOCOL_WALL ) return 1;
    else return 0;
}

int is_empty_wall(float alt) {
    if ( alt == T_EMPTY_WALL ) return 1;
    else return 0;
}

int is_unwalkable_wall(float alt) {
    if ( alt >= T_UNWALKABLE_WALL ) return 1;
    else return 0;
}

