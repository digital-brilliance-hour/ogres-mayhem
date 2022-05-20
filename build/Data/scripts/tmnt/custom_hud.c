
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/lib_drawing.c"
#import "data/scripts/custom_hud_commons.c"
#import "data/scripts/custom_hud_player_textmoves.c"
#import "data/scripts/custom_hud_game_over.c"


void check_char_select_inlevel(int p) {
    if ( openborvariant("in_level") ) {
        if ( getlevelproperty("type") != 2 ) {
            if ( NOSAME_FLAG == 1 || (NOSAME_FLAG_INIT == 1 && ggv("nosame"+p) == NULL()) ) {
                void player = gpp(p,"entity");
                char name = gpp(p,"name");
                int pexists = gep(player,"exists");
                int is_inselect = 0;

                // is in inlevel character select
                if ( (name != "" || ( name == "" && playerkeys(p,1,"start") )) && !pexists && ov("nojoin") == 0 && gpp(p,"credits") > 0 ) {
                    if ( playerkeys(p,1,"moveleft") || playerkeys(p,1,"moveright") || (is_flag_on(gpp(p,"keys"),oc("FLAG_START"))) ) {
                        if ( name != "" ) {
                            int i = 0, pp = 0;
                            int char_num = 0, pos = 0;
                            void char_list = array(0);
                            void flag_list = array(0);

                            is_inselect = 1;

                            i = 0;
                            while( getCharacter(i) != "" ) {
                                int flagged = 0;

                                set(char_list,i,getCharacter(i));
                                for (pp = 0; pp < MAX_PLAYERS; ++pp) {
                                    if ( gpp(pp,"name") == getCharacter(i) && pp != p ) {
                                        set(flag_list,i,1);
                                        flagged = 1;
                                        break;
                                    }
                                }
                                if ( !flagged ) set(flag_list,i,0);

                                if ( getCharacter(i) == gpp(p,"name") ) pos = i;

                                ++char_num;
                                ++i;
                            }
                            i = 0;

                            i = pos;
                            if ( playerkeys(p,1,"moveright") || (is_flag_on(gpp(p,"keys"),oc("FLAG_START"))) ) {
                                while( get(flag_list,i) == 1 ) {
                                    ++i;
                                    if ( i > size(flag_list)-1 ) i = 0;
                                }
                            } else if ( playerkeys(p,1,"moveleft") ) {
                                while( get(flag_list,i) == 1 ) {
                                    --i;
                                    if ( i < 0 ) i = size(flag_list)-1;
                                }
                            }

                            cpp(p,"name",getCharacter(i));

                            free(char_list);
                            free(flag_list);
                        }
                    }
                }
                if ( is_flag_on(gpp(p,"keys"),oc("FLAG_START")) ) sgv("nosame"+p,1);
                //drawstring(10,50+p*10,0,"name("+(p+1)+"): "+name+"  "+pexists+"  "+is_inselect);
            } // fine if NO_SAME

            if ( NOSAME_COLOR_FLAG == 1 || (NOSAME_COLOR_FLAG_INIT == 1 && ggv("nosamecolour"+p) == NULL()) ) {
                void player = gpp(p,"entity");
                char name = gpp(p,"name");
                int pexists = gep(player,"exists");
                int is_inselect = 0;

                // is in inlevel character select
                if ( (name != "" || ( name == "" && playerkeys(p,1,"start") )) && !pexists && ov("nojoin") == 0 && gpp(p,"credits") > 0 ) {
                    if ( playerkeys(p,1,"moveleft") || playerkeys(p,1,"moveright") || playerkeys(p,1,"moveup") || playerkeys(p,1,"movedown") || (is_flag_on(gpp(p,"keys"),oc("FLAG_START"))) ) {
                        if ( name != "" ) {
                            int i = 0, pp = 0;
                            int pos = 0;
                            void flag_list = array(0);
                            int color_flag = 0, selected_maps = 0;
                            int map_count;
                            int hmapl;
                            int hmapu;

                            is_inselect = 1;

                            /*void fakeent;
                            clearspawnentry();
                            setspawnentry("name",name);
                            setspawnentry("alias","fakeent");
                            setspawnentry("coords",100,200,0);
                            fakeent = spawn();
                            cep(fakeent,"noaicontrol",1);
                            cep(fakeent,"type",oc("TYPE_NONE"));

                            map_count = gep(fakeent, "mapcount");
                            hmapl = gep(fakeent, "hmapl");
                            hmapu = gep(fakeent, "hmapu");

                            killentity(fakeent);*/
                            map_count = gpp(p, "mapcount");
                            hmapl = gpp(p, "hmapl");
                            hmapu = gpp(p, "hmapu");

                            // init
                            if ( hmapl > 0 && hmapu > 0 )  map_count -= (hmapu-hmapl+1); // map_count - hide_maps

                            for (pp = 0; pp < MAX_PLAYERS; ++pp) {
                                if ( gpp(pp,"name") == name && p != pp ) {
                                    int existing_map_color = gpp(pp,"colourmap");
                                    int map_flag = exp(2,existing_map_color); // 1,2,4 etc...

                                    if ( !is_flag_on(color_flag,map_flag) ) {
                                        color_flag += map_flag; // add new flag
                                        ++selected_maps;
                                    }
                                }
                            }

                            if ( map_count > selected_maps ) {
                                int map_flag = exp(2,gpp(p,"colourmap"));
                                int map = gpp(p,"colourmap");

                                while ( is_flag_on(color_flag,map_flag) ) {
                                    if ( playerkeys(p,1,"moveup") ) map += 1;
                                    else if ( playerkeys(p,1,"movedown") || playerkeys(p,1,"moveleft") || playerkeys(p,1,"moveright") || is_flag_on(gpp(p,"keys"),oc("FLAG_START")) ) map -= 1;

                                    if ( map < 0 ) map = map_count-1;
                                    else if ( map >= map_count ) map = 0;

                                    while( map >= hmapl && map <= hmapu && hmapl != 0 && hmapu != 0 ) {
                                        if ( playerkeys(p,1,"movedown") || playerkeys(p,1,"moveleft") || playerkeys(p,1,"moveright") || is_flag_on(gpp(p,"keys"),oc("FLAG_START")) ) {
                                            --map;
                                        } else if ( playerkeys(p,1,"moveup") ) {
                                            ++map;
                                        }
                                        if ( map < 0 ) map = map_count-1;
                                        else if ( map >= map_count ) map = 0;
                                    }

                                    map_flag = exp(2,map);
                                }

                                cpp(p,"colourmap",map);
                            }

                            free(flag_list);
                        }
                    }
                    if ( is_flag_on(gpp(p,"keys"),oc("FLAG_START")) ) sgv("nosamecolour"+p,1);
                }

                //drawstring(10,50+p*10,0,"name("+(p+1)+"): "+name+"  "+pexists+"  "+is_inselect);
            } // fine if NO_SAME
        }
    }
}

void show_inserted_credits() {
    if ( openborvariant("in_level") ) {
        if ( getlevelproperty("type") != 2 && getglobalvar("in_menu") != 1 ) {
            if ( ggv("1VS1") == NULL() && ggv("add_credits_master") == 1 ) {
                int p = 0;
                int show_flag = 0;
                int game_speed = ov("game_speed");
                int time = ov("elapsed_time");
                int ETA = game_speed*3.0;
                int yshift = 12, y_fix = 0;

                for (p = 0; p < MAX_PLAYERS; ++p) {
                    int credits = 0, max_digits = 4, miss_digit_num;
                    char c_credits = "";
                    void player = gpp(p,"entity");
                    int i = 0;

                    if ( !gep(player,"exists") ) return; //continue;

                    credits = gpp(p,"credits");
                    miss_digit_num = max_digits - digit_count(credits);
                    c_credits = credits;
                    for ( i = 0; i < miss_digit_num; ++i ) c_credits = "0"+c_credits;

                    if ( ggv("add_credits_"+p) == 1 || glv("add_credits_time"+p) != NULL() ) {
                        if ( ggv("add_credits_"+p) == 1 ) {
                            slv("add_credits_time"+p,get_next_time(ETA));
                            sgv("add_credits_"+p,NULL());
                        }

                        if ( !show_flag ) show_flag = 1;

                        if ( gep(player,"aiflag","dead") && gpp(p,"lives") <= 1 ) y_fix = yshift+1;
                        drawstring(4+p*82,34+y_fix,7,"CREDITS"+c_credits);

                        if ( glv("add_credits_time"+p) != NULL() ) {
                            if ( time >= glv("add_credits_time"+p) ) slv("add_credits_time"+p,NULL());
                        }
                    }
                } // fine for
                if ( !show_flag ) { // don't show credits anymore!
                    sgv("add_credits_master",NULL());
                }
            }
        } else {
            if ( ggv("add_credits_master") == 1 ) {
                int p = 0;
                int show_flag = 0;
                int game_speed = ov("game_speed");
                int time = ov("elapsed_time");
                int ETA = game_speed*3.0;
                int yshift = 12, y_fix = 0;

                if ( ggv("1VS1") != NULL() && ggv("menu_mode") == NULL() ) return;

                for (p = 0; p < MAX_PLAYERS; ++p) {
                    int credits = 0, max_digits = 4, miss_digit_num;
                    char c_credits = "";
                    void player = gpp(p,"entity");
                    int i = 0;

                    credits = gpp(p,"credits");
                    if ( COIN_SYSTEM && no_players_ingame() && gpp(p,"credits") > 0 && p == 0 && ggv("fake_credit") > 0 ) credits -= 1;

                    miss_digit_num = max_digits - digit_count(credits);
                    c_credits = credits;
                    for ( i = 0; i < miss_digit_num; ++i ) c_credits = "0"+c_credits;

                    if ( ggv("add_credits_"+p) == 1 || glv("add_credits_time"+p) != NULL() ) {
                        if ( ggv("add_credits_"+p) == 1 ) {
                            slv("add_credits_time"+p,get_next_time(ETA));
                            sgv("add_credits_"+p,NULL());
                        }

                        if ( !show_flag ) show_flag = 1;

                        if ( gep(player,"aiflag","dead") && gpp(p,"lives") <= 1 ) y_fix = yshift+1;
                        drawstring(4+p*82,224+y_fix,7,"CREDITS"+c_credits,MAX_Z_LAYER);

                        if ( glv("add_credits_time"+p) != NULL() ) {
                            if ( time >= glv("add_credits_time"+p) ) slv("add_credits_time"+p,NULL());
                        }
                    }
                } // fine for
                if ( !show_flag ) { // don't show credits anymore!
                    sgv("add_credits_master",NULL());
                }
            }
        }
    }

    return;
}

void draw_credits_ps(int p) {
    if ( openborvariant("in_level") ) {
        if ( getlevelproperty("type") != 2 ) {
            //int p = 0;
            int time = openborvariant("elapsed_time");
            int game_speed = openborvariant("game_speed");
            int eta = game_speed*4; // 4 secs.
            int black = rgbcolor(0x00,0x00,0x00); // 0x00a8ff
            int white = rgbcolor(0xFF,0xFF,0xFF);
            int color1 = rgbcolor(0x15,0xAE,0xFD);
            int color2 = rgbcolor(0x2E,0xB8,0xFF);
            int color3 = rgbcolor(0x40,0xBD,0xFE);
            int color4 = rgbcolor(0x56,0xC5,0xFF);
            int color5 = rgbcolor(0x66,0xCA,0xFE);
            int color6 = rgbcolor(0x71,0xCD,0xFD);
            int color7 = rgbcolor(0x81,0xD3,0xFD);
            int color8 = rgbcolor(0x95,0xDA,0xFE);
            int color9 = rgbcolor(0xAD,0xE3,0xFF);
            int color10 = rgbcolor(0xC5,0xEB,0xFF);
            int color11 = rgbcolor(0xE1,0xF5,0xFF);

            load_sp_characters(); // carica gli sprites

            //for (p = 0; p < MAX_PLAYERS; ++p) {
                void player = getplayerproperty(p, "entity");
                char name = getplayerproperty(p, "name");
                int credits = getplayerproperty(p,"credits");
                //int health = getentityproperty(player,"health");
                float x1 = 4, x2 = 86, x3 = 168, x4 = 250, x;
                float y1 = 16, y2 = 16, y3 = 16, y4 = 16, y;
                float z = 5000, sortid = 0;

                     if ( p == 0 ) { x = x1; y = y1; }
                else if ( p == 1 ) { x = x2; y = y2; }
                else if ( p == 2 ) { x = x3; y = y3; }
                else if ( p == 3 ) { x = x4; y = y4; }

                if ( COIN_SYSTEM && no_players_ingame() && gpp(p,"credits") > 0 && p == 0 && ggv("fake_credit") > 0 ) credits -= 1;

                //drawstring(10,100,0,getplayerproperty(1, "keys"));
                //drawstring(10,100,0,openborvariant("player2"));
                // game_paused/pause

                /*
                    ---OLD----
                if ( playerkeys(p, 0, "start") && !openborvariant("game_paused") && !getentityproperty(player, "exists") ) setlocalvar("start_"+p,1);
                if ( getlocalvar("start_"+p) != NULL() && getplayerproperty(p,"lives") <= 0 ) setlocalvar("start_"+p,NULL()); //&& getentityproperty(player, "aiflag", "dead")

                if ( !getentityproperty(player, "exists") && getlocalvar("start_"+p) == NULL() ) { // name == "" trick
                */

                if ( playerkeys(p,0,"start") && !openborvariant("game_paused") && !getentityproperty(player, "exists") ) setlocalvar("start_"+p,1);
                if ( getlocalvar("start_"+p) != NULL() && getentityproperty(player, "exists") ) setlocalvar("start_"+p,NULL()); // && getentityproperty(player, "aiflag", "dead")
                if ( !getentityproperty(player, "exists") && name != "" && getlocalvar("start_"+p) == NULL() ) changeplayerproperty(p,"name","");
                //if(p==1)drawstring(10,100,0,"var: "+getplayerproperty(p,"lives"));
                //if ( p == 0 ) drawstring(10,100,0,"var: "+getlocalvar("start_"+p) );

                check_char_select_inlevel(p); // check character select inlevel!

                if ( !getplayerproperty(p,"hasplayed") && credits != getglobalvar("current_credits_"+p)-1 && getglobalvar("current_credits_"+p) > 0 && name != "" ) changeplayerproperty(p,"credits",getglobalvar("current_credits_"+p)-1);

                if ( !getentityproperty(player, "exists") && name == "" ) { // name == "" trick
                    int refresh_time = time%eta;
                    int tot_parts = 40; // tempo di visualizzazione di press start == 2 sec. tot == 4 sec. -> 200*2 == 400.
                    int mid_part = tot_parts/2-2-1; //((40/2) -2 -1) == 17; -2 perchè tolgo white+black colors. -1 perchè si parte da 0
                    int time_range;
                    int half_time = 0; // caso estremo: (200/2/40)*4*3 -> (100/40)*12

                    time_range = truncA((eta/2)/tot_parts); // la parte da 0 a 11 (dei 12 colori)
                    //if ( refresh_time >= eta/2 ) half_time = eta/2;

                    //if ( refresh_time >= eta/2 ) half_time = (eta/2)+30*p;
                    //else half_time = 30*p;
                    // con p > 0 facciamo brillare progressivamente il testo
                    if ( refresh_time >= eta/2 ) half_time = (eta/2)+(time_range*4)*p;
                    else half_time = time_range*4*p;

                    //else time_range = truncA((eta/2) + (eta/2)/tot_parts);

                        changedrawmethod(NULL(), "enabled", 1);
                        changedrawmethod(NULL(), "reset", 1);
                        //changedrawmethod(NULL(), "tintmode", 1);

                             if ( refresh_time <= half_time+time_range    ) changedrawmethod(NULL(), "fillcolor", black);
                        else if ( refresh_time <= half_time+time_range*2  ) changedrawmethod(NULL(), "fillcolor", color1);
                        else if ( refresh_time <= half_time+time_range*3  ) changedrawmethod(NULL(), "fillcolor", color2);
                        else if ( refresh_time <= half_time+time_range*4  ) changedrawmethod(NULL(), "fillcolor", color3);
                        else if ( refresh_time <= half_time+time_range*5  ) changedrawmethod(NULL(), "fillcolor", color4);
                        else if ( refresh_time <= half_time+time_range*6  ) changedrawmethod(NULL(), "fillcolor", color5);
                        else if ( refresh_time <= half_time+time_range*7  ) changedrawmethod(NULL(), "fillcolor", color6);
                        else if ( refresh_time <= half_time+time_range*8  ) changedrawmethod(NULL(), "fillcolor", color7);
                        else if ( refresh_time <= half_time+time_range*9  ) changedrawmethod(NULL(), "fillcolor", color8);
                        else if ( refresh_time <= half_time+time_range*10 ) changedrawmethod(NULL(), "fillcolor", color9);
                        else if ( refresh_time <= half_time+time_range*11 ) changedrawmethod(NULL(), "fillcolor", color10);
                        else if ( refresh_time <= half_time+time_range*12 ) changedrawmethod(NULL(), "fillcolor", color11);
                        else if ( refresh_time <= half_time+time_range*(mid_part)    ) changedrawmethod(NULL(), "fillcolor", white);
                        else if ( refresh_time <= half_time+time_range*(mid_part+1)  ) changedrawmethod(NULL(), "fillcolor", color11);
                        else if ( refresh_time <= half_time+time_range*(mid_part+2)  ) changedrawmethod(NULL(), "fillcolor", color10);
                        else if ( refresh_time <= half_time+time_range*(mid_part+3)  ) changedrawmethod(NULL(), "fillcolor", color9);
                        else if ( refresh_time <= half_time+time_range*(mid_part+4)  ) changedrawmethod(NULL(), "fillcolor", color8);
                        else if ( refresh_time <= half_time+time_range*(mid_part+5)  ) changedrawmethod(NULL(), "fillcolor", color7);
                        else if ( refresh_time <= half_time+time_range*(mid_part+6)  ) changedrawmethod(NULL(), "fillcolor", color6);
                        else if ( refresh_time <= half_time+time_range*(mid_part+7)  ) changedrawmethod(NULL(), "fillcolor", color5);
                        else if ( refresh_time <= half_time+time_range*(mid_part+8)  ) changedrawmethod(NULL(), "fillcolor", color4);
                        else if ( refresh_time <= half_time+time_range*(mid_part+9)  ) changedrawmethod(NULL(), "fillcolor", color3);
                        else if ( refresh_time <= half_time+time_range*(mid_part+10) ) changedrawmethod(NULL(), "fillcolor", color2);
                        else if ( refresh_time <= half_time+time_range*(mid_part+11) ) changedrawmethod(NULL(), "fillcolor", color1); // 17+11= 28 su 40
                        else if ( refresh_time <= half_time+time_range*tot_parts     ) changedrawmethod(NULL(), "fillcolor", black);

                    if ( openborvariant("nojoin") ) { // Visualizza WAIT
                        void spr;
                        int shiftx = 20;

                        spr = getglobalvar("sp_char_wait_fill");
                        if ( spr != NULL() ) drawsprite(spr,x+shiftx,y,z,sortid); // drawsprite(sprite, int x, int y, int z, int sortid)

                        changedrawmethod(NULL(), "enabled", 0);
                        changedrawmethod(NULL(), "reset", 1);

                        spr = getglobalvar("sp_char_wait_outs");
                        if ( spr != NULL() ) drawsprite(spr,x+shiftx,y,z,sortid);
                    } else { // visualizza PRESS START/CREDITS
                        // credits or press_start or insert_coins(s)
                        if ( refresh_time < eta/2 ) {
                            if ( getlocalvar("chud_show_credits") == 1 ) {
                                if ( getlocalvar("ps_ic_sem") == NULL() || getlocalvar("ps_ic_sem") == 0 ) setlocalvar("ps_ic_sem",1);
                                else setlocalvar("ps_ic_sem",0);
                            }

                            if ( getlocalvar("chud_show_credits") == 1 ) setlocalvar("chud_show_credits",0);
                        } else {
                            if ( getlocalvar("chud_show_credits") == NULL() || getlocalvar("chud_show_credits") == 0 ) setlocalvar("chud_show_credits",1);
                        }

                        if ( refresh_time < eta/2 ) { // visualizza PRESS START
                            void spr;
                            int shifty = 2;

                            if ( getlocalvar("ps_ic_sem") == NULL() || getlocalvar("ps_ic_sem") == 0 ) shifty = 0;

                            if ( getlocalvar("ps_ic_sem") == NULL() || getlocalvar("ps_ic_sem") == 0 ) spr = getglobalvar("sp_char_ps_fill");
                            else if ( getlocalvar("ps_ic_sem") == 1 ) spr = getglobalvar("sp_char_ic_fill");
                            if ( spr != NULL() ) drawsprite(spr,x,y-shifty,z,sortid); // drawsprite(sprite, int x, int y, int z, int sortid)

                            changedrawmethod(NULL(), "enabled", 0);
                            changedrawmethod(NULL(), "reset", 1);

                            if ( getlocalvar("ps_ic_sem") == NULL() || getlocalvar("ps_ic_sem") == 0 ) spr = getglobalvar("sp_char_ps_outs");
                            else if ( getlocalvar("ps_ic_sem") == 1 ) spr = getglobalvar("sp_char_ic_outs");
                            if ( spr != NULL() ) drawsprite(spr,x,y-shifty,z,sortid);
                        } else { // visualizza CREDITS
                            void spr;
                            int i;
                            int divider = 1000, tmp_divider;
                            int shiftx = 6, shift_tot = 41;
                            int tmp_credits = credits;

                            spr = getglobalvar("sp_char_credits_fill");
                            if ( spr != NULL() ) drawsprite(spr,x,y,z,sortid);

                            tmp_divider = divider;
                            tmp_credits = credits;
                            for (i = 0; tmp_divider >= 1; ++i) {
                                int num = truncA(tmp_credits/tmp_divider); // ex. 526/100 = 5

                                tmp_credits %= tmp_divider; // ex. 526%100 = 26
                                tmp_divider /= 10; // divide to 10
                                spr = getglobalvar("sp_num_"+num);
                                if ( spr != NULL() ) drawsprite(spr,x+shift_tot+shiftx*i,y,z,sortid);
                            }

                            changedrawmethod(NULL(), "enabled", 0);
                            changedrawmethod(NULL(), "reset", 1);

                            tmp_divider = divider;
                            tmp_credits = credits;
                            for (i = 0; tmp_divider >= 1; ++i) {
                                int num = truncA(tmp_credits/tmp_divider); // ex. 526/100 = 5

                                tmp_credits %= tmp_divider; // ex. 526%100 = 26
                                tmp_divider /= 10; // divide to 10
                                spr = getglobalvar("sp_num_outs_"+num);
                                if ( spr != NULL() ) drawsprite(spr,x+shift_tot+shiftx*i,y,z,sortid);
                            }

                            spr = getglobalvar("sp_char_credits_outs");
                            if ( spr != NULL() ) drawsprite(spr,x,y,z,sortid);
                        } // fine if credits
                        //else drawstring(10,120,6,"PRESS START");
                    }

                } // fine if il player non c'è
            //} // fine for
        } // fine if type_level
    } /*else {
        //unload_sp_characters();
    }*/ // fine if in_level
}

void load_sp_characters() {
    if ( getglobalvar("sp_num_0") == NULL() ) {
        int i;

        for (i = 0; i < 10; ++i) {
            char path_fill = "data/sprites/special_characters/num_"+i+".gif";
            char path_outs = "data/sprites/special_characters/num_outs_"+i+".gif";
            void spr_fill = loadsprite(path_fill);
            void spr_outs = loadsprite(path_outs);

            setglobalvar("sp_num_"+i, spr_fill);
            setglobalvar("sp_num_outs_"+i, spr_outs);
        }

        setglobalvar("sp_char_ps_fill", loadsprite("data/sprites/special_characters/ps01.gif"));
        setglobalvar("sp_char_ps_outs", loadsprite("data/sprites/special_characters/ps_outs.gif"));
        setglobalvar("sp_char_ic_fill", loadsprite("data/sprites/special_characters/ic01.gif"));
        setglobalvar("sp_char_ic_outs", loadsprite("data/sprites/special_characters/ic_outs.gif"));
        setglobalvar("sp_char_credits_fill", loadsprite("data/sprites/special_characters/credits01.gif"));
        setglobalvar("sp_char_credits_outs", loadsprite("data/sprites/special_characters/credits_outs.gif"));
        setglobalvar("sp_char_wait_fill", loadsprite("data/sprites/special_characters/wait_spr.gif"));
        setglobalvar("sp_char_wait_outs", loadsprite("data/sprites/special_characters/wait_outs.gif"));
    } // fine if check NULL()
}

void unload_sp_characters() {
    int i;

    for (i = 0; i < 10; ++i) {
        if ( getglobalvar("sp_num_"+i) != NULL() ) { free(getglobalvar("sp_num_"+i)); setglobalvar("sp_num_"+i, NULL()); }
        if ( getglobalvar("sp_num_outs_"+i) != NULL() ) { free(getglobalvar("sp_num_outs_"+i)); setglobalvar("sp_num_outs_"+i, NULL()); }
    }

    if ( getglobalvar("sp_char_ps_fill") != NULL() ) { free(getglobalvar("sp_char_ps_fill")); setglobalvar("sp_char_ps_fill", NULL()); }
    if ( getglobalvar("sp_char_ps_outs") != NULL() ) { free(getglobalvar("sp_char_ps_outs")); setglobalvar("sp_char_ps_outs", NULL()); }
    if ( getglobalvar("sp_char_ic_fill") != NULL() ) { free(getglobalvar("sp_char_ic_fill")); setglobalvar("sp_char_ic_fill", NULL()); }
    if ( getglobalvar("sp_char_ic_outs") != NULL() ) { free(getglobalvar("sp_char_ic_outs")); setglobalvar("sp_char_ic_outs", NULL()); }
    if ( getglobalvar("sp_char_credits_fill") != NULL() ) { free(getglobalvar("sp_char_credits_fill")); setglobalvar("sp_char_credits_fill", NULL()); }
    if ( getglobalvar("sp_char_credits_outs") != NULL() ) { free(getglobalvar("sp_char_credits_outs")); setglobalvar("sp_char_credits_outs", NULL()); }
    if ( getglobalvar("sp_char_wait_fill") != NULL() ) { free(getglobalvar("sp_char_wait_fill")); setglobalvar("sp_char_wait_fill", NULL()); }
    if ( getglobalvar("sp_char_wait_outs") != NULL() ) { free(getglobalvar("sp_char_wait_outs")); setglobalvar("sp_char_wait_outs", NULL()); }
}

void clear_color_font() {
    if ( getlocalvar("font_stand_color1") ) { free(getlocalvar("font_stand_color1")); setlocalvar("font_stand_color1",NULL()); }
    if ( getlocalvar("font_stand_color2") ) { free(getlocalvar("font_stand_color2")); setlocalvar("font_stand_color2",NULL()); }
}

/*
 * Se volessi utilizzare gli sprites invece delle fonts dovrei caricarli una volta dopo la pausa e poi catturare il tasto start
 * per liberare la memoria e pulire le variabili.
 */
void check_pause() {
    //drawstring(10,100,0,openborvariant("in_sound_options"));
    if ( openborvariant("game_paused") && !openborvariant("in_options") ) { // game_paused/pause
        int vres = openborvariant("vresolution");
		int hres = openborvariant("hresolution");
		int p = getlocalvar("player_paused");
		void player = getplayerproperty(p, "entity");
        char text1 = "Player  Paused:  ";//+(p+1)+"p";
        int color_shad, color_hi;

        color_hi = get_hi_text_color(p);
        //color_shad = get_shadow_text_color(p);
        color_shad = rgbcolor(0x00,0x00,0x00);

        drawstring((hres/2-strwidth(text1, 0)/2)+1,130,0,text1);
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);
        changedrawmethod(NULL(), "tintmode", 2); //2 or 1
        changedrawmethod(NULL(), "fillcolor", color_shad);
        draw_fontstring((hres/2+strwidth(text1, 0)/2)+1,130,9999,20,(p+1)+"p","data/sprites/special_fonts/font0b.gif",128,128,"font_stand_color2",0);
        changedrawmethod(NULL(), "tintmode", 1);
        changedrawmethod(NULL(), "fillcolor", color_hi);
        draw_fontstring((hres/2+strwidth(text1, 0)/2)+1,130,9999,20,(p+1)+"p","data/sprites/special_fonts/font0a.gif",128,128,"font_stand_color1",0);
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);


        if ( getlocalvar("pause_sound") == NULL() ) {
            int fx = loadsample("data/sounds/nes_pause.wav");

            playsample(fx);
            setlocalvar("pause_showmoves_page",0);
            setlocalvar("pause_sound",1);
        }

        draw_moves(player);
        //drawstring(10,100,3,"[\\]^   <=>"); // [=UP, \\=DOWN, ]=LEFT, ^=RIGHT, <=A, ==B, >=J
    } else if ( openborvariant("game_paused") && openborvariant("in_options") ) {
        int vres = openborvariant("vresolution");
		int hres = openborvariant("hresolution");
		int p = getlocalvar("player_paused");
		void player = getplayerproperty(p, "entity");
        char text1, text2 = "GAME  OPTIONS";
        int color_shad, color_hi;
        int transp = 100;
		int width_box = hres-2, height_box = truncA(vres/2);
		int y_base = truncA(vres/2)-80, y_box = truncA(vres/2)-40;

		// prevent BUG
		/*if ( openborvariant("in_options") && openborvariant("in_sound_options")
            && (openborvariant("in_control_options") || openborvariant("in_video_options") || openborvariant("in_system_options")) ) {
                changeopenborvariant("in_sound_options",0);
		}*/

        //setlocalvar("pause_showmoves_page",0);
        if ( !openborvariant("in_control_options") && !openborvariant("in_sound_options") && !openborvariant("in_video_options") && !openborvariant("in_system_options") ) { //  && !openborvariant("in_sound_options")
            //width_box = 160; height_box = 90; y_box = truncA(vres/2)-10;
            y_base = truncA(vres/2)-50;
        } else if ( openborvariant("in_control_options") ) {
            //width_box = hres-90; height_box = 135; y_box = truncA(vres/2)-59;
            y_base = 32;
        } else if ( openborvariant("in_system_options") ) {
            //width_box = hres-2, height_box = 116; y_box = truncA(vres/2)-46;
        } else if ( openborvariant("in_sound_options") ) {
            //width_box = hres-80;
        } else if ( openborvariant("in_cheat_options") ) {
            //width_box = hres-80;
            y_base = truncA(vres/2)-50;
        }
        width_box = hres-20; height_box = vres - 40; y_box = truncA(vres/2)-90;
        //drawstring(10,50,6,ov("in_control_options")+""+ov("in_sound_options")+""+ov("in_video_options")+""+ov("in_system_options")+""+ov("in_options"));

		text1 = "Player  Paused:  ";//+(p+1)+"p";
		drawstring((hres/2-strwidth(text2, 3)/2)+1,y_base,3,text2);
        drawstring((hres/2-strwidth(text1, 0)/2)+1,y_base+20,0,text1);

        color_hi = get_hi_text_color(p);
        //color_shad = get_shadow_text_color(p);
        color_shad = rgbcolor(0x00,0x00,0x00);

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        changedrawmethod(NULL(), "tintmode", 2); //2 or 1
        changedrawmethod(NULL(), "fillcolor", color_shad);
        draw_fontstring((hres/2+strwidth(text1, 0)/2)+1,y_base+20,9999,20,(p+1)+"p","data/sprites/special_fonts/font0b.gif",128,128,"font_stand_color2",0);
        changedrawmethod(NULL(), "tintmode", 1);
        changedrawmethod(NULL(), "fillcolor", color_hi);
        draw_fontstring((hres/2+strwidth(text1, 0)/2)+1,y_base+20,9999,20,(p+1)+"p","data/sprites/special_fonts/font0a.gif",128,128,"font_stand_color1",0);



        changedrawmethod(NULL(), "alpha", 6);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);
		drawbox((hres/2-width_box/2), y_box , width_box, height_box, 5000, rgbcolor(0x00,0x00,0x00), 6); // x,y,hres,vres

        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);
    } else {
        int p;

        for (p = 0; p < MAX_PLAYERS; ++p) {
            if ( getplayerproperty(p,"keys") == 2 ) { // playkeys
                setlocalvar("player_paused", p);
                //break;
            }
        } // fine for

        if ( getlocalvar("pause_sound") != NULL() ) setlocalvar("pause_sound",NULL());
    } // fine if game_paused
}

int set_hpbar(int p) {
     if ( openborvariant("in_level") > 0 ) {
            if ( getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 ) {
                 // a fine livello chiama free(handle) per liberare spazio dalla memoria
                 //int p;
                 int i = 0, t = 0;
                 int p1x_hpbar=36,p2x_hpbar=118,p3x_hpbar=200,p4x_hpbar=282,p1y_hpbar=18,p2y_hpbar=18,p3y_hpbar=18,p4y_hpbar=18;

                 // Passiamo a rassegna tutti i players
                 //for (p = 0; p < MAX_PLAYERS; ++p) {
                    int hp_x,hp_y;
                    int bar_num;
                    void player = getplayerproperty(p, "entity");

                    // Sistemiamo il punteggio (max 9999)
                    if ( getentityproperty(player, "exists") ) {
                        if ( getplayerproperty(player,"score") > MAX_SCORES ) {
                            changeplayerproperty(player,"score",0);
                        }
                        if ( getplayerproperty(player,"lives") > MAX_LIVES ) {
                            changeplayerproperty(player,"lives",MAX_LIVES);
                        }
                        if ( getplayerproperty(player,"credits") > MAX_CREDITS ) {
                            changeplayerproperty(player,"credits",MAX_CREDITS);
                        }
                    }

                    //if ( getentityproperty(player, "defaultmodel") == "MIKE" ) drawstring( 10, 100,0,"HP =  "+getentityproperty(player,"health") );

                    // Generiamo le barre solo se il player esiste
                    if ( getentityproperty(player, "exists") ) {
                        int lives = getplayerproperty(p, "lives");

                        // Assegnamo le variabili di posizione in base al player
                        if ( p == 0 ) { hp_x = p1x_hpbar; hp_y = p1y_hpbar; }
                        else if ( p == 1 ) { hp_x = p2x_hpbar; hp_y = p2y_hpbar; }
                        else if ( p == 2 ) { hp_x = p3x_hpbar; hp_y = p3y_hpbar; }
                        else if ( p == 3 ) { hp_x = p4x_hpbar; hp_y = p4y_hpbar; }

                        if ( getentityproperty(player, "health") < 0 ) changeentityproperty(player, "health", 0); // preveniamo bugs

                        bar_num = (10)*getentityproperty(player, "health")/( getentityproperty(player, "maxhealth") );
                        //if ( getentityproperty(player, "health") > 0 ) bar_num += 1;
                        if ( getentityproperty(player, "health") < getentityproperty(player, "maxhealth") ) bar_num += 1;
                        if ( getentityproperty(player, "health") <= 0 ) bar_num = 0;

                        if ( getentityproperty(player, "health") <= 0 && lives <= 1 ) lives = 0;
                        drawstring(p*82+15,16,4,lives);
                        drawhpbar(player,bar_num,hp_x,hp_y);

                    } // fine if player exists

                 //} // fine for creazione hpbar

                 /*
                 // Disegnare Palette
                 // Per ottenere il colore si deve invertire a specchio il suo valore esadecimale. Es.: 0,0,255 -> 0xFF0000
                 drawbox (22,220,10, 10, 5000,0xFF0000,0);
                 for (i = 0; i < 16; ++i) {
                      for (t = 0; t < 16; ++t) {
                        drawbox (24+(t*4),24+(i*4),4, 4, 5000,i*t*256*256,0);
                      }
                 }
                 */
            }
     } // fine if in_level
}

void draw_moves(void player) {
		int transp = 100;
        //int vres = openborvariant("vresolution");
		int hres = openborvariant("hresolution");
		int width_box = 280, height_box = 95;

        changedrawmethod(NULL(), "enabled", 1);
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "alpha", 6);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);
		drawbox ((hres/2-width_box/2), 140 , width_box, height_box, 5000, rgbcolor(0x00,0x00,0x00), 6); // x,y,hres,vres
        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);

        show_player_moves(player,width_box,height_box);
}

int drawhpbar(void player, int bar_num, int hp_x, int hp_y) {
    int i;
    char defaultmodel = getentityproperty(player, "defaultmodel");
            // disegnamo le barre a 2 a 2 di colori uguali a seconda del personaggio scelto
            for (i = 0; i < bar_num; ++i) {
                drawbox (hp_x+(i*(4-1)-1),hp_y,4, 11, 5000,rgbcolor(0x00,0x00,0x00),0);
                if ( i >= 0 && i <=1 ) {
                    if ( defaultmodel == "RAPH" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xFF,0x00,0x5B),0);
                    else if ( defaultmodel == "DON" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xAD,0x10,0xC6),0);
                    else if ( defaultmodel == "MIKE" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xCE,0xB5,0x00),0);
                    else if ( defaultmodel == "LEO" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0x00,0x00,0xFF),0);
                    else drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0x00,0x59,0x00),0);
                } else if ( i >= 2 && i <=3 ) {
                    if ( defaultmodel == "RAPH" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xFF,0x9C,0x7B),0);
                    else if ( defaultmodel == "DON" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xC6,0x08C,0xE7),0);
                    else if ( defaultmodel == "MIKE" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xDE,0xCE,0x00),0);
                    else if ( defaultmodel == "LEO" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0x00,0x9C,0xFF),0);
                    else drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0x00,0x87,0x00),0);
                } else if ( i >= 4 && i <=5 ) {
                    if ( defaultmodel == "RAPH" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xFF,0xBD,0x8C),0);
                    else if ( defaultmodel == "DON" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xD6,0x94,0xFF),0);
                    else if ( defaultmodel == "MIKE" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xEF,0xE7,0x00),0);
                    else if ( defaultmodel == "LEO" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0x00,0xBD,0xFF),0);
                    else drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0x00,0xB2,0x00),0);
                } else if ( i >= 6 && i <=7 ) {
                    if ( defaultmodel == "RAPH" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xFF,0xDE,0xA5),0);
                    else if ( defaultmodel == "DON" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xD6,0xB5,0xF7),0);
                    else if ( defaultmodel == "MIKE" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xFF,0xFF,0x94),0);
                    else if ( defaultmodel == "LEO" ) drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xA5,0xDE,0xFF),0);
                    else drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0x67,0xF2,0x67),0);
                } else if ( i >= 8 && i <=9 ) {
                    drawbox (hp_x+(i*(2+1)),hp_y,2, 10, 5001,rgbcolor(0xEF,0xEF,0xEF),0);
                }
            }
}

int get_hi_text_color(int p) {
    void player = getplayerproperty(p,"entity");
    char defaultmodel = getentityproperty(player, "defaultmodel");

    if ( defaultmodel == "RAPH" ) return rgbcolor(0xFF,0x52,0x00);
    else if ( defaultmodel == "DON" ) return rgbcolor(0xFF,0x4A,0xFF);
    else if ( defaultmodel == "MIKE" ) return rgbcolor(0xFD,0xBB,0x00);
    else if ( defaultmodel == "LEO" ) return rgbcolor(0x00,0x94,0xFF);
    else return rgbcolor(0x60,0xB0,0x48);
}

int get_shadow_text_color(int p) {
    void player = getplayerproperty(p,"entity");
    char defaultmodel = getentityproperty(player, "defaultmodel");

    if ( defaultmodel == "RAPH" ) return rgbcolor(0xFF,0x00,0x5B);
    else if ( defaultmodel == "DON" ) return rgbcolor(0xAD,0x10,0xC6);
    else if ( defaultmodel == "MIKE" ) return rgbcolor(0xCE,0xB5,0x00);
    else if ( defaultmodel == "LEO" ) return rgbcolor(0x00,0x00,0xFF);
    else return rgbcolor(0x00,0x59,0x00);
}


