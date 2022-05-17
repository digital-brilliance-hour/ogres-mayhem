
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

int check_continue() {
    if ( no_players_ingame() ) {
        if ( !zero_credits() ) {
            int vres = openborvariant("vresolution");
            int hres = openborvariant("hresolution");
            char text_continue = "CONTINUE";
            int ETA = 20; // secs.

            if ( getglobalvar("countdown_time") != NULL() ) {
                    int countdown_time = getglobalvar("countdown_time");
                    int time = openborvariant("elapsed_time");
                    int game_speed = openborvariant("game_speed");
                    char count = "";
                    int count_num;

                    if ( countdown_time >= time ) {
                        int num = ((countdown_time-time)/game_speed)%ETA;

                        if ( keyc("attack",1) || keyc("jump",1) || keyc("special",1) ) {
                            //changeopenborvariant("elapsed_time",get_next_time(game_speed*1));
                            countdown_time -= game_speed*1 - game_speed/10;
                            setglobalvar("countdown_time",countdown_time);
                        }

                        if ( getglobalvar("prev_music") == NULL() ) {
                            setglobalvar("prev_music",getglobalvar("current_music"));
                            playmusic("data/music/continue.bor",1);
                        }
                        load_continue_nums(); // LOAD sprites if they aren't loaded

                        count = ""+num; // string
                        count_num = num;
                    } else {
                        count = "";
                        text_continue = "";
                        count_num = NULL();
                        setglobalvar("countdown_time",NULL());
                        setglobalvar("prev_music",NULL());
                        if ( getglobalvar("fake_credit") > 0 ) { changeplayerproperty(0,"credits",0); setglobalvar("fake_credit",NULL()); }
                        pausemusic(1);
                        force_game_over(); // <--- GAME_OVER

                    }

                    drawstring((hres/2-strwidth(text_continue,0)/2),60,6,text_continue);
                    draw_countdown(count_num,80);
                    //drawstring((hres/2-strwidth(count,6)/2),75,6,count);
            } else {
                int time = openborvariant("elapsed_time");
                int game_speed = openborvariant("game_speed");

                setglobalvar("countdown_time",get_next_time(ETA*game_speed));
            }
        } else { // no credits
            setglobalvar("countdown_time",NULL());
            setglobalvar("prev_music",NULL());
            //pausemusic(1);
            //force_game_over(); // <--- GAME_OVER

            // Add a fake credits to smulate a not game over!!
            if ( COIN_SYSTEM ) {
                if ( getplayerproperty(0,"credits") <= 0 ) {
                    changeplayerproperty(0,"credits",1);
                    setglobalvar("fake_credit",1);
                }
            }
        } // fine if zero_credits
    } else {
        if ( getglobalvar("countdown_time") != NULL() ) {
            if ( getglobalvar("prev_music") != NULL() ) playmusic(getglobalvar("prev_music"),1);
            setglobalvar("countdown_time",NULL());
            setglobalvar("prev_music",NULL());

            if ( getglobalvar("fake_credit") > 0 ) {
                if ( getplayerproperty(0,"credits") > 0 ) changeplayerproperty(0,"credits",getplayerproperty(0,"credits")-1);
                setglobalvar("fake_credit",NULL());
            }
        }
    } // fine if no_players_ingame
}

int draw_countdown(int num, int y) {
    void spr;
    int i = 0;
    int vres = openborvariant("vresolution");
    int hres = openborvariant("hresolution");
    int mini_hres = 40, mini_vres = 72, big_hres = 64, big_vres = 72;
    float x = 0, z = 5000, sortid = 0, xshift; //y = 80;
    int tmp_num = num, digit_num;

    if ( num < 0 || num == NULL() ) return -1;
    digit_num = digit_num(num);

    tmp_num = num;
    if ( tmp_num == 0 ) {
        int digit = 0; // ex. 526%100 = 26

            if ( num >= 10 ) spr = getglobalvar("continue_num_"+digit);
            else spr = getglobalvar("continue_big_num_"+digit);
            if ( spr != NULL() ) {
                // Allineamento al centro
                if ( num >= 10 ) {
                    xshift = mini_hres;
                    x = (hres/2)-(mini_hres/2)*digit_num;
                } else {
                    xshift = big_hres;
                    x = (hres/2)-(big_hres/2)*digit_num;
                }
                drawsprite(spr,x+xshift*i,y,z,sortid);
            }
    } else {
        i = 0;
        while( tmp_num > 0 ) {
            int digit;

            digit = truncA(tmp_num%10); // ex. 526%100 = 26
            tmp_num = truncA(tmp_num/10); // ex. 526/100 = 5

            //drawstring(10,100,0,"DIGITS: "+digit_num(num));
            if ( num >= 10 ) spr = getglobalvar("continue_num_"+digit);
            else spr = getglobalvar("continue_big_num_"+digit);
            if ( spr != NULL() ) {
                // Allineamento al centro
                if ( num >= 10 ) {
                    xshift = mini_hres;
                    x = (hres/2)-(mini_hres/2)*digit_num;
                } else {
                    xshift = big_hres;
                    x = (hres/2)-(big_hres/2)*digit_num;
                }
                drawsprite(spr,x+xshift*(digit_num-(i+1)),y,z,sortid);
            }
            ++i;
        } // fine while
    } // fine if num == 0 ?
}

void load_continue_nums() {
    if ( getglobalvar("continue_num_0") == NULL() ) {
        int i;

        for (i = 0; i < 10; ++i) {
            char path_mini = "data/sprites/special_characters/continue_"+i+".gif";
            char path_big = "data/sprites/special_characters/continue_big_"+i+".gif";
            void spr_mini = loadsprite(path_mini);
            void spr_big = loadsprite(path_big);

            setglobalvar("continue_num_"+i, spr_mini);
            setglobalvar("continue_big_num_"+i, spr_big);
        }
    } // fine if check NULL()
}

void unload_continue_nums() {
    int i;

    for (i = 0; i < 10; ++i) {
        if ( getglobalvar("continue_num_"+i) != NULL() ) { free(getglobalvar("continue_num_"+i)); setglobalvar("continue_num_"+i, NULL()); }
        if ( getglobalvar("continue_big_num_"+i) != NULL() ) { free(getglobalvar("continue_big_num_"+i)); setglobalvar("continue_big_num_"+i, NULL()); }
    }
}

int no_players_ingame() {
    if ( openborvariant("in_level") ) { // force game_over
        if ( getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 && openborvariant("count_players") <= 0 ) {
            int p = 0, no_players = 0;

            for (p = 0; p < MAX_PLAYERS; p++) {
                char name = getplayerproperty(p, "name");

                if ( name != "" ) ++no_players;
            }

            if ( no_players <= 0 ) {
                return 1;
            }
        }
    }

    return 0;
}

int zero_credits() {
    int p = 0;

    for (p = 0; p < MAX_PLAYERS; p++) {
        if ( getplayerproperty(p,"credits") > 0 ) return 0;
    }

    return 1;
}

int force_game_over() {
    if ( no_players_ingame() ) {
        int p = 0;

        for (p = 0; p < MAX_PLAYERS; p++) {
            if ( getplayerproperty(p,"credits") > 0 ) changeplayerproperty(p,"credits",0);
            if ( getplayerproperty(p,"lives") > 0 ) changeplayerproperty(p,"lives",0);
        }
        jumptobranch("inexistent ",1); // inexistent branch to simulate gameover
    }
}

