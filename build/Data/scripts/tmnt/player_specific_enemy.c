
#import "data/scripts/lib.c"

int check_mouser_attack(void player) {
    void opp = getentityproperty(player, "opponent");
    int p = getentityproperty(player, "playerindex");
    int time = openborvariant("elapsed_time");
    int anim_id = getentityproperty(player, "animationid");
    int middle_stance_time = 150, last_stance_time = 300;
    int max_combo_buttons = 15;

    if ( anim_id == openborconstant("ANI_PAIN23") ) {
       check_safe_pos(player,0,1); // safe position
       if ( !getentityproperty(player,"aiflag","inpain") ) changeentityproperty(player,"aiflag","inpain",1);

        // facciamo comparire la prima mano
	   if ( getglobalvar("any_button"+p) == NULL() && getglobalvar("any_button_active"+p) == NULL() ) {
           void ent;

           clearspawnentry();
           setspawnentry("name", "press_any_arrow");
           ent = spawn();
           changeentityproperty(ent, "parent", player);
           setglobalvar("any_button"+p, ent);
           setglobalvar("any_button_active"+p, 1);
	   }

		if ( getplayerproperty(p, "keys") >= 192 && getplayerproperty(p, "keys") <= 252 ) changeplayerproperty(p, "keys", 0); // disabilitiamo la special

		if ( playerkeys(p,1,"moveleft") == 4 || playerkeys(p,1,"moveright") == 8 || playerkeys(p,1,"moveup") == 16 || playerkeys(p,1,"movedown") == 32 ) {
            setentityvar(player,1,get_next_time(middle_stance_time));
            setentityvar(opp,0,NULL()); // flag per l'opp. se off può cambiare animazione
            changeentityproperty(player, "animation", openborconstant("ANI_PAIN22"));
            // Uccidiamo la mano
            if ( getglobalvar("any_button"+p) != NULL() && getentityproperty(getglobalvar("any_button"+p),"exists") ) killentity(getglobalvar("any_button"+p));
            setglobalvar("any_button"+p, NULL());
		}

    } else if ( anim_id == openborconstant("ANI_PAIN22") ) {
        check_safe_pos(player,0,1); // safe position
        if ( !getentityproperty(player,"aiflag","inpain") ) changeentityproperty(player,"aiflag","inpain",1);

        if ( getglobalvar("any_button"+p) == NULL() && getglobalvar("any_button_active"+p) == 1 ) {
           void ent;
            // facciamo comparire la seconda mano
           if ( getglobalvar("any_button"+p) != NULL() && getentityproperty(getglobalvar("any_button"+p),"exists") ) killentity(getglobalvar("any_button"+p));
           clearspawnentry();
           setspawnentry("name", "press_any_button");
           ent = spawn();
           changeentityproperty(ent, "parent", player);
           setglobalvar("any_button"+p, ent);
	   }

		if ( getplayerproperty(p, "keys") >= 192 && getplayerproperty(p, "keys") <= 252 ) changeplayerproperty(p, "keys", 0);

        // Check Time-Reset
        if ( check_stored_time(getentityvar(player,1),9999) ) setentityvar(player,1,openborconstant("MIN_INT"));
		if ( getentityvar(player,1) < time ) {
		    // resettiamo la seconda mano affinchè compaia la prima
            if ( getglobalvar("any_button_active"+p) != NULL() && getglobalvar("any_button"+p) != NULL() ) {
                   if ( getglobalvar("any_button"+p) != NULL() && getentityproperty(getglobalvar("any_button"+p),"exists") ) killentity(getglobalvar("any_button"+p));
                   setglobalvar("any_button"+p, NULL());
                   setglobalvar("any_button_active"+p, NULL());
            }
            setentityvar(opp,0,NULL()); // flag per l'opp. (mouser solo) se off può cambiare animazione
            changeentityproperty(player, "animation", openborconstant("ANI_PAIN23"));
		} else {
            if ( getplayerproperty(p, "keys") > 0 && getplayerproperty(p, "keys") <= 256 && getplayerproperty(p, "keys") != 2 ) { // 2 + start/pause
                setentityvar(player,1,get_next_time(last_stance_time));
                setentityvar(opp,0,NULL()); // flag per l'opp. se off può cambiare animazione
                changeentityproperty(player, "animation", openborconstant("ANI_PAIN21"));
            }
		}
    } else if ( anim_id == openborconstant("ANI_PAIN21") ) {
        check_safe_pos(player,0,1); // safe position
        if ( !getentityproperty(player,"aiflag","inpain") ) changeentityproperty(player,"aiflag","inpain",1);

        if ( getplayerproperty(p, "keys") >= 192 && getplayerproperty(p, "keys") <= 252 ) changeplayerproperty(p, "keys", 128);

        // Check Time-Reset
        if ( check_stored_time(getentityvar(player,1),9999) ) setentityvar(player,1,openborconstant("MIN_INT"));
		if ( getentityvar(player,1) > time ) { // finchè hai tempo a disposizione...
		    if ( playerkeys(p,1,"moveleft") == 4 || playerkeys(p,1,"moveright") == 8 || playerkeys(p,1,"moveup") == 16 || playerkeys(p,1,"movedown") == 32
                    || playerkeys(p,1,"attack") == 64 || playerkeys(p,1,"jump") == 128 || playerkeys(p,1,"special") == 256 ) {
                // contiamo la combo!
                if ( getentityvar(player,2) != NULL() ) setentityvar(player,2,getentityvar(player,2)+1);
                else setentityvar(player,2,0);
		    }
		} else if ( getentityvar(player,2) < max_combo_buttons || getentityvar(player,2) == NULL() ) { // torniamo alla precedente animazione
            setentityvar(player,2,NULL());
            setentityvar(player,1,get_next_time(middle_stance_time));
            setentityvar(opp,0,NULL()); // flag per l'opp. se off può cambiare animazione
            changeentityproperty(player, "animation", openborconstant("ANI_PAIN22"));
		} else { // LIBERIAMOCI grazie alla combo!!
		    setglobalvar("any_button_active"+p, 2);
		    setentityvar(player,1,NULL());
            setentityvar(player,2,NULL());
            // uccidiamo la mano
            if ( getglobalvar("any_button"+p) != NULL() && getentityproperty(getglobalvar("any_button"+p),"exists") ) killentity(getglobalvar("any_button"+p));
            setglobalvar("any_button"+p, NULL());

            //changeentityproperty(player, "takeaction", "common_vault", 1);
            //changeentityproperty(player, "noaicontrol", 0);
            //changeentityproperty(player, "aiflag", "idling", 1);
            //changeentityproperty(player, "animation", openborconstant("ANI_IDLE"));
            if ( getentityproperty(player,"health") > 0 ) {
                setidle(player,openborconstant("ANI_IDLE"),1);
                changeentityproperty(player, "opponent", NULL());
                changeentityproperty(player, "invincible", 1);
    			changeentityproperty(player, "invinctime", openborvariant("elapsed_time") + (openborvariant("game_speed")*0.5));
            }
		}

    }

    // aggiungere se player è 21-22-23 e mouser no allora setidle. quindi se flag di mouser e se il nome dell'opp è mouser
    check_reset_mouser(player);
}

int check_reset_mouser(void player) {
    int anim_id = getentityproperty(player, "animationid");

    // Può succedere che si colpisca il mouser (e può anche morire)
    if ( anim_id == openborconstant("ANI_PAIN21") || anim_id == openborconstant("ANI_PAIN22") || anim_id == openborconstant("ANI_PAIN23") ) {
            void opp = getentityproperty(player, "opponent");

            if ( !getentityproperty(opp,"exists") ) { // Non esiste? RESET!!
                int p = getentityproperty(player, "playerindex");

                setentityvar(player,1,NULL());
                setentityvar(player,2,NULL());
                if ( getentityproperty(player,"health") > 0 ) setidle(player,openborconstant("ANI_IDLE"),1);
                changeentityproperty(player, "opponent", NULL());
                // uccidiamo la mano
                if ( getglobalvar("any_button"+p) != NULL() && getentityproperty(getglobalvar("any_button"+p),"exists") ) killentity(getglobalvar("any_button"+p));
                setglobalvar("any_button"+p, NULL());
            } else { // Esiste e non è in animazione bite_atk??? RESET!!
                int o_anim_id = getentityproperty(opp, "animationid");

                if ( o_anim_id != openborconstant("ANI_PAIN21") && o_anim_id != openborconstant("ANI_PAIN22") && o_anim_id != openborconstant("ANI_PAIN23")
                     && o_anim_id != openborconstant("ANI_ATTACK23") && o_anim_id != openborconstant("ANI_FOLLOW23") ) {
                    int p = getentityproperty(player, "playerindex");

                    setentityvar(player,1,NULL());
                    setentityvar(player,2,NULL());
                    if ( getentityproperty(player,"health") > 0 ) setidle(player,openborconstant("ANI_IDLE"),1);
                    changeentityproperty(player, "opponent", NULL());
                    // uccidiamo la mano
                    if ( getglobalvar("any_button"+p) != NULL() && getentityproperty(getglobalvar("any_button"+p),"exists") ) killentity(getglobalvar("any_button"+p));
                    setglobalvar("any_button"+p, NULL());
                }
            }
    } // fine if pains

    // Può succedere che il player venga colpito: in tal caso si occupa la funzione check_general in player_moves.c

}

