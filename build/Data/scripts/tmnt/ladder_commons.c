
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

#define GRAB_LADDER_BTN "special"
#define SUBJECT_TO_OBSTACLES 0
#define T_DIST 18
#define T_LAYER 200
#define CHANGE_LAYER_FLAG 0


int is_action_btn_pressed_ladder_by_up(int p_index) {
    return playerkeys(p_index,1,get_ladder_btn());
}

int is_action_btn_pressed_ladder_by_down(int p_index) {
    return playerkeys(p_index,1,get_ladder_btn());
}

int is_action_btn_pressed_ladder_by_air(int p_index) {
    return playerkeys(p_index,1,get_ladder_btn());
}

char get_ladder_btn(void btn) {
    if( btn != NULL() ) return btn;
    else return GRAB_LADDER_BTN;
}

int jump(void player) {
    if ( !getentityproperty(player,"aiflag","dead") ) performattack(player, openborconstant("ANI_JUMP"), 1);
    changeentityproperty(player, "aiflag", "jumping", 1);
    changeentityproperty(player, "takeaction", "common_jump", 1);
    playsample(loadsample("data/sounds/jump.wav"));
    tossentity(player, getentityproperty(player, "jumpheight"),0,0);
}

int jump_from_climbing(void player) {
    /*changeentityproperty(player, "aiflag", "jumping", 0);
    performattack(player, openborconstant("ANI_WALKOFF"), 1);
    set_falling_dir(player);*/

    changeentityproperty(player,"animation",oc("ANI_WALKOFF"));
    changeentityproperty(player, "takeaction", "common_jump", 1);
    changeentityproperty(player, "aiflag", "jumping", 1);
    //changeentityproperty(player, "aiflag", "jumpid", 2);

    return 1;
}

int set_falling_dir(void player) {
    int p = getentityproperty(player, "playerindex");
    float speed = getentityproperty(player, "speed");
    float xdir = NULL(), zdir = NULL();

    if ( playerkeys(p,0,"moveup") == 16 ) zdir = -1*(speed/2);
    else if ( playerkeys(p,0,"movedown") == 32 ) zdir = speed/2;
    if ( playerkeys(p,0,"moveright") == 8 ) xdir = speed;
    else if ( playerkeys(p,0,"moveleft") == 4 ) xdir = -1*speed;

    changeentityproperty(player, "velocity", xdir, zdir, NULL());
}

int reset_jumping(void self, int tossv_flag) {
    if ( getentityproperty(self, "aiflag", "animating") && getentityproperty(self, "tossv") < 0 && getentityproperty(self, "tosstime") != 0 ) { // && !is_in_pain(self)
        //changeentityproperty(self, "aiflag", "animating", 0); // <--- NO GOOD!

        if ( getentityproperty(self, "y") != 0 ) changeentityproperty(self, "position", NULL(),NULL(),0); // because 0 == base
    }
    //if ( getentityproperty(self, "aiflag", "falling") ) changeentityproperty(self, "aiflag", "falling", 0); // <--- NO GOOD!

    if ( getentityproperty(self, "tossv") < 0 && !tossv_flag ) changeentityproperty(self, "velocity", NULL(),NULL(),0);
    if ( getentityproperty(self, "tosstime") != 0 ) changeentityproperty(self, "tosstime", 0);
    if ( getentityproperty(self, "aiflag", "jumping") ) changeentityproperty(self, "aiflag", "jumping", 0);
}

int player_ladder_reset(void player) {
    int p = getentityproperty(player, "playerindex");

    if ( getentityproperty(player, "setlayer") != getglobalvar("p"+p+"layer") ) changeentityproperty(player, "setlayer", getglobalvar("p"+p+"layer"));
    setglobalvar("p"+p+"ladder", NULL());
    setentityvar(player,"ladder",NULL());

    if ( is_subject_to_obstacles() ) {
        changeentityproperty(player,"subject_to_wall",1);
        changeentityproperty(player,"subject_to_platform",1);
    }
}

int check_subjects(void player) {
    if ( is_subject_to_obstacles() ) {
        if ( getentityproperty(player, "subject_to_wall") ) changeentityproperty(player,"subject_to_wall",0);
        if ( getentityproperty(player, "subject_to_platform") ) changeentityproperty(player,"subject_to_platform",0);
    } else {
        if ( !getentityproperty(player, "subject_to_wall") ) changeentityproperty(player,"subject_to_wall",1);
        if ( !getentityproperty(player, "subject_to_platform") ) changeentityproperty(player,"subject_to_platform",1);
    }
}

int is_subject_to_obstacles() {
    return SUBJECT_TO_OBSTACLES;
}

int get_T_DIST() {
    int minz = openborvariant("player_min_z");
    int maxz = openborvariant("player_max_z");

    if ( is_subject_to_obstacles() || minz == maxz ) return 0;
    else return T_DIST;
}

int get_T_LAYER() {
    return T_LAYER;
}

int get_CHANGE_LAYER_FLAG() {
    return CHANGE_LAYER_FLAG;
}

int get_T_TOSSTIME() {
    return get_next_time(openborvariant("game_speed")/10);
}
