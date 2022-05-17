
#import "data/scripts/lib.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/lib_platform.c"

int set_coords_predeath_swim(void player, float max_alt) {
    int frames = 2; // mem_frames = (openborvariant("game_speed")/4)*3; // 150
    int refresh_time = openborvariant("elapsed_time")%frames;
    int anim_id = getentityproperty(player, "animationid");

    if ( refresh_time == 0 && anim_id != openborconstant("ANI_SPAWN") && anim_id != openborconstant("ANI_RESPAWN") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        int health = getentityproperty(player, "health");
        int p = getentityproperty(player, "playerindex");
        void float_platform = NULL();

        if ( getentityproperty(player, "opponent") ) {
            float_platform = getentityvar(getentityproperty(player, "opponent"), 0);
        }

        if ( ( anim_id == openborconstant("ANI_IDLE") || anim_id == openborconstant("ANI_RUN") || anim_id == openborconstant("ANI_WALK") || anim_id == openborconstant("ANI_LAND")
            || anim_id == openborconstant("ANI_GET") || anim_id == openborconstant("ANI_BLOCK") || anim_id == openborconstant("ANI_UP") || anim_id == openborconstant("ANI_DOWN")
            || anim_id == openborconstant("ANI_JUMPLAND") || anim_id == openborconstant("ANI_FOLLOW60") || anim_id == openborconstant("ANI_FOLLOW61") || anim_id == openborconstant("ANI_FOLLOW62")
            || anim_id == openborconstant("ANI_FOLLOW63") || anim_id == openborconstant("ANI_FOLLOW64") || anim_id == openborconstant("ANI_FOLLOW65") || anim_id == openborconstant("ANI_TURN") )
            && float_platform != "float_platform"
            && checkhole(x,z) == 0 && (a == base) && health > 0 ) { // && (a >= 0 && a < max_alt && a == getlocalvar("a_coord"))
            void platform = get_platform_on(player);

            if ( platform != NULL() ) {
                if ( getentityvar(platform,0) != "walkable_platform" ) return 0;
            }

            /*if ( getglobalvar("p"+(p+1)+"respawn_xo") == NULL() || getglobalvar("p"+(p+1)+"respawn_zo") == NULL() || getglobalvar("p"+(p+1)+"respawn_ao") == NULL() ) { // Inizializziamole comunque!!
                setglobalvar("p"+(p+1)+"respawn_xo", x);
                setglobalvar("p"+(p+1)+"respawn_zo", z);
                setglobalvar("p"+(p+1)+"respawn_ao", a);
            } else if (openborvariant("elapsed_time")%mem_frames == 0 && x != getglobalvar("p"+(p+1)+"respawn_x") && z != getglobalvar("p"+(p+1)+"respawn_z") ) {
                setglobalvar("p"+(p+1)+"respawn_xo", x); // vecchie coordinate per chi respawna vicino
                setglobalvar("p"+(p+1)+"respawn_zo", z);
                setglobalvar("p"+(p+1)+"respawn_ao", a);

                // Guardiamo le due variabili globali per registrare la vecchia posizione (anche 'a'). In questo modo possiamo memorizzare
                // le variabili respawn_#o solo se il personaggio si sposta. Questo trick lo adottiamo per non far spawnare il partner nella stessa identica posizione del player!
            }*/
            //drawstring( 10,200,0,"Storing Coords...");
            setglobalvar("p"+(p+1)+"respawn_x", x);
            setglobalvar("p"+(p+1)+"respawn_z", z);
            setglobalvar("p"+(p+1)+"respawn_a", a);
            setglobalvar("p"+(p+1)+"respawn_base", base);

            // Memorizziamo l'altitudine così da confrontarla con la prossima al prossimo frame. In caso si deve memorizzare la posizione sopra una piattaforma.
            // Infatti se stesse saltando o cadendo l'altitudine non sarebbe mai uguale da un frame all'altro...
            //setlocalvar("a_coord", a);
        } else if ( anim_id == openborconstant("ANI_FOLLOW21") ) { // morte nel tombino
            if ( getentityproperty(player, "direction") == 1 ) setglobalvar("p"+(p+1)+"respawn_x", x+17);
            else  setglobalvar("p"+(p+1)+"respawn_x", x-0);
            setglobalvar("p"+(p+1)+"respawn_z", z+8);
            setglobalvar("p"+(p+1)+"respawn_a", a);
        /*} else if ( a != getlocalvar("a_coord") ) {
            setlocalvar("a_coord", a);*/
        }
    }

}


