
#include "data/scripts/defines.h"

void main() {
  void self = getlocalvar("self");

    check_opp(self,10,50,"ATK_NORMAL");

}

int check_opp(void self, float threshold, float t_dead, void ATK_TYPE) {
  float width = getentityvar(self, 1);
  float depth = getentityvar(self, 2);
  float height = getentityproperty(self, "height");
  float x = getentityproperty(self, "x");
  float z = getentityproperty(self, "z");
  float a = getentityproperty(self, "y");
  float speed = getentityproperty(self, "speed");
  float xdir = getentityproperty(self, "xdir");
  int i;

    //for (i = 0; i < MAX_PLAYERS; ++i) {
        //void opp = getplayerproperty(i, "entity");
        void opp = getentityproperty(self,"opponent");

        if ( getentityproperty(opp, "exists") ) {
            float xo = getentityproperty(opp, "x");
            float zo = getentityproperty(opp, "z");
            float ao = getentityproperty(opp, "y");
            int hres = openborvariant("hresolution");
            int vres = openborvariant("vresolution");
            float scr_posx = openborvariant("xpos");
            float scr_posy = openborvariant("ypos");

            if ( xo > x-width/2-width/4 && xo < x+width/2+width/4 && zo > z-depth && zo < z+5 && ao >= a && ao <= a+height ) {
                if ( xo < x && xdir < 0 ) changeentityproperty(opp, "velocity", -1*speed*4, NULL(), NULL());
                else if ( xo > x && xdir > 0 ) changeentityproperty(opp, "velocity", speed*4, NULL(), NULL());

                if ( xo <= scr_posx+threshold ) {
                    //drawstring( 10,200,0,"OK");
                    changeentityproperty(opp, "subject_to_screen", 0);
                    changeentityproperty(opp, "opponent", self);
                    //changeentityproperty(opp, "animation", openborconstant("ANI_WALKOFF"));
                } else if ( xo >= scr_posx+hres-threshold ) {
                    //drawstring( 10,200,0,"OK");
                    changeentityproperty(opp, "subject_to_screen", 0);
                    changeentityproperty(opp, "opponent", self);
                    //changeentityproperty(opp, "animation", openborconstant("ANI_WALKOFF"));
                }

                if ( xo <= scr_posx-t_dead ) {
                    int health = getentityproperty(opp,"health");
                    damageentity(opp,self,health,0,openborconstant(ATK_TYPE));
                } else if ( xo >= scr_posx+hres+t_dead ) {
                    int health = getentityproperty(opp,"health");
                    damageentity(opp,self,health,0,openborconstant(ATK_TYPE));
                }

            } // fine if coords
        } // fine if exists
    //} // fine for


}
