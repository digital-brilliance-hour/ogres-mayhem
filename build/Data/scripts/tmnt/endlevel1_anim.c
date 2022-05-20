
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_movements.c"
#import "data/scripts/endlevel_anim.c"
#import "data/scripts/player_commons.c"

/*
cancellare riga: 50(if), e player_script
*/

void main() {
  void self = getlocalvar("self");

/*
 * Usiamo le entityvar come semafori. Gli indici dallo 0..3 indicano i passi per ogni giocatore. es. getentityvar(player,0,0) indica il passo 0 del giocatore 1 (index 0)
 * L'indice 4 della entityvar indica il passo generale che si avvia solo se tutti i giocatori presenti sullo schermo sono preparati!
 * Si capisce quindi che il primo passo generale è con la variabile a indice 4 == NULL() e finirà con valore uguale al numero_giocatori_correnti.
 * Il secondo passso finirà con numero_giocatori_correnti*2!! il terzo numero_giocatori_correnti*3 ecc...
 */


    //drawstring( 20,180,0,"semaphore: "+getlocalvar("endlevel_semaphore") );
    //drawstring( 20,180,0,"ANIMATION: " );
    //drawstring( 20,180,0,"ANIMATION: "+getentityproperty(getplayerproperty(0,"ent"), "x") );

    set_invincibility(ov("game_speed")*1);
    if ( (getlocalvar("endlevel_semaphore") > 1 && getlocalvar("player_count_mem") != openborvariant("count_players")) || getlocalvar("reset_animation") == 1 ) { // Se cambia d'un tratto il numero di giocatori resettiamo l'animazione
            int p,dying_flag, count = 0;

            //clearlocalvar();
            setlocalvar("player_count_mem", openborvariant("count_players"));
            //setmusictempo(0);
            //fademusic(200);
            pausemusic();
            setlocalvar("endlevel_semaphore", 2);
            if ( getlocalvar("comic_spawned") ) killentity(getlocalvar("comic_spawned"));
            setlocalvar("comic_spawned",NULL());
            clear_animsteps(self, 5);

            changelevelproperty("cameraxoffset", 0);
            changelevelproperty("camerazoffset", -120);
            changelevelproperty("scrollspeed", 1);

            setlocalvar("surfes", NULL());
            setlocalvar("music_vol", NULL());
            setlocalvar("music_fade", NULL());

            if ( openborvariant("count_players") <= 0 ) {
                setlocalvar("reset_animation",1);
                return;
            }
            // check for dying players
            for ( p = 0; p < MAX_PLAYERS; ++p ) {
                void surf = getlocalvar("surf"+p);
                void player = getplayerproperty(p,"entity");
                dying_flag = 0;

                if ( getentityproperty(surf,"exists") ) killentity(surf);
                setlocalvar("surf"+p, NULL());

                if ( getentityproperty(player,"exists") ) {
                        if ( getentityproperty(player,"aiflag","dead") ) {
                            setlocalvar("reset_animation",1);
                            //return;
                            dying_flag = 1;
                            continue;
                        }

                        changeplayerproperty(player, "weapon",0); // <---------- ADDED
                        changeentityproperty(player, "antigravity", 0);
                        changeentityproperty(player, "velocity", 0,0,0);
                        changeentityproperty(player, "position",NULL(),NULL(),getentityproperty(player,"base"));
                        if ( getentityproperty(player,"animationid") != openborconstant("ANI_IDLE") && !getentityproperty(player,"aiflag","dead") ) setidle(player,openborconstant("ANI_IDLE"),1);
                        //changeentityproperty(player, "subject_to_wall", 0);
                        changeentityproperty(player, "subject_to_screen", 0);
                        changeentityproperty(player, "position", 5465+p*20, 800, NULL()); // antiwall??
                }
                if ( p+1 >= MAX_PLAYERS && dying_flag ) return;
            }

            for ( p = 0; p < MAX_PLAYERS; ++p ) {
                void player = getplayerproperty(p,"entity");

                if ( getentityproperty(player,"exists") ) {
                    if ( getentityproperty(player,"x") <= 5465+p*20 ) ++count;
                }
            }
            if ( count != openborvariant("count_players") ) setlocalvar("player_count_mem", 0); // controlla la posizione. se non sono tutti ok ripeti questa funzione!! La ripeteremo perchè azzeriamo il flag!!
            //else return;

            setlocalvar("reset_animation",NULL());
    }

        // Check Time-Reset
        if ( check_stored_time(getlocalvar("wait"),9999) ) setlocalvar("wait", openborconstant("MIN_INT"));
    //if ( getentityproperty(getplayerproperty(0,"ent"),"x")>5420 ) { // <--------------- CANCELLA
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
                changelevelproperty("camerazoffset", -120);
                setlocalvar("wait", get_next_time(openborvariant("game_speed")*0.5));
                setlocalvar("player_count_mem", openborvariant("count_players"));
                //changeopenborvariant("nojoin", 1);
                setlocalvar("endlevel_semaphore", 1);
            }
        } else if ( getlocalvar("endlevel_semaphore") == 1 && getlocalvar("wait") < openborvariant("elapsed_time") ) { // STEP 2: WAIT FADE #####################
            int p, i, dying_flag;

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

                        changeplayerproperty(player, "weapon",0); // <---------- ADDED
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
            //changeopenborvariant("player_max_z", 900);

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
            changeopenborvariant("nojoin", 1);
            changeopenborvariant("nopause", 1);
            setlocalvar("endlevel_semaphore", 2);
            //setlocalvar("endlevel_semaphore", 6);
        } else if ( getlocalvar("endlevel_semaphore") == 2 ) {  // STEP 3: MOVE PLAYERS #####################
            int count_players = openborvariant("count_players");
            float xpos = 5465, zpos = 850;
            int p, player_num = 0, final_dir;
            int all_turtles_flag = areAllTurtles();

            if ( count_players <= 0 ) {
                setlocalvar("reset_animation",1);
                return;
            }

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
                    xpos -= 20;
                    final_dir = 1;
                } else {
                    xpos += 82;
                    zpos -= 10;
                    final_dir = 0;
                }
               } else {
                    if ( player_num%2 == 1 ) { // se è dispari
                        xpos -= 55*(player_num-1);
                        final_dir = 1;
                    } else {
                        xpos += 55*(player_num-1);
                        final_dir = 0;
                    }
               } // fine if player count

                if ( get_animstep(self,p) == NULL() ) {
                    if ( movetoxz(player,xpos,zpos,final_dir) ) {
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
                setmusicvolume(openborvariant("musicvol"),openborvariant("musicvol"));
                playmusic("data/music/complete1.bor", 0);
                setglobalvar("current_music","data/music/complete1.bor");
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
                            ++player_num;
                       }
                       if ( getentityproperty(player,"animvalid",openborconstant("ANI_FOLLOW25")) && isATurtle(player) ) performattack(player, openborconstant("ANI_FOLLOW25"), 1);
                       else performattack(player, openborconstant("ANI_IDLE"), 1);
                    } // fine for
               }

            setlocalvar("endlevel_semaphore", 5);
            setlocalvar("wait", get_next_time(openborvariant("game_speed")*2));
        } else if ( getlocalvar("endlevel_semaphore") == 5 && getlocalvar("wait") < openborvariant("elapsed_time") ) { // STEP 5: WAIT + CAMERA ADJUST #####################
            changelevelproperty("camerazoffset", -60);
            setlocalvar("endlevel_semaphore", 6);
        } else if ( getlocalvar("endlevel_semaphore") == 6 && getlocalvar("wait") < openborvariant("elapsed_time") ) { // STEP 6: CAMERA SHIFT + SURF GENERATION #####################
            int time = openborvariant("elapsed_time");
            float xcam = getlevelproperty("cameraxoffset");
            float zcam = getlevelproperty("camerazoffset");
            int p, player_num = 0;

            for ( p = 0; p < MAX_PLAYERS; ++p ) { // Giriamo i players a destra
                void player = getplayerproperty(p,"entity");

                if ( !getentityproperty(player,"exists") ) continue;

                if ( getentityproperty(player,"aiflag","dead") ) {
                    setlocalvar("reset_animation",1);
                    return;
                }

                if ( !getentityproperty(player,"aiflag","dead") ) setidle(player, openborconstant("ANI_IDLE"), 1);
                changeentityproperty(player, "direction", 1);
            } // fine for

            if ( getlocalvar("surfes") == NULL() ) { // GENERIAMO I SURFES
                for ( p = 0; p < MAX_PLAYERS; ++p ) {
                    void player = getplayerproperty(p,"entity");
                    void surf;
                    char turtle_surf = "surf", foot_surf = "esurf", surf_name;
                    char defaultmodel = getentityproperty(player,"defaultmodel");

                    if ( !getentityproperty(player,"exists") ) continue;
                    ++player_num;

                    if ( isATurtle(player) ) surf_name = turtle_surf;
                    else surf_name = foot_surf;

                    surf = spawnsubentity(surf_name, 5740+(player_num-1)*30, 750-2+(player_num-1)*30, NULL());
                    changeentityproperty(surf, "setlayer", getentityproperty(player,"setlayer")-1);
                    setentityvar(surf,"scene",1);
                    changeentityproperty(surf, "parent", player);
                    if ( isATurtle(player) ) set_board_map(player, surf);
                    changeentityproperty(surf, "direction", 1);
                    setlocalvar("surf"+p, surf);
                } // fine for
                setlocalvar("surfes",1);
            } // fine if surfes

            changelevelproperty("cameraxoffset", xcam+1);
            if ( zcam > -5 ) changelevelproperty("camerazoffset", zcam-1);
            if ( xcam > 70 && zcam < -5 ) setlocalvar("endlevel_semaphore", 7);
        } else if ( getlocalvar("endlevel_semaphore") == 7 && getlocalvar("wait") < openborvariant("elapsed_time") ) { // STEP 7: MOVE TO SURF #####################
            int time = openborvariant("elapsed_time");
            int p, player_num = 0;

            for ( p = 0; p < MAX_PLAYERS; ++p ) { // Giriamo i players a destra
                void player = getplayerproperty(p,"entity");
                float xpos = 5600, zpos = 780;

                if ( !getentityproperty(player,"exists") ) continue;
                ++player_num;

                if ( getentityproperty(player,"aiflag","dead") ) {
                    setlocalvar("reset_animation",1);
                    return;
                }

                if ( get_animstep(self,p) == 0 ) {
                    if ( movetoxz(player,xpos+(player_num-1)*30,zpos+(player_num-1)*30,1) ) {
                        //changeentityproperty(player, "noaicontrol", 0);
                        if ( get_globalanimstep(self, 1) == NULL() ) set_globalanimstep(self,1,1);
                        else set_globalanimstep(self,1,get_globalanimstep(self, 1)+1); // si somma 1 per ogni giocatore che ha concluso la sua path

                        set_animstep(self,p,1); // passo 1 per il giocatore p
                    } // fine step (passo) 0 per tutti
                }
            } // fine for

            if ( get_globalanimstep(self,1) == openborvariant("count_players") ) setlocalvar("endlevel_semaphore", 8);
        } else if ( getlocalvar("endlevel_semaphore") == 8 && getlocalvar("wait") < openborvariant("elapsed_time") ) { // STEP 8: JUMP TO SURF #####################
            int p, player_num = 0;

            for ( p = 0; p < MAX_PLAYERS; ++p ) { // Giriamo i players a destra
                void player = getplayerproperty(p,"entity");
                float xpos, zpos;
                void surf;
                float jump_height;
                int defaultmodel = getentityproperty(player,"defaultmodel");

                if ( !getentityproperty(player,"exists") ) continue;

                if ( getentityproperty(player,"aiflag","dead") ) {
                    setlocalvar("reset_animation",1);
                    return;
                }

                surf = getlocalvar("surf"+p);
                xpos = getentityproperty(surf, "x");
                zpos = getentityproperty(surf, "z");
                jump_height = getentityproperty(player, "jumpheight");

                if ( get_animstep(self,p) == 1 ) {
                    //if ( getentityproperty(player, "subject_to_wall") != 0 ) changeentityproperty(player, "subject_to_wall", 0);

                    /*
                    if ( getlocalvar("jumpdelay_anim"+p) == NULL() && getentityproperty(player,"animvalid",openborconstant("ANI_JUMPDELAY")) ) {
                        performattack(player,openborconstant("ANI_JUMPDELAY"),1);
                        setlocalvar("jumpdelay_anim"+p,1);
                    }
                    if ( getlocalvar("jumpdelay_anim"+p) == 1 && getentityproperty(player,"animvalid",openborconstant("ANI_JUMPDELAY")) && getentityproperty(player,"animationid") == openborconstant("ANI_JUMPDELAY") ) continue;
                    */

                    //if ( jumptoxz2(player,xpos+3,zpos+2,100,1) ) {
                    //drawstring(10,180+gep(player,"playerindex")*10,0,gep(player,"name")+": "+gep(player,"aiflag","idling"));
                    if ( jumptoxz_bezier_anim(player,xpos-0,zpos+2,150,1,0.006,1) ) {
                        //changeentityproperty(player, "noaicontrol", 0);
                        if ( get_globalanimstep(self, 2) == NULL() ) set_globalanimstep(self,2,1);
                        else set_globalanimstep(self,2,get_globalanimstep(self, 2)+1); // si somma 1 per ogni giocatore che ha concluso la sua path

                        bindentity(surf, player, 3, -2, NULL(), 1);

                        /*if ( defaultmodel == "LEO" || defaultmodel == "MIKE" || defaultmodel == "DON" || defaultmodel == "RAPH" ) {
                            if ( getentityproperty(player, "animationid") != openborconstant("ANI_JUMPLAND") ) changeentityproperty(player, "animation", openborconstant("ANI_JUMPLAND"));
                        } else {
                            //if ( getentityproperty(player, "animationid") != openborconstant("ANI_IDLE") ) changeentityproperty(player, "animation", openborconstant("ANI_IDLE"));
                            if ( getentityproperty(player, "animationid") != openborconstant("ANI_IDLE") && !getentityproperty(player,"aiflag","dead") ) setidle(player,openborconstant("ANI_IDLE"),1);
                        }*/

                        set_animstep(self,p,2); // passo 1 per il giocatore p
                    } // fine step (passo) 0 per tutti
                } else if ( get_animstep(self,p) == 2 ) {
                        /*
                        if ( getlocalvar("jumpland_anim"+p) == NULL() && getentityproperty(player,"animvalid",openborconstant("ANI_JUMPLAND")) ) {
                            performattack(player,openborconstant("ANI_JUMPLAND"),1);
                            setlocalvar("jumpland_anim"+p,1);
                        }
                        if ( getlocalvar("jumpland_anim"+p) == 1 && getentityproperty(player,"animvalid",openborconstant("ANI_JUMPLAND")) && getentityproperty(player,"animationid") == openborconstant("ANI_JUMPLAND") ) continue;
                        */

                        if ( get_globalanimstep(self,3) == NULL() ) set_globalanimstep(self,3,1);
                        else set_globalanimstep(self,3,get_globalanimstep(self,3)+1); // si somma 1 per ogni giocatore che ha concluso la sua path
                        set_animstep(self,p,3); // passo 1 per il giocatore p
                }
            } // fine for

            if ( get_globalanimstep(self,3) == openborvariant("count_players") ) setlocalvar("endlevel_semaphore", 9);
        } else if ( getlocalvar("endlevel_semaphore") == 9 && getlocalvar("wait") < openborvariant("elapsed_time") ) { // STEP 9: GO WITH SURF #####################
            int p, player_num = 0;

            changelevelproperty("scrollspeed", 0);

            for ( p = 0; p < MAX_PLAYERS; ++p ) { // Giriamo i players a destra
                void player = getplayerproperty(p,"entity");
                float xpos = 6200, zpos = 750-0;

                if ( !getentityproperty(player,"exists") ) continue;
                ++player_num;

                if ( getentityproperty(player,"aiflag","dead") ) {
                    setlocalvar("reset_animation",1);
                    return;
                }

                if ( getentityproperty(player, "subject_to_wall") != 0 ) changeentityproperty(player, "subject_to_wall", 0);
                if ( get_animstep(self,p) == 3 ) {
                    if ( smovetoxz(player,xpos+(player_num-1)*30,zpos+(player_num-1)*30,1,2.2) ) {
                        //changeentityproperty(player, "noaicontrol", 0);
                        if ( get_globalanimstep(self, 4) == NULL() ) set_globalanimstep(self,4,1);
                        else set_globalanimstep(self,4,get_globalanimstep(self, 4)+1); // si somma 1 per ogni giocatore che ha concluso la sua path

                        set_animstep(self,p,4); // passo 1 per il giocatore p
                    } // fine step (passo) 0 per tutti
                }
            } // fine for

            if ( get_globalanimstep(self,4) == openborvariant("count_players") ) {
                    setlocalvar("endlevel_semaphore", 10);
                    //setlocalvar("wait", openborvariant("elapsed_time")+openborvariant("game_speed")*2);
            }
        } else if ( getlocalvar("endlevel_semaphore") == 10 && getlocalvar("wait") < openborvariant("elapsed_time") ) {
            setlocalvar("comic_spawned",NULL());
            changeopenborvariant("nojoin", 0);
            changeopenborvariant("nopause", 0);
            killentity(self);
        }

    //} // <--------------- CANCELLA
}

int jumptoxz2(void player, float dir_x, float dir_z, float max_altitude, int final_direction) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float speed = getentityproperty(player,"speed");
    float final_x = dir_x, final_z = dir_z;
    int p = getentityproperty(player, "playerindex");

    //speed = 1.3;
    speed = 2.0;

    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( getlocalvar("start_pos_x"+p) == NULL() )  setlocalvar("start_pos_x"+p, x);
        if ( getlocalvar("start_pos_z"+p) == NULL() )  setlocalvar("start_pos_z"+p, z);
        if ( getlocalvar("half_jump"+p) == NULL() ) setlocalvar("half_jump"+p, 0);

        if ( dir_x > getlocalvar("start_pos_x"+p) ) { // Impostiamo metà strada!!
            dir_x = (dir_x - getlocalvar("start_pos_x"+p))/2 + getlocalvar("start_pos_x"+p);
            changeentityproperty(player, "direction", 1);
        } else {
            dir_x = (getlocalvar("start_pos_x"+p) - dir_x)/2 + dir_x;
            changeentityproperty(player, "direction", 0);
        }
        if ( dir_z > getlocalvar("start_pos_z"+p) ) dir_z = (dir_z - getlocalvar("start_pos_z"+p))/2 + getlocalvar("start_pos_z"+p);
        else dir_z = (getlocalvar("start_pos_z"+p) - dir_z)/2 + dir_z;

        if ( getlocalvar("half_jump"+p) == 0 ) {
            if ( getentityproperty(player, "defaultmodel") == "FOOT_SOLDIER" ) {
                if ( getentityproperty(player, "animationid") != openborconstant("ANI_FOLLOW80") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW80"));
            } else {
                if ( getentityproperty(player, "animationid") != openborconstant("ANI_JUMP") && getentityproperty(player,"animvalid",openborconstant("ANI_JUMP")) ) changeentityproperty(player, "animation", openborconstant("ANI_JUMP"));
            }

            if ( getentityproperty(player, "antigravity") != 0 ) {
                changeentityproperty(player, "antigravity", 0); // attiviamo la gravità
                if ( a != base ) changeentityproperty(player, "position", x, z, base);
            }

            //if ( jumpingtoxz_speedx(player, dir_x, dir_z, max_altitude, speed*1.3, speed*2) ) {
            if ( jumpingtoxz_smoothed(player, dir_x, dir_z, max_altitude, speed) ) {
                setlocalvar("half_jump"+p, 1);
            }
        } else {
            if ( getentityproperty(player, "subject_to_wall") != 0 ) changeentityproperty(player, "subject_to_wall", 0);
            changeentityproperty(player, "antigravity", 0);
            //if ( jumpingtoxz_speedx(player, final_x, final_z, base, speed*1.4, speed*2) ) {
            if ( jumpingtoxz_smoothed(player, final_x, final_z, base, speed) ) {
                if ( a >= base && a <= base+10 ) changeentityproperty(player, "animation", openborconstant("ANI_JUMPLAND"));
                changeentityproperty(player, "velocity", 0, 0, 0);
                changeentityproperty(player, "direction", final_direction);
                //changeentityproperty(player, "antigravity", 0);
                //changeentityproperty(player, "position", NULL(), NULL(), getentityproperty(player, "base"));
                setlocalvar("start_pos_x"+p, NULL());
                setlocalvar("start_pos_z"+p, NULL());
                setlocalvar("half_jump"+p, NULL());

                return 1;
            }
        }
    } // fine if spawn

  return 0;
}



