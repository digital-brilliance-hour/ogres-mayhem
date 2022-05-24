
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

                    map = get_board_map(player);

                    //playsample(loadsample("data/sounds/mhead03.wav"));
                    if ( glider != NULL() ) {
                            if ( anim_id == openborconstant("ANI_UP") ) {
                                generate_bomb("dropbomb", -10,-47,0, player, glider, map);
                                setbombtime(player, get_next_time(eta));
                            } else if ( anim_id == openborconstant("ANI_DOWN") ) {
                                generate_bomb("dropbomb", -20,-10,0, player, glider, map);
                                setbombtime(player, get_next_time(eta));
                            }  else {
                                generate_bomb("dropbomb", -20,-20,0, player, glider, map);
                                setbombtime(player, get_next_time(eta));
                            }
                    }
                }// fine if time
            } // fine if animazioni
        } // fine if attack
    } // fine if stoppedent

}

void generate_bomb(void ent, float rx, float rz, float ra, void parent, void glider, int map) {
    void subent;
    float x, z, a;
      clearspawnentry();
      setspawnentry("name", ent);
      subent = spawn();

      changeentityproperty(subent, "parent", parent);
      x = getentityproperty(parent, "x");
      z = getentityproperty(parent, "z");
      a = getentityproperty(parent, "y");
      //changeentityproperty(subent, "position", x+rx, z+rz, a+ra);
      //changeentityproperty(subent, "base", a+ra);
      changeentityproperty(subent, "map", map);

      if ( glider ) bindentity(subent, glider, rx, rz, ra, 1);
      //changeentityproperty(subent, "velocity", 20, NULL(), NULL());
      //changeentityproperty(subent, "speed", 20);

    return subent;
}

int check_doublegun(void player) {
    int p = getentityproperty(player, "playerindex");

    // Check Time-Reset
    if ( check_stored_time(getentityvar(player,"gdouble_gun"),9999) ) setentityvar(player,"gdouble_gun",openborconstant("MIN_INT"));
    if ( getglobalvar("gdoublegun_"+p) == 1 && openborvariant("elapsed_time") > getentityvar(player, "gdouble_gun") ) {
        setglobalvar("gdoublegun_"+p, NULL());
    }

    if ( getentityproperty(player, "health") <= 0 && getglobalvar("gdoublegun_"+p) == 1 ) {
        setglobalvar("gdoublegun_"+p, NULL());
    }

}

int check_runback(void player) {
    int anim_id = getentityproperty(player, "animationid");
    int p = getentityproperty(player, "playerindex");
    int stall = 50;

    if ( playerkeys(p,1,"moveleft") == 4 ) {

            if ( getcombostall(player) == 0 ) {
                setcombotime(player, get_next_time(stall));
                setcombostall(player, 1);
            } else if ( getcombostall(player) == 2 && getcombotime(player) >= openborvariant("elapsed_time") ) {
                setcombostall(player, 3);
            }

            if ( getcombostall(player) == 3 ) {
                changeentityproperty(player, "aiflag", "running", 1);
                if ( getentityproperty(player, "direction") == 1 ) changeentityproperty(player, "velocity", -1*getentityproperty(player, "running", "speed"), NULL(), NULL());
                else changeentityproperty(player, "velocity", getentityproperty(player, "running", "speed"), NULL(), NULL());
                //changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW60"));
                setcombostall(player, 0);
                setcombotime(player, 0);
            }

    } else if ( playerkeys(p,2,"moveleft") == 4 ) { // button release
            if ( getcombostall(player) >= 1 && getcombostall(player) <= 2 ) {
                setcombostall(player, 2);
            } else setcombostall(player, 0);
    }

    if ( getcombotime(player) < openborvariant("elapsed_time") ) { // Resettiamo il tempo
        setcombostall(player, 0);
        setcombotime(player, 0);
    }

    /*if ( playerkeys(p,0,"moveleft") != 4 && anim_id == openborconstant("ANI_FOLLOW60") ) {
        setidle(player, openborconstant("ANI_IDLE"), 1);
        changeentityproperty(player, "aiflag", "running", 0);
        setcombostall(player, 0);
        setcombotime(player, 0);
    }*/

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

                    map = get_board_map(player);

                    if ( glider != NULL() ) {
                        playsample(loadsample("data/sounds/mhead03.wav"));
                        if ( getglobalvar("gdoublegun_"+p) != NULL() ) {
                            if ( anim_id == openborconstant("ANI_UP") ) {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW13"));
                                generate_shoot("bullet", 7,-9,0, player, map); // bullet3
                                generate_shoot("bullet", -3,-59,0, player, map,setlayer);
                                settime(player, get_next_time(eta));
                            } else if ( anim_id == openborconstant("ANI_DOWN") ) {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW14"));
                                generate_shoot("bullet", 4,-53,0, player, map,setlayer); // bullet2
                                generate_shoot("bullet", -1,2,0, player, map);
                                settime(player, get_next_time(eta));
                            }  else {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW15"));
                                generate_shoot("bullet", 7,-44,0, player, map,setlayer); // bullet2
                                generate_shoot("bullet", -8,-11,0, player, map);
                                settime(player, get_next_time(eta));
                            }
                        } else {
                            if ( anim_id == openborconstant("ANI_UP") ) {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW10"));
                                generate_shoot("bullet", 46,-30,0, player, map);
                                settime(player, get_next_time(eta));
                            } else if ( anim_id == openborconstant("ANI_DOWN") ) {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW11"));
                                generate_shoot("bullet", 44,-28,0, player, map);
                                settime(player, get_next_time(eta));
                            }  else {
                                changeentityproperty(glider, "animation", openborconstant("ANI_FOLLOW12"));
                                generate_shoot("bullet", 44,-36,0, player, map);
                                settime(player, get_next_time(eta));
                            }
                        }
                    }
                }// fine if time
            } // fine if animazioni
        } // fine if attack
    } // fine if stoppedent
}

void generate_shoot(void ent, float rx, float rz, float ra, void parent, int map, int setlayer) {
    void subent;
    float x, z, a;
      clearspawnentry();
      setspawnentry("name", ent);
      subent = spawn();

      changeentityproperty(subent, "parent", parent);
      x = getentityproperty(parent, "x");
      z = getentityproperty(parent, "z");
      a = getentityproperty(parent, "y");
      changeentityproperty(subent, "position", x+rx, z+rz, a+ra);
      changeentityproperty(subent, "base", a+ra);
      if ( map != NULL() ) changeentityproperty(subent, "map", map);
      if ( setlayer != NULL() ) changeentityproperty(subent, "setlayer", setlayer);
      //changeentityproperty(subent, "velocity", 20, NULL(), NULL());
      //changeentityproperty(subent, "speed", 20);

    return subent;
}

int check_spawn(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_SPAWN") || anim_id == openborconstant("ANI_RESPAWN") ) {
        changeentityproperty(player, "velocity", NULL(), NULL(), 0);
        changeentityproperty(player, "base", 0);
        changeentityproperty(player, "position", NULL(), NULL(), 0);
    }
}

void getstopped_ent(void player) {
    return getentityvar(player, "stop_ent");
}

void getglider(void player) {
    return getentityvar(player, "glider");
}

int setcombostall(void player, int semaphore) {
    int p = getentityproperty(player, "playerindex");

    setglobalvar("combostall_"+p, semaphore);
}

int getcombostall(void player) {
    int p = getentityproperty(player, "playerindex");

    if ( getglobalvar("combostall_"+p) == NULL() ) setglobalvar("combostall_"+p, 0);
    return getglobalvar("combostall_"+p);
}

int settime(void player, int time) {
    int p = getentityproperty(player, "playerindex");

    setglobalvar("timing_"+p, time);
}

int gettime(void player) {
    int p = getentityproperty(player, "playerindex");

    // Check Time-Reset
    if ( check_stored_time(getglobalvar("timing_"+p),9999) ) setglobalvar("timing_"+p,openborconstant("MIN_INT"));
    return getglobalvar("timing_"+p);
}

int setbombtime(void player, int time) {
    int p = getentityproperty(player, "playerindex");

    setglobalvar("bombtiming_"+p, time);
}

int getbombtime(void player) {
    int p = getentityproperty(player, "playerindex");

    //if ( getglobalvar("bombtiming_"+p) == NULL() ) setglobalvar("bombtiming_"+p, openborvariant("elapsed_time"));

    // Check Time-Reset
    if ( check_stored_time(getglobalvar("bombtiming_"+p),9999) ) setglobalvar("bombtiming_"+p,openborconstant("MIN_INT"));
    return getglobalvar("bombtiming_"+p);
}

int setcombotime(void player, int time) {
    int p = getentityproperty(player, "playerindex");

    setglobalvar("combotiming_"+p, time);
}

int getcombotime(void player) {
    int p = getentityproperty(player, "playerindex");

    // Check Time-Reset
    if ( check_stored_time(getglobalvar("combotiming_"+p),9999) ) setglobalvar("combotiming_"+p,openborconstant("MIN_INT"));
    return getglobalvar("combotiming_"+p);
}



