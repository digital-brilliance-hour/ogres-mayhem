
/*
 * Da Ricordare: Impostare "last_player_spawned" per i players!
 * ATK_ usati: 1-6-7-8-9-10
 * entityvar usate (indexes): 0(getwalkingdir), 1(time/timer/mouser/check_splat_grab), 2(combo counter), 3 (grabber), 4 (backpains),
                              5(jump on hold wall/normal atk), 6 (opp x for backpain), 7 (walkoff), 8 (time for backpain), 9 (wallsplat dir),
                              10 (jump velocity), 14 (prev anim/block), 15 (time bind move), 16 (shadowtrail), 17 (damage time/frozen/bubbled/muddied), 18 (damaged/block)
                              19 (value channel for bubbled), 20 (bind object/flame toes cancelled), 21 (prev_animpos/block), 22 (splatgrab opp), 23 (doublegun time),
                              24 (backpain_flag), 25 (grabbed_fase animpos), 26 (bind grab feedback), 27 (opp for bind grab), 28 (last damage)
                              "x","z","y","base","direction","opp","weapon","grabbed_opp","bind_toes_object","shock_no_rope_opp","opp_list"
 */

 // DA FARE:

#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/player_moves.c"
#import "data/scripts/player_specific_enemy.c"
#import "data/scripts/player_ropeshock.c"
#import "data/scripts/player_frozen.c"
#import "data/scripts/player_bubbled.c"
#import "data/scripts/player_muddied.c"
#import "data/scripts/player_roped.c"
#import "data/scripts/player_bind_move.c"
#import "data/scripts/player_bind_grab.c"

#import "data/scripts/player_grab.c"
#import "data/scripts/player_respawn.c"
#import "data/scripts/player_offset.c"
#import "data/scripts/player_backpain.c"
#import "data/scripts/wall_splat.c"
#import "data/scripts/platform_splat.c"
#import "data/scripts/walkoff.c"
//#import "data/scripts/shadowtrail.c"

//#import "data/scripts/lib_platform.c"
//#import "data/scripts/lib_movements.c"
//#import "data/scripts/keyall.c"
//#import "data/scripts/ladder.c"
//#import "data/scripts/climbable_area.c"
//#import "data/scripts/stairs.c"
//#import "data/scripts/wave.c"
//#import "data/scripts/halcove.c"
//#import "data/scripts/water_puddle.c"
//#import "data/scripts/player_actions.c"
//#import "data/chars/misc/help/press_any_button.c"


void main() {
  void self = getlocalvar("self");
  //float x = getentityproperty(self, "x");
  //float z = getentityproperty(self, "z");
  //float a = getentityproperty(self, "y");
  //float base = getentityproperty(self, "base");
  //void opp = getentityproperty(self, "opponent");
  //int refresh_time = openborvariant("elapsed_time")%2;

    // IMPORTANTE: Attenzione all'ordine!!

    // FOR ALL
    //select_screen(self);

    set_inspawning(self,RESPAWN_DISTANCE); // (ANIMSCRIPT OR RESPAWN) va messo prima di ogni set_coords <---------------------- FOR ALL
    set_coords_predeath(self,PREDEATH_COORDS_REFRESH); // (ANIMSCRIPT) memorizziamo le coordinate per il respawn... <---------------------- FOR ALL
    set_coords(self,MAX_ALTITUDE); // (ANIMSCRIPT) <---------------------- FOR ALL
    check_zmax_distance(self); // (onmovezscript) <---------------------- FOR ALL
    check_zbounds(self,ZMIN_BOUND__,ZMAX_BOUND__); // (onmovezscript) <---------------------- FOR ALL
    //update_coords(self); //  <---------------------- FOR ALL

    setentityvar(self,0,getwalkingdir(self,2)); // (ANIMSCRIPT??) for wall-splat and any
    check_general(self); // (ANIMSCRIPT) anti-bugs
    setjumpvel(self); // (ANIMSCRIPT) for multicancels
    set_prev_anim(self); // (ANIMSCRIPT) for block
    set_prev_animpos(self); // (ANIMSCRIPT) for block
    set_anim_time(self); // (ANIMSCRIPT??) bind attack
    check_hole_layer_turtles(self); // for walkoff
    check_walkoff(self, 8); // walkoff

    // FOR ALL BUT SPECIFICS
    setprevdir(self); // (ANIMSCRIPT??) backpains
    //setoppx(self); // backpains --> takedamagescript
    settimebp(self); // (ANIMSCRIPT??) backpains
    check_backpain(self,0); // (onpainscript/onfallscript) backpains
    clear_hitbyid(self); // (ANIMSCRIPT)

    // TURTLES SPECIFICS
    check_grabbed_pain(self); // (ANIMSCRIPT)
    check_grabbed_anim(self); // (ANIMSCRIPT)
    check_antigrab(self); // (ANIMSCRIPT)
    check_nomore_grabbed(self); // (ANIMSCRIPT)
    check_splatgrab(self); // (ANIMSCRIPT)
    check_splatgrab_release(self); // (ANIMSCRIPT)
    check_duck(self,NULL(),"ANI_FOLLOW90");

    // WALL/PLATFORM-SPLAT
    check_hold_platform(self); // (onblockascript??)
    check_hold_wall_anim(self); // (onblockwscript??)
    check_platformsplat(self,0); // (onblockascript??)
    check_splatfall(self);

    // CHARACTER SPECIFICS
    check_damage_recovery_on_jump(self,12); // (ANIMSCRIPT)
    check_pos_moves(self);
    check_mytoes(self); // (ANIMSCRIPT??)
    //check_blockpain(self); // (ANIMSCRIPT??)
    jump_attack_multicancel(self);
    check_mouser_attack(self); // (ANIMSCRIPT??)
    check_ropeshock(self); // (ANIMSCRIPT??)
    check_frozen(self); // (ANIMSCRIPT??)
    check_bubbled(self); // (ANIMSCRIPT??)
    check_muddied(self); // (ANIMSCRIPT??)
    check_roped(self); // (ANIMSCRIPT??)
    check_pizzapow(self); // (ANIMSCRIPT??)
    check_forwardkick(self);
    disable_keys_for_moves(self); // (KEYALL??)
    check_bind_move(self); // (ANIMSCRIPT??)
    check_bind_grab(self); // (ANIMSCRIPT??)
    check_attack_shift(self); // (KEYALL??) <-- OPTIONAL
    check_specific_turtle_moves(self);


    // TESTS
    //check_step_stair(self,10);
    //draw_clipping(self, 70, 20, 0xDA3300);
    //clip_character(self, 70, 15);






    //drawstring( 10,200,0,"Var(edge): "+gep(self,"edge"));

    //changeanimationproperty(self,oc("ANI_IDLE"),"vulnerable",0,0);
    //drawstring(10,50,0,"a: "+getanimationproperty(self,oc("ANI_IDLE"),"vulnerable",0));
    //drawstring(10,50,0,"a: "+getanimationproperty(self,oc("ANI_IDLE"),"attack","dropv","velocity.x",0));
    //drawstring(10,50,0,"a: "+getanimationproperty(self,oc("ANI_IDLE"),"move","x",0));

    //drawstring(10,50,0,"a: "+getanimationproperty(self,gep(self,"animationid"),"delay",0));
    //if (gep(self,"animpos")==0) playsample(getanimationproperty(self,oc("ANI_ATTACK1"),4));

    //drawstring(10,50,0,"a: "+getanimationproperty(self,gep(self,"animationid"),"numframes"));

    //if ( getentityproperty(self,"playerindex") == 0 ) show_enemies();
    //if ( getentityproperty(self,"playerindex") == 0 ) destroy_all_enemies(gep(self,"playerindex"),1);
    //if ( getentityproperty(self,"playerindex") == 0 ) autokill_keyc(gep(self,"playerindex"),"ATTACK","ATK_NORMAL",0);

    //drawstring( 10,220,0,"Var(incam): "+is_incam_coords(gep(self,"x"),gep(self,"z"),gep(self,"a"),gep(self,"base"),T_CAM));
    //drawstring( 10,220,0,"Var(): "+getglobalvar("test"));
    //drawstring( 10,20,0,"var: "+openborvariant("waiting"));
    //drawstring( 10,20,0,"var: "+getentityproperty(self,"rush_tally") );
    //drawstring(10,100,0,"val: "+getlocalvar("bezier_time"));
    //if ( !is_in_pain(self) && getentityproperty(self,"aiflag","attacking") && (playerkeys(0,0,"attack") || playerkeys(0,0,"special")) )
    //drawstring( 240,230,0,"waiting: "+openborvariant("waiting") );
    //drawstring( 10,180,0,"x: "+getentityproperty(self,"x")+"\nz: "+getentityproperty(self,"z") );
    //drawstring( 10,180,0,"base: "+getentityproperty(self,"base")+"\ny: "+getentityproperty(self,"y") );
    /*if ( getentityproperty(self,"x")>100 && getentityproperty(self,"x")<3100 && openborvariant("in_level") && getlevelproperty("type") != 2 ) {
        changeentityproperty(self,"position",3400,380,NULL());
        changeopenborvariant("xpos", 3200);
        changeopenborvariant("ypos", 320);
    }*/
    /*if ( getentityproperty(self,"x")>100 && getentityproperty(self,"x")<5000 && openborvariant("in_level") && getlevelproperty("type") != 2 ) {
        changeentityproperty(self,"position",5400,780,NULL());
        changeopenborvariant("xpos", 5100);
        changeopenborvariant("ypos", 700);
    }*/
    //drawstring( 10,200,0,find_platform_in_a(getentityproperty(self, "x")+5,getentityproperty(self, "z"),getentityproperty(self, "y"),MAX_ALTITUDE));
    //if ( getentityproperty(self,"opponent") ) drawstring( 10,190,0,"Var (): "+getentityproperty(getentityproperty(self,"opponent"), "hitbyid"));
    //drawstring( 10,200,0,getentityproperty(getentityproperty(self, "opponent"), "grabbing"));
    //drawstring( 10,190,0,"Var (animid): "+getentityproperty(self, "animationid"));
    //drawstring( 10,200,6,"HOW TO PLAY");
    //draw_shadowtrail(self, 7, 15, 20, 160, 0);
    //drawstring( 10,170,0,"Var (opp):  "+getentityproperty(self,"opponent") );
    //drawstring( 10,180,0,"Var (stalltime):  "+getentityproperty(self, "stalltime") );
    //drawstring( 10,180,0,"Var ():  "+playerkeys(0,0,"jump", "attack") );
    //drawstring( 10,180,0,"Var (WalkDir):  "+getentityvar(self,0) );
    //if ( getentityproperty(self,"playerindex") == 0 ) drawstring( 10,100,0,"grabbed: "+getentityvar(self,22));
    //if ( playerkeys(getentityproperty(self, "playerindex"),1,"attack") && getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 ) damageentity(self,self,999,1,openborconstant("ATK_NORMAL16"));
    //if ( playerkeys(0,0,"attack") ) damageentity(self,self,999,1,openborconstant("ATK_NORMAL"));
    /*drawstring( 10,190,0,"Var(comboETA): "+ getentityvar(self,"__combo01_comboETA") );
    drawstring( 10,200,0,"Var(combostep): "+ getentityvar(self,"__combo01_combostep") );
    drawstring( 10,210,0,"Var(comboRB): "+ getentityvar(self,"__combo01_comboRB") );
    drawstring( 10,220,0,"Var(comboRB): "+ getentityvar(self,"__combo_prevdir") );
    //drawstring( 10,230,0,"Var(): "+playerkeys_multi(0,0,"forward+up") );*/
    /*drawstring( 10,50,0,"Var(aa): "+getglobalvar("aa"));
    drawstring( 10,60,0,"Var(z): "+gep(self,"z"));
    drawstring( 10,70,0,"Var(base): "+gep(self,"base"));
    drawstring( 10,80,0,"Var(ypos): "+(ov("ypos")+20));*/
}

void ondestroy() {
    destroy_hitbyid_list("opp_list");
}
