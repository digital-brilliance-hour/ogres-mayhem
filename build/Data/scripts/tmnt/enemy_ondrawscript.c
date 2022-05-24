
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/player_check_entities.c"

#import "data/scripts/water_puddle.c"
#import "data/scripts/ladder.c"
#import "data/scripts/climbable_area.c"
#import "data/scripts/stairs.c"
#import "data/scripts/wave.c"

#import "data/scripts/AI_ladder.c"
#import "data/scripts/AI_ladder_climbing.c"

void main() {
    void self = getlocalvar("self");
    int type = getentityproperty(self,"type");

    //drawstring(10,220,0,"near_a_ladder: "+glv("near_a_ladder"));

    // CLIMBING AND ENTITIES COUNT FUNCS
    check_entities(self);

    if ( type == openborconstant("TYPE_PLAYER") ) {
        check_climb(self);
        check_climb_area(self);
        //check_climb_halcove(self);
    } else if ( type == openborconstant("TYPE_ENEMY") ) {
        int game_speed = ov("game_speed");

        if ( glv("near_a_ladder") == NULL() ) {
            //if ( time_to_wait("check_AI_ladder_refresh",0) ) {
            if ( time_to_wait("check_AI_ladder_refresh",game_speed/2) ) {
                check_AI_ladder(self);
                reset_time_to_wait("check_AI_ladder_refresh");
            }
        }
        //drawstring(10,220,0,"ladder: "+glv("near_a_ladder"));

        goto_ladder(self);
        //goto_ladder_waypoints(self);
        check_AI_climb(self);
    }

    if ( getlocalvar("water_puddle_found") == 1 ) check_ripple(self);
}

int check_entities(void self) {
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
                    int type = getentityproperty(self,"type");
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
                    if ( type == openborconstant("TYPE_PLAYER") ) {
                        check_ladder(self,ent);
                        check_climbing(self,ent);
                        //check_halcove(self,ent);
                    }
                } // fine if exists
            } // fine for all_ents

            /// WAVE ///
            if ( !wave_found_flag || wave_is_on <= 0 ) {
                reset_wave(self);
            }

            /// STAIR ///
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

int is_foot(void self) {
    if ( getentityproperty(self,"exists") ) {
        char cText = getentityproperty(self, "defaultmodel");
        char cFind = "foot";
        char cFind2 = "FOOT";
        char substr = strinfirst(cText, cFind);
        char substr2 = strinfirst(cText, cFind2);

        if ( substr != -1 || substr2 != -1 ) {
            return 1;
        } else return 0;
    } else return 0;
}

