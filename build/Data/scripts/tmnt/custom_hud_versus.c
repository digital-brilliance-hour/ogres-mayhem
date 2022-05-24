
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/custom_hud_stage_select.c"
#import "data/scripts/custom_hud_player_select.c"
#import "data/scripts/custom_hud_commons.c"
#import "data/scripts/lib_drawing.c"
//#import "data/scripts/var_resetter.c"

#define MUSIC "data/music/score01.bor"
#define STAGE_SELECT_TYPE 1
#define Y_SCR_TITLE 14
#define X_PLAYERS 40
#define Y_PLAYERS 80
#define LINE_SPACES 20
#define X_MARKS 80
#define Y_WINNER 50

void main() {
    //drawstring(10,100,0,openborvariant("nofadeout"));
    //drawstring(10,100,0,glv("1VS1_stageitem"));
    //drawstring(10,110,0,ggv("1VS1"));

    if ( glv("start") == NULL() ) {
        if ( glv("mode") == NULL() ) {
            show_versus_summary();
        }
    } else if ( glv("start") == "mode" ) {
        if ( glv("mode") == "stage" ) {
            stageSelect(STAGE_SELECT_TYPE);
        }
    } else if ( glv("start") == "select" ) {
        selectPlayer();
    }
}

void oncreate() {
    int i = 0;

    sgv("1VS1",1); // <--- to comment (optional)

    setup_vs_summary();
    setupStages();
    setup_1VS_Stages();

    slv("mselector",0);
    //changeopenborvariant("nofadeout", 1);
    changeplayerproperty(0,"lives",0); // to avoid game over (in vs_winner_msg)

    for ( i = 0; i < MAX_PLAYERS; ++i ) {
        if ( ggv("versus_mark_"+i) == NULL() ) sgv("versus_mark_"+i,0);
        if ( ggv("versus_mark_tot_"+i) == NULL() ) sgv("versus_mark_tot_"+i,0);

        sgv("versus_mark_tot_"+i,ggv("versus_mark_tot_"+i)+ggv("versus_mark_"+i));
        sgv("versus_mark_"+i,0); // reset versus marks

        if ( ggv("versus_wins_"+i) == NULL() ) sgv("versus_wins_"+i,0);
    }

    if ( ggv("vs_winner") != NULL() && ggv("vs_winner") >= 0 ) {
        int p = ggv("vs_winner");
        sgv("versus_wins_"+p,ggv("versus_wins_"+p)+1);

        // anti-bug zero credits
        cpp(p,"credits",gpp(p,"credits")+1);
        sgv("current_credits_"+p,gpp(p,"credits"));
    }

    sgv("vs_fight_flag",NULL());

    slv("versus_menu",1);
}

void ondestroy() {
    destroy_vs_summar_vars();
}

void destroy_vs_summar_vars() {
    int i = 0;

    if( glv("text_highlight1") ) { free(glv("text_highlight1")); slv("text_highlight1",NULL()); }
    if( glv("text_highlight2") ) { free(glv("text_highlight2")); slv("text_highlight2",NULL()); }
    if( glv("txt_map1") ) { free(glv("txt_map1")); slv("txt_map1",NULL()); }

    if( glv("logo") ) { free(glv("logo")); slv("logo",NULL()); }
    if( glv("versus_mark") ) { free(glv("versus_mark")); slv("versus_mark",NULL()); }
    if( glv("versus_item") ) { free(glv("versus_item")); slv("versus_item",NULL()); }
	if( glv("playerlist") ) { free(glv("playerlist")); slv("playerlist",NULL()); }
	if( glv("playerflag") ) { free(glv("playerflag")); slv("playerflag",NULL()); }
    if( glv("1VS1_stageitem") ) { free(glv("1VS1_stageitem")); slv("1VS1_stageitem",NULL()); }
    if( glv("1VS1_stageprev") ) {
        for( i = 0; i < size(glv("1VS1_stageprev")); i++) free(get(glv("1VS1_stageprev"),i));
        free(glv("1VS1_stageprev"));
        slv("1VS1_stageprev",NULL());
    }
    if( glv("stageitem") ) { free(glv("stageitem")); slv("stageitem",NULL()); }
    if( glv("stageprev") ) {
        for( i = 0; i < size(glv("stageprev")); i++) free(get(glv("stageprev"),i));
        free(glv("stageprev"));
        slv("stageprev",NULL());
    }
    slv("start",NULL());
    slv("mode",NULL());
    slv("wait",NULL());
    slv("versus_menu",NULL());
    slv("ps_load_flag",NULL());
    if( glv("time_icon") ) { free(glv("time_icon")); slv("time_icon", NULL()); }

    sgv("vs_fight_flag",NULL());
}

void setup_vs_summary() {
    int i = 0;
    void items = array(0);

    set(items, i++, "next  battle");
    set(items, i++, "exit  to  main  menu");

    slv("versus_item",items);
}

void show_versus_summary() {
	int i;
	char text  = "VERSUS  SUMMARY", text_winner = "";
	int se = loadsample("data/sounds/beep2.wav");
	int se2 = loadsample("data/sounds/beep.wav");
	int se3 = loadsample("data/sounds/cancel01.wav");
    int vres = openborvariant("vresolution");
	int hres = openborvariant("hresolution");
	void items = glv("versus_item");
	int selector = glv("mselector");
    int time = ov("elapsed_time");
    int game_speed = ov("game_speed");

	draw_blackscreen();

	if ( getlocalvar("flag") == NULL() ) {
		setlocalvar("logo", loadsprite("data/bgs/bg_pattern2.gif"));
		setlocalvar("versus_mark", loadsprite("data/chars/misc/custom_hud/score/versus_pizza.gif"));
		changeopenborvariant("nofadeout", 1);
		playmusic(MUSIC,1);
		setlocalvar("flag", 1);
	} else {
	    drawOffsetShiftBg(getlocalvar("logo"), 128, 128);
	}

	drawstring(hres/2-3-strwidth(text, 0)/2, Y_SCR_TITLE, 6, text);

	for( i = 0; i < size(items); i++) {
		drawstring((hres/2)-strwidth(get(items,i),(selector==i))/2, 190+i*12, (selector==i),get(items,i));
	}

    //sgv("vs_winner",0); // <--- comment
	if ( ggv("vs_winner") != NULL() ) {
	    int winner = ggv("vs_winner");

        int lighting_time = game_speed*0.35;
        int waiting_time = game_speed*0.1;
        int tot_color_changes = 12 - 1;
        int refresh_time = time%(lighting_time + (waiting_time*1.0));
        int black = rgbcolor(0x00,0x00,0x00);
        int white = rgbcolor(0xFF,0xFF,0xFF);
        int color0 = rgbcolor(0x02,0x00,0x5A); // dark
        int color1 = rgbcolor(0x04,0x00,0xA3);
        int color2 = rgbcolor(0x1E,0x3B,0xB9);
        int color3 = rgbcolor(0x2F,0x4E,0xD5);
        int color4 = rgbcolor(0x67,0x82,0xF6);
        int color5 = rgbcolor(0xAF,0xBE,0xFF); // light

        if ( winner == -1 ) text_winner = "DRAW";
        else if ( winner >= 0 ) text_winner = (winner+1)+"  PLAYER  WINS";

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        //(hres/2)-((144/16)*strlength(text_winner))/2
        //(hres/2)-strwidth(text_winner,6)/2

        changedrawmethod(NULL(), "tintmode", 2);
        changedrawmethod(NULL(), "fillcolor", black);
        draw_fontstring((hres/2)-strwidth(text_winner,6)/2,Y_WINNER,2000,0,text_winner,"data/sprites/special_fonts/font7b.gif",144,240,"text_highlight2",0,NULL(),1,NULL(),"data/sprites/font7.gif","txt_map1");

        changedrawmethod(NULL(), "tintmode", 1);
        changedrawmethod(NULL(), "fillcolor", 0);
        refresh_time = (time + 1*waiting_time)%(lighting_time);
             if ( refresh_time <= (lighting_time/tot_color_changes)*0  ) changedrawmethod(NULL(), "fillcolor", color0);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*1  ) changedrawmethod(NULL(), "fillcolor", color1);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*2  ) changedrawmethod(NULL(), "fillcolor", color2);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*3  ) changedrawmethod(NULL(), "fillcolor", color3);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*4  ) changedrawmethod(NULL(), "fillcolor", color4);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*5  ) changedrawmethod(NULL(), "fillcolor", color5);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*6  ) changedrawmethod(NULL(), "fillcolor", white);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*7  ) changedrawmethod(NULL(), "fillcolor", color5);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*8  ) changedrawmethod(NULL(), "fillcolor", color4);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*9  ) changedrawmethod(NULL(), "fillcolor", color3);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*10 ) changedrawmethod(NULL(), "fillcolor", color2);
        else if ( refresh_time <= (lighting_time/tot_color_changes)*11 ) changedrawmethod(NULL(), "fillcolor", color1);
        draw_fontstring((hres/2)-strwidth(text_winner,6)/2,Y_WINNER,2000,0,text_winner,"data/sprites/special_fonts/font7a.gif",144,240,"text_highlight1",0,NULL(),1,NULL(),"data/sprites/font7.gif","txt_map1");

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}

	for( i = 0; i < MAX_PLAYERS; i++) {
		drawstring(X_PLAYERS,Y_PLAYERS+i*LINE_SPACES,6,(i+1)+"  PLAYER  ");
		drawstring(X_PLAYERS+100,Y_PLAYERS+i*LINE_SPACES,6,"WINS  "+ggv("versus_wins_"+i));
		drawsprite(glv("versus_mark"),X_PLAYERS+100+X_MARKS,Y_PLAYERS-4+i*LINE_SPACES,2000,0);
		drawstring(X_PLAYERS+X_MARKS+100+22,Y_PLAYERS+i*LINE_SPACES,6,"*  "+ggv("versus_mark_tot_"+i));
	}


	if ( !get_blackscreen() ) {
        set_blackscreen(1);
        return;
	}

	if ( glv("wait") != NULL() ) {
        if ( time > glv("wait") ) {
            //destroy_vs_summar_vars();
            reset_versus_mode();
            //reset_vars(NULL(),0,1);
            sgv("skip_intro",1);
            playgame(0);
        }
        return;
	}

	if(keyc("moveup")) {
		if(--selector<0) {
			selector = size(items)-1;
		}
		playsample(se2);
	} else if(keyc("movedown")) {
		if(++selector>=size(items)) {
			selector = 0;
		}
		playsample(se2);
	} else if( keyc("start") || keyc("attack") ) {
        playsample(se);

        if ( selector == 0 ) { // GOTO STAGE SELECT
            slv("start","mode");
            slv("mode","stage");
        } else { // EXIT
            int ETA = 0.32;

            if ( glv("wait") == NULL() ) {
                slv("wait",get_next_time(game_speed*ETA));
            }
        }
	} else if( keyc("esc") || keyc("special") ) {
        playsample(se3);
        slv("mselector", size(items)-1);
        return;
	}
	slv("mselector", selector);
}

