
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/player_check_entities.c"

#import "data/scripts/water_puddle.c"
#import "data/scripts/ladder.c"
#import "data/scripts/climbable_area.c"
#import "data/scripts/halcove.c"
//#import "data/scripts/stairs.c"
//#import "data/scripts/wave.c"
#import "data/scripts/player_moves.c"
#import "data/scripts/player_bind_move.c"


void main() {
  void self = getlocalvar("self");

    // SINGLE CHARACTER SPECIFIC
    check_specific_turtle_shadow_moves(self); // (ANIMSCRIPT)
    check_shadow_trail(self);

    // CLIMBING AND ENTITIES COUNT FUNCS
    check_entitiesA(self);
    //if ( get_bindtype(self) == NULL() ) check_ladder(self);
    if ( get_bindtype(self) == NULL() ) check_climb(self);
    //if ( get_bindtype(self) == NULL() ) check_climbing(self);
    if ( get_bindtype(self) == NULL() ) check_climb_area(self);
    //if ( get_bindtype(self) == NULL() ) check_halcove(self);
    if ( get_bindtype(self) == NULL() ) check_climb_halcove(self);
    //check_wave_ents(self);
    //check_stair(self);
    //check_step_stair(self, 2);
    if ( getlocalvar("water_puddle_found") == 1 ) check_ripple(self);

    //check_bind_animation(self); // (ANIMSCRIPT) Controlli d'animazione della mossa
}

