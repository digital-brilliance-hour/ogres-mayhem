
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/player_didhit.c"
#import "data/scripts/player_grab.c"
#import "data/scripts/endlevel_anim.c"
#import "data/scripts/lib_platform.c"

void main() {
    void self = getlocalvar("self");
    int damage = getlocalvar("damage");
    int drop = getlocalvar("drop");
    void other = getlocalvar("other"); // When called on attacker, this recipient of attack. When called on defender, this is the attacker.
    int attacktype = getlocalvar("attacktype");
    int which = getlocalvar("which"); // Refer to self. 0 = Caller is defender. 1 = Caller is attacker.
    int attackid = getlocalvar("attackid");
    int pauseadd = getlocalvar("pauseadd");
    int guardcost = getlocalvar("guardcost");
    int jugglecost = getlocalvar("jugglecost");

    check_animvalid_atk36(self,other,which,attacktype,drop,damage,pauseadd,"ATK_NORMAL"); // for DON ONLY
    check_invalid_atks(self,other,which,attacktype,drop,damage,pauseadd);
    check_blockpain(self,other,which,attacktype,drop,damage,pauseadd);
    check_turtle_throw(self,other,which,attacktype,drop,damage,pauseadd);

    //check_backpain(self,other,which,attacktype,drop,damage,pauseadd);
}

void check_turtle_throw(void self, void other, int which, int attacktype, int drop, int damage, int pauseadd) {
    if ( which && getentityproperty(other,"exists") ) {
        int board_level = getglobalvar("board_level");

        if ( (board_level == NULL() || board_level == 0) && (getglobalvar("player_nograb") == NULL() || getglobalvar("player_nograb") == 0) ) {
            if ( isATurtle(other) && !getentityproperty(other, "invincible") && getentityproperty(other, "vulnerable") ) {
                int p = getentityproperty(self,"playerindex");
                int dir = getentityproperty(self,"direction");

                if ( getentityproperty(self, "invincible") || !getentityproperty(self, "vulnerable") ) return;

                //if ( (dir && playerkeys(p, 0, "moveleft")) || (!dir && playerkeys(p, 0, "moveright")) ) {
                if ( playerkeys(p, 0, "movedown") || playerkeys(p, 0, "moveup") ) {
                    float x = getentityproperty(self,"x");
                    float z = getentityproperty(self,"z");
                    float a = getentityproperty(self,"y");
                    float base = getentityproperty(self,"base");
                    float xo = getentityproperty(other,"x");
                    float zo = getentityproperty(other,"z");
                    float new_z = 0;
                    int anim_id = getentityproperty(self,"animationid");
                    float t_shift = 40;
                    damage = 25;
                    //drop = 1;

                    // if animation
                    if ( x >= xo-t_shift && x <= xo+t_shift && z >= zo-t_shift && z <= zo+t_shift && is_on_base(self) && is_on_base(other) ) {
                        float wall_h = checkwall(x,z+1);
                        float platform_h = get_platform_alt_plus_h_by_coords(x,z,a);
                        void grabbed = getentityproperty(other, "grabbing");
                        void grabber = get_grabber(other);

                        if ( grabbed != NULL() || grabber != NULL() ) return;

                        if ( playerkeys(p, 0, "movedown") ) {
                            if ( dir ) changeentityproperty(self,"direction",0);
                            else changeentityproperty(self,"direction",1);
                            dir = getentityproperty(self,"direction");
                        } /*else if ( playerkeys(p, 0, "moveup") ) {

                        }*/

                        if ( wall_h > a || platform_h > a ) new_z = z;
                        else new_z = z+1;

                        changeentityproperty(other,"direction",dir);
                        changeentityproperty(other,"position",x,z+1,NULL());

                        changeopenborvariant("lasthitc",0); //nullify last hit collision!!

                        setentityvar(other,"damage",damage);
                        changeentityproperty(self,"velocity",0,0,0);
                        changeentityproperty(other,"velocity",0,0,0);
                        changeentityproperty(other,"opponent",self);
                        performattack(self,openborconstant("ANI_FOLLOW71"),1);
                        damageentity(other,self,0,0,openborconstant("ATK_NORMAL71"));
                        //check_versus_mark(self,other); //  in takedamagescript
                    } // fine if coords
                } // fine if buttons
            } // fine if is a turtle
        } // fine if right level
    } // fine if which

    return;
}

void check_backpain(void self, void other, int which, int attacktype, int drop, int damage, int pauseadd) {
    if ( !which && getentityproperty(other,"exists") ) { // other is the opp!!
        float x = getentityproperty(self,"x");
        int dir = getentityproperty(self,"direction");
        float ox = getentityproperty(other,"x");
        float threshold = 0;

        if ( (!dir && ox > x+threshold) || (dir && ox < x-threshold) ) {
            char REPLACED_ATK = "ATK_NORMAL";
            int health = getentityproperty(self,"health");
            //int blockback = getentityproperty(self,"blockback");

            // back-blocking actived?
            //if ( getentityproperty(self,"aiflag","blocking") && (blockback || blockback == NULL()) ) return;
            if ( getentityproperty(self,"aiflag","blocking") ) return;

            changeopenborvariant("lasthitc",0); //nullify last hit collision!!

            if ( health-damage > 0 ) {
                if ( !drop ) {
                    if ( attacktype == openborconstant("ATK_NORMAL") || attacktype == openborconstant("ATK_NORMAL2") || attacktype == openborconstant("ATK_NORMAL3") || attacktype == openborconstant("ATK_NORMAL8") ) {
                        REPLACED_ATK = "ATK_NORMAL16";
                    } else if ( attacktype == openborconstant("ATK_NORMAL6") ) {
                        REPLACED_ATK = "ATK_NORMAL17";
                    } else if ( attacktype == openborconstant("ATK_NORMAL4") || attacktype == openborconstant("ATK_NORMAL5") ) {
                        REPLACED_ATK = "ATK_NORMAL11";
                    } else if ( attacktype == openborconstant("ATK_NORMAL9") ) {
                        REPLACED_ATK = "ATK_NORMAL50";
                        drop = 1;
                    }
                } else {
                    if ( attacktype == openborconstant("ATK_NORMAL") ) {
                        REPLACED_ATK = "ATK_NORMAL51";
                    } else if ( attacktype == openborconstant("ATK_NORMAL2") || attacktype == openborconstant("ATK_NORMAL9") ) {
                        REPLACED_ATK = "ATK_NORMAL50";
                    } else if ( attacktype == openborconstant("ATK_NORMAL3") ) {
                        REPLACED_ATK = "ATK_NORMAL52";
                    }
                }
            } else {
                if ( attacktype == openborconstant("ATK_NORMAL") ) {
                    REPLACED_ATK = "ATK_NORMAL52";
                } else if ( attacktype == openborconstant("ATK_NORMAL2") || attacktype == openborconstant("ATK_NORMAL3") || attacktype == openborconstant("ATK_NORMAL5") || attacktype == openborconstant("ATK_NORMAL9") || attacktype == openborconstant("ATK_NORMAL12") ) {
                    REPLACED_ATK = "ATK_NORMAL50";
                }
            }

            if ( !getentityproperty(self,"aiflag","blocking") ) {
                // Il flash è in mostra?? Se no allora eseguilo.
                if ( !getentityproperty(self,"flash","noattack") ) show_flash();
                //playsample(loadsample("data/sounds/punch004.wav")); // openborvariant("sample_play_id")
                //playsample(loadsample(getentityproperty(other,"attack","hitsound")));
            }
            /*if ( getentityproperty(other,"aiflag","attacking") && getentityproperty(self,"flash","block") ) {
                show_block_flash();
                playsample(loadsample("data/sounds/punch003.wav"));
            }*/

            damageentity(self,other,damage,drop,openborconstant(REPLACED_ATK));
            //check_versus_mark(other,self);

            // restore direction
            if ( dir ) changeentityproperty(self,"direction",1);
            else if ( !dir ) changeentityproperty(self,"direction",0);

            changeentityproperty(self,"freezetime",openborvariant("elapsed_time")+pauseadd);
            changeentityproperty(other,"freezetime",openborvariant("elapsed_time")+pauseadd);
            changeentityproperty(self,"frozen",1);
            changeentityproperty(other,"frozen",1);
        } // fine if backpain ok
    } // fine if defender and no blocking

    return;
}

void check_blockpain(void self, void other, int which, int attacktype, int drop, int damage, int pauseadd) {
    if ( getentityproperty(self, "aiflag", "blocking") ) {
        int anim_id = getentityproperty(self, "animationid");

        if ( drop > 0 ) {
            changeentityproperty(self, "animation", openborconstant("ANI_BLOCKPAIN9"));
        } else if ( drop <= 0 ) {
            changeentityproperty(self, "animation", openborconstant("ANI_BLOCKPAIN"));
        }
    } // fine if blocking
}

void check_invalid_atks(void self, void other, int which, int attacktype, int drop, int damage, int pauseadd) {
    int anim_id = getentityproperty(self,"animationid");
    int anim_pos = getentityproperty(self,"animpos");
    char defaultmodel = getentityproperty(self,"defaultmodel");

    //if ( defaultmodel == openborconstant("DON") ) {
        if ( !which && getentityproperty(other,"exists") ) { // other is the opp!!
            if (  attacktype == openborconstant("ATK_NORMAL30") || attacktype == openborconstant("ATK_NORMAL31") || attacktype == openborconstant("ATK_NORMAL33") || attacktype == openborconstant("ATK_NORMAL35") ) { //  !getentityproperty(other,"animvalid",openborconstant("ANI_PAIN5"))
                void odefaultmodel = getentityproperty(other,"defaultmodel");

                if ( odefaultmodel == "LEO" || odefaultmodel == "MIKE" || odefaultmodel == "DON" || odefaultmodel == "RAPH" ) {
                    changeopenborvariant("lasthitc",0); //nullify last hit collision!!
                }
            }
        }
    //}
}

void check_animvalid_atk36(void self, void other, int which, int attacktype, int drop, int damage, int pauseadd, char REPLACED_ATK) {
    int anim_id = getentityproperty(self,"animationid");
    int anim_pos = getentityproperty(self,"animpos");
    char defaultmodel = getentityproperty(self,"defaultmodel");

    if ( defaultmodel == "DON" ) {
        if ( which && getentityproperty(other,"exists") && gep(self,"attacking") == 1 ) { // other is the opp!!
            if (  attacktype == openborconstant("ATK_NORMAL36") ) { //  !getentityproperty(other,"animvalid",openborconstant("ANI_PAIN5"))
                void odefaultmodel = getentityproperty(other,"defaultmodel");
                float ohealth = getentityproperty(other,"health");
                float oa = getentityproperty(other,"y");
                float obase = getentityproperty(other,"base");

                if ( odefaultmodel == "LEO" || odefaultmodel == "MIKE" || odefaultmodel == "DON" || odefaultmodel == "RAPH" ) {
                    changeopenborvariant("lasthitc",0); //nullify last hit collision!!
                    drop = 1;

                    if ( oa <= obase ) {
                        if ( ohealth-damage > 0 ) damageentity(other,self,damage,drop,openborconstant(REPLACED_ATK));
                        else damageentity(other,self,damage,drop,openborconstant(REPLACED_ATK));
                    } else {
                        if ( ohealth-damage > 0 ) damageentity(other,self,damage,1,openborconstant(REPLACED_ATK));
                        else damageentity(other,self,damage,1,openborconstant(REPLACED_ATK));
                    }
                    check_versus_mark(self,other);

                    changeentityproperty(self,"attacking",0);
                    if ( !getentityproperty(other,"aiflag","blocking") ) {
                        // Il flash è in mostra?? Se no allora eseguilo.
                        if ( !getentityproperty(other,"flash","noattack") ) show_flash();
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
}



