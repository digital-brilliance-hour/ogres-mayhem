
#import "data/scripts/lib.c"
#import "data/scripts/player_didhit.c"

int check_muddied(void player) {
    int anim_id = getentityproperty(player, "animationid");

            if ( anim_id == openborconstant("ANI_PAIN41") || anim_id == openborconstant("ANI_PAIN42") ) {
                int time = openborvariant("elapsed_time");
                int reset_time = 400;
                int combo_max = 13;
                int damage_time = 400;
                int damage = 15;
                int p = getentityproperty(player, "playerindex");

                changeentityproperty(player, "aiflag", "inpain", 1);
                changeentityproperty(player, "noaicontrol", 1);

                // ############ DAMAGE TIME ############
                if ( getentityvar(player,17) == NULL() ) {
                    setentityvar(player,17,get_next_time(damage_time));
                } else if ( getentityvar(player,17) < time ) { // tempo scaduto
                    if ( getentityproperty(player, "health") >= damage+1 ) changeentityproperty(player,"health",getentityproperty(player, "health")-damage);
                    else { // se il player muore...
                        void opp = getentityproperty(player,"opponent");
                        void weap_opp = getentityvar(player,"weapon_opp");

                        changeentityproperty(player,"noaicontrol",0);
                        if ( getentityproperty(opp,"exists") ) damageentity(player,opp,damage,0,openborconstant("ATK_NORMAL"));
                        else damageentity(player,player,damage,0,openborconstant("ATK_NORMAL"));
                        setentityvar(player,1,NULL());
                        setentityvar(player,2,NULL());

                        if ( getentityproperty(weap_opp,"exists") ) {
                            //give_score(weap_opp,player); // give score to player (if exists)
                            check_versus_mark(weap_opp,player);
                            setentityvar(player,"weapon_opp",NULL());
                        }

                        /// SPAWN DESTROYED OBJECTS
                        //spawn_destroyed_obj(player);
                    }
                    setentityvar(player,17,NULL());
                }
                // ######### FINE DAMAGE TIME ##########

                // Check Time-Reset
                if ( check_stored_time(getentityvar(player,1),9999) ) setentityvar(player,1,openborconstant("MIN_INT"));
                if ( getentityvar(player,1) == NULL() ) { // se non abbiamo impostato il tempo...
                    //---------PRESS BUTTON-----------
                    if ( getglobalvar("muddied_button_escape"+p) == NULL() && getglobalvar("muddied_active"+p) == NULL() ) {
                        void ent = spawnbuttons("press_any_button", player);
                        setglobalvar("muddied_button_escape"+p, ent);
                    }
                    //--------------------------------
                    setentityvar(player,1,get_next_time(reset_time));
                } else if ( getentityvar(player,1) > time ) {
                    if ( anim_id == openborconstant("ANI_PAIN42") || anim_id == openborconstant("ANI_PAIN41") ) {
                        // Premiamo i pulsanti
                        if ( playerkeys(p,1,"attack") == 64 || playerkeys(p,1,"moveleft") == 4 || playerkeys(p,1,"moveright") == 8 || playerkeys(p,1,"moveup") == 16
                          || playerkeys(p,1,"movedown") == 32 || playerkeys(p,1,"jump") == 128 || playerkeys(p,1,"special") == 256 ) {
                            if ( getentityvar(player,2) == NULL() ) setentityvar(player,2,0);
                            else setentityvar(player,2,getentityvar(player,2)+1);
                            // Cambiamo animazione
                            //if ( anim_id != openborconstant("ANI_PAIN33") ) changeentityproperty(player,"animation",openborconstant("ANI_PAIN33"));
                            if ( getentityproperty(player,"health") > 0 ) {
                                if ( anim_id != openborconstant("ANI_PAIN41") ) performattack(player,openborconstant("ANI_PAIN41"),1);
                            }
                        } // fine if playerkeys
                    } // fine if pain32

                    if ( getentityvar(player, 2) > combo_max ) { // LIBERIAMOCI grazie alla combo!!
                        if ( getglobalvar("muddied_button_escape"+p) != NULL() && getentityproperty(getglobalvar("muddied_button_escape"+p), "exists") ) killentity(getglobalvar("muddied_button_escape"+p)); // Eliminiamo l'animazione pulsante
                        setglobalvar("muddied_button_escape"+p, NULL());
                        setglobalvar("muddied_active"+p, 1);
                        setentityvar(player,1,NULL()); // Annulliamo il tempo
                        setentityvar(player,2,NULL()); // Annulliamo la combo
                        changeentityproperty(player,"noaicontrol",0);
                        if ( getentityproperty(player,"health") > 0 ) {
                            setidle(player, openborconstant("ANI_IDLE"), 1);
                            performattack(player, openborconstant("ANI_RISE11"), 1);
                        }

                        /// SPAWN DESTROYED OBJECTS
                        //spawn_destroyed_obj(player);
                    }
                } else if ( time > getentityvar(player,1)  ) { // se il tempo è passato resettiamo per la prossima combo!
                    setentityvar(player,1,NULL()); // Annulliamo il tempo
                    setentityvar(player,2,NULL()); // Annulliamo la combo
                    // Cambiamo animazione
                    if ( anim_id != openborconstant("ANI_PAIN42") ) changeentityproperty(player,"animation",openborconstant("ANI_PAIN42"));
                } // fine if tempo
            } else if ( anim_id != openborconstant("ANI_PAIN41") && anim_id != openborconstant("ANI_PAIN42") ) { // Eliminiamo l'animazione pulsante per prevenire bugs
                int p = getentityproperty(player, "playerindex");

                if ( getglobalvar("muddied_button_escape"+p) != NULL() && getentityproperty(getglobalvar("muddied_button_escape"+p), "exists") ) killentity(getglobalvar("muddied_button_escape"+p)); // Eliminiamo l'animazione pulsante
                setglobalvar("muddied_button_escape"+p, NULL());
            }

}

int spawn_destroyed_obj(void self) {
    void subent = spawnsubentity_relative("clay_explo",0,1,0,self);
}

