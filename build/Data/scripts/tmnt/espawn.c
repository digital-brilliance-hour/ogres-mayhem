
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"
#import "data/scripts/spawn.c"

void main() {

    void self = getlocalvar("self");

    setentityvar(self,1,NULL());
    setentityvar(self,2,NULL());
    setentityvar(self,3,NULL());

    if ( getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 ) { // Waiting, Select
        set_board(self);
    }

    set_candamage(self,"TYPE_NPC"); // to allow to hit the weapons

    set_snowball_on(self);
}

void set_snowball_on(void self) {
    char defaultmodel = gep(self,"defaultmodel");

    if ( defaultmodel == "foot_snowflood_follower" ) {
        float x = gep(self,"x");
        float z = gep(self,"z");
        float y = gep(self,"y");
        int dir = gep(self,"direction");
        int candamage;
        int type_enemy = oc("TYPE_ENEMY");
        float xdir = 1.1, alt = 46.0;
        void subent;

        cep(self,"noaicontrol",1);
        cep(self,"no_adjust_base",1);
        cep(self,"base",alt);

        subent = spawnsubentity_parent("p_snowflood",x,z,y,self);
        cep(subent,"direction",dir);

        if ( !dir ) xdir *= -1;
        cep(subent,"velocity",xdir,NULL(),NULL());
        sev(subent,"no_bouncing",1);
        sev(subent,"binded_weapon",self);
        bindentity(self,subent,0,1,alt,1,0);
        if ( xdir > 0 ) cep(subent,"animation",openborconstant("ANI_FOLLOW1"));


        /*candamage = gep(subent,"candamage");
		if ( !is_flag_on(candamage,type_enemy)  ) {
			cep(subent,"candamage",candamage+type_enemy);
		}*/
    }

    return;
}
