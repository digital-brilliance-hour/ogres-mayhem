
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/lib_wall.c"
//#import "data/scripts/player_bind_move.c"

#define MAX_ALT_THRESHOLD 300

/*
 * Per fare in modo di avere sempre in primo piano il pavimento più vicino alla telecamera impostare come posizione
 * lo z alla fine della risoluzione verticale (in txt del livello) es. coords xxx 240 se la vres è 240.
 * Quindi fare la gif di conseguenza e qualche volta aggiungere lo spazio vuoto in basso (0xff00ff) per avere l'immagine più in alto.
 */
int check_hole_layer_turtles(void player) {
  float x = getentityproperty(player, "x");
  float z = getentityproperty(player, "z");
  float a = getentityproperty(player, "y");
  int anim_id = getentityproperty(player, "animationid");
  int hole_layer = -3, standard_layer = 0;

    //drawstring( 10,160,0,"layer:  "+getentityproperty(player, "setlayer"));
    if ( anim_id == openborconstant("ANI_FOLLOW16") ) {
        if ( checkhole(x,z) == 1 && a <= 0  ) {
            changeentityproperty(player, "setlayer", hole_layer);
        }
    } else if ( getentityproperty(player, "setlayer") == hole_layer ) {
        if ( checkhole(x,z) == 0 && anim_id != openborconstant("ANI_FOLLOW16") ) {
            changeentityproperty(player, "setlayer", standard_layer);
        }
    }
}

int check_hole_layer(void self) {
  float x = getentityproperty(self, "x");
  float z = getentityproperty(self, "z");
  float a = getentityproperty(self, "y");
  float base = getentityproperty(self, "base");
  int anim_id = getentityproperty(self, "animationid");
  int hole_layer = -3, standard_layer = 0;

    //drawstring( 10,160,0,"layer:  "+getentityproperty(self, "setlayer"));
    if ( checkhole(x,z) == 1 && a <= 0 ) { // or && a <= base???
        changeentityproperty(self, "setlayer", hole_layer);
    } else if ( getentityproperty(self, "setlayer") == hole_layer && checkhole(x,z) == 0 ) changeentityproperty(self, "setlayer", standard_layer);
}

// use idle {flag} to enable key input on follow15 anim
int check_walkoff(void player, float threshold) {
    float x = getentityproperty(player,"x");
    float z = getentityproperty(player,"z");
    float a = getentityproperty(player,"y");
    float base = getentityproperty(player,"base");
    int dir = getentityproperty(player,"direction");
    int p = getentityproperty(player, "playerindex");
    int anim_id = getentityproperty(player, "animationid");
    int hole;
    int animvalid_edges = getentityproperty(player, "animvalid", openborconstant("ANI_FOLLOW15"));
    int animvalid_holefall = getentityproperty(player, "animvalid", openborconstant("ANI_FOLLOW16"));
    float t_alt = 1;

    //if ( (base == check_multi_platform_alt(x,z,a+MAX_ALT_THRESHOLD) || getentityvar(platform,0) == "float_platform") && a == base && anim_id == openborconstant("ANI_IDLE") ) {
    if ( animvalid_edges ) {
        float wall_h;
        void platform;

        if ( a <= base && anim_id == openborconstant("ANI_IDLE") ) {
            if ( (getplayerproperty(p, "keys") <= 2 || getplayerproperty(p, "keys") == 512) ) {
                platform = get_platform_on(player,t_alt);
                if ( platform != NULL() ) {
                    void platform_right = checkplatform(x+threshold,z,a+t_alt);
                    void platform_left  = checkplatform(x-threshold,z,a+t_alt);
                    void platform_up    = checkplatform(x,z-threshold/2,a+t_alt);
                    void platform_down  = checkplatform(x,z+threshold/2,a+t_alt);
                    float plat_a_right = 0, plat_a_left = 0, plat_a_up = 0, plat_a_down = 0;
                    float plat_height_right = 0, plat_height_left = 0, plat_height_up = 0, plat_height_down = 0;

                    if ( platform_right != NULL() ) { plat_a_right = getentityproperty(platform_right,"y"); if (getentityproperty(platform_right,"height") != NULL()) plat_height_right = getentityproperty(platform_right,"height"); }
                    if ( platform_left  != NULL() ) { plat_a_left  = getentityproperty(platform_left, "y"); if (getentityproperty(platform_left, "height") != NULL()) plat_height_left =  getentityproperty(platform_left, "height"); }
                    if ( platform_up    != NULL() ) { plat_a_up    = getentityproperty(platform_up,   "y"); if (getentityproperty(platform_up,   "height") != NULL()) plat_height_right = getentityproperty(platform_up,   "height"); }
                    if ( platform_down  != NULL() ) { plat_a_down  = getentityproperty(platform_down, "y"); if (getentityproperty(platform_down, "height") != NULL()) plat_height_down =  getentityproperty(platform_down, "height"); }

                    //drawstring(10,100,0,"IS_ON_PLATFORM");

                    if ( (!dir && platform_left == NULL()) || (dir && platform_right == NULL())
                        || platform_down == NULL() || platform_up == NULL()
                        || (!dir && platform_left  != NULL() && platform != platform_left  && (plat_a_left+plat_height_left < a-t_alt   || plat_a_left+plat_height_left > a+t_alt) )
                        || ( dir && platform_right != NULL() && platform != platform_right && (plat_a_right+plat_height_right < a-t_alt || plat_a_right+plat_height_right > a+t_alt) )
                        || (platform_up   != NULL() && platform != platform_up             && (plat_a_up+plat_height_up < a-t_alt       || plat_a_up+plat_height_up > a+t_alt) )
                        || (platform_down != NULL() && platform != platform_down           && (plat_a_down+plat_height_down < a-t_alt    || plat_a_down+plat_height_down > a+t_alt) )
                        || getentityvar(platform,"walkoff") == "unbalanced" ) { // || getentityvar(platform,0) == "float_platform"
                        changeentityproperty(player, "velocity", 0,0,0);
                        if ( anim_id != openborconstant("ANI_FOLLOW15") ) performattack(player, openborconstant("ANI_FOLLOW15"), 1);
                        changeentityproperty(player, "attacking", 0);
                        changeentityproperty(player, "aiflag", "attacking", 0);
                        //changeentityproperty(player,"position",NULL(),NULL(),getentityproperty(player,"base"));
                        setlocalvar("EDGE_ANIMATION",1);

                        return 1;
                    } // fine if le varie directions

                } // fine if platform
            } // fine if keys
        } // fine if a <= base
        if ( getlocalvar("EDGE_ANIMATION") == 1 ) {
            if ( checkplatformaround(player,threshold,threshold,MAX_ALTITUDE) == NULL() ) {
                if ( anim_id == openborconstant("ANI_FOLLOW15") ) setidle(player,openborconstant("ANI_IDLE"),1);
                setlocalvar("EDGE_ANIMATION",NULL());
            }
        }

        // check su WALL
        if (a <= base) {
            wall_h = checkwall(x,z);
            if ( wall_h != 0 && base == wall_h && anim_id == openborconstant("ANI_IDLE")
                && (getplayerproperty(p, "keys") <= 2 || getplayerproperty(p, "keys") == 512) ) {

                if ( (!dir && checkwall(x-threshold,z) < base ) || (dir && checkwall(x+threshold,z) < base )
                      || checkwall(x,z+threshold/2) < base || checkwall(x,z-threshold/2) < base ) {
                    changeentityproperty(player, "velocity", 0,0,0);
                    if ( anim_id != openborconstant("ANI_FOLLOW15") ) performattack(player, openborconstant("ANI_FOLLOW15"), 1);
                    changeentityproperty(player, "attacking", 0);
                    changeentityproperty(player, "aiflag", "attacking", 0);

                    return 1;
                }
            }
        }
    }

    // check su HOLE
    if (base <= 0) {
        hole = checkhole(x,z);

        if ( !hole ) {
            if ( anim_id == openborconstant("ANI_IDLE") && (getplayerproperty(p, "keys") <= 2 || getplayerproperty(p, "keys") == 512) && animvalid_edges ) {
                int hole_right = checkhole(x+threshold,z);
                int hole_left  = checkhole(x-threshold,z);

                if ( ((!dir && hole_left) || (dir && hole_right)) ){
                      //||  checkhole(x,z+threshold) != 0 || checkhole(x,z-threshold) != 0 ) {
                    changeentityproperty(player, "velocity", 0,0,0);
                    if ( anim_id != openborconstant("ANI_FOLLOW15") ) performattack(player, openborconstant("ANI_FOLLOW15"), 1);
                    changeentityproperty(player, "attacking", 0);
                    changeentityproperty(player, "aiflag", "attacking", 0);

                    return 1;
                }
            }
        } else {
            if ( animvalid_holefall ) {
                // Controlliamo se stiamo cadendo in un buco. Se si cambiamo animazione e non la solita walkoff
                if ( anim_id == openborconstant("ANI_WALKOFF") && a <= 0 ) {
                    changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW16"));
                }

                // Controlliamo se stiamo cadendo in un buco dopo un wallsplat
                if ( ( anim_id == openborconstant("ANI_FOLLOW32") || anim_id == openborconstant("ANI_RISE10") || anim_id == openborconstant("ANI_DIE10") ) && a <= 0 ) {
                    changeentityproperty(player, "direction", getentityvar(player,9));
                    changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW16"));
                }
            }
        }
    }

    // Con alcune mosse di jumping potrebbe accadere che si esegua la walkoff anim. Non permettiamolo!!
    /*if ( anim_id == openborconstant("ANI_WALKOFF") && a <= base ) {
        if ( checkwallaround(player,threshold,threshold) == 0 ) setidle(player, openborconstant("ANI_IDLE"), 1);
        else if ( checkwallaround(player,threshold,threshold) != 0 ) setidle(player, openborconstant("ANI_IDLE"), 1);

        if ( checkplatformaround(player,threshold,threshold,MAX_ALTITUDE) == NULL() ) setidle(player, openborconstant("ANI_IDLE"), 1);
        else if ( checkplatformaround(player,threshold,threshold,MAX_ALTITUDE) != NULL() ) setidle(player, openborconstant("ANI_IDLE"), 1);
    }*/

  return 0;
}


