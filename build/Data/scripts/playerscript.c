#include "data/scripts/afterimg.c"
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
            afterimg();
         }

      }
   }

}