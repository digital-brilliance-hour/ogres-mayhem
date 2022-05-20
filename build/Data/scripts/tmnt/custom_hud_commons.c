
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#define BGLAYERS_NUM 6

int getCharacter(int index) {
    char name = "";

    switch (index) {
        case 0:
            name = "LEO";
            break;
        case 1:
            name = "MIKE";
            break;
        case 2:
            name = "DON";
            break;
        case 3:
            name = "RAPH";
            break;
        case 4:
            name = "FOOT_SOLDIER";
            break;
        case 5:
            name = "ROCK_SOLDIER";
            break;
        case 6:
            name = "TOKKA";
            break;
        case 7:
            name = "RAHZAR";
            break;
        case 8:
            name = "ROCKSTEADY";
            break;
        case 9:
            name = "BEBOP";
            break;
        case 10:
            name = "BAXTER";
            break;
        default:
            name = "";
            break;
    }

    return name;
}

void setupPlayerList(int player_num) {
	int i = 0;
	void list = array(0);
	void flag = array(0);
	//void decision_list = array(0);

	while ( getCharacter(i) != "" ) set(list, i++, getCharacter(i));
	//set(list, i++, "debug_chain_head");

	if ( player_num > 0 ) {
        void tmp_list = array(0);
        for (i = 0; i < player_num; ++i ) {
            set(tmp_list, i, get(list,i));
        }

        free(list); // free old_list
        list = tmp_list;
        //free(tmp_list);
	}

	for( i=size(list)-1; i >= 0; i--) set(flag, i, 0); // FLAG per vedere se il personaggio è stato già scelto. (NOSAME)
    //for ( i = 0; i < MAX_PLAYERS; ++i ) set(decision_list, i, 0); // Servirà a registrare quando un personaggio ha premuto start e ha scelto!

    if ( glv("playerlist") != NULL() ) free(glv("playerlist"));
    if ( glv("playerflag") != NULL() ) free(glv("playerflag"));

	slv("playerlist", list);
	slv("playerflag", flag);
}

void setupModes() {
	int i = 0;
	void items = array(0);
    void prevs = array(0);

	set(items, i++, "ARCADE");
    set(items, i++, "VERSUS");

    for(i=size(items)-1; i >= 0; i--) set(prevs, i, loadsprite("data/sprites/modes/"+(i+1)));
	slv("modeitem", items);
    slv("modeprev", prevs);
}

void setupStages() {
	int i = 0;
	void items = array(0);
    void prevs = array(0);

	set(items, i++, "Beach_Level");
    set(items, i++, "Debug_Room");
	set(items, i++, "Sky_Level_Test");
	set(items, i++, "Water_Level_Test");
    set(items, i++, "River_Level_Test");
    set(items, i++, "Surf_Level_Test");
    set(items, i++, "Bridge_Level_Test");
    set(items, i++, "Snowboard_Level_Test");
    set(items, i++, "Sewer_Level_(TMNT1)_Test");
    set(items, i++, "Deck_Level_Test");
    set(items, i++, "Snow_Park_Level_Test");

    for(i=size(items)-1; i >= 0; i--) set(prevs, i, loadsprite("data/sprites/stages/previews/"+(i+1)));
	slv("stageitem", items);
    slv("stageprev", prevs);

    sgv("num_stages",size(items));
}

void setup_1VS_Stages() {
	int i = 0;
	void items = array(0);
    void prevs = array(0);

	set(items, i++, "Sewer_(Versus)");
	set(items, i++, "Technodrome_(Versus)");

    for(i=size(items)-1; i >= 0; i--) set(prevs, i, loadsprite("data/sprites/stages/1VS1/"+(i+1)));
	slv("1VS1_stageitem", items);
    slv("1VS1_stageprev", prevs);

    sgv("num_versus_stages",size(items));
}

int change_bglayer(int bg_num_tot, int bg_index, int switch_flag) {
    int i = 0;

    if ( switch_flag == NULL() ) switch_flag = 0;
    for ( i = 0; i < bg_num_tot; ++i ) {
        if ( i != bg_index ) {
            if ( glp("bglayer", i, "enabled") != 0 ) clp("bglayer", i, "enabled", 0); // off
        } else {
            if ( glp("bglayer", i, "enabled") != 1 ) clp("bglayer", i, "enabled", switch_flag); // on
        }
    }
}

int get_bglayer_num() {
    return BGLAYERS_NUM;
}

void reset_versus_mode(int flag) {
    int i = 0;

    for (i = 0; i < MAX_PLAYERS; ++i) {
        setglobalvar("versus_mark_"+i,NULL());
        setglobalvar("versus_mark_tot_"+i,NULL());
        setglobalvar("versus_wins_"+i,NULL());
    }
    setglobalvar("1VS1",NULL());
    setglobalvar("vs_winner",NULL());
    setglobalvar("versus_time",NULL());

    if ( flag != NULL() && flag == 1 ) {
        changeopenborvariant("nojoin", 0);
    } else if ( flag != NULL() && flag > 1 ) {
        changeopenborvariant("nojoin", 0);
        changeopenborvariant("nopause", 0);
    }
}

