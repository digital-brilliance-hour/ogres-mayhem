
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_data.c"
#import "data/scripts/custom_hud_commons.c"
#import "data/scripts/lib_drawing.c"
//#import "data/scripts/var_resetter.c"

#define BGLAYERS_NUM get_bglayer_num()
#define SELECT_MUSIC "data/music/menu_.bor"
#define TITLE_MUSIC "data/music/remix.bor"
#define HISCORE_MUSIC "data/music/score.bor"
#define ICON_NUM 4
#define HISCORE_NUM 10
#define HIGHLIGHTED_TOPLINES_NUM 3
#define HIGHLIGHTED_LINES_NUM 10 // +1 == all lines highlighted +1 a vuoto
#define HIGHLIGHTING_TIME 1.2
#define HISCORE_TYPES 3 // score/character_name/player_name
#define HISCORE_FILENAME "hiscore.dat"
#define HISCORE_PATHNAME "saves/"+ov("pakname")+"/"
#define X_SCORE 20
#define Y_SCORE 53
#define OVERLAPPED_HISCORE_BG "data/bgs/hiscore_cus.gif"

void overlap_hiscore_scr() { // for update.c / updated.c
    if ( ov("in_halloffamescreen") ) {
        int max_int = oc("MAX_INT");

        max_int -= 999999;

        //clearspriteq(ov("vscreen"));
        //clearscreen(ov("vscreen"));

        if ( glv("hiscore_bg_overlapped") == NULL() ) {
            //int vres = openborvariant("vresolution");
            //int hres = openborvariant("hresolution");
            //slv("hiscore_screen",allocscreen(hres,vres)); /// --- TEST ---

            clearspriteq(); // CLEAR ALL PREV SPRITES
            slv("hiscore_bg_overlapped",loadsprite(OVERLAPPED_HISCORE_BG));
        }

        //if ( glv("hiscore_screen") != NULL() ) drawscreen(glv("hiscore_screen"),0,0,max_int);  /// --- TEST --- // 999999999
        //if ( glv("hiscore_bg_overlapped") != NULL() ) drawspritetoscreen(glv("hiscore_screen"),glv("hiscore_bg_overlapped"), 0, 0);  /// --- TEST ---

        if ( glv("hiscore_bg_overlapped") != NULL() ) drawsprite(glv("hiscore_bg_overlapped"), 0, 0, max_int, 1);

        //drawstringtoscreen(glv("hiscore_screen"), 20, 20, 6, "TEST");
        if ( glv("game_speed") != NULL() ) show_hiscores(glv("game_speed"),1,max_int,1,NULL());
    } else {
        if ( glv("game_speed") == NULL() ) slv("game_speed",ov("game_speed"));
        if ( glv("hiscore_bg_overlapped") != NULL() ) {
            destroy_overlap_hiscore_scr();
            //reset_vars(NULL(),0,1);
        }
    }
    //drawstring(20, 20, 6, "TEST");

}

// called by updated.c, title_script.c
void destroy_overlap_hiscore_scr() {
    int i = 0;

    slv("select_music",NULL());
    if ( glv("hiscore_screen") != NULL() ) { free(glv("hiscore_screen")); slv("hiscore_screen",NULL()); }
    if ( glv("hiscore_bg_overlapped") != NULL() ) { free(glv("hiscore_bg_overlapped")); slv("hiscore_bg_overlapped",NULL()); }
    if ( glv("font_hiscore_text1") != NULL() ) { free(glv("font_hiscore_text1")); slv("font_hiscore_text1",NULL()); }
    if ( glv("font_hiscore_text2") != NULL() ) { free(glv("font_hiscore_text2")); slv("font_hiscore_text2",NULL()); }
    if ( glv("font_hiscore_text3") != NULL() ) { free(glv("font_hiscore_text3")); slv("font_hiscore_text3",NULL()); }
    if ( glv("hiscore_logo") != NULL() ) { free(glv("hiscore_logo")); slv("hiscore_logo",NULL()); }
    if ( glv("hiscore_score_list") != NULL() ) { free(glv("hiscore_score_list")); slv("hiscore_score_list",NULL()); }
    if ( glv("hiscore_name_list") != NULL() ) { free(glv("hiscore_name_list")); slv("hiscore_name_list",NULL()); }
    if ( glv("hiscore_player_list") != NULL() ) { free(glv("hiscore_player_list")); slv("hiscore_player_list",NULL()); }
	if ( glv("bor_hiscore_name_list") ) { free(glv("bor_hiscore_name_list")); slv("bor_hiscore_name_list",NULL()); }
	if ( glv("bor_hiscore_score_list") ) { free(glv("bor_hiscore_score_list")); slv("bor_hiscore_score_list",NULL()); }
    if ( glv("font_hiscore1") != NULL() ) { free(glv("font_hiscore1")); slv("font_hiscore1",NULL()); }
    if ( glv("font_hiscore2") != NULL() ) { free(glv("font_hiscore2")); slv("font_hiscore2",NULL()); }
    if ( glv("hs_color_cyan") != NULL() ) { free(glv("hs_color_cyan")); slv("hs_color_cyan",NULL()); }
    if ( glv("hs_color_yellow") != NULL() ) { free(glv("hs_color_yellow")); slv("hs_color_yellow",NULL()); }
    if ( glv("hs_color_orange") != NULL() ) { free(glv("hs_color_orange")); slv("hs_color_orange",NULL()); }
    if ( glv("hs_color_red") != NULL() ) { free(glv("hs_color_red")); slv("hs_color_red",NULL()); }
    for (i = 1; i <= ICON_NUM; ++i) if ( glv("enter_name_spr"+i) != NULL() ) { free(glv("enter_name_spr"+i)); slv("enter_name_spr"+i,NULL()); }
}

void show_hiscores(int saved_game_speed, int nokey_flag, int forced_z, int overlap_flag, void screen) {
    char text1  = "HI - SCORES";
    char text2  = "best  "+HISCORE_NUM+"  players";
    char hiscore_name_list = glv("hiscore_name_list");
    char hiscore_player_list = glv("hiscore_player_list");
    int hiscore_score_list = glv("hiscore_score_list");
    int vres = openborvariant("vresolution");
    int hres = openborvariant("hresolution");
    int time = ov("elapsed_time");
    int game_speed;
    int i = 0;
    int base_x = X_SCORE, base_y = Y_SCORE;
    int STR_SORTID = 20;

    if ( saved_game_speed == NULL() ) game_speed = ov("game_speed");
    else game_speed = saved_game_speed;
    if ( forced_z == NULL() ) forced_z = 0;

	if ( getlocalvar("hiscore_logo") == NULL() ) {
        //update_hiscores(gpp(0,"ent"),"TEST",28);
        get_hiscores();
		setlocalvar("hiscore_logo", loadsprite("data/bgs/hiscore_logo.gif"));
		/*setlocalvar("enter_name_spr1", loadsprite("data/bgs/hi_raph2.gif"));
		setlocalvar("enter_name_spr2", loadsprite("data/bgs/hi_don2.gif"));
		setlocalvar("enter_name_spr3", loadsprite("data/bgs/hi_mike2.gif"));
		setlocalvar("enter_name_spr4", loadsprite("data/bgs/hi_leo2.gif"));*/
	}
    drawOrizShiftBg(glv("hiscore_logo"),338,240,0,0,forced_z,STR_SORTID-10,screen);
    //drawVericalScaledRouletteBg(glv("enter_name_spr1"),glv("enter_name_spr2"),glv("enter_name_spr3"),glv("enter_name_spr4"),80,132,1.60,1.80,0);
    //drawVericalScaledRouletteBg(glv("enter_name_spr1"),glv("enter_name_spr2"),glv("enter_name_spr3"),glv("enter_name_spr4"),96,128,1.60,1.80,0);
    //drawOrizScaledRouletteBg(glv("enter_name_spr1"),glv("enter_name_spr2"),glv("enter_name_spr3"),glv("enter_name_spr4"),96,128,1.60,1.80,0);
    //drawOrizontalRouletteBg(glv("enter_name_spr1"),glv("enter_name_spr2"),glv("enter_name_spr3"),glv("enter_name_spr4"),96,128,1.60,1.80,0);

    if ( glv("select_music") == NULL() ) {
        playmusic(HISCORE_MUSIC, 1);
        slv("select_music",1);
    }

    if ( overlap_flag == NULL() ) drawstring(hres/2-3-strwidth(text1, 0)/2, 10-2, 6, text1, forced_z); else draw_fontstring(hres/2-3-strwidth(text1, 0)/2,10-2,forced_z,STR_SORTID,text1,"data/sprites/font7.gif",144,240,"font_hiscore_text1",0,NULL(),1);
    if ( overlap_flag == NULL() ) drawstring(hres/2-3-strwidth(text2, 0)/2, 20+5, 0, text2, forced_z); else draw_fontstring(hres/2-3-strwidth(text2, 0)/2,20+5,forced_z,STR_SORTID,text2,"data/sprites/font.gif",128,128,"font_hiscore_text2",0,NULL(),1);
    //if ( overlap_flag == NULL() ) drawstring(base_x, base_y-12, 0, "");
    if ( overlap_flag == NULL() ) drawstring(base_x+20, base_y-12, 0, "CHARACTER", forced_z); else draw_fontstring(base_x+20,base_y-12,forced_z,STR_SORTID,"CHARACTER","data/sprites/font.gif",128,128,"font_hiscore_text2",0,NULL(),1);
    if ( overlap_flag == NULL() ) drawstring(base_x+140, base_y-12, 0, "SCORE", forced_z); else draw_fontstring(base_x+140,base_y-12,forced_z,STR_SORTID,"SCORE","data/sprites/font.gif",128,128,"font_hiscore_text2",0,NULL(),1);
    if ( overlap_flag == NULL() ) drawstring(base_x+200, base_y-12, 0, "PLAYER", forced_z); else draw_fontstring(base_x+200,base_y-12,forced_z,STR_SORTID,"PLAYER","data/sprites/font.gif",128,128,"font_hiscore_text2",0,NULL(),1);

    // colors
    int black = rgbcolor(0x00,0x00,0x00);
    int white = rgbcolor(0xFF,0xFF,0xFF);

    // cyan (basic)
    if ( glv("hs_color_cyan") == NULL() ) {
        void color = array(0);
        int ci = 0;

        set(color, ci++, rgbcolor(0x0A,0xA4,0xF4)); // dark
        set(color, ci++, rgbcolor(0x15,0xAE,0xFD));
        set(color, ci++, rgbcolor(0x2E,0xB8,0xFF));
        set(color, ci++, rgbcolor(0x40,0xBD,0xFE));
        set(color, ci++, rgbcolor(0x56,0xC5,0xFF));
        set(color, ci++, rgbcolor(0x66,0xCA,0xFE));
        set(color, ci++, rgbcolor(0x71,0xCD,0xFD));
        set(color, ci++, rgbcolor(0x81,0xD3,0xFD));
        set(color, ci++, rgbcolor(0x95,0xDA,0xFE));
        set(color, ci++, rgbcolor(0xAD,0xE3,0xFF)); // more white
        set(color, ci++, rgbcolor(0xC5,0xEB,0xFF));
        set(color, ci++, rgbcolor(0xE1,0xF5,0xFF)); // light

        slv("hs_color_cyan", color);
    }

    // orange
    if ( glv("hs_color_orange") == NULL() ) {
        void color = array(0);
        int ci = 0;

        set(color, ci++, rgbcolor(0xF8,0x46,0x08)); // dark
        set(color, ci++, rgbcolor(0xFF,0x5A,0x00));
        set(color, ci++, rgbcolor(0xFF,0x67,0x0C));
        set(color, ci++, rgbcolor(0xFF,0x70,0x19));
        set(color, ci++, rgbcolor(0xFF,0x7E,0x30));
        set(color, ci++, rgbcolor(0xFF,0x84,0x3A));
        set(color, ci++, rgbcolor(0xFF,0x8A,0x44));
        set(color, ci++, rgbcolor(0xFF,0x91,0x4F));
        set(color, ci++, rgbcolor(0xFE,0x96,0x58));
        set(color, ci++, rgbcolor(0xFF,0x9E,0x63)); // more white
        set(color, ci++, rgbcolor(0xFF,0xC6,0xA4));
        set(color, ci++, rgbcolor(0xFF,0xD5,0xBC)); // light

        slv("hs_color_orange", color);
    }

    // yellow
    if ( glv("hs_color_yellow") == NULL() ) {
        void color = array(0);
        int ci = 0;

        set(color, ci++, rgbcolor(0xFF,0xA2,0x00)); // dark
        set(color, ci++, rgbcolor(0xFF,0xAE,0x00));
        set(color, ci++, rgbcolor(0xFF,0xB4,0x00));
        set(color, ci++, rgbcolor(0xFF,0xC6,0x00));
        set(color, ci++, rgbcolor(0xFF,0xE4,0x00));
        set(color, ci++, rgbcolor(0xFE,0xE4,0x07));
        set(color, ci++, rgbcolor(0xFF,0xE6,0x11));
        set(color, ci++, rgbcolor(0xFF,0xE7,0x1B));
        set(color, ci++, rgbcolor(0xFF,0xE8,0x24));
        set(color, ci++, rgbcolor(0xFF,0xEC,0x4C)); // more white
        set(color, ci++, rgbcolor(0xFF,0xEF,0x65));
        set(color, ci++, rgbcolor(0xFF,0xF2,0x84)); // light

        slv("hs_color_yellow", color);
    }

    // red
    if ( glv("hs_color_red") == NULL() ) {
        void color = array(0);
        int ci = 0;

        set(color, ci++, rgbcolor(0xFF,0x66,0x00)); // dark
        set(color, ci++, rgbcolor(0xFF,0x4e,0x00));
        set(color, ci++, rgbcolor(0xFF,0x48,0x00));
        set(color, ci++, rgbcolor(0xFF,0x36,0x00));
        set(color, ci++, rgbcolor(0xFF,0x00,0x00));
        set(color, ci++, rgbcolor(0xFF,0x1D,0x1D));
        set(color, ci++, rgbcolor(0xFF,0x35,0x35));
        set(color, ci++, rgbcolor(0xFF,0x4B,0x4B));
        set(color, ci++, rgbcolor(0xFF,0x60,0x60));
        set(color, ci++, rgbcolor(0xFF,0x94,0x94)); // more white
        set(color, ci++, rgbcolor(0xFF,0xA5,0xA5));
        set(color, ci++, rgbcolor(0xFF,0xB5,0xB5)); // light

        slv("hs_color_red", color);
    }

    // SHOW SCORE LINES
    if ( hiscore_score_list != NULL() ) {
        for (i = 0; i < size(hiscore_score_list); ++i) {
            int font = 0;
            int lifespan = (game_speed*HIGHLIGHTING_TIME);
            int time_range = lifespan/(HIGHLIGHTED_LINES_NUM+1)-HIGHLIGHTED_TOPLINES_NUM;

            /*
            if ( time%lifespan >= i*time_range && time%lifespan < (i+1)*time_range && i < (HIGHLIGHTED_LINES_NUM+1)-HIGHLIGHTED_TOPLINES_NUM ) font = 1;
            //if ( time%lifespan < time_range*(i+1) && i < (HIGHLIGHTED_LINES_NUM+1)-HIGHLIGHTED_TOPLINES_NUM ) font = 1;
            //if ( time%lifespan >= i*time_range && time%lifespan < (i+3)*time_range && i < (HIGHLIGHTED_LINES_NUM+1)-HIGHLIGHTED_TOPLINES_NUM ) font = 1;
            //if ( (time%lifespan <= lifespan/2 && i%2 == 0) || (time%lifespan > lifespan/2 && i%2 == 1) && i < (HIGHLIGHTED_LINES_NUM+1)-HIGHLIGHTED_TOPLINES_NUM ) font = 1;

            if ( overlap_flag == NULL() ) drawstring(base_x,base_y+1+14*i,font,(i+1)+".",forced_z); else draw_fontstring(base_x,base_y+1+14*i,forced_z,20,(i+1)+".","data/sprites/font.gif",128,128,"font_hiscore_text2",0);
            if ( overlap_flag == NULL() ) drawstring(base_x+20,base_y+1+14*i,font,get(hiscore_name_list,i),forced_z); else draw_fontstring(base_x+20,base_y+1+14*i,forced_z,20,get(hiscore_name_list,i),"data/sprites/font.gif",128,128,"font_hiscore_text2",0);
            if ( overlap_flag == NULL() ) drawstring(base_x+140,base_y+1+14*i,font,get(hiscore_score_list,i),forced_z); else draw_fontstring(base_x+140,base_y+1+14*i,forced_z,20,get(hiscore_score_list,i),"data/sprites/font.gif",128,128,"font_hiscore_text2",0);
            if ( overlap_flag == NULL() ) drawstring(base_x+200,base_y+1+14*i,font,get(hiscore_player_list,i),forced_z); else draw_fontstring(base_x+200,base_y+1+14*i,forced_z,20,get(hiscore_player_list,i),"data/sprites/font.gif",128,128,"font_hiscore_text2",0);
            */

            int lighting_time = game_speed*1.5;
            int waiting_time = game_speed*0.1;
            int tot_color_changes = 24 - 1; // -1 to not count the color zero

            int top_color_changes = 24 - 1; // no white color
            int top_lighting_time = game_speed*1.5;
            int top_waiting_time = game_speed*0.1;

            changedrawmethod(NULL(), "reset", 1);
            changedrawmethod(NULL(), "enabled", 1);

            if ( i >= HIGHLIGHTED_TOPLINES_NUM ) {
                draw_fontstring(base_x,base_y+1+14*i,forced_z,STR_SORTID,(i+1)+".","data/sprites/special_fonts/font.gif",128,128,"font_hiscore2",0);
                draw_fontstring(base_x+20,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_name_list,i),"data/sprites/special_fonts/font0b.gif",128,128,"font_hiscore2",0);
                draw_fontstring(base_x+140,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_score_list,i),"data/sprites/special_fonts/font0b.gif",128,128,"font_hiscore2",0);
                draw_fontstring(base_x+200,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_player_list,i),"data/sprites/special_fonts/font0b.gif",128,128,"font_hiscore2",0);
            } else {
                changedrawmethod(NULL(), "tintmode", 2);
                changedrawmethod(NULL(), "fillcolor", black);

                draw_fontstring(base_x,base_y+1+14*i,forced_z,STR_SORTID,(i+1)+".","data/sprites/special_fonts/font0b.gif",128,128,"font_hiscore2",0);
                draw_fontstring(base_x+20,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_name_list,i),"data/sprites/special_fonts/font0b.gif",128,128,"font_hiscore2",0);
                draw_fontstring(base_x+140,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_score_list,i),"data/sprites/special_fonts/font0b.gif",128,128,"font_hiscore2",0);
                draw_fontstring(base_x+200,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_player_list,i),"data/sprites/special_fonts/font0b.gif",128,128,"font_hiscore2",0);
            }

            changedrawmethod(NULL(), "tintmode", 1);
            changedrawmethod(NULL(), "fillcolor", 0);

            if ( i >= HIGHLIGHTED_TOPLINES_NUM ) {
                int refresh_time; // = time%(lighting_time + (waiting_time*size(hiscore_score_list)));
                int ci = 0;
                void color = glv("hs_color_cyan");

                /*     if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*0  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", white);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*1  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color11);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*2  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color10);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*3  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color9);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*4  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color8);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*5  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color7);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*6  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color6);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*7  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color5);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*8  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color4);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*9  && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color3);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*10 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color2);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*11 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color1);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*12 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color0);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*13 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color1);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*14 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color2);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*15 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color3);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*16 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color4);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*17 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color5);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*18 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color6);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*19 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color7);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*20 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color8);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*21 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color9);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*22 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color10);
                else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*23 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color11);*/
                //else if ( refresh_time <= i*waiting_time + (lighting_time/tot_color_changes)*24 && refresh_time >= i*waiting_time && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", white);
                //else changedrawmethod(NULL(), "fillcolor", color0);

                refresh_time = (time + (size(hiscore_score_list)-i)*waiting_time)%(lighting_time); // instead size(hiscore_score_list)-i use just i to revert the lighting order
                     if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", white);
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,11));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,10));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,9));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,8));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,7));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,6));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,5));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,4));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,3));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,2));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,1));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,0));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,1));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,2));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,3));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,4));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,5));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,6));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,7));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,8));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,9));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,10));
                else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,11));
                //else if ( refresh_time <= (lighting_time/tot_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", white);
            } else {
                int refresh_time; // = time%(top_lighting_time + (top_waiting_time*size(hiscore_score_list)));
                int ci = 0;
                void color;

                if (i == 0) color = glv("hs_color_yellow");
                else if (i == 1) color = glv("hs_color_orange");
                else color = glv("hs_color_red");

                refresh_time = (time + (size(hiscore_score_list)-i)*top_waiting_time)%(top_lighting_time); // instead size(hiscore_score_list)-i use just i to revert the lighting order
                     if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,0));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,1));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,2));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,3));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,4));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,5));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,6));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,7));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,8));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,9));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,10));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,11));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", white);
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,11));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,10));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,9));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,8));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,7));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,6));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,5));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,4));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,3));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,2));
                else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", get(color,1));
                //else if ( refresh_time <= (top_lighting_time/top_color_changes)*(ci++) && i < (HIGHLIGHTED_LINES_NUM+1) ) changedrawmethod(NULL(), "fillcolor", color_dark0);
            }

            draw_fontstring(base_x,base_y+1+14*i,forced_z,STR_SORTID,(i+1)+".","data/sprites/special_fonts/font0a.gif",128,128,"font_hiscore1",0);
            draw_fontstring(base_x+20,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_name_list,i),"data/sprites/special_fonts/font0a.gif",128,128,"font_hiscore1",0);
            draw_fontstring(base_x+140,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_score_list,i),"data/sprites/special_fonts/font0a.gif",128,128,"font_hiscore1",0);
            draw_fontstring(base_x+200,base_y+1+14*i,forced_z,STR_SORTID,get(hiscore_player_list,i),"data/sprites/special_fonts/font0a.gif",128,128,"font_hiscore1",0); // ,NULL(),1

            changedrawmethod(NULL(), "reset", 1);
            changedrawmethod(NULL(), "enabled", 0);
        }
    }

	if( (nokey_flag == NULL() || nokey_flag == 0) && (keyc("attack") || keyc("start") || keyc("esc") || keyc("special") || keyc("jump")) ) {
	    //playsample(se);
	    //slv("select_music",NULL());
	    destroy_overlap_hiscore_scr();
        playmusic(TITLE_MUSIC, 1);
		clp("background", 0, "enabled", 0);
		change_bglayer(BGLAYERS_NUM,0,1);
		//slv("mselector", 0);
		slv("start", "menu");
	}
}

void bor_hiscore_to_custom_hiscore() {
    int i = 0;
    char hiscore_player_list = array(0);

    //get_hiscores();
    get_bor_hiscores();

    slv("hiscore_name_list",glv("bor_hiscore_name_list")); slv("bor_hiscore_name_list",NULL());
    slv("hiscore_score_list",glv("bor_hiscore_score_list")); slv("bor_hiscore_score_list",NULL());

    for ( i = 0; i < size(glv("bor_hiscore_score_list")); ++i ) {
        set(hiscore_player_list,i++,"NONE");
    }
    slv("hiscore_player_list",hiscore_player_list);
    write_hiscores();
    if( glv("hiscore_score_list") ) { free(glv("hiscore_score_list")); slv("hiscore_score_list",NULL()); }
    if( glv("hiscore_name_list") ) { free(glv("hiscore_name_list")); slv("hiscore_name_list",NULL()); }
    if( glv("hiscore_player_list") ) { free(glv("hiscore_player_list")); slv("hiscore_player_list",NULL()); }
}

void update_hiscores(void player, char player_name, int forced_score) {
    //if ( gep(player,"exists") ) {
        void filestream = openfilestream(HISCORE_PATHNAME+HISCORE_FILENAME,0);
        int p = gep(player, "playerindex");
        int score = gpp(p,"score");
        //char defaultname = gpp(p,"name");
        char defaultname = gep(player,"defaultname");
        int i = 0;

        //drawstring(30,20,0,filestream);
        //drawstring(30,20,0,ov("pakname"));
        if ( filestream == -1 ) {
            void savefile = createfilestream();

            if ( forced_score != NULL() ) score = forced_score;
            if ( score == NULL() || score < 0 || typeof(score) == oc("VT_EMPTY") ) score = 0;
            if ( defaultname == NULL() || typeof(defaultname) == oc("VT_EMPTY") ) defaultname = "NONE";
            if ( player_name == NULL() || typeof(player_name) == oc("VT_EMPTY") ) defaultname = "NONE";
            filestreamappend(savefile,score,1); // senza \n
            filestreamappend(savefile,defaultname,0); // con \n
            filestreamappend(savefile,player_name,0);

            for ( i = 0; i < (HISCORE_NUM-1)*HISCORE_TYPES; ++i ) {
                if ( i%HISCORE_TYPES == 0 ) filestreamappend(savefile,0,0);
                else if ( i%HISCORE_TYPES == 1 ) filestreamappend(savefile,"NONE",0);
                else filestreamappend(savefile,"NONE",0);
            }

            savefilestream(savefile,HISCORE_FILENAME);
            closefilestream(savefile);
        } else {
            int size = 0;

            closefilestream(filestream);
            get_hiscores();

            // add new score/name
            if ( forced_score != NULL() ) score = forced_score;
            if ( score == NULL() || score < 0 || typeof(score) == oc("VT_EMPTY") ) score = 0;
            if ( defaultname == NULL() || typeof(defaultname) == oc("VT_EMPTY") ) defaultname = "NONE";
            if ( player_name == NULL() || typeof(player_name) == oc("VT_EMPTY") ) defaultname = "NONE";

            if ( glv("hiscore_score_list") != NULL() ) size = size(glv("hiscore_score_list"));
            else size = 0;
            /// ############################ AGGIUNGI NOME/SCORE ############################
            set( glv("hiscore_score_list"), size, score);
            set( glv("hiscore_name_list"), size, defaultname);
            set( glv("hiscore_player_list"), size++, player_name);

            write_hiscores();
        }
        if( glv("hiscore_score_list") ) { free(glv("hiscore_score_list")); slv("hiscore_score_list",NULL()); }
        if( glv("hiscore_name_list") ) { free(glv("hiscore_name_list")); slv("hiscore_name_list",NULL()); }
        if( glv("hiscore_player_list") ) { free(glv("hiscore_player_list")); slv("hiscore_player_list",NULL()); }
    //} // fine if player exists
}

void write_hiscores() {
    void savefile = createfilestream();
    char hiscore_name_list = glv("hiscore_name_list");
    char hiscore_player_list = glv("hiscore_player_list");
    int hiscore_score_list = glv("hiscore_score_list");
    int i = 0, j = 0, c_score = 0, c_name = 0, c_player = 0;

    // quicksort
    quickSortIterative(hiscore_score_list, hiscore_name_list, hiscore_player_list, 0, size(hiscore_score_list)-1,1);

    for ( i = 0; i < HISCORE_NUM*HISCORE_TYPES; ++i ) {
        if ( i%HISCORE_TYPES == 0 ) {
            if ( i == 0 ) filestreamappend(savefile,get(hiscore_score_list,c_score++),1);
            else filestreamappend(savefile,get(hiscore_score_list,c_score++),0);
        } else if ( i%HISCORE_TYPES == 1 ) {
            filestreamappend(savefile,get(hiscore_name_list,c_name++),0);
        } else {
            filestreamappend(savefile,get(hiscore_player_list,c_player++),0);
        }
    }
    savefilestream(savefile,HISCORE_FILENAME);
    closefilestream(savefile);
}

void get_hiscores() {
    void filestream = openfilestream(HISCORE_PATHNAME+HISCORE_FILENAME,0);
    char hiscore_name_list = array(0);
    char hiscore_player_list = array(0);
    int hiscore_score_list = array(0);
    char line = "";
    int integer = 0;
    int i = 0, c = 0;
    int nextline = 0;
    int num = 0;

    //drawstring(30,20,0,setfilestreamposition(0, 9999);
    if ( filestream != -1 ) {
        setfilestreamposition(0, 0);

        //line = getfilestreamline(0);
        //drawstring(30,20,0,line);
        //return;

        // load scores/names
        for ( i = 0; i < HISCORE_NUM*HISCORE_TYPES; ++i ) {
            line = getfilestreamline(0);
            if ( i%HISCORE_TYPES == 0 ) num = getfilestreamargument(0, 0, "int");
            //if ( line == -1 ) break;

            nextline = strlength(line)+2;
            line = substr(line,0,strlength(line)-1); // exclude last char
            if ( i%HISCORE_TYPES == 0 ) set(hiscore_score_list, c, num);
            else if ( i%HISCORE_TYPES == 1 ) set(hiscore_name_list, c, line);
            else set(hiscore_player_list, c++, line);
            setfilestreamposition(0, getfilestreamposition(0)+nextline);
        }

        slv("hiscore_name_list", hiscore_name_list);
        slv("hiscore_player_list", hiscore_player_list);
        slv("hiscore_score_list", hiscore_score_list);

        closefilestream(filestream);
    } // fine if open_OK
    else {
        void savefile = createfilestream();

        filestreamappend(savefile,0,1); set(hiscore_score_list, c, 0); // senza \n
        filestreamappend(savefile,"NONE",0); set(hiscore_name_list, c, "NONE"); // con \n
        filestreamappend(savefile,"NONE",0); set(hiscore_player_list, c++, "NONE"); // con \n

        for ( i = 0; i < (HISCORE_NUM-1)*HISCORE_TYPES; ++i ) {
            if ( i%HISCORE_TYPES == 0 ) { filestreamappend(savefile,0,0); set(hiscore_score_list, c, 0); }
            else if ( i%HISCORE_TYPES == 1 ) { filestreamappend(savefile,"NONE",0); set(hiscore_name_list, c, "NONE"); }
            else { filestreamappend(savefile,"NONE",0); set(hiscore_player_list, c++, "NONE"); }
        }

        savefilestream(savefile,HISCORE_FILENAME);
        closefilestream(savefile);

        slv("hiscore_name_list", hiscore_name_list);
        slv("hiscore_player_list", hiscore_player_list);
        slv("hiscore_score_list", hiscore_score_list);
    }
}

void get_bor_hiscores() {
    void filestream = openfilestream("saves/bor.hi",0); // saves/bor.hi //data/temp/moves.docx --> oppure openfilestream("saves/bor.hi",0);
    char hiscore_name_list = array(0);
    int hiscore_score_list = array(0);
    char line = "";
    int integer = 0;
    int i = 0;
    int start_name_offset = 0x2C;
    int next_name_line = 0x30; // num bytes fra un nome e l'altro
    int start_score_offset = 0x04;
    int next_score_line = 0x04; // num bytes fra uno score e l'altro
    int max_score_length = 3; // intero di 3 bytes
    int num_names = 10;

    if ( filestream >= 0 ) {
        int c = 0;
        // load scores
        setfilestreamposition(0, start_score_offset);
        for (i = 0; i < num_names; ++i) {
            line = "";
            //line = getfilestreamline(0);
            // Inserire un for che legge max_score_length bytes
            for ( c = 0; c < max_score_length; ++c ) {
                char chara = getfilestreamline(0);

                if ( chara == "" ) line += " "; // byte 0x00
                else line += getchar(chara,0);
                setfilestreamposition(0, getfilestreamposition(0)+1);
            }
            if ( line == "" ) line = " "; // all bytes 0x00

            set(hiscore_score_list, i, line);
            start_score_offset += next_score_line;
            setfilestreamposition(0, start_score_offset);
        }

        // load names
        setfilestreamposition(0, start_name_offset);
        for (i = 0; i < num_names; ++i) {
            line = getfilestreamline(0);
            //line = getfilestreamargument(0, 0, "string");

            set(hiscore_name_list, i, line);
            //start_name_offset += next_name_line;
            setfilestreamposition(0, getfilestreamposition(0)+next_name_line);
        }

        slv("bor_hiscore_name_list", hiscore_name_list);
        slv("bor_hiscore_score_list", hiscore_score_list);

        closefilestream(filestream);

        for ( i = 0; i < size(hiscore_score_list); ++i ) {
            char string;
            int num = 0, c = 0;
            int score_size;

            string = get(hiscore_score_list,i);

            score_size = strlength(string);
            if ( score_size > max_score_length ) score_size = max_score_length;
            for ( c = 0; c < score_size; ++c ) {
                char chara = getchar(string,c);
                //int reverted_index = strlength(string)-1 - c;
                int digit;

                digit = byte_to_int(chara);
                if ( c > 0 ) {
                    int t = 0;
                    for ( t = 1; t <= c; ++t ) {
                        digit *= 256; // or digit = digit << 8;
                    }
                }
                if ( c == 0 ) num = digit;
                else num = num | digit;
            }

            set(hiscore_score_list,i,num);
        }
    } // fine if open_OK
}

void swap(void list1, void list2, void list3, int i, int j) {
    int t;
    char t_str2, t_str3;

    t = get(list1,i); t_str2 = get(list2,i); t_str3 = get(list3,i);
    set(list1,i,get(list1,j)); set(list2,i,get(list2,j)); set(list3,i,get(list3,j));
    set(list1,j,t); set(list2,j,t_str2); set(list3,j,t_str3);
}

int partition(void list1, void list2, void list3, int l, int r, int order_flag) {
    int x = get(list1,r);
    int i = (l - 1), j;

    for (j = l; j <= r-1; j++) {
        if ( !order_flag ) {
            if (get(list1,j) <= x) {
                i++;
                swap(list1,list2,list3,i,j);
            }
        } else {
            if (get(list1,j) > x) {
                i++;
                swap(list1,list2,list3,i,j);
            }
        }
    }
    swap(list1,list2,list3,i+1,r);

    return (i+1);
}

void quickSortIterative(void list1, void list2, void list3, int l, int r, int order_flag) {
    // Create an auxiliary stack
    int stack = array(r-l+1);

    // initialize top of stack
    int top = -1;

    // push initial values of l and h to stack
    set(stack, ++top, l);
    set(stack, ++top, r);

    if ( order_flag == NULL() ) order_flag = 0;

    // Keep popping from stack while is not empty
    while ( top >= 0 ) {
        // Pop h and l
        r = get(stack, top--);
        l = get(stack, top--);

        // Set pivot element at its correct position in sorted array
        int p = partition(list1, list2, list3, l, r, order_flag);

        // If there are elements on left side of pivot, then push left
        // side to stack
        if ( p-1 > l ) {
            set(stack, ++top, l);
            set(stack, ++top, p-1);
        }

        // If there are elements on right side of pivot, then push right
        // side to stack
        if ( p+1 < r ) {
            set(stack, ++top, p+1);
            set(stack, ++top, r);
        }
    }

    free(stack);
}

/*int sort(void list1, void list2) {
    for (i = 0; i < size(list1); ++i ) {
        for (j = 0; j < size(list1); ++j ) {
            if ( i != j ) {
                if ( get(list1,i) > get(list1,j) ) {
                    swap(list1,list2,list3,i,j);
                }
            }
        }
    }
}*/

/*void quicksort(void list1, void list2, void list3, int l, int r, int order_flag) {
   int j;

   if ( order_flag == NULL() ) order_flag = 0;
   if( l < r )  {
       // divide and conquer
       j = partition(list1, list2, list3, l, r, order_flag);
       quicksort(list1, list2, list3, l, j-1, order_flag);
       quicksort(list1, list2, list3, j+1, r, order_flag);
   }
}

int partition(void list1, void list2, void list3, int l, int r, int order_flag) {
   int pivot, i, j, t;

   pivot = get(list1,l);
   i = l; j = r+1;

   if ( order_flag == NULL() ) order_flag = 0;
   while(1) {
        if ( !order_flag ) { // asc
            do ++i; while (get(list1,i) <= pivot && i <= r );
            do --j; while (get(list1,j) > pivot && j > l );
        } else { // desc
            do ++i; while (get(list1,i) > pivot && i <= r );
            do --j; while (get(list1,j) <= pivot && j > l );
        }

        if( i >= j ) break;

        swap(list1,list2,list3,i,j);
   }
   swap(list1,list2,list3,l,j);

   return j;
}*/
