/*
 * Da Ricordare: Impostare "last_player_spawned" per i players!
 * ATK_ usati: 1-5-7-8-9
 * entityvar usate (indexes): 0(getwalkingdir), 1(hp for check_splat_grab), 2(timer), 3 (grabber), 4 (backpains),
                              5 (throw) , 6 (opp x for backpain), 7 (walkingzdir), 8 (time for backpain), 9 (wallsplat dir)
                              10 (nograb), 11 (throw/grab damage), 12 (grabforce to anti-foot-grab), 13 (hook), 14 (bind grab feedback),
                              15 (prev_animid), 16 (prev_animpos), 27 (not used but custom grabbed), 28 (custom grabbed), 29 (nograb tmp)
                              "x","z","y","base","direction"
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
//#import "data/scripts/ladder.c"
//#import "data/scripts/climbable_area.c"
//#import "data/scripts/stairs.c"
//#import "data/scripts/wave.c"

void main() {
  void self = getlocalvar("self");
  //float x = getentityproperty(self, "x");
  //float z = getentityproperty(self, "z");
  //float a = getentityproperty(self, "y");

    // IMPORTANTE: Attenzione all'ordine!!

    // COMMONS
    set_inspawning(self,RESPAWN_DISTANCE); // va messo prima di ogni set_coords <---------------------- FOR ALL
    set_coords_predeath(self,PREDEATH_COORDS_REFRESH); // memorizziamo le coordinate per il respawn... <---------------------- FOR ALL
    set_coords(self,MAX_ALTITUDE); // <---------------------- FOR ALL
    check_zmax_distance(self); // <---------------------- FOR ALL
    check_zbounds(self,ZMIN_BOUND__,ZMAX_BOUND__); //  <---------------------- FOR ALL
    //update_coords(self); //  <---------------------- FOR ALL

    //setentityvar(self,7,getwalkingzdir(self,2));
    setentityvar(self,0,getwalkingdir(self,2));
    set_prev_anim(self);
    set_prev_animpos(self);
    check_hole_layer(self);

    // CHARACTER SPECIFICS
    check_safe_pos_moves(self);
    check_foot_grab(self,10); // to grab ONLY the turtles
    check_grab_pos(self); // always opp z-1
    check_grabbing(self);
    check_antigrab(self); // no grab if globalvar...
    check_nomore_grabbing(self); // reset dalla presa del soldato
    //check_throw(self);
    //check_runjump(self);
    disable_keys_for_moves(self); // (ANIMSCRIPT?? OR KEYSCRIPT??)
    //disable_moves(self);
    check_duck(self,NULL(),"ANI_FOLLOW90");

    check_reset_new_grab_system(self);
    check_bind_grab_falling(self);
    check_attack_shift(self); // <-- OPTIONAL

    // FOR ALL BUT SPECIFICS
    //check_falldie(self);
    setprevdir(self); // backpains
    //setoppx(self); // backpains --> takedamagescript
    settimebp(self); // backpains
    check_backpain(self,0); // backpains

    //check_tmnt3_move(self);
    check_eplatformsplat(self,0);
    check_splatfall(self);
    check_walkoff(self);

    // PLAYER SPECIFICS (WEAPONS)
    check_rifle_atk(self,-1,0);
    check_fan_atk(self,3,0);
    check_lasergun_atk(self,-1,0);
    //check_hook_reset(self);
    return_to_noweapon(self);








    //show_enemies();
    //destroy_all_enemies(self,0);
    //drawstring( 10,170,0,"Var ():  "+getentityproperty(self,"noaicontrol") );
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

int check_lasergun_atk(void self, int max_shots, int hold_flag) {
        int anim_id = gep(self,"animationid");

        if ( anim_id == oc("ANI_FREESPECIAL20") || anim_id == oc("ANI_FREESPECIAL21") ) {
            int p = gep(self, "playerindex");
            int animpos = gep(self, "animpos");
            char modelname = gep(self,"model");

            if ( getentityproperty(self,"dead") ) return;

            if ( strinfirst("player_foot_lasergun", modelname) != -1 ) {

                //drawstring(10,100,0,"aaa");
                    if ( max_shots == NULL() ) max_shots = -1;

                    // Azzeriamo il conto degli spari
                    if ( glv("shot_count") == NULL() || anim_id == oc("ANI_FREESPECIAL20") ) slv("shot_count",0);

                    if ( playerkeys(p,0,"moveleft") ) {
                        if ( gep(self,"direction") != 0 ) cep(self,"direction",0);
                    } else if ( playerkeys(p,0,"moveright") ) {
                        if ( gep(self,"direction") != 1 ) cep(self,"direction",1);
                    }

                    if ( glv("shot_count") < max_shots-1 || max_shots == -1 ) {
                        if ( anim_id == oc("ANI_FREESPECIAL20") && animpos >= 9 && animpos <= 9 ) {
                            if ( playerkeys(p,hold_flag,"attack") ) performattack(self,oc("ANI_FREESPECIAL21"),1);

                            if ( playerkeys(p,hold_flag,"attack") && max_shots > 0 ) {
                                slv("shot_count",glv("shot_count")+1);
                            }
                        } else if ( anim_id == oc("ANI_FREESPECIAL21") && animpos >= 5 && animpos <= 5 ) {
                            if ( playerkeys(p,hold_flag,"attack") ) performattack(self,oc("ANI_FREESPECIAL21"),1);

                            if ( playerkeys(p,hold_flag,"attack") && max_shots > 0 ) {
                                slv("shot_count",glv("shot_count")+1);
                            }
                        }
                    } // fine if max_count
            } // fine if modelname
        } // fine if anim_id
}


int check_fan_atk(void self, int max_shots, int hold_flag) {
        int anim_id = gep(self,"animationid");

        if ( anim_id == oc("ANI_FREESPECIAL20") || anim_id == oc("ANI_FREESPECIAL21") || anim_id == oc("ANI_FREESPECIAL22") || anim_id == oc("ANI_FREESPECIAL23")
            || anim_id == oc("ANI_FREESPECIAL24") || anim_id == oc("ANI_FREESPECIAL25")
            || anim_id == oc("ANI_FREESPECIAL30") || anim_id == oc("ANI_FREESPECIAL31") || anim_id == oc("ANI_FREESPECIAL32") || anim_id == oc("ANI_FREESPECIAL33")
            || anim_id == oc("ANI_FREESPECIAL34") || anim_id == oc("ANI_FREESPECIAL35") ) {
            int p = gep(self, "playerindex");
            int animpos = gep(self, "animpos");
            char modelname = gep(self,"model");

            if ( getentityproperty(self,"dead") ) return;

            if ( strinfirst("player_foot_fan", modelname) != -1 ) {

                //drawstring(10,100,0,"aaa");
                    if ( max_shots == NULL() ) max_shots = -1;
                    if ( hold_flag == NULL() ) hold_flag = 1;

                    // Azzeriamo il conto degli spari
                    if ( glv("shot_count") == NULL() || anim_id == oc("ANI_FREESPECIAL20") || anim_id == oc("ANI_FREESPECIAL21")
                        || anim_id == oc("ANI_FREESPECIAL22") || anim_id == oc("ANI_FREESPECIAL23") || anim_id == oc("ANI_FREESPECIAL24")
                        || anim_id == oc("ANI_FREESPECIAL25") ) slv("shot_count",0);

                    if ( playerkeys(p,0,"moveleft") ) {
                        if ( gep(self,"direction") != 0 ) cep(self,"direction",0);
                    } else if ( playerkeys(p,0,"moveright") ) {
                        if ( gep(self,"direction") != 1 ) cep(self,"direction",1);
                    }

                    if ( glv("shot_count") < max_shots-1 || max_shots == -1 ) {
                        if ( (anim_id == oc("ANI_FREESPECIAL20") || anim_id == oc("ANI_FREESPECIAL21") || anim_id == oc("ANI_FREESPECIAL22") || anim_id == oc("ANI_FREESPECIAL23")
                            || anim_id == oc("ANI_FREESPECIAL24") || anim_id == oc("ANI_FREESPECIAL25"))
                            && animpos >= 7-3 && animpos <= 10 ) {
                            if ( playerkeys(p,hold_flag,"attack") ) {
                                     if ( anim_id == oc("ANI_FREESPECIAL20") ) performattack(self,oc("ANI_FREESPECIAL30"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL21") ) performattack(self,oc("ANI_FREESPECIAL31"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL22") ) performattack(self,oc("ANI_FREESPECIAL32"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL23") ) performattack(self,oc("ANI_FREESPECIAL33"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL24") ) performattack(self,oc("ANI_FREESPECIAL34"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL25") ) performattack(self,oc("ANI_FREESPECIAL35"),1);
                            }

                            if ( playerkeys(p,hold_flag,"attack") && max_shots > 0 ) {
                                slv("shot_count",glv("shot_count")+1);
                            }
                        } else if ( (anim_id == oc("ANI_FREESPECIAL30") || anim_id == oc("ANI_FREESPECIAL31") || anim_id == oc("ANI_FREESPECIAL32") || anim_id == oc("ANI_FREESPECIAL33")
                                   || anim_id == oc("ANI_FREESPECIAL34") || anim_id == oc("ANI_FREESPECIAL35"))
                                   && animpos >= 4-3 && animpos <= 7 ) {
                            if ( playerkeys(p,hold_flag,"attack") ) {
                                     if ( anim_id == oc("ANI_FREESPECIAL30") ) performattack(self,oc("ANI_FREESPECIAL30"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL31") ) performattack(self,oc("ANI_FREESPECIAL31"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL32") ) performattack(self,oc("ANI_FREESPECIAL32"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL33") ) performattack(self,oc("ANI_FREESPECIAL33"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL34") ) performattack(self,oc("ANI_FREESPECIAL34"),1);
                                else if ( anim_id == oc("ANI_FREESPECIAL35") ) performattack(self,oc("ANI_FREESPECIAL35"),1);
                            }

                            if ( playerkeys(p,hold_flag,"attack") && max_shots > 0 ) {
                                slv("shot_count",glv("shot_count")+1);
                            }
                        }
                    } // fine if max_count
            } // fine if modelname
        } // fine if anim_id
}


int check_rifle_atk(void self, int max_shots, int hold_flag) {
        int anim_id = gep(self,"animationid");

        if ( anim_id == oc("ANI_FREESPECIAL20") || anim_id == oc("ANI_FREESPECIAL21") ) {
            int p = gep(self, "playerindex");
            int animpos = gep(self, "animpos");
            char modelname = gep(self,"model");

            if ( getentityproperty(self,"dead") ) return;

            if ( strinfirst("player_foot_rifle", modelname) != -1 ) {

                //drawstring(10,100,0,"aaa");
                    if ( max_shots == NULL() ) max_shots = -1;

                    // Azzeriamo il conto degli spari
                    if ( glv("shot_count") == NULL() || anim_id == oc("ANI_FREESPECIAL20") ) slv("shot_count",0);

                    if ( playerkeys(p,0,"moveleft") ) {
                        if ( gep(self,"direction") != 0 ) cep(self,"direction",0);
                    } else if ( playerkeys(p,0,"moveright") ) {
                        if ( gep(self,"direction") != 1 ) cep(self,"direction",1);
                    }

                    if ( glv("shot_count") < max_shots-1 || max_shots == -1 ) {
                        if ( anim_id == oc("ANI_FREESPECIAL20") && animpos >= 4 && animpos <= 4 ) {
                            if ( playerkeys(p,hold_flag,"attack") ) performattack(self,oc("ANI_FREESPECIAL21"),1);

                            if ( playerkeys(p,hold_flag,"attack") && max_shots > 0 ) {
                                slv("shot_count",glv("shot_count")+1);
                            }
                        } else if ( anim_id == oc("ANI_FREESPECIAL21") && animpos >= 3 && animpos <= 3 ) {
                            if ( playerkeys(p,hold_flag,"attack") ) performattack(self,oc("ANI_FREESPECIAL21"),1);

                            if ( playerkeys(p,hold_flag,"attack") && max_shots > 0 ) {
                                slv("shot_count",glv("shot_count")+1);
                            }
                        }
                    } // fine if max_count
            } // fine if modelname
        } // fine if anim_id
}

