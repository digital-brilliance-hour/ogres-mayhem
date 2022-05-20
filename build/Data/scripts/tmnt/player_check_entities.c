
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/water_puddle.c"
#import "data/scripts/ladder.c"
#import "data/scripts/climbable_area.c"
#import "data/scripts/halcove.c"
#import "data/scripts/stairs.c"
#import "data/scripts/wave.c"
#import "data/scripts/player_bind_move.c" // delete for lib_interact_ent

void check_interact_entities(void self, int no_ladder, int no_stair, int no_wave, int no_water_puddle) {
    if ( (getlocalvar("__interact_ent_size") == NULL() && get_interact_ent_semaphore(self) == 0) || get_interact_ent_semaphore(self) == 3 ) {
        int i = 0, c = 0;

        if ( get_interact_ent_semaphore(self) == 3 ) reset_interact_ent();

        //if ( openborvariant("elapsed_time") < openborvariant("game_speed")/20 ) return;
        for (i = 0; i < openborvariant("count_entities"); ++i) { // openborconstant("MAX_ENTS")
            void ent = getentity(i);

            if ( getentityproperty(ent, "exists") ) {
                char ent_type = getentityvar(ent, "type");

                if ( ((ent_type == "wave_r" || ent_type == "wave_l" || ent_type == "wave_c") && (no_wave == NULL() || no_wave <= 0))
                   || ((ent_type == "stair_r" || ent_type == "stair_l" || ent_type == "stair_q" || ent_type == "stair_c") && (no_stair == NULL() || no_stair <= 0))
                   || ((ent_type == "ladder_r" || ent_type == "ladder_l" || ent_type == "ladder_v" || ent_type == "ladder_h" || ent_type == "climbing_area" || ent_type == "halcove") && (no_ladder == NULL() || no_ladder <= 0))
                   || ((ent_type == "water_puddle") && (no_water_puddle == NULL() || no_water_puddle <= 0)) ) {
                       setlocalvar("__interact_ent"+(c++),ent);
                }
            }
        }
        setlocalvar("__interact_ent_size",c);
        set_interact_ent_semaphore(self,1);
    } else if ( get_interact_ent_semaphore(self) == 2 ) {
        reset_interact_ent();
        set_interact_ent_semaphore(self,0);
    }

    return;
}

int reset_global_interact_ents(int reset_init_flag, int force_flag) {
    int i = 0, ret_flag = 0, semaphore = 2;

    if ( force_flag == NULL() ) force_flag = 0;
    if ( reset_init_flag == NULL() || reset_init_flag <= 0 ) semaphore = 2;
    else semaphore = 3;
    for (i = 0; i < openborvariant("count_entities"); ++i) {
        void ent = getentity(i);

        if ( getentityproperty(ent, "exists") ) {
            if ( get_interact_ent_semaphore(ent) == 1 || force_flag ) {
                set_interact_ent_semaphore(ent,semaphore);
                if ( !ret_flag ) ret_flag = 1;
            }
        }
    }

    return ret_flag;
}

int get_interact_ent_size() {
    int size = getlocalvar("__interact_ent_size");

    if ( size == NULL() ) size = 0;

    return size;
}

void get_interact_ent(int index) {
    return getlocalvar("__interact_ent"+index);
}

void reset_interact_ent() {
    int i = 0;

    for (i = 0; i < get_interact_ent_size(); ++i) {
        setlocalvar("__interact_ent"+i,NULL());
    }
    setlocalvar("__interact_ent_size",NULL());
}

// 0 == not initialized, 1 == all interact ents found, 2 = reset all interact ents, 3 == reset all + init
int get_interact_ent_semaphore(void self) {
    if ( getentityvar(self,"interact_ent_semaphore") == NULL() ) setentityvar(self,"interact_ent_semaphore",0);
    return getentityvar(self,"interact_ent_semaphore");
}

void set_interact_ent_semaphore(void self, int value) {
    setentityvar(self,"interact_ent_semaphore",value);
}

int check_entitiesA(void self) {
    int i = 0;
    int stair_found_flag = 0, stair_is_on = 0, tmp_stair_is_on = 0;
    int wave_found_flag = 0, wave_is_on = 0;
    int water_puddle_found_flag = 0, water_puddle_is_on = 0;

    if ( getglobalvar("in_menu") != 1 && openborvariant("in_level") ) { // Waiting, Select
        if ( getlevelproperty("type") != 2 ) {

            check_interact_entities(self);
            for (i = 0; i < get_interact_ent_size(); ++i) { // openborvariant("count_entities") --- openborconstant("MAX_ENTS")
                void ent = get_interact_ent(i);

                if ( getentityproperty(ent, "exists") ) {
                    char ent_type = getentityvar(ent, "type");

                    /// WAVE ///
                    if ( ent_type == "wave_r" || ent_type == "wave_l" || ent_type == "wave_c" ) {
                        wave_found_flag = 1;
                        if ( !wave_is_on || wave_is_on == 2 ) wave_is_on = check_wave(self, ent);
                        //if (wave_is_on == 1) break;
                    } // fine if wave

                    /// STAIR ///
                    if ( ent_type == "stair_r" || ent_type == "stair_l" || ent_type == "stair_q" ) {
                        stair_found_flag = 1;
                        if ( !stair_is_on || stair_is_on == 2 ) {
                            tmp_stair_is_on = check_side_stairs(self, ent);
                            if ( tmp_stair_is_on ) stair_is_on = tmp_stair_is_on;
                        }
                        //if (stair_is_on == 1) break;
                    } else if ( ent_type == "stair_c" ) {
                        stair_found_flag = 1;
                        if ( !stair_is_on || stair_is_on == 2 ) {
                            tmp_stair_is_on = check_frontal_stairs(self, ent);
                            if ( tmp_stair_is_on ) stair_is_on = tmp_stair_is_on;
                        }
                        //if (stair_is_on == 1) break;
                    } // fine if stair

                    /// WATER PUDDLE ///
                    if ( ent_type == "water_puddle" ) {
                        int anim_id = getentityproperty(self,"animationid");
                        int water_puddle_flag = 0;

                        water_puddle_found_flag = 1;
                        if ( anim_id == openborconstant("ANI_SPAWN") || anim_id == openborconstant("ANI_RESPAWN") ) {
                            int anim_pos = getentityproperty(self,"animpos");
                            if ( anim_pos <= 1 ) water_puddle_flag = 1;
                        }
                        if ( !water_puddle_flag && !water_puddle_is_on ) water_puddle_is_on = check_water_puddle(self, ent);
                        if ( !water_puddle_is_on && getlocalvar("water_puddle_found") != 1 ) setlocalvar("water_puddle_found",1);
                    }

                    /// LADDER/CLIMBABLE AREA/HALCOVE ///
                    if ( get_bindtype(self) == NULL() ) check_ladder(self,ent);
                    if ( get_bindtype(self) == NULL() ) check_climbing(self,ent);
                    if ( get_bindtype(self) == NULL() ) check_halcove(self,ent);
                } // fine if exists
            } // fine for all_ents

            /// WAVE RESET ///
            if ( !wave_found_flag || wave_is_on <= 0 ) {
                reset_wave(self);
            }

            /// STAIR RESET ///
            if ( !stair_found_flag || stair_is_on <= 0 ) {
                reset_stairs(self);
            }

            /// WATER PUDDLE RESET ///
            if ( !water_puddle_found_flag || water_puddle_is_on <= 0 ) {
                reset_water_puddle(self);
            }
        } // fine if level prop
    }
}
