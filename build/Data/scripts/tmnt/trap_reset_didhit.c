
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
}
