
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/player_didhit.c"
#import "data/scripts/player_moves.c"

/*
 * Nel caso i foot non dovessero colpire mentre sei preso togli l'aiflag inpain ponendolo a 0
 */

int check_antigrab(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id != openborconstant("ANI_SPAWN") && anim_id != openborconstant("ANI_RESPAWN") ) {
        int p = getentityproperty(player, "playerindex");

        check_antigrab_for_board(player);
        if ( anim_id == openborconstant("ANI_FOLLOW44") || anim_id == openborconstant("ANI_FOLLOW41") || anim_id == openborconstant("ANI_GRABBED") || getentityproperty(player, "invincible") != 0 ) {
            if ( getentityproperty(player, "nograb") != 1 ) changeentityproperty(player, "nograb", 1);
        } else if ( anim_id != openborconstant("ANI_FOLLOW22") && anim_id != openborconstant("ANI_FOLLOW23") && anim_id != openborconstant("ANI_FOLLOW24") ) {
            if ( getglobalvar("player_nograb") == NULL() || getglobalvar("player_nograb") != 1 ) {
                    if ( getentityproperty(player, "nograb") != getglobalvar("tmp_nograb"+p) ) changeentityproperty(player, "nograb", getglobalvar("tmp_nograb"+p)); //changeentityproperty(player, "nograb", 0);
            } else {
                if ( getentityproperty(player, "nograb") != 1 ) changeentityproperty(player, "nograb", 1);
            }
        }
    }
}

int check_grabbed_pain(void player) {
        void grabber = get_grabber(player);

		// # (GRABBED PAIN) Vediamo se è grabbed...
        if ( getentityproperty(grabber, "animationid") == openborconstant("ANI_GRAB") ) {
            int p = getentityproperty(player, "playerindex");
            //void anim_id = getentityproperty(player, "animationid");

            if ( getentityproperty(player, "health") > 0 && getentityproperty(player, "animpos") <= 0 && grabber != NULL() && getentityproperty(grabber,"exists") ) {
		          if ( getentityproperty(player, "aiflag", "inpain") ) {
                        set_fake_grab(player);
                        performattack(grabber,openborconstant("ANI_FOLLOW41"),1);
                        performattack(player, openborconstant("ANI_FOLLOW41"),1);
                        changeentityproperty(player,"aiflag","inpain",1);
		          }
            }
		} // fine if 1
}

int set_fake_grab(void player) {
        void anim_id = getentityproperty(player, "animationid");
        int p = getentityproperty(player, "playerindex");
        void grabber = get_grabber(player);

		// # (GRABBED PAIN) Vediamo se è grabbed...
		if ( getentityproperty(player, "health") > 0 && grabber != NULL() ) {
                        float gx = getentityproperty(grabber, "x");
                        float gz = getentityproperty(grabber, "z");
                        float ga = getentityproperty(grabber, "y");
                        float gbase = getentityproperty(grabber, "base");

                        if ( getentityproperty(grabber, "direction") == 1 ) changeentityproperty(player, "direction", 0);
                        else changeentityproperty(player, "direction", 1);

                        changeentityproperty(grabber,"noaicontrol",1);
                        changeentityproperty(player,"noaicontrol",1);
                        changeentityproperty(grabber,"velocity",0,0,0);
                        changeentityproperty(player,"velocity",0,0,0);


                        if ( checkwall(gx,gz+1) == gbase && checkhole(gx,gz+1) == 0 && checkplatform(gx,gz+1,MAX_ALTITUDE) == NULL() ) { // Evitiamo di rimanere incastrati nel muro!!
                            // Per ora nulla
                        } else {
                            --gz;
                            changeentityproperty(grabber,"position",gx,gz,ga);
                        }
                        bindentity(player, grabber, 20, 1, 0, 0, 0); // Non preoccupiamoci di collisioni (walls/platforms) che sembrano autogestite

                        changeentityproperty(grabber, "nograb",1);
                        changeentityproperty(player, "nograb",1);

                        setentityvar(player,2,NULL());
                        //changeentityproperty(player,"position",x-20,z+1,a);
		} // fine if 1
}

int check_grab_on_players(void player) { // tra tartarughe non ci si prende
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_GRABBED") ) {
        void opp = getentityproperty(player, "opponent");

        if ( getentityproperty(opp,"exists") ) {
            void o_defaultmodel = getentityproperty(opp, "defaultmodel");

            if ( o_defaultmodel == "RAPH" || o_defaultmodel == "MIKE" || o_defaultmodel == "LEO" || o_defaultmodel == "DON" ) {
                setidle(player,openborconstant("ANI_IDLE"),1);
            }
        } // fine if exists
    }
}

int get_grabber(void player) {
    return getentityvar(player, 3);
}

int set_grabber(void player, void grabber) {
    setentityvar(player, 3, grabber);
    changeentityproperty(player,"opponent",grabber);
    changeentityproperty(grabber,"opponent",player);
}

int check_nomore_grabbed(void player) { // Se non è più GRABBED e se il flag GRABBER è ancora attivo...
    void grabber = get_grabber(player);

    if ( grabber != NULL() && getentityproperty(grabber,"exists") ) {
            int grabber_anim_id = getentityproperty(grabber, "animationid");
            int anim_id = getentityproperty(player, "animationid");
            int p = getentityproperty(player, "playerindex");

            if ( !getentityproperty(grabber, "exists") || (getentityproperty(grabber, "exists")
                    && grabber_anim_id != openborconstant("ANI_GRAB") && grabber_anim_id != openborconstant("ANI_FOLLOW41")) ) { // || ( anim_id != openborconstant("ANI_PAIN") && anim_id != openborconstant("ANI_FOLLOW41")
                    changeplayerproperty(p, "keys", 0);
                    changeplayerproperty(p, "playkeys", 0);

                    changeentityproperty(player,"noaicontrol",0);
                    changeentityproperty(player, "nograb", getglobalvar("tmp_nograb"+p)); //changeentityproperty(player, "nograb", 0);
                    changeentityproperty(player, "aiflag", "jumping", 0);
                    changeentityproperty(player, "aiflag", "running", 0);

                    bindentity(player, NULL());
                    setentityvar(player,1,NULL()); // Annulliamo il tempo
                    setentityvar(player,2,NULL()); // Annulliamo la combo
                    setlocalvar("fast_grabbed_timer", NULL());
                    setlocalvar("max_combo_buttons", NULL());
                    if ( getglobalvar("attack_button_escape"+p) != NULL() && getentityproperty(getglobalvar("attack_button_escape"+p), "exists") ) { // Eliminiamo l'animazione pulsante
                        killentity(getglobalvar("attack_button_escape"+p));
                    }
                    setglobalvar("attack_button_escape"+p, NULL());

                    if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1 ) {
                        changeentityproperty(player,"velocity",0,0,0);
                        if ( anim_id != openborconstant("ANI_SPECIAL") ) {
                                setidle(player,openborconstant("ANI_IDLE"),1);
                        }
                    }

                    set_grabber(player,NULL());
            }
    }
}

// controlla se il player è stato preso
int check_grabbed(void player) {
    void opp = getentityproperty(player, "opponent");

    if ( getentityproperty(opp, "exists") ) {
        void grabbed = getentityproperty(opp, "grabbing");

        if ( getentityproperty(grabbed, "exists") && grabbed == player ) return 1;
        else return 0;
    } else return 0;
}

int check_grabbed_anim(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_GRABBED") || anim_id == openborconstant("ANI_FOLLOW44") ) {
        void opp = getentityproperty(player, "opponent");
        int p = getentityproperty(player, "playerindex");
        void grabber = get_grabber(player);
        int game_speed = openborvariant("game_speed");
        int energycost = 3, eta = 300, eta_fastanim = 30, mul_escape_factor = 5; // 6

        if ( getentityproperty(opp,"exists") ) {
            if ( getentityproperty(opp, "grabbing") == player && grabber == NULL() ) set_grabber(player,opp);
        }

        // se il player è grabbed allora imposta il nuovo idle. In questo modo i soldati attaccheranno durante la holdgrab
        //drawstring( 10,190,0,"Var (): "+check_grabbed(player));
        if ( check_grabbed(player) ) {
            if ( getentityproperty(opp,"exists") ) {
                char cText = getentityproperty(opp, "defaultmodel");
                char cFind = "foot";
                char substr = strinfirst(cText, cFind);

                if ( substr != -1 ) {
                    damageentity(opp,opp,0,0,openborconstant("ATK_NORMAL6"));
                    //damageentity(player,player,0,0,openborconstant("ATK_NORMAL7"));
                    //if ( getentityproperty(opp, "animationid") != openborconstant("ANI_GRAB") )
                    set_fake_grab(player); // <---------------- SIMULIAMO LA PRESA PER FAR VENIRE ALTRI NEMICI AD ATTACCARE!!
                    if ( getentityproperty(opp,"health") > 0 ) setidle(opp, openborconstant("ANI_GRAB"), 1);
                    if ( getentityproperty(player,"health") > 0 ) setidle(player, openborconstant("ANI_GRABBED"), 1);
                }
            }
        } // fine if check_grabbed

        // Se sei grabbato puoi usare una special A+J
        if ( (playerkeys(p,1,"attack") == 64 && playerkeys(p,1,"jump") == 128) || (playerkeys(p,0,"attack") == 64 && playerkeys(p,1,"jump") == 128) || (playerkeys(p,1,"attack") == 64 && playerkeys(p,0,"jump") == 128)
            && grabber != NULL() && getentityproperty(grabber,"exists") ) {
            // Può fare la special solo se ha abbastanza HP da soddisfare l'energycost
            if ( getentityproperty(player,"health") > energycost ) {
                    if ( getentityproperty(grabber,"health") > 0 ) setidle(grabber,openborconstant("ANI_IDLE"),1); // grabber
                    if ( getentityproperty(player,"health") > 0 ) {
                        performattack(player,openborconstant("ANI_SPECIAL"),1); // il resto sarà autogestito!
                        setentityvar(player,1,NULL()); // Annulliamo il tempo
                        setentityvar(player,2,NULL()); // Annulliamo la combo
                    }
            }
        } else if ( grabber != NULL() && getentityproperty(grabber,"exists") ) {
            /*
             * ##### Aggiungiamo la GRAB ESCAPE #####
             */
            // Check Time-Reset
            if ( check_stored_time(getentityvar(player,1),9999) ) setentityvar(player, 1, openborconstant("MIN_INT"));
            if ( getentityvar(player,1) == NULL() ) { // Se non è stato inizializzato il tempo e abbiamo impostato il grabber allora impostiamo il tempo!
                //---------PRESS BUTTON-----------
                if ( getglobalvar("attack_button_escape"+p) == NULL() && getglobalvar("atk_btn_escape_active"+p) == NULL() ) {
                   void ent;

                   clearspawnentry();
                   setspawnentry("name", "press_any_button");
                   ent = spawn();
                   changeentityproperty(ent, "parent", player);
                   setglobalvar("attack_button_escape"+p, ent);
                }
                //--------------------------------
                setentityvar(player,1,get_next_time(eta));

                // Memorizziamo la max_combo in una localvar perchè si deve inizializzare a seconda dei parametri dei giocatori in gara
                if ( getentityproperty(grabber,"grabforce") > getentityproperty(player,"antigrab") ) setlocalvar("max_combo_buttons", getentityproperty(grabber,"grabforce")-getentityproperty(player,"antigrab"));
                else setlocalvar("max_combo_buttons", getentityproperty(player,"antigrab")-getentityproperty(grabber,"grabforce"));
                setlocalvar("max_combo_buttons", getlocalvar("max_combo_buttons")*mul_escape_factor);
            } else if ( getentityvar(player,1) > openborvariant("elapsed_time") ) {
                float a = getentityproperty(player,"y");
                float base = getentityproperty(player,"base");
                float oa = getentityproperty(grabber,"y");

                if ( playerkeys(p,1,"attack") == 64 || playerkeys(p,1,"moveleft") == 4 || playerkeys(p,1,"moveright") == 8 || playerkeys(p,1,"moveup") == 16
                  || playerkeys(p,1,"movedown") == 32 || playerkeys(p,1,"jump") == 128 || playerkeys(p,1,"special") == 256 ) {
                    if ( getentityvar(player,2) == NULL() ) setentityvar(player,2,0);
                    else setentityvar(player,2,getentityvar(player,2)+1);

                    if ( anim_id != openborconstant("ANI_FOLLOW44") && anim_id != openborconstant("ANI_SPECIAL") && anim_id != openborconstant("ANI_FOLLOW41") ) {
                            int grabbed_frame = getentityvar(player,25);
                            int last_grabbed_frame = 4;

                            changeentityproperty(player,"animation",openborconstant("ANI_FOLLOW44"));
                            //performattack(player,openborconstant("ANI_FOLLOW44"),0);
                            if ( grabbed_frame != NULL() ) {
                                grabbed_frame = (++grabbed_frame)%last_grabbed_frame; // set grabbed_frame+1 <= last_grabbed_frame
                                changeentityproperty(player,"animpos",grabbed_frame); // set right frame!!
                            }
                    }
                    setlocalvar("fast_grabbed_timer", get_next_time(eta_fastanim));
                } else {
                    // Check Time-Reset
                    if ( check_stored_time(getlocalvar("fast_grabbed_timer"),9999) ) setlocalvar("fast_grabbed_timer", openborconstant("MIN_INT"));
                    if ( getlocalvar("fast_grabbed_timer") < openborvariant("elapsed_time") ) {
                        if ( anim_id != openborconstant("ANI_GRABBED") && anim_id != openborconstant("ANI_FOLLOW41") ) {
                            int grabbed_frame = getentityvar(player,25);
                            int last_grabbed_frame = 4;

                            changeentityproperty(player,"animation",openborconstant("ANI_GRABBED"));
                            if ( grabbed_frame != NULL() ) {
                                grabbed_frame = (++grabbed_frame)%last_grabbed_frame; // set grabbed_frame+1 <= last_grabbed_frame
                                changeentityproperty(player,"animpos",grabbed_frame); // set right frame!!
                            }
                        }
                    }
                }
                if ( getentityvar(player,2) > getlocalvar("max_combo_buttons") || (a != oa) || (a != base) ) { // LIBERIAMOCI grazie alla combo!!
                    if ( getglobalvar("attack_button_escape"+p) != NULL() && getentityproperty(getglobalvar("attack_button_escape"+p), "exists") ) { // Eliminiamo l'animazione pulsante
                        killentity(getglobalvar("attack_button_escape"+p));
                    }
                    setglobalvar("attack_button_escape"+p, NULL());
                    setglobalvar("atk_btn_escape_active"+p, 1);
                    setentityvar(player,1,NULL()); // Annulliamo il tempo
                    setentityvar(player,2,NULL()); // Annulliamo la combo

                    // IMPORTANTE: E' il nemico che bisogna idlingare!! Perchè il motore lascia la grab dipendentemente da lui!! (vedere con entityprop "grabbing")
                    damageentity(grabber,player,0,0,openborconstant("ATK_NORMAL6"));
                    changeentityproperty(player, "invincible", 1);
                    changeentityproperty(player, "invinctime", get_next_time(game_speed*0.5));
                    //setidle(player,openborconstant("ANI_IDLE"),1);
                }
            } else if ( openborvariant("elapsed_time") > getentityvar(player,1)  ) { // se il tempo è passato resettiamo per la prossima combo!
                setentityvar(player,1,NULL()); // Annulliamo il tempo
                setentityvar(player,2,NULL()); // Annulliamo la combo
                setlocalvar("fast_grabbed_timer", NULL());
                setlocalvar("max_combo_buttons", NULL());
                if ( anim_id != openborconstant("ANI_GRABBED") && anim_id != openborconstant("ANI_FOLLOW41") ) changeentityproperty(player,"animation",openborconstant("ANI_GRABBED"));
            } // fine if entityvar 1
             // ######### FINE GRAB ESCAPE ##########
        } // fine if entityvar 3

    } // fine if ANI_GRABBED

}

int check_splatgrab_release(void player) {
    int anim_id = getentityproperty(player, "animationid");
    //int p = getentityproperty(player, "playerindex");

    if ( anim_id == openborconstant("ANI_FOLLOW40") ) {
        int time = openborvariant("elapsed_time");
        int max_stall_time = 300;
        void opp = getentityproperty(player, "opponent");
        int p = getentityproperty(player, "playerindex");

        if (opp && getentityproperty(opp,"exists") ) {
            int health = getentityproperty(opp,"health");

            if ( health <= 0 ) return;

            // facciamo comparire il press button
            if ( getglobalvar("attack_button_active"+p) == NULL() ) {
                 void ent;

                 clearspawnentry();
                 setspawnentry("name", "press_attack_button");
                 ent = spawn();
                 changeentityproperty(ent, "parent", player);
                 setglobalvar("attack_button"+p, ent);
                 setglobalvar("attack_button_active"+p, 1);
            }

            // Check Time-Reset
            if ( check_stored_time(getentityvar(player,1),9999) ) setentityvar(player,1,openborconstant("MIN_INT"));
            // Il grabbed si toglie dalla presa!!
            if ( getentityvar(player,1) == NULL() && getentityproperty(opp,"health") > 0 ) setentityvar(player,1,get_next_time(max_stall_time));
            else if ( time > getentityvar(player,1) && getentityproperty(opp,"health") > 0 ) { // fai rialzare l'avversario
                //damageentity(opp,player,getentityvar(opp,1),0,openborconstant("ATK_NORMAL")); // colpisci 1/3 di HP (memorizzata nella entityvar di GRABATTACK)

               	changeentityproperty(opp, "antigravity", 0);
               	changeentityproperty(opp, "noaicontrol", 0);
			    bindentity(opp, NULL());
              	setentityvar(opp,1,NULL());
             	setentityvar(player,1,NULL());
             	setentityvar(player,22,NULL());
                     if ( check_obstacles(opp,0,0,0) ) {
                        float x = getentityproperty(player,"x");
                        float z = getentityproperty(player,"z");
                        float ox = getentityproperty(opp,"x");
                        float oz = getentityproperty(opp,"z");

                        changeentityproperty(opp,"position",x,z,NULL());
                        /*set_safe_path(opp,ox,oz,1,1,"x_safe_pos","z_safe_pos");
                        changeentityproperty(opp,"position",getlocalvar("x_safe_pos"),getlocalvar("z_safe_pos"),NULL());
                        setlocalvar("x_safe_pos",NULL());
                        setlocalvar("z_safe_pos",NULL());*/
                        // oppure subject_to_wall 0
                     }

                //changeentityproperty(opp,"takeaction","common_rise",1);
                if ( getentityproperty(player,"direction") == 1 ) changeentityproperty(player,"direction",0);
			    else  changeentityproperty(player,"direction",1);

                if ( getentityproperty(opp,"direction") == 1 ) changeentityproperty(opp,"direction",0);
			    else  changeentityproperty(opp,"direction",1);

                if ( getglobalvar("attack_button"+p) != NULL() && getentityproperty(getglobalvar("attack_button"+p),"exists") ) killentity(getglobalvar("attack_button"+p));
                setglobalvar("attack_button_active"+p, NULL());

                damageentity(opp,opp,0,0,openborconstant("ATK_NORMAL9"));
                changeentityproperty(opp, "aiflag", "falling", 1); // Forziamo il falling
                changeentityproperty(opp, "animation", openborconstant("ANI_FOLLOW29"));
                //performattack(opp, openborconstant("ANI_FALL9"), 1);

                //changeentityproperty(player,"aiflag","idling",1);
                damageentity(player,player,0,0,openborconstant("ATK_NORMAL7"));
                setidle(player,openborconstant("ANI_IDLE"),1);

            }

        } // fine opp

    } // fine ani_follow40
}

int check_splatgrab(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_GRABATTACK") || anim_id == openborconstant("ANI_FREESPECIAL7") || anim_id == openborconstant("ANI_GRABFORWARD") || anim_id == openborconstant("ANI_GRABUP")
         || anim_id == openborconstant("ANI_GRABDOWN") || anim_id == openborconstant("ANI_THROW") || anim_id == openborconstant("ANI_FOLLOW61") || anim_id == openborconstant("ANI_FOLLOW40") ) {
        void opp = getentityproperty(player, "opponent");

        if ( getentityproperty(opp, "exists") ) {
            char cText = getentityproperty(opp, "defaultmodel");
            char cFind = "rock_soldier";
            char cFind2 = "ROCK_SOLDIER";
            char substr = strinfirst(cText, cFind);
            char substr2 = strinfirst(cText, cFind2);

            if ( substr != -1 || substr2 != -1 ) {
                perform_wrong_splatgrab(player);
            } else perform_splatgrab(player);
        } // fine if exists
    } // fine if animid
}

int perform_wrong_splatgrab(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_GRABATTACK") || anim_id == openborconstant("ANI_GRABUP") || anim_id == openborconstant("ANI_GRABDOWN") || anim_id == openborconstant("ANI_FOLLOW61") ) { // || anim_id == openborconstant("ANI_GRABFORWARD") || anim_id == openborconstant("ANI_THROW")
        void opp = getentityproperty(player, "opponent");

        if ( getentityproperty(player, "animpos") <= 0 && getlocalvar(player+"sp") != NULL() ) {
            setlocalvar(player+"sp", NULL());
        }

		if ( getentityproperty(player, "animpos") <= 0 && getlocalvar(player+"sp") == NULL() ) {
			changeentityproperty(opp, "noaicontrol", 1);

            if ( anim_id == openborconstant("ANI_GRABATTACK") ) {
                performattack(opp, openborconstant("ANI_PAIN37"), 1);
            } else if ( anim_id == openborconstant("ANI_GRABUP") || anim_id == openborconstant("ANI_GRABDOWN") || anim_id == openborconstant("ANI_FOLLOW61") ) { // || anim_id == openborconstant("ANI_THROW") || anim_id == openborconstant("ANI_GRABFORWARD")
                performattack(opp, openborconstant("ANI_PAIN39"), 1);
            }
			setlocalvar(player+"sp", 1);
			check_safe_pos(player,5,1,0);
		} else if ( getentityproperty(player, "animpos") >= 5 && getlocalvar(player+"sp") == 1 ) {
                bindentity(opp, NULL());
                changeentityproperty(opp, "noaicontrol", 0);
                changeentityproperty(opp, "antigravity", 0);
                if ( check_obstacles(opp,0,0,0) ) {
                    float x = getentityproperty(player,"x");
                    float z = getentityproperty(player,"z");
                    float ox = getentityproperty(opp,"x");
                    float oz = getentityproperty(opp,"z");

                    changeentityproperty(opp,"position",x,z,NULL());
                    /*set_safe_path(opp,ox,oz,1,1,"x_safe_pos","z_safe_pos");
                    changeentityproperty(opp,"position",getlocalvar("x_safe_pos"),getlocalvar("z_safe_pos"),NULL());
                    setlocalvar("x_safe_pos",NULL());
                    setlocalvar("z_safe_pos",NULL());*/
                    // oppure subject_to_wall 0
                }

                if ( anim_id == openborconstant("ANI_GRABATTACK") || anim_id == openborconstant("ANI_GRABUP") ) { // || anim_id == openborconstant("ANI_GRABFORWARD")
                    if ( getentityproperty(player, "direction") == 1 ) changeentityproperty(player, "direction", 0);
                    else changeentityproperty(player, "direction", 1);
                }

                damageentity(player, player, 0,0, openborconstant("ATK_NORMAL7"));
                setidle(player,openborconstant("ANI_IDLE"),1);
                //damageentity(opp, opp, 0,0, openborconstant("ATK_NORMAL32"));
                performattack(opp, openborconstant("ANI_PAIN38"), 1);

                setlocalvar(player+"sp", NULL());
                return;
		}
    }
}

int perform_splatgrab(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_GRABATTACK") || anim_id == openborconstant("ANI_FREESPECIAL7") ) {
		void opp = getentityproperty(player, "opponent");

        if ( getentityproperty(player, "animpos") <= 0 && anim_id == openborconstant("ANI_GRABATTACK") && getlocalvar(player+"sp") != NULL() ) {
            setlocalvar(player+"sp", NULL());
        }

		setentityvar(player,1,NULL());
		if ( getentityproperty(player, "animpos") <= 0 && getlocalvar(player+"sp") == NULL() ) {
            if ( anim_id == openborconstant("ANI_FREESPECIAL7") ) {
                if ( getentityproperty(player, "direction") == 1 ) changeentityproperty(player, "direction", 0);
                else changeentityproperty(player, "direction", 1);
            }
			changeentityproperty(opp, "noaicontrol", 1);
			//changeentityproperty(opp, "antigravity", 1);
			//changeentityproperty(opp, "animation", openborconstant("ANI_PAIN32"));
			/*
			 * Impostiamo questo altrimenti l'animazione si blocca. Ciò accade perche l'animazione del player ha 5 frame e quella
			 * dell'enemy ne ha 4. Per cui poi al 5° frame del player il nemico va in idle...
			 */
			//setidle(opp, openborconstant("ANI_PAIN32"),1); //(entity, int anim, int resetable, int stalltime)
            performattack(opp, openborconstant("ANI_PAIN32"), 1);
			//changeentityproperty(opp, "animation", openborconstant("ANI_PAIN32"));
			setlocalvar(player+"sp", 1);
			check_safe_pos(player,5,1,0);
		} else if ( getentityproperty(player, "animpos") >= 7 && getlocalvar(player+"sp") == 1 ) {
			    setentityvar(player,1,NULL()); // Azzeriamo il timer della release dell'opponent
                if ( anim_id == openborconstant("ANI_GRABATTACK") ) {
                    int ohealth = getentityproperty(opp, "health");

                    if ( ohealth >= 3 ) setentityvar(opp, 1, ohealth/3);
                    else setentityvar(opp, 1, ohealth);
                }

                // vediamo se sta per morire o no...
            	if ( getentityproperty(opp, "health")-getentityvar(opp,1) > 0 ) changeentityproperty(opp, "health", getentityproperty(opp, "health")-getentityvar(opp,1));
           		else {
                     int p = getentityproperty(player, "playerindex");

                     changeentityproperty(opp, "antigravity", 0);
                     changeentityproperty(opp, "noaicontrol", 0);
                     bindentity(opp, NULL());
                     if ( check_obstacles(opp,0,0,0) ) {
                        float x = getentityproperty(player,"x");
                        float z = getentityproperty(player,"z");
                        float ox = getentityproperty(opp,"x");
                        float oz = getentityproperty(opp,"z");

                        changeentityproperty(opp,"position",x,z,NULL());
                        /*set_safe_path(opp,ox,oz,1,1,"x_safe_pos","z_safe_pos");
                        changeentityproperty(opp,"position",getlocalvar("x_safe_pos"),getlocalvar("z_safe_pos"),NULL());
                        setlocalvar("x_safe_pos",NULL());
                        setlocalvar("z_safe_pos",NULL());*/
                        // oppure subject_to_wall 0
                     }

                     if ( getglobalvar("attack_button"+p) != NULL() && getentityproperty(getglobalvar("attack_button"+p),"exists") ) killentity(getglobalvar("attack_button"+p)); // per il press button
                     setglobalvar("attack_button"+p,NULL());
                     setentityvar(opp,1,NULL());
                     setentityvar(player,1,NULL());
                     setentityvar(player,22,NULL());
                     damageentity(opp,player,getentityproperty(opp, "health"),0,openborconstant("ATK_NORMAL9")); // colpisci 1/3 di HP (memorizzata nella entityvar di GRABATTACK)

                     check_versus_mark(player,opp);

                     if ( getentityproperty(opp,"direction") == 1 ) changeentityproperty(opp,"direction",0);
                     else  changeentityproperty(opp,"direction",1);
                     setidle(player, openborconstant("ANI_IDLE"), 1);
                     if ( getentityproperty(player,"direction") == 1 ) changeentityproperty(player,"direction",0);
                     else  changeentityproperty(player,"direction",1);

                     setlocalvar(player+"sp", NULL());
                     return;
          		}

                if ( getentityproperty(opp,"health") > 0 ) {
                    changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW40"));
                    if ( getentityproperty(opp,"animationid") != openborconstant("ANI_FOLLOW40") ) changeentityproperty(opp, "animation", openborconstant("ANI_FOLLOW40"));
                    //performattack(opp, openborconstant("ANI_FOLLOW40"), 1);
                    setentityvar(player,22,opp);
                }
                setlocalvar(player+"sp", 2);
		}
    } else if ( anim_id == openborconstant("ANI_FOLLOW40") ) {
        void opp = getentityproperty(player, "opponent");

        if ( getentityproperty(opp,"animationid") != openborconstant("ANI_FOLLOW40") ) changeentityproperty(opp, "animation", openborconstant("ANI_FOLLOW40"));
        //performattack(opp, openborconstant("ANI_FOLLOW40"), 1);
        setlocalvar(player+"sp", NULL());
    } // fine if anim_id

    if ( getlocalvar(player+"sp") != NULL()
        && (anim_id != openborconstant("ANI_FREESPECIAL7") && anim_id != openborconstant("ANI_GRABATTACK") && anim_id != openborconstant("ANI_FOLLOW40")) ) {
            //setentityvar(player,1,NULL());
            setlocalvar(player+"sp", NULL());
    }

}
