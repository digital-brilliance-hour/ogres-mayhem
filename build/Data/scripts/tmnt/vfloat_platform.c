
#include "data/scripts/defines.h"

void main() {
  void self = getlocalvar("self");

    //check_opp(self, 20);

    //drawstring( 10,190,0,"Var (a): "+getentityproperty(self, "y"));
    //drawstring( 10,200,0,"Var (base): "+getentityproperty(self, "base"));
    //drawstring( 10,210,0,"Var (tossv): "+getentityproperty(self, "tossv"));
}

int check_opp(void self, float threshold) {
  float width = getentityvar(self, 1);
  float depth = getentityvar(self, 2);
  float height = getentityproperty(self, "height");
  float x = getentityproperty(self, "x");
  float z = getentityproperty(self, "z");
  float a = getentityproperty(self, "y");
  float speed = getentityproperty(self, "speed");
  float xdir = getentityproperty(self, "xdir");
  float threshold = 10;
  int i;

    //for (i = 0; i < openborvariant("count_entities"); ++i) { // openborconstant("MAX_ENTS")
        //void opp = getentity(i);
        void opp = getentityproperty(self,"opponent");

        if ( opp ) {
            float xo = getentityproperty(opp, "x");
            float zo = getentityproperty(opp, "z");
            float ao = getentityproperty(opp, "y");

            if ( xo >= x-width && xo <= x+width && zo <= z+depth && zo >= z-depth && ao >= a+height && ao <= a+height+threshold ) {
                //drawstring( 10,160,0,"Var (opp a): "+getentityproperty(opp, "y"));
                //drawstring( 10,170,0,"Var (opp base): "+getentityproperty(opp, "base"));
                //drawstring( 10,180,0,"Var (opp tossv): "+getentityproperty(opp, "tossv"));
                if ( getentityproperty(opp, "animationid") == openborconstant("ANI_WALKOFF")
                    || (getentityproperty(opp, "aiflag", "idling") == 1 || getentityproperty(opp, "aiflag", "idling") == 3) ) {
                    changeentityproperty(opp, "position", NULL(), NULL(), a+height);
                    changeentityproperty(opp, "base", a+height);
                    //changeentityproperty(opp, "velocity", NULL(), NULL(), 0);
                    if ( getentityproperty(opp, "animationid") == openborconstant("ANI_WALKOFF") ) {
                            setidle(opp, openborconstant("ANI_IDLE"), 1);
                    }
                }
            } // fine if soglia
        }

    //} // fine for


}
