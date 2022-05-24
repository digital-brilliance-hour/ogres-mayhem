
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"

// Salta atterrando su x,z
int jumptoxz(void player, float dir_x, float dir_z, float max_altitude, int final_direction) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float speed = getentityproperty(player,"speed");
    float final_x = dir_x, final_z = dir_z;
    int p = player;

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

            if ( getentityproperty(player, "animationid") != openborconstant("ANI_JUMP") ) {
                changeentityproperty(player, "antigravity", 0); // attiviamo la gravità
                if ( a != base ) changeentityproperty(player, "position", x, z, base);
                changeentityproperty(player, "animation", openborconstant("ANI_JUMP"));
            }

            if ( jumpingtoxz(player, dir_x, dir_z, max_altitude, speed*2) ) {
                setlocalvar("half_jump"+p, 1);
            }
        } else {
            changeentityproperty(player, "antigravity", 0);
            if ( jumpingtoxz(player, final_x, final_z, base, speed*2) ) {
                if ( a >= base && a <= base+10 && getentityproperty(player,"animvalid",openborconstant("ANI_JUMPLAND")) ) changeentityproperty(player, "animation", openborconstant("ANI_JUMPLAND"));
                changeentityproperty(player, "velocity", 0, 0, 0);
                changeentityproperty(player, "direction", final_direction);
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

// sub-func: salta fino a max_altitude o base con nuova velocità per il salto
int jumpingtoxz(void player, float dir_x, float dir_z, float max_altitude, float jump_speed) { // max_altitude <= 0 indica landing
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed = getentityproperty(player,"speed");
    float speed_x, speed_z;

    if ( x > dir_x-threshold && x < dir_x+threshold  ) {
        speed_x = 0;
    } else if ( x < dir_x+threshold ) { // pg deve andare a destra
        speed_x = speed;
        if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3);
    } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
        speed_x = -1*speed;
        if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
    }

    if ( z > dir_z-threshold && z < dir_z+threshold  ) {
        speed_z = 0;
    } else if ( z < dir_z+threshold ) { // pg deve andare giù
        speed_z = speed/2;
        if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
    } else if ( z > dir_z-threshold ) { // pg deve andare su
        speed_z = -1*(speed/2);
        if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
    }

    if ( max_altitude > base && a >= max_altitude ) jump_speed = 0; // Raggiunta altezza max quindi non salire più.
    else if ( a <= base && max_altitude <= base ) jump_speed = 0;

    if ( max_altitude > base ) {
        if ( speed_x == 0 && speed_z == 0 && a >= max_altitude ) {
            return 1;
        }
    } else {
        if ( speed_x == 0 && speed_z == 0 && a <= base ) {
            return 1;
        }
    }


    if ( max_altitude > base ) changeentityproperty(player, "velocity", speed_x, speed_z, jump_speed);
    else changeentityproperty(player, "velocity", speed_x, speed_z, -1*jump_speed);

  return 0;
}

// sub-func: salta fino a max_altitude o base: velocità uguale sia per salto che per x,z
int jumpingtoxz_speed(void player, float dir_x, float dir_z, float max_altitude, float jump_speed) { // max_altitude <= 0 indica landing
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed_x, speed_z;

    if ( x > dir_x-threshold && x < dir_x+threshold  ) {
        speed_x = 0;
    } else if ( x < dir_x+threshold ) { // pg deve andare a destra
        speed_x = jump_speed;
        if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3);
    } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
        speed_x = -1*jump_speed;
        if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
    }

    if ( z > dir_z-threshold && z < dir_z+threshold  ) {
        speed_z = 0;
    } else if ( z < dir_z+threshold ) { // pg deve andare giù
        speed_z = jump_speed/2;
        if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
    } else if ( z > dir_z-threshold ) { // pg deve andare su
        speed_z = -1*(jump_speed/2);
        if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
    }

    if ( max_altitude > base && a >= max_altitude ) jump_speed = 0; // Raggiunta altezza max quindi non salire più.
    else if ( a <= base && max_altitude <= base ) jump_speed = 0;

    if ( max_altitude > base ) {
        if ( speed_x == 0 && speed_z == 0 && a >= max_altitude ) {
            return 1;
        }
    } else {
        if ( speed_x == 0 && speed_z == 0 && a <= base ) {
            return 1;
        }
    }


    if ( max_altitude > base ) changeentityproperty(player, "velocity", speed_x, speed_z, jump_speed);
    else changeentityproperty(player, "velocity", speed_x, speed_z, -1*jump_speed);

  return 0;
}

// sub-func: salta fino a max_altitude o base: velocità diversa per salto e per x,z
int jumpingtoxz_speedx(void player, float dir_x, float dir_z, float max_altitude, float speed, float jump_speed) { // max_altitude <= 0 indica landing
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed_x, speed_z;

    if ( x > dir_x-threshold && x < dir_x+threshold  ) {
        speed_x = 0;
    } else if ( x < dir_x+threshold ) { // pg deve andare a destra
        speed_x = speed;
        if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3);
    } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
        speed_x = -1*speed;
        if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
    }

    if ( z > dir_z-threshold && z < dir_z+threshold  ) {
        speed_z = 0;
    } else if ( z < dir_z+threshold ) { // pg deve andare giù
        speed_z = speed/2;
        if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
    } else if ( z > dir_z-threshold ) { // pg deve andare su
        speed_z = -1*(speed/2);
        if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
    }

    if ( max_altitude > base && a >= max_altitude ) jump_speed = 0; // Raggiunta altezza max quindi non salire più.
    else if ( a <= base && max_altitude <= base ) jump_speed = 0;

    if ( max_altitude > base ) {
        if ( speed_x == 0 && speed_z == 0 && a >= max_altitude ) {
            return 1;
        }
    } else {
        if ( speed_x == 0 && speed_z == 0 && a <= base ) {
            return 1;
        }
    }


    if ( max_altitude > base ) changeentityproperty(player, "velocity", speed_x, speed_z, jump_speed);
    else changeentityproperty(player, "velocity", speed_x, speed_z, -1*jump_speed);

  return 0;
}

// sub-func: salta/scendi fino ad "y" e x,z con speed uguale per tutto
int jumpingtoxza(void player, float dir_x, float dir_z, float altitude, float jump_speed) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed_x, speed_z;

    //drawstring(10,100,0,"trymove: "+getentityproperty(player,"trymove"));
    if ( x > dir_x-threshold && x < dir_x+threshold  ) {
        speed_x = 0;
    } else if ( x < dir_x+threshold ) { // pg deve andare a destra
        speed_x = jump_speed;
        if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3);
    } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
        speed_x = -1*jump_speed;
        if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
    } //else speed_x = 0;

    if ( z > dir_z-threshold && z < dir_z+threshold  ) {
        speed_z = 0;
    } else if ( z < dir_z+threshold ) { // pg deve andare giù
        speed_z = jump_speed/2;
        if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
    } else if ( z > dir_z-threshold ) { // pg deve andare su
        speed_z = -1*(jump_speed/2);
        if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
    } //else speed_z = 0;

    if ( a > altitude-threshold && a < altitude+threshold  ) {
        jump_speed = 0; // Raggiunta altezza max quindi non salire più.
    } else if ( a < altitude+threshold ) { // pg deve andare in alto
        jump_speed = jump_speed;
        if ( (altitude-a) <= threshold ) jump_speed = (altitude-a)*(2/3);
    } else if ( a > altitude-threshold ) { // pg deve andare in basso
        jump_speed = -1*jump_speed;
        if ( (a-altitude) <= threshold ) jump_speed = -1*(a-altitude)*(2/3);
    }

    /*drawstring(10,100,0,"var: "+getentityproperty(player,"y"));
    drawstring(10,110,0,"var: "+altitude);
    drawstring(10,120,0,"var: "+jump_speed);*/

    // To avoid bug to return on base with antigravity
    /*if ( altitude == base && a != base && jump_speed != 0 && getentityproperty(player,"antigravity") > 0 ) {
        jump_speed = 0;
        changeentityproperty(player,"antigravity",0);
        changeentityproperty(player,"position",NULL(),NULL(),base);
    }*/

    if ( altitude > base ) {
        if ( speed_x == 0 && speed_z == 0 && jump_speed == 0 ) {
            return 1;
        }
    } else {
        if ( speed_x == 0 && speed_z == 0 && jump_speed == 0 ) {
            return 1;
        }
    }

    changeentityproperty(player, "velocity", speed_x, speed_z, jump_speed);

  return 0;
}

// Cammina verso x,z
int movetoxz(void player, float dir_x, float dir_z, int final_direction) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed = getentityproperty(player,"speed");
    float speed_x, speed_z;

    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( a != base ) changeentityproperty(player, "position", x, z, base);

        if ( x > dir_x-threshold && x < dir_x+threshold  ) {
            speed_x = 0;
        } else if ( x < dir_x+threshold ) { // pg deve andare a destra
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_WALK") ) changeentityproperty(player, "animation", openborconstant("ANI_WALK"));
            changeentityproperty(player, "direction", 1);
            speed_x = speed;
            if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3); // Impostiamo la velocità proprio uguale alla distanza che manca
        } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_WALK") ) changeentityproperty(player, "animation", openborconstant("ANI_WALK"));
            changeentityproperty(player, "direction", 0);
            speed_x = -1*speed;
            if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
        }

        // Diamo la priorità all'animazione UP/DOWN
        if ( z > dir_z-threshold && z < dir_z+threshold  ) {
            speed_z = 0;
        } else if ( z < dir_z+threshold ) { // pg deve andare giù
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_DOWN") ) changeentityproperty(player, "animation", openborconstant("ANI_DOWN"));
            speed_z = speed/2;
            if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
        } else if ( z > dir_z-threshold ) { // pg deve andare su
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_UP") ) changeentityproperty(player, "animation", openborconstant("ANI_UP"));
            speed_z = -1*(speed/2);
            if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
        }

        if ( speed_x == 0 && speed_z == 0 ) {
            changeentityproperty(player, "velocity", speed_x, speed_z, NULL());
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_IDLE") ) changeentityproperty(player, "animation", openborconstant("ANI_IDLE"));
            changeentityproperty(player, "direction", final_direction);

            return 1;
        }

        changeentityproperty(player, "velocity", speed_x, speed_z, NULL());
    } // fine if spawn

  return 0;
}

// Corri verso x,z
int runningtoxz(void player, float dir_x, float dir_z, int final_direction) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed = getentityproperty(player,"running","speed");
    float speed_x, speed_z;

    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( a != base ) changeentityproperty(player, "position", x, z, base);

        if ( x > dir_x-threshold && x < dir_x+threshold  ) {
            speed_x = 0;
        } else if ( x < dir_x+threshold ) { // pg deve andare a destra
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_RUN") ) changeentityproperty(player, "animation", openborconstant("ANI_RUN"));
            changeentityproperty(player, "direction", 1);
            speed_x = speed;
            if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3); // Impostiamo la velocità proprio uguale alla distanza che manca
        } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_RUN") ) changeentityproperty(player, "animation", openborconstant("ANI_RUN"));
            changeentityproperty(player, "direction", 0);
            speed_x = -1*speed;
            if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
        }

        // Diamo la priorità all'animazione UP/DOWN
        if ( z > dir_z-threshold && z < dir_z+threshold  ) {
            speed_z = 0;
        } else if ( z < dir_z+threshold ) { // pg deve andare giù
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_RUN") ) changeentityproperty(player, "animation", openborconstant("ANI_RUN"));
            speed_z = speed/2;
            if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
        } else if ( z > dir_z-threshold ) { // pg deve andare su
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_RUN") ) changeentityproperty(player, "animation", openborconstant("ANI_RUN"));
            speed_z = -1*(speed/2);
            if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
        }

        if ( speed_x == 0 && speed_z == 0 ) {
            changeentityproperty(player, "velocity", speed_x, speed_z, NULL());
            if ( getentityproperty(player, "animationid") != openborconstant("ANI_IDLE") ) changeentityproperty(player, "animation", openborconstant("ANI_IDLE"));
            changeentityproperty(player, "direction", final_direction);

            return 1;
        }

        changeentityproperty(player, "velocity", speed_x, speed_z, NULL());
    } // fine if spawn

  return 0;
}

// SPECIAL: no animation change + speed
int smovetoxz(void player, float dir_x, float dir_z, int final_direction, float speed) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed_x, speed_z;

    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( a != base ) changeentityproperty(player, "position", x, z, base);

        if ( x > dir_x-threshold && x < dir_x+threshold  ) {
            speed_x = 0;
        } else if ( x < dir_x+threshold ) { // pg deve andare a destra
            changeentityproperty(player, "direction", 1);
            speed_x = speed;
            if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3); // Impostiamo la velocità proprio uguale alla distanza che manca
        } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
            changeentityproperty(player, "direction", 0);
            speed_x = -1*speed;
            if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
        }

        // Diamo la priorità all'animazione UP/DOWN
        if ( z > dir_z-threshold && z < dir_z+threshold  ) {
            speed_z = 0;
        } else if ( z < dir_z+threshold ) { // pg deve andare giù
            speed_z = speed/2;
            if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
        } else if ( z > dir_z-threshold ) { // pg deve andare su
            speed_z = -1*(speed/2);
            if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
        }

        if ( speed_x == 0 && speed_z == 0 ) {
            changeentityproperty(player, "velocity", speed_x, speed_z, NULL());
            changeentityproperty(player, "direction", final_direction);

            return 1;
        }

        changeentityproperty(player, "velocity", speed_x, speed_z, NULL());
    } // fine if spawn

  return 0;
}

// Salta verso x,z ma invece di atterrare finisci l'animazione quando arrivi ad altitude
int jumptoxza(void player, float dir_x, float dir_z, float max_altitude, float altitude, int final_direction) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float speed = getentityproperty(player,"speed");
    float final_x = dir_x, final_z = dir_z;
    int p = player;

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

            if ( getentityproperty(player, "animationid") != openborconstant("ANI_JUMP") ) {
                changeentityproperty(player, "antigravity", 0); // attiviamo la gravità
                if ( a != base ) changeentityproperty(player, "position", x, z, base);
                changeentityproperty(player, "animation", openborconstant("ANI_JUMP"));
            }

            if ( jumpingtoxza(player, dir_x, dir_z, max_altitude, speed*2) ) {
                setlocalvar("half_jump"+p, 1);
            }
        } else {
            changeentityproperty(player, "antigravity", 1);
            //changeentityproperty(player, "no_adjust_base", 1);
            //changeentityproperty(player, "base", altitude);
            if ( jumpingtoxza(player, final_x, final_z, altitude, speed*2) ) {
                if ( a >= altitude && a <= altitude+10 && getentityproperty(player,"animvalid",openborconstant("ANI_JUMPLAND")) ) changeentityproperty(player, "animation", openborconstant("ANI_JUMPLAND"));
                changeentityproperty(player, "velocity", 0, 0, 0);
                changeentityproperty(player, "direction", final_direction);
                changeentityproperty(player, "antigravity", 0);
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

// SPECIAL: no animation change
int sjumptoxza(void player, float dir_x, float dir_z, float max_altitude, float altitude, int final_direction, int dir_flag) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float speed = getentityproperty(player,"speed");
    float final_x = dir_x, final_z = dir_z;
    int p = player;

    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( getlocalvar("start_pos_x"+p) == NULL() )  setlocalvar("start_pos_x"+p, x);
        if ( getlocalvar("start_pos_z"+p) == NULL() )  setlocalvar("start_pos_z"+p, z);
        if ( getlocalvar("half_jump"+p) == NULL() ) setlocalvar("half_jump"+p, 0);

        if ( dir_x > getlocalvar("start_pos_x"+p) ) { // Impostiamo metà strada!!
            dir_x = (dir_x - getlocalvar("start_pos_x"+p))/2 + getlocalvar("start_pos_x"+p);
            if (!dir_flag) changeentityproperty(player, "direction", 1);
        } else {
            dir_x = (getlocalvar("start_pos_x"+p) - dir_x)/2 + dir_x;
            if (!dir_flag) changeentityproperty(player, "direction", 0);
        }
        if ( dir_z > getlocalvar("start_pos_z"+p) ) dir_z = (dir_z - getlocalvar("start_pos_z"+p))/2 + getlocalvar("start_pos_z"+p);
        else dir_z = (getlocalvar("start_pos_z"+p) - dir_z)/2 + dir_z;

        if ( getlocalvar("half_jump"+p) == 0 ) {

            if ( getentityproperty(player, "antigravity") != 1 ) {
                changeentityproperty(player, "antigravity", 1);
                //if ( a != base ) changeentityproperty(player, "position", x, z, base);
            }

            if ( jumpingtoxza(player, dir_x, dir_z, max_altitude, speed*2) ) {
                setlocalvar("half_jump"+p, 1);
            }
        } else {
            changeentityproperty(player, "antigravity", 1);
            //changeentityproperty(player, "no_adjust_base", 1);
            //changeentityproperty(player, "base", altitude);
            if ( jumpingtoxza(player, final_x, final_z, altitude, speed*2) ) {
                changeentityproperty(player, "velocity", 0, 0, 0);
                changeentityproperty(player, "direction", final_direction);
                changeentityproperty(player, "antigravity", 0);
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

// SPECIAL: no animation change ---> Salta e arriva a x,z già a metà salto!!
int sjump_half_toxza(void player, float dir_x, float dir_z, float max_altitude, float altitude, int final_direction, int dir_flag) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float speed = getentityproperty(player,"speed")*3;
    float final_x = dir_x, final_z = dir_z;
    int p = player;

    //drawstring(10,100,0,"var: "+getentityproperty(player,"y"));
    //drawstring(10,110,0,"var: "+altitude);
    //drawstring(10,120,0,"var: "+getlocalvar("half_jump"+p));
    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( getlocalvar("start_pos_x"+p) == NULL() )  setlocalvar("start_pos_x"+p, x);
        if ( getlocalvar("start_pos_z"+p) == NULL() )  setlocalvar("start_pos_z"+p, z);
        if ( getlocalvar("half_jump"+p) == NULL() ) setlocalvar("half_jump"+p, 0);

        if ( dir_x > getlocalvar("start_pos_x"+p) ) { // Impostiamo metà strada!!
            if (!dir_flag) changeentityproperty(player, "direction", 1);
        } else {
            if (!dir_flag) changeentityproperty(player, "direction", 0);
        }

        if ( getlocalvar("half_jump"+p) == 0 ) {

            //drawstring(10,100,0,"var: "+getentityproperty(player,"y"));
            if ( getentityproperty(player, "antigravity") != 1 ) {
                changeentityproperty(player, "antigravity", 1);
                //if ( a != base ) changeentityproperty(player, "position", x, z, base);
            }

            if ( jumpingtoxza(player, dir_x, dir_z, max_altitude, speed) ) {
                setlocalvar("half_jump"+p, 1);
            }
        } else {
            //drawstring(10,100,0,"var: "+getentityproperty(player,"y"));
            changeentityproperty(player, "antigravity", 1);
            //changeentityproperty(player, "no_adjust_base", 1);
            //changeentityproperty(player, "base", altitude);
            if ( jumpingtoxza(player, final_x, final_z, altitude, speed) ) {
                changeentityproperty(player, "velocity", 0, 0, 0);
                changeentityproperty(player, "direction", final_direction);
                changeentityproperty(player, "antigravity", 0);
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

// SPECIAL: no animation change ---> Salta atterrando su x,z con speed
int sjumptoxz(void player, float dir_x, float dir_z, float max_altitude, int final_direction, float speed, int dir_flag) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float final_x = dir_x, final_z = dir_z;
    int p = player;

    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( getlocalvar("start_pos_x"+p) == NULL() )  setlocalvar("start_pos_x"+p, x);
        if ( getlocalvar("start_pos_z"+p) == NULL() )  setlocalvar("start_pos_z"+p, z);
        if ( getlocalvar("half_jump"+p) == NULL() ) setlocalvar("half_jump"+p, 0);
        //if ( speed == 0 ) speed = getentityproperty(player,"speed");

        if ( dir_x > getlocalvar("start_pos_x"+p) ) { // Impostiamo metà strada!!
            dir_x = (dir_x - getlocalvar("start_pos_x"+p))/2 + getlocalvar("start_pos_x"+p);
            if (!dir_flag) changeentityproperty(player, "direction", 1);
        } else {
            dir_x = (getlocalvar("start_pos_x"+p) - dir_x)/2 + dir_x;
            if (!dir_flag) changeentityproperty(player, "direction", 0);
        }
        if ( dir_z > getlocalvar("start_pos_z"+p) ) dir_z = (dir_z - getlocalvar("start_pos_z"+p))/2 + getlocalvar("start_pos_z"+p);
        else dir_z = (getlocalvar("start_pos_z"+p) - dir_z)/2 + dir_z;

        if ( getlocalvar("half_jump"+p) == 0 ) {

            if ( getentityproperty(player, "antigravity") != 1 ) {
                changeentityproperty(player, "antigravity", 1);
                //if ( a != base ) changeentityproperty(player, "position", x, z, base);
            }

            if ( jumpingtoxz_speed(player, dir_x, dir_z, max_altitude, speed) ) {
                setlocalvar("half_jump"+p, 1);
            }
        } else {
            changeentityproperty(player, "antigravity", 0);
            if ( jumpingtoxz_speed(player, final_x, final_z, base, speed) ) {
                changeentityproperty(player, "velocity", 0, 0, 0);
                changeentityproperty(player, "direction", final_direction);
                changeentityproperty(player, "antigravity", 0);
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

// smoothed movement
int jumpingtoxz_smoothed(void player, float dir_x, float dir_z, float max_altitude, float jump_speed) { // max_altitude <= 0 indica landing
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed_x, speed_z;
    float max_dist = 0, x_dist, z_dist, a_dist, mdf_speedx, mdf_speedz, mdf_speeda;

    // Calcola quale coord è in progresso: x,z,a?? condizioni di speed_x == 0 x es. if ( x > dir_x-threshold && x < dir_x+threshold )
    // Dunque impostiamo tutte le distanze. Se la distanza == 0 allora è stata raggiunta
    if ( x < dir_x+threshold ) x_dist = abs(dir_x-x);
    else if ( x > dir_x-threshold ) x_dist = abs(x-dir_x);
    else x_dist = 0;

    if ( z < dir_z+threshold ) z_dist = abs(dir_z-z);
    else if ( z > dir_z-threshold ) z_dist = abs(z-dir_z);
    else z_dist = 0;

    if ( a < max_altitude+threshold ) a_dist = abs(max_altitude-a);
    else if ( a > max_altitude-threshold ) a_dist = abs(a-max_altitude);
    else a_dist = 0;

    // Poi calcolare dist max. da x a dir_x per es.
    if ( x_dist > 0 ) max_dist = x_dist;
    if ( z_dist > max_dist ) max_dist = z_dist;
    if ( a_dist > max_dist ) max_dist = a_dist;
    if ( max_dist <= 0 ) max_dist = 1;

    // Associare speed a dist min. Stabilire attraverso una percentuale la velocità per le altre distanze.
    // L'associacione avviene tramite proporzione -> speed:max_dist = new_speed:new_dist -> max_dist*new_speed = speed*new_dist
    // Ovviamente la speed == max_speed rimarrà immodificata se dist/max_dist = 1
    mdf_speedx = (x_dist*jump_speed)/max_dist;
    mdf_speedz = (z_dist*jump_speed)/max_dist;
    mdf_speeda = (a_dist*jump_speed)/max_dist;

    if ( x > dir_x-threshold && x < dir_x+threshold ) {
        speed_x = 0;
    } else if ( x < dir_x+threshold ) { // pg deve andare a destra
        speed_x = mdf_speedx;
        if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3);
    } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
        speed_x = -1*mdf_speedx;
        if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
    }

    if ( z > dir_z-threshold && z < dir_z+threshold ) {
        speed_z = 0;
    } else if ( z < dir_z+threshold ) { // pg deve andare giù
        speed_z = mdf_speedz;
        if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
    } else if ( z > dir_z-threshold ) { // pg deve andare su
        speed_z = -1*mdf_speedz;
        if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
    }

    /*drawstring( 240,230,0,"test: " );
    if ( max_altitude <= base && a > max_altitude-threshold && a < max_altitude+threshold ) {
        //mdf_speeda = 0;
    }*/

    if ( max_altitude > base && a >= max_altitude ) mdf_speeda = 0; // Raggiunta altezza max quindi non salire più.
    else if ( a <= base && max_altitude <= base ) mdf_speeda = 0;

    if ( max_altitude > base ) {
        if ( speed_x == 0 && speed_z == 0 && a >= max_altitude ) {
            return 1;
        }
    } else {
        if ( speed_x == 0 && speed_z == 0 && a <= base ) {
            return 1;
        }
    }


    if ( max_altitude > base ) changeentityproperty(player, "velocity", speed_x, speed_z, mdf_speeda);
    else changeentityproperty(player, "velocity", speed_x, speed_z, -1*mdf_speeda);

  return 0;
}

// ----- SMOOTHED+SPEED ----- SPECIAL: no animation change ---> Salta atterrando su x,z
int sjumptoxz_smoothed(void player, float dir_x, float dir_z, float max_altitude, int final_direction, float speed, int dir_flag) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float final_x = dir_x, final_z = dir_z;
    int p = player;

    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( getlocalvar("start_pos_x"+p) == NULL() )  setlocalvar("start_pos_x"+p, x);
        if ( getlocalvar("start_pos_z"+p) == NULL() )  setlocalvar("start_pos_z"+p, z);
        if ( getlocalvar("half_jump"+p) == NULL() ) setlocalvar("half_jump"+p, 0);
        //if ( speed == 0 ) speed = getentityproperty(player,"speed");

        if ( dir_x > getlocalvar("start_pos_x"+p) ) { // Impostiamo metà strada!!
            dir_x = (dir_x - getlocalvar("start_pos_x"+p))/2 + getlocalvar("start_pos_x"+p);
            if (!dir_flag) changeentityproperty(player, "direction", 1);
        } else {
            dir_x = (getlocalvar("start_pos_x"+p) - dir_x)/2 + dir_x;
            if (!dir_flag) changeentityproperty(player, "direction", 0);
        }
        if ( dir_z > getlocalvar("start_pos_z"+p) ) dir_z = (dir_z - getlocalvar("start_pos_z"+p))/2 + getlocalvar("start_pos_z"+p);
        else dir_z = (getlocalvar("start_pos_z"+p) - dir_z)/2 + dir_z;

        if ( getlocalvar("half_jump"+p) == 0 ) {

            if ( getentityproperty(player, "antigravity") != 1 ) {
                changeentityproperty(player, "antigravity", 1);
                //if ( a != base ) changeentityproperty(player, "position", x, z, base);
            }

            if ( jumpingtoxz_smoothed(player, dir_x, dir_z, max_altitude, speed) ) {
                setlocalvar("half_jump"+p, 1);
            }
        } else {
            changeentityproperty(player, "antigravity", 0);
            if ( jumpingtoxz_smoothed(player, final_x, final_z, base, speed) ) {
                changeentityproperty(player, "velocity", 0, 0, 0);
                changeentityproperty(player, "direction", final_direction);
                changeentityproperty(player, "antigravity", 0);
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

// MOVE SMOOTHED to x,z - SPECIAL: no animation change + speed
int smovetoxz_smoothed(void player, float dir_x, float dir_z, int final_direction, float speed) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float threshold = 0.99; // precisione
    float speed_x, speed_z;
    float max_dist = 0, x_dist, z_dist, mdf_speedx, mdf_speedz;

    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        if ( a != base ) changeentityproperty(player, "position", x, z, base);

        // Calcola quale coord è in progresso: x,z,a?? condizioni di speed_x == 0 x es. if ( x > dir_x-threshold && x < dir_x+threshold )
        // Dunque impostiamo tutte le distanze. Se la distanza == 0 allora è stata raggiunta
        if ( x < dir_x+threshold ) x_dist = abs(dir_x-x);
        else if ( x > dir_x-threshold ) x_dist = abs(x-dir_x);
        else x_dist = 0;

        if ( z < dir_z+threshold ) z_dist = abs(dir_z-z);
        else if ( z > dir_z-threshold ) z_dist = abs(z-dir_z);
        else z_dist = 0;

        // Poi calcolare dist max. da x a dir_x per es.
        if ( x_dist > 0 ) max_dist = x_dist;
        if ( z_dist > max_dist ) max_dist = z_dist;

        // Associare speed a dist min. Stabilire attraverso una percentuale la velocità per le altre distanze.
        // L'associacione avviene tramite proporzioe -> speed:max_dist = new_speed:new_dist
        // Ovviamente la speed == a max_speed rimarrà immodificata se dist/max_dist = 1
        mdf_speedx = (x_dist*speed)/max_dist;
        mdf_speedz = (z_dist*speed)/max_dist;
        if ( max_dist <= 0 ) max_dist = 1;

        if ( x > dir_x-threshold && x < dir_x+threshold  ) {
            speed_x = 0;
        } else if ( x < dir_x+threshold ) { // pg deve andare a destra
            changeentityproperty(player, "direction", 1);
            speed_x = mdf_speedx;
            if ( (dir_x-x) <= threshold ) speed_x = (dir_x-x)*(2/3); // Impostiamo la velocità proprio uguale alla distanza che manca
        } else if ( x > dir_x-threshold ) { // pg deve andare a sinistra
            changeentityproperty(player, "direction", 0);
            speed_x = -1*mdf_speedx;
            if ( (x-dir_x) <= threshold ) speed_x = -1*(x-dir_x)*(2/3);
        }

        // Diamo la priorità all'animazione UP/DOWN
        if ( z > dir_z-threshold && z < dir_z+threshold  ) {
            speed_z = 0;
        } else if ( z < dir_z+threshold ) { // pg deve andare giù
            speed_z = mdf_speedz;
            if ( (dir_z-z) <= threshold ) speed_z = (dir_z-z)*(2/3);
        } else if ( z > dir_z-threshold ) { // pg deve andare su
            speed_z = -1*mdf_speedz;
            if ( (z-dir_z) <= threshold ) speed_z = -1*(z-dir_z)*(2/3);
        }

        if ( speed_x == 0 && speed_z == 0 ) {
            changeentityproperty(player, "velocity", speed_x, speed_z, NULL());
            changeentityproperty(player, "direction", final_direction);

            return 1;
        }

        changeentityproperty(player, "velocity", speed_x, speed_z, NULL());
    } // fine if spawn

  return 0;
}

int jumptoxz_bezier_anim(void player, float dir_x, float dir_z, float max_altitude, int final_direction, float bezier_time_speed, int wall_deactive_flag) {
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float speed = getentityproperty(player,"speed");
    float final_x = dir_x, final_z = dir_z;
    int p = player;
    int anim_id = getentityproperty(player, "animationid");

    //if ( x == dir_x && z == dir_z ) return 1; // avoid loop
    if ( anim_id != openborconstant("ANI_SPAWN") && anim_id != openborconstant("ANI_RESPAWN") ) {
        float t = getlocalvar("bezier_time"+p);
        int lost_frame = get_lost_frames()+1;

        if ( getlocalvar("start_pos_x"+p) == NULL() )  setlocalvar("start_pos_x"+p, x);
        if ( getlocalvar("start_pos_z"+p) == NULL() )  setlocalvar("start_pos_z"+p, z);
        if ( getlocalvar("max_altitude"+p) == NULL() )  setlocalvar("max_altitude"+p, max_altitude);
        if ( getlocalvar("start_base"+p) == NULL() )  setlocalvar("start_base"+p, base);

        // set the half coords
        if ( getlocalvar("half_pos_z"+p) == NULL() ) {
            if ( final_x > getlocalvar("start_pos_x"+p) ) {
                setlocalvar("half_pos_x"+p,(final_x - getlocalvar("start_pos_x"+p))/2 + getlocalvar("start_pos_x"+p));
                changeentityproperty(player, "direction", 1);
            } else {
                setlocalvar("half_pos_x"+p,(getlocalvar("start_pos_x"+p) - final_x)/2 + final_x);
                changeentityproperty(player, "direction", 0);
            }
            if ( final_z > getlocalvar("start_pos_z"+p) ) setlocalvar("half_pos_z"+p,(final_z - getlocalvar("start_pos_z"+p))/2 + getlocalvar("start_pos_z"+p));
            else setlocalvar("half_pos_z"+p,(getlocalvar("start_pos_z"+p) - final_z)/2 + final_z);
        }

        // time setting
        if ( t == NULL() ) setlocalvar("bezier_time"+p,0);
        else setlocalvar("bezier_time"+p,getlocalvar("bezier_time"+p)+bezier_time_speed); // *lost_frame
        if ( getlocalvar("bezier_time"+p) < 0 ) setlocalvar("bezier_time"+p,0);
        if ( getlocalvar("bezier_time"+p) > 1 ) setlocalvar("bezier_time"+p,1);
        t = getlocalvar("bezier_time"+p);

        if ( gep(player,"animvalid",oc("ANI_JUMPDELAY")) ) {
            if ( anim_id != oc("ANI_JUMPDELAY") && glv("jump_delay_performed"+p) == NULL() ) {
                //cep(player,"animation",oc("ANI_JUMPDELAY"));
                performattack(player,oc("ANI_JUMPDELAY"),1);
                slv("jump_delay_performed"+p,1);
                return 0;
            }
            if ( glv("jump_delay_performed"+p) == 1 && !gep(player,"aiflag","idling") && anim_id == oc("ANI_JUMPDELAY") ) {
                slv("bezier_time"+p,NULL());
                t = glv("bezier_time"+p);
                return 0;
            } else if ( glv("jump_delay_performed"+p) == 1 ) slv("jump_delay_performed"+p,2);
            //drawstring(10,180+gep(player,"playerindex")*10,0,gep(player,"name")+": "+gep(player,"aiflag","idling"));
        }

        /// CHECK FINAL BASE
        if ( t == 0 && wall_deactive_flag ) {
            if ( getentityproperty(player, "subject_to_wall") ) changeentityproperty(player, "subject_to_wall", 0);
            if ( getlocalvar("final_base"+p) == NULL() ) setlocalvar("final_base"+p,0);
        } else if ( t == 0 && (!wall_deactive_flag || wall_deactive_flag == NULL()) ) {
            if ( getlocalvar("final_base"+p) == NULL() ) {
                float t_speed = 1/100;
                float ii = 0;

                setlocalvar("final_base"+p,checkwall(final_x,final_z));
                for ( ii = 0.5; ii <= 1; ii += t_speed ) {
                    void platform = NULL();

                    x = bezier_curve(ii,2,getlocalvar("start_pos_x"+p),getlocalvar("half_pos_x"+p),final_x);
                    z = bezier_curve(ii,2,getlocalvar("start_pos_z"+p),getlocalvar("half_pos_z"+p),final_z);
                    a = bezier_curve(ii,2,getlocalvar("start_base"+p),getlocalvar("max_altitude"+p),getlocalvar("final_base"+p));

                    platform = find_platform_in_a(x,z,a,MAX_ALTITUDE);
                    if ( platform != NULL() ) {
                        float platform_a = getentityproperty(platform,"height");
                        float platform_height = getentityproperty(platform,"height");

                        if ( platform_height == NULL() ) platform_height = 0;
                        setlocalvar("final_base"+p,platform_a+platform_height);

                        break;
                    }
                }
            }
        }

        // bezier curve (quadratic)
        x = bezier_curve(t,2,getlocalvar("start_pos_x"+p),getlocalvar("half_pos_x"+p),final_x);
        z = bezier_curve(t,2,getlocalvar("start_pos_z"+p),getlocalvar("half_pos_z"+p),final_z);
        a = bezier_curve(t,2,getlocalvar("start_base"+p),getlocalvar("max_altitude"+p),getlocalvar("final_base"+p));
        changeentityproperty(player, "position", x, z, a);
        /*drawstring(10,100,0,"x: "+x);
        drawstring(10,110,0,"z: "+z);
        drawstring(10,120,0,"a: "+a);
        drawstring(10,130,0,"t: "+t);*/

        // set animations
        if ( t < 1 && a != base && getlocalvar("anim"+p) == NULL() ) {
            if ( gep(player, "defaultmodel") == "FOOT_SOLDIER" && gep(player,"animvalid",oc("ANI_FOLLOW80") ) ) {
                if ( gep(player, "animationid") != oc("ANI_FOLLOW80") ) cep(player, "animation", oc("ANI_FOLLOW80"));
            } else {
                if ( gep(player, "animationid") != oc("ANI_JUMP") && gep(player,"animvalid",oc("ANI_JUMP")) ) cep(player, "animation", oc("ANI_JUMP"));
            }
            setlocalvar("anim"+p,1);
        }

        // at start
        if ( t <= 0 ) {
            changeentityproperty(player, "antigravity", 1);
        } else if ( t >= 1 ) { // at end
            if ( a >= base && a <= base+10 && gep(player,"animvalid",oc("ANI_JUMPLAND")) ) {
                if ( glv("jump_land_performed"+p) == NULL() ) {
                    //cep(player, "animation", oc("ANI_JUMPLAND"));
                    performattack(player,oc("ANI_JUMPLAND"),1);
                    slv("jump_land_performed"+p,1);
                    return 0;
                } else if ( glv("jump_land_performed"+p) == 1 ) {
                    if ( anim_id == oc("ANI_JUMPLAND") && !gep(player,"aiflag","idling") && anim_id == oc("ANI_JUMPLAND") ) return 0;
                }
            }

            changeentityproperty(player, "velocity", 0, 0, 0);
            changeentityproperty(player, "direction", final_direction);
            changeentityproperty(player, "antigravity", 0);

            setlocalvar("start_pos_x"+p, NULL());
            setlocalvar("start_pos_z"+p, NULL());
            setlocalvar("half_pos_x"+p, NULL());
            setlocalvar("half_pos_z"+p, NULL());
            setlocalvar("max_altitude"+p, NULL());
            setlocalvar("start_base"+p, NULL());
            setlocalvar("final_base"+p, NULL());
            setlocalvar("bezier_time"+p, NULL());
            setlocalvar("jump_delay_performed"+p,NULL());
            setlocalvar("jump_land_performed"+p,NULL());
            setlocalvar("anim"+p, NULL());

            return 1;
        }
    } // fine if spawn

    return 0;
}

// speed: greater value == speedy, jump_speed == y speed, theta == shot angle
int parabolic_move(void player, int final_direction, float speed, float gravity, float theta, float stop_base) {
    if ( getentityproperty(player, "animationid") != openborconstant("ANI_SPAWN") && getentityproperty(player, "animationid") != openborconstant("ANI_RESPAWN") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        float dist_x;
        //int p = player;
        float denom = 1, y_parabolic = 0;
        int time = openborvariant("elapsed_time"), context_time;
        float range, fall_range, total_range, jump_speed;

        // ES.

        // Nuovi parametri (primi 2 con no 1/speed):
        /*
        float theta = 70; // theta == 0 è l'ascissa -1 mentre 90 è in alto (ordinata 1)
        float jump_speed = 0.6; //    2 -   0.5 -   0.6 -   1.6
        float speed = 1.4;      //  0.5 -   0.5 -   1.4 -   1.6
        float gravity = 0.004;  // 0.05 - 0.004 - 0.004 - 0.004
        */
        jump_speed = 1/speed;

        if ( getentityproperty(player, "antigravity") != 1 ) changeentityproperty(player, "antigravity", 1);

        // Conserviamo le coords iniziali e consideriamo sempre quelle
        if ( getlocalvar("__init_x") == NULL() ) setlocalvar("__init_x", x);
        if ( getlocalvar("__init_a") == NULL() ) setlocalvar("__init_a", a);
        if ( getlocalvar("__init_time") == NULL() || time < getlocalvar("__init_time") ) setlocalvar("__init_time", time);

        context_time = time - getlocalvar("__init_time");
        a = getlocalvar("__init_a");

        // Calcolo dell'angolo theta
        if ( final_direction == 0 ) theta = -1*theta;

        // Gittata
        if ( final_direction == 0 ) x = getlocalvar("__init_x")-context_time/speed;
        else x = getlocalvar("__init_x")+context_time/speed;

        // Gittata calcolata: funziona su altezze uguali
        //range = (qrt(jump_speed)*sin(2*theta))/gravity; // sin(theta)*cos(theta) == sin(2*theta) --> 1/speed perchè quell'1 è un frame.
        // Poi dobbiamo sommare la gittata di un corpo in caduta libera nel vuoto (no frizioni)
        /*
        v(f)^2 = 2*g*h, quindi delta(v)/(2*g) = fall_range
        */
        //fall_range = ((2*gravity*getlocalvar("init_a"+p))-qrt(jump_speed))/(2*gravity);
        //total_range = abs(range)+fall_range;

        // Distanza percorsa
        if ( x > getlocalvar("__init_x") ) dist_x = x-getlocalvar("__init_x");
        else dist_x = getlocalvar("__init_x")-x;
        if ( final_direction == 0 ) dist_x = -1*dist_x;

        // Movimento parabolico su y
        denom = 2*qrt(jump_speed)*qrt(cos(theta));
        y_parabolic = dist_x*tan(theta)-(gravity/denom)*qrt(dist_x);

        a = a + y_parabolic;
        changeentityproperty(player, "position", x, NULL(), a);

        //float max_altitude = (qrt(jump_speed)*qrt(sin(theta)))/(2*gravity);

        /*drawstring(10,90,0,"y_parabolic: "+y_parabolic);
        drawstring(10,100,0,"x: "+x);
        drawstring(10,110,0,"a: "+a);
        drawstring(10,120,0,"theta: "+theta);
        drawstring(10,130,0,"dist_x: "+dist_x);*/
        /*drawstring(10,70,0,"dist_x: "+dist_x);
        drawstring(10,80,0,"range: "+range);
        drawstring(10,90,0,"fall_range: "+fall_range);
        drawstring(10,100,0,"total_range: "+total_range);
        drawstring(10,110,0,"max_altitude: "+(getlocalvar("__init_a")+max_altitude));*/

        if ( base+y_parabolic > stop_base || y_parabolic < 0 ) setlocalvar("__init_jump",1);

        if ( a <= stop_base && getlocalvar("__init_jump") > 0 ) {
            //changeentityproperty(player, "antigravity", 0);
            //cv("textbox","aa: "+getlocalvar("init_a"+p));
            setlocalvar("__init_x", NULL());
            setlocalvar("__init_a", NULL());
            setlocalvar("__init_time", NULL());
            setlocalvar("__init_jump", NULL());

            return 1;
            //if ( final_direction == 0 ) return ( getlocalvar("init_x"+p)-x );
            //else return ( x-getlocalvar("init_x"+p) );
        }
    } // fine if spawn

  return 0;
}

int get_parabolic_range(void player, int final_direction, float speed, float gravity, float theta, float altitude) {
    float range, fall_range, total_range, jump_speed;

    jump_speed = 1/speed;

    // Calcolo dell'angolo theta
    if ( final_direction == 0 ) theta = -1*theta;

    // Gittata calcolata: funziona su altezze uguali
    range = (qrt(jump_speed)*sin(2*theta))/gravity; // sin(theta)*cos(theta) == sin(2*theta) --> 1/speed perchè quell'1 è un frame.
    // Poi dobbiamo sommare la gittata di un corpo in caduta libera nel vuoto (no frizioni)
    /*
    v(f)^2 = 2*g*h, quindi delta(v)/(2*g) = fall_range
    */
    fall_range = ((2*gravity*altitude)-qrt(jump_speed))/(2*gravity);
    total_range = abs(range)+fall_range;

    return total_range;
}

int get_parabolic_velocity(float dist, float gravity, float theta, float altitude) {
    float velocity;

    // From total-range
    velocity = (2*gravity*(dist-altitude))/(2*sin(2*theta)-1);
    velocity = sqrt(velocity);

    return velocity;
}

int get_parabolic_angle(float dist, float gravity, float speed, float altitude) {
    float theta;

    // From total-range
    theta = 0.5 + gravity*(dist-altitude)/qrt(speed);
    theta = 2*arcsin(theta)*arccos(theta); // 2*cos(theta)*sin(theta) == sin(2*theta)

    return theta;
}

int get_normalized_velocity(float dist, float gravity, float theta, float altitude) {
    float velocity, diff, referral_vel = 1.66;

    velocity = get_parabolic_velocity(dist,gravity,theta,altitude);
    velocity = 1/velocity; // perchè time/vel == 1 frame / vel
    diff = referral_vel - velocity; // otteniamo la differenza
    velocity = referral_vel - diff; // decrementiamo il valore di riferimento della differenza

    return velocity;
}

