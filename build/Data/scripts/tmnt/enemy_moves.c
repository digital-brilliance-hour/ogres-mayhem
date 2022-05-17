
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

#import "data/scripts/e_platform_splat.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/player_didhit.c"
#import "data/scripts/lib_movements.c"


int check_autoattack(void self) {
    int auto_atk = gev(self,"auto_attack_flag");

    //drawstring(10,50,0,":  "+glv("auto_atk_step"));
    if ( auto_atk >= 1 ) {
        float dist_x = gev(self,"auto_atk_distance_x");
        float dist_z = gev(self,"auto_atk_distance_z");
        int final_dir = gev(self,"auto_atk_direction");
        char atk_anim = gev(self,"auto_atk_animation");
        int run_flag = gev(self,"auto_atk_run_flag");
        int remain_flag = gev(self,"auto_atk_remain_flag");
        float atk_wait_start_time = gev(self,"auto_atk_wait_start_time");
        float atk_wait_end_time = gev(self,"auto_atk_wait_end_time");
        float x = gep(self,"x");
        float z = gep(self,"z");
        float y = gep(self,"y");
        float b = gep(self,"base");
        int dir = gep(self,"direction");
        int anim_id = gep(self,"animation");
        int auto_atk_step = glv("auto_atk_step");
        int init_x = glv("auto_atk_init_x");
        int init_z = glv("auto_atk_init_z");
        int final_x = glv("auto_atk_final_x");
        int final_z = glv("auto_atk_final_z");

        if ( dist_x == NULL() ) dist_x = 0;
        if ( dist_z == NULL() ) dist_z = 0;
        if ( final_dir == NULL() ) final_dir = 0;
        if ( atk_anim == NULL() || atk_anim == "" ) {
            sev(self,"auto_attack_flag",0);
            return 0;
        }
        if ( atk_wait_start_time == NULL() ) atk_wait_start_time = 0.3;
        if ( atk_wait_end_time == NULL() ) atk_wait_end_time = 0.3;
        if ( auto_atk_step == NULL() ) auto_atk_step = 0;

        switch ( auto_atk_step ) {
            case 0:
                cep(self,"noaicontrol",1);
                slv("auto_atk_init_x",x);
                slv("auto_atk_init_z",z);
                slv("auto_atk_final_x",x+dist_x);
                slv("auto_atk_final_z",z+dist_z);
                slv("auto_atk_step",1);
                break;
            case 1:
                if ( run_flag >= 1 ) {
                    if ( runningtoxz(self,final_x,final_z,final_dir) ) {
                        setidle(self,oc("ANI_IDLE"),1);
                        slv("auto_atk_step",2);
                    }
                } else {
                    if ( movetoxz(self,final_x,final_z,final_dir) ) {
                        setidle(self,oc("ANI_IDLE"),1);
                        slv("auto_atk_step",2);
                    }
                }

                break;
            case 2:
                if ( glv("auto_atk_wait") == NULL() ) slv("auto_atk_wait",get_next_time(ov("game_speed")*atk_wait_start_time));

                if ( ov("elapsed_time") >= glv("auto_atk_wait") ) {
                    slv("auto_atk_wait",NULL());
                    slv("auto_atk_step",3);
                }
                break;
            case 3:
                if ( is_in_idle_anim(self,10) && !gep(self,"aiflag","attacking") ) {
                    performattack(self,oc(atk_anim),1);
                    slv("auto_atk_step",4);
                }
                break;
            case 4:
                if ( is_in_idle_anim(self,10) && !gep(self,"aiflag","attacking") ) {
                    slv("auto_atk_step",5);
                }
                break;
            case 5:
                if ( glv("auto_atk_wait") == NULL() ) slv("auto_atk_wait",get_next_time(ov("game_speed")*atk_wait_end_time));

                if ( ov("elapsed_time") >= glv("auto_atk_wait") ) {
                    slv("auto_atk_wait",NULL());
                    slv("auto_atk_step",6);
                }
                break;
            case 6:
                if ( remain_flag >= 1 ) {
                    setidle(self,oc("ANI_IDLE"),1);
                    slv("auto_atk_step",8);
                    gev(self,"auto_attack_flag",NULL());
                    break;
                }

                if ( run_flag >= 1 ) {
                    if ( runningtoxz(self,init_x,init_z,!final_dir) ) {
                        setidle(self,oc("ANI_IDLE"),1);
                        slv("auto_atk_step",7);
                    }
                } else {
                    if ( movetoxz(self,init_x,init_z,!final_dir) ) {
                        setidle(self,oc("ANI_IDLE"),1);
                        slv("auto_atk_step",7);
                    }
                }

                break;
            case 7:
                killentity(self);
                return 1;
                break;
            default:
                break;
        }
    }

    return 0;
}

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

int check_antigrab_for_board(void player) {
    int antigrab = getentityproperty(player, "antigrab");
    //int board_level = check_board_level();
    int board_level = getglobalvar("board_level");

    if ( (board_level > 0 || getglobalvar("enemy_nograb") != NULL() ) && antigrab != 999999 ) changeentityproperty(player,"antigrab",999999);
}

int set_prev_anim(void player) {
    int anim_id = getentityproperty(player, "animationid");

    //if ( getentityproperty(player, "animpos") > 0 ) setentityvar(player, 14, anim_id);

    if ( getlocalvar("temp_anim") != anim_id ) {
        int prev_anim = get_prev_anim(player);

        /*if ( prev_anim == openborconstant("ANI_BLOCKPAIN") || prev_anim == openborconstant("ANI_BLOCKPAIN9") ) {
            if ( anim_id == openborconstant("ANI_BLOCK") ) return;
        }*/

        setentityvar(player,15,getlocalvar("temp_anim"));
        setlocalvar("temp_anim",anim_id);
    }
}

int get_prev_anim(void player) {
    return getentityvar(player, 15);
}

int set_prev_animpos(void player) {
    int anim_pos = getentityproperty(player, "animpos");

    //if ( getentityproperty(player, "animpos") > 0 ) setentityvar(player, 14, anim_id);

    if ( getlocalvar("temp_animpos") != anim_pos ) {
        setentityvar(player,16,getlocalvar("temp_animpos"));
        setlocalvar("temp_animpos",anim_pos);
    }
}

int get_prev_animpos(void player) {
    return getentityvar(player, 16);
}

int disable_keys_for_moves(void player) {
    int anim_id = getentityproperty(player, "animationid");
    int p = getentityproperty(player, "playerindex");
    void model = getentityproperty(player, "model");

       //if ( anim_id == 45 ) changeplayerproperty(0, "keys", 64);
       if ( (model == "player_foot_sword" || model == "player_foot_fish") && anim_id == openborconstant("ANI_FREESPECIAL23 ") ) {
                    float xdir = getentityproperty(player, "xdir");
                    float zdir = getentityproperty(player, "zdir");
                    float tossv = getentityproperty(player, "tossv");
                    int keys = getplayerproperty(p, "keys");

                    // Disabling z shift
                    if ( playerkeys(p,0,"movedown") ) changeplayerproperty(p, "keys", keys-playerkeys(p,0,"movedown"));
                    if ( playerkeys(p,0,"moveup") ) changeplayerproperty(p, "keys", keys-playerkeys(p,0,"moveup"));
                    changeentityproperty(player, "velocity", xdir,0,tossv);
                    //drawstring(10,160,0,"OK");
       }
}

int disable_moves(void player) {
    int anim_id = getentityproperty(player, "animationid");

       if ( getglobalvar("board_level") > 0
           && ( anim_id == openborconstant("ANI_FREESPECIAL7") || anim_id == openborconstant("ANI_FREESPECIAL8") || anim_id == openborconstant("ANI_FREESPECIAL30") || anim_id == openborconstant("ANI_FREESPECIAL31") ) ) {
            setidle(player,openborconstant("ANI_IDLE"),1);
       }
}

void check_attack_shift(void self) {
    int anim_id = getentityproperty(self,"animationid");

    if ( anim_id == openborconstant("ANI_ATTACK1") || anim_id == openborconstant("ANI_ATTACK2") || anim_id == openborconstant("ANI_ATTACK3")
        || anim_id == openborconstant("ANI_ATTACK4") || anim_id == openborconstant("ANI_ATTACKBACKWARD") ) {
            int anim_pos = getentityproperty(self,"animpos");
            float xdir = getentityproperty(self,"xdir");
            int dir = getentityproperty(self,"direction");
            int p = getentityproperty(self,"playerindex");
            char defaultmodel = getentityproperty(self,"defaultmodel");
            char model = getentityproperty(self,"model");
            float shift_speed = 0.5;
            int buffering_frame = 1;

            //drawstring(10,100,0,"model: "+model);
            if ( model == "FOOT_SOLDIER" ) {
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos < 4-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK2") && anim_pos < 6-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK3") && anim_pos < 3-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACK4") && anim_pos < 3-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                } else if ( anim_id == openborconstant("ANI_ATTACKBACKWARD") && anim_pos < 3-buffering_frame ) {
                    if ( xdir != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
                    return;
                }
            }

            if ( model != "player_foot_bow" && model != "player_foot_hammer" ) {
                if ( (!dir && playerkeys(p,0,"moveright")) || (dir && playerkeys(p,0,"moveright")) ) {
                    changeentityproperty(self,"velocity",shift_speed,NULL(),NULL());
                } else if ( (dir && playerkeys(p,0,"moveleft")) || (!dir && playerkeys(p,0,"moveleft")) ) {
                    changeentityproperty(self,"velocity",-1*shift_speed,NULL(),NULL());
                } else if ( getentityproperty(self,"xdir") != 0 ) changeentityproperty(self,"velocity",0,NULL(),NULL());
            }
    }

    return;
}

void check_bind_grab_falling(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_FOLLOW63") ) {
        int anim_pos = getentityproperty(player, "animpos");

        if ( anim_pos >= 3 && anim_pos <= 5 ) {
            float tossv = getentityproperty(player, "tossv");

            if ( tossv < 0 ) {
                tossv /= 1.08;
                changeentityproperty(player,"velocity",NULL(),NULL(),tossv);
            }
        }
    }

    /*if ( anim_id == openborconstant("ANI_DIE60") ) {
        int anim_pos = getentityproperty(player, "animpos");

        if ( anim_pos >= 3 ) {
            float tossv = getentityproperty(player, "tossv");

            if ( tossv < 0 ) {
                tossv /= 1.05;
                changeentityproperty(player,"velocity",NULL(),NULL(),tossv);
            }
        }
    }*/
}

void check_reset_new_grab_system(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_FOLLOW60") || anim_id == openborconstant("ANI_FOLLOW62") ) {
        void opp = getentityproperty(player,"opponent");

        //drawstring(10,210,0,"opp: "+getentityproperty(opp,"model"));
        if ( getentityproperty(opp,"exists") ) { // reset anim
            int o_anim_id = getentityproperty(opp, "animationid");

            if ( o_anim_id != openborconstant("ANI_FOLLOW60") && o_anim_id != openborconstant("ANI_FOLLOW62") ) {
                check_deactive_custom_grabbed(player);
            }
        }
    } else {
        check_deactive_custom_grabbed(player);
    }
}

void check_deactive_custom_grabbed(void player) {
    // Se sei stato custom grabbed...
    if ( getentityvar(player,28) != NULL() && getentityvar(player,28) ) {
        if ( getentityproperty(player,"noaicontrol") ) changeentityproperty(player,"noaicontrol",0);
        if ( getentityproperty(player,"nograb") != getentityvar(player,29) && getentityvar(player,29) != NULL() ) {
            changeentityproperty(player,"nograb",getentityvar(player,29));
            setentityvar(player,29,NULL());
        }
        setentityvar(player,28,NULL());
        if ( !getentityproperty(player,"aiflag","inpain") && !getentityproperty(player,"aiflag","falling") && !getentityproperty(player,"aiflag","dead") ) {
            int anim_id = getentityproperty(player, "animationid");

            if ( anim_id != openborconstant("ANI_FOLLOW63") && anim_id != openborconstant("ANI_FOLLOW64") ) setidle(player,openborconstant("ANI_IDLE"),1);
        }
    }
}

// per le tmnt3 moves
int check_safe_pos_moves(void self) {
    int anim_id = getentityproperty(self, "animationid");
    float x = getentityproperty(self,"x");
    float z = getentityproperty(self,"z");
    float a = getentityproperty(self,"y");
    float base = getentityproperty(self,"base");

    if ( anim_id == openborconstant("ANI_PAIN20") ) {
        int anim_pos = getentityproperty(self, "animpos");
        float threshold = 1;
        int unbind_frame = 4;

        if ( anim_pos <= unbind_frame-1 ) {
                void opp = getentityproperty(self,"opponent");

                if ( getentityproperty(opp,"exists") ) {
                    setlocalvar("temp_x",getentityproperty(self,"x"));
                    setlocalvar("temp_z",getentityproperty(self,"z"));
                    setlocalvar("temp_a",getentityproperty(self,"y"));
                }
        } else if ( anim_pos == unbind_frame ) {
                void opp = getentityproperty(self,"opponent");

                if ( getentityproperty(opp,"exists") ) {
                    void platform;
                    float pa = 0, ph = 0;
                    int platform_flag = 0;

                    if ( !getentityproperty(opp,"direction") ) threshold *= -1;

                    platform = checkplatformbelow(x+threshold,z,a);
                    if ( getentityproperty(platform,"exists") ) {
                        pa = getentityproperty(platform, "y");
                        ph = getentityproperty(platform, "height");
                        if ( a >= pa && a <= pa+ph ) platform_flag = 1;
                    }

                    if ( checkwall(x+threshold,z) > a || platform_flag ) {
                        changeentityproperty(self,"position",getlocalvar("temp_x"),getlocalvar("temp_z"),getlocalvar("temp_a"));
                    }
                }
        }
    } /*else if ( anim_id == openborconstant("ANI_PAIN36") ) {
        int anim_pos = getentityproperty(self, "animpos");
        float threshold = 1;

        if ( anim_pos <= 20 ) {
                void opp = getentityproperty(self,"opponent");

                if ( getentityproperty(opp,"exists") ) {
                    setlocalvar("temp_x",getentityproperty(opp,"x"));
                    setlocalvar("temp_z",getentityproperty(self,"z"));
                    setlocalvar("temp_a",getentityproperty(self,"y"));
                }
        } else if ( anim_pos == 21 ) {
                void opp = getentityproperty(self,"opponent");

                if ( getentityproperty(opp,"exists") ) {
                    void platform;
                    float pa = 0, ph = 0;
                    int platform_flag = 0;

                    if ( !getentityproperty(opp,"direction") ) threshold *= -1;

                    platform = checkplatformbelow(x+threshold,z,a);
                    if ( getentityproperty(platform,"exists") ) {
                        pa = getentityproperty(platform, "y");
                        ph = getentityproperty(platform, "height");
                        if ( a >= pa && a <= pa+ph ) platform_flag = 1;
                    }

                    if ( checkwall(x+threshold,z) > a || platform_flag ) {
                        changeentityproperty(self,"position",getlocalvar("temp_x"),getlocalvar("temp_z"),getlocalvar("temp_a"));
                    }
                }
        }
    }*/ // fine check
}

int check_foot_grab(void player, int default_grabforce) {
    int anim_id = getentityproperty(player, "animationid");
    void opp = getentityproperty(player, "opponent");
    void target = findtarget(player);

    if ( getentityproperty(target,"exists") ) {
        char defaultmodel = getentityproperty(target,"defaultmodel");
        int grabforce = getentityproperty(player,"grabforce");

        if ( defaultmodel != "LEO" && defaultmodel != "MIKE" && defaultmodel != "DON" && defaultmodel != "RAPH" ) {
            if ( get_grabforce(player) == NULL() ) set_grabforce(player,getentityproperty(player,"grabforce"));
            if ( grabforce != 0 ) changeentityproperty(player, "grabforce", 0); // -1??
        } else {
            if ( get_grabforce(player) != NULL() ) changeentityproperty(player, "grabforce", get_grabforce(player));
            else if ( grabforce != default_grabforce ) changeentityproperty(player, "grabforce", default_grabforce);
        }
    }

    // JUST-IN-CASE
    if ( getentityproperty(opp, "exists") && (anim_id == openborconstant("ANI_GRAB") || anim_id == openborconstant("ANI_FOLLOW41")) ) {
        char oname = getentityproperty(opp, "defaultmodel");

        if ( oname != "LEO" && oname != "MIKE" && oname != "DON" && oname != "RAPH" ) {
            damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
        }
    } // fine if exists
}

void set_grabforce(void self, int value) {
    setentityvar(self, 12, value);
}

int get_grabforce(void self) {
    return getentityvar(self,12);
}

int check_runjump(void player) {
    int running = getentityproperty(player,"aiflag","running");
    int jumping = getentityproperty(player,"aiflag","jumping");
    int anim_id = getentityproperty(player, "animationid");

    if ( running && jumping && anim_id == openborconstant("ANI_JUMP") ) {
        changeentityproperty(player,"animation",openborconstant("ANI_FOLLOW80"));
        //performattack(player,openborconstant("ANI_FOLLOW80"),1);
    }
}

int check_grab_pos(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_GRAB") || anim_id == openborconstant("ANI_FOLLOW41") ) {
        //void opp = getentityproperty(player, "opponent");
        void opp = get_grabber(player);

        if ( getentityproperty(opp, "exists") ) {
            float x = getentityproperty(player, "x");
            float z = getentityproperty(player, "z");
            float a = getentityproperty(player, "y");
            float base = getentityproperty(player, "base");
            float zo = getentityproperty(opp, "z");
            float t_dist = 1.01;

            if ( zo <= z ) {
                int hole = checkhole(x,z-t_dist);
                float wall_h = checkwall(x,z-t_dist);
                //void platform = find_platform_around_in_a(player,0,t_dist,a,MAX_ALTITUDE);
                void platform = find_platform_in_a(player,x,z-t_dist,a,MAX_ALTITUDE);
                float platform_base = get_platform_base(platform);

                // Se il grabber sta più in giù del nemico allora facciamo in modo che sia sempre su!!
                if ( wall_h == base && !hole && platform == NULL() ) {
                    if ( platform != NULL() ) {
                        if ( platform_base == base ) changeentityproperty(player, "position", NULL(), zo-t_dist, NULL());
                    } else changeentityproperty(player, "position", NULL(), zo-t_dist, NULL());
                } else {
                    //++zo;
                    changeentityproperty(player, "position", NULL(), zo-t_dist, NULL());
                    changeentityproperty(opp, "position", NULL(), zo, NULL());
                }
            }
        }
    } // fine if exists
}

int check_antigrab(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id != openborconstant("ANI_SPAWN") && anim_id != openborconstant("ANI_RESPAWN") ) {

        check_antigrab_for_board(player);
        if ( anim_id == openborconstant("ANI_FOLLOW41") || anim_id == openborconstant("ANI_GRAB") || getentityproperty(player, "invincible") != 0 ) {
                if ( getentityproperty(player, "nograb") != 1 ) changeentityproperty(player, "nograb", 1);
        } else {
            if ( getglobalvar("enemy_nograb") == NULL() || getglobalvar("enemy_nograb") != 1 ) {
                    if ( getentityproperty(player, "nograb") != getnograb(player) ) changeentityproperty(player, "nograb", getnograb(player));
            } else {
                if ( getentityproperty(player, "nograb") != 1 ) changeentityproperty(player, "nograb", 1);
            }
        }
    }
}

// reset dalla presa del soldato
int check_nomore_grabbing(void player) {
    void grabber = get_grabber(player);

    if ( grabber != NULL() && getentityproperty(grabber,"exists") ) { // Se in effetti il grabber è proprio se stesso...
        int opp_anim_id = getentityproperty(grabber, "animationid");
        int anim_id = getentityproperty(player, "animationid");

        if ( opp_anim_id != openborconstant("ANI_GRABBED") && opp_anim_id != openborconstant("ANI_FOLLOW41") && opp_anim_id != openborconstant("ANI_FOLLOW44")
            && opp_anim_id != openborconstant("ANI_PAIN") ) {
                    if ( getentityproperty(player,"type") == openborconstant("TYPE_PLAYER") ) {
                        int p = getentityproperty(player, "playerindex");

                        changeplayerproperty(p, "keys", 0);
                        changeplayerproperty(p, "playkeys", 0);
                    }

                    changeentityproperty(player, "noaicontrol", 0);
                    changeentityproperty(player, "nograb", getnograb(player));
                    changeentityproperty(player, "aiflag", "jumping", 0);
                    changeentityproperty(player, "aiflag", "running", 0);

                    bindentity(player, NULL());
                    setentityvar(player, 2, NULL()); // Annulliamo il tempo del grabber
                    set_grabber(player,NULL()); // Resettiamo il grabbed

                    if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1 ) {
                        changeentityproperty(player,"velocity",0,0,0);
                        if ( getentityproperty(player, "invincible") == 1 ) damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                        setidle(player,openborconstant("ANI_IDLE"),1);
                    }

        }

    } // fine if grabber == player

}

int getnograb(void self) {
    return getentityvar(self,10);
}

void set_grabber(void player, void grabber) {
    setentityvar(player,3,grabber);
    return;
}

void get_grabber(void player) {
    return getentityvar(player,3);
}

// se il player non vuole più grabbare: tieni premuto freccia direz- per non grabbare
int check_grabbing(void self) {
    int anim_id = getentityproperty(self, "animationid");

    //Può scappare con avanti
    if ( anim_id == openborconstant("ANI_GRAB") ) {
        int dir = getentityproperty(self, "direction");
        void opp = getentityproperty(self, "opponent");
        int time = openborvariant("elapsed_time");
        int game_speed = openborvariant("game_speed");
        int damage_time = game_speed*2;
        int damage = 8;
        int escape_time = game_speed*1.1;
        void grabber = get_grabber(self);

        if ( grabber != opp ) set_grabber(self,opp);

        // Facciamo in modo da togliersi dalla presa premendo il tasto nella direzione opposta per pochi secondi -> playerkeys 0==hold, 1==press, 2==release
        // Dunque assicuriamoci prima che quest'entità sia effetivamente guidata da un giocatore...
        if ( getentityproperty(self,"type") == openborconstant("TYPE_PLAYER") ) {
            int p = getentityproperty(self, "playerindex");

            if ( getplayerproperty(p, "entity") == self ) {
                // Disabilitiamo tutti i tasti tranne quelli importanti per scappare
                if ( (dir && playerkeys(p,0,"moveleft") != 4) || (!dir && playerkeys(p,0,"moveright") != 8) ) changeplayerproperty(p, "playkeys", 0);

                // Check Time-Reset
                if ( check_stored_time(getlocalvar("escape_timer"),9999) ) setlocalvar("escape_timer", openborconstant("MIN_INT"));
                if ( (dir && playerkeys(p,0,"moveleft") == 4) || (!dir && playerkeys(p,0,"moveright")) ) {
                    if ( getlocalvar("escape_timer") == NULL() ) setlocalvar("escape_timer",get_next_time(escape_time));
                    else if ( getlocalvar("escape_timer") < time ) {
                        changeentityproperty(self,"noaicontrol",0);
                        changeentityproperty(self, "nograb",0);
                        setentityvar(self, 2, NULL());
                        set_grabber(self,NULL());
                        setidle(self,openborconstant("ANI_IDLE"),1);

                        setlocalvar("escape_timer",NULL());
                    }
                }
                else if ( (dir && playerkeys(p,0,"moveleft") != 4) || (!dir && playerkeys(p,0,"moveright") != 8) ) setlocalvar("escape_timer",NULL()); // resettiamo il flag se non premiamo più i tasti
            } // fine if p == self
        } // fine if type player

        // Facciamo perdere al player grabbed una certa quantità di HP ogni damage_time frames...
        if ( opp != NULL() && getentityproperty(opp,"exists") ) {
            if ( get_grabber(opp) == self ) { // se l'avversario dell'opponente è effettivamente questo nemico...

                // Check Time-Reset
                if ( check_stored_time(getentityvar(self,2),9999) ) setentityvar(self, 2, openborconstant("MIN_INT"));
                if ( getentityvar(self,2) == NULL() ) setentityvar(self,2,get_next_time(damage_time));
                else if ( getentityvar(self,2) < time ) {
                    if ( getentityproperty(opp, "health") >= damage+1 ) changeentityproperty(opp,"health",getentityproperty(opp, "health")-damage);
                    else { // se il player muore...
                        changeentityproperty(self,"noaicontrol",0);
                        changeentityproperty(self, "nograb",0);
                        bindentity(self, NULL());
                        set_grabber(self,NULL());
                        if ( !is_in_pain(self) ) {
                            setidle(self,openborconstant("ANI_IDLE"),1); // grabber
                        }
                        if ( !is_in_pain(opp) ) {
                            damageentity(opp,self,damage,0,0,openborconstant("ATK_NORMAL"));
                        }

                        //give_score(self,opp); // give score to player (if exists)
                        check_versus_mark(self,opp); // give versus mark to player against opp
                    }
                    setentityvar(self,2,NULL());
                }
            }
        } // fine if opp

    } // fine if ANI_GRAB

}

int check_walkoff(void player) {
    float x = getentityproperty(player,"x");
    float z = getentityproperty(player,"z");
    float a = getentityproperty(player,"y");
    int anim_id = getentityproperty(player, "animationid");

    // Controlliamo se stiamo cadendo in un buco. Se si cambiamo animazione e non la solita walkoff
    if ( anim_id == openborconstant("ANI_WALKOFF") && checkhole(x,z) != 0 && a <= 0 ) {
        changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW16"));
    }
    // Controlliamo se stiamo cadendo in un buco dopo un wallsplat
    if ( ( anim_id == openborconstant("ANI_FOLLOW32") || anim_id == openborconstant("ANI_FOLLOW33") || anim_id == openborconstant("ANI_RISE10") || anim_id == openborconstant("ANI_DIE10") )
        && checkhole(x,z) != 0 && a <= 0 ) {
        int dir = getentityvar(player,9);

        if ( dir == 0 ) {
            if ( dir == 0 ) changeentityproperty(player, "direction", 1);
            else changeentityproperty(player, "direction", 0);
            changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW16"));
        } else {
            changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW16"));
        }

    }
}

int check_throw(void player) { // Facciamo in modo che ci sia un'animazione scelta sulla throw!!
    if ( getentityproperty(player, "animationid") == openborconstant("ANI_FALL") && getentityvar(player,5) == 1 ) {
        setentityvar(player,5,NULL());
        if ( getentityproperty(player, "health") > 0 ) {
            //if ( getentityproperty(player, "direction") == 0 ) performattack(player, openborconstant("ANI_FALL34"), 1);
            //else performattack(player, openborconstant("ANI_FALL34"), 1);
            changeentityproperty(player, "animation", openborconstant("ANI_FALL33"));
        }
    }
}

int check_falldie(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( getentityproperty(player, "health") <= 0 ) {
        if ( anim_id == openborconstant("ANI_FALL12") ) {
                changeentityproperty(player, "falldie", 1); // dopo la morte si ripristina!
                changeentityproperty(player, "animation", openborconstant("ANI_DIE12") );
                //changeentityproperty(player, "aiflag", "falling",0);
                //changeentityproperty(player, "aiflag", "dead",1);
                //changeentityproperty(player,"nextthink",0);
                //changeentityproperty(player,"nextmove",0);
                //changeentityproperty(player,"nextanim",0);
                //changeentityproperty(player,"tosstime",0);
        }
    }
}

int check_tmnt3_move(void self) {
    if ( getentityproperty(self, "animationid") == openborconstant("ANI_PAIN20") ) {
        void opp = getentityproperty(self, "opponent");

        if ( getentityproperty(opp, "exists") ) {
            void oanim_id = getentityproperty(opp, "animationid");

            if ( oanim_id != openborconstant("ANI_FREESPECIAL9") ) bindentity(self,NULL());
        }
    }
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
                    || gep(ent, "animationid") == oc("ANI_FOLLOW35")|| gep(ent, "animationid") == oc("ANI_FOLLOW36")|| gep(ent, "animationid") == oc("ANI_FOLLOW37") ) {
                    result = getentityvar(ent,0); // Se avviene wall splat non consideriamo le relative animazioni... restituiamo quindi quello che c'era prima!
                } else result = 2;
            }
        } else result = NULL();

        return result;
    }

    return getentityvar(ent,0);
}

/*
 * 0 = going up
 * 1 = going down
 * 2 = stop
 * NULL = not detectable
 */
int getwalkingzdir(void ent, int frames) {
    float zdir = getentityproperty(ent,"zdir");
    int result = NULL();

    if ( zdir != NULL() ) {
            if ( zdir > 0 ) {
                result = 1;
            } else if (zdir < 0 ) {
                result = 0;
            } else {
                result = 2;
            }
    } else result = NULL();

    return result;

    //return getentityvar(ent, 2);
}

int checkwalls(void player, int distx, int distz) {
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


