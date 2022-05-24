
/*
 * Da Ricordare: Impostare "last_player_spawned" per i players!
 * ATK_ usati: 1-6-7-8-9-10
 * entityvar usate (indexes): 0(getwalkingdir), 1(time/timer/mouser/check_splat_grab), 2(combo counter), 3 (grabber), 4 (backpains),
                              5(jump on hold wall), 6 (opp x for backpain), 7 (walkoff), 8 (time for backpain), 9 (wallsplat dir),
                              10 (jump velocity), 11 (fly_foot_animpos), 12 (glider entity), 13 (stopped entity), 21 (doublegun),
                              24 (backpain_flag)
 */

#include "data/scripts/defines.h"
#import "data/scripts/lib.c"
#import "data/scripts/player_glider.c"
#import "data/scripts/player_moves.c"
#import "data/scripts/player_respawn.c"
#import "data/scripts/player_offset.c"
#import "data/scripts/player_commons.c"

void main() {
  void self = getlocalvar("self");
  int p = getentityproperty(self, "playerindex");

    // For All
    setentityvar( self, 0, getwalkingdir(self,2) );
    check_general(self);
    setjumpvel(self);
    set_inspawning(self,RESPAWN_DISTANCE_WEAP); // va messo prima di ogni set_coords
    set_coords_predeath(self,PREDEATH_COORDS_REFRESH); // memorizziamo le coordinate per il respawn...
    set_coords(self,MAX_ALTITUDE_SP_LEVEL);
    check_zbounds(self,ZMIN_BOUND__,ZMAX_BOUND__);
    check_zmax_distance(self);

    check_spawn(self);
    check_shoot(self,0);
    check_runback(self);
    check_doublegun(self);
    check_dropbomb(self);

    //setglobalvar("gdoublegun_"+0, 1);




    //drawstring( 10,190,0,"Var (doublegun): "+getglobalvar("gdoublegun_0"));
    //drawstring( 10,200,0,"Var (time): "+getentityvar(self, 23));
    //drawstring( 10,190,0,"Var (xdir): "+getentityproperty(self, "xdir"));
    //drawstring( 10,200,0,"Var (stall): "+getcombostall(self));
    //drawstring( 10,200,0,"Var (zdir): "+getentityproperty(self, "zdir"));
    //drawstring( 10,210,0,"Var (tossv): "+getentityproperty(self, "tossv"));
    //drawstring( 10,220,0,"Var (time): "+openborvariant("elapsed_time"));
    //drawstring( 10,210,0,"Var (stage): "+openborvariant("current_stage")+"  set:  "+openborvariant("current_set"));

}

int check_dropbomb(void player) {
    void stopped_ent = getstopped_ent(player);

    if (stopped_ent) {
        int s_anim_id = getentityproperty(stopped_ent, "animationid");
        int anim_id = getentityproperty(player, "animationid");
        int p = getentityproperty(player, "playerindex");
        int map = 0;
        int eta = 150;

        if ( playerkeys(p,1,"jump") == 128 ) {

            if ( s_anim_id == openborconstant("ANI_IDLE") || s_anim_id == openborconstant("ANI_RUN") || s_anim_id == openborconstant("ANI_WALK")
                 || s_anim_id == openborconstant("ANI_BACKWALK") || s_anim_id == openborconstant("ANI_UP") || s_anim_id == openborconstant("ANI_DOWN") ) {

                // Vediamo di lasciare uno spazio di tempo tra uno sparo e l'altro...
                if ( getbombtime(player) < openborvariant("elapsed_time") || getbombtime(player) == NULL() ) {
                    void glider = getglider(stopped_ent);

                    map = 5;

                    //playsample(loadsample("data/sounds/mhead03.wav"));
                    if ( glider != NULL() ) {
                            if ( anim_id == openborconstant("ANI_UP") ) {
                                generate_bomb("epdropbomb", -10,-47,0, player, glider, map);
                                setbombtime(player, get_next_time(eta));
                            } else if ( anim_id == openborconstant("ANI_DOWN") ) {
                                generate_bomb("epdropbomb", -20,-10,0, player, glider, map);
                                setbombtime(player, get_next_time(eta));
                            }  else {
                                generate_bomb("epdropbomb", -20,-20,0, player, glider, map);
                                setbombtime(player, get_next_time(eta));
                            }
                    }
                }// fine if time
            } // fine if animazioni
        } // fine if attack
    } // fine if stoppedent

}

int check_shoot(void player, int hold_flag) {
    void stopped_ent = getstopped_ent(player);

    if (stopped_ent) {
        int s_anim_id = getentityproperty(stopped_ent, "animationid");
        int anim_id = getentityproperty(player, "animationid");
        int p = getentityproperty(player, "playerindex");
        int map = 0;
        int eta = 20;
        int setlayer = 1000;

        if ( hold_flag == NULL() ) hold_flag = 1;

        if ( playerkeys(p,hold_flag,"attack") == 64 ) {

            if ( s_anim_id == openborconstant("ANI_IDLE") || s_anim_id == openborconstant("ANI_RUN") || s_anim_id == openborconstant("ANI_WALK")
                 || s_anim_id == openborconstant("ANI_BACKWALK") || s_anim_id == openborconstant("ANI_UP") || s_anim_id == openborconstant("ANI_DOWN") ) {

                // Vediamo di lasciare uno spazio di tempo tra uno sparo e l'altro...
                if ( gettime(player) < openborvariant("elapsed_time") || gettime(player) == NULL() ) {
                    void glider = getglider(stopped_ent);

                    map = 0;

                    if ( glider != NULL() ) {
                        playsample(loadsample("data/sounds/mhead03.wav"));
                        if ( getglobalvar("gdoublegun_"+p) != NULL() ) {
                            if ( anim_id == openborconstant("ANI_UP") ) {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW13"));
                                generate_shoot("epbullet", 7,-9,0, player, map); // bullet3
                                generate_shoot("epbullet", -3,-59,0, player, map,setlayer);
                                settime(player, get_next_time(eta));
                            } else if ( anim_id == openborconstant("ANI_DOWN") ) {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW14"));
                                generate_shoot("epbullet", 4,-53,0, player, map,setlayer); // bullet2
                                generate_shoot("epbullet", -1,2,0, player, map);
                                settime(player, get_next_time(eta));
                            }  else {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW15"));
                                generate_shoot("epbullet", 7,-44,0, player, map,setlayer); // bullet2
                                generate_shoot("epbullet", -8,-11,0, player, map);
                                settime(player, get_next_time(eta));
                            }
                        } else {
                            if ( anim_id == openborconstant("ANI_UP") ) {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW10"));
                                generate_shoot("epbullet", 46,-30,0, player, map);
                                settime(player, get_next_time(eta));
                            } else if ( anim_id == openborconstant("ANI_DOWN") ) {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW11"));
                                generate_shoot("epbullet", 44,-28,0, player, map);
                                settime(player, get_next_time(eta));
                            }  else {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW12"));
                                //changeentityproperty(glider, "animpos", 0);
                                generate_shoot("epbullet", 44,-37,0, player, map);
                                settime(player, get_next_time(eta));
                            }
                        }
                    }
                }// fine if time
            } // fine if animazioni
        } // fine if attack
    } // fine if stoppedent
}

