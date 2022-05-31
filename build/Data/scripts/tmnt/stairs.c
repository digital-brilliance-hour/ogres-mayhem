
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/ladder_commons.c"
#import "data/scripts/flying_script.c"

#define T_WALKOFF 4
#define T_TOSSTIME get_T_TOSSTIME()
#define T_EDGES 2
#define T_LAYER 1000
#define T_FRONTAL_FACTOR 2
#define T_FRONTAL_Z_BOUND 10
#define T_DIV_BASE_FACT 10

void main() {

    //drawstring( 10,170,0,"W!");

}

int is_on_stair() {
    if ( glv("is_on_step_stair") || glv("is_on_stair") || glv("is_on_stair_sider") || glv("is_on_stair_sidel") || glv("is_on_stair_sideq") ) return 1;
    else return 0;
}

int check_step_stair(void self, float threshold) {
   float x = getentityproperty(self, "x");
   float z = getentityproperty(self, "z");
   float a = getentityproperty(self, "y");
   float base = getentityproperty(self, "base");
   void platform, actual_platform;

   if ( getglobalvar("in_menu") == 1 || getlevelproperty("type") == 2 ) return 0;

    if ( getentityproperty(self, "aiflag", "jumping") == 0 ) {
            // SALI
           platform = find_platform_in_a(x+threshold,z,a,MAX_ALTITUDE);
           if ( platform != NULL() && getentityproperty(self, "xdir") > 0 ) {
                void type_name = getentityvar(platform, 0);

                //drawstring( 10,170,0,"platform:  "+platform);
                if ( type_name == "step" ) {
                    float height = getentityvar(platform, "height");
                    float width = getentityvar(platform, "width");
                    float depth = getentityvar(platform, "depth");
                    float a_step = getentityproperty(platform, "y");

                    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                    changeentityproperty(self, "no_adjust_base", 1);
                    setlocalvar("is_on_step_stair", 1);
                    changeentityproperty(self, "position", x+threshold,NULL(),a_step+height);
                    changeentityproperty(self, "base", a_step+height);
                }
           }

           platform = find_platform_in_a(x-threshold,z,a,MAX_ALTITUDE);
           if ( platform != NULL() && getentityproperty(self, "xdir") < 0 ) {
                void type_name = getentityvar(platform, "type");

                if ( type_name == "step" ) {
                    float height = getentityproperty(platform, "height");
                    float width = getentityvar(platform, "width");
                    float depth = getentityvar(platform, "depth");
                    float a_step = getentityproperty(platform, "y");

                    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                    changeentityproperty(self, "no_adjust_base", 1);
                    setlocalvar("is_on_step_stair", 1);
                    changeentityproperty(self, "position", x-threshold,NULL(),a_step+height);
                    changeentityproperty(self, "base", a_step+height);
                }
           }

           platform = find_platform_in_a(x,z-threshold,a,MAX_ALTITUDE);
           if ( platform != NULL() && getentityproperty(self, "zdir") < 0 ) {
                void type_name = getentityvar(platform, "type");

                if ( type_name == "step" ) {
                    float height = getentityproperty(platform, "height");
                    float width = getentityvar(platform, "width");
                    float depth = getentityvar(platform, "depth");
                    float a_step = getentityproperty(platform, "y");

                    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                    changeentityproperty(self, "no_adjust_base", 1);
                    setlocalvar("is_on_step_stair", 1);
                    changeentityproperty(self, "position", NULL(),z-threshold,a_step+height);
                    changeentityproperty(self, "base", a_step+height);
                }
           }

           platform = find_platform_in_a(x,z+threshold,a,MAX_ALTITUDE);
           if ( platform != NULL() && getentityproperty(self, "zdir") > 0 ) {
                void type_name = getentityvar(platform, "type");

                if ( type_name == "step" ) {
                    float height = getentityproperty(platform, "height");
                    float width = getentityvar(platform, "width");
                    float depth = getentityvar(platform, "depth");
                    float a_step = getentityproperty(platform, "y");

                    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                    changeentityproperty(self, "no_adjust_base", 1);
                    setlocalvar("is_on_step_stair", 1);
                    changeentityproperty(self, "position", NULL(),z+threshold,a_step+height);
                    changeentityproperty(self, "base", a_step+height);
                }
           }

            // SCENDI
           actual_platform = checkplatform(x,z,a);

           platform = checkplatform(x-threshold,z,a);
           if ( platform != NULL() && platform != actual_platform && getentityproperty(self, "xdir") < 0 ) {
                void type_name = getentityvar(platform, "type");

                if ( type_name == "step" ) {
                    float height = getentityproperty(platform, "height");
                    float width = getentityvar(platform, "width");
                    float depth = getentityvar(platform, "depth");
                    float a_step = getentityproperty(platform, "y");

                    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                    changeentityproperty(self, "no_adjust_base", 1);
                    setlocalvar("is_on_step_stair", 1);
                    changeentityproperty(self, "position", x-threshold,NULL(),a_step+height);
                    changeentityproperty(self, "base", a_step+height);
                }
           }

           platform = checkplatform(x+threshold,z,a);
           if ( platform != NULL() && platform != actual_platform && getentityproperty(self, "xdir") > 0 ) {
                void type_name = getentityvar(platform, "type");

                //drawstring( 10,170,0,"platform:  "+platform);
                if ( type_name == "step" ) {
                    float height = getentityproperty(platform, "height");
                    float width = getentityvar(platform, "width");
                    float depth = getentityvar(platform, "depth");
                    float a_step = getentityproperty(platform, "y");

                    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                    changeentityproperty(self, "no_adjust_base", 1);
                    setlocalvar("is_on_step_stair", 1);
                    changeentityproperty(self, "position", x+threshold,NULL(),a_step+height);
                    changeentityproperty(self, "base", a_step+height);
                }
           }

           platform = checkplatform(x,z-threshold,a);
           if ( platform != NULL() && platform != actual_platform && getentityproperty(self, "zdir") < 0 ) {
                void type_name = getentityvar(platform, "type");

                if ( type_name == "step" ) {
                    float height = getentityproperty(platform, "height");
                    float width = getentityvar(platform, "width");
                    float depth = getentityvar(platform, "depth");
                    float a_step = getentityproperty(platform, "y");

                    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                    changeentityproperty(self, "no_adjust_base", 1);
                    setlocalvar("is_on_step_stair", 1);
                    changeentityproperty(self, "position", NULL(),z-threshold,a_step+height);
                    changeentityproperty(self, "base", a_step+height);
                }
           }

           platform = checkplatform(x,z-threshold,a);
           if ( platform != NULL() && platform != actual_platform && getentityproperty(self, "zdir") > 0 ) {
                void type_name = getentityvar(platform, "type");

                if ( type_name == "step" ) {
                    float height = getentityproperty(platform, "height");
                    float width = getentityvar(platform, "width");
                    float depth = getentityvar(platform, "depth");
                    float a_step = getentityproperty(platform, "y");

                    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                    changeentityproperty(self, "no_adjust_base", 1);
                    setlocalvar("is_on_step_stair", 1);
                    changeentityproperty(self, "position", NULL(),z+threshold,a_step+height);
                    changeentityproperty(self, "base", a_step+height);
                }
           }

           if ( platform == NULL() && getentityproperty(self, "no_adjust_base") == 1 && getlocalvar("is_on_step_stair") == 1 ) {
               if ( getlocalvar("stored_nab") != NULL() ) changeentityproperty(self, "no_adjust_base", getlocalvar("stored_nab"));
               else changeentityproperty(self, "no_adjust_base", 0);
               setlocalvar("is_on_step_stair", 0);
           }

    } // fine if jumping
    else if ( getentityproperty(self, "no_adjust_base") == 1 && getlocalvar("is_on_step_stair") == 1 ) {
        if ( getlocalvar("stored_nab") != NULL() ) changeentityproperty(self, "no_adjust_base", getlocalvar("stored_nab"));
        else changeentityproperty(self, "no_adjust_base", 0);
        setlocalvar("is_on_step_stair", 0);
    }

}


int check_stair(void self) {
    //void stair = getentityproperty(self, "opponent");
    int i, found_flag = 0, is_on = 0;
    //reset_opp_by_range(self,stair,openborconstant("ANI_IDLE"));

    if ( getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 ) { // Waiting, Select
        for (i = 0; i < openborvariant("count_entities"); ++i) { // openborconstant("MAX_ENTS")
            void stair = getentity(i);

            if ( getentityproperty(stair, "exists") ) {
                if ( getentityvar(stair, "type") == "stair_r" || getentityvar(stair, "type") == "stair_l" || getentityvar(stair, "type") == "stair_q" ) {
                    found_flag = 1;
                    is_on = check_side_stairs(self, stair);
                    if (is_on == 1) break;
                } else if ( getentityvar(stair, "type") == "stair_c" ) {
                    found_flag = 1;
                    is_on = check_frontal_stairs(self, stair);
                    if (is_on == 1) break;
                } // fine if stair
            } // fine if exists
        } // fine for all_ents


        if ( !found_flag || is_on <= 0 ) {
            reset_stairs(self);
        }
    }
}

int reset_stairs(void self, int z_flag) {
    if ( getlocalvar("is_on_stair_sider") == 1 || getlocalvar("is_on_stair_sidel") == 1 || getlocalvar("is_on_stair_sideq") == 1 || getlocalvar("is_on_stair") == 1 ) {
        if ( getlocalvar("stored_nab") != NULL() ) changeentityproperty(self, "no_adjust_base", getlocalvar("stored_nab"));
        else changeentityproperty(self, "no_adjust_base", 0);
    }

    if ( getlocalvar("is_on_stair_sider") == 1 ) {
        setlocalvar("is_on_stair_sider", NULL());
    }
    if ( getlocalvar("is_on_stair_sidel") == 1 ) {
        setlocalvar("is_on_stair_sidel", NULL());
    }
    if ( getlocalvar("is_on_stair_sideq") == 1 ) {
        setlocalvar("is_on_stair_sideq", NULL());
    }
    if ( getlocalvar("is_on_stair") == 1 ) {
        float a = getentityproperty(self,"y");
        float base = getlocalvar("base_player");
        float zs = getlocalvar("zs_stair");
        float bases = getlocalvar("bases_stair");

        if ( zs == NULL() ) zs = 0;
        if ( bases == NULL() ) bases = 0;
        if ( base == NULL() ) base = getentityproperty(self,"base");

        if ( get_flying_base(self) != NULL() ) base = get_flying_base(self);
        if ( z_flag == NULL() || z_flag <= 0 ) changeentityproperty(self, "position", NULL(), zs-((base-bases)/5), NULL());

        setlocalvar("zs_stair", NULL());
        setlocalvar("bases_stair", NULL());
        setlocalvar("base_player", NULL());
        setlocalvar("is_on_stair", NULL());
        setlocalvar("frontal_stair_base", NULL());

        if ( getentityvar(self, "stair_layer") != NULL() ) changeentityproperty(self, "setlayer", getentityvar(self, "stair_layer"));
        setentityvar(self,"stair_layer", NULL());
    }
    setentityvar(self,"stair",NULL());

    /*if ( glv("is_on_step_stair") == 1 ) { //  reset in step_stair function
        if ( getlocalvar("stored_nab") != NULL() ) changeentityproperty(self, "no_adjust_base", getlocalvar("stored_nab"));
        else changeentityproperty(self, "no_adjust_base", 0);
        setlocalvar("is_on_step_stair", NULL());
    }*/

    if ( get_flying_base(self) != NULL() ) set_flying_base(self,NULL());
}

int check_frontal_stairs(void self, void stair) {
   float x = getentityproperty(self, "x");
   float z = getentityproperty(self, "z");
   float a = getentityproperty(self, "y");
   float base = getentityproperty(self, "base");
   float speed = getentityproperty(self, "speed");
   float xdir = getentityproperty(self, "xdir");
   float zdir = getentityproperty(self, "zdir");
   float tossv = getentityproperty(self, "tossv");
   float xs = getentityproperty(stair, "x");
   float zs = getentityproperty(stair, "z");
   float as = getentityproperty(stair, "y");
   float bases = getentityproperty(stair, "base");
   float height = getentityvar(stair, "height");
   float width = getentityvar(stair, "width");
   float depth = getentityvar(stair, "depth");
   int subject_to_depth = getentityvar(stair,"subject_to_depth");
   int subject_to_base = getentityvar(stair,"subject_to_base");
   int subject_to_edges = getentityvar(stair,"subject_to_edges");
   int subject_to_stair = getentityvar(self,"subject_to_stair");
   float shifta = speed/3, mul_shifta;
   int lost_frames = get_lost_frames();
   float new_base, tmp_new_base, zshift, xshift;

    if ( subject_to_stair <= 0 ) {
        //reset_stairs(self);
        return 0;
    }

   if ( lost_frames == 0 ) lost_frames = 1;
   mul_shifta = sum(shifta,lost_frames);

   if ( depth == NULL() ) depth = 0;

   if ( height == NULL() ) height = 0;
   width /= 4;

   if ( z > zs ) zshift = z-zs;
   else zshift = zs-z;

   // base:z=height:(zs+width) -> base = (z*height)/(zs+width)
   new_base = (zshift*height)/(width);
   new_base += as;
   new_base /= 2;
   tmp_new_base = new_base*2;

   /*drawstring( 10,110,0,"is_on_stair:  "+getlocalvar("is_on_stair"));
   drawstring( 10,120,0,"base:  "+base);
   drawstring( 10,130,0,"a:  "+a);
   drawstring( 10,140,0,"frontal_stair_base:  "+getlocalvar("frontal_stair_base"));
   drawstring( 10,150,0,"no_adjust_base:  "+getentityproperty(self,"no_adjust_base"));
   drawstring( 10,160,0,"stair_layer:  "+getentityvar(self,"stair_layer"));
   drawstring( 10,170,0,"is_flying:  "+is_flying(self));*/

   if ( getlocalvar("is_on_stair") == NULL() && x <= xs+depth+2 && x >= xs-2 && z <= zs+T_FRONTAL_FACTOR+T_FRONTAL_Z_BOUND && z >= zs-width-width/8 ) {
        //drawstring( 10,170,0,"new_base:  "+new_base);
        //drawstring( 10,190,0,"as:  "+as);
        if ( subject_to_base && a <= new_base+T_WALKOFF && !is_flying(self) && tossv <= 0 && !is_in_pain(self) ) {
            changeentityproperty(self, "position", NULL(),NULL(),new_base);
            a = getentityproperty(self, "y");
        }
        if ( base != new_base && a <= new_base+T_WALKOFF && a >= new_base-T_WALKOFF ) {
            float tmp_new_base2 = new_base;
            if ( getentityproperty(self, "aiflag", "jumping") || tossv != 0 ) tmp_new_base2 = tmp_new_base; // ebita sul primo gradino, quando subject_to_base è on, di usare l'altezza tmp_new_base. usa invece new_base!

            if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
            changeentityproperty(self, "no_adjust_base", 1);
            changeentityproperty(self, "base", tmp_new_base2);
            setlocalvar("base_player", tmp_new_base2);
            setlocalvar("frontal_stair_base", tmp_new_base2);
            if ( is_flying(self) ) set_flying_base(self,tmp_new_base2);
            if ( !is_flying(self) ) changeentityproperty(self, "position", NULL(),NULL(), tmp_new_base2);
            if ( getentityvar(self,"stair_layer") == NULL() ) setentityvar(self,"stair_layer", getentityproperty(self, "setlayer"));
            setlocalvar("is_on_stair", 1);
            setentityvar(self,"stair",stair);
            setlocalvar("zs_stair", zs);
            setlocalvar("bases_stair", bases);

            if ( getentityproperty(self, "tosstime") != 0 ) changeentityproperty(self, "tosstime", 0);

            if ( z != zs+T_FRONTAL_FACTOR ) {
                if ( subject_to_depth == 2 ) {
                    changeentityproperty(self, "position", xs+depth/2, NULL(), NULL());
                } else if ( subject_to_depth > 0 && xdir == 0 ) {
                    changeentityproperty(self, "position", xs+depth/2, NULL(), NULL());
                }
                changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL()); // il +2 è per mostrare il pg anche quando va UP
            }

            if ( (!getentityproperty(self, "aiflag", "jumping") && zdir == 0 && tossv == 0) || (tossv < 0) || (a <= base) ) { // (tossv < 0 && zdir == 0)
                //finish_anim(self); // for landframes

                if ( subject_to_depth == 2 ) {
                    changeentityproperty(self, "position", xs+depth/2, NULL(), NULL());
                } else if ( subject_to_depth > 0 && xdir == 0 ) {
                    changeentityproperty(self, "position", xs+depth/2, NULL(), NULL());
                }

                changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL()); // il +2 è per mostrare il pg anche quando va UP
                if ( !is_flying(self) ) changeentityproperty(self, "position", NULL(),NULL(), tmp_new_base2);
                //changeentityproperty(self, "tosstime", T_TOSSTIME);
                reset_jumping(self,1);
            } // fine if jump, zdir
        } else if ( base < new_base && a > new_base && a != base && is_flying(self) ) { // && is_flying(self)
            if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
            changeentityproperty(self, "no_adjust_base", 1);
            changeentityproperty(self, "base", tmp_new_base);
            setlocalvar("base_player", tmp_new_base);
            setlocalvar("frontal_stair_base", tmp_new_base);
            if ( is_flying(self) ) set_flying_base(self,tmp_new_base);
            changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL());

            if ( getentityvar(self,"stair_layer") == NULL() ) setentityvar(self,"stair_layer", getentityproperty(self, "setlayer"));
            setlocalvar("is_on_stair", 1);
            setentityvar(self,"stair",stair);
            setlocalvar("zs_stair", zs);
            setlocalvar("bases_stair", bases);

            return 2; // possono esserci più scale in alto a queste coords. continuiamo la ricerca.
        } // fine if base

        if ( a <= new_base+T_WALKOFF && a >= new_base-T_WALKOFF ) return 1;
   } else if ( getlocalvar("is_on_stair") == 1 && x <= xs+depth+2 && x >= xs-2 && z <= zs+T_FRONTAL_FACTOR+T_FRONTAL_Z_BOUND && z >= zs-width-width/8 ) { // sei sulla scala
        /*drawstring( 10,100,0,"no_adjust_base:  "+getentityproperty(self, "no_adjust_base"));
        drawstring( 10,110,0,"base:  "+base);
        drawstring( 10,120,0,"a:  "+a);
        drawstring( 10,130,0,"is_flying:  "+is_flying(self));*/
        //drawstring( 10,100,0,"is_on_stair:  "+getlocalvar("is_on_stair"));

        if ( !getentityproperty(self, "aiflag", "jumping") ) {
            /*int layer = getentityproperty(self, "setlayer");
            int orig_layer = getentityvar(self,"stair_layer");
            if ( layer != orig_layer && orig_layer != NULL() ) changeentityproperty(self, "setlayer", orig_layer);*/
            if ( getentityvar(self, "stair_layer") != NULL() ) {
                int new_layer = getentityproperty(stair, "setlayer")+T_LAYER;
                if ( getlocalvar("base_player") > T_DIV_BASE_FACT ) new_layer -= truncA(getlocalvar("base_player")/T_DIV_BASE_FACT);
                else if ( getlocalvar("base_player") > 0 ) new_layer -= truncA(getlocalvar("base_player"));
                if ( getentityproperty(self, "setlayer") != new_layer ) changeentityproperty(self, "setlayer", new_layer);
            }

            if ( getentityproperty(self,"tossv") == 0 ) {
                if ( subject_to_depth == 2 ) {
                    changeentityproperty(self, "position", xs+depth/2, NULL(), NULL());
                } else if ( subject_to_depth > 0 && xdir == 0 ) {
                    changeentityproperty(self, "position", xs+depth/2, NULL(), NULL());
                }
            }

            if ( subject_to_edges ) {
                if ( xdir < 0 && x < xs+T_EDGES ) changeentityproperty(self, "position", xs+T_EDGES, NULL(), NULL());
                else if ( xdir > 0 && x > xs+depth-T_EDGES ) changeentityproperty(self, "position", xs+depth-T_EDGES, NULL(), NULL());
            }

            // sta sulla scala in volo e smette di volare -> atterra su scala
            if ( !is_flying(self) && getlocalvar("frontal_stair_base") && a > base && !getentityproperty(self,"no_adjust_base") ) {
                changeentityproperty(self, "no_adjust_base", 1);
                //changeentityproperty(self, "tosstime", T_TOSSTIME);
                changeentityproperty(self, "base", getlocalvar("frontal_stair_base"));
                setlocalvar("base_player", getlocalvar("frontal_stair_base"));
                changeentityproperty(self, "position", NULL(), NULL(), getentityproperty(self,"base"));
                changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL());
                changeentityproperty(self,"velocity",0,0,0);
                //setlocalvar("frontal_stair_base", NULL());

                return 1;
            }
            // sta su scala ma non vola -> inizia a volare
            if ( is_flying(self) && getlocalvar("frontal_stair_base") && a == base && getentityproperty(self,"no_adjust_base") ) {
                changeentityproperty(self, "no_adjust_base", 1);
                changeentityproperty(self, "base", getlocalvar("frontal_stair_base"));
                setlocalvar("base_player", getlocalvar("frontal_stair_base"));
                set_flying_base(self,getlocalvar("frontal_stair_base"));
                changeentityproperty(self, "position", NULL(), NULL(), getentityproperty(self,"base"));
                changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL());

                return 1;
            }

            if ( zdir > 0 ) { // scende le scale
                if ( is_flying(self) ) {
                    //set_flying(self,0);
                    changeentityproperty(self, "no_adjust_base", 1);
                    if ( getlocalvar("frontal_stair_base") == NULL() ) setlocalvar("frontal_stair_base",tmp_new_base);
                    setlocalvar("frontal_stair_base",getlocalvar("frontal_stair_base")-mul_shifta);
                    changeentityproperty(self, "base", getlocalvar("frontal_stair_base"));
                    setlocalvar("base_player", getlocalvar("frontal_stair_base"));
                    set_flying_base(self,getlocalvar("frontal_stair_base"));
                    changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL());
                    //set_flying(self,1);
                } else {
                    //changeentityproperty(self, "tosstime", T_TOSSTIME);
                    if ( a == base ) changeentityproperty(self, "position", NULL(), NULL(), base-mul_shifta); // if ( a == base )
                    changeentityproperty(self, "base", base-mul_shifta);
                    setlocalvar("base_player", base-mul_shifta);
                    setlocalvar("frontal_stair_base",base-mul_shifta);
                    if ( a-1 > as ) changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL()); // a-1 perchè se sta sul bordo può uscire fuori dalla scala (avoid subject_to_base bug) a-1 > as
                }
            } else if ( zdir < 0 ) { // sale le scale
                if ( is_flying(self) ) {
                    //set_flying(self,0);
                    changeentityproperty(self, "no_adjust_base", 1);
                    if ( getlocalvar("frontal_stair_base") == NULL() ) setlocalvar("frontal_stair_base",tmp_new_base);
                    setlocalvar("frontal_stair_base",getlocalvar("frontal_stair_base")+mul_shifta);
                    changeentityproperty(self, "base", getlocalvar("frontal_stair_base"));
                    setlocalvar("base_player", getlocalvar("frontal_stair_base"));
                    set_flying_base(self,getlocalvar("frontal_stair_base"));
                    changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL());
                    //set_flying(self,1);
                } else {
                    //changeentityproperty(self, "tosstime", T_TOSSTIME);
                    if ( a == base ) changeentityproperty(self, "position", NULL(), NULL(), base+mul_shifta); // if ( a == base )
                    changeentityproperty(self, "base", base+mul_shifta);
                    setlocalvar("base_player", base+mul_shifta);
                    setlocalvar("frontal_stair_base",base+mul_shifta);
                    changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL());
                }
            } else if ( getentityproperty(self, "aiflag", "jumping") == 1 ) {
                //changeentityproperty(self, "tosstime", openborvariant("elapsed_time")-1);
                if ( getentityproperty(self, "tosstime") != 0 ) changeentityproperty(self, "tosstime", 0);
            } else { // sta fermo
                if ( is_flying(self) ) {
                    changeentityproperty(self, "no_adjust_base", 1);
                    changeentityproperty(self, "base", getlocalvar("frontal_stair_base"));
                    setlocalvar("base_player", getlocalvar("frontal_stair_base"));
                    set_flying_base(self,getlocalvar("frontal_stair_base"));
                } else changeentityproperty(self, "no_adjust_base", 1);

                //changeentityproperty(self, "base", a);
                if ( z != zs+T_FRONTAL_FACTOR ) changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR, NULL()); // il +2 è per mostrare il pg anche quando va UP
                //changeentityproperty(self, "tosstime", T_TOSSTIME);
            }

            // sei agli estremi
            if ( is_flying(self) ) {
                if ( getlocalvar("frontal_stair_base") <= as && zdir > 0 ) { // alla base
                    changeentityproperty(self, "position", NULL(), zs+T_FRONTAL_FACTOR+T_FRONTAL_Z_BOUND, NULL());
                    reset_stairs(self,1);
                    return 0;
                } else if ( getlocalvar("frontal_stair_base") > as+height*(1.2) && zdir < 0 ) { // in cima
                    changeentityproperty(self, "position", NULL(), zs-width-width/8-1, NULL());
                    reset_stairs(self,1);
                    return 0;
                }
            } else {
                if ( a <= as && zdir > 0 ) { // alla base
                    if ( a == base && !is_flying(self) ) changeentityproperty(self, "position", NULL(), NULL(), as);
                    changeentityproperty(self, "base", as);
                    setlocalvar("base_player", as);
                    changeentityproperty(self, "tosstime", openborvariant("elapsed_time")-1);
                    reset_stairs(self,1);
                    return 0;
                } else if ( a > as+height && zdir < 0 ) { // in cima
                    if ( a == base && !is_flying(self) ) changeentityproperty(self, "position", NULL(), NULL(), as+height-height/3);
                    changeentityproperty(self, "base", as+height);
                    setlocalvar("base_player", as+height);
                    changeentityproperty(self, "position", NULL(), zs-width-width/8-1, NULL());
                    changeentityproperty(self, "tosstime", openborvariant("elapsed_time")-1);
                    reset_stairs(self,1);
                    return 0;
                }
            }
        } // fine if jump, zdir
        else { // salta in su dalla cima (salta fuori dalla scala)
            int new_layer = getentityproperty(stair, "setlayer")+T_LAYER;
            if ( getlocalvar("base_player") > T_DIV_BASE_FACT ) new_layer -= truncA(getlocalvar("base_player")/T_DIV_BASE_FACT);
            else if ( getlocalvar("base_player") > 0 ) new_layer -= truncA(getlocalvar("base_player"));
            if ( getentityvar(self, "stair_layer") == NULL() ) setentityvar(self,"stair_layer",getentityproperty(self, "setlayer"));
            if ( getentityproperty(self, "setlayer") != new_layer ) changeentityproperty(self, "setlayer", new_layer);

            if ( a > as+height && zdir < 0 ) { // in cima
                reset_stairs(self);
                changeentityproperty(self, "position", NULL(), zs-width-width/8-1, NULL());
                //changeentityproperty(self, "tosstime", openborvariant("elapsed_time")-1);
            }
        }

        return 1;
   } /*else if ( getlocalvar("is_on_stair") == 1 ) {  // fine if coords
       //changeentityproperty(self, "position", NULL(), zs-a/5, NULL());
   }*/

 return 0;
}

int check_side_stairs(void self, void stair) {
                float x = getentityproperty(self, "x");
                float z = getentityproperty(self, "z");
                float a = getentityproperty(self, "y");
                float base = getentityproperty(self, "base");
                float tossv = getentityproperty(self, "tossv");
                float xs = getentityproperty(stair, "x");
                float zs = getentityproperty(stair, "z");
                float as = getentityproperty(stair, "y");
                float bases = getentityproperty(stair, "base");
                float height = getentityvar(stair, "height");
                float width = getentityvar(stair, "width");
                float depth = getentityvar(stair, "depth");
                int subject_to_base = getentityvar(stair,"subject_to_base");
                int subject_to_stair = getentityvar(self,"subject_to_stair");
                float new_base, zshift, xshift;

                if ( subject_to_stair <= 0 ) {
                    //reset_stairs(self);
                    return 0;
                }

                if ( depth == NULL() ) depth = 0;

                // Lunghezza della scala
                if (x > xs) xshift = x-xs;
                else xshift = xs-x;
                // Cambiare zs in base a x del personaggio
                /*
                 * Calcoliamo lo zshift. L'ultimo gradino avrà zs = height - depth
                 * ?:x = zs:w -> ? = (x*zs)/w
                 */
                 //zshift = xshift*(height/7+depth)/width;

                // Aggiungere la "y" --- Calcoliamo l'altezza in un punto qualunque della scala -> W:H = X:? -> ?=(H*X)/W
                new_base = (height*xshift)/width;
                new_base += as;

                if ( getentityvar(stair, "type") == "stair_r" ) {
                    if ( x <= xs+width && x >= xs && z <= zs+2+depth/2 && z >= zs-depth/2 ) {
                        if ( subject_to_base && a <= new_base+T_WALKOFF && !is_flying(self) && tossv <= 0 && !is_in_pain(self) ) {
                            changeentityproperty(self, "position", NULL(),NULL(),new_base);
                            a = getentityproperty(self, "y");
                        }
                        if ( base != new_base && a <= new_base+T_WALKOFF && a >= new_base-T_WALKOFF ) {
                            set_entity_pos(self,stair,new_base);
                        } else if ( base < new_base && a > new_base && a != base && is_flying(self) ) { // && is_flying(self)
                            if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                            changeentityproperty(self, "no_adjust_base", 1);
                            changeentityproperty(self, "base", new_base);
                            set_flying_base(self,new_base);
                            setlocalvar("is_on_stair_sider", 1);
                            setentityvar(self,"stair",stair);

                            return 2; // possono esserci più scale in alto a queste coords. continuiamo la ricerca.
                        }

                        if ( a <= new_base+T_WALKOFF && a >= new_base-T_WALKOFF ) return 1;
                    } // fine if coords
                } else if ( getentityvar(stair, "type") == "stair_l" ) {
                    if ( x <= xs && x >= xs-width && z <= zs+2+depth/2 && z >= zs-depth/2 ) {

                        if ( subject_to_base && a <= new_base+T_WALKOFF && !is_flying(self) && tossv <= 0 && !is_in_pain(self) ) {
                            changeentityproperty(self, "position", NULL(),NULL(),new_base);
                            a = getentityproperty(self, "y");
                        }
                        if ( base != new_base && a <= new_base+T_WALKOFF && a >= new_base-T_WALKOFF ) {
                            set_entity_pos(self,stair,new_base);
                        } else if ( base < new_base && a > new_base && a != base && is_flying(self) ) { // && is_flying(self)
                            if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                            changeentityproperty(self, "no_adjust_base", 1);
                            changeentityproperty(self, "base", new_base);
                            set_flying_base(self,new_base);
                            setlocalvar("is_on_stair_sidel", 1);
                            setentityvar(self,"stair",stair);

                            return 2; // possono esserci più scale in alto a queste coords. continuiamo la ricerca.
                        }

                        if ( a <= new_base+T_WALKOFF && a >= new_base-T_WALKOFF ) return 1;
                    } // fine if coords
                } else if ( getentityvar(stair, "type") == "stair_q" ) {
                    new_base = height;
                    new_base += as;

                    if ( x <= xs && x >= xs-width && z <= zs+2+depth/2 && z >= zs-depth/2 ) {

                        if ( subject_to_base && a <= new_base+T_WALKOFF && !is_flying(self) && tossv <= 0 && !is_in_pain(self) ) {
                            changeentityproperty(self, "position", NULL(),NULL(),new_base);
                            a = getentityproperty(self, "y");
                        }
                        if ( base != new_base && a <= new_base+T_WALKOFF && a >= new_base-T_WALKOFF ) {
                            set_entity_pos(self,stair,new_base);
                        } else if ( base < new_base && a > new_base && a != base && is_flying(self) ) { // && is_flying(self)
                            if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                            changeentityproperty(self, "no_adjust_base", 1);
                            changeentityproperty(self, "base", new_base);
                            set_flying_base(self,new_base);
                            setlocalvar("is_on_stair_sideq", 1);
                            setentityvar(self,"stair",stair);

                            return 2; // possono esserci più scale in alto a queste coords. continuiamo la ricerca.
                        }

                        if ( a <= new_base+T_WALKOFF && a >= new_base-T_WALKOFF ) return 1;
                    } // fine if coords
                }
 return 0;
}

float set_entity_pos(void self, void stair, float new_base) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float zdir = getentityproperty(self, "zdir");
    float tossv = getentityproperty(self, "tossv");
    float xs = getentityproperty(stair, "x");
    float zs = getentityproperty(stair, "z");
    float as = getentityproperty(stair, "y");
    float bases = getentityproperty(stair, "base");
    int subject_to_depth = getentityvar(stair,"subject_to_depth");
    int subject_to_edges = getentityvar(stair,"subject_to_edges");
    //float height = getentityvar(stair, "height");
    //float width = getentityvar(stair, "width");
    float depth = getentityvar(stair, "depth");

    //drawstring( 10,160,0,"base:  "+base);
    //drawstring( 10,170,0,"a:  "+a);
    //drawstring( 10,160,0,"z:  "+z);
    //drawstring( 10,170,0,"zs:  "+zs);
    //drawstring( 10,180,0,"zdir:  "+zdir);

    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
    changeentityproperty(self, "no_adjust_base", 1);
    changeentityproperty(self, "base", new_base);
    if ( is_flying(self) ) set_flying_base(self,new_base);
    if ( getentityvar(stair, "type") == "stair_r" ) setlocalvar("is_on_stair_sider", 1);
    else if ( getentityvar(stair, "type") == "stair_l" ) setlocalvar("is_on_stair_sidel", 1);
    else if ( getentityvar(stair, "type") == "stair_q" ) setlocalvar("is_on_stair_sideq", 1);
    setentityvar(self,"stair",stair);

    if ( subject_to_edges ) {
        if ( zdir < 0 && z < zs-(depth/2)+T_EDGES ) changeentityproperty(self, "position", NULL(), zs-(depth/2)+T_EDGES, NULL());
        else if ( zdir > 0 && z > zs+(depth/2)-T_EDGES ) changeentityproperty(self, "position", NULL(), zs+(depth/2)-T_EDGES, NULL());
    }

    // togliendo zdir non si oltrepassano i bordi delle scale
    if ( (!getentityproperty(self, "aiflag", "jumping") && zdir == 0 && tossv == 0) || (tossv < 0) || (a <= base) ) { //(tossv < 0 && zdir == 0)
        //changeentityproperty(self, "tosstime", T_TOSSTIME);
        //changeentityproperty(self, "position", NULL(), zs+2, NULL());
        //finish_anim(self); // for landframes

        if ( subject_to_depth == 2 ) {
            changeentityproperty(self, "position", NULL(), zs+2, NULL()); // il +2 è per mostrare il pg anche quando va UP
        } else if ( subject_to_depth > 0 && zdir == 0 ) {
            changeentityproperty(self, "position", NULL(), zs+2, NULL());
        }

        if ( !is_flying(self) ) changeentityproperty(self, "position", NULL(), NULL(), new_base); // se non imposto la z il pg può cambiare la sua depth (impatta sul reset_jumping)
        //if ( is_flying(self) ) changeentityproperty(self, "base", new_base);

        reset_jumping(self,1);
    } // fine if jump, zdir
    // se il pg è già sulla scala (infatti non cade) facciamolo rimanere sospeso così appena
    // tocca su o giù può agire la depth per capire se il pg può cadere dalla scala o meno
    /*else if ( getentityproperty(self, "tossv") == 0 ) {
        changeentityproperty(self, "tosstime", T_TOSSTIME);
    }*/

    return new_base;
}

int finish_anim(void self) {
    //float x = getentityproperty(self, "x");
    //float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");

    //drawstring( 10,170,0,"base: " +base);
    //drawstring(10,170,0,"var: " +getentityproperty(self,"landframe","frame"));
    if ( a != base ){
        //if ( getentityproperty(self,"aiflag","idling") <= 0 && !is_in_pain(self) ) changeentityproperty(self,"takeaction","common_land",1);
        if ( getentityproperty(self,"aiflag","idling") <= 0 ) changeentityproperty(self,"aiflag","animating",0); // for landframes
        //changeentityproperty(self,"nextthink",0);
        //changeentityproperty(self,"nextmove",0);
        //if ( getentityproperty(self, "tossv") != 0 ) changeentityproperty(self,"tosstime",0);
        //changeentityproperty(self,"velocity",NULL(),NULL(),0);
        changeentityproperty(self,"aiflag","jumping",0);
        //changeentityproperty(self,"landframe","frame",0);
    } /*else {
        if ( getentityproperty(self,"aiflag","idling") <= 0 ) changeentityproperty(self,"takeaction","common_land",1);
    }*/
}

