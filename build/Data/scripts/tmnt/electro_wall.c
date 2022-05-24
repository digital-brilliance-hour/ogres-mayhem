
#import "data/scripts/lib.c"

void main() {
  void self = getlocalvar("self");
  //int damage = getlocalvar("damage");
  //int drop = getlocalvar("drop");
  //int attacktype = getlocalvar("attacktype");
  //void damagetaker = getlocalvar("damagetaker");
  int damage = 10;
  int drop = 1;
  int pauseadd = 5;
  void opp = getentityproperty(self,"opponent");
  int width = 6, height = 157, depth = 9;
  float threshold = 1.5;
  float xshift = 0.25;
  float alt = 1;

        //drawstring( 10,200,0,"opp: "+opp);
		if ( getentityproperty(opp,"exists") ) {
            int eta = openborvariant("game_speed")*1;
            int time = openborvariant("elapsed_time");
            float x = getentityproperty(self, "x");
            float xo = getentityproperty(opp, "x");
            float zo = getentityproperty(opp, "z");
            float ao = getentityproperty(opp, "y");

            if ( getentityvar(opp,"weapon") >= 1 ) return;

            damage = 10;

			bindentity(opp,NULL());

            damageentity(opp,self,10,drop,openborconstant("ATK_SHOCK"));
            playsample(loadsample("data/sounds/rrodney03.wav"));
            changeentityproperty(opp, "velocity", 0,0,0);
			changeentityproperty(opp, "tosstime", get_next_time(eta));
			changeentityproperty(opp, "hitbyid", 0);

            if (xo <= x+(width/2)-1) {
                changeentityproperty(opp, "position", x-width-1,zo,ao+alt);
                xshift *= -1;
                changeentityproperty(opp, "direction", 0);
            } else {
                changeentityproperty(opp, "position", x+width+1,zo,ao+alt);
                changeentityproperty(opp, "direction", 1);
            }

            if ( getentityproperty(opp, "health") <= 0 ) {
                    changeentityproperty(opp, "falldie", 2);
            }

            tossentity(opp, 0, xshift, 0);
            changeentityproperty(opp,"freezetime",get_next_time(pauseadd));
            changeentityproperty(opp,"frozen",1);

            //damageentity(opp,self,10,drop,openborconstant("ATK_SHOCK"));
            changeentityproperty(opp, "opponent", NULL()); // evitiamo problemi con backpains
            changeentityproperty(self, "opponent", NULL());
		} /*else {
		    int i;
		    void ent;

		    for (i = 0; i < openborvariant("count_entities"); ++i) { // openborconstant("MAX_ENTS")
                ent = getentity(i);

                if ( getentityproperty(ent, "exists") ) {
                    float x = getentityproperty(self, "x");
                    float z = getentityproperty(self, "z");
                    float a = getentityproperty(self, "y");
                    float base = getentityproperty(self, "base");
                    float xo = getentityproperty(ent, "x");
                    float zo = getentityproperty(ent, "z");
                    float ao = getentityproperty(ent, "y");
                    float bo = getentityproperty(ent, "base");
                    int anim_id = getentityproperty(ent, "animationid");

                    if ( xo >= x-threshold && xo <= x+threshold && zo <= z && zo >= z-depth && bo >= base && ao <= base+height ) {
                        // Cioè se nn è vulnerabile e se è un foot o tartaruga (solo loro hanno la entityvar 6 sempre piena)...
                        if ( getentityvar(ent, 6) != NULL() && anim_id != openborconstant("ANI_SHOCK") && getentityproperty(ent, "health") > 0 ) {
                                changeentityproperty(ent, "velocity", 0,0,0);
                                if (xo <= x+(width/2)-1) changeentityproperty(ent, "position", x,zo,ao);
                                else changeentityproperty(ent, "position", x+(width/2),zo,ao);
                                setidle(ent, openborconstant("ANI_IDLE"), 1);
                                //performattack(ent, openborconstant("ANI_IDLE"), 1);
                                //changeentityproperty(opp, "animation", openborconstant("ANI_IDLE"));
                        }
                    } // fine if coords
                } // fine if exists
		    } // fine for

		}*/ // fine else


}

