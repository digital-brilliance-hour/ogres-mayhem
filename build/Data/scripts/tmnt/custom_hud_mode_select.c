
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/custom_hud_player_select.c"
#import "data/scripts/custom_hud_versus.c"
#import "data/scripts/custom_hud_commons.c"

#define BGLAYERS_NUM get_bglayer_num()

void modeSelect(int select_type, int invert_flag) {
    if ( select_type <= 0 ) chooseMode1();
    else if ( select_type == 1 ) chooseMode1();
    else chooseMode1();
}

void chooseMode1() {
	void ent;
	int i;
	char text  = "MODE SELECT";
	int selector = glv("mode_selector");
	void items = glv("modeitem");
	void prevs = glv("modeprev");
	int se = loadsample("data/sounds/beep2.wav");
	int se2 = loadsample("data/sounds/beep.wav");
	int se3 = loadsample("data/sounds/cancel01.wav");
    int vres = openborvariant("vresolution");
	int hres = openborvariant("hresolution");
	void title_bg = glv("title_bg");

	draw_blackscreen();

	if (!title_bg) slv("title_bg", spawnsubentity("title_bg", 0,0,0));

    drawstring(hres/2-3-strwidth(text, 0)/2, 10, 6, text);
    drawsprite(get(prevs, selector), 80, 32, 2);

	for(i=0; i<size(items); i++) {
		drawstring(160-strwidth(get(items,i),(selector==i))/2, 160+i*12, (selector==i),get(items,i));
	}

	if ( !get_blackscreen() ) {
        set_blackscreen(1);
        return;
	}

	if(key_coin("moveup")) {
		if(--selector<0) {
			selector = size(items)-1;
		}
		playsample(se2);
	} else if(key_coin("movedown")) {
		if(++selector>=size(items)) {
			selector = 0;
		}
		playsample(se2);
	} else if(key_coin("start") || key_coin("attack")) {
	    char mode_type = "";

	    if ( selector == 0 ) mode_type = "stage";
	    else if ( selector == 1 ) {
            mode_type = "1VS1_stage";
            sgv("1VS1",1);
	    }

		from_mode_to_stage_select(mode_type);
	} else if(key_coin("esc") || key_coin("special")) {
	    from_mode_to_main_menu();
	}
	slv("mode_selector", selector);
}

void from_mode_to_main_menu() {
    int se = loadsample("data/sounds/beep2.wav");
    int se2 = loadsample("data/sounds/beep.wav");
    int se3 = loadsample("data/sounds/cancel01.wav");

    playsample(se3);
    setglobalvar("menu_mode","main");
    if (glv("title_bg")) killentity(glv("title_bg"));
    slv("title_bg", NULL());
    clp("background", 0, "enabled", 0);
    change_bglayer(BGLAYERS_NUM,0,1);
    slv("mode_selector", 0);
    slv("mselector", 0);
    set_blackscreen(0);
    slv("start", "menu");
    slv("mode", NULL());
    reset_versus_mode(1);
    slv("delta", 0);
    slv("da_time",NULL());
    reset_lost_frames();
}

void from_mode_to_stage_select(char mode_type) {
    int se = loadsample("data/sounds/beep2.wav");
    int se2 = loadsample("data/sounds/beep.wav");
    int se3 = loadsample("data/sounds/cancel01.wav");
    int i = 0;

    playsample(se);
    setglobalvar("menu_mode","stage");
    slv("delta", 0);
    slv("da_time",NULL());
    reset_lost_frames();
    if (glv("title_bg")) killentity(glv("title_bg"));
    slv("title_bg", NULL());
    clp("background", 0, "enabled", 0);
    change_bglayer(BGLAYERS_NUM,0,1);

    slv("mode_selector", 0);
    slv("mselector", 0);
    set_blackscreen(0);
    slv("start", "mode");
    slv("mode", mode_type);
}

void from_mode_to_player_select() {
    int se = loadsample("data/sounds/beep2.wav");
    int se2 = loadsample("data/sounds/beep.wav");
    int se3 = loadsample("data/sounds/cancel01.wav");
    int i = 0;

    playsample(se);
    setglobalvar("menu_mode","player");
    checkSetupPlayerList(glv("mode_selector"));
    slv("delta", 0);
    slv("da_time",NULL());
    reset_lost_frames();
    if (glv("title_bg")) killentity(glv("title_bg"));
    slv("title_bg", NULL());
    playmusic("data/music/menu.bor", 1);

    for(i=0; i<MAX_PLAYERS; i++) {
        if(keyp_coin(i, "start") || keyp_coin(i, "attack")) slv("player."+i+".join", 1);
    }
    clp("background", 0, "enabled", 0);
    change_bglayer(BGLAYERS_NUM,0,1);

    slv("mode_selector", 0);
    slv("mselector", 0);
    set_blackscreen(0);
    slv("start", "select");
}

