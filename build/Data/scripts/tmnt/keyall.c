
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

//#import "data/scripts/player_moves.c"

// DA FARE:

void main() {
    int p = getlocalvar("player"); //Get calling player index

    ///hold_keyc(p); // hold key pre-processor

    //check_duck(NULL(),p,"ANI_FOLLOW90"); // test ducking
    check_credit_input(p);

    // combo test
    /*if ( getentityproperty(getplayerproperty(p,"entity"),"exists") ) {
        void player = getplayerproperty(p,"entity");

        if( combo(p,"combo01",0,1,"forward","up","attack+attack2") ) {
            char anim = "ANI_FOLLOW42";

            if ( ready_to_attack(player,1,0,NULL()) ) {
                changeentityproperty(player,"velocity",0,0,0);
                performattack(player,openborconstant(anim),1);
            }
        }
    }*/
}

/// CREDIT SYSTEM: custom_hud.c in draw_credits_ps() and show_inserted_credits(), custom_hud_player_select.c in play_game(), title_script.c in set_start_credits() and oncreate(), custom_hud_versus.c in oncreate();
void check_credit_input(int p) {
    if ( openborvariant("in_level") ) { // openborvariant("in_cheat_options")
        if ( playerkeys(p,1,"ATTACK4") ) {
            int se = loadsample("data/sounds/cowabunga.wav");
            int credits = getplayerproperty(p,"credits");

            if ( ggv("1VS1") != NULL() && ggv("menu_mode") == NULL() ) return;

            if ( COIN_SYSTEM == 0 ) {
                if ( ggv("in_menu") == 1 && ggv("playing_gif") != 0 && ggv("playing_gif") != NULL() ) return;
                if ( getlevelproperty("type") == 2 ) return;
                /*if ( getlevelproperty("type") == 2 && ggv("in_menu") == 1 &&
                    (ggv("menu_mode") != "player" && ggv("menu_mode") != "mode" && ggv("menu_mode") != "stage") ) return;*/
            }

            if ( credits < MAX_CREDITS ) {
                playsample(se);

                if ( p == 0 ) {
                    if ( getglobalvar("fake_credit") == NULL() || getglobalvar("fake_credit") <= 0 ) changeplayerproperty(p,"credits",++credits);
                    else setglobalvar("fake_credit", NULL());
                } else {
                    changeplayerproperty(p,"credits",++credits);
                }
            }
            setglobalvar("add_credits_"+p,1);
            setglobalvar("current_credits_"+p,credits);
            setglobalvar("add_credits_master",1);
        }
        if ( playerkeys(p,1,"START") && getplayerproperty(p,"credits") <= 0 && getplayerproperty(p,"name") == "" ) {
            changeplayerproperty(p,"keys",0);
            changeplayerproperty(p,"newkeys",0);
            changeplayerproperty(p,"playkeys",0);
        }

        if ( COIN_SYSTEM && p == 0 && getplayerproperty(0,"credits") == 1 && getglobalvar("fake_credit") > 0 ) {
            changeplayerproperty(p,"keys",0);
            changeplayerproperty(p,"newkeys",0);
            changeplayerproperty(p,"playkeys",0);
        }
    } // fine if in_level

    return;
}

/// ----------- MAIN func -----------
/*
 * PARAMS:
 * int p: Player Index
 * char combo_id: Combo ID (string)
 * int ETA: time between two buttons pression (buffer window): 0 or NULL() is default. default is 0.5
 * int hold_btn_combo_flag: hold button flag (the same of playerkeys())
 * char btn1, char btn2, char btn3, ...: buttons
 */
int combo(int p, char combo_id, int ETA, int hold_btn_combo_flag, char btn1, char btn2, char btn3, char btn4, char btn5, char btn6, char btn7, char btn8, char btn9, char btn10) {
    void player = getplayerproperty(p,"entity");

    if ( !ent_exists(player) ) { return 0; }
    else {
        int time = openborvariant("elapsed_time");
        int game_speed = openborvariant("game_speed");
        int keys = getplayerproperty(p,"keys");
        int combo_step = getlocalvar("__"+combo_id+"_combostep");
        int combo_eta = getlocalvar("__"+combo_id+"_comboETA");
        int combo_num = 0;
        char buttons = array(0); // save in global var and free into updated??
        int playerkeys_response = 0, first_playerkeys_response = 0;
        int i = 0;

        // load array
        if ( btn1  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn1));
        if ( btn2  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn2));
        if ( btn3  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn3));
        if ( btn4  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn4));
        if ( btn5  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn5));
        if ( btn6  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn6));
        if ( btn7  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn7));
        if ( btn8  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn8));
        if ( btn9  != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn9));
        if ( btn10 != NULL() ) set(buttons,combo_num++,get_pkeyc(p,btn10));

        // --- INIT ---
        if ( ETA == NULL() || ETA <= 0 ) ETA = truncA(game_speed*0.5);

        if ( combo_step == NULL() ) {
            setlocalvar("__"+combo_id+"_combostep",0);
            combo_step = getlocalvar("__"+combo_id+"_combostep");
        }
        // --- INIT ---

        first_playerkeys_response = playerkeys_multi(p,0,get(buttons,0));

        //------ RESET COMBO ------
        // time is over
        if ( combo_eta != NULL() ) {
            if ( time > combo_eta ) {
                if ( first_playerkeys_response == 1 ) {
                    setlocalvar("__"+combo_id+"_comboETA",time+ETA);
                    setlocalvar("__"+combo_id+"_combostep",1);
                    setlocalvar("__"+combo_id+"_comboRB",NULL());
                    combo_step = getlocalvar("__"+combo_id+"_combostep");
                    free(buttons);
                    return 0;
                } else {
                    setlocalvar("__"+combo_id+"_combostep",NULL());
                    setlocalvar("__"+combo_id+"_comboETA",NULL());
                    setlocalvar("__"+combo_id+"_comboRB",NULL());
                    free(buttons);
                    return 0;
                }
            }
        }

        // not a good buttons
        // prendi key flag, escludi esc/start/screenshot e btn1 (retrieve btn_flag del giusto combo_step)
        keys = remove_flag(keys,get_key_flag("esc"));
        keys = remove_flag(keys,get_key_flag("start"));
        keys = remove_flag(keys,get_key_flag("screenshot"));
        // add remove all prev buttons flag (for all hold combos) -> OPTIONAL
        if ( hold_btn_combo_flag > 0 ) {
            for ( i = 0; i < combo_step-1; ++i ) {
                keys = remove_multi_flag_btns(keys,p,get(buttons,i));
            }
        }
        //keys = remove_multi_flag_btns(keys,p,get(buttons,0));
        keys = remove_multi_flag_btns(keys,p,get(buttons,combo_step-1));
        keys = remove_multi_flag_btns(keys,p,get(buttons,combo_step));
        // key flag è ancora vero? se si reset combo

        if ( keys ) {
            // set attivata questa if, si può iniziare la combo con altri testi mentre si preme quello iniziale
            // inoltre non si tiene cnto del cambio della direzione
            /*if ( first_playerkeys_response == 1 ) { // if is the 1st step key, reset to step 0
                setlocalvar("__"+combo_id+"_comboETA",time+ETA);
                setlocalvar("__"+combo_id+"_combostep",1);
                setlocalvar("__"+combo_id+"_comboRB",NULL());
                combo_step = getlocalvar("__"+combo_id+"_combostep");

                if ( combo_step >= combo_num ) {
                    setlocalvar("__"+combo_id+"_combostep",NULL());
                    setlocalvar("__"+combo_id+"_comboETA",NULL());
                    setlocalvar("__"+combo_id+"_comboRB",NULL());
                    free(buttons);
                    return 1;
                }

                free(buttons);
                return 0;
            }*/

            setlocalvar("__"+combo_id+"_combostep",NULL());
            setlocalvar("__"+combo_id+"_comboETA",NULL());
            setlocalvar("__"+combo_id+"_comboRB",NULL());
            free(buttons);
            return 0;
        }
        //------ RESET COMBO ------

        // mappa tutti i tasti rilasciati (flag) da combo_step-1 in giù. se anche solo uno di questi viene ripremuto allora reset combo
        if ( is_a_prev_btn_repressed(p,combo_id,buttons,combo_step-1) ) {
            if ( first_playerkeys_response == 1 ) { // if is the 1st step key, reset to step 0
                setlocalvar("__"+combo_id+"_comboETA",time+ETA);
                setlocalvar("__"+combo_id+"_combostep",1);
                setlocalvar("__"+combo_id+"_comboRB",NULL());
                combo_step = getlocalvar("__"+combo_id+"_combostep");
                free(buttons);
                return 0;
            }
            setlocalvar("__"+combo_id+"_combostep",NULL());
            setlocalvar("__"+combo_id+"_comboETA",NULL());
            setlocalvar("__"+combo_id+"_comboRB",NULL());
            free(buttons);
            return 0;
        }

        //------ SET COMBO STEP ------
        playerkeys_response = playerkeys_multi(p,0,get(buttons,combo_step)); // check pressed button
        if ( playerkeys_response == 1 ) {
            setlocalvar("__"+combo_id+"_comboETA",time+ETA);
            setlocalvar("__"+combo_id+"_combostep",++combo_step);
        } else {
            if ( first_playerkeys_response == 1 ) { // if is the 1st step key, reset to step 0
                setlocalvar("__"+combo_id+"_comboETA",time+ETA);
                setlocalvar("__"+combo_id+"_combostep",1);
                setlocalvar("__"+combo_id+"_comboRB",NULL());
                combo_step = getlocalvar("__"+combo_id+"_combostep");
            }
        }
        //------ SET COMBO STEP ------

        // reset if combo is ok
        if ( combo_step >= combo_num ) {
            setlocalvar("__"+combo_id+"_combostep",NULL());
            setlocalvar("__"+combo_id+"_comboETA",NULL());
            setlocalvar("__"+combo_id+"_comboRB",NULL());

            free(buttons);
            return 1;
        }

        free(buttons);
    }

    return 0;
}

int is_a_prev_btn_repressed(int p, char combo_id, char buttons, int combo_step) {
    void player = getplayerproperty(p,"entity");

    if ( getentityproperty(player,"exists") ) {
        int comboRB = getlocalvar("__"+combo_id+"_comboRB");
        int i = 0;

        if ( combo_step < 0 ) return 0;
        if ( comboRB == NULL() ) comboRB = 0;

        // check prev buttons
        for ( i = 0; i < combo_step+1; ++i ) {
            if ( !is_flag_on(comboRB,get_flag_from_value(i)) ) {
                if ( playerkeys_multi(p,0,get(buttons,i)) != 1 ) {
                    comboRB = add_flag(comboRB,i);
                }
            }
        }
        setlocalvar("__"+combo_id+"_comboRB",comboRB);
        comboRB = getlocalvar("__"+combo_id+"_comboRB");

        // check prev repressed buttons
        for ( i = 0; i < combo_step+1; ++i ) {
            if ( is_flag_on(comboRB,get_flag_from_value(i)) ) {
                // == 1 or != 0??
                if ( playerkeys_multi(p,0,get(buttons,i)) == 1 ) return 1;
            }
        }
    }

    return 0;
}

int remove_multi_flag_btns(int flag, int p, char key_string) {
    char buttons = array(0);
    int button_num = 0;
    int src_index = -1;
    int i = 0;

    if ( key_string == NULL() ) { free(buttons); return flag; }
    if ( strlength(key_string) <= 0 ) { free(buttons); return flag; }

    // set array buttons
    do {
        src_index = searchstr_index(key_string,"+");

        if ( src_index == -1 && strlength(key_string) <= 0 ) break;
        else if ( src_index == -1 && strlength(key_string) > 0 ) {
            set(buttons,button_num++,key_string);
            break;
        }

        set(buttons,button_num++,substr(key_string,0,src_index));
        key_string = substr(key_string,src_index+1,strlength(key_string)-src_index-1);
    } while( src_index != -1 );

    for( i = 0; i < button_num; ++i ) {
        char key = get_pkeyc(p,get(buttons,i));

        flag = remove_flag(flag,get_key_flag(  key  ));
    }

    free(buttons);
    return flag;
}

/// ----------- MAIN func -----------
// return 1 if all buttons are pressed, 2 if button are partially pressed and 0 if buttons aren't pressed
int playerkeys_multi(int p, int hold_flag, char key_string) {
    char buttons = array(0);
    int button_num = 0, pressed_btns = 0;
    int src_index = -1;
    int i = 0;

    if ( key_string == NULL() ) { free(buttons); return 0; }
    if ( strlength(key_string) <= 0 ) { free(buttons); return 0; }

    // set array buttons
    do {
        src_index = searchstr_index(key_string,"+");

        if ( src_index == -1 && strlength(key_string) <= 0 ) break;
        else if ( src_index == -1 && strlength(key_string) > 0 ) {
            set(buttons,button_num++,key_string);
            break;
        }

        set(buttons,button_num++,substr(key_string,0,src_index));
        key_string = substr(key_string,src_index+1,strlength(key_string)-src_index-1);
    } while( src_index != -1 );

    for( i = 0; i < button_num; ++i ) {
        char key = get_pkeyc(p,get(buttons,i));

        if ( playerkeys(p,hold_flag,key) ) ++pressed_btns;
    }

    free(buttons);
    if ( button_num == pressed_btns ) return 1;
    else if ( pressed_btns > 0 ) return 2;
    else return 0;
}

char get_pkeyc(int p, char key) {
    void player = getplayerproperty(p,"entity");

    if ( ent_exists(player) ) {
        int dir = getentityproperty(player,"direction");

        if ( key == "forward" ) {
            if ( dir ) return "moveright";
            else return "moveleft";
        } else if ( key == "backward" ) {
            if ( dir ) return "moveleft";
            else return "moveright";
        } else if ( key == "up" ) return "moveup";
        else if ( key == "down" ) return "movedown";
        else return key;
    } else return NULL();
}

/// ----------- MAIN func -----------
int is_multi_hold_pressed(int p, char btn1, char btn2, char btn3, char btn4, char btn5, char btn6, char btn7, char btn8) {
    int btn_num = 0;
    int btn_flag = 0;

    if ( btn1 != NULL() ) {
        ++btn_num;
        if ( get_hold_time(p,btn1) >= 0 ) ++btn_flag;
    }
    if ( btn2 != NULL() ) {
        ++btn_num;
        if ( get_hold_time(p,btn2) >= 0 ) ++btn_flag;
    }
    if ( btn3 != NULL() ) {
        ++btn_num;
        if ( get_hold_time(p,btn3) >= 0 ) ++btn_flag;
    }
    if ( btn4 != NULL() ) {
        ++btn_num;
        if ( get_hold_time(p,btn4) >= 0 ) ++btn_flag;
    }
    if ( btn5 != NULL() ) {
        ++btn_num;
        if ( get_hold_time(p,btn5) >= 0 ) ++btn_flag;
    }
    if ( btn6 != NULL() ) {
        ++btn_num;
        if ( get_hold_time(p,btn6) >= 0 ) ++btn_flag;
    }
    if ( btn7 != NULL() ) {
        ++btn_num;
        if ( get_hold_time(p,btn7) >= 0 ) ++btn_flag;
    }
    if ( btn8 != NULL() ) {
        ++btn_num;
        if ( get_hold_time(p,btn8) >= 0 ) ++btn_flag;
    }

    if ( btn_flag == btn_num ) return 1;
    else return 0;
}

void hold_keyc(int p) {
    //int time = openborvariant("elapsed_time");
    //int game_speed = openborvariant("game_speed");
    int i = 0;

    for (i = 0; i < keys_num(); ++i) {
        int key = get_keyc(i);
        int key_flag = playerkeys(p,0,key);

        if ( key_flag ) {
          if ( get_hold_time(p,key) == -1 ) set_hold_time(p,key);
        } else {
          set_hold_time(p,key,-1);
        }
    }

    return;
}

char get_keyc(int i) {
         if ( i ==  0 ) return "esc";
    else if ( i ==  1 ) return "start";
    else if ( i ==  2 ) return "moveleft";
    else if ( i ==  3 ) return "moveright";
    else if ( i ==  4 ) return "moveup";
    else if ( i ==  5 ) return "movedown";
    else if ( i ==  6 ) return "attack";
    else if ( i ==  7 ) return "attack2";
    else if ( i ==  8 ) return "attack3";
    else if ( i ==  9 ) return "attack4";
    else if ( i == 10 ) return "jump";
    else if ( i == 11 ) return "special";
    else if ( i == 12 ) return "screenshot";
    else if ( i == 13 ) return "anybutton";
}

int get_key_flag(char label) {
    // 2^0 =1; 2^1 = 2; 2^2 = 4; 2^3 = 8; 2^4 = 16; 2^5 = 32; 2^6 = 64; 2^7 = 128; 2^8 = 256;
    int flag_value = 0;

         if ( label == "esc"        ) flag_value = 0;
    else if ( label == "start"      ) flag_value = 1;
    else if ( label == "moveleft"   ) flag_value = 2;
    else if ( label == "moveright"  ) flag_value = 3;
    else if ( label == "moveup"     ) flag_value = 4;
    else if ( label == "movedown"   ) flag_value = 5;
    else if ( label == "attack"     ) flag_value = 6;
    else if ( label == "jump"       ) flag_value = 7;
    else if ( label == "special"    ) flag_value = 8;
    else if ( label == "screenshot" ) flag_value = 9;
    else if ( label == "attack2"    ) flag_value = 10;
    else if ( label == "attack3"    ) flag_value = 11;
    else if ( label == "attack4"    ) flag_value = 12;

    return flag_value;
}

int keys_num() {
    return 14;
}

/// ----------- MAIN func -----------
int get_hold_time(int p, char key) {
    int time = openborvariant("elapsed_time");
    int key_flag = playerkeys(p,0,key);

    if ( key_flag ) {
        int hold_start_time = getglobalvar("__"+p+"_"+key+"");

        if ( hold_start_time == NULL() || hold_start_time == -1 ) {
            return -1;
        } else {
            return (time-hold_start_time);
        }
    } else return -1;
}

void set_hold_time(int p, char key, int value) {
    int key_flag = playerkeys(p,0,key);

    if ( value == NULL() ) {
        if ( key_flag ) setglobalvar("__"+p+"_"+key+"", openborvariant("elapsed_time"));
    } else {
        setglobalvar("__"+p+"_"+key+"", value);
    }
}

