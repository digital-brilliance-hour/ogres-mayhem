
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/player_check_entities.c"

#import "data/scripts/wave.c"
#import "data/scripts/stairs.c"

// ENTITYVARS: "scene"

void main() {
    void self = getlocalvar("self");
    void parent = getentityproperty(self,"parent");

    /*if ( getentityproperty(parent,"exists") ) {
        //if ( getentityproperty(parent,"no_adjust_base") && !is_on_base(parent) ) {
        if ( !is_on_base(parent) ) {
            wave_flag = check_wave_ents(self);
        }
    }*/
    check_entities(self);

    if ( getentityvar(self,"scene") != 1 ) {
        check_altitude(self,parent);
        check_position(self,parent);
    }

    check_existence(self,parent);

    setlocalvar("wave_is_on_board_flag",NULL());
    setlocalvar("stair_is_on_board_flag",NULL());
    //drawstring(0,100,0,"esisto!");
}

int check_existence(void self, void parent) {
    if ( !getentityproperty(parent,"exists") && getlocalvar("candie") != NULL() ) {
        bindentity(self,NULL());
        killentity(self);
    } else {
        setlocalvar("candie",1);
    }
}

int check_position(void self, void parent) {
    if ( getentityproperty(parent,"exists") ) {
        float x = getentityproperty(self,"x");
        float z = getentityproperty(self,"z");
        float a = getentityproperty(self,"y");
        float base = getentityproperty(self,"base");
        int dir = getentityproperty(self,"direction");
        float px = getentityvar(parent,"x");
        float pz = getentityvar(parent,"z");
        float pa = getentityvar(parent,"y");
        float pbase = getentityvar(parent,"base");
        float pxdir = getentityproperty(parent,"xdir");
        float pzdir = getentityproperty(parent,"zdir");
        float ptossv = getentityproperty(parent,"tossv");
        int pdir = getentityvar(parent,"direction");
        int p_ent_type = getentityproperty(parent,"type");
        char defaultmodel = getentityproperty(self,"defaultmodel");
        float scr_posy = openborvariant("ypos");
        int wave_flag = getlocalvar("wave_is_on_board_flag");
        int stair_flag = getlocalvar("stair_is_on_board_flag");
        float xshift = 0, zshift = 0, new_dir = 0;

        // zshift = -2 or 0 but sorid < parent

        if ( px == NULL() ) px = getentityproperty(parent, "x");
        if ( pz == NULL() ) pz = getentityproperty(parent, "z");
        if ( pa == NULL() ) pa = getentityproperty(parent, "y");
        if ( pbase == NULL() ) pbase = getentityproperty(parent, "base");
        if ( pdir == NULL() ) pdir = getentityproperty(parent, "direction");

        if ( p_ent_type == openborconstant("TYPE_PLAYER") ) {
            new_dir = 1;
        } else if ( p_ent_type == openborconstant("TYPE_ENEMY") ) {
            new_dir = 0;
        }

        if ( defaultmodel == "surf" ) {
            xshift = 5;
            if ( !dir ) xshift *= 1;
            //zshift = 20;
        } else if ( defaultmodel == "esurf" ) {
            xshift = 0;
            if ( !dir ) xshift *= 1;
            //zshift = 18;
        } else if ( defaultmodel == "skate" ) {
            xshift = 2;
            if ( !dir ) xshift *= 1;
            //zshift = 10;
        } else if ( defaultmodel == "eskate" ) {
            xshift = -1;
            if ( !dir ) xshift *= 1;
            //zshift = 10;
        } else if ( defaultmodel == "ufo" ) {
            xshift = 5;
            if ( !dir ) xshift *= 1;
            //zshift = 20;
        } else if ( defaultmodel == "eufo" ) {
            if ( !dir ) xshift = -2;
            else xshift = 1;
            //zshift = 20;
        } else if ( defaultmodel == "snow" ) {
            xshift = 5;
            if ( !dir ) xshift *= 1;
            //zshift = 20;
        } else if ( defaultmodel == "esnow" ) {
            if ( !dir ) xshift = -2;
            else xshift = 1;
            //zshift = 20;
        }

        //drawstring( 10,50,0,"Var (board x):  "+px );
        //drawstring( 10,60,0,"Var (board z):  "+pz );
        //drawstring( 10,60,0,"Var:  "+getentityproperty(self,"speed") );

        //bindentity(self,parent,xshift,zshift,-1*(pa-pbase),0,0);
        //bindentity(self,NULL());

        /*float threshold = 0.001, inc = 1;
        float decimal_px, decimal_pz;
        decimal_px = px-truncA(px);
        decimal_pz = pz-truncA(pz);
        if ( decimal_px >= threshold ) {
            if ( !pdir ) inc *= -1;
            px = px + inc;
        } else px = px;
        if ( decimal_pz >= threshold ) {
            if ( getentityproperty(parent,"zdir") < 0 ) inc *= -1;
            pz = pz + inc;
        } else pz = pz;*/

        //if ( glv("aaa") == NULL() ) {
            //if ( pzdir == 0 ) zshift = -1;
            changeentityproperty(self,"position",px+xshift,pz+zshift,NULL()); // pbase
            //slv("aaa",1);
        //}

        //changeentityproperty(self,"speed",getentityproperty(parent,"speed"));
        //changeentityproperty(self,"aiflag","running","speed",getentityproperty(parent,"aiflag","running","speed"));
        changeentityproperty(self,"velocity",pxdir/2,pzdir/2,NULL());

        if ( (wave_flag == NULL() || wave_flag == 0) && (stair_flag == NULL() || stair_flag == 0) ) {
            if ( pa <= pbase ) {
                changeentityproperty(self,"no_adjust_base",1);
            } else {
                if ( getentityproperty(self,"no_adjust_base") == 1 ) changeentityproperty(self,"no_adjust_base",0);
            }
        }

        if ( pa <= pbase && ptossv == 0 ) {
            changeentityproperty(self,"base",pbase);
            changeentityproperty(self,"position",NULL(),NULL(),pbase);
        }
        if ( is_on_base(parent) && base != pbase && ptossv == 0 ) {
            changeentityproperty(self,"base",pbase);
        }

        if ( dir != new_dir ) changeentityproperty(self,"direction",new_dir);
    }

    /*if ( pa != pbase ) {
        bindentity(self,NULL());
    } else bindentity(self,parent,xshift,zshift,0,0,0);*/

    return 1;
}

int check_altitude(void self, void parent) {
    if ( getentityproperty(parent,"exists") ) {
        float x = getentityproperty(self,"x");
        float z = getentityproperty(self,"z");
        float a = getentityproperty(self,"y");
        float base = getentityproperty(self,"base");
        float px = getentityproperty(parent,"x");
        float pz = getentityproperty(parent,"z");
        float pa = getentityproperty(parent,"y");
        float pbase = getentityproperty(parent,"base");

        if ( pa < a ) {
            changeentityproperty(self,"position",NULL(),NULL(),pa);
            changeentityproperty(self,"base",pbase);
        }
    }
}

int check_entities(void self) {
    int i = 0;
    int stair_found_flag = 0, stair_is_on = 0, tmp_stair_is_on = 0;
    int wave_found_flag = 0, wave_is_on = 0;
    void parent = getentityproperty(self,"parent");

    if ( getglobalvar("in_menu") != 1 && openborvariant("in_level") ) { // Waiting, Select
        if ( getlevelproperty("type") != 2 ) {

            check_interact_entities(self);
            for (i = 0; i < get_interact_ent_size(); ++i) { // openborvariant("count_entities") --- openborconstant("MAX_ENTS")
                void ent = get_interact_ent(i);

                if ( getentityproperty(ent, "exists") ) {
                    int type = getentityproperty(self,"type");
                    char ent_type = getentityvar(ent, "type");

                    /// WAVE ///
                    if ( getentityproperty(parent,"exists") ) {
                        if ( !is_on_base(parent) ) {
                            if ( ent_type == "wave_r" || ent_type == "wave_l" || ent_type == "wave_c" ) {
                                wave_found_flag = 1;
                                if ( !wave_is_on || wave_is_on == 2 ) {
                                    wave_is_on = check_wave(self, ent);
                                    setlocalvar("wave_is_on_board_flag",wave_is_on);
                                }
                                //if (wave_is_on == 1) break;
                            } // fine if wave
                        }
                    }

                    /// STAIR ///
                    if ( ent_type == "stair_r" || ent_type == "stair_l" || ent_type == "stair_q" ) {
                        stair_found_flag = 1;
                        if ( !stair_is_on || stair_is_on == 2 ) {
                            tmp_stair_is_on = check_side_stairs(self, ent);
                            if ( tmp_stair_is_on ) { stair_is_on = tmp_stair_is_on; setlocalvar("stair_is_on_board_flag",stair_is_on); }
                        }
                        //if (stair_is_on == 1) break;
                    } else if ( ent_type == "stair_c" ) {
                        stair_found_flag = 1;
                        if ( !stair_is_on || stair_is_on == 2 ) {
                            tmp_stair_is_on = check_frontal_stairs(self, ent);
                            if ( tmp_stair_is_on ) { stair_is_on = tmp_stair_is_on; setlocalvar("stair_is_on_board_flag",stair_is_on); }
                        }
                        //if (stair_is_on == 1) break;
                    } // fine if stair
                } // fine if exists
            } // fine for all_ents

            /// WAVE ///
            if ( !wave_found_flag || wave_is_on <= 0 ) {
                reset_wave(self);
                setlocalvar("wave_is_on_board_flag",NULL());
            }

            /// STAIR ///
            if ( !stair_found_flag || stair_is_on <= 0 ) {
                reset_stairs(self);
                setlocalvar("stair_is_on_board_flag",NULL());
            }
        }
    }
}

