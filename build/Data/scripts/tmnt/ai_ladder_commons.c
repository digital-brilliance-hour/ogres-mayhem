
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/ladder_commons.c"

#define THRESHOLD_LADDER_X openborvariant("hresolution")/1
#define THRESHOLD_LADDER_Z (openborvariant("scrollmaxz")-openborvariant("scrollminz"))/1
#define T_LADDER_RANGE 5
#define T_TARGET_RANGE 1
#define T_LADDER_ALT_VISIBILITY 10

// se ai_flag == 1 e non c'è ai control resetta oppure se c'è ai_control ma non ai_flag resetta
int AI_ladder_reset_cond(void self, int noai_flag) {
    if ( glv("near_a_ladder") == 1 ) {

        if ( !gep(self,"noaicontrol") && (noai_flag == NULL() || noai_flag == 0) ) return 0;
        else if ( gep(self,"noaicontrol") && noai_flag >= 1 ) return 0;

        if ( is_attacking(self) || is_in_pain(self) ) {
            AI_ladder_reset(self);
        }
    }

    return 0;
}

int AI_ladder_reset(void self, int ladder_flag, int ladder_h_flah) {
    // reset
    cep(self,"noaicontrol",0);
    cep(self,"opponent",NULL());
    //cep(self,"velocity",0,0,0);
    slv("blockade_num",NULL());
    slv("near_a_ladder",NULL());
    if ( ladder_flag != NULL() && ladder_flag == 1 ) sev(self,"ladder",NULL());
    sev(self,"setlayer",NULL());
    slv("action_flag",NULL());

    slv("flagged_xdir",NULL());
    slv("flagged_zdir",NULL());

    slv("x_dist_climb",NULL());
    slv("y_dist_climb",NULL());
    slv("xdir_climb",NULL());
    slv("ydir_climb",NULL());
    slv("target_x_climb",NULL());
    slv("target_y_climb",NULL());
    slv("climb_target_flag",NULL());
    slv("climb_no_target_flag",NULL());
    slv("climb_obstacle",NULL());
    slv("climb_obstacle_solved",NULL());
    if ( ladder_h_flah != NULL() && ladder_h_flah == 1 ) slv("ladder_h_ent",NULL());

    slv("dir_timeout",NULL());
    changeentityproperty(self, "tosstime", 0);
    waypoints(self);

    if ( is_subject_to_obstacles() ) {
        changeentityproperty(self,"subject_to_wall",1);
        changeentityproperty(self,"subject_to_platform",1);
    }
}

int set_action(char label) {
    int action_flag = 0;
    int flag_value = retrieve_flag_value_pos(label);

    if ( label == "none" ) action_flag = 0;
    else action_flag = add_flag(action_flag,flag_value);

    setlocalvar("action_flag",action_flag);
}

int get_action(char label) {
    int action_flag = getlocalvar("action_flag");
    int flag_value = retrieve_flag_value_pos(label);

    return get_flag_value(action_flag,flag_value);
}

int add_action(char label) {
    int action_flag = getlocalvar("action_flag");
    int flag_value = retrieve_flag_value_pos(label);

    action_flag = add_flag(action_flag,flag_value);
    setlocalvar("action_flag",action_flag);

    return action_flag;
}

int remove_action(char label) {
    int action_flag = getlocalvar("action_flag");
    int flag_value = retrieve_flag_value_pos(label);

    action_flag = remove_flag(action_flag,flag_value);
    setlocalvar("action_flag",action_flag);

    return action_flag;
}

int retrieve_flag_value_pos(char label) {
    // 2^0 =1; 2^1 = 2; 2^2 = 4; 2^3 = 8; 2^4 = 16; 2^5 = 32; 2^6 = 64; 2^7 = 128; 2^8 = 256;
    int flag_value = 0;

         if ( label == "start"     ) flag_value = 1;
    else if ( label == "moveleft"  ) flag_value = 2;
    else if ( label == "moveright" ) flag_value = 3;
    else if ( label == "moveup"    ) flag_value = 4;
    else if ( label == "movedown"  ) flag_value = 5;
    else if ( label == "attack"    ) flag_value = 6;
    else if ( label == "jump"      ) flag_value = 7;
    else if ( label == "special"   ) flag_value = 8;

    return flag_value;
}

int check_subjects(void self) {
    if ( is_subject_to_obstacles() ) {
        if ( getentityproperty(self, "subject_to_wall") ) changeentityproperty(self,"subject_to_wall",0);
        if ( getentityproperty(self, "subject_to_platform") ) changeentityproperty(self,"subject_to_platform",0);
    }
}

int get_ladder_alt_visibility() {
    return T_LADDER_ALT_VISIBILITY;
}

