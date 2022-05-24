
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/player_backpain.c"
#import "data/scripts/player_didhit.c"

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

    //changeentityproperty(self,"aiflag","running",0);
    //changeentityproperty(self,"aiflag","jumping",0);

    setentityvar(self,28,damage);

    // (damaged/block)
    if ( drop > 0 ) setentityvar(self,18,drop);
    else setentityvar(self,18,-1);

    setoppx_ondmg(self,attacker); // backpains
    check_pain_on_splatgrab(self); // rialza il foot se la tartaruga viene colpita durante splatgrab

    check_vs_mark(self,attacker,attacktype);
}

int check_vs_mark(void self, void attacker, int attacktype) {
    //int prev_animid = get_prev_anim(self);
    int prev_animid = gep(self,"animationid");

    if ( prev_animid == oc("ANI_PAIN71") && attacktype == oc("ATK_NORMAL71") ) check_versus_mark(attacker,self);
}

int setoppx_ondmg(void player, void attacker) {
    setentityvar(player, 6, getentityproperty(attacker, "x") );
}

void check_pain_on_splatgrab(void self) {
    int anim_id = gep(self,"animationid");
    //void opp = gep(self,"opponent");

    if ( anim_id == oc("ANI_FOLLOW40") ) {
        int p = getentityproperty(self, "playerindex");
        int dir = gep(self,"direction");
        void grabbed_opp = gev(self,22);

        if ( ggv("attack_button"+p) != NULL() && gep(ggv("attack_button"+p),"exists") ) killentity(ggv("attack_button"+p));

        if ( gep(grabbed_opp,"exists") ) {
                if ( !dir ) cep(grabbed_opp,"direction",1);
                else cep(grabbed_opp,"direction",0);

               	changeentityproperty(grabbed_opp, "antigravity", 0);
               	changeentityproperty(grabbed_opp, "noaicontrol", 0);
                bindentity(grabbed_opp,NULL());
                damageentity(grabbed_opp,grabbed_opp,0,0,oc("ATK_NORMAL9"));
                cep(grabbed_opp, "aiflag", "falling", 1); // Forziamo il falling
                cep(grabbed_opp, "animation", oc("ANI_FOLLOW29"));
                setentityvar(grabbed_opp,1,NULL());
                setentityvar(grabbed_opp,3,NULL());
                     if ( check_obstacles(grabbed_opp,0,0,0) ) {
                        float x = getentityproperty(self,"x");
                        float z = getentityproperty(self,"z");
                        float ox = getentityproperty(grabbed_opp,"x");
                        float oz = getentityproperty(grabbed_opp,"z");

                        changeentityproperty(grabbed_opp,"position",x,z,NULL());
                        /*set_safe_path(grabbed_opp,ox,oz,1,1,"x_safe_pos","z_safe_pos");
                        changeentityproperty(grabbed_opp,"position",getlocalvar("x_safe_pos"),getlocalvar("z_safe_pos"),NULL());
                        setlocalvar("x_safe_pos",NULL());
                        setlocalvar("z_safe_pos",NULL());*/
                        // oppure subject_to_wall 0
                     }
        }

        setentityvar(self,1,NULL());
        sev(self,22,NULL());
    }
}

