#include "data/scripts/defines.h"
#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/lib_wall.c"

void main() {
    void self = getlocalvar("self");

    check_front_walkoff_from_edges(self,"ANI_FOLLOW85");
}

int check_front_walkoff_from_edges(void player, char ANIM_NAME) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float y = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float zdir = getentityproperty(player, "zdir");
    float T_MIN_ALTITUDE = 50;
    float T_DIST = 5, T_SIDE_DIST = 10;

    if ( zdir > 0 ) {
        int anim_id = getentityproperty(player, "animationid");

        if ( is_on_base(player) ) {
            if ( ready_to_attack(player,1,1) && anim_id != openborconstant(ANIM_NAME) ) {
                void platform = checkplatform(x,z+T_DIST,base+1);
                float wall_h = checkwall(x,z+T_DIST);
                float platform_h = get_platform_base(platform);
                int hole = checkhole(x,z+T_DIST);
                float max_t_base;
                float jfp_x = 0, jfp_z = 1.0, jfp_y = 3.5;

                if ( hole ) return 0;
                if ( platform_h > wall_h ) max_t_base = platform_h;
                else max_t_base = wall_h;

                if ( max_t_base < base ) {
                    if ( base-max_t_base >= T_MIN_ALTITUDE ) {
                        if ( !is_on_platform(player) && !is_on_wall(player) && gev(player,"stair") == NULL() ) return 0;

                        sev(player,"jfp_x",jfp_x);
                        sev(player,"jfp_z",jfp_z);
                        sev(player,"jfp_y",jfp_y);

                        // SIDE CONTROL
                        hole = checkhole(x+T_SIDE_DIST,z+T_DIST);
                        if ( hole ) return 0;
                        platform = checkplatform(x+T_SIDE_DIST,z+T_DIST,base+1);
                        wall_h = checkwall(x+T_SIDE_DIST,z+T_DIST);
                        platform_h = get_platform_base(platform);
                        if ( platform_h > wall_h ) max_t_base = platform_h;
                        else max_t_base = wall_h;
                        if ( max_t_base >= base ) return 0;

                        hole = checkhole(x-T_SIDE_DIST,z+T_DIST);
                        if ( hole ) return 0;
                        platform = checkplatform(x-T_SIDE_DIST,z+T_DIST,base+1);
                        wall_h = checkwall(x-T_SIDE_DIST,z+T_DIST);
                        platform_h = get_platform_base(platform);
                        if ( platform_h > wall_h ) max_t_base = platform_h;
                        else max_t_base = wall_h;
                        if ( max_t_base >= base ) return 0;
                        // SIDE CONTROL

                        performattack(player,openborconstant(ANIM_NAME),1);
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

int check_front_walkoff_from_edges_for_target(void player, char ANIM_NAME) {
    void target = findtarget(player);

    if ( getentityproperty(target,"exists") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float y = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        float tx = getentityproperty(target, "x");
        float tz = getentityproperty(target, "z");
        float ty = getentityproperty(target, "y");
        float tbase = getentityproperty(target, "base");
        float T_MIN_ALTITUDE = 15;
        float T_DIST = 5;

        if ( is_on_base(player) && base > tbase ) {
            if ( base-tbase >= T_MIN_ALTITUDE ) {
                int anim_id = getentityproperty(player, "animationid");

                if ( ready_to_attack(player,1,1) && anim_id != openborconstant(ANIM_NAME) ) {
                    void platform = checkplatform(x,z+T_DIST,base+1);
                    float wall_h = checkwall(x,z+T_DIST);
                    float platform_h = get_platform_base(platform);

                    if ( !is_on_platform(player) && !is_on_wall(player) && gev(player,"stair") == NULL() ) return 0;

                    if ( platform_h < base && wall_h < base ) {
                        performattack(player,openborconstant(ANIM_NAME),1);
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

