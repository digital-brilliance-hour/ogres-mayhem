
#import "data/scripts/lib.c"

int check_backpain(void player, float threshold) {
    //void opp = getentityproperty(player, "opponent");
    int anim_id = getentityproperty(player, "animationid");

            // Se il nemico è davanti a noi (più a destra) ma noi siamo girati a sinistra allora backpain!!
            // Quindi valutiamo se stanno l'uno alle spalle dell'altro...
            if ( gettimebp(player) >= 0 && gettimebp(player) <= 1) {
                if ( (getprevdir(player) == 0 && getoppx(player) > getentityproperty(player, "x")+threshold)
                      || (getprevdir(player) == 1 && getoppx(player) < getentityproperty(player, "x")-threshold) ) {

                            if ( getentityproperty(player, "health") > 0 ) {
                                int pain_flag = 0;

                                if ( getentityproperty(player,"aiflag","inpain") ) pain_flag = 1;
                                else if ( getentityproperty(player,"aiflag","falling") ) pain_flag = 2;
                                else if ( getentityproperty(player,"aiflag","dead") ) pain_flag = 3;

                                if ( anim_id == openborconstant("ANI_FALL") || anim_id == openborconstant("ANI_FALL70") || anim_id == openborconstant("ANI_FALL2") ) {
                                    set_backdir(player,threshold);
                                    performattack(player, openborconstant("ANI_FALL11"), 1);
                                    set_backdir(player,threshold);
                                } else if ( anim_id == openborconstant("ANI_FALL12") ) {
                                    set_backdir(player,threshold);
                                    performattack(player, openborconstant("ANI_FALL14"), 1);
                                    set_backdir(player,threshold);
                                }

                                if ( pain_flag == 1 ) changeentityproperty(player,"aiflag","inpain",1);
                                else if ( pain_flag == 2 ) changeentityproperty(player,"aiflag","falling",1);
                            } else { // else health <= 0...
                                if ( anim_id == openborconstant("ANI_FALL") || anim_id == openborconstant("ANI_FALL70") || anim_id == openborconstant("ANI_FALL2") ) {
                                    set_backdir(player,threshold);
                                    changeentityproperty(player, "falldie", 1); // dopo la morte si ripristina!
                                    changeentityproperty(player, "animation", openborconstant("ANI_DIE11") );
                                    set_backdir(player,threshold);
                                } else if ( anim_id == openborconstant("ANI_DIE12") ) {
                                    set_backdir(player,threshold);
                                    changeentityproperty(player, "falldie", 1); // dopo la morte si ripristina!
                                    changeentityproperty(player, "animation", openborconstant("ANI_DIE15") );
                                    set_backdir(player,threshold);
                                }
                            } // fine if health > 0

                            reset_oppx(player);
                } // fine if stanno alle spalle
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
    if ( (getentityproperty(player, "aiflag", "dead") != 0 || getentityproperty(player, "aiflag", "falling") != 0 || getentityproperty(player, "aiflag", "inpain") != 0)
        && getentityvar(player, 8) == NULL() && getentityproperty(player, "animpos") <= 0 ) {
        setentityvar(player, 8, openborvariant("elapsed_time"));
    }
    if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1
        && getentityvar(player, 8) != NULL() ) {
        setentityvar(player, 8, NULL());
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

    if ( opp != NULL() && getentityproperty(opp,"exists") ) {
        if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1 ) {
            setentityvar(player, 6, getentityproperty(opp, "x") );
        } else if ( getentityvar(player, 6) == NULL() && opp ) {
            setentityvar(player, 6, getentityproperty(opp, "x") );
        }
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


