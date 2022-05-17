
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/player_moves.c"
#import "data/scripts/player_specific_enemy.c"
#import "data/scripts/player_frozen.c"
#import "data/scripts/player_bubbled.c"
#import "data/scripts/player_bind_move.c"

#import "data/scripts/player_grab.c"
#import "data/scripts/player_respawn.c"
#import "data/scripts/player_offset.c"
#import "data/scripts/player_backpain.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/walkoff.c"
#import "data/scripts/ladder.c"
#import "data/scripts/climbable_area.c"
#import "data/scripts/stairs.c"
#import "data/scripts/halcove.c"

#import "data/scripts/shadowtrail.c"
#import "data/scripts/wave.c"

/*void main() {
*/

//void main() {

///void main() {

void main() {
    void self = getlocalvar("self");
    int frame = getlocalvar("frame");
    int animhandle = getlocalvar("animhandle");
    //int anim_id = gep(self,"animationid");

    // CHARACTER SPECIFICS
    //check_damage_recovery_on_jump(self,12); // (ANIMSCRIPT)

    /*if ( anim_id == oc("ANI_ATTACK1") ) {
        drawstring(10,100,0,"Aaaa");
    }*/
    //drawstring(10,100,0,"ANIMATIONSCRIPT!");

    test();
}

void test() {
    //drawstring(10,100,0,"ANIMATIONSCRIPT!");
    return;
}
