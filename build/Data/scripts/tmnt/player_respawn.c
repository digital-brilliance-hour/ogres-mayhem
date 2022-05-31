
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/player_offset.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/flying_script.c"
#import "data/scripts/endlevel_anim.c"


int set_coords(void player, float max_alt) {
    int anim_id = getentityproperty(player, "animationid");
    int anim_pos = getentityproperty(player, "animpos");

    if ( anim_id == openborconstant("ANI_RESPAWN") && anim_pos >= 0 ) { // && getlocalvar("respawn") != 1
        float x,z,a,base;
        int p = getentityproperty(player, "playerindex"); // non puoi ricavarti il player con getentityproperty(getplayerproperty(p, "entity")) poichè non esiste ancora
        int t;
        void platform_ex;

        x = getglobalvar("p"+(p+1)+"respawn_x");
        z = getglobalvar("p"+(p+1)+"respawn_z");
        a = getglobalvar("p"+(p+1)+"respawn_a");
        base = getglobalvar("p"+(p+1)+"respawn_base");
        platform_ex = getglobalvar("p"+(p+1)+"respawn_platform");

        if ( anim_pos > 3 && platform_ex == NULL() ) {
            return 0; // for float/walkable platforms
        }

        if ( get_ready_players_count(player) >= 1 ) {
            if ( is_incam_coords(x,z,a,base,T_CAM) ) { // siamo nel riquadro giusto?

                // se sono nulle allora non è mai stato nel livello il personaggio. Verrà eseguita l'animazione RESPAWN
                if ( x != NULL() && z != NULL() && a != NULL() && a < max_alt ) {
                    //changeentityproperty(player, "subject_to_wall", 0);
                    //changeentityproperty(player, "subject_to_platform", 0);
                    set_respawn_on(player,x,z,a,base);
                    setlocalvar("respawn", 1);

                    return 1;
                } else {
                    void last_player = getglobalvar("last_player_spawned");
                    int p = getentityproperty(last_player, "playerindex");
                    int distance = getlocalvar("__distance");
                    int result;

                    //if ( !getentityproperty(last_player,"exists") || getentityproperty(last_player,"type") != openborconstant("type_player") ) {
                        last_player = generate_last_player_spawned(player,1);
                    //}

                    if (distance == NULL()) distance = 15;
                    result = check_respawn(player,distance);
                    if ( getlocalvar("respawn") != 1 && !is_incam(player,T_CAM) ) check_respawn_incam(player);
                    if ( result < 1 ) set_latest_safe_coords(player);
                    setlocalvar("respawn", 1);

                    return 1;
                }
            } else {
                void last_player = getglobalvar("last_player_spawned");
                int p = getentityproperty(last_player, "playerindex");
                int distance = getlocalvar("__distance");
                int result;

                //if ( !getentityproperty(last_player,"exists") || getentityproperty(last_player,"type") != openborconstant("type_player") ) {
                    last_player = generate_last_player_spawned(player,1);
                //}

                if (distance == NULL()) distance = 15;
                result = check_respawn(player,distance,1);
                if ( getlocalvar("respawn") != 1 && !is_incam(player,T_CAM) ) check_respawn_incam(player);
                if ( result < 1 ) set_latest_safe_coords(player);
                setlocalvar("respawn", 1);

                return 1;
            } // fine if incam
        } else { // la funzione incam() ha senso con più di un giocatore in gara
                if ( x != NULL() && z != NULL() && a != NULL() && a < max_alt ) {
                    // fai controllo incam: se cadi in buca e poi la cam si blocca per i nemici allora è utile
                    if ( is_incam_coords(x,z,a,base,T_CAM) || platform_ex != NULL() ) {
                        set_respawn_on(player,x,z,a,base);
                    } else if ( getlocalvar("respawn") != 1 ) {
                        check_respawn_incam(player);
                    }
                    setlocalvar("respawn", 1);

                    return 1;
                } else {
                    void last_player = getglobalvar("last_player_spawned");
                    int p = getentityproperty(last_player, "playerindex");
                    int distance = getlocalvar("__distance");
                    int result;

                    //if ( !getentityproperty(last_player,"exists") || getentityproperty(last_player,"type") != openborconstant("type_player") ) {
                        last_player = generate_last_player_spawned(player,1);
                    //}

                    if (distance == NULL()) distance = 15;
                    result = check_respawn(player,distance);
                    if ( getlocalvar("respawn") != 1 && !is_incam(player,T_CAM) ) check_respawn_incam(player);
                    if ( result < 1 ) set_latest_safe_coords(player);
                    setlocalvar("respawn", 1);

                    return 1;
                }
        } // fine if ready_players_count()

        // ##### IMPORTANTE: Facciamo eseguire questo script per tutta la durata del respawn!! Perchè al primo frame potrebbero accadere errori!!

        return 1;
    }

    return 0;
}

void set_latest_safe_coords(void player) {
    float xmax = NULL();
    int p;
    float xp,zp,ap,basep,x,z,a;
    int pindex = getentityproperty(player, "playerindex");

    for (p = 0; p < MAX_PLAYERS; ++p) {
        int max_player = getplayerproperty(p, "entity");

        //if ( !getentityproperty(max_player, "exists") ) continue;
        //if ( p == pindex ) continue;

        xp = getglobalvar("p"+(p+1)+"respawn_x");
        zp = getglobalvar("p"+(p+1)+"respawn_z");
        ap = getglobalvar("p"+(p+1)+"respawn_a");
        basep = getglobalvar("p"+(p+1)+"respawn_base");

        if ( xmax == NULL() || xp > xmax ) {
            if ( xp != NULL() && zp != NULL() && ap != NULL() && is_incam_coords(xp,zp,ap,basep,T_CAM) ) {
                xmax = xp;
                setglobalvar("p"+(pindex+1)+"respawn_x",xp);
                setglobalvar("p"+(pindex+1)+"respawn_z",zp);
                setglobalvar("p"+(pindex+1)+"respawn_a",ap);
                setglobalvar("p"+(pindex+1)+"respawn_base",basep);
            }
        } // fine if xmax
    } // fine for

    if ( x != NULL() && z != NULL() && a != NULL() ) {
        changeentityproperty(player, "position", x, z, a);
    }
}

// if flag on, search last player spawned but not == self
void generate_last_player_spawned(void self, int self_flag) {
    int p;
    float last_player = NULL();

    for (p = 0; p < MAX_PLAYERS; ++p) {
        int player = getplayerproperty(p, "entity");
        int time = NULL(), last_time = NULL();

        if ( !getentityproperty(player, "exists") ) continue;
        if ( getglobalvar("p"+(p+1)+"spawning") == 1 ) continue; // sta spawnando ora
        if ( self_flag != NULL() && self_flag > 0 && self != NULL() && self == player ) continue;
        if ( !is_incam(player) ) continue;

        // Check Time-Reset
        if ( check_stored_time(getglobalvar("p"+(p+1)+"spawn_time"),9999) ) setglobalvar("p"+(p+1)+"spawn_time", openborconstant("MIN_INT"));
        time = getglobalvar("p"+(p+1)+"spawn_time");
        if ( last_time == NULL() || time > last_time ) {
            last_player = player;
            last_time = time;
        }
    } // fine for

    if ( last_player != NULL() ) setglobalvar("last_player_spawned", last_player);

    return last_player;
}

void set_inspawning(void player, int respawn_distance) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id == openborconstant("ANI_RESPAWN") || anim_id == openborconstant("ANI_SPAWN") ) {
        int p = getentityproperty(player, "playerindex");

        if ( respawn_distance != getlocalvar("__distance") ) setlocalvar("__distance", respawn_distance);
        if ( getglobalvar("p"+(p+1)+"spawning") != 1 ) setglobalvar("p"+(p+1)+"spawning", 1);
        if ( anim_id == openborconstant("ANI_SPAWN") ) {
            if ( getglobalvar("last_player_spawned") == NULL() ) setglobalvar("last_player_spawned", player);
            else if ( getglobalvar("last_player_spawned") == player ) setglobalvar("last_player_spawned", player); // setta per tutta la duranta dello spawn se è il primo...
        }
    } else {
        int p = getentityproperty(player, "playerindex");

        if ( getglobalvar("p"+(p+1)+"spawning") == 1 ) {
            setglobalvar("last_player_spawned", player);
            setglobalvar("p"+(p+1)+"spawn_time", openborvariant("elapsed_time"));
            //if ( getlocalvar("respawn") != NULL() ) setlocalvar("respawn", NULL());
            setglobalvar("p"+(p+1)+"spawning", 0);

            if ( getlocalvar("respawn") != NULL() ) setlocalvar("respawn", NULL());
        }
    }
}

/*int is_incam_zdistance(float x, float z, float a, float base, float threshold) {
    float scr_posx = openborvariant("xpos");
    float scr_posy = openborvariant("ypos");
    int hres = openborvariant("hresolution");
    int vres = openborvariant("vresolution");
    //float camx = getlevelproperty("cameraxoffset");
    //float camz = getlevelproperty("camerazoffset");
    float zmin_distance = get_zmin_distance();
    float zmax_distance = get_zmax_distance();

    if ( threshold == NULL() ) threshold = 5;

    if ( x == NULL() || z == NULL() ) return 0;
    if ( x < scr_posx-threshold || x > scr_posx+hres+threshold ) return 0;
    //if ( (z-base < zmin_distance-threshold || z-base > zmax_distance+threshold) ) return 0; // && openborvariant("count_players") > 1
    if ( (z-base < scr_posy-threshold || z-base > scr_posy+vres+threshold) ) return 0;

    return 1;
}*/

int set_coords_predeath(void player, float max_alt) {
    if ( !is_in_pain(player) ) {
        int frames = 2; // mem_frames = (openborvariant("game_speed")/4)*3; // 150
        int anim_id = getentityproperty(player, "animationid");

        if ( time_to_wait("coord_predeath_frm",frames) == 1 && anim_id != openborconstant("ANI_RESPAWN") ) {
            float x = getentityproperty(player, "x");
            float z = getentityproperty(player, "z");
            float a = getentityproperty(player, "y");
            float base = getentityproperty(player, "base");
            int health = getentityproperty(player, "health");
            int p = getentityproperty(player, "playerindex");
            void float_platform = NULL();
            int hole = checkhole(x,z);

            reset_time_to_wait("coord_predeath_frm");

            if ( getentityproperty(player, "opponent") ) {
                float_platform = getentityvar(getentityproperty(player, "opponent"),0);
            }

            if ( ( ( gep(player,"aiflag","walking") || gep(player,"aiflag","running")
                || anim_id == openborconstant("ANI_IDLE") || anim_id == openborconstant("ANI_RUN") || anim_id == openborconstant("ANI_WALK") || anim_id == openborconstant("ANI_LAND")
                || anim_id == openborconstant("ANI_GET") || anim_id == openborconstant("ANI_BLOCK") || anim_id == openborconstant("ANI_UP") || anim_id == openborconstant("ANI_DOWN")
                || anim_id == openborconstant("ANI_JUMPLAND")
                || (anim_id == openborconstant("ANI_SPAWN") && getglobalvar("last_player_spawned") == player && ov("count_players") <= 1) )
                && ( (!gep(player,"no_adjust_base") && !is_flying(player)) || is_flying(player) )
                && float_platform != "float_platform" && (!hole || (hole && base > 0)) && health > 0 ) ) { // && (a >= 0 && a < max_alt && a == getlocalvar("a_coord"))
                void platform = get_platform_on(player);
                float wall_height = checkwall(x,z);

                if ( a != base && anim_id != openborconstant("ANI_SPAWN") && !is_flying(player) ) return 0;

                if ( platform != NULL() ) {
                    if ( getentityvar(platform,0) != "walkable_platform" ) return 0;
                } else if ( hole ) return 0;

                if ( wall_height > a ) return 0;

                if ( is_flying(player) ) {
                    void platform_fly_on = checkplatform(x,z,a);

                    if ( platform_fly_on != NULL() ) {
                        a = get_platform_base(platform_fly_on);
                        base = a;
                    } else {
                        if ( wall_height > 0 ) a = wall_height;
                        else a = 0;
                        base = a;
                    }
                }

                // AVOID BUG: not register on platform edges
                if ( getglobalvar("p"+(p+1)+"respawn_platform") != NULL() ) {
                    if ( platform == NULL() ) {
                        float wall_height = checkwall(x,z);
                        float hole_flag = checkhole(x,z);

                        if ( base > 0 && wall_height != base ) return 0;
                        if ( hole_flag ) return 0;
                    }
                } else if ( platform == NULL() ) {
                    float ypos = ov("vresolution");
                    float hole_flag = checkhole(x,z);

                    if ( hole_flag ) return 0;
                    if ( a-base > ypos/1.5 ) return 0;
                }

                //drawstring( 10,200,0,"Storing Coords...");
                setglobalvar("p"+(p+1)+"respawn_x", x);
                setglobalvar("p"+(p+1)+"respawn_z", z);
                setglobalvar("p"+(p+1)+"respawn_a", a);
                setglobalvar("p"+(p+1)+"respawn_base", base);
                setglobalvar("p"+(p+1)+"respawn_platform", platform);
            } else if ( anim_id == openborconstant("ANI_FOLLOW21") && isATurtle(player) ) { // morte nel tombino
                if ( getentityproperty(player, "direction") == 1 ) setglobalvar("p"+(p+1)+"respawn_x", x+17);
                else  setglobalvar("p"+(p+1)+"respawn_x", x-0);
                setglobalvar("p"+(p+1)+"respawn_z", z+8);
                setglobalvar("p"+(p+1)+"respawn_a", a);
                setglobalvar("p"+(p+1)+"respawn_base", base);
            }
        } // fine if no spawn && no respawn
    }

    return 1;
}

// Sistemiamo il personaggio dopo il respawn ne caso è dentro una platform (lo script può essere eseguito solo ora e non onspawn event)
// 1 ha esito positivo, 0 non esiste l'ultimo player spawned o non è nel quadro, -1 ultimo giocatore spawnato è se stesso, 2 non viene eseguito
int check_respawn(void player, float threshold, int no_cam_flag) {
    if ( getentityproperty(player, "animationid") == openborconstant("ANI_RESPAWN") && getlocalvar("respawn") == NULL() ) { // && getentityproperty(player, "animpos") > 0
            void last_player = getglobalvar("last_player_spawned");

            //drawstring( 20,160,0,"Siamo qui!" );
            if ( last_player == player ) {
                return -1; // Questa funzione ha senso rispetto ad altri giocatori e non a se stessi!! Altrimenti usare le p#respawn_X
            }

            if ( !getentityproperty(last_player,"exists") || getentityproperty(last_player,"type") != openborconstant("type_player") || last_player == NULL() ) {
                return 0;
            } else {
                int p = getentityproperty(last_player, "playerindex");
                int pp = getentityproperty(player, "playerindex");
                float x = getglobalvar("p"+(p+1)+"respawn_x");
                float z = getglobalvar("p"+(p+1)+"respawn_z");
                float a = getglobalvar("p"+(p+1)+"respawn_a");
                float base = getglobalvar("p"+(p+1)+"respawn_base");
                void platform_ex = getglobalvar("p"+(p+1)+"respawn_platform");
                int minz = openborvariant("player_min_z");
                int maxz = openborvariant("player_max_z");
                int i, c;
                float t_alt = MAX_ALTITUDE;
                float x2, z2;
                float wall = 0, platform_base = 0, wall_last_spawned = 0, platform_base_last_spawned = 0;
                void platform = NULL(), platform_last_spawned = NULL();
                int hole;
                int ok_flag = 0;

                    if ( x == NULL() || z == NULL() || a == NULL() || base == NULL() ) {
                        return 0;
                    }

                    if ( !is_incam_coords(x,z,a,base,T_CAM) && (no_cam_flag == NULL() || no_cam_flag <= 0) ) {
                        if ( platform_ex == NULL() ) {
                            return 0;
                        }
                    }

                    // Respawniamo un valore threshold di px in avanti/dietro/giù/su rispetto al giocatore già in gara
                    for ( i = 0; i <= threshold; ++i ) { // <= così può assumere valore 0!
                            for ( c = 0; c < 12; ++c ) {
                                if      ( c ==  0 ) { x2 = x-(threshold-i); z2 = z+(threshold-i); }
                                else if ( c ==  1 ) { x2 = x-(threshold-i); z2 = z-(threshold-i); }
                                else if ( c ==  2 ) { x2 = x+(threshold-i); z2 = z+(threshold-i); }
                                else if ( c ==  3 ) { x2 = x+(threshold-i); z2 = z-(threshold-i); }
                                else if ( c ==  4 ) { x2 = x; z2 = z+(threshold-i); }
                                else if ( c ==  5 ) { x2 = x; z2 = z-(threshold-i); }
                                else if ( c ==  6 ) { x2 = x; z2 = z+(threshold-i); }
                                else if ( c ==  7 ) { x2 = x; z2 = z-(threshold-i); }
                                else if ( c ==  8 ) { x2 = x-(threshold-i); z2 = z; }
                                else if ( c ==  9 ) { x2 = x-(threshold-i); z2 = z; }
                                else if ( c == 10 ) { x2 = x+(threshold-i); z2 = z; }
                                else if ( c == 11 ) { x2 = x+(threshold-i); z2 = z; }

                                if ( minz == maxz ) z2 = z;

                                hole = checkhole(x2,z2);
                                wall = checkwall(x2,z2);
                                platform = checkplatform(x2,z2,base);

                                // vedi se stai sopra un muro o una piattaforma o sulla base (wall <= 0)
                                if ( !hole || (hole && base > 0) ) {
                                     if ( platform != NULL() ) {
                                        platform_base = get_platform_base(platform);
                                        base = platform_base;
                                        a = base;
                                     } else if ( wall > 0 ) {
                                        base = wall;
                                        a = base;
                                     }
                                     if ( !is_incam_coords(x2,z2,a,base,T_CAM) ) {
                                        base = getglobalvar("p"+(p+1)+"respawn_base");
                                        a = base;
                                     }

                                    /*
                                     * Se il last_spawned è stato registrato sopra una muro/piattaforma, allora solo su quell muro/piattaforma potrai spawnare
                                     */
                                    if ( last_player != player ) {
                                        wall_last_spawned = checkwall(x,z);
                                        platform_last_spawned = checkplatform(x,z,base);
                                        if ( platform_last_spawned != NULL() ) {
                                            platform_base_last_spawned = get_platform_base(platform_last_spawned);

                                            if ( platform != platform_last_spawned && base == platform_base_last_spawned ) continue;
                                        } else if ( wall > 0 && wall != wall_last_spawned && base == wall_last_spawned ) continue;
                                    }


                                    if ( !is_incam_coords(x2,z2,a,base,T_CAM) ) continue;


                                    ok_flag = 1;
                                    break;
                                } else if ( hole ) continue; // there's a hole
                            } // fine for

                            if ( ok_flag ) break;
                    } // fine for

                if ( ok_flag ) {
                    // coords are ok, can save them last_spawned -> current_player (to respawn on the same platform of last_spawned)
                    setglobalvar("p"+(pp+1)+"respawn_x", x2);
                    setglobalvar("p"+(pp+1)+"respawn_z", z2);
                    setglobalvar("p"+(pp+1)+"respawn_a", a);
                    setglobalvar("p"+(pp+1)+"respawn_base", base);
                    setglobalvar("p"+(pp+1)+"respawn_platform", platform_ex);

                    //changeentityproperty(player, "position", x2, z2, a);
                    set_respawn_on(player,x2,z2,a,base);
                } else {
                    return 0;
                }

                setlocalvar("respawn", 1);
                return 1;
            } // fine if last_player exists
    } else {
        return 2;
    }
}

// riposiziona il player durante la respawn. se stava su una piattaforma in movimento, spawnalo nella giusta posizione
void set_respawn_on(void player, float x, float z, float a, float base, int force_mid_platform, void force_platform) {
    int p = gep(player,"playerindex");
    float wall = 0;
    void platform = NULL(), platform_ex = getglobalvar("p"+(p+1)+"respawn_platform");
    float t_alt = 2.0;
    int hole;

    if ( force_platform != NULL() ) platform_ex = force_platform;

    wall = checkwall(x,z);
    platform = get_platform_by_coords_in_range(x,z,a,base);
    hole = checkhole(x,z);

    if ( platform != NULL() ) {
        base = get_platform_base(platform);
        a = base;
    } else if ( ent_exists(platform_ex) ) {
        base = get_platform_base(platform_ex);
        a = base;
    } else if ( wall > 0 ) {
        base = wall;
        a = base;
    } else {
        platform = find_platform_in_a(x,z,a,MAX_ALTITUDE);
        if ( platform != NULL() ) {
            base = get_platform_base(platform);
            a = base;
        }
    }


    if ( ent_exists(platform_ex) ) {
        if ( (hole && base <= 0) || platform != platform_ex || is_moving(platform_ex) || (force_mid_platform != NULL() && force_mid_platform > 0) ) { // NULL() compreso su platform != platform_ex
            int i = 0;
            float platform_width = 0;
            float platform_depth = 0;

            // respawna in mezzo
            base = get_platform_base(platform_ex);
            a = base;
            x = gep(platform_ex,"x");
            z = gep(platform_ex,"z");

            //x = get_platform_extreme_x(platform_ex) + get_platform_width(platform_ex)/2;
            //z = get_platform_extreme_z(platform_ex) + get_platform_depth(platform_ex)/2;

            // get extremes x
            for ( i = x; checkplatform(i,z,base+t_alt) == platform_ex; --i ) {
                x = i;
            }
            for ( i = x; checkplatform(i,z,base+t_alt) == platform_ex; ++i ) {
                ++platform_width;
            }
            x += platform_width/2;

            // get extremes z
            for ( i = z; checkplatform(x,i,base+t_alt) == platform_ex; --i ) {
                z = i;
            }
            for ( i = z; checkplatform(x,i,base+t_alt) == platform_ex; ++i ) {
                ++platform_depth;
            }
            z += platform_depth/2;
        }
    }

    changeentityproperty(player, "position", x, z, a);
    changeentityproperty(player, "base", base);
}

int check_respawn_incam(void player) {
    if ( !is_incam(player) ) {
        float scr_posx = ov("xpos");
        float scr_posy = ov("ypos");
        int hres = ov("hresolution");
        int vres = ov("vresolution");
        float x, z, y, base, max_h = 0, t_h = 100;
        float tmp_dist = -1;
        void platform_ex = NULL();
        int i = 0, j = 0, min_p = -1, sum = 1;

        // search player in min distance from middle screen
        for (i = 0; i < MAX_PLAYERS; ++i) {
            x = getglobalvar("p"+(i+1)+"respawn_x");
            z = getglobalvar("p"+(i+1)+"respawn_z");

            if ( x != NULL() && z != NULL() ) {
                float dist = distAB(x,z,scr_posx+hres/2,scr_posy+vres/2);

                if ( dist < tmp_dist || tmp_dist == -1 ) {
                    tmp_dist = dist;
                    min_p = i;
                }
            }
        } // fine for
        if ( min_p == -1 ) return 0;

        x = getglobalvar("p"+(min_p+1)+"respawn_x");
        z = getglobalvar("p"+(min_p+1)+"respawn_z");
        y = getglobalvar("p"+(min_p+1)+"respawn_a");
        base = getglobalvar("p"+(min_p+1)+"respawn_base");
        platform_ex = getglobalvar("p"+(min_p+1)+"respawn_platform");
        max_h = y+t_h;

        // scan all screen to find a good place
        sum = 2;
        for (i = 0; i < hres; i+=sum) {
            for (j = 0; j < vres; j+=sum) {
                float wall = checkwall(scr_posx+i,scr_posy+j);
                int hole = checkhole(scr_posx+i,scr_posy+j);
                void platform = checkplatform(scr_posx+i,scr_posy+j,max_h); //find_platform_in_a(scr_posx+i,scr_posy+j,max_h,MAX_ALTITUDE);
                float platform_base = -1;

                if ( platform != NULL() ) {
                    platform_base = get_platform_base(platform);
                }

                if ( hole ) continue;
                if ( wall > MAX_WALL_HEIGHT ) continue;
                if ( platform_base > MAX_WALL_HEIGHT ) continue;

                if ( platform_base < max_h && platform_base > 0 ) {
                    if ( !is_incam_coords(scr_posx+i,scr_posy+j,y,base,T_CAM) ) continue;
                    if ( y < platform_base ) y = platform_base;
                    set_respawn_on(player,scr_posx+i,scr_posy+j,y,base,1,platform);
                    return 1;
                } else if ( wall < max_h ) { // it works on wall or on ground (0)
                    if ( !is_incam_coords(scr_posx+i,scr_posy+j,y,base,T_CAM) ) continue;
                    changeentityproperty(player,"position",scr_posx+i,scr_posy+j,wall);
                    return 1;
                }
            }
        }

    } else return 1; // fine if incam

    return 0;
}

