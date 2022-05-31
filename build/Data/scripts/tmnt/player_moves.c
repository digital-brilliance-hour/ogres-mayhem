
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/shadowtrail.c"
#import "data/scripts/player_commons.c"
#import "data/scripts/endlevel_anim.c"



int check_splatfall(void player) {
    int anim_id = gep(player,"animationid");

    if ( anim_id == oc("ANI_FOLLOW31") || anim_id == oc("ANI_FOLLOW35") ) {
        float x = getentityproperty(player,"x");
        float z = getentityproperty(player,"z");
        float y = getentityproperty(player,"y");
        int prev_dir = gev(player,0);
        float t_dist = 5;

        cep(player, "noaicontrol", 0);
        cep(player, "antigravity", 0);

        //if ( gep(player,"aiflag","dead") ) cep(player,"falldie",2);

        if ( ( prev_dir == 1 && find_platform_in_a(x+t_dist,z,y,MAX_ALTITUDE) == NULL() && checkwall(x+t_dist,z) <= y )
        || ( prev_dir == 0 && find_platform_in_a(x-t_dist,z,y,MAX_ALTITUDE) == NULL() && checkwall(x-t_dist,z) <= y ) ) {
            if ( anim_id == oc("ANI_FOLLOW31") ) cep(player,"animation",oc("ANI_FOLLOW36"));
            else if ( anim_id == oc("ANI_FOLLOW35") ) cep(player,"animation",oc("ANI_FOLLOW37"));
        }
    }
}

void clear_hitbyid(void player) {
    int anim_id = getentityproperty(player,"animationid");

    if ( anim_id != openborconstant("ANI_FREESPECIAL34") && anim_id != openborconstant("ANI_FREESPECIAL10") && anim_id != openborconstant("ANI_FREESPECIAL41") ) {
        setlocalvar("opp_list",clear_hitbyid_list(player,"opp_list")); // total clear
    } else {
        setlocalvar("opp_list",clear_hitbyid_list(player,"opp_list",1,1));
    }
}

// ANTI-BUG check
void check_general(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_IDLE") ) {
        int p = getentityproperty(player, "playerindex");

        if ( getentityvar(player,1) != NULL() ) setentityvar(player,1,NULL());
        if ( getentityvar(player,2) != NULL() ) setentityvar(player,2,NULL());

        // uccidiamo la mano
        /*if ( getglobalvar("any_button"+p) != NULL() && getentityproperty(getglobalvar("any_button"+p),"exists") ) {
                killentity(getglobalvar("any_button"+p));
                setglobalvar("any_button"+p,NULL());
        }*/
    } // fine if idle
}

int check_antigrab_for_board(void player) {
    int antigrab = getentityproperty(player, "antigrab");
    int board_level = check_board_level();

    if ( (board_level > 0 || getglobalvar("player_nograb") != NULL() ) && antigrab != 999999 ) changeentityproperty(player,"antigrab",999999);
}

void check_damage_recovery_on_jump(void self, int t_damage) {
    if ( getentityproperty(self,"aiflag","falling") && !getentityproperty(self,"aiflag","dead") ) {
        float a = getentityproperty(self,"y");
        float base = getentityproperty(self,"base");
        int last_damage = getentityvar(self,28);
        int anim_pos = getentityproperty(self,"animpos");

        if ( a > base && last_damage != NULL() && last_damage <= t_damage && anim_pos <= 1 ) {
            int anim_id = getentityproperty(self,"animationid");

            if ( anim_id == openborconstant("ANI_FALL") || anim_id == openborconstant("ANI_FALL2") || anim_id == openborconstant("ANI_FALL3") || anim_id == openborconstant("ANI_FALL4") ) {
                int r = rand_gen(0,9);

                if ( r >= 3 ) {
                    setentityvar(self,28,NULL());
                    return;
                }

                changeentityproperty(self,"aiflag","inpain",0);
                changeentityproperty(self,"aiflag","falling",0);
                //changeentityproperty(self,"animation",openborconstant("ANI_FOLLOW70"));
                performattack(self,openborconstant("ANI_FOLLOW70"),1);
                setentityvar(self,28,NULL());
            }
        }
    }

    return;
}

void check_attack_shift(void self) {
    int anim_id = getentityproperty(self,"animationid");

    /*if ( anim_id == openborconstant("ANI_ATTACK1") || anim_id == openborconstant("ANI_ATTACK2") || anim_id == openborconstant("ANI_ATTACK3")
        || anim_id == openborconstant("ANI_ATTACK4") || anim_id == openborconstant("ANI_ATTACK5") || anim_id == openborconstant("ANI_FREESPECIAL14")
        || anim_id == openborconstant("ANI_ATTACK6") || anim_id == openborconstant("ANI_FOLLOW6") || anim_id == openborconstant("ANI_ATTACKBACKWARD")
        || anim_id == openborconstant("ANI_FREESPECIAL3") || anim_id == openborconstant("ANI_FREESPECIAL33") ) {
            int dir = getentityproperty(self,"direction");
            int p = getentityproperty(self,"playerindex");
            float shift_speed = 0.4;

            if ( anim_id == openborconstant("ANI_ATTACK5") || anim_id == openborconstant("ANI_FREESPECIAL3") || anim_id == openborconstant("ANI_FREESPECIAL33") ) shift_speed *= 2;

            if ( (!dir && playerkeys(p,0,"moveright")) || (dir && playerkeys(p,0,"moveright")) ) {
                changeentityproperty(self,"velocity",shift_speed,NULL(),NULL());
            } else if ( (dir && playerkeys(p,0,"moveleft")) || (!dir && playerkeys(p,0,"moveleft")) ) {
                changeentityproperty(self,"velocity",-1*shift_speed,NULL(),NULL());
            } else if ( getentityproperty(self,"xdir") != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
    }*/

    if ( anim_id == openborconstant("ANI_ATTACK1") || anim_id == openborconstant("ANI_ATTACK2") || anim_id == openborconstant("ANI_ATTACK3")
        || anim_id == openborconstant("ANI_ATTACK4") || anim_id == openborconstant("ANI_FREESPECIAL14") || anim_id == openborconstant("ANI_ATTACKBACKWARD")
        || anim_id == openborconstant("ANI_ATTACK5") || anim_id == openborconstant("ANI_ATTACK6") || anim_id == openborconstant("ANI_FOLLOW6") ) {
            int anim_pos = getentityproperty(self,"animpos");
            float xdir = getentityproperty(self,"xdir");
            int dir = getentityproperty(self,"direction");
            int p = getentityproperty(self,"playerindex");
            char defaultmodel = getentityproperty(self,"defaultmodel");
            float shift_speed = 0.5;
            int buffering_frame = 1;

            if ( defaultmodel == "MIKE" || defaultmodel == "RAPH" ) ++buffering_frame;

            if ( defaultmodel == "LEO" ) {
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos < 5-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK2") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK3") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK4") && anim_pos < 7-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK5") && anim_pos < 4-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK6") && anim_pos < 8-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK10") && anim_pos < 7-buffering_frame ) {
                    return;
                } else if ( anim_id == openborconstant("ANI_FOLLOW6") && anim_pos < 21-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACKBACKWARD") && anim_pos < 7-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_FREESPECIAL14") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                }
            } else if ( defaultmodel == "MIKE" ) {
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos < 8-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK2") && anim_pos < 11-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK3") && anim_pos < 11-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK4") && anim_pos < 16-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK5") && anim_pos < 7-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK6") ) {
                    //if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK10") && anim_pos < 16-buffering_frame ) {
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACKBACKWARD") && anim_pos < 8-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_FREESPECIAL14") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                }
            } else if ( defaultmodel == "DON" ) {
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK2") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK3") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK4") && anim_pos < 12-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK5") ) {
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK6") && anim_pos < 4-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK10") && anim_pos < 7-buffering_frame ) {
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACKBACKWARD") && anim_pos < 7-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_FREESPECIAL14") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                }
            } else if ( defaultmodel == "RAPH" ) {
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK2") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK3") && anim_pos < 7-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK4") && anim_pos < 16-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK5") ) {
                    if ( anim_pos < 3 ) {
                        if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                        return;
                    } else return;
                } else if ( anim_id == openborconstant("ANI_ATTACK6") && anim_pos < 4-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK10") && anim_pos < 6-buffering_frame ) {
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACKBACKWARD") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_FREESPECIAL14") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                }
            }

            if ( (!dir && playerkeys(p,0,"moveright")) || (dir && playerkeys(p,0,"moveright")) ) {
                changeentityproperty(self,"velocity",shift_speed,NULL(),NULL());
            } else if ( (dir && playerkeys(p,0,"moveleft")) || (!dir && playerkeys(p,0,"moveleft")) ) {
                changeentityproperty(self,"velocity",-1*shift_speed,NULL(),NULL());
            } else if ( getentityproperty(self,"xdir") != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
    }

    return;
}

void check_pos_moves(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_GRABFORWARD") ) {
        int anim_pos = getentityproperty(player, "animpos");

        if ( anim_pos <= 0 ) {
            check_safe_pos(player,5,0,0); // prima effettua un controllo esclusivamente orizzontale
            check_safe_pos(player,5,1,0);
        }
    } else if ( anim_id == openborconstant("ANI_FREESPECIAL9") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        int dir = getentityproperty(player,"direction");
        float threshold = 5, xthreshold = 10;
        int anim_pos = getentityproperty(player, "animpos");

        if ( anim_pos <= 0 ) {
            check_safe_pos(player,threshold,threshold,0);
        }
    }
}

void check_blockpain(void player) {
    if ( getentityproperty(player, "aiflag", "blocking") ) {
        int anim_id = getentityproperty(player, "animationid");

        if ( getentityvar(player,18) > 0 ) {
            changeentityproperty(player, "animation", openborconstant("ANI_BLOCKPAIN9"));
        } else if ( getentityvar(player,18) == -1 && anim_id == openborconstant("ANI_BLOCK") ) {
            changeentityproperty(player, "animation", openborconstant("ANI_BLOCKPAIN"));
        }

        if ( getentityvar(player,18) != NULL() ) setentityvar(player,18,NULL());
    } else {
        if ( getentityvar(player,18) != NULL() ) setentityvar(player,18,NULL());
    } // fine if blocking
}

int disable_keys_for_moves(void player) {
    int anim_id = getentityproperty(player, "animationid");
    int p = getentityproperty(player, "playerindex");

       //if ( anim_id == 45 ) changeplayerproperty(0, "keys", 64);
       if ( anim_id == openborconstant("ANI_JUMPATTACK") || anim_id == openborconstant("ANI_FREESPECIAL2") || anim_id == openborconstant("ANI_JUMPFORWARD")
                || anim_id == openborconstant("ANI_RUNJUMPATTACK") || anim_id == openborconstant("ANI_FREESPECIAL6 ") || anim_id == openborconstant("ANI_JUMPATTACK3")
                || anim_id == openborconstant("ANI_JUMPATTACK2") || anim_id == 45 || anim_id == openborconstant("ANI_FREESPECIAL30") ) { // 45 == jumpattack2
                    float xdir = getentityproperty(player, "xdir");
                    float zdir = getentityproperty(player, "zdir");
                    float tossv = getentityproperty(player, "tossv");
                    int keys = getplayerproperty(p, "keys");

                    //changeentityproperty(player, "noaicontrol", 1);
                    //changeplayerproperty(p, "keys", 0);

                    // Disabling z shift
                    if ( anim_id == openborconstant("ANI_FREESPECIAL6 ") || anim_id == openborconstant("ANI_JUMPATTACK2")
                        || anim_id == openborconstant("ANI_JUMPATTACK3") || anim_id == openborconstant("ANI_FREESPECIAL30") ) {
                            if ( playerkeys(p,0,"movedown") ) changeplayerproperty(p, "keys", keys-playerkeys(p,0,"movedown"));
                            if ( playerkeys(p,0,"moveup") ) changeplayerproperty(p, "keys", keys-playerkeys(p,0,"moveup"));
                            changeentityproperty(player, "velocity", xdir,0,tossv);
                        }
       } else if ( anim_id == openborconstant("ANI_SHOCK") || anim_id == openborconstant("ANI_SHOCKPAIN") ) { // 32 == spain
            changeplayerproperty(p, "keys", 0);
       }

       /*if ( getglobalvar("board_level") > 0
           && ( anim_id == openborconstant("ANI_RUNATTACK") || anim_id == openborconstant("ANI_FREESPECIAL18") || anim_id == openborconstant("ANI_FREESPECIAL19") ) ) {
            setidle(player,openborconstant("ANI_IDLE"),1);
       }*/

       // disabling special on pain
       if ( getentityproperty(player, "aiflag", "inpain") ) {

            if ( anim_id != openborconstant("ANI_GRABBED") && anim_id != openborconstant("ANI_FOLLOW44") && anim_id != openborconstant("ANI_FOLLOW41")
                && anim_id != openborconstant("ANI_FOLLOW40") && anim_id != openborconstant("ANI_PAIN30") && anim_id != openborconstant("ANI_PAIN31")
                && anim_id != openborconstant("ANI_PAIN32") && anim_id != openborconstant("ANI_PAIN33") && anim_id != openborconstant("ANI_PAIN34")
                && anim_id != openborconstant("ANI_SHOCKPAIN") ) { // 32 == ANI_SPAIN
                    if ( playerkeys(p, 0, "attack") && playerkeys(p, 0, "jump") ) {
                            changeplayerproperty(p, "keys", 0);
                            changeplayerproperty(p, "playkeys", 0);
                    }
            }
       }
}

int check_don_spacial_throw(void player) {

    // IN ANIMATION
    /*if ( getentityproperty(player, "defaultmodel") == "DON" && getentityproperty(player, "animationid") == openborconstant("ANI_FOLLOW13") ) {
        void opp = getentityproperty(player, "opponent");

        if ( getentityproperty(opp, "exists") ) {
            if ( getentityproperty(opp, "animationid") != openborconstant("ANI_PAIN36") || getentityproperty(opp, "aiflag", "dead") ) {
                performattack(player, openborconstant("ANI_FOLLOW14"), 1);
            }
        }
    }*/ // fine follow13

    // controlliamo se ci sono muri o piattaforme troppo vicini altrimenti rimane incastrao il nemico
    if ( getentityproperty(player, "defaultmodel") == "DON" && getentityproperty(player, "animationid") == openborconstant("ANI_FOLLOW13") ) {
        int anim_pos = getentityproperty(player, "animpos");

        if ( anim_pos == 17 ) {
            float x = getentityproperty(player, "x");
            float z = getentityproperty(player, "z");
            float a = getentityproperty(player, "y");
            float a = getentityproperty(player, "base");
            int dir = getentityproperty(player, "direction");
            float dist = 70;
            float wall_l = checkwall(x-dist,z);
            float wall_r = checkwall(x+dist,z);
            void platform_l = find_platform_in_a(x-dist,z,a,MAX_ALTITUDE);
            void platform_r = find_platform_in_a(x+dist,z,a,MAX_ALTITUDE);

            // vediamo se ci sono muri da entrambi i lati
            if ( (wall_r > a || platform_r != NULL()) && (wall_l > a || platform_l != NULL()) ) {
                void opp = getentityproperty(player, "opponent");
                void platform_on = get_platform_on(player);

                if ( platform_on == platform_l || platform_on == platform_r ) return 0;

                if ( getentityproperty(opp,"exists") ) {
                    if ( getentityproperty(opp,"animationid") == openborconstant("ANI_PAIN36") ) changeentityproperty(opp, "animpos", 21);
                    changeentityproperty(opp, "antigravity", 0);
                    bindentity(opp, NULL());
                    changeentityproperty(opp, "subject_to_wall", 1);
                    changeentityproperty(opp, "subject_to_platform", 1);
                }
                setidle(player, openborconstant("ANI_IDLE"), 1);
            } else {
                if ( !dir ) {
                    if ( wall_r > a || platform_r != NULL() ) {
                        changeentityproperty(player, "direction", 1);
                    }
                } else {
                    if ( wall_l > a || platform_l != NULL() ) {
                        changeentityproperty(player, "direction", 0);
                    }
                }
            }
        }

        return 1;
    } // fine follow13

    return 0;
}

int check_shadow_trail(void player) {
    if ( get_shad_trail_flag(player) != NULL() && get_shad_trail_flag(player) != 0 ) {
        if ( draw_shadowtrail(player, 5, 15, 20, 160, 0, get_shad_trail_flag(player)) == 0 ) set_shad_trail_flag(player,NULL());
    }
}

int get_shad_trail_flag(void player) {
    return getentityvar(player, 16);
}

int set_shad_trail_flag(void player, int value) {
    setentityvar(player, 16, value);
}

int check_specific_turtle_shadow_moves(void player) {
    char defaultmodel = getentityproperty(player, "defaultmodel");

    if ( defaultmodel == "LEO" ) {
        check_leo_chargeatk(player);
    } else if ( defaultmodel == "MIKE" ) {
        check_mike_chargeatk(player);
    } else if ( defaultmodel == "DON" ) {
        check_don_chargeatk(player);
    } else if ( defaultmodel == "RAPH" ) {
        //check_raph_chargeatk(player);
    }
}

int check_specific_turtle_moves(void player) {
    char defaultmodel = getentityproperty(player, "defaultmodel");

    if ( defaultmodel == "LEO" ) {
    } else if ( defaultmodel == "MIKE" ) {
    } else if ( defaultmodel == "DON" ) {
        check_don_spacial_throw(player);
    } else if ( defaultmodel == "RAPH" ) {
    }
}

int check_don_chargeatk(void player) {
    char defaultmodel = getentityproperty(player, "defaultmodel");
    int anim_id = getentityproperty(player, "animationid");
    int anim_pos = getentityproperty(player, "animpos");

    if ( defaultmodel == "DON" && anim_id == openborconstant("ANI_CHARGEATTACK") && anim_pos < 25 ) {
        set_shad_trail_flag(player,1);
        //draw_shadowtrail(player, 5, 15, 20, 160, 0);
    } else if ( defaultmodel == "DON" && anim_id == openborconstant("ANI_CHARGEATTACK") && anim_pos >= 25 ) {
        set_shad_trail_flag(player,0);
        clear_shadowtrail(player);
    } else if ( defaultmodel == "DON" && anim_id != openborconstant("ANI_CHARGEATTACK") && get_shad_trail_flag(player) != 0 ) {
        set_shad_trail_flag(player,0);
        clear_shadowtrail(player);
    }

}

int check_mike_chargeatk(void player) {
    char defaultmodel = getentityproperty(player, "defaultmodel");
    int anim_id = getentityproperty(player, "animationid");
    int anim_pos = getentityproperty(player, "animpos");

    if ( defaultmodel == "MIKE" && anim_id == openborconstant("ANI_CHARGEATTACK") && anim_pos < 25 ) {
        set_shad_trail_flag(player,1);
        //draw_shadowtrail(player, 5, 15, 20, 160, 0);
    } else if ( defaultmodel == "MIKE" && anim_id == openborconstant("ANI_CHARGEATTACK") && anim_pos >= 28 ) {
        set_shad_trail_flag(player,0);
        clear_shadowtrail(player);
    } else if ( defaultmodel == "MIKE" && anim_id != openborconstant("ANI_CHARGEATTACK") && get_shad_trail_flag(player) != 0 ) {
        set_shad_trail_flag(player,0);
        clear_shadowtrail(player);
    }

}

int check_leo_chargeatk(void player) {
    char defaultmodel = getentityproperty(player, "defaultmodel");
    int anim_id = getentityproperty(player, "animationid");
    int anim_pos = getentityproperty(player, "animpos");

    if ( defaultmodel == "LEO" && anim_id == openborconstant("ANI_CHARGEATTACK") && anim_pos < 7 ) {
        set_shad_trail_flag(player,1);
        //draw_shadowtrail(player, 5, 15, 20, 160, 0);
    } else if ( defaultmodel == "LEO" && anim_id == openborconstant("ANI_CHARGEATTACK") && anim_pos >= 7 ) {
        set_shad_trail_flag(player,0);
        clear_shadowtrail(player);
    } else if ( defaultmodel == "LEO" && anim_id != openborconstant("ANI_CHARGEATTACK") && get_shad_trail_flag(player) != 0 ) {
        set_shad_trail_flag(player,0);
        clear_shadowtrail(player);
    }

}

int check_raph_chargeatk(void player) {
    char defaultmodel = getentityproperty(player, "defaultmodel");
    int anim_id = getentityproperty(player, "animationid");
    int anim_pos = getentityproperty(player, "animpos");

    if ( defaultmodel == "RAPH" && anim_id == openborconstant("ANI_CHARGEATTACK") && anim_pos < 39 ) {
        set_shad_trail_flag(player,1);
        //draw_shadowtrail(player, 5, 15, 20, 160, 0);
    } else if ( defaultmodel == "RAPH" && anim_id == openborconstant("ANI_CHARGEATTACK") && anim_pos >= 39 ) {
        set_shad_trail_flag(player,0);
        clear_shadowtrail(player);
    } else if ( defaultmodel == "RAPH" && anim_id != openborconstant("ANI_CHARGEATTACK") && get_shad_trail_flag(player) != 0 ) {
        set_shad_trail_flag(player,0);
        clear_shadowtrail(player);
    }

}

int check_forwardkick(void player) {
   if ( !getentityproperty(player, "noaicontrol") ) { // Cioè solo se stai controllando il personaggio
       int anim_id = getentityproperty(player, "animationid");
       int p = getentityproperty(player, "playerindex");

       if ( getentityproperty(player, "aiflag", "running") == 1 && anim_id == openborconstant("ANI_RUN")
            && playerkeys(p,0,"attack") == 64 && playerkeys(p,0,"jump") == 128 ) {

            performattack(player, openborconstant("ANI_FREESPECIAL11"), 1);
       }
   }
}

// TRANSFER IN CHARA.TXT???
int check_mytoes(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_PAIN14") || anim_id == openborconstant("ANI_PAIN39") ) {
        float xdir = getentityproperty(player,"xdir");
        float zdir = getentityproperty(player,"zdir");
        float tossv = getentityproperty(player,"tossv");
        float speed = getentityproperty(player,"speed");
        int p = getentityproperty(player, "playerindex");

        if ( playerkeys(p,0,"moveleft") == 4  ) xdir = -1*speed/2;
        else if ( playerkeys(p,0,"moveright") == 8  ) xdir = speed/2;
        else xdir = 0;

        if ( playerkeys(p,0,"moveup") == 16  ) zdir = -1*speed/4;
        else if ( playerkeys(p,0,"movedown") == 32  ) zdir = speed/4;
        else zdir = 0;

        changeentityproperty(player,"velocity", xdir, zdir, NULL());
    }

}

int check_pizzapow(void player) {

    if ( getentityproperty(player,"animationid") == openborconstant("ANI_FREESPECIAL10") ) {
        float xdir = getentityproperty(player,"xdir");
        float zdir = getentityproperty(player,"zdir");
        float tossv = getentityproperty(player,"tossv");
        int p = getentityproperty(player, "playerindex");
        void opp = getentityproperty(player, "opponent");
        void old_opp = getlocalvar("tmp_opp");
        int game_speed = openborvariant("game_speed");
        int eta = game_speed*5;

        /*if ( getentityproperty(opp,"exists") ) {
            if ( getentityproperty(opp,"hitbyid") != 0 ) changeentityproperty(opp, "hitbyid", 0); // vedere con vulnerable!! -> resettiamo sempre la possibilità di colpire l'avversario!!
            if ( opp != old_opp ) {
                if ( getentityproperty(old_opp,"exists") ) {
                    if ( getentityproperty(old_opp,"hitbyid") != 0 ) changeentityproperty(old_opp, "hitbyid", 0);
                }
            }
            setlocalvar("tmp_opp",opp);
        }*/

        // USE: clear_hitbyid()
        //setlocalvar("opp_list",clear_hitbyid_list(player,"opp_list"));

        if ( getentityvar(player,1) == -1001 ) {
            setentityvar(player,1,get_next_time(eta));
            changeentityproperty(player, "invincible", 1);
            changeentityproperty(player, "invinctime", get_next_time(eta));
        } else {
            // Check Time-Reset
            if ( check_stored_time(getentityvar(player,1),9999) ) setentityvar(player,1,openborconstant("MIN_INT"));
            if ( openborvariant("elapsed_time") > getentityvar(player,1) ) {
                changeentityproperty(player, "noaicontrol", 0);
                changeentityproperty(player, "invincible", 0);
                changeentityproperty(player, "aiflag", "jumping", 0);
                changeentityproperty(player, "aiflag", "running", 0);
                setentityvar(player,1,NULL());
                //setidle(player);
                setidle(player,openborconstant("ANI_IDLE"),1);
            }
        }

        if ( playerkeys(p,0,"moveleft") == 4  ) xdir = -1*getentityproperty(player,"speed");
        else if ( playerkeys(p,0,"moveright") == 8  ) xdir = getentityproperty(player,"speed");
        else xdir = 0;

        if ( playerkeys(p,0,"moveup") == 16  ) zdir = -1*getentityproperty(player,"speed")/2;
        else if ( playerkeys(p,0,"movedown") == 32  ) zdir = getentityproperty(player,"speed")/2;
        else zdir = 0;

        xdir *= 1.5; zdir *= 1.5;
        changeentityproperty(player,"velocity", xdir, zdir, tossv);
    }

}

int jump_attack_multicancel(void player) {
    int anim_id = getentityproperty(player, "animationid");

    // Questo a-threshold > base dice: possiamo eseguire la mossa finchè siamo alti rispetto alla base da cui siamo saltati. Alti significa da threshold pixels in su!
    if ( anim_id == openborconstant("ANI_FREESPECIAL3") || anim_id == openborconstant("ANI_FREESPECIAL33") ) {
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        float a_threshold = 20;

        //Se siamo arrivati a terra impostiamo l'idle!
        if ( a <= base ) {
            changeentityproperty(player, "aiflag", "running", 0);
            changeentityproperty(player, "aiflag", "jumping", 0);
            setentityvar(player, 5, NULL());
            setidle(player, openborconstant("ANI_IDLE"), 1);

            return;
        }

        if ( (a-a_threshold) > base ) {
            int p = getentityproperty(player, "playerindex");
            int start_frame = 4, end_frame = 5;
            float xdir = getentityproperty(player, "xdir");
            float zdir = getentityproperty(player, "zdir");
            float tossv = getentityproperty(player, "tossv");
            int dir = getentityproperty(player, "direction");

            tossv /= 1.05; // 1.03; // diminuiamo l'accelerazione di gravità
            if ( getentityproperty(player, "aiflag", "running") == 1 && getentityvar(player, 5) == NULL() ) {
                    if ( getentityproperty(player, "direction") == 1 ) xdir = getentityproperty(player, "running", "speed");
                    else xdir = -1*getentityproperty(player, "running", "speed");
                    changeentityproperty(player, "aiflag", "running", 0);
            } else if ( getentityproperty(player, "aiflag", "running") == 1 && getentityvar(player, 5) != NULL() ) { // Utilizziamo la forza di gravità della spinta!
                    xdir = getentityvar(player, 5);
                    //xdir /= 2;
                    changeentityproperty(player, "aiflag", "running", 0);
                    setentityvar(player, 5, NULL());
            } else if ( getentityvar(player, 5) != NULL() ) {
                    //drawstring( 10,200,0,"C: "+getentityvar(player,5));
                    xdir = getentityvar(player, 5);
                    //xdir /= 3;
                    setentityvar(player, 5, NULL());
            } else if ( getentityvar(player, 5) == NULL() && xdir == 0 ) {
                    xdir = getjumpvel(player);
                    if ( (xdir < 0 && dir) || (xdir > 0 && !dir) ) xdir *= -1;
            }

            changeentityproperty(player, "velocity", xdir, zdir, tossv);
            changeentityproperty(player, "aiflag", "jumping", 0); // evitiamo mosse cancels impostate su jump

            // Window di bufferizzazione della multicancel
            if ( getentityproperty(player, "animpos") >= start_frame && getentityproperty(player, "defaultmodel") != "MIKE" ) { // && getentityproperty(player, "animpos") <= end_frame
                if ( anim_id == openborconstant("ANI_FREESPECIAL33") ) {
                    if ( (playerkeys(p, 1, "attack") || playerkeys(p, 1, "special"))
                        || ((playerkeys(p, 1, "attack") || playerkeys(p, 1, "special")) && playerkeys(p, 0, "moveleft"))
                        || ((playerkeys(p, 1, "attack") || playerkeys(p, 1, "special")) && playerkeys(p, 0, "moveright")) ) {
                        performattack(player, openborconstant("ANI_FREESPECIAL33"), 1);
                        //changeentityproperty(player, "animation", openborconstant("ANI_FREESPECIAL3"));
                    }
                } else {
                    if ( playerkeys(p, 1, "attack") || (playerkeys(p, 1, "attack") && playerkeys(p, 0, "moveleft")) || (playerkeys(p, 1, "attack") && playerkeys(p, 0, "moveright")) ) {
                        performattack(player, openborconstant("ANI_FREESPECIAL3"), 1);
                        //changeentityproperty(player, "animation", openborconstant("ANI_FREESPECIAL3"));
                    }
                } // fine if anim ANI_FREESPECIAL3
            } else if ( getentityproperty(player, "animpos") >= start_frame+1 && getentityproperty(player, "defaultmodel") == "MIKE" ) { // && getentityproperty(player, "animpos") <= end_frame
                if ( anim_id == openborconstant("ANI_FREESPECIAL33") ) {
                    if ( (playerkeys(p, 1, "attack") || playerkeys(p, 1, "special"))
                        || ((playerkeys(p, 1, "attack") || playerkeys(p, 1, "special")) && playerkeys(p, 0, "moveleft"))
                        || ((playerkeys(p, 1, "attack") || playerkeys(p, 1, "special")) && playerkeys(p, 0, "moveright")) ) {
                        performattack(player, openborconstant("ANI_FREESPECIAL33"), 1);
                        //changeentityproperty(player, "animation", openborconstant("ANI_FREESPECIAL3"));
                    }
                } else {
                    if ( playerkeys(p, 1, "attack") || (playerkeys(p, 1, "attack") && playerkeys(p, 0, "moveleft")) || (playerkeys(p, 1, "attack") && playerkeys(p, 0, "moveright")) ) {
                        performattack(player, openborconstant("ANI_FREESPECIAL3"), 1);
                        //changeentityproperty(player, "animation", openborconstant("ANI_FREESPECIAL3"));
                    }
                } // fine if anim ANI_FREESPECIAL3
            } else {
                     // se siamo out-of-windows di bufferizzazione, la pressione del tasto attacco verrà ignorata!!
                     if ( playerkeys(p,0,"attack") == 64 ) changeplayerproperty(p, "keys", getplayerproperty(p, "keys")-64);
            }
        } // fine if (a-a_threshold) > base
    } // fine if anim_id == openborconstant("ANI_FREESPECIAL3") || anim_id == openborconstant("ANI_FREESPECIAL33")


    // Prendiamo la entityvar 5 da check_hold_wall_anim() da platform_splat.c
    else if ( anim_id != openborconstant("ANI_FREESPECIAL3") && anim_id != openborconstant("ANI_FREESPECIAL33") && anim_id != openborconstant("ANI_FOLLOW45") ) {
            if ( getentityvar(player, 5) != NULL() ) {
                changeentityproperty(player, "aiflag", "running", 0);
                setentityvar(player, 5, NULL());
            }
    }
}

int setjumpvel(void player) { // prende la velocità xdir mentre è in salto
    if ( getentityproperty(player, "animationid") == openborconstant("ANI_JUMP") && getentityproperty(player, "aiflag", "jumping") == 1 ) {
        setentityvar(player, 10, getentityproperty(player, "xdir"));
    } else if ( getentityproperty(player, "defaultmodel") == "LEO" && getentityproperty(player, "animationid") == openborconstant("ANI_FREESPECIAL15") ) {
        setentityvar(player, 10, getentityproperty(player, "xdir"));
    } //else setentityvar(player, 10, NULL());
}

int getjumpvel(void player) {
    return getentityvar(player, 10);
}

int set_prev_anim(void player) {
    int anim_id = getentityproperty(player, "animationid");

    //if ( getentityproperty(player, "animpos") > 0 ) setentityvar(player, 14, anim_id);

    if ( getlocalvar("temp_anim") != anim_id ) {
        int prev_anim = get_prev_anim(player);

        /*if ( prev_anim == openborconstant("ANI_BLOCKPAIN") || prev_anim == openborconstant("ANI_BLOCKPAIN9") ) {
            if ( anim_id == openborconstant("ANI_BLOCK") ) return;
        }*/

        setentityvar(player,14,getlocalvar("temp_anim"));
        setlocalvar("temp_anim",anim_id);
    }
}

int get_prev_anim(void player) {
    return getentityvar(player, 14);
}

int set_prev_animpos(void player) {
    int anim_pos = getentityproperty(player, "animpos");

    //if ( getentityproperty(player, "animpos") > 0 ) setentityvar(player, 14, anim_id);

    if ( getlocalvar("temp_animpos") != anim_pos ) {
        setentityvar(player,21,getlocalvar("temp_animpos"));
        setlocalvar("temp_animpos",anim_pos);
    }
}

int get_prev_animpos(void player) {
    return getentityvar(player, 21);
}

/*
 * 0 = going left
 * 1 = going right
 * 2 = stop
 * NULL = not detectable
 */
int getwalkingdir(void ent, int frames) {
    int animid = getentityproperty(ent,"animationid");

    if ( animid != oc("ANI_FOLLOW31") && animid != oc("ANI_FOLLOW32")
        && animid != oc("ANI_FOLLOW34") && animid != oc("ANI_FOLLOW35")
         && animid != oc("ANI_FOLLOW36") && animid != oc("ANI_FOLLOW37") ) {
        float xdir = getentityproperty(ent,"xdir");
        int result = NULL();

        if ( xdir != NULL() ) {
            if ( xdir > 0 ) {
                result = 1;
            } else if ( xdir < 0 ) {
                result = 0;
            } else { // se x2 == x1...
                // se corri o cammini ma urti sul muro guarda la direzione senza osservare se c'è un cambiamento fra x1 e x2 (se x1==x2 evidentemente stavi già sul muro)
                /*if ( checkwalls(ent,1,1) ) {
                        return getentityproperty(ent,"direction");
                } else*/
                if ( gep(ent, "animationid") == oc("ANI_FOLLOW30") || gep(ent, "animationid") == oc("ANI_FOLLOW31") || gep(ent, "animationid") == oc("ANI_FOLLOW34")
                    || gep(ent, "animationid") == oc("ANI_FOLLOW35") || gep(ent, "animationid") == oc("ANI_FOLLOW36") || gep(ent, "animationid") == oc("ANI_FOLLOW37")) {
                    result = getentityvar(ent,0); // Se avviene wall splat non consideriamo le relative animazioni... restituiamo quindi quello che c'era prima!
                }else result = 2;
            }
        } else result = NULL();

        return result;
    }

    return getentityvar(ent,0);
}

int checkwalls(void player, float distx, float distz) {
    int i,j;
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");

/*
 * f:  1,0
 * d/f:1,1
 * d:  0,1
 * d/b:-1,1
 * b:  -1,0
 * u/b:-1,-1
 * u:0,-1
 * u/f:1,-1
 */

    for (i = -1; i < 1+1; ++i) {
        for (j = -1; j < 1+1; ++j) {
            if ( checkwall(x+(i*distx),z+(j*distz)) > 0 ) return 1;
            else continue;
        }
    }

    return 0;
}



