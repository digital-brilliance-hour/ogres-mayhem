
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/player_didhit.c"
#import "data/scripts/wall_splat.c"
//#import "data/scripts/player_backpain.c"

void main() {
  //void self = getlocalvar("self");
  //void plane = getlocalvar("plane");
  //void height = getlocalvar("height"); Height of blocking wall.
  /*onblockascript
        Each update while entity is prevented from moving upward through platform (i.e. "hitting head").
        self: Caller.
        obstacle: Entity blocking caller.
   */


     //drawstring( 20,180,0,"PLATFORM!!" );return;
     //check_platformsplat(self,0); // (onblockascript)
     //check_hold_platform(self); // (onblockascript??)
}

int getoppx(void player) {
    return getentityvar(player, 6);
}

int check_hold_platform(void player) {
    int anim_id = getentityproperty(player,"animationid");
    int p = getentityproperty(player, "playerindex");
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    int dir = getentityproperty(player, "direction");
    float t_dist = 3;

    if ( anim_id == openborconstant("ANI_JUMP") ) { // && anim_id != openborconstant("ANI_FOLLOW45") && getentityproperty(player, "aiflag", "jumping")
        if ( playerkeys(p,0,"moveright") == 8 && playerkeys(p,0,"moveleft") != 4 && dir ) {
            void platform = find_platform_in_a(x+t_dist+17,z+0,a,MAX_ALTITUDE,10,2); // Stabiliamo l'altezza della piattaforma e la quota della stessa

            if ( platform != NULL() ) {
                void unwalkable = getentityvar(platform, "walkable");

                if ( unwalkable != "unwalkable" ) {
                    changeentityproperty(player, "aiflag", "jumping", 0); // se è a 1 possiamo muoverci
                    //changeentityproperty(player, "aiflag", "running", 0);
                    performattack(player, openborconstant("ANI_FOLLOW45"), 1);
                    //set_hold_wall_anim(player);
                }
            }
        } else if ( playerkeys(p,0,"moveleft") == 4 && playerkeys(p,0,"moveright") != 8 && !dir ) {
            void platform = find_platform_in_a(x-t_dist,z+0,a,MAX_ALTITUDE,10,2); // Stabiliamo l'altezza della piattaforma e la quota della stessa

            if ( platform != NULL() ) {
                void unwalkable = getentityvar(platform, "walkable");

                if ( unwalkable != "unwalkable" ) {
                    changeentityproperty(player, "aiflag", "jumping", 0); // se è a 1 possiamo muoverci
                    //changeentityproperty(player, "aiflag", "running", 0);
                    performattack(player, openborconstant("ANI_FOLLOW45"), 1);
                    //set_hold_wall_anim(player);
                }
            }

        }
    } // fine se sta saltando
}

int check_platformsplat(void player, float a_threshold) {
    void opp = getentityproperty(player, "opponent");
    int anim_id = getentityproperty(player,"animationid");
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float height = getentityproperty(player, "height");
    float t_diff = 0, t_dist = 5; // t_diff significa che se la soglia a_threshold = 0 splatta anche a terra
    int dir = getentityproperty(player, "direction");
    int prev_dir = getentityvar(player,0);
    void platform;

    if ( !is_in_pain(player) ) return 0;

    if ( height == NULL() ) height = T_ENT_HEIGHT;

    //platform = find_platform_around_in_a(player,t_dist,t_dist,a,MAX_ALTITUDE);
    //platform = find_platform_around_in_a_with_height(player,t_dist,t_dist,a,height,MAX_ALTITUDE,1,1);
    platform = find_platform_around_in_a_with_height(player,t_dist,0,a,height,MAX_ALTITUDE,1,1);

    if ( platform != NULL() ) {
        float platform_x = getentityproperty(platform, "x");
        void unsplattable = getentityvar(platform, "splattable");
        //drawstring( 20,180,0,"PIATTAFORMA!!" );

        if ( platform_x > x && prev_dir != 1 ) return 0;
        if ( platform_x < x && prev_dir != 0 ) return 0;

        if ( a > base ) t_diff = a-base;

        /*if ( anim_id != 2333 && getentityproperty(player, "invincible") == 0  && a >= base && getentityproperty(player, "aiflag", "running") == 1
                        && anim_id != openborconstant("ANI_FOLLOW30") && anim_id != openborconstant("ANI_FOLLOW34") && anim_id != openborconstant("ANI_FOLLOW31")
                        && anim_id != openborconstant("ANI_FOLLOW35") && anim_id != openborconstant("ANI_FOLLOW32") && anim_id != openborconstant("ANI_RISE10")
                        && anim_id != openborconstant("ANI_DIE10") && checksplattablewall(player,1,1,1001) == 1 ) {*/

        // getentityproperty(self, "invincible") == 0 && getentityproperty(self,"animationid") == FALL (IN ARIA) e anche "y" >= threshold  ||||||||||  getentityproperty(self, "running", "speed")
        // a_threshold: if (a > base ) a_threshold = a-base; --> if ( t_diff >= a_threshold )
        // CAMBIARE LE PRIME 2!!
        if ( getentityproperty(player, "invincible") == 0  && a >= base && t_diff >= a_threshold && unsplattable != "unsplattable"
                        && anim_id != openborconstant("ANI_FOLLOW30") && anim_id != openborconstant("ANI_FOLLOW34") && anim_id != openborconstant("ANI_FOLLOW31")
                        && anim_id != openborconstant("ANI_FOLLOW35") && anim_id != openborconstant("ANI_FOLLOW32") && anim_id != openborconstant("ANI_RISE10")
                        && anim_id != openborconstant("ANI_DIE10") && anim_id != openborconstant("ANI_SHOCK") &&
                        ( anim_id == openborconstant("ANI_FALL") || anim_id == openborconstant("ANI_FALL2") || anim_id == openborconstant("ANI_FALL3")
                            || anim_id == openborconstant("ANI_FALL50") || anim_id == openborconstant("ANI_FALL51") || anim_id == openborconstant("ANI_FALL52") || anim_id == openborconstant("ANI_FALL71")) ) {

                // IMPORTANTISSIMO: noaicontrol + cambio d'animazione funziona solo se lo script viene ripetuto a ciclo ad ogni frame!!
                changeentityproperty(player, "noaicontrol", 1); // lo facciamo perchè con qualche mossa speciale altrimenti può evitare di cadere
                changeentityproperty(player, "antigravity", 1);
                changeentityproperty(player, "position", x, z, a);
                //changeentityproperty(self, "base", a);

                bindentity(player,NULL());
                if (!opp || !getentityproperty(opp,"exists")) {
                        damageentity(player,player,30,0,openborconstant("ATK_NORMAL10")); // <--------------
                } else {
                        damageentity(player,opp,30,0,openborconstant("ATK_NORMAL10")); // <--------------
                }
                check_versus_mark(opp,player);

                // SUPER IMPORTANTISSIMO: Se non imposti la velocity a 0 su tutti i parametri, il player continuerà a camminare anche dopo noaicontrol impostato a vero!!
                changeentityproperty(player, "velocity", 0, 0, 0);

                if (opp && getentityproperty(opp,"exists")) {
                    if ( getentityproperty(player, "x") < getoppx(player) ) setentityvar(player,0,0);
                    else setentityvar(player,0,1);
                }

                if ( getentityvar(player,0) != 2 && getentityvar(player,0) != NULL() ) {
                    if ( getentityvar(player,0) == dir ) {
                        changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW30"));
                    } else {
                        changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW34"));
                    }
                } else { // use prev dir
                    if ( getentityvar(player, 4) == dir ) {
                        changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW30"));
                    } else {
                        changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW34"));
                    }
                }
        } // fine serie di if


    } // fine check_platformaround

    return 1;
}

