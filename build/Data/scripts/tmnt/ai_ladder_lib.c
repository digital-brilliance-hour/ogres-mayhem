
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"

#import "data/scripts/AI_ladder_commons.c"

#define THRESHOLD_LADDER_X openborvariant("hresolution")/1 // serching ladder range
#define THRESHOLD_LADDER_Z (openborvariant("scrollmaxz")-openborvariant("scrollminz"))/1
#define T_LADDER_RANGE 5
#define T_LADDER_PROXIMITY 30
#define T_LADDER_ALT_VISIBILITY get_ladder_alt_visibility()

// path_flag == 1 for up, 0 for down. ladder1 is linked with ladder2? yes returns 1, no returns 0
int is_linked_ladder(void ladder1, void ladder2, int path_flag) {
    int minz = ov("player_min_z");
    int maxz = ov("player_max_z");
    float ladder1_base = gep(ladder1,"base");
    float ladder1_a = gep(ladder1,"y");
    float ladder1_x = gep(ladder1,"x");
    float ladder1_z = gep(ladder1,"z");
    float ladder1_height = getentityvar(ladder1,"height");
    float ladder1_width = getentityvar(ladder1,"width");
    void ladder1_type = getentityvar(ladder1,"type");
    float side_ladder1_shift = 0, front_ladder1_shift = 0, climb_area1_width = 0;
    float ladder2_base = gep(ladder2,"base");
    float ladder2_a = gep(ladder2,"y");
    float ladder2_x = gep(ladder2,"x");
    float ladder2_z = gep(ladder2,"z");
    float ladder2_height = getentityvar(ladder2,"height");
    float ladder2_width = getentityvar(ladder2,"width");
    void ladder2_type = getentityvar(ladder2, "type");
    float side_ladder2_shift = 0, front_ladder2_shift = 0, climb_area2_width = 0;

    void platform_end, platform_begin;
    float wall_height, wall_base;
    int i = 0, t_inc = 1;

    if ( ladder1_width == NULL() ) ladder1_width = 0;
    if ( ladder2_width == NULL() ) ladder2_width = 0;

    if ( ladder1_type == "ladder_v" || ladder1_type == "ladder_h" || ladder1_type == "climbing_area" ) front_ladder1_shift = -10;
    else if ( ladder1_type == "ladder_l" ) side_ladder1_shift = 30;
    else if ( ladder1_type == "ladder_r" ) side_ladder1_shift = -10;

    if ( ladder2_type == "ladder_v" || ladder2_type == "ladder_h" || ladder2_type == "climbing_area" ) front_ladder2_shift = -10;
    else if ( ladder2_type == "ladder_l" ) side_ladder2_shift = 30;
    else if ( ladder2_type == "ladder_r" ) side_ladder2_shift = -10;

    if ( ladder1_type == "climbing_area" ) climb_area1_width = ladder1_width;
    else climb_area1_width = 0;
    if ( ladder2_type == "climbing_area" ) climb_area2_width = ladder2_width;
    else climb_area2_width = 0;

    if ( ladder1_type == "ladder_h" && ladder2_type == "ladder_h" ) {
        void plat1_left  = find_platform_in_a(side_ladder1_shift,ladder1_z+front_ladder1_shift,ladder1_base,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
        void plat1_right  = find_platform_in_a(ladder1_width+side_ladder1_shift,ladder1_z+front_ladder1_shift,ladder1_base,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
        void plat2_left = find_platform_in_a(side_ladder2_shift,ladder2_z+front_ladder2_shift,ladder2_base,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
        void plat2_right = find_platform_in_a(ladder2_width+side_ladder2_shift,ladder2_z+front_ladder2_shift,ladder2_base,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
        float wall1_left = checkwall(side_ladder1_shift,ladder1_z+front_ladder1_shift);
        float wall1_right = checkwall(ladder1_width+side_ladder1_shift,ladder1_z+front_ladder1_shift);
        float wall2_left = checkwall(side_ladder2_shift,ladder2_z+front_ladder2_shift);
        float wall2_right = checkwall(ladder2_width+side_ladder2_shift,ladder2_z+front_ladder2_shift);

        if ( plat1_right != NULL() && (plat1_right == plat2_left) ) return 1;
        if ( plat1_left != NULL() && (plat2_right == plat1_left) ) return 1;

        if ( wall1_right > 0 && (wall1_right == wall2_left) ) return 1;
        if ( (wall2_right == wall1_left) ) return 1; // no > 0 to compare ground 0 too

        return 0;
    }

    if ( path_flag ) { // UP PATH (target > self)
            for ( i = ladder1_x; i <= ladder1_x+climb_area1_width; i += t_inc ) { // to check climbable area too
                float platform_end_walkable_base = 0, platform_begin_walkable_base = 0;

                platform_end = find_platform_in_a(i+side_ladder1_shift,ladder1_z+front_ladder1_shift,ladder1_a+ladder1_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma alla fine della scala
                platform_begin = find_platform_in_a(i+side_ladder2_shift,ladder2_z+front_ladder2_shift,ladder2_a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma sulla base della scala
                wall_height = checkwall(i+side_ladder1_shift,ladder1_z+front_ladder1_shift);
                wall_base = checkwall(i+side_ladder2_shift,ladder2_z);

                if ( gep(platform_end, "exists") ) {
                    platform_end_walkable_base = gep(platform_end,"y")+gep(platform_end,"height");
                } else if ( wall_height > 0 ) {
                    platform_end_walkable_base = wall_height;
                }
                if ( gep(platform_begin, "exists") ) {
                    platform_begin_walkable_base = gep(platform_begin,"y")+gep(platform_begin,"height");
                } else if ( wall_base > 0 ) {
                    platform_begin_walkable_base = wall_base;
                }

                if ( platform_end_walkable_base == platform_begin_walkable_base ) return 1;
            } // fine for
    } else { // DOWN PATH (target <= self)
            for ( i = ladder2_x; i <= ladder2_x+climb_area2_width; i += t_inc ) { // to check climbable area too
                float platform_end_walkable_base = 0, platform_begin_walkable_base = 0;

                platform_end = find_platform_in_a(i+side_ladder2_shift,ladder2_z+front_ladder2_shift,ladder2_a+ladder2_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma alla fine della scala
                platform_begin = find_platform_in_a(i+side_ladder1_shift,ladder1_z+front_ladder1_shift,ladder1_a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma sulla base della scala
                wall_height = checkwall(i+side_ladder2_shift,ladder2_z+front_ladder2_shift);
                wall_base = checkwall(i+side_ladder1_shift,ladder1_z);

                if ( gep(platform_end, "exists") ) {
                    platform_end_walkable_base = gep(platform_end,"y")+gep(platform_end,"height");
                } else if ( wall_height > 0 ) {
                    platform_end_walkable_base = wall_height;
                }
                if ( gep(platform_begin, "exists") ) {
                    platform_begin_walkable_base = gep(platform_begin,"y")+gep(platform_begin,"height");
                } else if ( wall_base > 0 ) {
                    platform_begin_walkable_base = wall_base;
                }

                if ( platform_end_walkable_base == platform_begin_walkable_base ) return 1;
            } // fine for
    }

    return 0;
}

// cerca scale che possono portare al target e restituisce 0 se non è candidata, 1 se è candidata (la sua y è tra self e target),
// 2 se ha un collegamento diretto con il target, 3 se ha collegamento diretto con self, 4 se collega direttamente self a target
int is_candidate_ladder(void self, void target, void ladder) {
    if ( is_incam(ladder) ) {
        int minz = ov("player_min_z");
        int maxz = ov("player_max_z");
        float target_base = gep(target,"base");
        float target_x = gep(target,"x");
        float target_z = gep(target,"z");
        float self_base = gep(self,"base");
        float self_x = gep(self,"x");
        float self_z = gep(self,"z");
        float ladder_base = gep(ladder,"base");
        float ladder_a = gep(ladder,"y");
        float ladder_x = gep(ladder,"x");
        float ladder_z = gep(ladder,"z");
        float ladder_height = getentityvar(ladder,"height");
        void ladder_type = getentityvar(ladder, "type");
        //void target_platform = get_platform_on(target);
        void platform_end, platform_begin;
        float wall_height, wall_base;
        float ladder_width = getentityvar(ladder,"width"), climb_area_width = 0;
        //void platform_on = get_platform_on(self);
        //float wall_on = checkwall(self_x,self_z);
        float side_ladder_shift = 0, front_ladder_shift = 0;
        int i = 0, t_inc = 1;

        if ( ladder_width == NULL() ) ladder_width = 0;

        if ( ladder_type == "ladder_v" || ladder_type == "ladder_h" || ladder_type == "climbing_area" ) front_ladder_shift = -10;
        else if ( ladder_type == "ladder_l" ) side_ladder_shift = 30;
        else if ( ladder_type == "ladder_r" ) side_ladder_shift = -10;

        if ( ladder_type == "climbing_area" ) climb_area_width = ladder_width;
        else climb_area_width = 0;

        if ( ladder_type == "ladder_h" ) {
            if ( (ladder_x <= target_x && ladder_x >= self_x) || (ladder_x <= self_x && ladder_x >= target_x) ) {
                int res = 0, target_app = 0, self_app = 0;

                //left
                if ( is_proximity(self_x,self_z,ladder_x,ladder_z+front_ladder_shift,T_LADDER_PROXIMITY) ) { ++res; self_app = 1; }
                if ( is_proximity(target_x,target_z,ladder_x,ladder_z+front_ladder_shift,T_LADDER_PROXIMITY) ) { ++res; target_app = 1; }
                //right
                if ( is_proximity(self_x,self_z,ladder_x+ladder_width,ladder_z+front_ladder_shift,T_LADDER_PROXIMITY) ) { ++res; self_app = 1; }
                if ( is_proximity(target_x,target_z,ladder_x+ladder_width,ladder_z+front_ladder_shift,T_LADDER_PROXIMITY) ) { ++res; target_app = 1; }

                     if ( self_app == 1 && target_app == 1 ) return 4;
                else if ( self_app == 1 && target_app == 0 ) return 3;
                else if ( self_app == 0 && target_app == 1 ) return 2;
                else return 1;
            } else return 0;
        }

        if ( target_base > self_base ) {
            for ( i = ladder_x; i <= ladder_x+climb_area_width; i += t_inc ) { // to check climbable area too
                platform_end = find_platform_in_a(i+side_ladder_shift,ladder_z+front_ladder_shift,ladder_a+ladder_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma alla fine della scala
                platform_begin = find_platform_in_a(i+side_ladder_shift,ladder_z+front_ladder_shift,ladder_a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma sulla base della scala
                wall_height = checkwall(i+side_ladder_shift,ladder_z+front_ladder_shift);
                wall_base = checkwall(i+side_ladder_shift,ladder_z);

                if ( ladder_a <= 0 && self_base <= 0 ) {
                    if ( gep(platform_end, "exists") ) {
                        float platform_end_walkable_base = gep(platform_end,"y")+gep(platform_end,"height");

                        if ( platform_end_walkable_base == target_base && 0 == self_base ) return 4;
                        else if ( platform_end_walkable_base == target_base ) return 2;
                    } else {
                        if ( wall_height == target_base && 0 == self_base ) return 4;
                        else if ( wall_height == target_base ) return 2;
                    }
                    if ( 0 == self_base ) return 3;
                    else return 1;
                }

                if ( gep(platform_end, "exists") ) {
                    float platform_a =  gep(platform_end,"y");
                    float platform_height =  gep(platform_end,"height");
                    float platform_walkable_base = platform_a+platform_height;

                    if ( platform_walkable_base > self_base && platform_walkable_base <= target_base ) {
                        if ( gep(platform_begin, "exists") ) {
                            float platform_begin_walkable_base = gep(platform_begin,"y")+gep(platform_begin,"height");

                            if ( platform_walkable_base == target_base && platform_begin_walkable_base == self_base ) return 4;
                            else if ( platform_begin_walkable_base == self_base ) return 3;
                        } else if ( wall_base > 0 ) {
                            if ( platform_walkable_base == target_base && wall_base == self_base ) return 4;
                            else if ( wall_base == self_base ) return 3;
                        }
                        if ( platform_walkable_base == target_base ) return 2;
                        else return 1;
                    } else continue;
                }

                if ( wall_height > self_base && wall_height <= target_base ) { // funziona anche per la base 0
                    if ( gep(platform_begin, "exists") ) {
                        float platform_begin_walkable_base = gep(platform_begin,"y")+gep(platform_begin,"height");

                        if ( wall_height == target_base && platform_begin_walkable_base == self_base ) return 4;
                        else if ( platform_begin_walkable_base == self_base ) return 3;
                    }
                    if ( wall_height == target_base ) return 2;
                    else if ( wall_base == self_base ) return 3;
                    else if ( wall_height == target_base && wall_base == self_base ) return 4;
                    else return 1;
                } else continue;
            } // fine for
        } else if ( target_base < self_base ) {
            for ( i = ladder_x; i <= ladder_x+climb_area_width; i += t_inc ) { // to check climbable area too
                platform_end = find_platform_in_a(i+side_ladder_shift,ladder_z+front_ladder_shift,ladder_a+ladder_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma alla fine della scala
                platform_begin = find_platform_in_a(i+side_ladder_shift,ladder_z+front_ladder_shift,ladder_a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma sulla base della scala
                wall_height = checkwall(i+side_ladder_shift,ladder_z+front_ladder_shift);
                wall_base = checkwall(i+side_ladder_shift,ladder_z);

                //drawstring(10,180,0,"TEST");
                if ( ladder_a <= 0 && target_base <= 0 ) {
                    if ( gep(platform_end, "exists") ) {
                        float platform_end_walkable_base = gep(platform_end,"y")+gep(platform_end,"height");

                        if ( platform_end_walkable_base == self_base && 0 == target_base ) return 4;
                        else if ( platform_end_walkable_base == self_base ) return 3;
                    } else {
                        if ( wall_height == self_base && 0 == target_base ) return 4;
                        else if ( wall_height == self_base ) return 3;
                    }
                    if ( 0 == target_base ) return 2;
                    else return 1;
                }

                if ( gep(platform_begin, "exists") ) {
                    float platform_a =  gep(platform_begin,"y");
                    float platform_height =  gep(platform_begin,"height");
                    float platform_walkable_base = platform_a+platform_height;

                    if ( platform_walkable_base < self_base && platform_walkable_base >= target_base ) {
                        if ( gep(platform_end, "exists") ) {
                            float platform_end_walkable_base = gep(platform_end,"y")+gep(platform_end,"height");

                            if ( platform_end_walkable_base == self_base && platform_walkable_base == target_base ) return 4;
                            else if ( platform_end_walkable_base == self_base ) return 3;
                        } else if ( wall_height > 0 ) {
                            if ( platform_walkable_base == target_base && wall_height == self_base ) return 4;
                            else if ( wall_height == self_base ) return 3;
                        }
                        if ( platform_walkable_base == target_base ) return 2;
                        else return 1;
                    } else continue;
                }

                if ( wall_height < self_base && wall_height >= target_base ) {
                    if ( gep(platform_end, "exists") ) {
                        float platform_end_walkable_base = gep(platform_end,"y")+gep(platform_end,"height");

                        if ( platform_end_walkable_base == self_base && wall_height == target_base ) return 4;
                        else if ( platform_end_walkable_base == self_base ) return 3;
                    }
                    if ( wall_height == target_base ) return 2;
                    else if ( wall_base == self_base ) return 3;
                    else if ( wall_height == target_base && wall_base == self_base ) return 4;
                    else return 1;
                } else continue;
            } // fine for
        } else { // target_base == self_base
            return 0;
        } // fine if target base comparison
    }

    return 0;
}
