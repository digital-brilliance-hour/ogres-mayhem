
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

//#import "data/scripts/endlevel_anim.c"

void main() {
    void self = getlocalvar("self");
    int damage = getlocalvar("damage");
    int drop = getlocalvar("drop");
    void other = getlocalvar("other"); // When called on attacker, this recipient of attack. When called on defender, this is the attacker.
    int attacktype = getlocalvar("attacktype");
    int which = getlocalvar("which"); // 0 = Caller is defender. 1 = Caller is attacker.
    int attackid = getlocalvar("attackid");
    int pauseadd = getlocalvar("pauseadd");
    int guardcost = getlocalvar("guardcost");
    int jugglecost = getlocalvar("jugglecost");

    check_weapon(self,other,which);
}

void check_weapon(void self, void other, int which) {
    if ( which && getentityproperty(other,"exists") ) { // other is the opp!!
            int type = getentityproperty(other,"type");

            if ( type != oc("TYPE_PLAYER") ) {
                changeopenborvariant("lasthitc",0); //nullify last hit collision!!
            }
    }
}
