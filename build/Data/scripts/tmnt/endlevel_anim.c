
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_movements.c"

void main() {
  void self = getlocalvar("self");

/*
 * Usiamo le entityvar come semafori. Gli indici dallo 0..3 indicano i passi per ogni giocatore. es. getentityvar(player,0,0) indica il passo 0 del giocatore 1 (index 0)
 * L'indice 4 della entityvar indica il passo generale che si avvia solo se tutti i giocatori presenti sullo schermo sono preparati!
 * Si capisce quindi che il primo passo generale è con la variabile a indice 4 == NULL() e finirà con valore uguale al numero_giocatori_correnti.
 * Il secondo passso finirà con numero_giocatori_correnti*2!! il terzo numero_giocatori_correnti*3 ecc...
 */

    //drawstring( 20,180,0,"ANIMATION: " );

    set_invincibility(ov("game_speed")*1);
    if ( (getlocalvar("endlevel_semaphore") > 1 && getlocalvar("player_count_mem") != openborvariant("count_players")) || getlocalvar("reset_animation") == 1 ) { // Se cambia d'un tratto il numero di giocatori resettiamo l'animazione
            int p, dying_flag;

            //clearlocalvar();
            setlocalvar("player_count_mem", openborvariant("count_players"));
            //setmusictempo(0);
            //fademusic(200);
            pausemusic();
            setlocalvar("endlevel_semaphore", 2);
            if ( getlocalvar("comic_spawned") ) killentity(getlocalvar("comic_spawned"));
            setlocalvar("comic_spawned",NULL());
            clear_animsteps(self, 1);

            setlocalvar("music_vol", NULL());
            setlocalvar("music_fade", NULL());

            if ( openborvariant("count_players") <= 0 ) {
                setlocalvar("reset_animation",1);
                return;
            }
            // check for dying players
            for ( p = 0; p < MAX_PLAYERS; ++p ) {
                void player = getplayerproperty(p,"entity");
                dying_flag = 0;

                if ( getentityproperty(player,"exists") ) {
                        if ( getentityproperty(player,"aiflag","dead") ) {
                            setlocalvar("reset_animation",1);
                            //return;
                            dying_flag = 1;
                            continue;
                        }

                        changeplayerproperty(player, "weapon",0);
                        changeentityproperty(player, "antigravity", 0);
                        changeentityproperty(player, "subject_to_screen", 0);
                        changeentityproperty(player, "velocity", 0,0,0);
                        changeentityproperty(player, "position",NULL(),NULL(),getentityproperty(player,"base"));
                        if ( getentityproperty(player,"animationid") != openborconstant("ANI_IDLE") && !getentityproperty(player,"aiflag","dead") ) setidle(player,openborconstant("ANI_IDLE"),1);
                }
                if ( p+1 >= MAX_PLAYERS && dying_flag ) return;
            }

            /*for ( p = 0; p < MAX_PLAYERS; ++p ) {
                void player = getplayerproperty(p,"entity");

                if ( getentityproperty(player,"exists") ) {
                    if ( getentityproperty(player,"x") <= ((xpos+hres)/2)+p*20 ) ++count;
                }
            }
            if ( count != openborvariant("count_players") ) setlocalvar("player_count_mem", 0);*/ // controlla la posizione. se non sono tutti ok ripeti questa funzione!! La ripeteremo perchè azzeriamo il flag!!

            setlocalvar("reset_animation",NULL());
    }

    // Check Time-Reset
    if ( check_stored_time(getlocalvar("wait"),9999) ) setlocalvar("wait", openborconstant("MIN_INT"));
    if ( getlocalvar("endlevel_semaphore") == NULL() ) { // STEP 1: MUSIC FADE #####################
            if ( getlocalvar("music_fade") == NULL() ) {
                int vol_dec = 0.2;

                if ( getlocalvar("music_vol") == NULL() ) setlocalvar("music_vol",openborvariant("musicvol"));

                setlocalvar("music_vol",getlocalvar("music_vol")-vol_dec);
                if ( getlocalvar("music_vol") < 0 ) setlocalvar("music_vol",0);

                setmusicvolume(getlocalvar("music_vol"),getlocalvar("music_vol"));
                if ( getlocalvar("music_vol") <= 0 ) setlocalvar("music_fade",1);
                //fademusic(0.8);
            } else {
                pausemusic();
                setlocalvar("wait", get_next_time(openborvariant("game_speed")*0.5));
                setlocalvar("player_count_mem", openborvariant("count_players"));
                //changeopenborvariant("nojoin", 1);
                setlocalvar("endlevel_semaphore", 1);
            }
    } else if ( getlocalvar("endlevel_semaphore") == 1 && getlocalvar("wait") < openborvariant("elapsed_time") ) { // STEP 2: WAIT FADE #####################
        int i,p,dying_flag;

            if ( openborvariant("count_players") <= 0 ) {
                setlocalvar("reset_animation",1);
                return;
            }
            // check for dying players
            for ( p = 0; p < MAX_PLAYERS; ++p ) {
                void player = getplayerproperty(p,"entity");
                dying_flag = 0;

                if ( getentityproperty(player,"exists") ) {
                        if ( getentityproperty(player,"aiflag","dead") ) {
                            setlocalvar("reset_animation",1);
                            //return;
                            dying_flag = 1;
                            continue;
                        }

                        changeplayerproperty(player, "weapon",0);
                        changeentityproperty(player, "antigravity", 0);
                        //changeentityproperty(player, "subject_to_wall", 0);
                        changeentityproperty(player, "subject_to_screen", 0);
                        changeentityproperty(player, "velocity", 0,0,0);
                        changeentityproperty(player, "position",NULL(),NULL(),getentityproperty(player,"base"));
                        if ( getentityproperty(player,"animationid") != openborconstant("ANI_IDLE") && !getentityproperty(player,"aiflag","dead") ) setidle(player,openborconstant("ANI_IDLE"),1);
                }
                if ( p+1 >= MAX_PLAYERS && dying_flag ) return;
            }

            changeopenborvariant("waiting", 0); // IMPORTANT!! Screen can scroll!

            // check for all enemies destroied
            for ( i = 0; i < openborvariant("count_entities"); ++i ) { // openborconstant("MAX_ENTS")
                    void ent = getentity(i);
                    char defaultname = getentityproperty(ent,"defaultname");

                    if ( !getentityproperty(ent,"exists") ) continue;
                    if ( ent == self ) continue;
                    if ( defaultname == "wait_boss1_death" ) continue;

                    if ( getentityproperty(ent,"type") == openborconstant("TYPE_ENEMY") ) {
                        int health = getentityproperty(ent,"health");
                        int anim_id = getentityproperty(ent,"animationid");

                        if ( getentityproperty(ent,"aiflag","dead") || health <= 0 ) continue;

                        //damageentity(ent,ent,health,1,openborconstant("ATK_NORMAL"));
                        killentity(ent);
                    }
            }
        set_noaicontrol(); // CHECK ANTI-BUGS
        changeopenborvariant("nojoin", 1); // <--------------
        changeopenborvariant("nopause", 1);
        setlocalvar("endlevel_semaphore", 2);
    } else if ( getlocalvar("endlevel_semaphore") == 2 ) {  // STEP 3: MOVE PLAYERS TO CENTER #####################
        float level_width = openborvariant("levelwidth");
        float level_height = openborvariant("levelheight");
        int minz = openborvariant("player_min_z");
        int maxz = openborvariant("player_max_z");
        float xpos = openborvariant("xpos");
        float ypos = openborvariant("ypos");
        float hres = openborvariant("hresolution");
        float vres = openborvariant("vresolution");
        int count_players = openborvariant("count_players");
        float xfinal, zfinal;
        int p, player_num = 0, final_dir;
        int all_turtles_flag = areAllTurtles();

        if ( ggv("1VS1") != NULL() && ggv("1VS1") > 0 ) {
            float w_add = 0, h_add = 0;
            float width_add = getentityvar(self,"level_width_add");
            float height_add = getentityvar(self,"level_height_add");

            if ( width_add != NULL() ) w_add = width_add;
            if ( height_add != NULL() ) h_add = height_add;

            hres = truncA( level_width  + w_add );
            vres = truncA( level_height + h_add );
            xpos = 0;
            ypos = 0;
        }

        if ( count_players <= 0 ) {
            setlocalvar("reset_animation",1);
            return;
        }

        xfinal = (xpos+hres)/2 - (count_players-1)*10;
        zfinal = ((ypos+vres)/2)+80; // x=160, z=200

        if ( count_players == 2 && all_turtles_flag ) xfinal = (xpos+hres)/2 - 30;

        for ( p = 0; p < MAX_PLAYERS; ++p ) {
           void player = getplayerproperty(p,"entity");

           if ( !getentityproperty(player,"exists") ) continue;
           ++player_num;

            if ( getentityproperty(player,"aiflag","dead") ) {
                setlocalvar("reset_animation",1);
                return;
            }

           changeentityproperty(player, "noaicontrol", 1);
           bindentity(player, NULL());
           //changelevelproperty("cameraxoffset", 100);

           // ############ MUOVIAMO I PERSONAGGI #############
           if ( count_players == 1 ) {
             final_dir = 1;
           } else if ( count_players == 2 && all_turtles_flag ) {
            if ( player_num == 1 ) {
                xfinal -= 20;
                final_dir = 1;
            } else {
                xfinal += 82;
                zfinal -= 10;
                final_dir = 0;
            }
           } else {
                if ( player_num%2 == 1 ) { // se è dispari
                    xfinal -= 55*(player_num-1);
                    final_dir = 1;
                } else {
                    xfinal += 55*(player_num-1);
                    final_dir = 0;
                }
           } // fine if player count

            if ( get_animstep(self,p) == NULL() ) {
                if ( movetoxz(player,xfinal,zfinal,final_dir) ) {
                    //changeentityproperty(player, "noaicontrol", 0);
                    if ( getentityproperty(player,"direction") != final_dir ) changeentityproperty(player,"direction",final_dir);
                    if ( get_globalanimstep(self, 0) == NULL() ) set_globalanimstep(self,0,1);
                    else set_globalanimstep(self,0,get_globalanimstep(self, 0)+1); // si somma 1 per ogni giocatore che ha concluso la sua path

                    set_animstep(self,p,0); // passo 0 per il giocatore p
                } // fine step (passo) 0 per tutti
            }

            if ( get_globalanimstep(self,0) == count_players ) setlocalvar("endlevel_semaphore", 3);
           // ############ FINE MUOVIAMO I PERSONAGGI #############
        } // fine for

        //playmusic("data/music/complete.bor", 0); // Deve essere eseguita 1 volta sola questa func!!
    } else if ( getlocalvar("endlevel_semaphore") == 3 ) {  // STEP 4: WAIT #####################
        if ( ggv("1VS1") != NULL() && ggv("1VS1") > 0 ) {
            void ent_1VS1_winner_msg = getentityvar(self,"1VS1_winner_msg");

            if ( ent_1VS1_winner_msg != NULL() && getentityproperty(ent_1VS1_winner_msg,"exists") ) {
                setentityvar(ent_1VS1_winner_msg,"ok_winner_msg_flag",1);
            }
        }

        setmusicvolume(openborvariant("musicvol"),openborvariant("musicvol"));
        if ( ggv("1VS1") ) { playmusic("data/music/endlevel01.bor", 0); setglobalvar("current_music","data/music/endlevel01.bor"); }
        else { playmusic("data/music/complete1.bor", 0); setglobalvar("current_music","data/music/complete1.bor"); }

        setlocalvar("wait", get_next_time(openborvariant("game_speed")*2));

        setlocalvar("endlevel_semaphore", 4);
    } else if ( getlocalvar("endlevel_semaphore") == 4 && getlocalvar("wait") < openborvariant("elapsed_time") ) { // STEP 5: PERFORM VICTORY POSE #####################
        int p, player_num = 0;
        int all_turtles_flag = areAllTurtles();
        int atleast_a_turtle_flag = atLeastATurtle();

           if ( atleast_a_turtle_flag && getlocalvar("comic_spawned") == NULL() ) {
               void comic;
               clearspawnentry();
               setspawnentry("name", "cowabunga");
               comic = spawn();
               setlocalvar("comic_spawned",comic);
           }
           if ( openborvariant("count_players") == 1 ) {
                //drawstring(10,100,0,"esisto: "+getlocalvar("endlevel_semaphore"));
                for ( p = 0; p < MAX_PLAYERS; ++p ) {
                   void player = getplayerproperty(p,"entity");

                    if ( !getentityproperty(player,"exists") ) continue;

                    if ( getentityproperty(player,"aiflag","dead") ) {
                        setlocalvar("reset_animation",1);
                        return;
                    }

                    if ( getlocalvar("comic_spawned") ) changeentityproperty(getlocalvar("comic_spawned"), "parent", player);
                    if ( getentityproperty(player,"animvalid",openborconstant("ANI_FOLLOW25")) && isATurtle(player) ) performattack(player, openborconstant("ANI_FOLLOW25"), 1);
                    else performattack(player, openborconstant("ANI_IDLE"), 1);
                } // fine for
           } else if ( openborvariant("count_players") == 2 && all_turtles_flag ) {
                for ( p = 0; p < MAX_PLAYERS; ++p ) {
                   void player = getplayerproperty(p,"entity");

                   if ( !getentityproperty(player,"exists") ) continue;
                   if ( isATurtle(player) ) ++player_num;

                   if ( player_num == 1 && getlocalvar("comic_spawned") ) changeentityproperty(getlocalvar("comic_spawned"), "parent", player);

                   if ( player_num == 1 ) performattack(player, openborconstant("ANI_FOLLOW26"), 1);
                   else performattack(player, openborconstant("ANI_FOLLOW27"), 1);
                } // fine for
           } else {
                for ( p = 0; p < MAX_PLAYERS; ++p ) {
                   void player = getplayerproperty(p,"entity");

                   if ( !getentityproperty(player,"exists") ) continue;
                   if ( isATurtle(player) ) ++player_num;

                   if ( player_num == 1 && getlocalvar("comic_spawned") ) {
                        changeentityproperty(getlocalvar("comic_spawned"), "parent", player);
                        //++player_num;
                   }

                   if ( getentityproperty(player,"animvalid",openborconstant("ANI_FOLLOW25")) && isATurtle(player) ) performattack(player, openborconstant("ANI_FOLLOW25"), 1);
                   else performattack(player, openborconstant("ANI_IDLE"), 1);
                } // fine for
           }

        setlocalvar("endlevel_semaphore", 5);
        setlocalvar("wait", get_next_time(openborvariant("game_speed")*2));
    } else if ( getlocalvar("endlevel_semaphore") == 5 && getlocalvar("wait") < openborvariant("elapsed_time") ) {
        setlocalvar("comic_spawned",NULL());
        if ( ggv("1VS1") == NULL() || ggv("1VS1") == 0 ) {
            changeopenborvariant("nojoin", 0);
            changeopenborvariant("nopause", 0);
            killentity(self);
        }
    }
}

int areAllTurtles() {
    int cc, count = 0;

    for ( cc = 0; cc < MAX_PLAYERS; ++cc ) {
           void player = getplayerproperty(cc,"entity");
           void defaultmodel = getentityproperty(player, "defaultmodel");

           if ( !getentityproperty(player,"exists") ) continue;

           if ( defaultmodel == "RAPH" || defaultmodel == "LEO" || defaultmodel == "DON" || defaultmodel == "MIKE" ) {
               ++count;
           }
    }

    if ( count == openborvariant("count_players") ) return 1;
    else return 0;
}

int atLeastATurtle() {
    int cc, count = 0;

    for ( cc = 0; cc < MAX_PLAYERS; ++cc ) {
           void player = getplayerproperty(cc,"entity");
           void defaultmodel = getentityproperty(player, "defaultmodel");

           if ( !getentityproperty(player,"exists") ) continue;

           if ( defaultmodel == "RAPH" || defaultmodel == "LEO" || defaultmodel == "DON" || defaultmodel == "MIKE" ) {
               ++count;
           }
    }

    if ( count >= 1 ) return 1;
    else return 0;
}

int isATurtle(void player) {
    void defaultmodel = getentityproperty(player, "defaultmodel");

    if ( defaultmodel == "RAPH" || defaultmodel == "LEO" || defaultmodel == "DON" || defaultmodel == "MIKE" ) {
        return 1;
    } else return 0;
}

int set_noaicontrol() {
    int cc;

    for ( cc = 0; cc < MAX_PLAYERS; ++cc ) {
           void player = getplayerproperty(cc,"entity");
           float base;

           if ( !getentityproperty(player,"exists") ) continue;

           base = getentityproperty(player,"base");
           if ( getentityproperty(player, "noaicontrol") != 1 ) changeentityproperty(player, "noaicontrol", 1);
           changeentityproperty(player,"position",NULL(),NULL(),base);
           if ( !getentityproperty(player,"aiflag","dead") ) setidle(player,openborconstant("ANI_IDLE"),1);
           changeentityproperty(player, "velocity", 0,0,0);
           changeentityproperty(player, "aiflag", "running", 0);
           changeentityproperty(player, "aiflag", "jumping", 0);
    }
}

int set_invincibility(int ETA, int blink_flag) {
    int p;

    for ( p = 0; p < MAX_PLAYERS; ++p ) {
        void player = getplayerproperty(p,"entity");

        if ( getentityproperty(player,"exists") ) {
            if ( !is_in_pain(player) && !getentityproperty(player,"invincible") ) {
                changeentityproperty(player, "invincible", 1);
                changeentityproperty(player, "invinctime", get_next_time(ETA));
                if ( blink_flag > 0 ) changeentityproperty(player, "aiflag", "blink", 1);
            }
        }
    }
}

int clear_animsteps(void self, int num) {
    int p;

    for ( p = 0; p < 4+1+num; ++p ) {
        setentityvar(self,p,NULL());
        setlocalvar("start_pos_x"+p, NULL());
        setlocalvar("start_pos_z"+p, NULL());
        setlocalvar("half_jump"+p, NULL());
    }
}

int get_animstep(void self, int p) {

    return getentityvar(self,p);
}

int set_animstep(void self, int p, int step) {

    setentityvar(self,p,step);
}

int get_globalanimstep(void self, int p) {

    return getentityvar(self,4+p);
}

int set_globalanimstep(void self, int p, int step) {

    setentityvar(self,4+p,step);
}

