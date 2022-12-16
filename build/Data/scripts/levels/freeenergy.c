#include "data/scripts/levelup/lvup.c"


void main() {

    
    void p;
    int i, hp, mp, lv, num;
    for(i=0; i<4; i++){
        p = getplayerproperty(i, "entity");
        num = i+1;
        if(p){
            if("Travel"!=getentityproperty(p, "model")) {
                changeplayerproperty(i, "score", 200000);
                lvup(i);
                changeentityproperty(p, "health", getentityproperty(p, "maxhealth"));
                changeentityproperty(p, "mp", getentityproperty(p, "maxmp"));
            }
        }
    }

    int  iMax       = openborvariant("ent_max");            //Entity count.
    int  iEntity;                                           //Loop counter.
    void vEntity;                                           //Target entity.
    for(iEntity=0; iEntity<iMax; iEntity++)
    {   
        vEntity = getentity(iEntity); 
        if(getentityproperty(vEntity, "type") == openborconstant("TYPE_ENEMY")) {
            changeentityproperty(vEntity, "health", getentityproperty(vEntity, "maxhealth"));
        }   
    } 
}
