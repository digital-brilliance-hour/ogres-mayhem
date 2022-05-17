
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/AI_ladder_commons.c"

#define THRESHOLD_LADDER_X openborvariant("hresolution")/1
#define THRESHOLD_LADDER_Z (openborvariant("scrollmaxz")-openborvariant("scrollminz"))/1
#define T_LADDER_RANGE 5
#define T_TARGET_RANGE 1
#define T_TARGET_PROXIMITY 10
#define T_TARGET_BASE_RANGE 5
#define T_LADDER_ALT_VISIBILITY get_ladder_alt_visibility()
#define SCAN_FACTOR_PIXELS 5

// DA FARE:
//

void main() {
  //void self = getlocalvar("self");

}

int generate_action_for_ladder_h(void player) {
    int action_flag = 0;
    void target = findtarget(player);

    if ( gep(target,"exists") ) {
        float target_base = 0, target_x = 0, target_z = 0, target_a = 0;
        float base = gep(player,"base");
        float x = gep(player,"x");
        float z = gep(player,"z");
        float a = gep(player,"y");
        float height = gep(player,"height");
        float threshold = 5;
        void platform;
        void ladder = getentityvar(player,"ladder");
        float t_range = 2;

        if ( height == NULL() ) height = T_ENT_HEIGHT; // protection

        target_base = gep(target,"base");
        target_x = gep(target,"x");
        target_z = gep(target,"z");
        target_a = gep(target,"y");

        // SINGLE DECISION
        if ( get_action("moveleft") || get_action("moveright") ) {
            action_flag = getlocalvar("action_flag");
            return action_flag;
        }

        /*if ( x <= target_x+t_range && x >= target_x-t_range ) {
            action_flag = set_action("jump");
            action_flag = getlocalvar("action_flag");
            return action_flag;
        }*/

        if ( target_x > x-t_range ) action_flag = set_action("moveright");
        else if ( target_x < x+t_range ) action_flag = set_action("moveleft");
        else action_flag = set_action("jump");

        if ( target_base == base && gev(player,"ladder") == NULL() ) action_flag = set_action("jump"); // protection
    } else {
        action_flag = set_action("jump");
    }

    action_flag = getlocalvar("action_flag");
    return action_flag;
}

int generate_action_for_ladder(void player) {
    int action_flag = 0;
    void target = findtarget(player);

    if ( gep(target,"exists") ) {
        float target_base = 0, target_x = 0, target_z = 0, target_a = 0;
        float base = gep(player,"base");
        float x = gep(player,"x");
        float z = gep(player,"z");
        float a = gep(player,"y");
        float height = gep(player,"height");
        float threshold = 5;
        void platform;
        void ladder = getentityvar(player,"ladder");
        float t_range = 2;

        if ( height == NULL() ) height = T_ENT_HEIGHT; // protection

        target_base = gep(target,"base");
        target_x = gep(target,"x");
        target_z = gep(target,"z");
        target_a = gep(target,"y");

        // SINGLE DECISION
        if ( get_action("moveup") || get_action("movedown") ) {
            action_flag = getlocalvar("action_flag");
            return action_flag;
        }

        if ( a <= target_a+t_range && a >= target_a-t_range ) {
            action_flag = set_action("jump");
            action_flag = getlocalvar("action_flag");
            return action_flag;
        }

        if ( target_a > a-t_range ) action_flag = set_action("moveup");
        else if ( target_a < a+t_range ) action_flag = set_action("movedown");
        else action_flag = set_action("jump");

        if ( target_base == base && gev(player,"ladder") == NULL() ) action_flag = set_action("jump"); // protection

        platform = find_platform_in_a(x,z,a+height+threshold,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
        if ( gep(platform,"exists") && gep(ladder,"exists") ) {
            float ladder_a = gep(ladder,"y");
            float climb_area_height = getentityvar(ladder,"height");

            if ( ladder_a+climb_area_height < gep(platform,"y")+gep(platform,"height") ) {
                action_flag = set_action("jump");
            }
        }

    } else {
        action_flag = set_action("jump");
    }

    action_flag = getlocalvar("action_flag");
    return action_flag;
}

// retrieve action for set_action()
/*
   - decidi direzione: soglia e half_dist
   - non tornare da dove stai venendo se non come ultima possibilità (flagga ultima direzione)
   - cerca appigli affianco ed in alto (lungo tutta la climb area), poi anche in basso (lungo la CA) CRITERIO: cerca l'appiglio più vcino
   - mentre climbi cerca ostacoli. se li trovi  ricalcola coord target e flagga vecchia direzione (quella con ostacolo)
   - se hai raggiunto il target, non seguirlo più (reset target)
*/
// BUGS:
int generate_action_for_climbable_area(void player) {
    int action_flag = 0;
    void ladder = getentityvar(player,"ladder");
    float target_base = 0, target_x = 0, target_z = 0, target_y = 0;
    int stop_flag = 0, has_climbed = 0, coords_response = 0, obstacle_flag = 0;
    //int i;

    /*drawstring(10,100,0,"x: "+gep(player,"x"));
    drawstring(10,110,0,"y: "+gep(player,"y"));
    drawstring(10,120,0,"action flag: "+getlocalvar("action_flag"));
    drawstring(10,130,0,"ladder: "+getentityvar(player,"ladder"));
    drawstring(10,140,0,"target_x: "+glv("target_x_climb"));
    drawstring(10,150,0,"target_y: "+glv("target_y_climb"));
    drawstring(10,160,0,"climb_no_target: "+glv("climb_no_target_flag"));
    drawstring(10,170,0,"climb_obstacle_solved: "+glv("climb_obstacle_solved"));*/

    obstacle_flag = check_climb_obstacles(player,10,10,20,"target_x_climb","target_y_climb");
    if ( !obstacle_flag && glv("climb_obstacle_solved") != NULL() ) {
        int eta = time_to_wait("climb_time_to_wait",ov("game_speed")*0.5);

        if ( eta ) {
            reset_time_to_wait("climb_time_to_wait");
            slv("climb_obstacle_solved",NULL());
        }
    }

    coords_response = get_target_coords(player,"target_x_climb","target_y_climb");
    if ( !coords_response ) {
        action_flag = getlocalvar("action_flag");
        return action_flag;
    }
    if ( glv("target_x_climb") == NULL() && glv("target_y_climb") == NULL() ) {
        set_action("start");
    }

    has_climbed = climb_to(player,glv("target_x_climb"),glv("target_y_climb"),stop_flag);
    if ( has_climbed && stop_flag ) {
        slv("target_x_climb",NULL());
        slv("target_y_climb",NULL());
        slv("climb_target_flag",NULL());
    }

    //climb_to(player,142+100,40,0);
    //climb_to(player,150,90,0);

    action_flag = getlocalvar("action_flag");
    return action_flag;
}

int check_climb_obstacles(void player, float t_border_obstacle, float t_dist, float t_y_adjust, char target_x_label, char target_y_label) { // t_border_obstacle = 5;
    void ladder = getentityvar(player,"ladder");

    if ( gep(ladder,"exists") && glv("climb_obstacle") == NULL() ) {
        float base = gep(player,"base");
        float x = gep(player,"x");
        float z = gep(player,"z");
        float a = gep(player,"y");
        float height = gep(player,"height");
        float target_x = glv(target_x_label);
        float target_y = glv(target_y_label);
        float ladder_base = gep(ladder,"base");
        float ladder_x = gep(ladder,"x");
        float ladder_z = gep(ladder,"z");
        float ladder_a = gep(ladder,"y");
        float climb_area_width = getentityvar(ladder,"width");
        float climb_area_height = getentityvar(ladder,"height");
        int hdir = glv("xdir_climb");
        int vdir = glv("ydir_climb");
        void platform;
        float wall_height;
        int hole, hole_target;
        //float t_dist = 10;
        int i = 0;

        if ( height == NULL() ) height = T_ENT_HEIGHT; // protection
        if ( hdir == NULL() ) hdir = 0;
        if ( vdir == NULL() ) vdir = 0;
        if ( target_x == NULL() ) target_x = -1;
        if ( target_y == NULL() ) target_y = -1;

        if ( hdir == 0 && vdir == 0 ) return 0;

        platform = find_platform_in_a(x+(t_dist*hdir),ladder_z+t_border_obstacle,a+(t_dist*vdir),MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
        wall_height = checkwall(x+(t_dist*vdir),ladder_z+t_border_obstacle);
        hole = checkhole(x+(t_dist*hdir),ladder_z);
        hole_target = checkhole(target_x,ladder_z);

        if ( platform != NULL() && gep(platform,"exists") ) {
            slv("climb_obstacle",1); // non azzerare le coords con reset_climb_to() (altrimenti il player si stacca)
            slv("climb_no_target_flag",1);

            return 1;
        }
        if ( wall_height > a+(t_dist*vdir) ) {
            slv("climb_obstacle",1);
            slv("climb_no_target_flag",1);

            return 1;
        }
        if ( hole && a <= ladder_a+t_dist ) {
            slv("climb_obstacle",1);
            slv("climb_no_target_flag",1);

            return 1;
        }
        if ( target_x > -1 && target_y > -1 ) {
            if ( hole_target && target_y <= ladder_a+t_y_adjust ) {
                slv("climb_obstacle",1);
                slv("climb_no_target_flag",1);

                return 1;
            }
        }
    }

    return 0;
}

float get_middle_x_coord(void ladder, void platform, float wall_h) {
    float middle_x = -1;

    if ( gep(ladder,"exists") ) {
        float ladder_base = gep(ladder,"base");
        float ladder_x = gep(ladder,"x");
        float ladder_z = gep(ladder,"z");
        float ladder_a = gep(ladder,"y");
        float climb_area_width = getentityvar(ladder,"width");
        float climb_area_height = getentityvar(ladder,"height");
        int i = 0;

        if ( gep(platform,"exists") ) {
            void new_platform;
            float platform_h = gep(platform,"height");
            float platform_y = gep(platform,"y");
            float platform_x = gep(platform,"x");
            float platform_z = gep(platform,"z");
            float platform_hy;
            float x1 = 0, x2 = 0;

            if ( platform_h == NULL() ) platform_h = 0;
            platform_hy = platform_y+platform_h;

            for (i = platform_x; i <= ladder_x+climb_area_width && i >= ladder_x; ++i ) {
                new_platform = find_platform_in_a(i,platform_z,platform_hy,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                if ( new_platform != platform ) break;
                x2 = i;
            }
            for (i = platform_x; i <= ladder_x+climb_area_width && i >= ladder_x; --i ) {
                new_platform = find_platform_in_a(i,platform_z,platform_hy,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                if ( new_platform != platform ) break;
                x1 = i;
            }

            if ( x1 != 0 && x2 != 0 ) {
                middle_x = (x1+x2)/2;
            }

            return middle_x;
        } else if ( wall_h > 0 ) {
            float new_wall_h;
            float front_ladder_shift = -10;
            float x1 = 0, x2 = 0;

            for (i = ladder_x; i <= ladder_x+climb_area_width && i >= ladder_x; ++i ) {
                new_wall_h = checkwall(i,ladder_z+front_ladder_shift);
                if ( wall_h == new_wall_h ) {
                    x1 = i;
                    break;
                }
            }
            for (i = ladder_x+climb_area_width; i <= ladder_x+climb_area_width && i >= ladder_x; --i ) {
                new_wall_h = checkwall(i,ladder_z+front_ladder_shift);
                if ( wall_h == new_wall_h ) {
                    x2 = i;
                    break;
                }
            }

            if ( x1 != 0 && x2 != 0 ) {
                middle_x = (x1+x2)/2;
            }
        }
    }

    return middle_x;
}

int get_target_coords(void player, char target_x_label, char target_y_label) {
    void target = findtarget(player);
    float target_x = -1, target_y = -1;
    float x_dist = glv("x_dist_climb"), y_dist = glv("y_dist_climb");

    if ( gep(target,"exists") && glv("climb_obstacle") == NULL() && glv("climb_no_target_flag") == NULL() ) {
        float target_z = gep(target,"z");
        float target_tossv = gep(target,"tossv");
        float target_is_jumping = gep(target,"aiflag","jumping");
        void ladder = getentityvar(player,"ladder");
        float t_dist = 2;

        if ( target_tossv != 0 && !target_is_jumping ) {
            slv(target_x_label,NULL());
            slv(target_y_label,NULL());
            slv("climb_target_flag",NULL());

            return 0;
        }
        if ( is_in_pain(target) ) {
            slv(target_x_label,NULL());
            slv(target_y_label,NULL());
            slv("climb_target_flag",NULL());

            return 0;
        }
        if ( x_dist != NULL() && y_dist != NULL() ) { // raggiunto il target? smettila di seguirlo
            if ( x_dist < t_dist && y_dist < t_dist ) {
                slv("climb_no_target_flag",1);
                return 0;
            }
        }

        target_x = gep(target,"x");
        target_y = gep(target,"y");

        if ( is_on_platform(target) ) {
            void ladder = getentityvar(player,"ladder");
            void platform = get_platform_on(target);

            target_x = get_middle_x_coord(ladder,platform,0);
            if ( target_x == -1 ) target_x = gep(target,"x");
        } else if ( is_on_wall(target) ) {
            void ladder = getentityvar(player,"ladder");
            float wall_h;

            wall_h = checkwall(target_x,target_z);
            target_x = get_middle_x_coord(ladder,NULL(),wall_h);
            if ( target_x == -1 ) target_x = gep(target,"x");
        }


        if ( ladder != NULL() && gep(ladder,"exists") ) {
            float ladder_base = gep(ladder,"base");
            float ladder_x = gep(ladder,"x");
            float ladder_z = gep(ladder,"z");
            float ladder_a = gep(ladder,"y");
            float climb_area_width = getentityvar(ladder,"width");
            float climb_area_height = getentityvar(ladder,"height");
            float t_adjust = 10, t_hole = 5; // t_hole < t_adjust

            if ( checkhole(target_x,ladder_z) ) {
                if ( target_y <= ladder_a+t_hole ) {
                    target_y = ladder_a+t_adjust;
                }
            }
        }

        if ( target_x > -1 && target_y > -1 ) {
            slv(target_x_label,target_x);
            slv(target_y_label,target_y);

            slv("climb_target_flag",1);

            return 1;
        }

        return 0;
    } else { // se il target non è un nemico
        void ladder = getentityvar(player,"ladder");

        // Stavi seguendo il target ed ora non puoi più? Allora svuota le coords e cerca un appiglio
        if ( glv(target_x_label) != NULL() && glv(target_y_label) != NULL() && glv("climb_target_flag") != NULL() ) {
            slv(target_x_label,NULL());
            slv(target_y_label,NULL());
            slv("climb_target_flag",NULL());
        }

        // -------------- SEARCH PROTRUSION --------------
        if ( ladder != NULL() && gep(ladder,"exists") && ((glv(target_x_label) == NULL() && glv(target_y_label) == NULL()) || glv("climb_obstacle") != NULL()) ) {
            float base = gep(player,"base");
            float x = gep(player,"x");
            float z = gep(player,"z");
            float a = gep(player,"y");
            float height = gep(player,"height");
            float ladder_base = gep(ladder,"base");
            float ladder_x = gep(ladder,"x");
            float ladder_z = gep(ladder,"z");
            float ladder_a = gep(ladder,"y");
            float climb_area_width = getentityvar(ladder,"width");
            float climb_area_height = getentityvar(ladder,"height");
            char ladder_type = getentityvar(ladder,"type");
            float side_ladder_shift = 0, front_ladder_shift = 0;
            void platform, old_platform, platform_obstacle;
            float wall_height, old_wall_height, wall_obstacle;
            int hole;
            float t_search_protrusion = 10, t_y_adjust = 20+5, t_border_obstacle = 10; // t_y_adjust: value to climb over the platform
            int i = 0; // ir = pixel_search_precision

            if ( height == NULL() ) height = T_ENT_HEIGHT; // protection

            if ( ladder_type == "ladder_v" || ladder_type == "ladder_h" || ladder_type == "climbing_area" ) front_ladder_shift = -10;
            else if ( ladder_type == "ladder_l" ) side_ladder_shift = 30;
            else if ( ladder_type == "ladder_r" ) side_ladder_shift = -10;

            if ( climb_area_width == NULL() ) climb_area_width = 0;
            if ( climb_area_height == NULL() ) climb_area_height = 0;

            if ( x_dist == NULL() && y_dist == NULL() && glv("climb_obstacle") == NULL() ) { // appeso per la prima volta (appena attaccato pronto per climbare) - CRITERIO: cerca appiglio nella metà opposta dell'area
                int area_width_dx = 0, area_height_up = 0;
                float ir = SCAN_FACTOR_PIXELS, ix = 0, iy = 0, ih = 1, iv = 1;
                int hdir = 0, vdir = 0;
                float h_middle = 0, v_middle = 0;
                float max_side = 0;

                // stabilisci il quadrante dell'area in cui si trova il pg
                h_middle = ladder_x+side_ladder_shift + climb_area_width/2;
                v_middle = ladder_a + climb_area_height/2;
                // vai a sx (-1) se sta a dx rispetto al centro
                if ( x >= h_middle ) hdir = -1;
                else hdir = 1;
                if ( a >= v_middle ) vdir = -1;
                else vdir = 1;

                // cerca dal quadrante OPPOSTO (ricerca per quadrante usare 1 solo for)
                if ( climb_area_width >= climb_area_height ) max_side = climb_area_width;
                else max_side = climb_area_height;

                // parte opposta
                if ( hdir == 1 ) area_width_dx = climb_area_width;
                if ( vdir == 1 ) area_height_up = climb_area_height;
                ih = ir*hdir*-1;
                iv = ir*vdir*-1;

                ////////////////// SEARCH COORDS FOR //////////////////
                for ( i = 0; i <= max_side; i += ir ) {
                    for ( ix = ladder_x+side_ladder_shift+area_width_dx; ix <= ladder_x+side_ladder_shift+climb_area_width && ix >= ladder_x+side_ladder_shift; ix += ih ) {
                        float h = 0;

                        if ( i > climb_area_height ) break;
                        if ( ih < 0 && ix < ladder_x+side_ladder_shift+climb_area_width-i ) break;
                        if ( ih > 0 && ix > ladder_x+side_ladder_shift+i ) break;

                        if ( iv < 0 ) h = ladder_a+climb_area_height-i;
                        else h = ladder_a+i;

                        hole = checkhole(ix,ladder_z);
                        if ( hole && h <= ladder_a+t_y_adjust ) {
                            //h = ladder_a+t_y_adjust+1;
                            continue;
                        }
                        platform = find_platform_in_a(ix,ladder_z+front_ladder_shift,h+t_search_protrusion,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                        wall_height = checkwall(ix,ladder_z+front_ladder_shift);

                        // ignore obstacles
                        platform_obstacle = find_platform_in_a(ix,ladder_z+t_border_obstacle,h+t_search_protrusion,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                        wall_obstacle = checkwall(ix,ladder_z+t_border_obstacle);
                        if ( platform == platform_obstacle ) platform = NULL();
                        if ( wall_height == wall_obstacle ) wall_height = 0;

                        ////////////////// SET COORDS //////////////////
                        // cerca wall, platform e a == 0. se è 0 attenzione agli holes
                        if ( platform != NULL() && gep(platform,"exists") ) { // platform
                            float platform_h = gep(platform,"height");
                            float platform_y = gep(platform,"y");

                            if ( platform_h == NULL() ) platform_h = 0;

                            if ( ((platform != old_platform && !old_platform) || old_wall_height > 0) && target_x > -1 && target_y > -1 ) {
                                slv(target_x_label,target_x);
                                slv(target_y_label,target_y);

                                return 1;
                            }
                            if ( vdir == 1 && ladder_a+climb_area_height <= platform_y+platform_h && ladder_a+climb_area_height > platform_y ) {
                                if ( platform == old_platform && target_x > -1 && target_y > -1 ) {
                                    target_x = get_middle_x_coord(ladder,platform,0);
                                    if ( target_x == -1 ) target_x = truncA((target_x+ix)/2);
                                    target_y = platform_y+platform_h;
                                } else {
                                    target_x = get_middle_x_coord(ladder,platform,0);
                                    if ( target_x == -1 ) target_x = ix;
                                    target_y = platform_y+platform_h;
                                }
                            } else if ( vdir == -1 && ladder_a == platform_y+platform_h ) {
                                if ( platform == old_platform && target_x > -1 && target_y > -1 ) {
                                    target_x = get_middle_x_coord(ladder,platform,0);
                                    if ( target_x == -1 ) target_x = truncA((target_x+ix)/2);
                                    target_y = platform_y+platform_h;
                                } else {
                                    target_x = get_middle_x_coord(ladder,platform,0);
                                    if ( target_x == -1 ) target_x = ix;
                                    target_y = platform_y+platform_h;
                                }
                            }

                            old_platform = platform;
                        } else {
                            if ( wall_height > 0 ) { // wall
                                if ( ((wall_height != old_wall_height && old_wall_height > 0) || old_platform != NULL()) && target_x > -1 && target_y > -1 ) {
                                    slv(target_x_label,target_x);
                                    slv(target_y_label,target_y);

                                    return 1;
                                }
                                if ( vdir == 1 && ladder_a+climb_area_height <= wall_height+t_search_protrusion && ladder_a+climb_area_height >= wall_height-t_search_protrusion ) {
                                    if ( wall_height == old_wall_height && target_x > -1 && target_y > -1 ) {
                                        target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        if ( target_x == -1 ) target_x = truncA((target_x+ix)/2);
                                        target_y = wall_height;
                                    } else {
                                        target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        if ( target_x == -1 ) target_x = ix;
                                        target_y = wall_height;
                                    }
                                } else if ( vdir == -1 && ladder_a == wall_height ) {
                                    if ( wall_height == old_wall_height && target_x > -1 && target_y > -1 ) {
                                        target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        if ( target_x == -1 ) target_x = truncA((target_x+ix)/2);
                                        target_y = wall_height;
                                    } else {
                                        target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        if ( target_x == -1 ) target_x = ix;
                                        target_y = wall_height;
                                    }
                                }

                                old_wall_height = wall_height;
                            } else { // base
                                if ( h <= 0 && vdir == -1 && ladder_a <= 0 ) {
                                    slv(target_x_label,ix);
                                    slv(target_y_label,h);

                                    return 1;
                                }
                            }
                        }
                        ////////////////// END SET COORDS //////////////////
                    }

                    for ( iy = ladder_a+area_height_up; iy <= ladder_a+climb_area_height && iy >= ladder_a; iy += iv ) {
                        float w = 0;

                        if ( i > climb_area_width ) break;
                        if ( iv < 0 && iy < ladder_a+climb_area_height-i ) break;
                        if ( iv > 0 && iy > ladder_a+i ) break;

                        if ( ih < 0 ) w = ladder_x+side_ladder_shift+climb_area_width-i;
                        else w = ladder_x+side_ladder_shift+i;

                        hole = checkhole(w,ladder_z);
                        if ( hole && iy <= ladder_a+t_y_adjust ) {
                            //iy = ladder_a+t_y_adjust+1;
                            continue;
                        }
                        platform = find_platform_in_a(w,ladder_z+front_ladder_shift,iy+t_search_protrusion,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                        wall_height = checkwall(w,ladder_z+front_ladder_shift);

                        // ignore obstacles
                        platform_obstacle = find_platform_in_a(w,ladder_z+t_border_obstacle,iy+t_search_protrusion,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                        wall_obstacle = checkwall(w,ladder_z+t_border_obstacle);
                        if ( platform == platform_obstacle ) platform = NULL();
                        if ( wall_height == wall_obstacle ) wall_height = 0;

                        ////////////////// SET COORDS //////////////////
                        // cerca wall, platform e a == 0. se è 0 attenzione agli holes
                        if ( platform != NULL() && gep(platform,"exists") ) { // platform
                            float platform_h = gep(platform,"height");
                            float platform_y = gep(platform,"y");

                            if ( platform_h == NULL() ) platform_h = 0;

                            if ( ((platform != old_platform && !old_platform) || old_wall_height > 0) && target_x > -1 && target_y > -1 ) {
                                slv(target_x_label,target_x);
                                slv(target_y_label,target_y);

                                return 1;
                            }
                            if ( hdir == 1 && ladder_a+climb_area_height <= platform_y+platform_h && ladder_a+climb_area_height > platform_y ) {
                                if ( platform == old_platform && target_x > -1 && target_y > -1 ) {
                                    target_x = get_middle_x_coord(ladder,platform,0);
                                    if ( target_x == -1 ) target_x = truncA((target_x+w)/2);
                                    target_y = platform_y+platform_h;
                                } else {
                                    target_x = get_middle_x_coord(ladder,platform,0);
                                    if ( target_x == -1 ) target_x = w;
                                    target_y = platform_y+platform_h;
                                }
                            } else if ( hdir == -1 && ladder_a == platform_y+platform_h ) {
                                if ( platform == old_platform && target_x > -1 && target_y > -1 ) {
                                    target_x = get_middle_x_coord(ladder,platform,0);
                                    if ( target_x == -1 ) target_x = truncA((target_x+w)/2);
                                    target_y = platform_y+platform_h;
                                } else {
                                    target_x = get_middle_x_coord(ladder,platform,0);
                                    if ( target_x == -1 ) target_x = w;
                                    target_y = platform_y+platform_h;
                                }
                            }
                        } else {
                            if ( wall_height > 0 ) { // wall
                                if ( ((wall_height != old_wall_height && old_wall_height > 0) || old_platform != NULL()) && target_x > -1 && target_y > -1 ) {
                                    slv(target_x_label,target_x);
                                    slv(target_y_label,target_y);

                                    return 1;
                                }
                                if ( vdir == 1 && ladder_a+climb_area_height <= wall_height+t_search_protrusion && ladder_a+climb_area_height >= wall_height-t_search_protrusion ) {
                                    if ( wall_height == old_wall_height && target_x > -1 && target_y > -1 ) {
                                        target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        if ( target_x == -1 ) target_x = truncA((target_x+w)/2);
                                        target_y = wall_height;
                                    } else {
                                        target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        if ( target_x == -1 ) target_x = w;
                                        target_y = wall_height;
                                    }
                                } else if ( vdir == -1 && ladder_a == wall_height ) {
                                    if ( wall_height == old_wall_height && target_x > -1 && target_y > -1 ) {
                                        target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        if ( target_x == -1 ) target_x = truncA((target_x+w)/2);
                                        target_y = wall_height;
                                    } else {
                                        target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        if ( target_x == -1 ) target_x = w;
                                        target_y = wall_height;
                                    }
                                }
                            } else { // base
                                if ( iy <= 0 && vdir == -1 && ladder_a <= 0 ) {
                                    slv(target_x_label,w);
                                    slv(target_y_label,iy);

                                    return 1;
                                }
                            }
                        }
                        ////////////////// END SET COORDS //////////////////
                    }
                }
                if ( target_x > -1 && target_y > -1 ) {
                    slv(target_x_label,target_x);
                    slv(target_y_label,target_y);

                    return 1;
                } else {
                    set_action("start");
                    return 0;
                }
                ////////////////// END SEARCH COORDS FOR //////////////////
            } else if (   (x_dist == NULL() && y_dist == 0)
                       || (x_dist == 0 && y_dist == NULL())
                       || (x_dist == 0 && y_dist == 0)
                       ||  (glv("climb_obstacle") != NULL() && glv("climb_obstacle_solved") == NULL() ) ) { // fermato - CRITERIO: cerca appiglio più vicino
                float ir = SCAN_FACTOR_PIXELS, ix = 0, iy = 0;
                float distAB = -1, tmp_distAB = -1;
                float min_target_x = -1, min_target_y = -1;

                if ( glv("climb_target_flag") != NULL() ) {
                    slv("climb_target_flag",NULL());
                }

                if ( glv("climb_obstacle") != NULL() ) {
                    slv("climb_obstacle",NULL());
                    slv("climb_obstacle_solved",1);
                    reset_climb_to();
                }

                // cerca doppio for (usare distAB())
                for ( ix = ladder_x+side_ladder_shift; ix <= ladder_x+side_ladder_shift+climb_area_width; ix += ir ) {
                    for ( iy = ladder_a; iy <= ladder_a+climb_area_height; iy += ir ) {
                        void platform;
                        float wall_height;
                        int hole;
                        int xdir_climb = glv("xdir_climb");
                        int ydir_climb = glv("ydir_climb");

                        if ( xdir_climb == NULL() ) xdir_climb = 0;
                        if ( ydir_climb == NULL() ) ydir_climb = 0;

                        // Se c'è un ostacolo, cerchiamo nella direzione opposta
                        if ( xdir_climb != 0 || ydir_climb != 0 ) {
                            if ( (xdir_climb < 0 && ix < x) || (xdir_climb > 0 && ix > x) || xdir_climb == 0 ) {
                                if ( (ydir_climb < 0 && iy < a) || (ydir_climb > 0 && iy > a) ) continue;
                                else if ( ydir_climb == 0 ) continue;
                            }
                        }

                        hole = checkhole(ix,ladder_z);
                        if ( hole && iy <= ladder_a+t_y_adjust ) {
                            //iy = ladder_a+t_y_adjust+1;
                            continue;
                        }
                        platform = find_platform_in_a(ix,ladder_z+front_ladder_shift,iy+t_search_protrusion,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                        wall_height = checkwall(ix,ladder_z+front_ladder_shift);

                        // ignore obstacles
                        platform_obstacle = find_platform_in_a(ix,ladder_z+t_border_obstacle,iy+t_search_protrusion,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                        wall_obstacle = checkwall(ix,ladder_z+t_border_obstacle);
                        if ( platform == platform_obstacle ) platform = NULL();
                        if ( wall_height == wall_obstacle ) wall_height = 0;

                        if ( platform != NULL() && gep(platform,"exists") ) { // platform
                            float platform_h = gep(platform,"height");
                            float platform_y = gep(platform,"y");

                            if ( platform_h == NULL() ) platform_h = 0;

                            if ( ladder_a+climb_area_height <= platform_y+platform_h && ladder_a+climb_area_height > platform_y ) {
                                tmp_distAB = distAB(x,a,ix,platform_y+platform_h);
                                if ( tmp_distAB != -1 && (distAB == -1 || tmp_distAB < distAB) ) {
                                    distAB = tmp_distAB;
                                    min_target_x = get_middle_x_coord(ladder,platform,0);
                                    min_target_y = platform_y+platform_h;
                                }
                            } else if ( ladder_a == platform_y+platform_h ) {
                                tmp_distAB = distAB(x,a,ix,platform_y+platform_h);
                                if ( tmp_distAB != -1 && (distAB == -1 || tmp_distAB < distAB) ) {
                                    distAB = tmp_distAB;
                                    min_target_x = get_middle_x_coord(ladder,platform,0);
                                    min_target_y = platform_y+platform_h;
                                }
                            }
                        } else {
                            if ( wall_height > 0 ) { // wall
                                if ( ladder_a+climb_area_height <= wall_height+t_search_protrusion && ladder_a+climb_area_height >= wall_height-t_search_protrusion ) {
                                    tmp_distAB = distAB(x,a,ix,wall_height);
                                    if ( tmp_distAB != -1 && (distAB == -1 || tmp_distAB < distAB) ) {
                                        distAB = tmp_distAB;
                                        min_target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        min_target_y = wall_height;
                                    }
                                } else if ( ladder_a == wall_height ) {
                                    tmp_distAB = distAB(x,a,ix,wall_height);
                                    if ( tmp_distAB != -1 && (distAB == -1 || tmp_distAB < distAB) ) {
                                        distAB = tmp_distAB;
                                        min_target_x = get_middle_x_coord(ladder,NULL(),wall_height);
                                        min_target_y = wall_height;
                                    }
                                }
                            } else { // base
                                if ( iy <= 0 && ladder_a <= 0 ) {
                                    tmp_distAB = distAB(x,a,ix,iy);
                                    if ( tmp_distAB != -1 && (distAB == -1 || tmp_distAB < distAB) ) {
                                        distAB = tmp_distAB;
                                        min_target_x = ix;
                                        min_target_y = iy;
                                    }
                                }
                            }
                        } // fine if platform exists
                    } // fine for2
                } // fine for1

                if ( min_target_x != -1 && min_target_y != -1 ) {
                    slv(target_x_label,min_target_x);
                    slv(target_y_label,min_target_y);

                    return 1;
                } else {
                    set_action("start"); // or jump
                    return 0;
                }
            } // fine if player fermo
        } // fine if ladder
        // -------------- END SEARCH PROTRUSION --------------
    }

    return 1;
}

int reset_climb_to(int reset_dir_flag) {
    slv("x_dist_climb",NULL());
    slv("y_dist_climb",NULL());

    if ( reset_dir_flag != NULL() && reset_dir_flag > 0 ) {
        slv("xdir_climb",NULL());
        slv("ydir_climb",NULL());
    }
}

int climb_to(void player, float target_x, float target_y, int stop_flag) {
    float base = gep(player,"base");
    float x = gep(player,"x");
    float z = gep(player,"z");
    float y = gep(player,"y");
    float base = gep(player,"base");
    float xdir = 0, ydir = 0, x_dist = 0, y_dist = 0;
    int climb_semaphore = 0;
    float t_dist = 2, t_half_dist = 12; // threshold for half_dist
    int mod_target_flag = 0;
    void ladder = getentityvar(player,"ladder");

    //if ( y <= 0 ) y = base;

    // Se target_x o target_y sono fuori area e l'altra coord però è dentro, aggiustiamo la coordinata sbagliata affinchè rimanga dentro l'area
    if ( gep(ladder,"exists") ) {
        float t_adjust_start = 10, t_adjust_end = 46;
        float ladder_base = gep(ladder,"base");
        float ladder_x = gep(ladder,"x");
        float ladder_z = gep(ladder,"z");
        float ladder_a = gep(ladder,"y");
        float climb_area_width = getentityvar(ladder,"width");
        float climb_area_height = getentityvar(ladder,"height");

        if ( x >= ladder_x && x <= ladder_x+climb_area_width ) { // se sei dentro l'area (in orizzontale)
            if ( x < target_x-t_dist*2 || x > target_x+t_dist*2 ) { // se ancora non sei arrivato a destinazione (in orizzontale)
                if ( target_y < ladder_a ) { target_y = ladder_a+t_adjust_start; mod_target_flag = 1; }
                else if ( target_y > ladder_a+climb_area_height/2 ) { target_y = ladder_a+climb_area_height-t_adjust_end; mod_target_flag = 1; }
            }
        } else if ( y >= ladder_a && y <= ladder_a+climb_area_height ) { // se sei dentro l'area (in verticale)
            if ( y < target_y-t_dist*2 || y > target_y+t_dist*2 ) { // se ancora non sei arrivato a destinazione (in verticale)
                if ( target_x < ladder_x ) { target_x = ladder_x+t_adjust_start; mod_target_flag = 1; }
                else if ( target_x > ladder_x+climb_area_width ) { target_x = ladder_x+climb_area_width-t_adjust_start; mod_target_flag = 1; }
            }
        }
    }

    if ( target_x < x+t_dist ) { // target a sx
        xdir = -1;
        // sul cambio di direzione, resettiamo le distanze
        if ( glv("xdir_climb") == 1 ) {
            slv("x_dist_climb",NULL());
            slv("y_dist_climb",NULL());
        }
    } else if ( target_x > x-t_dist ) { // target a dx
        xdir = 1;
        if ( glv("xdir_climb") == -1 ) {
            slv("x_dist_climb",NULL());
            slv("y_dist_climb",NULL());
        }
    } else {
        xdir = 0;
        slv("x_dist_climb",NULL());
    }
    if ( target_x >= x-t_dist && target_x <= x+t_dist ) {
        xdir = 0;
        slv("x_dist_climb",NULL());
    }
    slv("xdir_climb",xdir);

    if ( target_y < y+t_dist ) { // target è sotto
        ydir = -1;
        if ( glv("ydir_climb") == 1 ) {
            slv("x_dist_climb",NULL());
            slv("y_dist_climb",NULL());
        }
    } else if ( target_y > y-t_dist ) { // target è sopra
        ydir = 1;
        if ( glv("ydir_climb") == -1 ) {
            slv("x_dist_climb",NULL());
            slv("y_dist_climb",NULL());
        }
    } else {
        ydir = 0;
        slv("y_dist_climb",NULL());
    }
    if ( target_y >= y-t_dist && target_y <= y+t_dist ) {
        // se stai andando verso il basso vedi se c'è una piattaforma/muro/ground. In tal caso prosegui senza soglia.
        if ( ydir == -1 ) {
            int platform_exists = there_is_a_platform(target_x,z,target_y);
            int wall_exists = there_is_a_wall(target_x,z,target_y);

            if ( platform_exists || wall_exists || target_y <= 0 ) {
                ydir = -1;
            } else {
                ydir = 0;
                slv("y_dist_climb",NULL());
            }
        } else {
            ydir = 0;
            slv("y_dist_climb",NULL());
        }
    }
    slv("ydir_climb",ydir);

    x_dist = diff(x,target_x);
    y_dist = diff(y,target_y);

    // SMOOTH MOVEMENTS (HALF DISTANCE CRITERIA)
    if ( glv("x_dist_climb") == NULL() && glv("y_dist_climb") == NULL() ) {
        slv("x_dist_climb",x_dist/2);
        slv("y_dist_climb",y_dist/2);
    }

    if ( glv("x_dist_climb") <= t_dist ) {
        xdir = 0;
        slv("x_dist_climb",NULL());
    }
    if ( glv("y_dist_climb") <= t_dist ) {
        ydir = 0;
        slv("y_dist_climb",NULL());
    }

    if ( ydir == 0 && xdir == 0 && !mod_target_flag ) { // se sei giunto a destinazione
        if ( checkhole(x,z) ) {
            slv("x_dist_climb",0);
            slv("y_dist_climb",0);
            slv("xdir_climb",NULL());
            slv("ydir_climb",NULL());
            set_action("none");
            return 1;
        }

        if ( stop_flag > 0 ) set_action("none"); // or jump
        else set_action("start");

        return 1;
    }

    // SMOOTH MOVEMENTS (DARE LA PRIORITA' ALLA DISTANZA MAGGIORE)
    if ( glv("x_dist_climb") != NULL() && x_dist > glv("x_dist_climb") ) {
        climb_semaphore = 1;
    } else if ( glv("y_dist_climb") != NULL() && y_dist > glv("y_dist_climb") ) {
        climb_semaphore = 0;
    } else {
        slv("x_dist_climb",NULL());
        slv("y_dist_climb",NULL());
    }
    if ( glv("x_dist_climb") != NULL() && x_dist <= glv("x_dist_climb") && x_dist >= t_half_dist ) slv("x_dist_climb",NULL());
    if ( glv("y_dist_climb") != NULL() && y_dist <= glv("y_dist_climb") && y_dist >= t_half_dist ) slv("y_dist_climb",NULL());

    if ( climb_semaphore ) {
        if ( xdir == -1 ) set_action("moveleft");
        else if ( xdir == 1 ) set_action("moveright");
    } else {
        if ( ydir == 1 ) set_action("moveup");
        else if ( ydir == -1 ) set_action("movedown");
    }

    if ( getlocalvar("action_flag") == NULL() ) {
        set_action("start"); // or jump
        return 0;
    }

    return 0;
}


