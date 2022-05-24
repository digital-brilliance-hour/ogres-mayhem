
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

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

    check_hitbyid(self,damagetaker);
    check_versus_mark(self,damagetaker);
}

int check_versus_mark(void self, void damagetaker, int parent_flag) {

    if ( getglobalvar("1VS1") ) {
        void opp = getentityproperty(self, "opponent");
        int health = getentityproperty(self,"health");
        int ohealth;

        if ( self == damagetaker ) {
            damagetaker = opp;
            if ( !getentityproperty(opp, "exists") ) return -1;
        }

        ohealth = getentityproperty(damagetaker,"health");

        if ( !getentityproperty(self, "exists") ) return -1;
        if ( !getentityproperty(damagetaker, "exists") ) return -1;

        if ( getentityproperty(damagetaker,"aiflag","dead") || ohealth <= 0 ) {
            int hres = openborvariant("hresolution");
            int vres = openborvariant("vresolution");
            float xpos = openborvariant("xpos");
            float ypos = openborvariant("ypos");
            int p = getentityproperty(self, "playerindex");
            //void player = getplayerproperty(p, "entity");
            //int type = getentityproperty(self,"type");
            void ent;
            int wins_num = 0;

            if ( getentityvar(damagetaker,"weapon") == 1 ) return 0;

            if ( parent_flag != NULL() && parent_flag >= 1 ) {
                void parent = get_parent(self);
                if ( getentityproperty(parent,"exists") ) p = getentityproperty(parent, "playerindex");
                self = parent;
            }

            if ( getglobalvar("versus_mark_"+p) == NULL() ) setglobalvar("versus_mark_"+p,0);
            wins_num = getglobalvar("versus_mark_"+p);
            ent = spawnsubentity("versus_mark",xpos+p*82-2+(wins_num%4)*17,ypos+32+truncA(wins_num/4)*17,NULL());
            changeentityproperty(ent,"parent",self);
            setglobalvar("versus_mark_"+p,getglobalvar("versus_mark_"+p)+1);

            return 1;
        }
    }

    return 0;
}

int check_hitbyid(void self, void damagetaker) {
    int anim_id = getentityproperty(self,"animationid");
    int default_model = getentityproperty(self,"defaultmodel");

    // pizzaapow, bindatk, holdrope, (mike nunchaku)
    if ( anim_id == openborconstant("ANI_FREESPECIAL10") || anim_id == openborconstant("ANI_FOLLOW22") || anim_id == openborconstant("ANI_FREESPECIAL41") ) add_hitbyid_list(self,"opp_list",damagetaker);
    else if ( default_model == "MIKE" && anim_id == openborconstant("ANI_FREESPECIAL34") ) add_hitbyid_list(self,"opp_list",damagetaker);
}

/*int reset_hitbyid_pizzapow(void self, void damagetaker) {
    int anim_id = getentityproperty(self,"animationid");

    if ( anim_id == openborconstant("ANI_FREESPECIAL10") ) {
        if ( getentityproperty(damagetaker,"exists") ) changeentityproperty(damagetaker,"hitbyid",0);
    }
}*/
