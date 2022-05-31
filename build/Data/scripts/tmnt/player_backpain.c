
#import "data/scripts/lib.c"

int check_backpain(void player, float threshold) {
    if ( is_in_pain(player) ) {
        void opp = getentityproperty(player, "opponent");
        int anim_id = getentityproperty(player, "animationid");
        //float x = getentityproperty(player,"x");
        //float z = getentityproperty(player,"z");
        //float a = getentityproperty(player,"y");
        //float base = getentityproperty(player,"base");

            if ( getentityproperty(opp,"exists") ) {
                int oanim_id = getentityproperty(opp, "animationid");

                if ( oanim_id == openborconstant("ANI_GRAB") || oanim_id == openborconstant("ANI_FOLLOW41") ) return;
            }
            // Se il nemico è davanti a noi (più a destra) ma noi siamo girati a sinistra allora backpain!!
            // Quindi valutiamo se stanno l'uno alle spalle dell'altro...
            if ( gettimebp(player) >= 0 && gettimebp(player) <= 1 ) {
                if ( (getprevdir(player) == 0 && getoppx(player) > getentityproperty(player, "x")+threshold)
                      || (getprevdir(player) == 1 && getoppx(player) < getentityproperty(player, "x")-threshold) ) {

                            if ( get_backpain_flag(player) ) { // evitiamo la back pain/fall/die
                                reset_oppx(player);
                                return;
                            }
                            if ( getentityproperty(player, "health") > 0 ) { // && !get_backpain_flag(player)
                                int pain_flag = 0;

                                     if ( getentityproperty(player,"aiflag","dead")    ) pain_flag = 3;
                                else if ( getentityproperty(player,"aiflag","falling") ) pain_flag = 2;
                                else if ( getentityproperty(player,"aiflag","inpain")  ) pain_flag = 1;

                                if ( anim_id == openborconstant("ANI_PAIN") || anim_id == openborconstant("ANI_PAIN2") || anim_id == openborconstant("ANI_PAIN3") || anim_id == openborconstant("ANI_PAIN8") ) {
                                    set_backdir(player,threshold);
                                    performattack(player, openborconstant("ANI_PAIN16"), 1);
                                    set_backdir(player,threshold);
                                } else if ( anim_id == openborconstant("ANI_PAIN6") ) {
                                    set_backdir(player,threshold);
                                    performattack(player, openborconstant("ANI_PAIN17"), 1);
                                    set_backdir(player,threshold);
                                } else if ( anim_id == openborconstant("ANI_PAIN4") || anim_id == openborconstant("ANI_PAIN5") ) {
                                    set_backdir(player,threshold);
                                    performattack(player, openborconstant("ANI_PAIN11"), 1);
                                    set_backdir(player,threshold);
                                } else if ( anim_id == openborconstant("ANI_FALL") || anim_id == openborconstant("ANI_FALL4") ) {
                                    set_backdir(player,threshold);
                                    performattack(player, openborconstant("ANI_FALL51"), 1);
                                    set_backdir(player,threshold);
                                } else if ( anim_id == openborconstant("ANI_FALL2") || anim_id == openborconstant("ANI_PAIN9") ) {
                                    set_backdir(player,threshold);
                                    performattack(player, openborconstant("ANI_FALL50"), 1);
                                    set_backdir(player,threshold);
                                } else if ( anim_id == openborconstant("ANI_FALL3") ) {
                                    set_backdir(player,threshold);
                                    performattack(player, openborconstant("ANI_FALL52"), 1);
                                    set_backdir(player,threshold);
                                }

                                changeentityproperty(player,"aiflag","attacking",0);
                                changeentityproperty(player,"attacking",0);
                                changeentityproperty(player,"aiflag","inpain",1);
                                /*if ( pain_flag == 1 ) changeentityproperty(player,"aiflag","inpain",1);
                                else if ( pain_flag == 2 ) changeentityproperty(player,"aiflag","falling",1);*/
                                if ( pain_flag == 2 ) changeentityproperty(player,"aiflag","falling",1);
                            } else { // else health <= 0...
                                if ( anim_id == openborconstant("ANI_DIE") || anim_id == openborconstant("ANI_DIE4") || anim_id == openborconstant("ANI_DIE40") ) {
                                    set_backdir(player,threshold);
                                    changeentityproperty(player, "animation", openborconstant("ANI_DIE52"));
                                    set_backdir(player,threshold);
                                } else if ( anim_id == openborconstant("ANI_DIE2") || anim_id == openborconstant("ANI_DIE3") || anim_id == openborconstant("ANI_DIE5")
                                     || anim_id == openborconstant("ANI_DIE9") || anim_id == openborconstant("ANI_DIE12") ) {
                                    set_backdir(player,threshold);
                                    changeentityproperty(player, "animation", openborconstant("ANI_DIE50") );
                                    set_backdir(player,threshold);
                                }
                            } // fine if health > 0

                            reset_oppx(player);
                } // fine if stanno alle spalle
            }

    }
}

int set_backdir(void player, float threshold) {
    float oppx = getoppx(player);
    int prevdir = getprevdir(player);

    if ( oppx != NULL() && prevdir != NULL() ) {
        if ( prevdir == 0 && oppx > getentityproperty(player, "x")+threshold ) changeentityproperty(player,"direction",0);
        else if ( prevdir == 1 && oppx < getentityproperty(player, "x")-threshold ) changeentityproperty(player,"direction",1);
    }
}

int settimebp(void player) {
    int anim_id = getentityproperty(player, "animationid");

    if ( anim_id != getlocalvar("animationid_bp") ) { // In questo modo s'imposta il tempo SOLO al primo frame dell'animpos == 0
        setentityvar(player, 8, NULL());
        setlocalvar("animpos_bp_flag", NULL());
    }
    if ( getentityproperty(player, "animpos") > 0 ) { //  && getlocalvar("animpos_bp_flag") != NULL() ???
        setentityvar(player, 8, NULL());
        setlocalvar("animpos_bp_flag", NULL());
    }

    if ( (getentityproperty(player, "aiflag", "dead") != 0 || getentityproperty(player, "aiflag", "falling") != 0 || getentityproperty(player, "aiflag", "inpain") != 0)
        && getentityvar(player, 8) == NULL() && getentityproperty(player, "animpos") <= 0 && getlocalvar("animpos_bp_flag") == NULL() ) {
        setlocalvar("animpos_bp_flag", 1);
        setlocalvar("animationid_bp", anim_id);
        setentityvar(player, 8, openborvariant("elapsed_time"));
    }
    if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1
        && getentityvar(player, 8) != NULL() ) {
        setentityvar(player, 8, NULL());
        setlocalvar("animpos_bp_flag", NULL());
    }
}

int gettimebp(void player) {
    int time = openborvariant("elapsed_time");
    int timebp = getentityvar(player, 8);

    // Check Time-Reset
    if ( check_stored_time(timebp,9999) ) setentityvar(player, 8, openborconstant("MIN_INT"));
    if ( timebp != NULL() ) {
        if ( timebp >= 0 ) return time-getentityvar(player, 8);
        else return time+getentityvar(player, 8);
    } else return NULL();
}

int getoppx(void player) {
    return getentityvar(player, 6);
}

int reset_oppx(void player) {
    setentityvar(player, 6, NULL() );
}

/*int setoppx(void player) {
    void opp = getentityproperty(player, "opponent");

    if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1 ) {
        void opp = getentityproperty(player, "opponent");

        if (opp) setentityvar(player, 6, getentityproperty(opp, "x") );
    } else if ( getentityvar(player, 6) == NULL() && opp ) {
        setentityvar(player, 6, getentityproperty(opp, "x") );
    }
}*/

int setprevdir(void player) {
    int anim_id = getentityproperty(player, "animationid");
    int dir = getentityproperty(player, "direction");

    if ( anim_id != getlocalvar("animationid_dir") || dir != getentityvar(player, 4) ) { // In questo modo s'imposta la dir al primo frame dell'animpos == 0
        if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1 ) { //getentityproperty(player, "animpos") <= 0
            setentityvar(player, 4, getentityproperty(player, "direction") );
        } else if ( getentityvar(player, 4) == NULL() ) setentityvar(player, 4, getentityproperty(player, "direction") );
        setlocalvar("animationid_dir", anim_id);
    }
}

int getprevdir(void player) {
    return getentityvar(player, 4);
}

int get_backpain_flag(void player) {
    return getentityvar(player, 24);
}

int set_backpain_flag(void player, int value) {
    return setentityvar(player, 24, value);
}



