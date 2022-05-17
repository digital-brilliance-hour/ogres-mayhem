
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/lib_platform.c"

#define BIND_TIME openborvariant("game_speed")*1.5
#define DMG_ENT 10 // enemy+obstacle (+npc = 512+10)
#define ESCAPE_TIME openborvariant("game_speed")*1
#define INV_TIME 1000
#define HITBYID_RESET_TIME openborvariant("game_speed")*1

/*
 *
 */
void main() {
  //void self = getlocalvar("self");


}

int check_bind_move(void player) {
    /*if ( gep(player, "playerindex") == 0 ) drawstring( 10,170,0,"Var (0):  "+gpp(0, "keys"));
    if ( gep(player, "playerindex") == 1 ) drawstring( 10,180,0,"Var (1):  "+gpp(1, "keys"));*/

    /*if ( gep(player, "playerindex") == 0 ) drawstring( 10,170,0,"Var (0):  "+get_bindtype(player));
    if ( gep(player, "playerindex") == 0 ) drawstring( 10,180,0,"Var (0):  "+get_bindpartner(player));
    if ( gep(player, "playerindex") == 1 ) drawstring( 10,190,0,"Var (1):  "+get_bindtype(player));
    if ( gep(player, "playerindex") == 1 ) drawstring( 10,200,0,"Var (1):  "+get_bindpartner(player));*/

    check_bind_coords(player); // Controlla se i players sono sovrapposti
    check_bind_attached(player); // Controlla se i players sono abbracciati
    check_bind_movement(player); // Controlla la mossa
    //check_bind_animation(player); // Controlli d'animazione della mossa
    check_unbind(player); // Controlla se i personaggi devono essere staccati: IMPORTANTE!!
}

int check_bind_animation(void player) { // per un aggiornamento più veloce
    if ( gep(player,"animationid") == oc("ANI_FOLLOW22") ) {
        if ( get_bindpartner(player) != NULL() ) {
            int p = gep(player, "playerindex");
            void partner = get_bindpartner(player);


            if ( get_bindtype(player) == "leader" ) {
                int animpos = getentityproperty(player, "animpos");

                switch (animpos) {
                    case 0:
                        bindentity(player, partner, -6, -1, 1, 1, 1);
                        break;
                    case 1:
                        // Non utilizzare mai un valore negativo su a di binding
                        bindentity(player, partner, 2, 1, 0, 1, 1);
                        break;
                    case 2:
                        bindentity(player, partner, 5, 1, 0, 1, 1);
                        break;
                    case 3:
                        bindentity(player, partner, 5, 1, 0, 1, 1);
                        break;
                    case 4:
                        bindentity(player, partner, -5, -1, 2, 1, 1);
                        break;
                    case 5:
                        bindentity(player, partner, -3, -1, 0, 1, 1);
                        break;
                    default:
                        break;
                }
            } else if ( get_bindtype(player) == "partner" ) {
                        int animpos_partner = getentityproperty(partner, "animpos");
                        int animpos = (animpos_partner+3)%(5+1);

                        changeentityproperty(player, "animpos", animpos);
                    } // fine if leader
            } // fine if  partner
    } // fine if follow22
}

int check_bind_movement(void player) {

    if ( gep(player,"animationid") == oc("ANI_FOLLOW22") ) {
        void partner = get_bindpartner(player);

        if ( partner != NULL() && gep(partner,"exists") ) {
            float x = gep(player, "x");
            float z = gep(player, "z");
            float a = gep(player, "y");
            float base = gep(player, "base");
            float xf = gep(partner, "x");
            float zf = gep(partner, "z");
            float af = gep(partner, "y");
            float basef = gep(partner, "base");
            float xdir = gep(player,"xdir");
            float zdir = gep(player,"zdir");
            float tossv = gep(player,"tossv");
            float speed = gep(player, "speed");
            int p = gep(player, "playerindex");
            void opp = gep(player, "opponent");
            int eta = INV_TIME;
            int pp = gep(partner, "playerindex");

            // Reset hitbyid after a lot of time
            if ( getentityproperty(opp,"exists") ) {
                if ( getlocalvar("hitbyid_opp") != opp ) {
                    if ( getentityproperty(getlocalvar("hitbyid_opp"),"exists") ) {
                        if ( getentityproperty(getlocalvar("hitbyid_opp"),"hitbyid") != 0 ) changeentityproperty(getlocalvar("hitbyid_opp"), "hitbyid", 0);
                    }
                }
                setlocalvar("hitbyid_opp",opp);

                if ( getentityproperty(opp,"hitbyid") != 0 && getlocalvar("bind_time") == NULL() ) {
                    setlocalvar("bind_time", openborvariant("elapsed_time")+HITBYID_RESET_TIME);
                } else if ( getentityproperty(opp,"hitbyid") != 0 && getlocalvar("bind_time") < openborvariant("elapsed_time") ) {
                    changeentityproperty(opp, "hitbyid", 0); // vedere con vulnerable!! -> resettiamo sempre la possibilità di colpire l'avversario!!
                    setlocalvar("bind_time",NULL());
                }
           	 }

            // Check Time-Reset
            if ( check_stored_time(getentityvar(player,1),9999) ) setentityvar(player,1,openborconstant("MIN_INT"));
            if ( getentityvar(player,1) == NULL() ) {
                sev(player,1,get_next_time(eta));
                slv("bind_time",NULL());
                //cep(player, "invincible", 1);
                //cep(player, "invinctime", openborvariant("elapsed_time")+eta);
            } else {
                if ( openborvariant("elapsed_time") > getentityvar(player,1) ) {
                    float a = getentityproperty(player, "y");
                    float base = getentityproperty(player, "base");
                    float shiftx, zshift;
                    float threshold = 5;
                    float wall, wall2; //wall_below, wall_below_base;

                    if ( a > base+1.0 ) shiftx = 40;
                    else shiftx = 80;

                    if ( gep(partner, "exists") ) {
                        int partner_dir;
                        float leader_x, leader_z, leader_a, leader_base;
                        int pp = gep(partner, "playerindex");

                        if ( get_bindtype(player) == "leader" ) partner_dir = gep(player, "direction");
                        else partner_dir = gep(partner, "direction");
                        if (partner_dir == 1) partner_dir = 0;
                        else partner_dir = 1;
                        if ( get_bindtype(player) == "leader" ) {
                            leader_x = gep(player, "x");
                            leader_z = gep(player, "z");
                            leader_a = gep(player, "y");
                            leader_base = gep(player, "base");
                            //wall_below = checkwallbelow(player);
                            //wall_below_base = checkwallbelow_base(player);
                        } else {
                            leader_x = gep(partner, "x");
                            leader_z = gep(partner, "z");
                            leader_a = gep(partner, "y");
                            leader_base = gep(partner, "base");
                            //wall_below = checkwallbelow(partner);
                            //wall_below_base = checkwallbelow_base(partner);
                        }

                        // Prevent bugs
                        //cep(player,"noaicontrol",1);
                        //cep(partner,"noaicontrol",1);
                        cpp(p,"keys",0);
                        cpp(pp,"keys",0);

                        // -------- CHECK WALL --------
                        wall = checkwall(leader_x+threshold,leader_z);
                        if ( wall > leader_a ) leader_x -= threshold*1;

                        wall = checkwall(leader_x-threshold,leader_z);
                        if ( wall > leader_a ) leader_x += threshold*1;

                        wall = checkwall(leader_x,leader_z-threshold);
                        if ( wall > leader_a ) leader_z += threshold*1;

                        wall = checkwall(leader_x,leader_z+threshold);
                        if ( wall > leader_a ) leader_z -= threshold*1;

                        wall = checkwall(leader_x+shiftx+threshold,leader_z+threshold);
                        wall2 = checkwall(leader_x-shiftx-threshold,leader_z+threshold);
                        if ( (wall > leader_a) || (wall2 > leader_a) ) {
                            leader_z -= threshold*1;
                        }

                        wall = checkwall(leader_x+shiftx+threshold,leader_z-threshold);
                        wall2 = checkwall(leader_x-shiftx-threshold,leader_z-threshold);
                        if ( (wall > leader_a) || (wall2 > leader_a) ) {
                            leader_z += threshold*1;
                        }

                        wall = checkwall(leader_x+shiftx+threshold,leader_z);
                        wall2 = checkwall(leader_x-shiftx-threshold,leader_z);
                        if ( (wall > leader_a) || (wall2 > leader_a) ) {
                            shiftx = 0;
                        }
                        // -------- CHECK WALL --------

                        if ( get_bindtype(player) == "partner" ) {
                            cep(player, "direction", partner_dir);
                            cep(partner, "position", leader_x, leader_z, NULL());
                            if ( partner_dir == 0 ) cep(player, "position", leader_x+shiftx, leader_z, NULL());
                            else if ( partner_dir == 1 ) cep(player, "position", leader_x-shiftx, leader_z, NULL());
                        } else {
                            cep(partner, "direction", partner_dir);
                            cep(player, "position", leader_x, leader_z, NULL());
                            if ( partner_dir == 0 ) cep(partner, "position", leader_x+shiftx, leader_z, NULL());
                            else if ( partner_dir == 1 ) cep(partner, "position", leader_x-shiftx, leader_z, NULL());
                        }

                        cep(player, "aiflag", "jumping", 0);
                        cep(player, "aiflag", "running", 0);
                        cep(partner, "aiflag", "jumping", 0);
                        cep(partner, "aiflag", "running", 0);
                        cep(player, "velocity", 0,0,0);
                        cep(partner, "velocity", 0,0,0);

                        check_safe_pos(player,10,10);
                        check_safe_pos(partner,10,10);
                        //setidle(player,oc("ANI_IDLE"),1); // just in case
                        //setidle(partner,oc("ANI_IDLE"),1);
                        if ( a <= base || (a >= base && a <= base+2 ) ) { // 2 è il valore dell'altezza max di bindentity nell'animazione
                            cep(player, "position", NULL(), NULL(), base);
                            cep(partner, "position", NULL(), NULL(), base);
                            performattack(player,oc("ANI_FOLLOW23"),1);
                            performattack(partner,oc("ANI_FOLLOW23"),1);
                        } else {
                            performattack(player,oc("ANI_WALKOFF"),1);
                            performattack(partner,oc("ANI_WALKOFF"),1);
                        }

                        cep(player, "aiflag", "jumping", 0);
                        cep(player, "aiflag", "running", 0);
                        cep(partner, "aiflag", "jumping", 0);
                        cep(partner, "aiflag", "running", 0);

                        unbind(player);
                        unbind(partner);
                        sev(player,1,NULL());
                        sev(partner,1,NULL());

                        return; // IMPORTANTE!!
                    } // fine partner
                } // fine if tempo scaduto
            } // fine if tempo

            /*if ( gep(partner, "exists") ) {
                if ( gpp(pp, "keys") > 2 && gpp(pp, "keys") != 512 ) { // gpp(pp, "keys") > 2 && gpp(pp, "keys") != 512 && !is_flag_on(gpp(pp, "keys"),128)
                    if ( a <= base && playerkeys(p,1,"jump") == 128 ) {
                        if ( get_bindtype(player) == "leader" ) {
                            //jump(partner);
                        } else jump(player);
                        //return;
                    }
                    //return;
                }
            }*/ // fine if partner exists

            if ( playerkeys(p,0,"moveleft") == 4  ) xdir = -1*speed;
            else if ( playerkeys(p,0,"moveright") == 8  ) xdir = speed;
            else xdir = 0;

            if ( playerkeys(p,0,"moveup") == 16  ) zdir = -1*speed/2;
            else if ( playerkeys(p,0,"movedown") == 32  ) zdir = speed/2;
            else zdir = 0;

            xdir *= 1.5; zdir *= 1.5;

            if ( !playerkeys(pp,0,"moveleft") && !playerkeys(pp,0,"moveright") && !playerkeys(pp,0,"moveup") && !playerkeys(pp,0,"movedown") ) {
                if ( get_bindtype(player) == "leader" ) {
                    cep(partner, "velocity", xdir, zdir, NULL());
                } else cep(player, "velocity", xdir, zdir, NULL());
            }

            if ( get_bindtype(player) == "leader" ) {
                a = af;
                base = basef;
            }
            if ( a <= base && playerkeys(p,1,"jump") == 128 ) {
                if ( get_bindtype(player) == "leader" ) {
                    jump(partner);
                } else jump(player);
            }
        } // fine if  partner
    } // fine if follow22

}

int jump(void player) {
    //changeentityproperty(player, "aiflag", "jumping", 1);
    //changeentityproperty(player, "takeaction", "common_jump", 1);
    playsample(loadsample("data/sounds/jump.wav"));
    tossentity(player, getentityproperty(player, "jumpheight"),NULL(),NULL());
}

int check_bind_attached(void player) {
    int anim_id = gep(player, "animationid");
    int time = ov("elapsed_time");
    int p = gep(player, "playerindex");

    if ( anim_id == oc("ANI_FOLLOW24") ) {
            // Disabilitiamo tutti i tasti tranne quelli importanti per scappare
            //if ( gep(player, "direction") == 1 && playerkeys(p,0,"moveleft") != 4 ) changeplayerproperty(p, "playkeys", 0);
            //else if ( gep(player, "direction") == 0 && playerkeys(p,0,"moveright") != 8 ) changeplayerproperty(p, "playkeys", 0);

            // Check Time-Reset
            if ( check_stored_time(glv("escape_timer"),9999) ) slv("escape_timer",openborconstant("MIN_INT"));
            // ESCAPE
            if ( gep(player, "direction") == 1 && playerkeys(p,0,"moveleft") == 4 ) {
                if ( glv("escape_timer") == NULL() ) setlocalvar("escape_timer",get_next_time(ESCAPE_TIME));
                else if ( glv("escape_timer") < time ) {
                    if ( gep(player, "health") > 0 ) setidle(player,oc("ANI_IDLE"),1);
                    unbind(player);
                    return 0;
                }
            } else if ( gep(player, "direction") == 0 && playerkeys(p,0,"moveright") == 8 ) {
                if ( glv("escape_timer") == NULL() ) slv("escape_timer",get_next_time(ESCAPE_TIME));
                else if ( glv("escape_timer") < time ) {
                    if ( gep(player, "health") > 0 ) setidle(player,oc("ANI_IDLE"),1);
                    unbind(player);
                    return 0;
                }
            } else if ( gep(player, "direction") == 1 && playerkeys(p,0,"moveleft") != 4 ) slv("escape_timer",NULL()); // resettiamo il flag se non premiamo più i tasti
            else if ( gep(player, "direction") == 0 && playerkeys(p,0,"moveright") != 8 ) slv("escape_timer",NULL());

            if ( get_bindpartner(player) != NULL() ) {
                void partner = get_bindpartner(player);

                if ( gep(player, "y") != gep(partner, "y") || gep(player, "base") != gep(partner, "base") ) {
                    unbind(player);
                    return 0;
                }
            }

            if ( (gep(player, "direction") == 1 && playerkeys(p,0,"moveright") == 8) || (gep(player, "direction") == 0 && playerkeys(p,0,"moveleft") == 4) ) { // ESEGUIAMO LA MOSSA BIND!!
                void partner = get_bindpartner(player);
                void dmg_ent;

                playsample(loadsample("data/sounds/cbunga.wav"));
                cep(player, "direction", 1);
                if ( partner != NULL() ) cep(partner, "direction", 1);
                performattack(player, oc("ANI_FOLLOW22"), 1);

                // spawniamo l'entità di danneggiamento
                dmg_ent = spawnsubentity_relative("turtle_bindatk_ent",0,0,0,player);
                bindentity(dmg_ent, player, 5, 0, 0, 1, 0);

                //sgv("tmp_candamage"+p, gep(player,"candamage"));
                //cep(player, "candamage", gep(player,"candamage")-1); // escludi e players <-------------------------------

                if ( partner != NULL() ) {
                        int pp = gep(partner, "playerindex");

                        performattack(partner, oc("ANI_FOLLOW22"), 1);
                        //sgv("tmp_candamage"+pp, gep(partner,"candamage"));
                        //cep(partner, "candamage", gep(partner,"candamage")-1); // escludi e players <-------------------------------
                }

                /*
                ~direction: 0 no change 1 same direction as target -1 opposite direction as target 2 always right -2 always left
                ~bindanimation: 0 No effect. 1 Keep same animation as the target. 2 Also keep same frame as the target. 4 Kill the entity if the animation doesn't match.
                ~Notice: You can combine those values for bindanimation, so it can be 6 which means 2 and 4.
                */

                // Bindiamo SEMPRE il leader
                if ( get_bindtype(player) == "leader" ) bindentity(player, partner, -6, -1, 1, 1, 1);
                else bindentity(partner, player, -6, -1, 1, 1, 1);
                return 1;
            }
    } // fine if follow24

    return 0;
}

int check_unbind(void player) {
    int anim_id = gep(player, "animationid");

    if ( anim_id != oc("ANI_FOLLOW22") && anim_id != oc("ANI_FOLLOW23") && anim_id != oc("ANI_FOLLOW24") ) {
        if ( get_bindtype(player) != NULL() || get_bindpartner(player) != NULL() ) {
            unbind(player);
        }
    }
}

int unbind(void player) {
        void partner = get_bindpartner(player);
        int p = gep(player, "playerindex");

        set_bindtype(player, NULL());
        bindentity(player, NULL());
        cep(player, "nograb", ggv("tmp_nograb"+p));
        //cep(player, "candamage", ggv("tmp_candamage"+p)); // <-------------------------------
        //if ( getentityproperty(player, "noaicontrol") != 0 ) changeentityproperty(player, "noaicontrol", 0);
        if ( partner != NULL() ) {
            int anim_id = gep(player, "animationid");
            int anim_id_partner = gep(partner, "animationid");
            int pp = gep(partner, "playerindex");

            cep(partner, "nograb", ggv("tmp_nograb"+pp));
            //cep(partner, "candamage", ggv("tmp_candamage"+pp)); // <-------------------------------
            //if ( getentityproperty(partner, "noaicontrol") != 0 ) changeentityproperty(partner, "noaicontrol", 0);
            if ( anim_id_partner == oc("ANI_FOLLOW22") || anim_id_partner == oc("ANI_FOLLOW24") ) {
                    //if ( anim_id != oc("ANI_FOLLOW22") && anim_id != oc("ANI_FOLLOW24") && anim_id != oc("ANI_FOLLOW23") && anim_id != oc("ANI_WALKOFF") ) {
                        if ( gep(partner, "health") > 0 ) setidle(partner,oc("ANI_IDLE"),1);
                    //}
            }
            bindentity(partner, NULL());
            sev(partner, 1, NULL()); // time
        }
        set_bindpartner(player, NULL());
        sev(player, 1, NULL()); // time
        slv("escape_timer",NULL());
        slv("bind_time",NULL());
}

int check_bind_coords(void player) {
    int anim_id = gep(player, "animationid");
    int i;

    if ( ov("count_players") > 1 && anim_id == oc("ANI_IDLE") && getglobalvar("board_level") == NULL() && getglobalvar("player_nograb") == NULL() ) {
        for ( i = 0; i < MAX_PLAYERS; ++i ) {
            void friend_ent = gpp(i, "entity");

            if ( friend_ent == player ) continue;
            if ( getentityproperty(friend_ent, "type") != openborconstant("TYPE_PLAYER") ) continue;
            if ( gep(friend_ent, "defaultmodel") != "LEO" && gep(friend_ent, "defaultmodel") != "DON" && gep(friend_ent, "defaultmodel") != "MIKE" && gep(friend_ent, "defaultmodel") != "RAPH" ) continue;
            if ( gep(friend_ent, "exists") ) {
                float x = gep(player, "x");
                float z = gep(player, "z");
                float a = gep(player, "y");
                float base = gep(player, "base");
                float xf = gep(friend_ent, "x");
                float zf = gep(friend_ent, "z");
                float af = gep(friend_ent, "y");
                float basef = gep(friend_ent, "base");

                if ( x >= xf-30 && x <= xf+30 && z >= zf-10 && z <= zf+10 && a == af ) {
                    //if ( gep(player, "playerindex") == 0 ) drawstring( 10,170,0,"Var ():  "+get_anim_time(player));

                    if ( get_anim_time(player) >= BIND_TIME && get_anim_time(friend_ent) >= BIND_TIME && gep(friend_ent, "animationid") == oc("ANI_IDLE") && get_anim_time_anim(friend_ent) == oc("ANI_IDLE")
                        && gep(player, "noaicontrol") == 0 && gep(friend_ent, "noaicontrol") == 0 ) {
                        int p = gep(player, "playerindex");
                        int pp = gep(friend_ent, "playerindex");
                        float xpos = openborvariant("xpos");
                        float hres = openborvariant("hresolution");
                        float threshold = 10, t_space = 60, t_alt = 1;
                        float wall, wall_below;
                        float wall_pl, wall_pf;
                        void platform;
                        float platform_a;
                        // Decidiamo il leader

                        if ( a != af ) return;

                        // CHECK WALL/PLATFORM SPACE
                        // se siamo su un muro/piattaforma ch'è abbastanza spazio??
                        // Cerchiamo 60 pixel a sx e a dx
                        // ------- CHECK WALL -------
                        wall_pl = checkwall(x,z);
                        wall_pf = checkwall(xf,zf);
                        if ( wall_pl != wall_pf ) return; // se stiamo su 2 altezze diverse niente bind!!
                        // Verifichiamo che ci sia il giusto spazio
                        if ( checkwall(x-t_space,z) != checkwall(x+t_space,z) ) return; // Cerca piattaforme/muri anche se stai a terra (righe sotto)
                        if ( checkwall(x-t_space,z) != checkwall(x,z) ) return;
                        if ( checkwall(x+t_space,z) != checkwall(x,z) ) return;
                        // ------- CHECK PLATFORM -------
                        wall_pl = 0; wall_pf = 0;
                        if ( is_on_platform(player) != is_on_platform(friend_ent) ) return;
                        if ( is_on_platform(player) ) wall_pl = get_platform_on_alt(player);
                        if ( is_on_platform(friend_ent) ) wall_pf = get_platform_on_alt(friend_ent);
                        if ( wall_pl != wall_pf ) return;
                        if ( get_platform_a_plus_h_in_xza(x-t_space,z,a+t_alt,base) != get_platform_a_plus_h_in_xza(x+t_space,z,a+t_alt,base) ) return;
                        if ( get_platform_a_plus_h_in_xza(x-t_space,z,a+t_alt,base) != get_platform_a_plus_h_in_xza(x,z,a+t_alt,base) ) return;
                        if ( get_platform_a_plus_h_in_xza(x+t_space,z,a+t_alt,base) != get_platform_a_plus_h_in_xza(x,z,a+t_alt,base) ) return;

                        // CHECK IF YOU ARE "IN" A PLATFORM
                        platform = find_platform_in_a(x,z,a,MAX_ALTITUDE);
                        if ( platform != NULL() ) {
                            platform_a = getentityproperty(platform,"y");

                            if ( platform_a == a ) return;
                        }
                        platform = find_platform_in_a(x+t_space,z,a,MAX_ALTITUDE);
                        if ( platform != NULL() ) {
                            platform_a = getentityproperty(platform,"y");

                            if ( platform_a == a ) return;
                        }
                        platform = find_platform_in_a(x-t_space,z,a,MAX_ALTITUDE);
                        if ( platform != NULL() ) {
                            platform_a = getentityproperty(platform,"y");

                            if ( platform_a == a ) return;
                        }

                        // Per gli estremi x posizioniamolo bene, per i muri e il resto vediamo dopo...
                        check_safe_pos(player,0,0);
                        check_safe_pos(friend_ent,0,0);

                        if ( check_obstacles(player,5,5) || check_obstacles(friend_ent,5,5) ) continue;

                        check_safe_pos(player,5,5);
                        check_safe_pos(friend_ent,5,5);

                        x = gep(player, "x");
                        z = gep(player, "z");
                        a = gep(player, "y");
                        xf = gep(friend_ent, "x");
                        zf = gep(friend_ent, "z");
                        af = gep(friend_ent, "y");

                        // ------ CHECK WALL ------- MEGLIO >a invece di >0 !!!!!!!!!!!
                        // Cerca se i muri ti danno abbastanza spazio
                        wall = checkwall(x-threshold,z);
                        wall_below = checkwallbelow(player);
                        if ( wall > 0 && wall != wall_below ) return;

                        wall = checkwall(x+threshold,z);
                        if ( wall > 0 && wall != wall_below ) return;

                        wall = checkwall(xf-threshold,z);
                        wall_below = checkwallbelow(friend_ent);
                        if ( wall > 0 && wall != wall_below ) return;

                        wall = checkwall(xf+threshold,z);
                        if ( wall > 0 && wall != wall_below ) return;
                        // ------ CHECK WALL -------

                        cep(player, "velocity", 0,0,0);
                        cep(friend_ent, "velocity", 0,0,0);
                        set_bindpartner(player,friend_ent);
                        set_bindpartner(friend_ent,player);

                        sgv("tmp_nograb"+p, gep(player,"nograb"));
                        sgv("tmp_nograb"+pp, gep(player,"nograb"));
                        cep(player, "nograb", 1);
                        cep(friend_ent, "nograb", 1);

                        reset_bind_vars();
                        if ( get_anim_time(player) <= get_anim_time(friend_ent) ) { // il leader è il player (leader = quello che sta davanti)
                            int partner_dir = gep(friend_ent, "direction");
                            int shiftx;

                            set_bindtype(player, "leader");
                            set_bindtype(friend_ent, "partner");

                            performattack(player, oc("ANI_FOLLOW24"), 1);
                            performattack(friend_ent, oc("ANI_FOLLOW24"), 1);

                            // Fare z+0.1???
                            if ( partner_dir == 1 ) {
                                cep(player, "direction", 0);
                                shiftx = 52;
                            } else {
                                cep(player, "direction", 1);
                                shiftx = -52;
                            }

                            // sistemiamo estremi x
                            if ( xf-abs(shiftx) <= xpos || x-abs(shiftx) <= xpos ) {
                                x = xf;
                                if ( shiftx < 0 ) xf += abs(shiftx);
                                cep(player, "position", xf, NULL(), NULL());
                            } else if ( xf+abs(shiftx) >= xpos+hres || x >= xpos+hres ) {
                                x = xf;
                                if ( shiftx > 0 ) xf -= abs(shiftx);
                                cep(player, "position", xf, NULL(), NULL());
                            }

                            cep(player, "position", xf+shiftx ,zf+0.1, af);
                            cep(friend_ent, "position", xf ,zf, af);
                        } else { // il leader è il friend
                            int partner_dir = gep(player, "direction");
                            int shiftx;

                            set_bindtype(friend_ent, "leader");
                            set_bindtype(player, "partner");

                            performattack(friend_ent, oc("ANI_FOLLOW24"), 1);
                            performattack(player, oc("ANI_FOLLOW24"), 1);

                            if ( partner_dir == 1 ) {
                                cep(friend_ent, "direction", 0);
                                shiftx = 52;
                            } else {
                                cep(friend_ent, "direction", 1);
                                shiftx = -52;
                            }

                            // sistemiamo estremi x
                            if ( x-abs(shiftx) <= xpos || xf-abs(shiftx) <= xpos ) {
                                xf = x;
                                if ( shiftx < 0 ) x += abs(shiftx);
                                cep(friend_ent, "position", x, NULL(), NULL());
                            } else if ( x+abs(shiftx) >= xpos+hres || xf >= xpos+hres ) {
                                xf = x;
                                if ( shiftx > 0 ) x -= abs(shiftx);
                                cep(friend_ent, "position", x, NULL(), NULL());
                            }

                            cep(friend_ent, "position", x+shiftx ,z+0.1, a);
                            cep(player, "position", x ,z, a);
                        } // fine if leader

                        return; // Fondamentale (max 2)
                    } // fine if BIND_TIME
                } // fine if coord
            } // fine if friend_ent exists
        } // fine for
    } // fine if idle

}

void reset_bind_vars() {
    slv("bind_time",NULL());
    slv("escape_timer",NULL());
}

void set_bindpartner(void self, void partner) {
    int p = getentityproperty(self, "playerindex");
    setglobalvar("bind_partner"+p, partner);
}

void get_bindpartner(void self) {
    int p = getentityproperty(self, "playerindex");
    return getglobalvar("bind_partner"+p);
}

void set_bindtype(void self, void type) {
    int p = getentityproperty(self, "playerindex");
    setglobalvar("bind_type"+p, type);
}

void get_bindtype(void self) {
    int p = getentityproperty(self, "playerindex");
    return getglobalvar("bind_type"+p);
}

int set_anim_time(void player) {
    int anim_id = getentityproperty(player, "animationid");

    /*if ( anim_id != getlocalvar("animationid_tc") ) { // In questo modo s'imposta il tempo SOLO al primo frame dell'animpos == 0
        setentityvar(player, 15, NULL());
        setlocalvar("animpos_bind_flag", NULL());
    }

    if ( getentityvar(player, 15) == NULL() && getentityproperty(player, "animpos") <= 0 && getlocalvar("animpos_bind_flag") == NULL() ) {
        setlocalvar("animpos_bind_flag", 1);
        setlocalvar("animationid_tc", anim_id);
        setentityvar(player, 15, openborvariant("elapsed_time"));
    }*/

    if ( anim_id != getlocalvar("animationid_tc") ) { // In questo modo s'imposta il tempo SOLO al primo frame dell'animpos == 0
        setentityvar(player, 15, openborvariant("elapsed_time"));
        setlocalvar("animationid_tc", anim_id); // memorizzare questo var anche in una entity var per mettere la condizione == animationid_tc del partner
        setentityvar(player,"animationid_bind",anim_id);
    }
}

int get_anim_time_anim(void player) {
    return getentityvar(player,"animationid_bind");
}

int get_anim_time(void player) {
    // Check Time-Reset
    if ( check_stored_time(getentityvar(player, 15),9999) ) setentityvar(player,15,NULL());
    if ( getentityvar(player, 15) != NULL() ) return openborvariant("elapsed_time")-getentityvar(player, 15);
    else return 0;
}
