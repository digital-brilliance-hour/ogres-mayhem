
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/enemy_moves.c"
#import "data/scripts/player_didhit.c"
//#import "data/scripts/enemy_moves.c"

void main() {
    void self = getlocalvar("self");
    void attacker = getlocalvar("attacker");
    float damage = getlocalvar("damage");
    int drop = getlocalvar("drop");
    int attacktype = getlocalvar("attacktype");
    int noblock = getlocalvar("noblock");
    int pauseadd = getlocalvar("pauseadd");

    check_foot_follower(self);

    setoppx_ondmg(self,attacker); // backpains
    check_drop_bomb(self,attacker,damage);
    check_drop_surf(self,attacker,damage);
    check_drop_tire(self,attacker,damage);
    check_drop_torpedo(self,attacker,damage);
    check_drop_snowflood(self,attacker,damage);
    check_drop_leadball(self,attacker,damage);
    pain_recovery(self,attacktype);
    check_burn_falldie(self,attacker,attacktype,drop,damage);
    check_spindie(self,attacker,attacktype,drop,damage);

    check_vs_mark(self,attacker,attacktype);
}

void check_foot_follower(void self) {
    char defaultmodel = gep(self,"defaultmodel");

    if ( defaultmodel == "foot_snowball_follower" ) {
        bindentity(self,NULL());
        cep(self,"noaicontrol",0);
        cep(self,"no_adjust_base",0);
    }
}

int check_vs_mark(void self, void attacker, int attacktype) {
    //int prev_animid = get_prev_anim(self);
    int prev_animid = gep(self,"animationid");

    if ( prev_animid == oc("ANI_FALL35") && attacktype == oc("ATK_NORMAL") ) check_versus_mark(attacker,self);
    else if ( prev_animid == oc("ANI_PAIN35") && attacktype == oc("ATK_NORMAL35") ) check_versus_mark(attacker,self);
    else if ( prev_animid == oc("ANI_FALL33") && attacktype == oc("ATK_NORMAL21") ) check_versus_mark(attacker,self);
    else if ( prev_animid == oc("ANI_FALL61") && attacktype == oc("ATK_NORMAL21") ) check_versus_mark(attacker,self);
    else if ( prev_animid == oc("ANI_PAIN34") && attacktype == oc("ATK_NORMAL8") ) check_versus_mark(attacker,self);
    else if ( prev_animid == oc("ANI_PAIN31") && attacktype == oc("ATK_NORMAL8") ) check_versus_mark(attacker,self);
    else if ( prev_animid == oc("ANI_PAIN37") && attacktype == oc("ATK_NORMAL8") ) check_versus_mark(attacker,self);
    else if ( prev_animid == oc("ANI_PAIN30") && attacktype == oc("ATK_NORMAL8") ) check_versus_mark(attacker,self);
    else if ( prev_animid == oc("ANI_PAIN20") && attacktype == oc("ATK_NORMAL22") ) check_versus_mark(getentityproperty(self,"opponent"),self); // tmnt3 like
    else if ( prev_animid == oc("ANI_PAIN36") && attacktype == oc("ATK_NORMAL10") ) check_versus_mark(getentityproperty(self,"opponent"),self); // don tmnt3 like
}

int pain_recovery(void self, int attacktype) {

    if ( attacktype == openborconstant("ATK_NORMAL19") ) {
        int type = getentityproperty(self,"type");

        if ( type == openborconstant("TYPE_PLAYER") ) {
            changeentityproperty(self, "invincible", 1);
            changeentityproperty(self, "invinctime", openborvariant("elapsed_time") + (openborvariant("game_speed")*0.5));
        }
    }
}

int setoppx_ondmg(void player, void attacker) {
    setentityvar(player, 6, getentityproperty(attacker, "x") );
}

void check_spindie(void self, void attacker, void attacktype, int drop, int damage) {
    if ( attacktype == openborconstant("ATK_NORMAL13") ) {
        int health = getentityproperty(self,"health");
        if ( health <= 0 ) changeentityproperty(self, "falldie", 1);
    }
}

void check_burn_falldie(void self, void attacker, void attacktype, int drop, int damage) {
    int anim_id = getentityproperty(self,"animationid");
    int anim_pos = getentityproperty(self,"animpos");
    char model = getentityproperty(self,"model");

    if ( getentityproperty(self,"animvalid",openborconstant("ANI_BURNPAIN")) && attacktype == openborconstant("ATK_BURN") ) { //  && attacktype == openborconstant("ATK_NORMAL5")
        if ( getentityproperty(self,"health") <= 0 ) {
            //changeentityproperty(self,"animation",openborconstant("ANI_BURNPAIN"));
            setentityvar(self,5,2);
        }
    }
}

int check_drop_surf(void self, void opp, int damage) {
    char model = getentityproperty(self,"model");
    int ent_type = getentityproperty(self,"type");

    if ( (model == "foot_surf" || model == "player_foot_surf" || model == "board_foot_surf") ) {
        if ( ( ent_type == openborconstant("TYPE_ENEMY") || ent_type == openborconstant("TYPE_PLAYER") ) && damage > 0 ) {
            float x = getentityproperty(self, "x");
            float z = getentityproperty(self, "z");
            float a = getentityproperty(self, "y");
            int dir = getentityproperty(self, "direction");
            void subent;
            float bx = -15, ba = 55;
            float xdir = 2;
            int health = getentityproperty(self,"health");
            int anim_id = getentityproperty(self, "animationid");
            int anim_pos = getentityproperty(self, "animpos");

            float threshold = 10;
            void rplatform,lplatform;
            float pa = 0, ph = 0;
            int platform_flag = 0;
            float height = getentityproperty(self,"height");

                if ( (model == "foot_surf" || model == "board_foot_surf") && ent_type == openborconstant("TYPE_ENEMY") && health > 0 ) {
                    //if ( getentityproperty(self,"weapent") != 1 )
                        changeentityproperty(self,"weapon",1);
                } else if ( model == "player_foot_surf" && ent_type == openborconstant("TYPE_PLAYER") && health > 0 ) {
                    int p = getentityproperty(self, "playerindex");

                    //if ( getplayerproperty(p,"weapnum") != 0 )
                        changeplayerproperty(p,"weapon",0);
                }

                if (dir == 0) bx *= -1;

                rplatform = checkplatformbelow(x+threshold,z,a);
                lplatform = checkplatformbelow(x-threshold,z,a);
                if ( getentityproperty(rplatform,"exists") && dir ) {
                    pa = getentityproperty(rplatform, "y");
                    ph = getentityproperty(rplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx -= 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }
                if ( getentityproperty(lplatform,"exists") && !platform_flag && !dir ) {
                    pa = getentityproperty(lplatform, "y");
                    ph = getentityproperty(lplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx += 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }

                if ( checkwall(x+threshold,z) > a+height && !platform_flag ) {
                    bx -= 20;
                    xdir = 0;
                } else if ( checkwall(x-threshold,z) > a+height && !platform_flag ) {
                    bx += 20;
                    xdir = 0;
                }

                if ( opp == self ) return;
                if ( getentityproperty(opp,"parent") == self ) return;
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos >= 2 ) return;

                clearspawnentry();
                setspawnentry("name", "p_surf");
                subent = spawn();
                changeentityproperty(subent, "position", x+bx, z+1, a+ba);
                changeentityproperty(subent, "base", a+ba);
                changeentityproperty(subent, "parent", self);
                changeentityproperty(subent, "map", getentityproperty(self,"map"));
                check_safe_pos(subent,5,5,0);

                if ( dir == 0 ) changeentityproperty(subent, "direction", 0);
                else changeentityproperty(subent, "direction", 1);

                if ( !getentityproperty(self,"aiflag","dead") ) {
                    changeentityproperty(self, "invincible", 1);
                    changeentityproperty(self, "invinctime", get_next_time(openborvariant("game_speed")*0.5)); //makes entity invincible for 1.5 seconds, change decimal number to change time length
                }

                if (!dir) xdir *= -1;
                tossentity(subent,2.5,xdir,NULL());

                /*if ( health > 0 ) {
                    //changeentityproperty(self,"aiflag","falling",1);
                    //changeentityproperty(self,"aiflag","inpain",0);
                    //changeentityproperty(self,"aiflag","drop",1);
                }*/
        }
    }
}

int check_drop_bomb(void self, void opp, int damage) {
    char model = getentityproperty(self,"model");
    int ent_type = getentityproperty(self,"type");

    if ( (model == "foot_bomb" || model == "player_foot_bomb" || model == "board_foot_bomb") ) {
        if ( ( ent_type == openborconstant("TYPE_ENEMY") || ent_type == openborconstant("TYPE_PLAYER") ) && damage > 0 ) {
            float x = getentityproperty(self, "x");
            float z = getentityproperty(self, "z");
            float a = getentityproperty(self, "y");
            int dir = getentityproperty(self, "direction");
            void subent;
            float bx = 0, ba = 50;
            float xdir = 2;
            int health = getentityproperty(self,"health");
            int anim_id = getentityproperty(self, "animationid");
            int anim_pos = getentityproperty(self, "animpos");

            float threshold = 5;
            void rplatform,lplatform;
            float pa = 0, ph = 0;
            int platform_flag = 0;
            float height = getentityproperty(self,"height");

                if ( (model == "foot_bomb" || model == "board_foot_bomb") && ent_type == openborconstant("TYPE_ENEMY") && health > 0 ) {
                    //if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos >= 5 ) return 0;
                    //else if ( anim_id == openborconstant("ANI_ATTACK2") && anim_pos >= 7 ) return 0;
                    //if ( getentityproperty(self,"weapent") != 1 )
                        changeentityproperty(self,"weapon",1);
                } else if ( model == "player_foot_bomb" && ent_type == openborconstant("TYPE_PLAYER") && health > 0 ) {
                    int p = getentityproperty(self, "playerindex");

                    //if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos >= 5 ) return 0;
                    //else if ( anim_id == openborconstant("ANI_FREESPECIAL20") && anim_pos >= 7 ) return 0;
                    //if ( getplayerproperty(p,"weapnum") != 0 )
                        changeplayerproperty(p,"weapon",0);
                }

                if (dir == 0) bx *= -1;

                rplatform = checkplatformbelow(x+threshold,z,a);
                lplatform = checkplatformbelow(x-threshold,z,a);
                if ( getentityproperty(rplatform,"exists") && dir ) {
                    pa = getentityproperty(rplatform, "y");
                    ph = getentityproperty(rplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx -= 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }
                if ( getentityproperty(lplatform,"exists") && !platform_flag && !dir ) {
                    pa = getentityproperty(lplatform, "y");
                    ph = getentityproperty(lplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx += 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }

                if ( checkwall(x+threshold,z) > a+height && !platform_flag ) {
                    bx -= 20;
                    xdir = 0;
                } else if ( checkwall(x-threshold,z) > a+height && !platform_flag ) {
                    bx += 20;
                    xdir = 0;
                }

                if ( opp == self ) return;
                if ( getentityproperty(opp,"parent") == self ) return;
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos >= 5 ) return;
                else if ( (anim_id == openborconstant("ANI_ATTACK2") || anim_id == openborconstant("ANI_FREESPECIAL81") ) && anim_pos >= 7 ) return;
                else if ( (anim_id == openborconstant("ANI_JUMPATTACK") || anim_id == openborconstant("ANI_FREESPECIAL82")) && anim_pos >= 3 ) return;

                clearspawnentry();
                setspawnentry("name", "p_bomb");
                subent = spawn();
                changeentityproperty(subent, "position", x+bx, z+1, a+ba);
                changeentityproperty(subent, "base", a+ba);
                changeentityproperty(subent, "parent", self);
                check_safe_pos(subent,5,5,0);

                if ( dir == 0 ) changeentityproperty(subent, "direction", 0);
                else changeentityproperty(subent, "direction", 1);

                if ( !getentityproperty(self,"aiflag","dead") ) {
                    changeentityproperty(self, "invincible", 1);
                    changeentityproperty(self, "invinctime", get_next_time(openborvariant("game_speed")*0.5)); //makes entity invincible for 1.5 seconds, change decimal number to change time length
                }

                if (!dir) xdir *= -1;
                tossentity(subent,2.5,xdir,NULL());

                /*if ( health > 0 ) {
                    //changeentityproperty(self,"aiflag","falling",1);
                    //changeentityproperty(self,"aiflag","inpain",0);
                    //changeentityproperty(self,"aiflag","drop",1);
                }*/
        }
    }
}

int check_drop_tire(void self, void opp, int damage) {
    char model = getentityproperty(self,"model");
    int ent_type = getentityproperty(self,"type");

    if ( (model == "foot_tire" || model == "player_foot_tire" || model == "board_foot_tire") ) {
        if ( ( ent_type == openborconstant("TYPE_ENEMY") || ent_type == openborconstant("TYPE_PLAYER") ) && damage > 0 ) {
            float x = getentityproperty(self, "x");
            float z = getentityproperty(self, "z");
            float a = getentityproperty(self, "y");
            int dir = getentityproperty(self, "direction");
            void subent;
            float bx = -15, ba = 55;
            float xdir = 2;
            int health = getentityproperty(self,"health");
            int anim_id = getentityproperty(self, "animationid");
            int anim_pos = getentityproperty(self, "animpos");

            float threshold = 10;
            void rplatform,lplatform;
            float pa = 0, ph = 0;
            int platform_flag = 0;
            float height = getentityproperty(self,"height");

                if ( (model == "foot_tire" || model == "board_foot_tire") && ent_type == openborconstant("TYPE_ENEMY") && health > 0 ) {
                    //if ( getentityproperty(self,"weapent") != 1 )
                        changeentityproperty(self,"weapon",1);
                } else if ( model == "player_foot_tire" && ent_type == openborconstant("TYPE_PLAYER") && health > 0 ) {
                    int p = getentityproperty(self, "playerindex");

                    //if ( getplayerproperty(p,"weapnum") != 0 )
                        changeplayerproperty(p,"weapon",0);
                }

                if (dir == 0) bx *= -1;

                rplatform = checkplatformbelow(x+threshold,z,a);
                lplatform = checkplatformbelow(x-threshold,z,a);
                if ( getentityproperty(rplatform,"exists") && dir ) {
                    pa = getentityproperty(rplatform, "y");
                    ph = getentityproperty(rplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx -= 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }
                if ( getentityproperty(lplatform,"exists") && !platform_flag && !dir ) {
                    pa = getentityproperty(lplatform, "y");
                    ph = getentityproperty(lplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx += 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }

                if ( checkwall(x+threshold,z) > a+height && !platform_flag ) {
                    bx -= 20;
                    xdir = 0;
                } else if ( checkwall(x-threshold,z) > a+height && !platform_flag ) {
                    bx += 20;
                    xdir = 0;
                }

                if ( opp == self ) return;
                if ( getentityproperty(opp,"parent") == self ) return;
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos >= 2 ) return;

                clearspawnentry();
                setspawnentry("name", "p_tire");
                subent = spawn();
                changeentityproperty(subent, "position", x+bx, z+1, a+ba);
                changeentityproperty(subent, "base", a+ba);
                changeentityproperty(subent, "parent", self);
                check_safe_pos(subent,5,5,0);

                if ( dir == 0 ) changeentityproperty(subent, "direction", 0);
                else changeentityproperty(subent, "direction", 1);

                if ( !getentityproperty(self,"aiflag","dead") ) {
                    changeentityproperty(self, "invincible", 1);
                    changeentityproperty(self, "invinctime", get_next_time(openborvariant("game_speed")*0.5)); //makes entity invincible for 1.5 seconds, change decimal number to change time length
                }

                if (!dir) xdir *= -1;
                tossentity(subent,2.5,xdir,NULL());

                /*if ( health > 0 ) {
                    //changeentityproperty(self,"aiflag","falling",1);
                    //changeentityproperty(self,"aiflag","inpain",0);
                    //changeentityproperty(self,"aiflag","drop",1);
                }*/
        }
    }
}

int check_drop_torpedo(void self, void opp, int damage) {
    char model = getentityproperty(self,"model");
    int ent_type = getentityproperty(self,"type");

    if ( (model == "foot_torpedo" || model == "player_foot_torpedo" || model == "board_foot_torpedo") ) {
        if ( ( ent_type == openborconstant("TYPE_ENEMY") || ent_type == openborconstant("TYPE_PLAYER") ) && damage > 0 ) {
            float x = getentityproperty(self, "x");
            float z = getentityproperty(self, "z");
            float a = getentityproperty(self, "y");
            int dir = getentityproperty(self, "direction");
            void subent;
            float bx = -15, ba = 55;
            float xdir = 2;
            int health = getentityproperty(self,"health");
            int anim_id = getentityproperty(self, "animationid");
            int anim_pos = getentityproperty(self, "animpos");

            float threshold = 10;
            void rplatform,lplatform;
            float pa = 0, ph = 0;
            int platform_flag = 0;
            float height = getentityproperty(self,"height");

                if ( (model == "foot_torpedo" || model == "board_foot_torpedo") && ent_type == openborconstant("TYPE_ENEMY") && health > 0 ) {
                    //if ( getentityproperty(self,"weapent") != 1 )
                        changeentityproperty(self,"weapon",1);
                } else if ( model == "player_foot_torpedo" && ent_type == openborconstant("TYPE_PLAYER") && health > 0 ) {
                    int p = getentityproperty(self, "playerindex");

                    //if ( getplayerproperty(p,"weapnum") != 0 )
                        changeplayerproperty(p,"weapon",0);
                }

                if (dir == 0) bx *= -1;

                rplatform = checkplatformbelow(x+threshold,z,a);
                lplatform = checkplatformbelow(x-threshold,z,a);
                if ( getentityproperty(rplatform,"exists") && dir ) {
                    pa = getentityproperty(rplatform, "y");
                    ph = getentityproperty(rplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx -= 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }
                if ( getentityproperty(lplatform,"exists") && !platform_flag && !dir ) {
                    pa = getentityproperty(lplatform, "y");
                    ph = getentityproperty(lplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx += 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }

                if ( checkwall(x+threshold,z) > a+height && !platform_flag ) {
                    bx -= 20;
                    xdir = 0;
                } else if ( checkwall(x-threshold,z) > a+height && !platform_flag ) {
                    bx += 20;
                    xdir = 0;
                }

                if ( opp == self ) return;
                if ( getentityproperty(opp,"parent") == self ) return;
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos >= 2 ) return;

                clearspawnentry();
                setspawnentry("name", "p_torpedo");
                subent = spawn();
                changeentityproperty(subent, "position", x+bx, z+1, a+ba);
                changeentityproperty(subent, "base", a+ba);
                changeentityproperty(subent, "parent", self);
                check_safe_pos(subent,5,5,0);

                if ( dir == 0 ) changeentityproperty(subent, "direction", 0);
                else changeentityproperty(subent, "direction", 1);

                if ( !getentityproperty(self,"aiflag","dead") ) {
                    changeentityproperty(self, "invincible", 1);
                    changeentityproperty(self, "invinctime", get_next_time(openborvariant("game_speed")*0.5)); //makes entity invincible for 1.5 seconds, change decimal number to change time length
                }

                if (!dir) xdir *= -1;
                tossentity(subent,3.0,xdir,NULL());

                /*if ( health > 0 ) {
                    //changeentityproperty(self,"aiflag","falling",1);
                    //changeentityproperty(self,"aiflag","inpain",0);
                    //changeentityproperty(self,"aiflag","drop",1);
                }*/
        }
    }
}

int check_drop_leadball(void self, void opp, int damage) {
    char model = getentityproperty(self,"model");
    int ent_type = getentityproperty(self,"type");

    if ( (model == "foot_leadball" || model == "player_foot_leadball" || model == "board_foot_leadball") ) {
        if ( ( ent_type == openborconstant("TYPE_ENEMY") || ent_type == openborconstant("TYPE_PLAYER") ) && damage > 0 ) {
            float x = getentityproperty(self, "x");
            float z = getentityproperty(self, "z");
            float a = getentityproperty(self, "y");
            int dir = getentityproperty(self, "direction");
            void subent;
            float bx = -15, ba = 55;
            float xdir = 2;
            int health = getentityproperty(self,"health");
            int anim_id = getentityproperty(self, "animationid");
            int anim_pos = getentityproperty(self, "animpos");

            float threshold = 10;
            void rplatform,lplatform;
            float pa = 0, ph = 0;
            int platform_flag = 0;
            float height = getentityproperty(self,"height");

                if ( (model == "foot_leadball" || model == "board_foot_leadball") && ent_type == openborconstant("TYPE_ENEMY") && health > 0 ) {
                    //if ( getentityproperty(self,"weapent") != 1 )
                        changeentityproperty(self,"weapon",1);
                } else if ( model == "player_foot_leadball" && ent_type == openborconstant("TYPE_PLAYER") && health > 0 ) {
                    int p = getentityproperty(self, "playerindex");

                    //if ( getplayerproperty(p,"weapnum") != 0 )
                        changeplayerproperty(p,"weapon",0);
                }

                if (dir == 0) bx *= -1;

                rplatform = checkplatformbelow(x+threshold,z,a);
                lplatform = checkplatformbelow(x-threshold,z,a);
                if ( getentityproperty(rplatform,"exists") && dir ) {
                    pa = getentityproperty(rplatform, "y");
                    ph = getentityproperty(rplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx -= 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }
                if ( getentityproperty(lplatform,"exists") && !platform_flag && !dir ) {
                    pa = getentityproperty(lplatform, "y");
                    ph = getentityproperty(lplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx += 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }

                if ( checkwall(x+threshold,z) > a+height && !platform_flag ) {
                    bx -= 20;
                    xdir = 0;
                } else if ( checkwall(x-threshold,z) > a+height && !platform_flag ) {
                    bx += 20;
                    xdir = 0;
                }

                if ( opp == self ) return;
                if ( getentityproperty(opp,"parent") == self ) return;
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos >= 2 ) return;

                clearspawnentry();
                setspawnentry("name", "p_leadball");
                subent = spawn();
                changeentityproperty(subent, "position", x+bx, z+1, a+ba);
                changeentityproperty(subent, "base", a+ba);
                changeentityproperty(subent, "parent", self);
                check_safe_pos(subent,5,5,0);

                if ( dir == 0 ) changeentityproperty(subent, "direction", 0);
                else changeentityproperty(subent, "direction", 1);

                if ( !getentityproperty(self,"aiflag","dead") ) {
                    changeentityproperty(self, "invincible", 1);
                    changeentityproperty(self, "invinctime", get_next_time(openborvariant("game_speed")*0.5)); //makes entity invincible for 1.5 seconds, change decimal number to change time length
                }

                if (!dir) xdir *= -1;
                tossentity(subent,2.5,xdir,NULL());

                /*if ( health > 0 ) {
                    //changeentityproperty(self,"aiflag","falling",1);
                    //changeentityproperty(self,"aiflag","inpain",0);
                    //changeentityproperty(self,"aiflag","drop",1);
                }*/
        }
    }
}

int check_drop_snowflood(void self, void opp, int damage) {
    char model = getentityproperty(self,"model");
    int ent_type = getentityproperty(self,"type");

    if ( model == "foot_snowflood_follower" ) {
        changeentityproperty(self,"noaicontrol",0);
        changeentityproperty(self,"no_adjust_base",0);
        bindentity(self,NULL());
        changeentityproperty(self,"weapon",1);
    } else if ( (model == "foot_snowflood" || model == "player_foot_snowflood" || model == "board_foot_snowflood") ) {
        if ( ( ent_type == openborconstant("TYPE_ENEMY") || ent_type == openborconstant("TYPE_PLAYER") ) && damage > 0 ) {
            float x = getentityproperty(self, "x");
            float z = getentityproperty(self, "z");
            float a = getentityproperty(self, "y");
            int dir = getentityproperty(self, "direction");
            void subent;
            float bx = -15, ba = 55;
            float xdir = 2;
            int health = getentityproperty(self,"health");
            int anim_id = getentityproperty(self, "animationid");
            int anim_pos = getentityproperty(self, "animpos");

            float threshold = 10;
            void rplatform,lplatform;
            float pa = 0, ph = 0;
            int platform_flag = 0;
            float height = getentityproperty(self,"height");

                if ( (model == "foot_snowflood" || model == "board_foot_snowflood") && ent_type == openborconstant("TYPE_ENEMY") && health > 0 ) {
                    //if ( getentityproperty(self,"weapent") != 1 )
                        changeentityproperty(self,"weapon",1);
                } else if ( model == "player_foot_snowflood" && ent_type == openborconstant("TYPE_PLAYER") && health > 0 ) {
                    int p = getentityproperty(self, "playerindex");

                    //if ( getplayerproperty(p,"weapnum") != 0 )
                        changeplayerproperty(p,"weapon",0);
                }

                if (dir == 0) bx *= -1;

                rplatform = checkplatformbelow(x+threshold,z,a);
                lplatform = checkplatformbelow(x-threshold,z,a);
                if ( getentityproperty(rplatform,"exists") && dir ) {
                    pa = getentityproperty(rplatform, "y");
                    ph = getentityproperty(rplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx -= 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }
                if ( getentityproperty(lplatform,"exists") && !platform_flag && !dir ) {
                    pa = getentityproperty(lplatform, "y");
                    ph = getentityproperty(lplatform, "height");
                    if ( a >= pa && a+height <= pa+ph ) {
                        bx += 20;
                        xdir = 0;
                        platform_flag = 1;
                    }
                }

                if ( checkwall(x+threshold,z) > a+height && !platform_flag ) {
                    bx -= 20;
                    xdir = 0;
                } else if ( checkwall(x-threshold,z) > a+height && !platform_flag ) {
                    bx += 20;
                    xdir = 0;
                }

                if ( opp == self ) return;
                if ( getentityproperty(opp,"parent") == self ) return;
                if ( anim_id == openborconstant("ANI_ATTACK1") && anim_pos >= 2 ) return;

                clearspawnentry();
                setspawnentry("name", "p_snowflood");
                subent = spawn();
                changeentityproperty(subent, "position", x+bx, z+1, a+ba);
                changeentityproperty(subent, "base", a+ba);
                changeentityproperty(subent, "parent", self);
                check_safe_pos(subent,5,5,0);

                if ( dir == 0 ) changeentityproperty(subent, "direction", 0);
                else changeentityproperty(subent, "direction", 1);

                if ( !getentityproperty(self,"aiflag","dead") ) {
                    changeentityproperty(self, "invincible", 1);
                    changeentityproperty(self, "invinctime", get_next_time(openborvariant("game_speed")*0.5)); //makes entity invincible for 1.5 seconds, change decimal number to change time length
                }

                if (!dir) xdir *= -1;
                tossentity(subent,2.5,xdir,NULL());

                /*if ( health > 0 ) {
                    //changeentityproperty(self,"aiflag","falling",1);
                    //changeentityproperty(self,"aiflag","inpain",0);
                    //changeentityproperty(self,"aiflag","drop",1);
                }*/
        }
    }
}

