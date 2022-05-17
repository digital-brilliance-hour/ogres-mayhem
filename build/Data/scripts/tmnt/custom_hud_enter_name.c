
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/custom_hud_hiscores.c"
//#import "data/scripts/custom_hud_commons.c"
#import "data/scripts/lib_drawing.c"

#define ICON_NUM 4
#define MAX_CHAR 12
#define CURSOR_Z 999
#define CURSOR_YSHIFT 8
#define CURSOR_SPRITE "data/sprites/enter_name_cur.gif"
#define FIRST_KEYPRESS 1
#define IMPULSE_TIME 0.08
#define FIRST_IMPULSE_TIME 0.2
#define X_PLAYER 20
#define Y_PLAYER 80+38*i
#define X_NAME 110
#define X_SCORE 250
#define CLOSE_TIME 2
#define COUNTDOWN_TIME 200

// DA FARE:

void main() {
    show_bg();
    enter_player_name();
    check_name_completed();

    //drawstring(10,30,0,""+setchar("ABC",1,""));
}

void save_name(int index) {
    void player = gpp(index, "entity");
    char name = "";
    char name_array = glv("player_name_array"+index);
    int i = 0;

    // save name into string
    for ( i = 0; i < size(name_array); ++i ) {
        name += get(name_array,i);
    }

    // save name into string
    i = strlength(name)-1;
    while ( getchar(name,i) == " " && i >= 0 ) {
        name = setchar(name,i,"");
        if ( i > 0 ) i = strlength(name)-1;
    }
    if ( name == "" ) name = "NONE";

    /// ######### SAVE YOUR SCORE/NAME #########
    update_hiscores(player,name);

    //if ( glv("player_name_array"+index) ) { free(glv("player_name_array"+index)); slv("player_name_array"+index,NULL()); }
}

void enter_player_name() {
	int vres = ov("vresolution");
	int hres = ov("hresolution");
	int time = ov("elapsed_time");
	int game_speed = ov("game_speed");
	int sndfx_selected = loadsample("data/sounds/beep2.wav");
	int sndfx_move_cur = loadsample("data/sounds/beep.wav");
	int sndfx_cancel = loadsample("data/sounds/cancel01.wav");
	int i = 0, c = 0;

	if ( glv("set_names_flag") == NULL() ) {
		for ( i = 0; i < MAX_PLAYERS; ++i ) slv("player_name_array"+i, array(MAX_CHAR));
		setlocalvar("set_names_flag",1);
	}

    for ( i = 0; i < MAX_PLAYERS; ++i ) {
        void player = gpp(i, "entity");
        char player_name_array = glv("player_name_array"+i);
        int cursor_width = 9;
        int press_type = 1;
        int CURSOR_X = X_NAME, CURSOR_Y = Y_PLAYER;

        //hold_key_press(i,"movedown",1.0);
        if ( !gep(player,"exists") ) continue;

        if ( glv("name_completed"+i) == NULL() ) slv("name_completed"+i, 0); // flag
        if ( glv("name_index"+i) == NULL() ) slv("name_index"+i, 0); // player name index
        if ( glv("cursor_x"+i) == NULL() ) slv("cursor_x"+i, CURSOR_X);
        if ( glv("cursor_y"+i) == NULL() ) slv("cursor_y"+i, CURSOR_Y - 8);

        if ( glv("player_name_array"+i) == NULL() ) continue;

        // Init Player Name
        if ( get( glv("player_name_array"+i), glv("name_index"+i) ) == NULL() ) {
            for ( c = 0; c < MAX_CHAR; ++c ) {
                if ( c == 0 ) set( glv("player_name_array"+i), c, get_charlist(1) );
                else set( glv("player_name_array"+i), c, get_charlist(0) );
            }
        }

        // Draw Player Name
        for ( c = 0; c < MAX_CHAR; ++c ) drawstring(X_NAME+cursor_width*c, Y_PLAYER, 6, get( glv("player_name_array"+i), c));

        // RESETNAME?
        if ( glv("name_completed"+i) == 1 && glv("all_score_completed") != ov("count_players") && (keyp(i,"special",1) || keyp(i,"jump",1)) ) {
            playsample(sndfx_cancel);
            slv("all_score_completed",glv("all_score_completed")-1);
            slv("name_completed"+i,0);
            slv("name_index"+i, 0);
            slv("cursor_x"+i, CURSOR_X);
            slv("cursor_y"+i, CURSOR_Y - CURSOR_YSHIFT);
        }
        // STOP COMMANDS FOR THIS PLAYER
        if ( glv("name_completed"+i) == 1 ) continue;
        // Draw Cursor
        if ( time%(game_speed/8) > (game_speed/8)/2 ) drawsprite(glv("enter_name_cursor"), glv("cursor_x"+i), glv("cursor_y"+i), CURSOR_Z);
        // Draw END character if cursor is in last index
        if ( glv("name_index"+i) == MAX_CHAR-1 && get_charlist_index(get( glv("player_name_array"+i), glv("name_index"+i) )) != get_charlist_size()-1 ) drawstring(X_NAME+cursor_width*MAX_CHAR, Y_PLAYER, 6, get_charlist(get_charlist_size()-1));

        //if ( keyp(i,"moveright",press_type) ) {
        if ( hold_key_press(i,"moveright",IMPULSE_TIME,FIRST_KEYPRESS,FIRST_IMPULSE_TIME) == oc("FLAG_MOVERIGHT") ) {
            char chara;
            int c_index;

            playsample(sndfx_move_cur);
            chara = get( glv("player_name_array"+i), glv("name_index"+i) );
            c_index = get_charlist_index(chara);
            if ( c_index >= get_charlist_size()-2 ) set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(0) ); // NO SPECIAL CHATACTERS

            if ( glv("name_index"+i) < MAX_CHAR-1 ) {
                slv("cursor_x"+i, glv("cursor_x"+i)+cursor_width);
                slv("name_index"+i, glv("name_index"+i)+1);
            } else {
                slv("cursor_x"+i, CURSOR_X);
                slv("name_index"+i, 0);
            }
        } else if ( hold_key_press(i,"moveleft",IMPULSE_TIME,FIRST_KEYPRESS,FIRST_IMPULSE_TIME) == oc("FLAG_MOVELEFT") ) {
            char chara;
            int c_index;

            playsample(sndfx_move_cur);
            chara = get( glv("player_name_array"+i), glv("name_index"+i) );
            c_index = get_charlist_index(chara);
            if ( c_index >= get_charlist_size()-2 ) set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(0) ); // NO SPECIAL CHATACTERS

            if ( glv("name_index"+i) > 0 ) {
                slv("cursor_x"+i, glv("cursor_x"+i)-cursor_width);
                slv("name_index"+i, glv("name_index"+i)-1);
            } else {
                slv("cursor_x"+i, CURSOR_X+cursor_width*(MAX_CHAR-1));
                slv("name_index"+i, MAX_CHAR-1);
            }
        } else if ( hold_key_press(i,"movedown",IMPULSE_TIME,FIRST_KEYPRESS,FIRST_IMPULSE_TIME) == oc("FLAG_MOVEDOWN") ) {
            char chara;
            int c_index;

            playsample(sndfx_move_cur);
            chara = get( glv("player_name_array"+i), glv("name_index"+i) );
            // retrieve a char index and sum 1
            c_index = get_charlist_index(chara);
            if ( c_index+1 > get_charlist_size()-1 ) c_index = 0; else ++c_index;
            // set chara
            set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(c_index) );
        } else if ( hold_key_press(i,"moveup",IMPULSE_TIME,FIRST_KEYPRESS,FIRST_IMPULSE_TIME) == oc("FLAG_MOVEUP") ) {
            char chara;
            int c_index;

            playsample(sndfx_move_cur);
            chara = get( glv("player_name_array"+i), glv("name_index"+i) );
            // retrieve a char index and sum 1
            c_index = get_charlist_index(chara);
            if ( c_index-1 < 0 ) c_index = get_charlist_size()-1; else --c_index;
            // set chara
            set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(c_index) );
        } else if( hold_key_press(i,"special",IMPULSE_TIME,FIRST_KEYPRESS,FIRST_IMPULSE_TIME) == oc("FLAG_SPECIAL") || hold_key_press(i,"jump",IMPULSE_TIME,FIRST_KEYPRESS,FIRST_IMPULSE_TIME) == oc("FLAG_JUMP") ) {
            playsample(sndfx_cancel);
            // NOT CANCEL THE 1ST CHARACTER
            if ( glv("name_index"+i) > 0 ) set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(0) ); // cancel character
            else set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(1) );

            if ( glv("name_index"+i) > 0 ) {
                slv("cursor_x"+i, glv("cursor_x"+i)-cursor_width);
                slv("name_index"+i, glv("name_index"+i)-1);
            }
        } else if( hold_key_press(i,"attack",IMPULSE_TIME,FIRST_KEYPRESS,FIRST_IMPULSE_TIME) == oc("FLAG_ATTACK") ) {
            char chara;
            int c_index;

            chara = get( glv("player_name_array"+i), glv("name_index"+i) );
            c_index = get_charlist_index(chara);
            if ( c_index == get_charlist_size()-2 ) { // BACK CHARACTER
                playsample(sndfx_cancel);
                set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(0) ); // cancel character

                if ( glv("name_index"+i) > 0 ) {
                    slv("cursor_x"+i, glv("cursor_x"+i)-cursor_width);
                    slv("name_index"+i, glv("name_index"+i)-1);
                    // NOT CANCEL THE 1ST CHARACTER
                    set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(0) ); // cancel character
                } else set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(1) ); // NOT CANCEL THE 1ST CHARACTER
            } else if ( c_index == get_charlist_size()-1 ) { // END CHARACTER
                playsample(sndfx_selected);
                set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(0) ); // cancel character
                if ( glv("name_index"+i) > 0 ) {
                    slv("cursor_x"+i, glv("cursor_x"+i)-cursor_width);
                    slv("name_index"+i, glv("name_index"+i)-1);
                }
                slv("name_completed"+i, 1);
                if ( glv("all_score_completed") == NULL() ) slv("all_score_completed",1);
                else slv("all_score_completed",glv("all_score_completed")+1);
                save_name(i);
            } else {
                playsample(sndfx_selected);
                if ( glv("name_index"+i) < MAX_CHAR-1 ) {
                    slv("cursor_x"+i, glv("cursor_x"+i)+cursor_width);
                    slv("name_index"+i, glv("name_index"+i)+1);
                    if ( get( glv("player_name_array"+i), glv("name_index"+i) ) == get_charlist(0) ) set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(1) );
                } else { // END NAME STRING
                    slv("name_completed"+i, 1);
                    if ( glv("all_score_completed") == NULL() ) slv("all_score_completed",1);
                    else slv("all_score_completed",glv("all_score_completed")+1);
                    save_name(i);
                }
            }
        } else if( keyp(i,"start") == oc("FLAG_START") || glv("en_countdown_end") == 1 ) {
            char chara;
            int c_index;

            chara = get( glv("player_name_array"+i), glv("name_index"+i) );
            c_index = get_charlist_index(chara);
            if ( c_index >= get_charlist_size()-2 ) {
                set( glv("player_name_array"+i), glv("name_index"+i), get_charlist(0) ); // NO SPECIAL CHATACTERS
                //playsample(sndfx_cancel);
                //return;
            }

            if ( glv("en_countdown_end") == NULL() || (glv("en_countdown_end") == 1 && glv("confirm_snd") == NULL()) ) { playsample(sndfx_selected); slv("confirm_snd",1); }
            slv("name_completed"+i, 1);
            if ( glv("all_score_completed") == NULL() ) slv("all_score_completed",1);
            else slv("all_score_completed",glv("all_score_completed")+1);
            save_name(i);
            //finish_level();
        }
    }

    return;
}

void show_bg() {
    char text1  = "ENTER NAME";
    char text2  = "save your score";
	int vres = ov("vresolution");
	int hres = ov("hresolution");
    int time = ov("elapsed_time");
    int game_speed = ov("game_speed");
	int i = 0;

	if ( getlocalvar("set_bg_flag") == NULL() ) {
		setlocalvar("enter_name_spr1", loadsprite("data/bgs/hi_raph2.gif"));
		setlocalvar("enter_name_spr2", loadsprite("data/bgs/hi_don2.gif"));
		setlocalvar("enter_name_spr3", loadsprite("data/bgs/hi_mike2.gif"));
		setlocalvar("enter_name_spr4", loadsprite("data/bgs/hi_leo2.gif"));
		setlocalvar("enter_name_cursor", loadsprite(CURSOR_SPRITE));
		setlocalvar("max_players",MAX_PLAYERS);
		setlocalvar("set_bg_flag",1);
		//cpp(0,"score",250);
	}

    drawstring(hres/2-3-strwidth(text1, 0)/2, 10, 6, text1);
    drawstring(hres/2-3-strwidth(text2, 0)/2, 20+6, 0, text2);

    drawstring(X_NAME, 50, 6, "NAME");
    drawstring(X_SCORE, 50, 6, "SCORE");
    for ( i = 0; i < MAX_PLAYERS; ++i ) {
        void player = gpp(i, "entity");
        int score;
        char name;

        //if ( !gep(player,"exists") ) continue;
        score = gpp(i,"score");
        name = gpp(i,"name");
        if ( score == NULL() || score < 0 || typeof(score) == oc("VT_EMPTY") ) score = 0;
        if ( name == NULL() || typeof(name) == oc("VT_EMPTY") ) name = "NONE";

        drawstring(X_PLAYER, Y_PLAYER, 6, (i+1)+" PLAYER");
        if ( gep(player,"exists") ) drawstring(X_SCORE, Y_PLAYER, 6, score);
    }

    // TIME COUNTDOWN
    if ( COUNTDOWN_TIME > 0 ) {
        char time_txt = "TIME  ", time_digit_txt = "", time_space_txt = "";
        int font_index = 6;

        if ( getglobalvar("en_countdown_time") != NULL() ) {
            int countdown_time = getglobalvar("en_countdown_time");
            int num = ((countdown_time-time)/game_speed)%COUNTDOWN_TIME;

            if ( num < 0 ) num = 0;
            time_digit_txt = ""+num;
            if ( countdown_time >= time ) {
                // NONE
            } else {
                if ( glv("en_countdown_end") == NULL() ) slv("en_countdown_end",1);
            }
        } else {
            setglobalvar("en_countdown_time",get_next_time(COUNTDOWN_TIME*game_speed));
        }

        for (i = 0; i < strlength(""+COUNTDOWN_TIME); ++i) {
            time_space_txt += "0";
        }

        drawstring(hres/2-strwidth(time_txt+""+time_space_txt,font_index)/2,220,font_index,time_txt);
        drawstring(hres/2+strwidth(time_txt+""+time_space_txt,font_index)/2 - strwidth(time_digit_txt,font_index),220,font_index,time_digit_txt);
    }

    //drawVericalScaledRouletteBg(glv("enter_name_spr1"),glv("enter_name_spr2"),glv("enter_name_spr3"),glv("enter_name_spr4"),80,132,1.60,1.80,0);
    drawVericalScaledRouletteBg(glv("enter_name_spr1"),glv("enter_name_spr2"),glv("enter_name_spr3"),glv("enter_name_spr4"),96,128,1.60,1.80,0);
}

int get_charlist_size() {
    return 48;
}

int get_charlist_index(char character) {
         if ( character == " " ) return 0;
    else if ( character == "A" ) return 1;
    else if ( character == "B" ) return 2;
    else if ( character == "C" ) return 3;
    else if ( character == "D" ) return 4;
    else if ( character == "E" ) return 5;
    else if ( character == "F" ) return 6;
    else if ( character == "G" ) return 7;
    else if ( character == "H" ) return 8;
    else if ( character == "I" ) return 9;
    else if ( character == "J" ) return 10;
    else if ( character == "K" ) return 11;
    else if ( character == "L" ) return 12;
    else if ( character == "M" ) return 13;
    else if ( character == "N" ) return 14;
    else if ( character == "O" ) return 15;
    else if ( character == "P" ) return 16;
    else if ( character == "Q" ) return 17;
    else if ( character == "R" ) return 18;
    else if ( character == "S" ) return 19;
    else if ( character == "T" ) return 20;
    else if ( character == "U" ) return 21;
    else if ( character == "V" ) return 22;
    else if ( character == "W" ) return 23;
    else if ( character == "X" ) return 24;
    else if ( character == "Y" ) return 25;
    else if ( character == "Z" ) return 26;
    else if ( character == "0" ) return 27;
    else if ( character == "1" ) return 28;
    else if ( character == "2" ) return 29;
    else if ( character == "3" ) return 30;
    else if ( character == "4" ) return 31;
    else if ( character == "5" ) return 32;
    else if ( character == "6" ) return 33;
    else if ( character == "7" ) return 34;
    else if ( character == "8" ) return 35;
    else if ( character == "9" ) return 36;
    else if ( character == "-" ) return 37;
    else if ( character == "†" ) return 38; // "_"
    else if ( character == "!" ) return 39;
    else if ( character == "?" ) return 40;
    else if ( character == "'" ) return 41;
    else if ( character == "&" ) return 42;
    else if ( character == "$" ) return 43;
    else if ( character == "" ) return 44; // "®"
    else if ( character == "@" ) return 45;
    else if ( character == "ƒ" ) return 46; // SPECIAL: BACK CHARACETR
    else if ( character == "…" ) return 47; // SPECIAL: "END" ("„" == RETURN CHARACTER)
}

char get_charlist(int index) {
         if ( index == 0  ) return " ";
    else if ( index == 1  ) return "A";
    else if ( index == 2  ) return "B";
    else if ( index == 3  ) return "C";
    else if ( index == 4  ) return "D";
    else if ( index == 5  ) return "E";
    else if ( index == 6  ) return "F";
    else if ( index == 7  ) return "G";
    else if ( index == 8  ) return "H";
    else if ( index == 9  ) return "I";
    else if ( index == 10 ) return "J";
    else if ( index == 11 ) return "K";
    else if ( index == 12 ) return "L";
    else if ( index == 13 ) return "M";
    else if ( index == 14 ) return "N";
    else if ( index == 15 ) return "O";
    else if ( index == 16 ) return "P";
    else if ( index == 17 ) return "Q";
    else if ( index == 18 ) return "R";
    else if ( index == 19 ) return "S";
    else if ( index == 20 ) return "T";
    else if ( index == 21 ) return "U";
    else if ( index == 22 ) return "V";
    else if ( index == 23 ) return "W";
    else if ( index == 24 ) return "X";
    else if ( index == 25 ) return "Y";
    else if ( index == 26 ) return "Z";
    else if ( index == 27 ) return "0";
    else if ( index == 28 ) return "1";
    else if ( index == 29 ) return "2";
    else if ( index == 30 ) return "3";
    else if ( index == 31 ) return "4";
    else if ( index == 32 ) return "5";
    else if ( index == 33 ) return "6";
    else if ( index == 34 ) return "7";
    else if ( index == 35 ) return "8";
    else if ( index == 36 ) return "9";
    else if ( index == 37 ) return "-";
    else if ( index == 38 ) return "†"; // "_"
    else if ( index == 39 ) return "!";
    else if ( index == 40 ) return "?";
    else if ( index == 41 ) return "'";
    else if ( index == 42 ) return "&";
    else if ( index == 43 ) return "$";
    else if ( index == 44 ) return ""; // "®"
    else if ( index == 45 ) return "@";
    else if ( index == 46 ) return "ƒ"; // BACK CHARACETR
    else if ( index == 47 ) return "…"; // "END" ("„" == RETURN CHARACTER)
}

void check_name_completed() {
    int i = 0;
    int complete_flag = 0;

    for ( i = 0; i < MAX_PLAYERS; ++i ) {
        void player = gpp(i, "entity");

        if ( !gep(player,"exists") ) continue;
        if ( glv("name_completed"+i) == 1 ) ++complete_flag;
    }
    if ( complete_flag == ov("count_players") ) {
        int time = ov("elapsed_time");
        int gamespeed = ov("game_speed");
        int ETA = gamespeed*CLOSE_TIME;

        if ( glv("finish_level_time") == NULL() ) slv("finish_level_time",get_next_time(ETA)); // time+ETA
        else if ( time >= glv("finish_level_time") ) {
            finish_level();
        }
    }
}

void ondestroy() {
	int i;

	for (i = 1; i <= ICON_NUM; ++i) { if ( glv("enter_name_spr"+i) ) { free(glv("enter_name_spr"+i)); } }
	for (i = 0; i < glv("max_players"); ++i) { if ( glv("player_name_array"+i) ) { free(glv("player_name_array"+i)); slv("player_name_array"+i,NULL()); } }
	if ( glv("enter_name_cursor") ) { free(glv("enter_name_cursor")); }
}
