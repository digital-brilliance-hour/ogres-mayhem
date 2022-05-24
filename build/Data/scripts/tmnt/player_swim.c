
/*
 * Da Ricordare: Impostare "last_player_spawned" per i players!
 * ATK_ usati: 1-6-7-8-9-10
 * entityvar usate (indexes): 0(getwalkingdir), 1(time/timer/mouser/check_splat_grab), 2(combo counter), 3 (grabber), 4 (backpains),
                              5(jump on hold wall), 6 (opp x for backpain), 7 (walkoff), 8 (time for backpain), 9 (wallsplat dir),
                              10 (jump velocity), 11 (swim dir), 24 (backpain_flag)
 */

#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/player_moves.c"
#import "data/scripts/player_respawn.c"
#import "data/scripts/player_respawn_swim.c"
#import "data/scripts/player_offset.c"
#import "data/scripts/player_offset_swim.c"

#define MAX_SPEED 1.3
#define MIN_XFACTOR 0.01
#define MIN_SPEED 0.5
#define XFACTOR 0.25

#define MAX_ZSPEED 1.5
#define ZFACTOR 1.4

#define GRAVITY 1.1
#define MAX_GRAVITY 2.8

#define GRAVITY_DIE 0.07
#define MAX_GRAVITY_DIE 0.8

void main() {
  void self = getlocalvar("self");
  int p = getentityproperty(self, "playerindex");

    // For All
    setentityvar( self, 0, getwalkingdir(self,2) );
    check_general(self);
    setjumpvel(self);
    set_inspawning(self,RESPAWN_DISTANCE_WEAP); // va messo prima di ogni set_coords
    set_coords_predeath_swim(self,PREDEATH_COORDS_REFRESH); // memorizziamo le coordinate per il respawn...
    set_coords(self,MAX_ALTITUDE_SP_LEVEL);
    check_zbounds(self,ZMIN_BOUND__,ZMAX_BOUND__);
    check_zmax_swim_distance(self);

    check_speed(self);
    check_swimdir(self);
    if ( getglobalvar("zbound_"+p) == NULL() ) check_gravity(self);
    check_zbound_deactivation(self);
    check_controls(self);
    check_swim_forward(self);
    check_swim_up(self);
    check_swim_down(self);
    check_anim(self);
    check_idle(self);
    check_turn(self);
    check_spawn(self);






    //drawstring( 10,180,0,"Var (zbound): "+getglobalvar("zbound_"+p));
    //drawstring( 10,190,0,"Var (grav): "+getgravity(self));
    //drawstring( 10,190,0,"Var (xdir): "+getentityproperty(self, "xdir"));
    //drawstring( 10,200,0,"Var (zdir): "+getentityproperty(self, "zdir"));
    //drawstring( 10,210,0,"Var (tossv): "+getentityproperty(self, "tossv"));
    //drawstring( 10,220,0,"Var (time): "+openborvariant("elapsed_time"));
    //drawstring( 10,210,0,"Var (stage): "+openborvariant("current_stage")+"  set:  "+openborvariant("current_set"));

}

int check_zbound_deactivation(void player) {
    int p = getentityproperty(player, "playerindex");
    int zbound = getglobalvar("zbound_"+p);

    if ( zbound != NULL() ) {
        if ( openborvariant("count_players") <= 1 ) setglobalvar("zbound_"+p, NULL());
    }

}

int check_gravity(void player) {
    float xdir = getspeed(player);
    float zdir = getgravity(player);
    float tossv = getentityproperty(player, "tossv");
    float z = getentityproperty(player, "z");
    int anim_id = getentityproperty(player, "animationid");

        // SIMULATE GRAVITY
        if ( anim_id == 68 ) {
            xdir = 0;
        } else {
            if ( xdir > 0 ) {
                    xdir -= XFACTOR/100;
                    if ( xdir < 0 ) xdir = 0;
            } else if ( xdir < 0 ) {
                    xdir += XFACTOR/100;
                    if ( xdir > 0 ) xdir = 0;
            }
        } // fine if sdie

        setspeed(player, xdir);

        if ( anim_id == 68 ) { // SDIE
            if ( z < openborvariant("levelheight") ) {
                zdir += GRAVITY_DIE/100; // GRAVITY
                if ( zdir > MAX_GRAVITY_DIE ) zdir = MAX_GRAVITY_DIE;
            }
        } else {
            if ( z < openborvariant("levelheight") ) {
                zdir += GRAVITY/100; // GRAVITY
                if ( zdir > MAX_GRAVITY ) zdir = MAX_GRAVITY;
            }
        } // fine if sdie

        setgravity(player, zdir);

    changeentityproperty(player, "velocity", getspeed(player), getgravity(player), tossv);
}

int swim_down(void player, int anim_flag) {
    int p = getentityproperty(player, "playerindex");
    int anim_id = getentityproperty(player, "animationid");

        if ( playerkeys(p,1,"jump") == 128 && anim_id != openborconstant("ANI_TURN") && anim_id != openborconstant("ANI_SHOCK")
            && anim_id != openborconstant("ANI_DIE") && anim_id != 68 ) { // SDIE
            float zdir = getgravity(player);
            float vel;

                if ( playerkeys(p,0,"movedown") == 32 && playerkeys(p,0,"moveup") != 16 ) {
                    if ( anim_flag == 1 ) performattack(player, openborconstant("ANI_FOLLOW64"), 1);
                    else performattack(player, openborconstant("ANI_FOLLOW65"), 1);

                    //zdir = ZFACTOR-ZFACTOR/2;
                    vel = ZFACTOR-ZFACTOR/3;
                    zdir += vel;
                    if ( zdir > vel ) zdir = vel;

                    setgravity(player, zdir);
                    check_swim_diagonal(player); // Per il movimento diagonale
                }
        } // fine if jump
}

int check_swim_down(void player) {
    int p = getentityproperty(player, "playerindex");
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id != openborconstant("ANI_FOLLOW64") && anim_id != openborconstant("ANI_FOLLOW65") && anim_id != openborconstant("ANI_TURN") ) {
            if ( anim_id != openborconstant("ANI_IDLE") ) {
                swim_down(player, 1);
            } else if ( anim_id == openborconstant("ANI_IDLE") && timerdiff(player) > 30 ) {
                swim_down(player, 1);
            } // fine if idle
    } // fine if follow62

    if ( anim_id == openborconstant("ANI_FOLLOW65") && getentityproperty(player, "animpos") >= 3 ) {
        swim_down(player, 1);
    } // fine if follow61

    if ( anim_id == openborconstant("ANI_FOLLOW64") && getentityproperty(player, "animpos") >= 3 ) {
        swim_down(player, 0);
    } // fine if follow60

}

int swim_up(void player, int anim_flag) {
    int p = getentityproperty(player, "playerindex");
    int anim_id = getentityproperty(player, "animationid");

        if ( playerkeys(p,1,"jump") == 128 && anim_id != openborconstant("ANI_TURN") && anim_id != openborconstant("ANI_SHOCK")
            && anim_id != openborconstant("ANI_DIE") && anim_id != 68 ) { // SDIE
            float zdir = getgravity(player)/500;

                if ( playerkeys(p,0,"moveup") == 16 && playerkeys(p,0,"movedown") != 32 ) {
                    if ( anim_flag == 1 ) performattack(player, openborconstant("ANI_FOLLOW62"), 1);
                    else performattack(player, openborconstant("ANI_FOLLOW63"), 1);

                    /*
                    if ( zdir >= 0 ) zdir = -1*ZFACTOR;
                    else zdir -= ZFACTOR;
                    if ( zdir < -1*MAX_ZSPEED ) zdir = -1*MAX_ZSPEED;
                    */
                    // Anche se la gravità può averevaolri alti, poniamo un limite. Facciamo come se la gravità max fosse la velocità max di risalita.
                    // Questo ci aiuterà a riprendere subito quota in risalita!!
                    zdir = getgravity(player);
                    if ( zdir > MAX_ZSPEED ) zdir = MAX_ZSPEED;

                    if ( zdir >= 0 ) zdir -= ZFACTOR+ZFACTOR/2+ZFACTOR/3;
                    else zdir -= ZFACTOR;
                    if ( zdir < -1*MAX_ZSPEED ) zdir = -1*MAX_ZSPEED;

                    setgravity(player, zdir);
                    check_swim_diagonal(player); // Per il movimento diagonale
                }
        } // fine if jump
}

int check_swim_diagonal(void player) {
    int p = getentityproperty(player, "playerindex");

        // Per il movimento diagonale
        if ( playerkeys(p,0,"moveright") == 8 && getentityproperty(player, "direction") == 1 ) {
            float xdir = getspeed(player);

            xdir += XFACTOR;
            if ( xdir > MAX_SPEED ) xdir = MAX_SPEED;
            setspeed(player, xdir);
        } else if ( playerkeys(p,0,"moveleft") == 4 && getentityproperty(player, "direction") == 0 ) {
            float xdir = getspeed(player);

            xdir -= XFACTOR;
            if ( xdir < -1*MAX_SPEED ) xdir = -1*MAX_SPEED;
            setspeed(player, xdir);
        } // fine if diagonale
}

int check_swim_up(void player) {
    int p = getentityproperty(player, "playerindex");
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id != openborconstant("ANI_FOLLOW62") && anim_id != openborconstant("ANI_FOLLOW63") && anim_id != openborconstant("ANI_TURN") ) {
            if ( anim_id != openborconstant("ANI_IDLE") ) {
                swim_up(player, 1);
            } else if ( anim_id == openborconstant("ANI_IDLE") && timerdiff(player) > 30 ) {
                swim_up(player, 1);
            } // fine if idle
    } // fine if follow62

    if ( anim_id == openborconstant("ANI_FOLLOW63") && getentityproperty(player, "animpos") >= 3 ) {
        swim_up(player, 1);
    } // fine if follow61

    if ( anim_id == openborconstant("ANI_FOLLOW62") && getentityproperty(player, "animpos") >= 3 ) {
        swim_up(player, 0);
    } // fine if follow60

}

int swim_forward(void player, int anim_flag) {
    int p = getentityproperty(player, "playerindex");
    int anim_id = getentityproperty(player, "animationid");

    if ( playerkeys(p,1,"jump") == 128 && anim_id != openborconstant("ANI_TURN") && anim_id != openborconstant("ANI_SHOCK")
            && anim_id != openborconstant("ANI_DIE") && anim_id != 68 ) { // SDIE

        if ( playerkeys(p,0,"moveup") != 16 && playerkeys(p,0,"movedown") != 32 ) {
                float xdir = getspeed(player);
                //xdir = getentityproperty(player, "xdir");

            if ( playerkeys(p,0,"moveright") == 8 && getentityproperty(player, "direction") == 1 ) {
                if ( anim_flag == 1 ) performattack(player, openborconstant("ANI_FOLLOW60"), 1);
                else performattack(player, openborconstant("ANI_FOLLOW61"), 1);

                xdir += XFACTOR;
                if ( xdir > MAX_SPEED ) xdir = MAX_SPEED;
                setspeed(player, xdir);

                // Addolciamo la nuotata orizzontale sulla gravità
                //setgravity(player, getgravity(player)/500-1*ZFACTOR/3);
                if ( getgravity(player) > MAX_GRAVITY/2 ) setgravity(player, MAX_GRAVITY/2);
                setgravity(player, getgravity(player)-(getgravity(player)/5)-ZFACTOR/3);
            } else if ( playerkeys(p,0,"moveleft") == 4 && getentityproperty(player, "direction") == 0 ) {
                if ( anim_flag == 1 ) performattack(player, openborconstant("ANI_FOLLOW60"), 1);
                else performattack(player, openborconstant("ANI_FOLLOW61"), 1);

                xdir -= XFACTOR;
                if ( xdir < -1*MAX_SPEED ) xdir = -1*MAX_SPEED;
                setspeed(player, xdir);

                // Addolciamo la nuotata orizzontale sulla gravità
                //setgravity(player, getgravity(player)/500-1*ZFACTOR/3);
                if ( getgravity(player) > MAX_GRAVITY/2 ) setgravity(player, MAX_GRAVITY/2);
                setgravity(player, getgravity(player)-(getgravity(player)/4)-ZFACTOR/3);
            }
        }
   } // fine if jump
}

int check_swim_forward(void player) {
    int p = getentityproperty(player, "playerindex");
    int anim_id = getentityproperty(player, "animationid");
    int t_frames = 30;

    if ( anim_id != openborconstant("ANI_FOLLOW60") && anim_id != openborconstant("ANI_FOLLOW61") && anim_id != openborconstant("ANI_TURN") ) {
                if ( anim_id != openborconstant("ANI_IDLE") ) {
                    swim_forward(player, 1);
                } else if ( anim_id == openborconstant("ANI_IDLE") ) {
                    //if ( timerdiff_idle(player) > t_frames ) swim_forward(player, 1);
                    swim_forward(player, 1);
                }

    } // fine if follow60

    else if ( anim_id == openborconstant("ANI_FOLLOW61") && getentityproperty(player, "animpos") >= 3 ) {
        swim_forward(player, 1);
    } // fine if follow61

    else if ( anim_id == openborconstant("ANI_FOLLOW60") && getentityproperty(player, "animpos") >= 3 ) {
        swim_forward(player, 0);
    } // fine if follow60

}

int check_turn(void player) {
    int anim_id = getentityproperty(player, "animationid");
    int p = getentityproperty(player, "playerindex");

    if ( anim_id == openborconstant("ANI_TURN") ) {
            // Addolciamo la gravità
            if ( getgravity(player) > MAX_GRAVITY/2 ) setgravity(player, MAX_GRAVITY/2);
            setgravity(player, getgravity(player)-getgravity(player)/25);
            setspeed(player, getspeed(player)-getspeed(player)/350);
    }

    if ( getentityproperty(player, "direction") == 0 && playerkeys(p,0,"moveright") == 8 && playerkeys(p,0,"jump") == 128 ) {
        changeplayerproperty(player, "keys", 0);
    } else if ( getentityproperty(player, "direction") == 1 && playerkeys(p,0,"moveleft") == 4 && playerkeys(p,0,"jump") == 128 ) {
        changeplayerproperty(player, "keys", 0);
    }
    //if ( timerdiff(player) >= t_frames ) setglobalvar("turn_ok_"+p, NULL());
}

int check_controls(void player) {
    int anim_id = getentityproperty(player, "animationid");
    int p = getentityproperty(player, "playerindex");
    float xdir = getentityproperty(player, "xdir");
    float tempx = xdir;

    // Velocità quando su idle premiamo left/right
    if ( anim_id != openborconstant("ANI_TURN") ) {
        if ( playerkeys(p,0,"moveright") == 8 && getentityproperty(player, "direction") == 1 ) {
            xdir += MIN_XFACTOR;
            if ( anim_id == openborconstant("ANI_IDLE") ) {
                if ( tempx < MIN_SPEED && xdir > MIN_SPEED ) xdir = MIN_SPEED;
                else if ( xdir > MIN_SPEED ) xdir = tempx;
            } else {
                if ( xdir > MAX_SPEED ) xdir = MAX_SPEED;
            }

            setspeed(player, xdir);
        } else if ( playerkeys(p,0,"moveleft") == 4 && getentityproperty(player, "direction") == 0 ) {
            xdir -= MIN_XFACTOR;
            if ( anim_id == openborconstant("ANI_IDLE") ) {
                if ( tempx > -1*MIN_SPEED && xdir < -1*MIN_SPEED ) xdir = -1*MIN_SPEED;
                else if ( xdir < -1*MIN_SPEED ) xdir = tempx;
            } else {
                if ( xdir < -1*MAX_SPEED ) xdir = -1*MAX_SPEED;
            }

            setspeed(player, xdir);
        }
    } // fine if turn

    // Diamo priorità all'animazione idle che è come se fosse walk
    if ( anim_id == openborconstant("ANI_UP") ) {
        if ( playerkeys(p,0,"moveleft") == 4 || playerkeys(p,0,"moveright") == 8 ) {
            if ( playerkeys(p,0,"moveup") != 16 ) {
                setidle(player, openborconstant("ANI_IDLE"), 1);
            }
        }
    } else if ( anim_id == openborconstant("ANI_DOWN") ) {
        if ( playerkeys(p,0,"moveleft") == 4 || playerkeys(p,0,"moveright") == 8 ) {
            if ( playerkeys(p,0,"movedown") != 32 ) {
                setidle(player, openborconstant("ANI_IDLE"), 1);
            }
        }
    }
}

int check_spawn(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_SPAWN") || anim_id == openborconstant("ANI_RESPAWN") ) {
        changeentityproperty(player, "velocity", NULL(), NULL(), 0);
    }
}

int check_speed(void player) {
    if ( getentityproperty(player, "speed") != 0 ) changeentityproperty(player, "speed", 0); // azzeriamo la velocità
}

float getspeed(void player) {
    int p = getentityproperty(player, "playerindex");

    if ( getglobalvar("xspeed_"+p) == NULL() ) setglobalvar("xspeed_"+p, 0);
    return getglobalvar("xspeed_"+p);
}

float setspeed(void player, float speed) {
    int p = getentityproperty(player, "playerindex");

    setglobalvar("xspeed_"+p, speed);
}

float getgravity(void player) {
    int p = getentityproperty(player, "playerindex");

    if ( getglobalvar("gravity_"+p) == NULL() ) setglobalvar("gravity_"+p, 0);
    return getglobalvar("gravity_"+p);
}

float setgravity(void player, float gravity) {
    int p = getentityproperty(player, "playerindex");

    setglobalvar("gravity_"+p, gravity);
}

float check_swimdir(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id != openborconstant("ANI_SHOCK") && anim_id != openborconstant("ANI_DIE") && anim_id != 68 ) {
        setentityvar(player, 11, getentityproperty(player, "direction"));
    }
}

int check_anim(void player) { // impostiamo un nuovo tempo ogni volta che cambiamo animazione
    int p = getentityproperty(player, "playerindex");

    if ( getentityproperty(player, "animationid") != getlocalvar("anim_id_"+p) ) {
        setglobalvar("timer_"+p, openborvariant("elapsed_time"));
    }
    setlocalvar("anim_id_"+p, getentityproperty(player, "animationid"));
}

int check_idle(void player) { // impostiamo un nuovo tempo ogni volta che cambiamo animazione
    int p = getentityproperty(player, "playerindex");

    if ( getentityproperty(player, "animationid") == openborconstant("ANI_IDLE") && getentityproperty(player, "animationid") != getlocalvar("idle_anim_"+p) ) {
        setglobalvar("idle_timer_"+p, openborvariant("elapsed_time"));
    }
    setlocalvar("idle_anim_"+p, getentityproperty(player, "animationid"));
}

int timerdiff_idle(void player) {
    int p = getentityproperty(player, "playerindex");

    // Check Time-Reset
    if ( check_stored_time(getglobalvar("idle_timer_"+p),9999) ) setglobalvar("idle_timer_"+p, openborconstant("MIN_INT"));
    if ( openborvariant("elapsed_time") >= getglobalvar("idle_timer_"+p) ) return (openborvariant("elapsed_time") - getglobalvar("idle_timer_"+p));
    else return -1;
}

int timerdiff(void player) {
    int p = getentityproperty(player, "playerindex");

    // Check Time-Reset
    if ( check_stored_time(getglobalvar("timer_"+p),9999) ) setglobalvar("timer_"+p, openborconstant("MIN_INT"));
    if ( openborvariant("elapsed_time") >= getglobalvar("timer_"+p) ) return (openborvariant("elapsed_time") - getglobalvar("timer_"+p));
    else return -1;
}








