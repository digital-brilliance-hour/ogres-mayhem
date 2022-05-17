
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
//#import "data/scripts/custom_hud_commons.c"
#import "data/scripts/lib_drawing.c"

#define ICON_NUM 4
#define X_PLAYER 110+i*52
#define Y_PLAYER 40+4
#define X_SCORE 20
#define Y_SCORE 80
#define CLEAR_BONUS 100
#define LIFE_BONUS 20
#define MAX_SCORE 999999
#define CREDSCORE 200
#define LIFESCORE 100
#define PLAYER_SCORE_FLAG 1
#define PIMP_UNIT_VALUE 1
#define PIMP_UNIT_MUL_EVERY_VALUE 200 // multuply the pimp_value every ORIGINAL_SCORE/THIS_VALUE (ex. 200)
#define ADD_SCORE_IN_TOT_SCORE_FLAG 1
#define START_ETA_TIME 0.4
#define PIMP_ETA_TIME 0.04
#define CLOSE_TIME 2

// DA FARE:

void main() {
    set_scores();
    show_bg();
    check_score_completed();

    //drawstring(10,30,0,""+anykeyc());
}

void set_scores() {
    int i = 0;
    int time = ov("elapsed_time");
    int game_speed = ov("game_speed");
    int start_ETA = game_speed*START_ETA_TIME, pimp_ETA = game_speed*PIMP_ETA_TIME;
    int sndfx_selected = loadsample("data/sounds/beep2.wav");
    int life_up = loadsample("data/sounds/1up.wav");
    int credit_up = loadsample("data/sounds/credit_up.wav");
    int interrupt = 0, scores_flag = 0;

    if ( glv("all_score_completed") == ov("count_players") ) return;

    // INIT SCORES
    if ( glv("set_scores_flag") == NULL() ) {
        for ( i = 0; i < MAX_PLAYERS; ++i ) {
            void player = gpp(i, "entity");

            if ( !gep(player,"exists") ) continue;

            //cpp(i,"score",200);
            if ( CLEAR_BONUS > MAX_SCORE ) slv("clear_bonus_score"+i,MAX_SCORE); else slv("clear_bonus_score"+i,CLEAR_BONUS);
            if ( LIFE_BONUS*gpp(i,"lives") > MAX_SCORE ) slv("life_bonus_score"+i,MAX_SCORE); else slv("life_bonus_score"+i,LIFE_BONUS*gpp(i,"lives"));
            slv("player_score"+i,gpp(i,"score"));
            if ( ADD_SCORE_IN_TOT_SCORE_FLAG && (!PLAYER_SCORE_FLAG || PLAYER_SCORE_FLAG == 0 ) ) slv("tot_score"+i,gpp(i,"score")); else slv("tot_score"+i,0);
            slv("old_lives"+i,gpp(i,"lives"));
            slv("old_credits"+i,gpp(i,"credits"));
            slv("old_score"+i,gpp(i,"score"));
        }
        slv("set_scores_flag",1);
    }

    //drawstring(10,10,0,""+ov("count_players")+"  "+glv("all_score_completed"));
    if ( glv("set_scores_time") == NULL() ) {
        slv("set_scores_time",get_next_time(start_ETA)); // time+ETA
        return;
    } else if ( time < glv("set_scores_time") ) {
        return;
    }

    // INTERRUPT
    if ( anykeyc() && glv("all_score_completed") != ov("count_players") ) {
        interrupt = 1;
    }

    // CHECK NO SCORES
    for ( i = 0; i < MAX_PLAYERS; ++i ) {
        if ( PLAYER_SCORE_FLAG >= 1 ) { if ( glv("clear_bonus_score"+i) > 0 || glv("life_bonus_score"+i) > 0 || glv("player_score"+i) > 0 ) ++scores_flag; }
        else { if ( glv("clear_bonus_score"+i) > 0 || glv("life_bonus_score"+i) > 0 ) ++scores_flag; }
    } // NO SCORES IF scores_flag == 0

    // PLAY SCORE SOUND
    if ( glv("all_score_completed") != ov("count_players") && scores_flag != 0 && PIMP_UNIT_VALUE > 0 ) {
        if ( glv("score_sound_time") == NULL() ) {
            slv("score_sound_time",get_next_time(game_speed*0.1));
        } else if ( time >= glv("score_sound_time") ) {
            playsample(sndfx_selected);
            slv("score_sound_time",NULL());
        }
    }

    // PIMP SCORES
    if ( glv("score_pimp_time") == NULL() ) {
        slv("score_pimp_time",get_next_time(pimp_ETA));
    } else if ( time >= glv("score_pimp_time") ) {
          for ( i = 0; i < MAX_PLAYERS; ++i ) {
            int sum = 0;
            int pimp_value = PIMP_UNIT_VALUE, pimp_unit = pimp_value; // refresh every pimp_ETA
            int partial_score;

            if ( glv("score_completed"+i) == 1 ) continue;

            // SET PIMP VALUE
            if ( PLAYER_SCORE_FLAG >= 1 ) {
                int mul = glv("old_score"+i)/PIMP_UNIT_MUL_EVERY_VALUE; // or glv("player_score"+i)

                //mul *= 2;
                mul = truncA(mul);
                if ( mul >= 1 ) {
                    pimp_value *= mul;
                    pimp_value = truncA(pimp_value);
                    pimp_unit = pimp_value;
                }
            }

            pimp_unit = time-glv("score_pimp_time"); pimp_unit /= pimp_ETA; pimp_unit = (truncA(pimp_unit)+1)*pimp_value; // how units with lost frames?
            // use (time-glv("score_pimp_time"))%pimp_ETA fo the rets

            if ( glv("clear_bonus_score"+i) >= pimp_unit && !interrupt ) { sum += pimp_unit; slv("clear_bonus_score"+i,glv("clear_bonus_score"+i)-pimp_unit); }
            else if ( glv("clear_bonus_score"+i) > 0 ) { sum += glv("clear_bonus_score"+i); slv("clear_bonus_score"+i,0); }

            if ( glv("life_bonus_score"+i) >= pimp_unit && !interrupt ) { sum += pimp_unit; slv("life_bonus_score"+i,glv("life_bonus_score"+i)-pimp_unit); }
            else if ( glv("life_bonus_score"+i) > 0 ) { sum += glv("life_bonus_score"+i); slv("life_bonus_score"+i,0); }

            if ( PLAYER_SCORE_FLAG >= 1 ) {
                if ( glv("player_score"+i) >= pimp_unit && !interrupt ) { sum += pimp_unit; slv("player_score"+i,glv("player_score"+i)-pimp_unit); }
                else if ( glv("player_score"+i) > 0 ) { sum += glv("player_score"+i); slv("player_score"+i,0); }
            }

            // SUM TO TOTAL
            if ( glv("tot_score"+i)+sum > MAX_SCORE ) slv("tot_score"+i,MAX_SCORE); else slv("tot_score"+i,glv("tot_score"+i)+sum);

            // CHECK FOR LIVES/CREDITS UPDATE
            if ( glv("life_score"+i) == NULL() ) slv("life_score"+i,0);
            if ( glv("credits_score"+i) == NULL() ) slv("credits_score"+i,0);
            partial_score = glv("tot_score"+i) - gpp(i,"score") - glv("life_score"+i);
            if ( partial_score/LIFESCORE >= 1 ) {
                int bonus = truncA(partial_score/LIFESCORE);

                //playsample(life_up);
                cpp(i,"lives",gpp(i,"lives")+bonus); // UPDATE PLAYER LIVES
                slv("life_score"+i,glv("life_score"+i)+bonus*LIFESCORE);
            }
            partial_score = glv("tot_score"+i) - gpp(i,"score") - glv("credits_score"+i);
            if ( partial_score/CREDSCORE >= 1 ) {
                int bonus = truncA(partial_score/CREDSCORE);

                //playsample(credit_up);
                cpp(i,"credits",gpp(i,"credits")+bonus); // UPDATE PLAYER LIVES
                slv("credits_score"+i,glv("credits_score"+i)+bonus*CREDSCORE);
            }
            /////////////////////////

            // COMPLETED
            if ( PLAYER_SCORE_FLAG >= 1 ) {
                if ( glv("clear_bonus_score"+i) <= 0 && glv("life_bonus_score"+i) <= 0 && glv("player_score"+i) <= 0 ) { // completed
                    slv("score_completed"+i,1);
                    if ( glv("all_score_completed") == NULL() ) slv("all_score_completed",1);
                    else slv("all_score_completed",glv("all_score_completed")+1);

                    cpp(i,"score",gpp(i,"score")+glv("tot_score"+i)-glv("old_score"+i)); // UPDATE PLAYER SCORE
                }
            } else {
                if ( glv("clear_bonus_score"+i) <= 0 && glv("life_bonus_score"+i) <= 0 ) { // completed
                    slv("score_completed"+i,1);
                    if ( glv("all_score_completed") == NULL() ) slv("all_score_completed",1);
                    else slv("all_score_completed",glv("all_score_completed")+1);

                    if ( ADD_SCORE_IN_TOT_SCORE_FLAG ) cpp(i,"score",gpp(i,"score")+glv("tot_score"+i)-glv("old_score"+i)); // UPDATE PLAYER SCORE
                    else cpp(i,"score",gpp(i,"score")+glv("tot_score"+i));
                }
            }
        }

        // SOMEBODY 1UP AT THE COMPLETION TIME?
        if ( glv("all_score_completed") == ov("count_players") ) {
            int ok_flag = 0;

            for ( i = 0; i < MAX_PLAYERS; ++i ) {
                if ( glv("old_credits"+i) != gpp(i,"credits") || glv("old_lives"+i) != gpp(i,"lives") ) ok_flag = 1;
            }
            if ( ok_flag ) playsample(life_up);
        }

        slv("score_pimp_time",NULL());
    }

    return;
}

void show_bg() {
    char text1  = "STAGE "+ov("current_stage")+"  COMPLETED";
    //char text2  = "save your score";
	int vres = ov("vresolution");
	int hres = ov("hresolution");
	int i = 0, y_score = 0;

	if ( getlocalvar("enter_name_spr1") == NULL() ) {
		setlocalvar("enter_name_spr1", loadsprite("data/bgs/hi_raph.gif"));
		setlocalvar("enter_name_spr2", loadsprite("data/bgs/hi_don.gif"));
		setlocalvar("enter_name_spr3", loadsprite("data/bgs/hi_mike.gif"));
		setlocalvar("enter_name_spr4", loadsprite("data/bgs/hi_leo.gif"));
	}

    drawstring(hres/2-3-strwidth(text1, 0)/2, 10, 6, text1);
    //drawstring(hres/2-3-strwidth(text2, 0)/2, 20+6, 0, text2);

    for ( i = 0; i < MAX_PLAYERS; ++i) {
        drawstring(X_PLAYER, Y_PLAYER, 6, (i+1)+"P");
    }

    if ( PLAYER_SCORE_FLAG >= 1 ) drawstring(X_SCORE, Y_SCORE, 0, "score");
    else y_score = 20;
    drawstring(X_SCORE, Y_SCORE+20-y_score, 0, "clear bonus");
    drawstring(X_SCORE, Y_SCORE+40-y_score, 0, "life bonus");
    drawstring(X_SCORE, Y_SCORE+104-y_score, 0, "total score");

    for ( i = 0; i < MAX_PLAYERS; ++i) {
        void player = gpp(i, "entity");

        if ( !gep(player,"exists") ) continue;

        if ( PLAYER_SCORE_FLAG >= 1 ) drawstring(X_PLAYER, Y_SCORE, 0, glv("player_score"+i));
        else y_score = 20;
        drawstring(X_PLAYER, Y_SCORE+20-y_score, 0, glv("clear_bonus_score"+i));
        drawstring(X_PLAYER, Y_SCORE+40-y_score, 0, glv("life_bonus_score"+i));
        drawstring(X_PLAYER, Y_SCORE+104-y_score, 0, glv("tot_score"+i));
    }

    drawOrizScaledRouletteBg(glv("enter_name_spr1"),glv("enter_name_spr2"),glv("enter_name_spr3"),glv("enter_name_spr4"),80,132,1.60,1.80,0);
    //drawOrizScaledRouletteBg(glv("enter_name_spr1"),glv("enter_name_spr2"),glv("enter_name_spr3"),glv("enter_name_spr4"),96,128,1.60,1.80,0);
}

void check_score_completed() {
    //int i = 0;
    //int complete_flag = 0;

    /*for ( i = 0; i < MAX_PLAYERS; ++i ) {
        void player = gpp(i, "entity");

        if ( !gep(player,"exists") ) continue;
        if ( glv("score_completed"+i) == 1 ) ++complete_flag;
    }*/
    if ( glv("all_score_completed") == ov("count_players") ) {
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

	for (i = 1; i <= ICON_NUM; ++i) if ( glv("enter_name_spr"+i) ) { free(glv("enter_name_spr"+i)); }
}
