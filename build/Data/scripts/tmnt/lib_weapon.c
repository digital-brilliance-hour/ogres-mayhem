
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/player_check_entities.c"

#import "data/scripts/water_puddle.c"
#import "data/scripts/stairs.c"
#import "data/scripts/wave.c"

void main() {
  void self = getlocalvar("self");

    // ENTITIES COUNT FUNCS
    check_effects(self);

    //drawstring( 10,100,0,"Var():  "+is_on_stair());
}

int check_effects(void self) {
    check_entities(self);
    if ( getlocalvar("water_puddle_found") == 1 ) check_ripple(self);
}

int check_entities(void self) {
    int i = 0;
    int stair_found_flag = 0, stair_is_on = 0, tmp_stair_is_on = 0;
    int wave_found_flag = 0, wave_is_on = 0;
    int water_puddle_found_flag = 0, water_puddle_is_on = 0;

    if ( getglobalvar("in_menu") != 1 && openborvariant("in_level") ) { // Waiting, Select
        if ( getlevelproperty("type") != 2 ) {

            check_interact_entities(self,1,0,0,0);
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
        }
    }
}
