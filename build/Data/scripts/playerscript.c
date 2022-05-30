#include "data/scripts/afterimg.c"
#import "data/scripts/animation/animation.c"
void main()
{
   void AniID = getani();
   if(AniID == openborconstant("ANI_SPECIAL")
   ||AniID == openborconstant("ANI_FREESPECIAL")
   ||AniID == openborconstant("ANI_FREESPECIAL2")
   ||AniID == openborconstant("ANI_FREESPECIAL3")
   ||AniID == openborconstant("ANI_FREESPECIAL4")
   ||AniID == openborconstant("ANI_FREESPECIAL5")
   ||AniID == openborconstant("ANI_FREESPECIAL6")
   ||AniID == openborconstant("ANI_FREESPECIAL7")
   ||AniID == openborconstant("ANI_FREESPECIAL12")
   ||AniID == openborconstant("ANI_FREESPECIAL10")
   ){
      if(AniID != openborconstant("ANI_IDLE")) {
         if(!openborvariant("slowmotion")){
            //afterimg();
         }

      }
   }
   /*if(AniID == openborconstant("ANI_IDLE")) {
      void icharge = getlocalvar("idle_charge");
      if(icharge == NULL()) {
         void gcharge = spawnparent("deku_greencharge", 0, 0, 0);
         setlocalvar("idle_charge", gcharge);
      }
   }
   else {
      void icharge = getlocalvar("idle_charge");
      if(icharge != NULL()) {
         killentity(icharge);
         setlocalvar("idle_charge", NULL());
      }
   }*/

   void self = getlocalvar("self");
   int mp    = getentityproperty(self, "mp"); //Get entity's MP
   int maxmp   = getentityproperty(self, "maxmp"); //Get entity's Max MP
   if(openborvariant("in_level")) {
      if(mp >= maxmp && getlocalvar("showing_count") != 1) {
         setlocalvar("mp_full", 1);
         setlocalvar("showing_count", 1);
      }
      else {
         setlocalvar("showing_count", 0);
      }
   }

   //update functions
   //showMPFull();

}

void oncreate() {
   setlocalvar("mp_full", 0);
   setlocalvar("showing_count", 0);
}

/*void showMPFull() {
   void self = getlocalvar("self");
   int mp    = getentityproperty(self, "mp"); //Get entity's MP
   int maxmp   = getentityproperty(self, "maxmp"); //Get entity's Max MP
   int mpFull = getlocalvar("mp_full");
   int col = 0;
   int vtime = openborvariant("elapsed_time");
   while (mpFull) {

      changedrawmethod(self, "enabled", 3);
      //changedrawmethod(self, "alpha", 2);
      changedrawmethod(self, "tintmode", 4);
      changedrawmethod(self, "tintcolor", rgbcolor (col, col, col));
      col+=10;
      if(col > 240) {
         vtime = openborvariant("elapsed_time");
         mpFull = 0;
         setlocalvar("m_full", 0);
      }
   }

   if (vtime <= vtime + 6000){ 
      changedrawmethod(self, "tintmode",0);
      changedrawmethod(self, "tintcolor", 0);
   }

}*/