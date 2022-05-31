
void main() { // entityvar usate (indexes): 0 (flag), 1 (damage time), 2 (getwalkingzdir), 3 (time)
  void self = getlocalvar("self");

  setentityvar(self, 2, getwalkingzdir(self,2));
  check_player_pos(self, 140);
  check_target_pos(self, 140);


    //drawstring( 10,170,0,"Var ():  "+openborconstant("ANI_PAIN21") );
    //drawstring( 10,170,0,"Var (zdir):  "+getentityproperty(self, "zdir") );
    //drawstring( 10,180,0,"Var (walkzdir):  "+getentityvar(self, 2) );
    //drawstring( 10,190,0,"Var (speed):  "+getentityproperty(self, "speed") );

}

int check_target_pos(void self, float threshold) {
    void target = findtarget(self);
    float run_speed = getentityproperty(self, "running", "speed");
    float speed = getentityproperty(self, "speed");

    if (target) {
        float x = getentityproperty(self, "x");
        float z = getentityproperty(self, "z");
        float xt = getentityproperty(target, "x");
        float zt = getentityproperty(target, "z");

        if ( x > xt && x-xt < threshold ) changeentityproperty(self, "velocity", speed, NULL(), NULL());
    }

}

int check_player_pos(void self, float threshold) {
    int i;
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float min_xp = NULL();
    float run_speed = getentityproperty(self, "running", "speed");

    for ( i = 0; i < 4; ++i ) {
        void player = getplayerproperty(i, "entity");

        if ( !getentityproperty(player, "exists") ) continue;
        else {
            float xp = getentityproperty(player, "x");
            float zp = getentityproperty(player, "z");

            if ( xp > x && (min_xp == NULL() || xp < min_xp) ) min_xp = xp;
            else if (xp < x) return 1; // significa che c'è almeno un player a sx del nemico quindi è tutto ok!
        } // fine if exists
    } // fine for

    if ( min_xp != NULL() ) {
        if ( x < min_xp+threshold ) {
            changeentityproperty(self, "velocity", run_speed, NULL(), NULL());
            changeentityproperty(self, "aiflag", "running", 1);
        }
    }

    return 0;
}

/*
 * 0 = going up
 * 1 = going down
 * 2 = stop
 * NULL = not detectable
 */
int getwalkingzdir(void ent, int frames) {
    int refresh_time = openborvariant("elapsed_time")%frames;
    int animid = getentityproperty(ent,"animationid");

    if ( refresh_time == 0 ) {
        int z1 = getlocalvar("old_z");
        int z2 = getentityproperty(ent, "z");
        int result = NULL();

        if ( z1 != NULL() && z2 != NULL() ) {
            if ( z2 > z1 ) {
                result = 1;
            } else if ( z2 < z1 ) {
                result = 0;
            } else { // se x2 == x1...
                // se corri o cammini ma urti sul muro guarda la direzione senza osservare se c'è un cambiamento fra x1 e x2 (se x1==x2 evidentemente stavi già sul muro)
                if ( getentityproperty(ent, "zdir") > 0 ) {
                    return 1;
                } else if ( getentityproperty(ent, "zdir") < 0 ) {
                    return 0;
                } else result = 2;
            }
        } else result = NULL();

        setlocalvar("old_z", z2);

        return result;
    }

    return getentityvar(ent, 2);
}

