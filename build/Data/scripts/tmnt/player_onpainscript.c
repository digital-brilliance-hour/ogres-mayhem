
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"


void main() {
    void self = getlocalvar("self");
    int reset = getlocalvar("reset");
    int attacktype = getlocalvar("attacktype");

    cep(self,"animation",oc("ANI_ATTACK1"));
}
