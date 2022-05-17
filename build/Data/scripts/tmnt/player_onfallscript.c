
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

void main() {
    void self = getlocalvar("self");
    void attacker = getlocalvar("attacker");
    int damage = getlocalvar("damage");
    int drop = getlocalvar("drop");
    int attacktype = getlocalvar("attacktype");
    int noblock = getlocalvar("noblock");
    int pauseadd = getlocalvar("pauseadd");
    int guardcost = getlocalvar("guardcost");
    int jugglecost = getlocalvar("jugglecost");

    cep(self,"animation",oc("ANI_FALL52"));
}
