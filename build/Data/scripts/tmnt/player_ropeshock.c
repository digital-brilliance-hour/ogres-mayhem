
#import "data/scripts/lib.c"
#import "data/scripts/player_didhit.c"

int check_ropeshock(void player) {
    void opp = getentityproperty(player, "opponent");

    if ( opp && getentityproperty(opp,"exists") ) {
        int anim_id = getentityproperty(player, "animationid");
        int opp_anim_id = getentityproperty(opp, "animationid");
        float oa = getentityproperty(opp, "y");
        float a = getentityproperty(player, "y");
        //drawstring( 10,180,0,"Var: "+a );
        //drawstring( 10,190,0,"Var: "+oa );

        if ( anim_id == openborconstant("ANI_PAIN30") || anim_id == openborconstant("ANI_PAIN31") ) {
                int time = openborvariant("elapsed_time");
                int reset_time = 400;
                int combo_max = 15;
                int p = getentityproperty(player, "playerindex");

                changeentityproperty(player, "aiflag", "inpain", 1);
                changeentityproperty(player, "noaicontrol", 1);

                // Check Time-Reset
                if ( check_stored_time(getentityvar(player,1),9999) ) setentityvar(player,1,openborconstant("MIN_INT"));
                if ( getentityvar(player,1) == NULL() ) { // se non abbiamo impostato il tempo...
                    //---------PRESS BUTTON-----------
                    if ( getglobalvar("rrodney_button_escape"+p) == NULL() && getglobalvar("rrodney_active"+p) == NULL() ) {
                        void ent = spawnbuttons("press_any_button", player);
                        setglobalvar("rrodney_button_escape"+p, ent);
                    }
                    //--------------------------------
                    setentityvar(player,1,get_next_time(reset_time));
                } else if ( getentityvar(player,1) > time ) {
                    // Premiamo i pulsanti
                    if ( playerkeys(p,1,"attack") == 64 || playerkeys(p,1,"moveleft") == 4 || playerkeys(p,1,"moveright") == 8 || playerkeys(p,1,"moveup") == 16
                      || playerkeys(p,1,"movedown") == 32 || playerkeys(p,1,"jump") == 128 || playerkeys(p,1,"special") == 256 ) {
                        if ( getentityvar(player,2) == NULL() ) setentityvar(player,2,0);
                        else setentityvar(player,2,getentityvar(player,2)+1);
                        // Cambiamo animazione
                        if ( anim_id != openborconstant("ANI_PAIN31") ) changeentityproperty(player,"animation",openborconstant("ANI_PAIN31"));
                    } // fine if playerkeys

                    if ( getentityvar(player, 2) > combo_max ) { // LIBERIAMOCI grazie alla combo!!
                        if ( getglobalvar("rrodney_button_escape"+p) != NULL() && getentityproperty(getglobalvar("rrodney_button_escape"+p), "exists") ) killentity(getglobalvar("rrodney_button_escape"+p)); // Eliminiamo l'animazione pulsante
                        setglobalvar("rrodney_button_escape"+p, NULL());
                        setglobalvar("rrodney_active"+p, 1);
                        setentityvar(player,1,NULL());
                        setentityvar(player,2,NULL());
                        setentityvar(player,"shock_no_rope_opp",NULL());
                        setlocalvar("shock_wait_to_damage",NULL());
                        changeentityproperty(player,"noaicontrol",0);
                        if ( getentityproperty(player,"health") > 0 ) {
                            setidle(player, openborconstant("ANI_IDLE"), 1);
                            changeentityproperty(player, "invincible", 1);
                            changeentityproperty(player, "invinctime", openborvariant("elapsed_time") + (openborvariant("game_speed")*0.5));
                            //changeentityproperty(player, "aiflag", "blink", 0);
                        }
                    }
                } else if ( openborvariant("elapsed_time") > getentityvar(player,1)  ) { // se il tempo è passato resettiamo per la prossima combo!
                    setentityvar(player,1,NULL()); // Annulliamo il tempo
                    setentityvar(player,2,NULL()); // Annulliamo la combo
                    // Cambiamo animazione
                    if ( anim_id != openborconstant("ANI_PAIN30") ) changeentityproperty(player,"animation",openborconstant("ANI_PAIN30"));
                } // fine if tempo

                // ########## SOLO SE NEMICO E' ATTACCATO (ES. CORDA) ##########
                if ( getentityvar(player,"shock_no_rope_opp") == NULL() ) {
                    if ( opp_anim_id != openborconstant("ANI_FOLLOW90") || (a != oa)  ) { // qualcuno ha colpito il rrodney
                            setentityvar(player,1,NULL());
                            setentityvar(player,2,NULL());
                            setentityvar(player,"shock_no_rope_opp",NULL());
                            setlocalvar("shock_wait_to_damage",NULL());
                            changeentityproperty(player,"noaicontrol",0);
                            if ( getentityproperty(player,"health") > 0 ) setidle(player, openborconstant("ANI_IDLE"), 1);
                    }
                }
                check_ropeshock_damage(player, 15, 2); // SE IL NEMICO NON E' ATTACCATO (NO RRODNEY ROPE)

        } else if ( anim_id != openborconstant("ANI_PAIN30") && anim_id != openborconstant("ANI_PAIN31") ) { // Eliminiamo l'animazione pulsante per prevenire bugs
            int p = getentityproperty(player, "playerindex");
            if ( getglobalvar("rrodney_button_escape"+p) != NULL() && getentityproperty(getglobalvar("rrodney_button_escape"+p), "exists") ) killentity(getglobalvar("rrodney_button_escape"+p)); // Eliminiamo l'animazione pulsante
            setglobalvar("rrodney_button_escape"+p, NULL());
        }
    }
}

int check_ropeshock_damage(void self, int damage, int damage_time) {
    void opp = getentityvar(self,"shock_no_rope_opp");
    // Facciamo perdere al player una certa quantità di HP ogni damage_time frames...
    if ( opp != NULL() ) {
        int time = openborvariant("elapsed_time");
        int health = getentityproperty(self, "health");
        int dir = getentityproperty(self,"direction");

        if ( health <= 0 ) return;

        if ( !getentityproperty(opp,"exists") ) {
            opp = self;
        } else {
            if ( getentityvar(opp,"damage") != NULL() ) damage = getentityvar(opp,"damage");
        }

        //drawstring(10,200,0,"time: "+time);
        //drawstring(10,210,0,"shock_wait_to_damage: "+getlocalvar("shock_wait_to_damage"));
        damage_time *= openborvariant("game_speed");
        if ( getlocalvar("shock_wait_to_damage") == NULL() ) {
            setlocalvar("shock_wait_to_damage",get_next_time(damage_time)); // Check Time-Reset
        }
        if ( time >= getlocalvar("shock_wait_to_damage") ) {
            if ( health-damage > 0 ) changeentityproperty(self,"health",health-damage);
            else { // se il player muore...
                void oparent = getentityproperty(opp,"parent");

                playsample(loadsample("data/sounds/rrodney03.wav"));
                damageentity(self,opp,damage,1,openborconstant("ATK_SHOCK"));

                if ( !getentityproperty(oparent,"exists") ) oparent = opp;
                //give_score(oparent,self); // give score to player (if exists)
                check_versus_mark(oparent,self);
            }
            setlocalvar("shock_wait_to_damage",NULL());
        }
    } else {
        if ( getlocalvar("shock_wait_to_damage") != NULL() ) setlocalvar("shock_wait_to_damage",NULL());
    } // fine if opp
}


