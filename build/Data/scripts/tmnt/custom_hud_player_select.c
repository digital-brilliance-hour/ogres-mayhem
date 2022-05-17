
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/custom_hud_commons.c"

#define BGLAYERS_NUM get_bglayer_num()
#define SELECT_MUSIC "data/music/menu_.bor"
#define SELECT_VERSUS_MUSIC "data/music/menu01.bor"
#define TITLE_MUSIC "data/music/remix.bor"
#define VERSUS_MUSIC "data/music/score01.bor"

#define PRESS_START_DISPLAY 1
#define INSERT_COINS_ALT 1
#define CHAR_NAME_DISPLAY 1

#define Y_TITLE_SCR 12
#define Y_TIME openborvariant("vresolution")-16
#define Y_VS_TWO_PLAYERS 30

#define Y_FIX 16 - 12
#define Y_READY 45
#define Y_CHAR_CARD 20
#define Y_PRESS_START 80
#define Y_CHAR_NAME 186
#define Y_PLAYER_STR 188 + 16


void checkSetupPlayerList(int selector) {
    if ( selector == 3 ) setupPlayerList(4);
    else if ( selector == 2 || selector == 8 ) setupPlayerList(5);
    else setupPlayerList(0);
}

int set_unselected_map(void player_entity, int player_index, int selection_type) {
    int color_flag = 0;
    int selected_maps = 0;
    int map_count = gep(player_entity, "mapcount");
    int hmapl = gep(player_entity, "hmapl");
    int hmapu = gep(player_entity, "hmapu");
    int p;

    // contiamo solo le mappe non nascoste
    if ( hmapl > 0 && hmapu > 0 )  map_count -= (hmapu-hmapl+1); // map_count - hide_maps

    // vedi se è lo stesso pg e flagga la map. color_list che conterrà tutti i colori
    for (p = 0; p < MAX_PLAYERS; ++p) {
        if ( p == player_index ) continue;
        if ( glv("player."+p+".joint") ) {
            if ( gpp(player_index, "name") == gpp(p, "name") ) {
                int existing_map_color = glv("player."+p+".color");
                int map_flag = exp(2,existing_map_color); // 1,2,4 etc...

                if ( !is_flag_on(color_flag,map_flag) ) {
                    color_flag += map_flag; // add new flag
                    ++selected_maps;
                }
            }
        }
    }
    //if ( player_index == 1 ) slv("test.color.map",color_flag);

    if ( map_count > selected_maps ) {
        int map_flag = exp(2,gep(player_entity, "map"));

        while ( is_flag_on(color_flag,map_flag) ) {
            int map;

            if ( selection_type == NULL() || selection_type == 0 ) map = gep(player_entity, "map")-1;
            else map = gep(player_entity, "map")+1;

            if ( map < 0 ) map = gep(player_entity, "mapcount")-1;
            else if ( map >= gep(player_entity, "mapcount") ) map = 0;

            while( map >= hmapl && map <= hmapu && hmapl != 0 && hmapu != 0 ) {
                if ( selection_type == NULL() || selection_type == 0 ) {
                    --map;
                } else {
                    ++map;
                }
                if ( map < 0 ) map = gep(player_entity, "mapcount")-1;
                else if ( map >= gep(player_entity, "mapcount") ) map = 0;
            }
            cep(player_entity, "map", map);
            cpp(player_index, "colourmap", map);
            slv("player."+player_index+".color", map);

            map_flag = exp(2,gep(player_entity, "map"));
        }
    }
}

void selectPlayer() {
	int i = 0, s = 0, t = 0, d = 0, p = 0; // s = selected character in list, d = ready players, t = joint players
	void ent;
	void list = glv("playerlist");
	void flag = glv("playerflag");
	int se = loadsample("data/sounds/beep2.wav");
	int se2 = loadsample("data/sounds/beep.wav");
	int se3 = loadsample("data/sounds/cancel01.wav");
	char text  = "SELECT YOUR CHARACTER";
	int vres = openborvariant("vresolution");
	int hres = openborvariant("hresolution");
	void title_bg = glv("title_bg");

	//drawstring(10,100,0,glv("test.color.map"));

    if ( glv("select_music") == NULL() ) {
        if ( ggv("1VS1") ) playmusic(SELECT_VERSUS_MUSIC, 1);
        else playmusic(SELECT_MUSIC, 1);
        slv("select_music",1);
    }
    if (!title_bg) slv("title_bg", spawnsubentity("title_bg", 0,0,0));

	//int se = loadsample("data/sounds/lkicksh.wav");

    //if (ent) drawstring(10,160,0,"MAP:  "+gep(ent, "map"));
    //if (ent) drawstring(10,160,0,"MAP:  "+gep(ent, "hmapl"));
    //if (ent) drawstring(10,170,0,"MAP:  "+gep(ent, "hmapu"));
    //for(p = 0; p < size(list); ++p) drawstring(10, 40+10*p, 0, ":   "+ get(list, p) + " : " + get(flag, p) );

    drawstring(hres/2-strwidth(text, 0)/2, Y_TITLE_SCR, 6, text);
    for(i = 0; i < MAX_PLAYERS; i++) {
            if ( i<2 ) drawstring(13+79*i, Y_PLAYER_STR+Y_FIX, 0, "player "+(i+1));
            else drawstring(1+13+79*i, Y_PLAYER_STR+Y_FIX, 0, "player "+(i+1));
    }

	for(i = 0; i < MAX_PLAYERS; i++) {
		if( glv("player."+i+".join") ) { // == true significa che sta entrando (premuto start) o è già entrato
            if ( !glv("player."+i+".joint") ) {
                if ( ov("count_players") > 0 ) playsample(se); // in questo momento è count_players-1 perchè ancora non è stato spawnato il player
                slv("player."+i+".joint", 1); // == 1 significa che è appena entrato. Non entrato == NULL(), Entrato da tempo == 2
            }
			if( !glv("player."+i+".decided") ) {
				s = glv("player."+i+".selector");
				if(!s) { s = 0; }
				if( gpp(i, "name") == "" ) {
                    // CONTROLLO PERSONAGGIO GIA' PRESO
                    if ( NOSAME_FLAG && glv("player."+i+".joint") == 2 && size(list) > 1 && size(list) > ov("count_players") ) {
                        if( get(flag, s) ) {
                            if (glv("player."+i+".move") == -1) {
                                do {
                                    if( --s < 0 ) s = size(list)-1;
                                } while ( get(flag,s) > 0 );
                            } else {
                                do {
                                    if ( ++s >= size(list) ) s = 0;
                                } while ( get(flag,s) > 0 );
                            }
                        }
                    }
                    // FINE CONTROLLO PERSONAGGIO GIA' PRESO

                    // CONTROLLO PERSONAGGIO GIA' PRESO SU NEW ENTRY (START/ENTRY ON SELECT SCREEN)
                    if ( (NOSAME_FLAG || NOSAME_FLAG_INIT) && size(list) > 1 && size(list) > ov("count_players") && glv("player."+i+".joint") == 1 ) { // Eseguiamo questo processo solo se c'è più di un personaggio selezionabile e solo quando un giocatore è appena entrato
                        s = 0;
                        for(p = 0; p < size(list); ++p) {
                            if ( get(flag,p) == 0 ) {
                                break;
                            } else ++s;
                        }
                    }
                    slv("player."+i+".joint", 2);
                    // ################################

					cpp(i, "name", get(list, s));
					if ( NOSAME_FLAG || NOSAME_FLAG_INIT ) set(flag, s, get(flag,s)+1); // NOSAME: Chiudi a commento se vuoi settare NOSAME = 0

					ent = gpp(i, "entity");
					if(ent) killentity(ent);
					setspawnentry("name", gpp(i, "name"));
					ent = spawn();

                    // CHECK NOT EXISTING CHARACTER
					if ( !gep(ent,"exists") ) {
                        if ( glv("player."+i+".move") == 1 ) {
                            set(flag, s, get(flag,s)-1);
                            cpp(i, "name", "");
                            if(++s>=size(list)) s = 0;
                        } else {
                            set(flag, s, get(flag,s)-1);
                            cpp(i, "name", "");
                            if(--s<0) s = size(list)-1;
                        }
                        slv("player."+i+".selector", s);
                        continue;
					}
					// ################################

					cpp(i, "colourmap", gep(ent,"map")); // map reset
					slv("player."+i+".color", gep(ent,"map"));
                    // CONTROLLO COLORE GIA' PRESO
                    if ( NOSAME_COLOR_FLAG || NOSAME_COLOR_FLAG_INIT ) set_unselected_map(ent,i,0);
                    // FINE CONTROLLO COLORE GIA' PRESO

                    // set 1p, 2p etc...
					if ( glv("player."+i+".arrow") ) killentity(glv("player."+i+".arrow"));
                    slv( "player."+i+".arrow", spawnsubentity_relative("parrow"+(i+1),0,0,0,ent) );

                    // CHARACTER CARD
					if (i < 2) cep(ent, "position", 0+80*i, Y_CHAR_CARD+Y_FIX, 0);
					else cep(ent, "position", 1+80*i, Y_CHAR_CARD+Y_FIX, 0);

					cep(ent, "direction", 1);
					cep(ent, "aiflag", "blink", 0);
					cep(ent, "noaicontrol", 1);
					cep(ent, "gfxshadow", 1);
                    setidle(ent, oc("ANI_SELECT"), 1); //58 = waiting -- 24 = select
					cpp(i, "entity", ent);
				}

				ent = gpp(i, "entity");
				if(keyp_coin(i, "moveleft")) {
                    playsample(se2);
                    set(flag, s, get(flag,s)-1);
                    cpp(i, "name", "");
					if(--s<0) s = size(list)-1;
					slv("player."+i+".move", -1);
				} else if(keyp_coin(i, "moveright")) {
				    playsample(se2);
				    set(flag, s, get(flag,s)-1);
				    cpp(i, "name", "");
					if(++s>=size(list)) s = 0;
					slv("player."+i+".move", 1);
				} else if(keyp_coin(i, "movedown")) {
				    int map = gep(ent, "map")-1;
				    int hmapl = gep(ent, "hmapl");
				    int hmapu = gep(ent, "hmapu");

				    //playsample(se2);
				    if ( map < 0 ) map = gep(ent, "mapcount")-1;
                    while( map >= hmapl && map <= hmapu && hmapl != 0 && hmapu != 0 ) {
                        --map;
                        if ( map < 0 ) map = gep(ent, "mapcount")-1;
                    }
                    cep(ent, "map", map);
                    cpp(i, "colourmap", map);
                    slv("player."+i+".color", map);

                    if ( NOSAME_COLOR_FLAG ) set_unselected_map(ent,i,0);
				} else if(keyp_coin(i, "moveup")) {
                    int map = gep(ent, "map")+1;
				    int hmapl = gep(ent, "hmapl");
				    int hmapu = gep(ent, "hmapu");

				    //playsample(se2);
				    if ( map >= gep(ent, "mapcount") ) map = 0;
                    while( map >= hmapl && map <= hmapu && hmapl != 0 && hmapu != 0 ) {
                        ++map;
                        if ( map >= gep(ent, "mapcount") ) map = 0;
                    }
                    cep(ent, "map", map);
                    cpp(i, "colourmap", map);
                    slv("player."+i+".color", map);

                    if ( NOSAME_COLOR_FLAG ) set_unselected_map(ent,i,1);
				} else if (keyp_coin(i, "start") || keyp_coin(i, "attack")) {
					if( gep(ent, "aiflag", "idling") ) {
						setidle(ent, oc("ANI_PICK"), 1); //58 = waiting -- 24 = select
                        if ( glv("player."+i+".arrow") ) { // Elimina l'arrow
                            if ( gep(glv("player."+i+".arrow"),"exists") ) killentity( glv("player."+i+".arrow") );
                            slv("player."+i+".arrow", NULL());
                        }
						slv("player."+i+".decided", 1);
						/*if(glv("mselector") == 3) {
							setspawnentry("name", "title_bg");
							setspawnentry("coords", 160, 260, 0);
							spawn();
						}*/
					}
				}

				slv("player."+i+".selector", s);
			} else {
				ent = gpp(i, "entity");
				if( !gep(ent, "aiflag", "animating") && glv("player."+i+".decided") == 1 ) {
					d++;
                    if (i<2) drawstring(18+80*i,Y_READY+Y_FIX,0,"ready!");
                    else drawstring(1+18+80*i,Y_READY+Y_FIX,0,"ready!");
				}
			}
			t++; // t = giocatori entrati, d = giocatori che hanno scelto

            if ( CHAR_NAME_DISPLAY ) {
                char char_name = getentityproperty(ent,"defaultmodel");
                int pixel_fix = 0;
                float l_board;
                float card_width = 79;
                int font_index = 6;

                char_name = get_ps_player_name(char_name);

                if ( i >= 2 ) ++pixel_fix;
                l_board = pixel_fix+1+80*i;
                drawstring(l_board+card_width/2-strwidth(char_name,font_index)/2, Y_CHAR_NAME+Y_FIX, font_index, char_name);
            }
		} else {
		    int t = openborvariant("elapsed_time");
		    int game_speed = openborvariant("game_speed");
		    int pixel_fix = 0;
			//if ( t != d )

            if ( !COIN_SYSTEM ) slv("player."+i+".join",keyp_coin(i,"start")||keyp(i,"attack4"));
            else {
                if ( keyp(i,"attack4") || (keyp_coin(i,"start") && gpp(i,"credits") > 0) ) slv("player."+i+".join",1);
            }

            if ( INSERT_COINS_ALT && PRESS_START_DISPLAY && (t%(game_speed/2)) > (game_speed/2/2) ) {
                if ( getlocalvar("chud_show_ps_sel") == 1 ) {
                    if ( getlocalvar("ps_ic_sel_sem") == NULL() || getlocalvar("ps_ic_sel_sem") == 0 ) setlocalvar("ps_ic_sel_sem",1);
                    else setlocalvar("ps_ic_sel_sem",0);
                }

                if ( getlocalvar("chud_show_ps_sel") == 1 ) setlocalvar("chud_show_ps_sel",0);
                if ( getlocalvar("chud_show_ps_sel_first") == NULL() ) setlocalvar("chud_show_ps_sel_first",1);
            } else {
                if ( getlocalvar("chud_show_ps_sel_first") == 1 && (getlocalvar("chud_show_ps_sel") == NULL() || getlocalvar("chud_show_ps_sel") == 0) ) setlocalvar("chud_show_ps_sel",1);
            }

            if ( PRESS_START_DISPLAY && (t%(game_speed/2)) > (game_speed/2/2) ) {
                if ( i >= 2 ) ++pixel_fix;

                if ( getlocalvar("ps_ic_sel_sem") == NULL() || getlocalvar("ps_ic_sel_sem") == 0 ) {
                    drawstring(pixel_fix+21+79*i, Y_PRESS_START+Y_FIX, 6, "PRESS");
                    drawstring(pixel_fix+21+79*i, Y_PRESS_START+15+Y_FIX, 6, "START");
                } else {
                    drawstring(pixel_fix+21+79*i-3, Y_PRESS_START+Y_FIX, 6, "INSERT");
                    drawstring(pixel_fix+21+79*i-3-2, Y_PRESS_START+15+Y_FIX, 6, "COIN(S)");
                }
                //drawstring(-4+20+79*i, 80+15*2+Y_FIX, 6, "BUTTON");
            } else {

            }
		}
	}
	//drawstring(10,160,0,"t:  "+t+" d: "+d);

    // ----------- VERSUS TIME --------------
    if ( ggv("1VS1") ) {
        char text_vs_time = "time: ";
        float x_time_icon = hres-strwidth("time: off", 0)-11;

        if ( glv("ps_load_flag") == NULL() ) {
            if ( glv("time_icon") == NULL() ) slv("time_icon",loadsprite("data/chars/misc/timeicon.gif"));
            slv("ps_load_flag",1);
        }


        if ( key_coin("jump") || key_coin("attack2") ) {
            //int se = loadsample("data/sounds/beep2.wav");
            int se2 = loadsample("data/sounds/beep.wav");

            playsample(se2);
            if ( ggv("versus_time") == NULL() ) sgv("versus_time",1);
            else sgv("versus_time",NULL());
        }

        if ( glv("time_icon") ) drawsprite(glv("time_icon"),x_time_icon-16-2,vres-20,10,0);
        if ( ggv("versus_time") == NULL() ) text_vs_time += "off";
        else text_vs_time += "on";
        drawstring(x_time_icon, Y_TIME, 0, text_vs_time);
    }
    // --------------------------------------

	if( t > 0 && t == d ) { // s = selected character in list, d = ready players, t = joint players
        int counter = 0;

        if ( ggv("1VS1") ) {
            if ( t < 2 ) {
                int t = openborvariant("elapsed_time");
                int game_speed = openborvariant("game_speed");
                char text_1vs1 = "please wait for a second player at least...";

                //if ( (t%(game_speed/2.2)) > (game_speed/2.2/4.4) )
                    drawstring(hres/2-strwidth(text_1vs1, 0)/2, Y_VS_TWO_PLAYERS, 0, text_1vs1);
                if ( key_coin("esc") || key_coin("special") ) {
                    int i = 0;

                    for (i = 0; i < MAX_PLAYERS; ++i) {
                        if ( keyp_coin(i,"special") ) {
                            if ( glv("player."+i+".joint") == NULL() ) {
                                exit_to_stage();
                                break;
                            }
                            else if ( glv("player."+i+".decided") ) clean_player(i);
                            else if ( t > 1 ) clean_player(i,1);
                            else {
                                exit_to_stage();
                                break;
                            }
                        } else if ( keyp_coin(i,"esc") ) {
                            exit_to_stage();
                            break;
                        }
                    } // fine for
                }

                return;
            }
        }

        // re-check for all player decided
		for( i = 0; i < MAX_PLAYERS; i++) {
			if ( glv("player."+i+".decided") ) {
                ++counter;
			}
		}
		if ( counter != d || t != d || t != counter ) return;

		clp("background", 0, "enabled", 0);
		change_bglayer(BGLAYERS_NUM,-1,0);
		//sgv("in_menu", NULL());
        if (glv("title_bg")) killentity(title_bg);
        slv("title_bg", NULL());
		if( ggv("wild_card_array") ) { free(ggv("wild_card_array")); sgv("wild_card_array", NULL()); }

		play_game(); // <-------------------------

        //if(glv("playerlist")!=NULL()) {free(glv("playerlist")); slv("playerlist",NULL());}
        //if(glv("playerflag")!=NULL()) {free(glv("playerflag")); slv("playerflag",NULL());}
	} else if( key_coin("esc") || key_coin("special") ) {
	    int i = 0;

	    for (i = 0; i < MAX_PLAYERS; ++i) {
            if ( keyp_coin(i,"special") ) {
                if ( glv("player."+i+".joint") == NULL() ) {
                    exit_to_stage();
                    break;
                }
                else if ( glv("player."+i+".decided") ) clean_player(i);
                else if ( t > 1 ) clean_player(i,1);
                else {
                    exit_to_stage();
                    break;
                }
            } else if ( keyp_coin(i,"esc") ) {
                exit_to_stage();
                break;
            }
	    } // fine for
	} // fine if attesa pg selezionati

}

void play_game() {
    int i = 0;
    int selector;

    for (i = 0; i < MAX_PLAYERS; i++) {
        cpp(i, "entity", NULL());
        if(gpp(i, "name") != "") {
            if ( ggv("1VS1") ) cpp(i, "lives", LIVES);
            else cpp(i, "lives", LIVES);
            sgv("current_credits_"+i,ggv("current_credits_"+i)-1);
            cpp(i, "credits", ggv("current_credits_"+i));
        } else if ( !gpp(i,"hasplayed") ) {
            cpp(i, "credits", ggv("current_credits_"+i));
        }
        //if (i == 0) {cpp(i,"credits",0);cpp(i,"lives",0);}
    }

    if ( ggv("1VS1") ) {
        //changeopenborvariant("nojoin", 1); // <--- in levelscript
        selector = glv("mselector") + size(glv("stageitem"));
    } else {
        selector = glv("mselector");
    }

    //cpp(0, "lives", 0); cpp(0, "credits", 0); cpp(0, "name", ""); killentity(gpp(0, "entity"));
    playgame(selector+1, 2); //selector+1
}

void exit_to_stage() {
    int i = 0;
	int se = loadsample("data/sounds/beep2.wav");
	int se2 = loadsample("data/sounds/beep.wav");
	int se3 = loadsample("data/sounds/cancel01.wav");

    playsample(se3);
    setglobalvar("menu_mode","stage");
    slv("select_music",NULL());
    if (glv("title_bg")) killentity(glv("title_bg"));
    slv("title_bg", NULL());
    for(i=0; i<MAX_PLAYERS; i++) {
        void ent;

        cpp(i, "lives", 0);
        //cpp(i, "credits", 0);
        cpp(i, "colourmap", 0);
        cpp(i, "name", "");
        ent = gpp(i, "entity");
        if(ent) killentity(ent);
        cpp(i, "entity", NULL());
        slv("player."+i+".join", NULL());
        slv("player."+i+".joint", NULL());
        slv("player."+i+".decided", NULL());
        slv("player."+i+".move", NULL());
        slv("player."+i+".color", NULL());
        if ( glv("player."+i+".arrow") ) {
            killentity( glv("player."+i+".arrow") );
            slv("player."+i+".arrow", NULL());
        }
        slv("player."+i+".selector", NULL()); // Se non vuoi ricordare l'ultimo pg scelto
    }
    for(i=0; i<size(glv("playerflag")); i++) {
        set(glv("playerflag"), i, 0);
    }
    if ( ggv("1VS1") && glv("mode") != "1VS1_stage" ) playmusic(VERSUS_MUSIC, 1);
    else playmusic(TITLE_MUSIC, 1);
    //if(ent) killentity(ent);
    clp("background", 0, "enabled", 0);
    change_bglayer(BGLAYERS_NUM,2,1);
    //slv("mselector", 0); // <---------- NOT CLEAN LAST STAGE SELECTOR
    set_blackscreen(0);

    slv("start", "mode");
    if ( ggv("1VS1") && glv("versus_menu") != 1 ) slv("mode", "1VS1_stage");
    else slv("mode", "stage");
}

void clean_player(int i, int destroy_flag) {
	//int se = loadsample("data/sounds/beep2.wav");
	//int se2 = loadsample("data/sounds/beep.wav");
	int se3 = loadsample("data/sounds/cancel01.wav");
    void ent;

    playsample(se3);

    if ( destroy_flag == NULL() || destroy_flag <= 0 ) {
        slv("player."+i+".decided", NULL());
        ent = gpp(i, "entity");
        if (ent) setidle(ent, oc("ANI_SELECT"), 1);
        slv( "player."+i+".arrow", spawnsubentity_relative("parrow"+(i+1),0,0,0,ent) );
    } else {
        slv("player."+i+".decided", NULL());
        cpp(i, "lives", 0);
        cpp(i, "credits", 0);
        cpp(i, "colourmap", 0);
        cpp(i, "name", "");
        ent = gpp(i, "entity");
        if(ent) killentity(ent);
        cpp(i, "entity", NULL());
        slv("player."+i+".join", NULL());
        slv("player."+i+".joint", NULL());
        slv("player."+i+".move", NULL());
        slv("player."+i+".color", NULL());
        if ( glv("player."+i+".arrow") ) {
            killentity( glv("player."+i+".arrow") );
            slv("player."+i+".arrow", NULL());
        }
        slv("player."+i+".selector", NULL()); // Se non vuoi ricordare l'ultimo pg scelto
        set(glv("playerflag"), i, 0);
    }
}

char get_ps_player_name(char defaultmodel) {
    if ( defaultmodel == "LEO" ) return "LEO"; // LEONARDO
    else if ( defaultmodel == "MIKE" ) return "MIKE"; // MICHAELANGELO
    else if ( defaultmodel == "DON" ) return "DON"; // DONATELLO
    else if ( defaultmodel == "RAPH" ) return "RAPH"; // RAPHAEL
    else if ( defaultmodel == "FOOT_SOLDIER" ) return "FOOT_S.";
    else if ( defaultmodel == "ROCK_SOLDIER" ) return "ROCK_S.";
    else if ( defaultmodel == "TOKKA" ) return "TOKKA";
    else if ( defaultmodel == "RAHZAR" ) return "RAHZAR";
    else if ( defaultmodel == "ROCKSTEADY" ) return "ROCKSTEADY";
    else if ( defaultmodel == "BEBOP" ) return "BEBOP";
    else if ( defaultmodel == "BAXTER" ) return "BAXTER";
    else return defaultmodel;
}
