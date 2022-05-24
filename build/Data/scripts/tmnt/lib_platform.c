
#include "data/scripts/defines.h"
//#import "data/scripts/lib.c"

#define VISIB_FACT 2.0
#define T_CHECK_PLATFORM 2.0
#define MAX_PLATFORM_ALT 500.0

// Retrieves platform entity handler if player "y" is about the same height of the platform
void find_platform_in_a(float distx, float distz, float a, float alt_threshold, float shifty, float h_fix) {
    void platform = NULL(), platform_up = NULL();
    float height = NULL(), altitude = NULL();

    if ( a+MAX_PLATFORM_ALT < alt_threshold ) alt_threshold = a+MAX_PLATFORM_ALT;

    if ( h_fix == NULL() ) h_fix = 1;
    if ( shifty == NULL() ) shifty = 0;
    do {
        platform = checkplatform(distx,distz,alt_threshold);
        if ( platform == NULL() ) return NULL();

        height = getentityproperty(platform, "height");
        altitude = getentityproperty(platform, "y");
        if ( height == NULL() ) height = 0;

        if ( a >= altitude-shifty && a <= altitude+(height/h_fix)+shifty ) {
            return platform;
        }

        if ( altitude > a ) {
            platform_up = platform;
            alt_threshold = altitude+height-1;
            continue;
        }

        if ( platform == platform_up ) break;
        if ( a >= altitude || alt_threshold <= 0 ) break;

        --alt_threshold;
    } while( checkplatform(distx,distz,alt_threshold) != NULL() );

    return NULL();
}

// Retrieves platform entity handler if player "y" and his height is about the same height of the platform
void find_platform_in_a_with_height(float distx, float distz, float a, float player_height, float alt_threshold, float shifty, float h_fix) {
    void platform = NULL(), platform_up = NULL();
    float height = NULL(), altitude = NULL();

    if ( a+MAX_PLATFORM_ALT < alt_threshold ) alt_threshold = a+MAX_PLATFORM_ALT;

    if ( h_fix == NULL() ) h_fix = 1;
    if ( shifty == NULL() ) shifty = 0;
    do {
        platform = checkplatform(distx,distz,alt_threshold);
        if ( platform == NULL() ) return NULL();

        height = getentityproperty(platform, "height");
        altitude = getentityproperty(platform, "y");
        if ( height == NULL() ) height = 0;

        if ( a+player_height >= altitude-shifty && a <= altitude+(height/h_fix)+shifty ) {
            return platform;
        }

        if ( altitude > a ) {
            platform_up = platform;
            alt_threshold = altitude+height-1;
            continue;
        }

        if ( platform == platform_up ) break;
        if ( a >= altitude || alt_threshold <= 0 ) break;

        --alt_threshold;
    } while( checkplatform(distx,distz,alt_threshold) != NULL() );

    return NULL();
}

// returns platform entity if there's a platform under y
void get_platform_below_y(void player) {
    float x = getentityproperty(player,"x");
    float z = getentityproperty(player,"z");
    float y = getentityproperty(player,"y");
    float base = getentityproperty(player,"base");
    void platform;
    float threshold = 0.1;

    if ( y > base ) platform = checkplatform(x,z,y+threshold);
    else platform = checkplatform(x,z,base+threshold);

    return platform;
}

// returns 1 if you're on platform and character is on base
int is_a_platform_below(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");

    return is_a_platform_below_by_coords(x,z,a,base);
}

int is_a_platform_below_by_coords(float x, float z, float a, float base) {
    void platform;
    float threshold = T_CHECK_PLATFORM;

    platform = checkplatform(x,z,base+threshold);
    if ( platform != NULL() && getentityproperty(platform,"exists") ) {
        float platform_alt = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");

        if ( platform_height == NULL() ) platform_height = 0;

        if ( platform_alt+platform_height == base && a <= base ) return 1;
        else return 0;
    } else return 0;
}

// returns 1 is you're on platform and on base
void get_platform_below(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");

    return get_platform_below_by_coords(x,z,a,base);
}

void get_platform_below_by_coords(float x, float z, float a, float base) {
    void platform;
    float threshold = T_CHECK_PLATFORM;

    platform = checkplatform(x,z,base+threshold);
    if ( platform != NULL() && getentityproperty(platform,"exists") ) {
        float platform_alt = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");

        if ( platform_height == NULL() ) platform_height = 0;

        if ( platform_alt+platform_height == base && a <= base ) return platform;
        else return NULL();
    } else return NULL();
}

// returns 1 if you're on platform
int is_on_platform(void player) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");

    return is_on_platform_by_coords(x,z,a,base);
}

int is_on_platform_by_coords(float x, float z, float a, float base) {
    void platform;
    float platform_height, platform_a;
    float threshold = T_CHECK_PLATFORM;

    platform = checkplatform(x,z,base+threshold);
    if ( platform != NULL() ) { // && a <= base
        platform_height = getentityproperty(platform,"height");
        if ( platform_height == NULL() ) platform_height = 0;
        platform_a = getentityproperty(platform,"y");

        if ( base == platform_a+platform_height ) return 1;
        else return 0;
    } else return 0;

    return 0;
}

// returns 1 if you're on platform with approx
int is_on_platform_in_range(void player, float range) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");

    return is_on_platform_by_coords_in_range(x,z,a,base,range);
}

int is_on_platform_by_coords_in_range(float x, float z, float a, float base, float range) {
    void platform;
    float threshold;

    if ( range != NULL() ) threshold = range;
    else threshold = T_CHECK_PLATFORM;

    platform = checkplatform(x,z,base+threshold);
    if ( platform != NULL() && getentityproperty(platform,"exists") ) {
        float platform_alt = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");
        float platform_base;

        if ( platform_height == NULL() ) platform_height = 0;
        platform_base = platform_alt+platform_height;

        if ( base+threshold >= platform_base && base-threshold <= platform_base ) return 1;
        else return 0;
    } else return 0;
}

// returns platform entity if you're on platform with approx
void get_platform_in_range(void player, float range) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");

    return get_platform_by_coords_in_range(x,z,a,base,range);
}

void get_platform_by_coords_in_range(float x, float z, float a, float base, float range) {
    void platform;
    float threshold;

    if ( range != NULL() ) threshold = range;
    else threshold = T_CHECK_PLATFORM;

    platform = checkplatform(x,z,base+threshold);
    if ( platform != NULL() && getentityproperty(platform,"exists") ) {
        float platform_alt = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");
        float platform_base;

        if ( platform_height == NULL() ) platform_height = 0;
        platform_base = platform_alt+platform_height;

        if ( base+threshold >= platform_base && base-threshold <= platform_base ) return platform;
        else return NULL();
    } else return NULL();
}

int there_is_a_platform(float x, float z, float a) {
    void platform;
    float platform_height, platform_a;

    platform = checkplatform(x,z,a);
    if ( platform != NULL() ) { // && a <= base
        platform_height = getentityproperty(platform,"height");
        if ( platform_height == NULL() ) platform_height = 0;
        platform_a = getentityproperty(platform,"y");

        if ( a == platform_a+platform_height ) return 1;
        else return 0;
    } else return 0;

    return 0;
}

float get_platform_on(void player, float t_walkoff) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");

    return get_platform_on_by_coords(x,z,a,base,t_walkoff);
}


void get_platform_on_by_coords(float x, float z, float a, float base, float t_walkoff) {
    void platform;
    float platform_height, platform_a;
    float threshold = 0;

    if ( t_walkoff != NULL() || t_walkoff != 0 ) threshold = 0.1;

    platform = checkplatform(x,z,base);
    if ( platform != NULL() ) { // && a <= base
        platform_height = getentityproperty(platform,"height");
        if ( platform_height == NULL() ) platform_height = 0;
        platform_a = getentityproperty(platform,"y");

        if ( t_walkoff != NULL() || t_walkoff != 0 ) {
            if ( base >= platform_a+platform_height-t_walkoff && base <= platform_a+platform_height+t_walkoff ) return platform;
        }

        if ( base == platform_a+platform_height ) return platform;
        else return NULL();
    } else return NULL();

    return NULL();
}

float get_platform_on_alt(void player, float t_walkoff) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");

    return get_platform_on_alt_by_coords(x,z,a,base,t_walkoff);
}

float get_platform_on_alt_by_coords(float x, float z, float a, float base, float t_walkoff) {
    void platform;
    float platform_height, platform_a;
    float threshold = 0;

    if ( t_walkoff != NULL() || t_walkoff != 0 ) threshold = 0.1;

    platform = checkplatform(x,z,base);
    if ( platform != NULL() ) { // && a <= base
        platform_height = getentityproperty(platform,"height");
        if ( platform_height == NULL() ) platform_height = 0;
        platform_a = getentityproperty(platform,"y");

        if ( t_walkoff != NULL() || t_walkoff != 0 ) {
            if ( base >= platform_a+platform_height-t_walkoff && base <= platform_a+platform_height+t_walkoff ) return platform_a;
        }

        if ( base == platform_a+platform_height ) return platform_a;
        else return -1;
    } else return -1;

    return -1;
}

void get_platform_by_coords(float x, float z, float a) {
    void platform;
    float platform_height, platform_a;

    platform = checkplatform(x,z,a);
    if ( platform != NULL() ) {
        return platform;
    } else return NULL();

    return NULL();
}

float get_platform_alt_by_coords(float x, float z, float a) {
    void platform;
    float platform_height, platform_a;

    platform = checkplatform(x,z,a);
    if ( platform != NULL() ) {
        platform_a = getentityproperty(platform,"y");

        return platform_a;
    } else return 0;

    return 0;
}

float get_platform_alt_plus_h_by_coords(float x, float z, float a) {
    void platform;
    float platform_height, platform_a;

    platform = checkplatform(x,z,a);
    if ( platform != NULL() ) {
        platform_height = getentityproperty(platform,"height");
        if ( platform_height == NULL() ) platform_height = 0;
        platform_a = getentityproperty(platform,"y");

        return (platform_a+platform_height);
    } else return 0;

    return 0;
}

// Retrieve if let be x,z,a, you can be in a platform
void find_platform_in_base(float distx, float distz, float a, float base) {
    void platform;

    platform = checkplatform(distx,distz,a);
    if ( platform != NULL() ) {
        float platform_alt = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");

        if ( platform_alt+platform_height <= a && a <= base ) return platform;
    }

    return NULL();
}

// Retrieve a+height of a platform that is in x,z,a
float get_platform_a_plus_h_in_xza(float distx, float distz, float a, float base) {
    void platform;

    platform = checkplatform(distx,distz,a);
    if ( platform != NULL() ) {
        float platform_alt = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");

        if ( platform_alt+platform_height <= a && a <= base ) return (platform_alt+platform_height);
    }

    return 0;
}

void find_platform_around_in_a(void player, float distx, float distz, float a, float altitude, int platform_on_flag, int axis, float shifty) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    //float base = getentityproperty(player, "base");
    void platform;

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

    platform = find_platform_in_a(x,z,a,altitude,shifty);
    if ( platform != NULL() ) {
        float platform_a = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");

        if ( platform_height == NULL() ) platform_height = 0;

        if ( platform_on_flag && (platform == get_platform_on(player) || platform_a+platform_height-VISIB_FACT < a) ) {
        } else return platform;
    }

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search vertical only

            if ( checkplatform(x+(i*distx),z+(j*distz),altitude) != NULL() ) {
                platform = find_platform_in_a(x+(i*distx),z+(j*distz),a,altitude,shifty);

                if ( platform != NULL() ) {
                    float platform_a = getentityproperty(platform,"y");
                    float platform_height = getentityproperty(platform,"height");

                    if ( platform_height == NULL() ) platform_height = 0;

                    if ( ( platform_on_flag && (platform == get_platform_on(player) || platform_a+platform_height-VISIB_FACT < a) ) ) continue;
                    else return platform;
                } else continue;
            } else continue;

        }
    }

    return NULL();
}

void find_platform_around_in_a_with_height(void player, float distx, float distz, float a, float height, float altitude, int platform_on_flag, int axis, float shifty) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    //float base = getentityproperty(player, "base");
    void platform;
    float div_fact = 1.05;

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

    platform = find_platform_in_a_with_height(x,z,a,height,altitude,shifty);
    if ( platform != NULL() ) {
        float platform_a = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");

        if ( platform_height == NULL() ) platform_height = 0;

        if ( (axis == 0 || axis == 1) && platform_a >= a+(height/div_fact) ) {
        } else {
            if ( platform_on_flag && (platform == get_platform_on(player) || platform_a+platform_height-VISIB_FACT < a) ) {
            } else return platform;
        }
    }

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search vertical only

            if ( checkplatform(x+(i*distx),z+(j*distz),altitude) != NULL() ) {
                platform = find_platform_in_a_with_height(x+(i*distx),z+(j*distz),a,height,altitude,shifty);

                if ( platform != NULL() ) {
                    float platform_a = getentityproperty(platform,"y");
                    float platform_height = getentityproperty(platform,"height");

                    if ( platform_height == NULL() ) platform_height = 0;

                    if ( (axis == 0 || axis == 1) && platform_a >= a+(height/div_fact) ) continue;
                    //if ( axis == 1 && a+(height/div_fact) >= platform_a && a+(height/div_fact) <= platform_a) ) return platform;

                    if ( platform_on_flag && (platform == get_platform_on(player) || platform_a+platform_height-VISIB_FACT < a) ) continue;
                    else return platform;
                } else continue;
            } else continue;

        }
    }

    return NULL();
}

// Retrieve platform altitude: bin search (most lower platform)
float check_platform_alt(float distx, float distz, int threshold) {
    float altitude = 0;
    int min = 0, max = threshold, mid = max;

    do {
        if ( checkplatform(distx,distz,mid) != NULL() ) {
            altitude = mid;
            max = mid;
        } else if (max == threshold) {
            return NULL();
        } else {
            min = mid;
        }

        mid = (max+min)/2;
        mid = mid%(mid+1); //arrotondiamo
    } while ( max > min+1 ); // +1 per l'arrotondamento per difetto altrimenti va in loop

    return altitude;
}

// Safe check on multiple platforms (tutte, non solo la più bassa a partire da quell'altezza) - WARNING costo computazionale alto
float check_multi_platform_alt(float distx, float distz, int threshold) {
    float altitude = 0;
    int min = 0, max = threshold, mid = max;

    do {
        if ( checkplatform(distx,distz,mid) != NULL() ) {
            altitude = mid;
            max = mid;
            min = getentityproperty(checkplatform(distx,distz,mid) , "y");
        } else if (max == threshold) {
            return NULL();
        } else {
            if ( mid > min ) min = mid;
        }

        mid = (max+min)/2;
        mid = mid%(mid+1); //arrotondiamo
    } while ( max > min+1 ); // +1 per l'arrotondamento per difetto altrimenti va in loop

    return altitude;
}

float checkplatformaround_alt(void player, int distx, int distz, float altitude, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    //float base = getentityproperty(player, "base");
    void platform;

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

    platform = checkplatform(x,z,altitude);
    if ( platform != NULL() ) {
        float platform_a = getentityproperty(platform,"y");
        /*float platform_height = getentityproperty(platform,"height");
        if ( platform_height == NULL() ) platform_height = 0;*/

        //return check_platform_alt(x,z,altitude);
        return platform_a;
    }

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search vertical only

            platform = checkplatform(x+(i*distx),z+(j*distz),altitude);
            if ( platform != NULL() ) {
                float platform_a = getentityproperty(platform,"y");
                /*float platform_height = getentityproperty(platform,"height");
                if ( platform_height == NULL() ) platform_height = 0;*/

                //return check_platform_alt(x+(i*distx),z+(j*distz),altitude);
                return platform_a;
            } else continue;
        }
    }

    return NULL();
}

void checkplatformaround(void player, int distx, int distz, float altitude, int axis) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    //float base = getentityproperty(player, "base");
    void platform;

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

    platform = checkplatform(x,z,altitude);
    if ( platform != NULL() ) {
        /*float platform_a = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");
        if ( platform_height == NULL() ) platform_height = 0;*/

        return platform;
    }

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( axis == 1 && j != 0 ) continue; // search horizontal only
            else if ( axis == 2 && i != 0 ) continue; // search vertical only

            platform = checkplatform(x+(i*distx),z+(j*distz),altitude);
            if ( platform != NULL() ) {
                /*float platform_a = getentityproperty(platform,"y");
                float platform_height = getentityproperty(platform,"height");
                if ( platform_height == NULL() ) platform_height = 0;*/

                return platform;
            } else continue;
        }
    }

    return NULL();
}

float get_platform_base(void platform) {
    float platform_base = 0;

    if ( platform != NULL() ) {
        float platform_a = getentityproperty(platform,"y");
        float platform_height = getentityproperty(platform,"height");

        if ( platform_height == NULL() ) platform_height = 0;
        platform_base = platform_a+platform_height;
    }

    return platform_base;
}

float get_platform_extreme_x(void platform) {
    float x = 0;

    if ( platform != NULL() ) {
        x = gep(platform,"x");
        float z = gep(platform,"z");
        float base = get_platform_base(platform);
        int i = 0;

        if ( gep(platform,"tossv") != 0 ) base += VISIB_FACT;

        for ( i = x; checkplatform(i,z,base) == platform; --i ) {
            x = i;
        }
    }

    return x;
}

float get_platform_width(void platform) {
    float platform_width = 0;

    if ( platform != NULL() ) {
        float x = gep(platform,"x");
        float z = gep(platform,"z");
        float base = get_platform_base(platform);
        int i = 0;

        if ( gep(platform,"tossv") != 0 ) base += VISIB_FACT;

        for ( i = x; checkplatform(i,z,base) == platform; --i ) {
            x = i;
        }
        for ( i = x; checkplatform(i,z,base) == platform; ++i ) {
            ++platform_width;
        }
    }

    return platform_width;
}

float get_platform_extreme_z(void platform) {
    float z = 0;

    if ( platform != NULL() ) {
        float x = gep(platform,"x");
        z = gep(platform,"z");
        float base = get_platform_base(platform);
        float platform_width = 0;
        int i = 0;

        if ( gep(platform,"tossv") != 0 ) base += VISIB_FACT;

        // get extremes x
        for ( i = x; checkplatform(i,z,base) == platform; --i ) {
            x = i;
        }
        for ( i = x; checkplatform(i,z,base) == platform; ++i ) {
            ++platform_width;
        }
        x += platform_width/2;

        // get extremes z
        for ( i = z; checkplatform(x,i,base) == platform; --i ) {
            z = i;
        }
    }

    return z;
}

float get_platform_depth(void platform) {
    float platform_depth = 0;

    if ( platform != NULL() ) {
        float x = gep(platform,"x");
        float z = gep(platform,"z");
        float base = get_platform_base(platform);
        float platform_width = 0;
        int i = 0;

        if ( gep(platform,"tossv") != 0 ) base += VISIB_FACT;

        // get extremes x
        for ( i = x; checkplatform(i,z,base) == platform; --i ) {
            x = i;
        }
        for ( i = x; checkplatform(i,z,base) == platform; ++i ) {
            ++platform_width;
        }
        x += platform_width/2;

        // get extremes z
        for ( i = z; checkplatform(x,i,base) == platform; --i ) {
            z = i;
        }
        for ( i = z; checkplatform(x,i,base) == platform; ++i ) {
            ++platform_depth;
        }
    }

    return platform_depth;
}

void checkplatform(float distx, float distz, float altitude) {
    void platform = checkplatformbelow(distx,distz,altitude);

    //log("platform: "+platform+"\n");

    //if ( platform == NULL() ) return NULL();
    if ( typeof(platform) == openborconstant("VT_PTR") ) {
        if ( platform != checkplatformbelow(-1,-1,-1) ) {
            //if ( getentityproperty(platform,"exists") )
            return platform;
        }
    }

    return NULL();
}


