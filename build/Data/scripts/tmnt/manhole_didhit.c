
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/endlevel_anim.c"

void main() {
    void self = getlocalvar("self");
    int damage = getlocalvar("damage");
    int drop = getlocalvar("drop");
    int attacktype = getlocalvar("attacktype");
    void damagetaker = getlocalvar("damagetaker");

    /*
    self: Caller.
    damagetaker: Recipient of attack or item.
    damage: attack damage. "
    drop: knockdown power.
    attacktype: attack type, see 'openborconstant'.
    noblock: block break force of attack.
    guardcost: Guardcost of attack.
    jugglecost: Jugglecost of attack.
    pauseadd: Pause value of attack.
    blocked: Receiving entity did (1) or did not (0) block attack.
    */

    add_hitbyid_list(self,"opp_list",damagetaker);
    get_object(self,damagetaker);
}

int get_object(void self, void player) {
            //drawstring( 20,180,0,"Manhole1:  "+"   x:  "+x+"   z:  "+z+ "   a:  "+a );
            float x = getentityproperty(self, "x");
            float z = getentityproperty(self, "z");
            float a = getentityproperty(self, "y");
            float xp, zp, ap;

            xp = getentityproperty(player, "x");
            zp = getentityproperty(player, "z");
            ap = getentityproperty(player, "y");

            // ATTENZIONE A QUESTO "y" == 0 SE SI VOGLIONO BUCHI IN ALTO!!
            if ( xp > x-30 && xp < x+30 && zp > z-20 && zp < z+10 && a <= ap+T_GENERIC_WALKOFF ) {
                int anim_id = getentityproperty(player, "animationid");

                if ( getentityproperty(player, "invincible") == 0 && getentityproperty(player, "vulnerable") != 0 && anim_id != openborconstant("ANI_PAIN24") && anim_id != openborconstant("ANI_DIE24") ) {
                    if ( !is_in_pain(player) ) {
                        void defaultmodel = getentityproperty(self, "defaultmodel");
                        int health = getentityproperty(player,"health");
                        int damage = 20;

                        if ( isATurtle(player) ) {
                            // IMPORTANTISSIMO: noaicontrol + cambio d'animazione funziona solo se lo script viene ripetuto a ciclo ad ogni frame!!
                            //changeentityproperty(player, "noaicontrol", 1); // lo facciamo perchè con qualche mossa speciale altrimenti può evitare di cadere

                            changeentityproperty(player, "position", x+0, z-7, a+0);
                            changeentityproperty(player, "base", a+0);

                            // SUPER IMPORTANTISSIMO: Se non imposti la velocity a 0 su tutti i parametri, il player continuerà a camminare anche dopo noaicontrol impostato a vero!!
                            changeentityproperty(player, "velocity", 0, 0, 0);
                            changeentityproperty(player, "aiflag", "jumping", 0);
                            changeentityproperty(player, "aiflag", "running", 0);
                            if ( defaultmodel == "manhole_sx" ) changeentityproperty(player, "direction", 0); // <-------- FORCE DIRECTION!!
                            else if ( defaultmodel == "manhole_dx" ) changeentityproperty(player, "direction", 1);

                            // Con il seguente metodo dato dalle seguenti 2 funzioni, ci assicuriamo di far perdere una somma COSTANTE di HP al player...
                            //changeentityproperty( player,"health",getentityproperty(player,"health")-20 );
                            damageentity(player,self,damage,0,openborconstant("ATK_NORMAL24"));
                            changeentityproperty(player, "hitbyid", 0);
                            changeentityproperty(player, "aiflag", "inpain", 1);
                            //if ( getentityproperty(player,"health") <= 0 ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW21"));
                            //else performattack(player, openborconstant("ANI_FOLLOW20"), 1);

                            playsample(loadsample("data/sounds/falling_cave.wav"));
                        } // fine if defaultmodel
                    } // fine if dead/pain/fall
                } // fine if invincible
            } // fine if coords
}
