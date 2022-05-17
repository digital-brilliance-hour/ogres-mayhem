
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/lib_platform.c"

#define ESCAPE_TIME openborvariant("game_speed")*1

/*
   TODO:
         test opp non colpito (l'animazione deve finire)
         test opp non muore/muore
*/

// MAIN FUNCTION
int check_bind_grab(void player) {

    /// ############## ACTIVE CUSTOM GRAB SYSTEM ##############
    /*if ( gep(player, "animationid") == oc("ANI_GRAB") ) {
        set_custom_grab(player);
    }*/
    //drawstring(10,200,0,"grabbin: "+gep(get_grabbed_opp(self),"model"));

    check_bind_grab_coords(player); // Controlla se i players sono sovrapposti
    check_bind_attached(player); // Controlla se i players sono abbracciati

    check_new_grab_system(player); // NEW GRAB SYSTEM

    check_unbind_grab(player); // Controlla se i personaggi devono essere staccati dalla bind grab: IMPORTANTE!!
}

int check_bind_attached(void player) {
    int anim_id = gep(player, "animationid");

    if ( anim_id == oc("ANI_FOLLOW62") ) {
        void partner = get_bindgrab_partner(player);

        if ( gep(partner,"exists") ) {
            int p = gep(player, "playerindex");
            int pp = gep(partner, "playerindex");
            void opp = gep(partner,"opponent");

            if ( gep(opp,"exists") ) {
                char cText = gep(opp, "defaultmodel");
                char cFind = "rock_soldier";
                char cFind2 = "ROCK_SOLDIER";
                char substr = strinfirst(cText, cFind);
                char substr2 = strinfirst(cText, cFind2);

                // check escape from grab and set normal new grab the partner!!
                if ( check_escape_grab(player) ) {
                    unbind_grab(partner);
                    unbind_grab(player);
                    set_custom_grab(partner);
                }

                // Check almost one player ready
                if ( (playerkeys(p,0,"attack") && !playerkeys(pp,0,"attack")) || (!playerkeys(p,0,"attack") && playerkeys(pp,0,"attack")) ) {
                    if ( ggv("bind_grab_ready"+p) == NULL() ) {
                       void ent;

                       clearspawnentry();
                       setspawnentry("name", "bind_grab_ready");
                       ent = spawn();
                       if ( !playerkeys(p,0,"attack") && playerkeys(pp,0,"attack") ) changeentityproperty(ent, "parent", partner);
                       else changeentityproperty(ent, "parent", player);
                       sgv("bind_grab_ready"+p, ent);
                    }
                } else {
                    if ( getglobalvar("bind_grab_ready"+p) != NULL() && getentityproperty(getglobalvar("bind_grab_ready"+p), "exists") ) { // Eliminiamo l'animazione pulsante
                        killentity(getglobalvar("bind_grab_ready"+p));
                    }
                    setglobalvar("bind_grab_ready"+p, NULL());
                }

                if ( playerkeys(p,0,"moveup") || playerkeys(pp,0,"moveup") ) {
                    if ( (playerkeys(p,0,"attack") && playerkeys(pp,1,"attack")) || (playerkeys(pp,0,"attack") && playerkeys(p,1,"attack")) ) { // ESEGUIAMO LA MOSSA BIND!!
                        // if opp is ROCK
                        if ( substr != -1 || substr2 != -1 ) {
                            performattack(opp,oc("ANI_FOLLOW63"),1);
                        } else {
                            performattack(opp,oc("ANI_FOLLOW63"),1);
                        }

                        // Remain partner vars
                        unbind_grab(player,1);
                        unbind_grab(partner,1);
                        reset_bind_grab_vars(player);
                        reset_bind_grab_vars(partner);
                        performattack(player,oc("ANI_FOLLOW63"),1);
                        performattack(partner,oc("ANI_FOLLOW63"),1);
                    }
                } else {
                    if ( (playerkeys(p,0,"attack") && playerkeys(pp,1,"attack")) || (playerkeys(pp,0,"attack") && playerkeys(p,1,"attack")) ) { // ESEGUIAMO LA MOSSA BIND!!
                        performattack(opp,oc("ANI_FOLLOW64"),1);
                        performattack(player,oc("ANI_FOLLOW67"),1);
                        performattack(partner,oc("ANI_FOLLOW67"),1);

                        // Remain partner vars
                        unbind_grab(player,1);
                        unbind_grab(partner,1);
                        reset_bind_grab_vars(player);
                        reset_bind_grab_vars(partner);
                    }
                }
            } // fine if opp exists
        } // fine if partner exists
    } // fine if follow62
}

int check_bind_grab_coords(void player) {
    int anim_id = gep(player, "animationid");
    int i;

    if ( ov("count_players") > 1 && anim_id == oc("ANI_IDLE") && getglobalvar("board_level") == NULL() && getglobalvar("player_nograb") == NULL() ) {
        for ( i = 0; i < MAX_PLAYERS; ++i ) {
            void friend_ent = gpp(i, "entity");

            if ( friend_ent == player ) continue;
            if ( getentityproperty(friend_ent, "type") != openborconstant("TYPE_PLAYER") ) continue;
            if ( gep(friend_ent, "defaultmodel") != "LEO" && gep(friend_ent, "defaultmodel") != "DON" && gep(friend_ent, "defaultmodel") != "MIKE" && gep(friend_ent, "defaultmodel") != "RAPH" ) continue;
            if ( gep(friend_ent, "exists") ) {
                void f_opp = gep(friend_ent,"opponent");

                if ( gep(f_opp,"exists") ) {
                    float x = gep(player, "x");
                    float z = gep(player, "z");
                    float a = gep(player, "y");
                    float base = gep(player, "base");
                    float xf = gep(friend_ent, "x");
                    float zf = gep(friend_ent, "z");
                    float af = gep(friend_ent, "y");
                    float basef = gep(friend_ent, "base");
                    float xo = gep(f_opp, "x");
                    float zo = gep(f_opp, "z");
                    float ao = gep(f_opp, "y");
                    float baseo = gep(f_opp, "base");

                    if ( x >= xo-30 && x <= xo+30 && z >= zo-10 && z <= zo+10 && a == ao ) {
                        int f_anim_id = gep(friend_ent, "animationid");
                        //if ( gep(player, "playerindex") == 0 ) drawstring( 10,170,0,"Var ():  "+get_anim_time(player));

                        if ( f_anim_id == oc("ANI_GRAB") || f_anim_id == oc("ANI_FOLLOW60") ) {
                            int p = gep(player, "playerindex");
                            int pp = gep(friend_ent, "playerindex");
                            int dir = gep(player, "direction");
                            int dirf = gep(friend_ent, "direction");
                            char cText = gep(f_opp, "defaultmodel");
                            char cFind = "rock_soldier";
                            char cFind2 = "ROCK_SOLDIER";
                            char substr = strinfirst(cText, cFind);
                            char substr2 = strinfirst(cText, cFind2);
                            float threshold = 46, t_z = 2+2, t_z_givemefive = 10+2, t_x_givemefive = 20, t_x_dmf1,t_x_dmf2;
                            float xoshift = 6;

                            //drawstring(10,200,0,"GRABBED: ");

                            if ( !gep(f_opp,"animvalid",oc("ANI_FOLLOW62")) ) return;

                            if ( substr != -1 || substr2 != -1 ) {
                                threshold += 28;
                                xoshift += 13;
                            }

                            // friend_ent == leader
                            if ( !dirf ) threshold *= -1; // se il leader è girato a sx allora check su distanza a sx rispetto a opp
                            if ( !is_grab_safe_space(xf,zf,af,basef,x,z,a,base) ) return;
                            if ( !is_grab_safe_space(xo,zo,ao,baseo,xo+threshold,zo,ao,baseo) ) return;
                            if ( !is_grab_safe_space(xo,zo,ao,baseo,xo+threshold,zo-t_z,ao,baseo) ) return;

                            if ( dirf ) t_x_dmf1 = -1*t_x_givemefive;
                            else t_x_dmf1 = t_x_givemefive;
                            if ( dir ) t_x_dmf2 = -1*t_x_givemefive;
                            else t_x_dmf2 = t_x_givemefive;

                            if ( !is_grab_safe_space(xf+t_x_dmf1,zf,af,basef,x+t_x_dmf2,z-t_z_givemefive,a,base) ) return; // for givemefive

                            setentityvar(f_opp,14,NULL());
                            reset_bind_grab_vars(player);
                            reset_bind_grab_vars(friend_ent);
                            cep(player, "opponent", f_opp);

                            if ( ggv("custom_grab_active_flag"+p) ) deactive_custom_grab(player);
                            if ( ggv("custom_grab_active_flag"+pp) ) deactive_custom_grab(friend_ent);
                            cep(player, "velocity", 0,0,0);
                            cep(friend_ent, "velocity", 0,0,0);
                            sgv("tmp_no_bind_grab"+p, gep(player,"nograb"));
                            sgv("tmp_no_bind_grab"+pp, gep(player,"nograb"));
                            cep(player, "nograb", 1);
                            cep(friend_ent, "nograb", 1);
                            set_bindgrab_type(friend_ent, "leader");
                            set_bindgrab_type(player, "partner");
                            set_bindgrab_partner(player, friend_ent);
                            set_bindgrab_partner(friend_ent, player);
                            set_main_opp(player,f_opp);
                            set_main_opp(friend_ent,f_opp);
                            cep(player,"noaicontrol",1);
                            cep(friend_ent,"noaicontrol",1);
                            cep(f_opp,"noaicontrol",1);
                            setentityvar(f_opp,28,1); // set custom grab flag for noaicontrol
                            setentityvar(f_opp,29,gep(f_opp,"nograb")); // set tmp_nograb for custom grab flag
                            cep(f_opp,"nograb",1);

                            damageentity(friend_ent,friend_ent,0,0,openborconstant("ATK_NORMAL60")); // RESET NORMAL GRAB!!
                            damageentity(f_opp,f_opp,0,0,openborconstant("ATK_NORMAL60")); // RESET NORMAL GRAB!!
                            if ( !dirf ) xoshift *= -1;
                            cep(player,"position",xo+threshold,zf,af);
                            if ( f_anim_id == oc("ANI_GRAB") ) {
                                cep(f_opp,"position",xo+xoshift,zo-1,NULL());
                            } else cep(f_opp,"position",xo+xoshift,zo,NULL());
                            cep(player,"direction",!dirf);

                            performattack(f_opp, oc("ANI_FOLLOW62"), 1);
                            performattack(friend_ent, oc("ANI_FOLLOW62"), 1);
                            performattack(player, oc("ANI_FOLLOW62"), 1);

                            return;
                        } // fine if GRAB???
                    } // fine if coord
                } // fine if f_opp exists
            } // fine if friend_ent exists
        } // fine for
    } // fine if idle

}

void check_new_grab_system(void player) {
    int anim_id = gep(player, "animationid");

    if ( anim_id == oc("ANI_FOLLOW60") ) {
        int p = gep(player, "playerindex");
        int dir = gep(player,"direction");
        void opp = gep(player,"opponent");

        if ( (playerkeys(p,0,"moveright") && dir) || (playerkeys(p,0,"moveleft") && !dir) ) {
            if ( playerkeys(p,1,"attack") ) {
                performattack(player,oc("ANI_GRABFORWARD"),1);
                return;
            }
        } else if ( (playerkeys(p,0,"moveright") && !dir) || (playerkeys(p,0,"moveleft") && dir) ) {
            if ( playerkeys(p,1,"attack") ) {
                performattack(player,oc("ANI_FOLLOW61"),1);
                return;
            }
        } else if ( playerkeys(p,0,"moveup") ) {
            if ( playerkeys(p,1,"attack") ) {
                performattack(player,oc("ANI_GRABUP"),1);
                return;
            }
        } else if ( playerkeys(p,0,"movedown") ) {
            if ( playerkeys(p,1,"attack") ) {
                performattack(player,oc("ANI_GRABDOWN"),1);
                return;
            }
        } else if ( playerkeys(p,1,"attack") ) {
            performattack(player,oc("ANI_GRABATTACK"),1);
            return;
        }

        check_escape_grab(player); // check escape from grab

        if ( gep(opp,"exists") ) { // reset anim
            int o_anim_id = gep(opp, "animationid");

            if ( o_anim_id != oc("ANI_FOLLOW60") ) {
                if ( !gep(player,"aiflag","inpain") && !gep(player,"aiflag","falling") && !gep(player,"aiflag","dead") ) setidle(player,oc("ANI_IDLE"),1);
                //reset_bind_grab_vars(player);
                return;
            }
        }
    } else {
        int p = gep(player, "playerindex");

        if ( ggv("custom_grab_active_flag"+p) != NULL() && ggv("custom_grab_active_flag"+p) ) {
            deactive_custom_grab(player);
        }
    }
}

int check_escape_grab(void player) {
    int p = gep(player, "playerindex");
    int dir = gep(player,"direction");
    int time = ov("elapsed_time");

    // Check Time-Reset
    if ( check_stored_time(glv("escape_timer"),9999) ) slv("escape_timer",openborconstant("MIN_INT"));
    // ESCAPE
    if ( dir && playerkeys(p,0,"moveleft") ) {
        if ( glv("escape_timer") == NULL() ) setlocalvar("escape_timer",get_next_time(ESCAPE_TIME));
        else if ( glv("escape_timer") < time ) {
            if ( !gep(player,"aiflag","inpain") && !gep(player,"aiflag","falling") && !gep(player,"aiflag","dead") ) setidle(player,oc("ANI_IDLE"),1);
            return 1;
        }
    } else if ( !dir && playerkeys(p,0,"moveright") ) {
        if ( glv("escape_timer") == NULL() ) slv("escape_timer",get_next_time(ESCAPE_TIME));
        else if ( glv("escape_timer") < time ) {
            if ( !gep(player,"aiflag","inpain") && !gep(player,"aiflag","falling") && !gep(player,"aiflag","dead") ) setidle(player,oc("ANI_IDLE"),1);
            return 1;
        }
    } else if ( dir && !playerkeys(p,0,"moveleft") && glv("escape_timer") != NULL() ) slv("escape_timer",NULL()); // resettiamo il flag se non premiamo più i tasti
    else if ( !dir && !playerkeys(p,0,"moveright") && glv("escape_timer") != NULL() ) slv("escape_timer",NULL());

    return 0;
}

void set_custom_grab(void player) {
    void opp = gep(player,"opponent");
    //void grabbed_opp = gep(player,"grabbing");

    if ( gep(opp,"exists") ) {
        int p = gep(player, "playerindex");
        float x = gep(player, "x");
        float z = gep(player, "z");
        int dir = gep(player,"direction");
        float xoshift = 34;

        cep(player,"velocity",0,0,0);
        cep(opp,"velocity",0,0,0);
        bindentity(player,NULL());
        bindentity(opp,NULL());

        cep(opp,"opponent",player);
        cep(opp,"direction",!dir);
        if ( !dir ) xoshift *= -1;

        cep(player,"noaicontrol",1);
        cep(player,"noaicontrol",1);
        damageentity(opp,opp,0,0,openborconstant("ATK_NORMAL60")); // reset (optional)
        damageentity(player,player,0,0,openborconstant("ATK_NORMAL60")); // reset
        setentityvar(opp,28,1); // set custom grab flag for noaicontrol
        setentityvar(opp,29,gep(opp,"nograb")); // set tmp_nograb for custom grab flag
        cep(opp,"nograb",1);

        // TRICK: to set player OVER opp, player performattack AFTER opp
        //cep(player,"animation",oc("ANI_FOLLOW60"));
        //cep(opp,"animation",oc("ANI_FOLLOW60"));
        performattack(opp,oc("ANI_FOLLOW60"),1);
        performattack(player,oc("ANI_FOLLOW60"),1);

        cep(opp,"position",x+xoshift,z-1,NULL());

        set_grabbed_opp(player,opp);

        sgv("tmp_no_new_grab"+p, gep(player,"nograb"));
        cep(player, "nograb", 1);
        sgv("custom_grab_active_flag"+p,1);
    }
}

int deactive_custom_grab(void player) {
    int p = gep(player, "playerindex");

    if ( gep(player,"noaicontrol") ) cep(player,"noaicontrol",0);

    if ( ggv("tmp_no_new_grab"+p) != NULL() && gep(player,"nograb") != ggv("tmp_no_new_grab"+p) ) {
        cep(player,"nograb",ggv("tmp_no_new_grab"+p));
        sgv("tmp_no_new_grab"+p,NULL());
    }
}

int is_grab_safe_space(float x1, float z1, float a1, float base1, float x2, float z2, float a2, float base2) {
    float t_alt = 1, t_alt_platform = 40;
    float wall_pl, wall_pf;
    void platform1, platform2;
    float platform1_a, platform2_a;


        if ( a1 != a2 ) return 0;
        if ( base1 != base2 ) return 0;

        // CHECK BOUNDS
        if ( is_in_horizontal_bounds(x1,z1) ) return 0;
        if ( is_in_horizontal_bounds(x2,z2) ) return 0;

        // CHECK WALL/PLATFORM SPACE
        // se siamo su un muro/piattaforma ch'è abbastanza spazio??
        // ------- CHECK WALL -------
        wall_pl = checkwall(x1,z1);
        wall_pf = checkwall(x2,z2);
        if ( wall_pl != wall_pf ) return 0; // se stiamo su 2 altezze diverse niente bind!!
        // ------- CHECK PLATFORM -------
        wall_pl = 0; wall_pf = 0;
        if ( is_on_platform_by_coords(x1,z1,a1,base1) != is_on_platform_by_coords(x2,z2,a2,base2) ) return 0;
        if ( is_on_platform_by_coords(x1,z1,a1,base1) ) wall_pl = get_platform_on_alt_by_coords(x1,z1,a1,base1);
        if ( is_on_platform_by_coords(x2,z2,a2,base2) ) wall_pf = get_platform_on_alt_by_coords(x2,z2,a2,base2);
        if ( wall_pl != wall_pf ) return 0;
        if ( get_platform_a_plus_h_in_xza(x1,z1,a1+t_alt,base1) != get_platform_a_plus_h_in_xza(x2,z2,a2+t_alt,base2) ) return 0;

        // CHECK IF YOU ARE "IN" A PLATFORM
        platform1 = find_platform_in_a(x1,z1,a1,MAX_ALTITUDE);
        platform2 = find_platform_in_a(x2,z2,a2,MAX_ALTITUDE);
        if ( platform1 != NULL() ) {
            platform1_a = getentityproperty(platform1,"y");

            if ( platform1_a == a1 ) return 0;
        }
        if ( platform2 != NULL() ) {
            platform2_a = getentityproperty(platform2,"y");

            if ( platform2_a == a2 ) return 0;
        }

        return 1;
}

int check_unbind_grab(void player) {
    int anim_id = gep(player, "animationid");

    if ( anim_id != oc("ANI_FOLLOW62") && anim_id != oc("ANI_FOLLOW63") && anim_id != oc("ANI_FOLLOW64") && anim_id != oc("ANI_FOLLOW65") && anim_id != oc("ANI_FOLLOW66") && anim_id != oc("ANI_FOLLOW67") ) {
        if ( get_bindgrab_type(player) != NULL() || get_bindgrab_partner(player) != NULL() ) {
            unbind_grab(player);
            return 1;
        }
    } else if ( anim_id == oc("ANI_FOLLOW62") ) {
        void opp = gep(player,"opponent");
        void partner = get_bindgrab_partner(player);

        if ( gep(opp,"exists") && gep(partner,"exists") ) {
            float a = getentityproperty(player,"y");
            float oa = getentityproperty(opp,"y");
            float pa = getentityproperty(partner,"y");

            if ( a != oa || a != pa || pa != oa ) {
                unbind_grab(player);
                if ( !gep(player,"aiflag","inpain") && !gep(player,"aiflag","falling") && !gep(player,"aiflag","dead") ) setidle(player,oc("ANI_IDLE"),1);
                return 1;
            }
        }

        if ( gep(opp,"exists") ) {
            int o_anim_id = gep(opp, "animationid");

            if ( o_anim_id != oc("ANI_FOLLOW62") ) {
                unbind_grab(player);
                if ( !gep(player,"aiflag","inpain") && !gep(player,"aiflag","falling") && !gep(player,"aiflag","dead") ) setidle(player,oc("ANI_IDLE"),1);
                return 1;
            }
        } // fine if opp exists

        if ( gep(partner,"exists") ) {
            int p_anim_id = gep(partner, "animationid");

            if ( p_anim_id != oc("ANI_FOLLOW62") ) {
                unbind_grab(player);
                if ( !gep(player,"aiflag","inpain") && !gep(player,"aiflag","falling") && !gep(player,"aiflag","dead") ) setidle(player,oc("ANI_IDLE"),1);
                return 1;
            }
        } // fine if opp exists
    }

    return 0;
}

int unbind_grab(void player, int partner_flag) {
        int p = gep(player, "playerindex");
        void partner = get_bindgrab_partner(player);

        if ( partner != NULL() ) {
            int anim_id_partner = gep(partner, "animationid");
            int pp = gep(partner, "playerindex");

            if ( ggv("tmp_no_bind_grab"+pp) != NULL() && ggv("tmp_no_bind_grab"+pp) != gep(partner,"nograb") ) {
                cep(partner, "nograb", ggv("tmp_no_bind_grab"+pp));
                sgv("tmp_no_bind_grab"+pp,NULL());
            }
            bindentity(partner, NULL());
            if ( partner_flag == NULL() || !partner_flag ) {
                set_bindgrab_type(partner, NULL());
                set_bindgrab_partner(partner, NULL());
            }
            sev(partner, 1, NULL()); // time
            if ( gep(partner,"noaicontrol") ) cep(partner,"noaicontrol",0);
            reset_bind_grab_vars(partner);
        }

        if ( partner_flag == NULL() || !partner_flag ) {
            set_bindgrab_type(player, NULL());
            set_bindgrab_partner(player, NULL());
        }
        bindentity(player, NULL());
        if ( ggv("tmp_no_bind_grab"+p) != NULL() && ggv("tmp_no_bind_grab"+p) != gep(player,"nograb") ) {
            cep(player, "nograb", ggv("tmp_no_bind_grab"+p));
            sgv("tmp_no_bind_grab"+p,NULL());
        }
        sev(player, 1, NULL()); // time
        if ( gep(player,"noaicontrol") ) cep(player,"noaicontrol",0);
        reset_bind_grab_vars(player);
}

void set_main_opp(void self, void opp) {
    setentityvar(self,27,opp);
}

char get_main_opp(void self) {
    return getentityvar(self,27);
}

void set_bindgrab_partner(void self, void partner) {
    int p = getentityproperty(self, "playerindex");
    setglobalvar("bindgrab_partner"+p, partner);
}

void get_bindgrab_partner(void self) {
    int p = getentityproperty(self, "playerindex");
    return getglobalvar("bindgrab_partner"+p);
}

void set_bindgrab_type(void self, void type) {
    int p = getentityproperty(self, "playerindex");
    setglobalvar("bindgrab_type"+p, type);
}

void get_bindgrab_type(void self) {
    int p = getentityproperty(self, "playerindex");
    return getglobalvar("bindgrab_type"+p);
}

void reset_bind_grab_vars(void self) {
    int p = getentityproperty(self, "playerindex");

    slv("escape_timer",NULL());
    set_grabbed_opp(self,NULL());
    sev(self,26,NULL()); // feedback from turtle_bind_grab_ent
    //set_main_opp(self,NULL());
    if ( ggv("bind_grab_ready"+p) != NULL() && gep(ggv("bind_grab_ready"+p), "exists") ) { // Eliminiamo l'animazione pulsante
        killentity(ggv("bind_grab_ready"+p));
    }
    sgv("bind_grab_ready"+p, NULL());
}

void set_grabbed_opp(void self,void opp) {
    sev(self,"grabbed_opp",opp);
    changeentityproperty(self,"opponent",opp);
    changeentityproperty(opp,"opponent",self);
}

void get_grabbed_opp(void self) {
    if ( !gep(gev(self,"grabbed_opp"),"exists") ) sev(self,"grabbed_opp",NULL());

    return gev(self,"grabbed_opp");
}
