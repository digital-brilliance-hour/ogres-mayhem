/*
 * Da Ricordare: Impostare "last_player_spawned" per i players!
 * ATK_ usati: 1-5-7-8-9
 * entityvar usate (indexes): 0(getwalkingdir), 1(hp for check_splat_grab), 2(timer), 3 (grabber), 4 (backpains),
                              5 (throw) , 6 (opp x for backpain), 7 (walkingzdir), 8 (time for backpain), 9 (wallsplat dir)
                              10 (nograb), 11 (throw/grab damage), 12 (grabforce to anti-foot-grab), 13 (hook), 14 (bind grab feedback),
                              15 (prev_animid), 16 (prev_animpos), 17 (weapon launch flag), 27 (not used but custom grabbed), 28 (custom grabbed), 29 (nograb tmp)
 */

#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/enemy_moves.c"
#import "data/scripts/enemy_specific_moves.c"
#import "data/scripts/enemy_backpain.c"
#import "data/scripts/e_platform_splat.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/player_respawn.c"
#import "data/scripts/player_offset.c"
#import "data/scripts/walkoff.c"
//#import "data/scripts/stairs.c"
//#import "data/scripts/wave.c"
//#import "data/scripts/AI_ladder.c"

void main() {
  void self = getlocalvar("self");
  //float x = getentityproperty(self, "x");
  //float z = getentityproperty(self, "z");
  //float a = getentityproperty(self, "y");

    // IMPORTANTE: Attenzione all'ordine!!

    // COMMONS
    check_hole_layer(self);
    //setentityvar(self,7,getwalkingzdir(self,2));
    setentityvar(self,0,getwalkingdir(self,2)); // (ANIMSCRIPT??) for wall-splat and any
    //update_coords(self); //  <---------------------- FOR ALL

    check_safe_pos_moves(self); // (ANIMSCRIPT??)
    check_foot_grab(self,10); // (ANIMSCRIPT??)
    check_grab_pos(self); // (ANIMSCRIPT??)
    check_grabbing(self); // (ANIMSCRIPT??)
    check_antigrab(self); // (ANIMSCRIPT??)
    check_nomore_grabbing(self); // (ANIMSCRIPT??)
    //check_throw(self);
    //check_runjump(self);

    // CHARACTER SPECIFICS
    check_reset_new_grab_system(self); // (ANIMSCRIPT??)
    check_bind_grab_falling(self); // (ANIMSCRIPT??)
    //disable_moves(self); // (ANIMSCRIPT??)
    check_antigrab(self); // (ONSPAWNSCRIPT OR ANIMSCRIPT??)
    check_autoattack(self); // no ai attack at spawn

    // FOR ALL BUT SPECIFICS
    //check_falldie(self);
    setprevdir(self); // (ANIMSCRIPT??) backpains
    //setoppx(self); // backpains
    settimebp(self); // (ANIMSCRIPT??) backpains
    check_backpain(self,0); // (ANIMSCRIPT??)

    //check_tmnt3_move(self);
    check_eplatformsplat(self,0);
    check_splatfall(self);
    check_walkoff(self);

    // WEAPONS
    //check_hook_reset(self); // (ANIMSCRIPT??)
    if ( getentityproperty(self,"base") <= 0 && is_on_base(self) ) {
        check_custom_attack(self,"foot_sand","ANI_FOLLOW49",92,0.15,1); // (ANIMSCRIPT??)
        check_custom_attack(self,"board_foot_sand","ANI_FOLLOW49",92,0.15,1); // (ANIMSCRIPT??)
        check_custom_attack(self,"","ANI_FOLLOW91",92,0.15,1,"ANI_FOLLOW90");
    }
    return_to_noweapon(self); // (ANIMSCRIPT??)







    //drawstring( 10,190,0,"Var ():  "+getentityproperty(self, "subject_to_wall") );
    //drawstring( 10,190,0,"Var (weapon):  "+getentityproperty(self, "weapent") );
    //drawstring( 10,190,0,"Var (e):  "+getentityproperty(self, "z") );
    /*if ( getentityproperty(self, "playerindex") == 1 ) {
            //drawstring( 10,170,0,"Var (dir):  "+getentityproperty(self, "direction") );
            //drawstring( 10,180,0,"Var (prevdir):  "+getprevdir(self) );
            //drawstring( 10,180,0,"Var (falldie):  "+getentityproperty(self, "falldie") );
            drawstring( 10,180,0,"Var ():  "+getentityproperty(self, "aiflag", "dead") );
    }*/
    //drawstring( 10,170,0,"Var ():  "+gettimebp(self) );
    //drawstring( 10,170,0,"Var ():  "+getentityproperty(self,"aiflag","running") );
    //drawstring( 10,170,0,"Var ():  "+getentityproperty(self, "grabbing") );
    //drawstring( 10,170,0,"Var ():  "+getnograb(self) );
    //drawstring( 10,180,0,"Var ():  "+getentityproperty(self, "nograb") );
}

