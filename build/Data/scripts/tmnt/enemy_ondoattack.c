
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/player_didhit.c"

void main() {
    void self = getlocalvar("self");
    int damage = getlocalvar("damage");
    int drop = getlocalvar("drop");
    void other = getlocalvar("other"); // When called on attacker, this recipient of attack. When called on defender, this is the attacker.
    int attacktype = getlocalvar("attacktype");
    int which = getlocalvar("which"); // 0 = Caller is defender. 1 = Caller is attacker. (SELF)
    int attackid = getlocalvar("attackid");
    int noblock = getlocalvar("noblock");
    int pauseadd = getlocalvar("pauseadd");

    //sgv("test",attacktype);
    check_flare_atk(self,other,which,attacktype,drop,damage,pauseadd,"ATK_NORMAL");
    check_block_for_tonfa(self,other,which,attacktype,drop,damage,noblock,pauseadd);
    //check_spinpain_falldie(self,other,which,attacktype,drop,damage,noblock,pauseadd);
    //if ( attacktype == 9 ) performattack(self,openborconstant("ANI_FOLLOW12"),1);

    check_atks_on_stairs(self,other,which,attacktype);
}

void check_atks_on_stairs(void self, void other, int which, int attacktype) {
    if ( !which && getentityproperty(other, "exists") ) {
        if ( attacktype == openborconstant("ATK_NORMAL20") || attacktype == openborconstant("ATK_NORMAL36") ) {
            changeentityproperty(self,"no_adjust_base",0);
        }
    }
}

/*void check_spinpain_falldie(void self, void other, void which, int attacktype, int drop, int damage, int noblock, int pauseadd) {
    if ( !which && (attacktype == openborconstant("ATK_NORMAL13") || attacktype == openborconstant("ATK_NORMAL62")) ) { // other is the opp!!
        float a = getentityproperty(self,"y");
        float base = getentityproperty(self,"base");

        if ( a <= base ) {
            if ( !getentityproperty(self,"aiflag","blocking") || noblock ) {
                int health = getentityproperty(self,"health");

                if ( health-damage <= 0 ) changeentityproperty(self,"falldie",1);
            }
        }
    }
}*/

void check_block_for_tonfa(void self, void other, int which, int attacktype, int drop, int damage, int noblock, int pauseadd, int block_flag) {
    int anim_id = getentityproperty(self,"animationid");
    int anim_pos = getentityproperty(self,"animpos");
    int ent_type = getentityproperty(self,"type");
    float a = getentityproperty(self,"y");
    float base = getentityproperty(self,"base");
    void model = getentityproperty(self,"model");
    char atk01 = oc("ANI_FREESPECIAL10");
    char atk02 = oc("ANI_FREESPECIAL11");
    char atk03 = oc("ANI_FREESPECIAL13");
    char atk04 = oc("ANI_FREESPECIAL15");
    char atk05 = oc("ANI_FREESPECIAL16");
    char atk06 = oc("ANI_FREESPECIAL2");
    char atk07 = oc("ANI_FREESPECIAL20");
    char atk08 = oc("ANI_FREESPECIAL3");
    char atk09 = oc("ANI_FREESPECIAL33");
    char atk10 = oc("ANI_FREESPECIAL4");
    char atk11 = oc("ANI_FREESPECIAL5");
    char atk12 = oc("ANI_FREESPECIAL6");
    char atk13 = oc("ANI_FREESPECIAL8");
    char atk14 = oc("ANI_SPECIAL");
    char atk15 = oc("ANI_SPECIAL2");

    if ( model == "player_foot_tonfa" ) {

        if ( anim_id == oc("ANI_FREESPECIAL20") || anim_id == oc("ANI_BLOCK") || anim_id == oc("ANI_BLOCKPAIN") ) {
            int rand = rand_gen(0,100);

            //if ( block_flag > 0  && block_flag != NULL() )
                rand = 100;

            if ( !which && getentityproperty(other,"exists") && !noblock && rand >= 10 ) { // other is the opp!!
                int o_anim_id = getentityproperty(other,"animationid");

                if ( o_anim_id == atk01 || o_anim_id == atk02 || o_anim_id == atk03 || o_anim_id == atk04 || o_anim_id == atk05 || o_anim_id == atk06
                     || o_anim_id == atk07 || o_anim_id == atk08 || o_anim_id == atk09 || o_anim_id == atk10 || o_anim_id == atk11 || o_anim_id == atk12
                     || o_anim_id == atk13 || o_anim_id == atk14 || o_anim_id == atk15 ) return;

                if ( a > base ) return;

                changeopenborvariant("lasthitc",0); //nullify last hit collision!!
                if ( getentityproperty(other,"aiflag","attacking") && getentityproperty(self,"flash","block") ) {
                    show_block_flash();
                    playsample(loadsample("data/sounds/punch003.wav"));
                }
                changeentityproperty(self,"freezetime",openborvariant("elapsed_time")+pauseadd);
                changeentityproperty(other,"freezetime",openborvariant("elapsed_time")+pauseadd);
                changeentityproperty(self,"frozen",1);
                changeentityproperty(other,"frozen",1);

                changeentityproperty(other,"aiflag","attacking",0);

                changeentityproperty(self,"aiflag","blocking",0);
                changeentityproperty(self,"aiflag","attacking",0);
                changeentityproperty(self,"attacking",0);
                changeentityproperty(self,"aiflag","inpain",1);
                changeentityproperty(self,"velocity",0,0,0);
                performattack(self,openborconstant("ANI_BLOCKPAIN"),1);
                //changeentityproperty(other,"aiflag","attacking",0);

                //changeentityproperty(other,"attack","noblock",0);
            }
        }
    } else if ( model == "foot_tonfa" ) {
            int rand = rand_gen(0,100);

            if ( block_flag > 0  && block_flag != NULL() ) rand = 100;

            if ( !which && getentityproperty(other,"exists") && !noblock && rand >= 10 ) { // other is the opp!!
                int o_anim_id = getentityproperty(other,"animationid");

                if ( o_anim_id == atk01 || o_anim_id == atk02 || o_anim_id == atk03 || o_anim_id == atk04 || o_anim_id == atk05 || o_anim_id == atk06
                     || o_anim_id == atk07 || o_anim_id == atk08 || o_anim_id == atk09 || o_anim_id == atk10 || o_anim_id == atk11 || o_anim_id == atk12
                     || o_anim_id == atk13 || o_anim_id == atk14 || o_anim_id == atk15 ) return;

                if ( a > base ) return;

                changeopenborvariant("lasthitc",0); //nullify last hit collision!!
                if ( getentityproperty(other,"aiflag","attacking") && getentityproperty(self,"flash","block") ) {
                    show_block_flash();
                    playsample(loadsample("data/sounds/punch003.wav"));
                }
                changeentityproperty(self,"freezetime",openborvariant("elapsed_time")+pauseadd);
                changeentityproperty(other,"freezetime",openborvariant("elapsed_time")+pauseadd);
                changeentityproperty(self,"frozen",1);
                changeentityproperty(other,"frozen",1);

                changeentityproperty(other,"aiflag","attacking",0);

                changeentityproperty(self,"aiflag","blocking",0);
                changeentityproperty(self,"aiflag","attacking",0);
                changeentityproperty(self,"attacking",0);
                changeentityproperty(self,"aiflag","inpain",1);
                changeentityproperty(self,"velocity",0,0,0);
                performattack(self,openborconstant("ANI_BLOCKPAIN"),1);
                //changeentityproperty(other,"aiflag","attacking",0);

                //changeentityproperty(other,"attack","noblock",0);
            }
    }
}

void check_flare_atk(void self, void other, int which, int attacktype, int drop, int damage, int pauseadd, void REPLACED_ATK) {
    int anim_id = getentityproperty(self,"animationid");
    int anim_pos = getentityproperty(self,"animpos");
    int ent_type = getentityproperty(self,"type");
    int atk_anim;

    if ( ent_type == openborconstant("TYPE_PLAYER") ) {
        atk_anim = openborconstant("ANI_FREESPECIAL21");
    } else if ( ent_type == openborconstant("TYPE_ENEMY") ) {
        atk_anim = openborconstant("ANI_FREESPECIAL21");
    }

    if ( anim_id == atk_anim ) {
        if ( which && getentityproperty(other,"exists") && gep(self,"attacking") == 1 ) { // other is the opp!!
            if ( !getentityproperty(other,"animvalid",openborconstant("ANI_BURNPAIN")) && attacktype == openborconstant("ATK_BURN") ) { //  && attacktype == openborconstant("ATK_NORMAL5")
                changeopenborvariant("lasthitc",0); //nullify last hit collision!!
                drop = 1;

                damageentity(other,self,damage,drop,openborconstant(REPLACED_ATK));
                check_versus_mark(self,other);
                cep(self,"attacking",0);
                if ( !getentityproperty(other,"aiflag","blocking") ) {
                    if ( !gep(other,"flash","noattack") ) show_flash();
                    playsample(loadsample("data/sounds/punch004.wav"));
                }
                changeentityproperty(self,"freezetime",openborvariant("elapsed_time")+pauseadd);
                changeentityproperty(other,"freezetime",openborvariant("elapsed_time")+pauseadd);
                changeentityproperty(self,"frozen",1);
                changeentityproperty(other,"frozen",1);
            }
        }
    }
}

