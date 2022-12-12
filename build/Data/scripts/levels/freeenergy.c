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
                executeanimation(p, openborconstant("ANI_RUN"));
                void iUp = playerkeys(i, 0, "moveup"); // New key status of "Up"
                void iDown = playerkeys(i, 0, "movedown"); // New key status of "Down"
                void iLeft = playerkeys(i, 0, "moveleft"); // New key status of "Left"
                void iRight = playerkeys(i, 0, "moveright"); // New key status of "Right"
                void iUpR = playerkeys(i, 2, "moveup"); // New key status of "Up"
                void iDownR = playerkeys(i, 2, "movedown"); // New key status of "Down"
                void iLeftR = playerkeys(i, 2, "moveleft"); // New key status of "Left"
                void iRightR = playerkeys(i, 2, "moveright"); // New key status of "Right"

                int Vx, Vy, Vz;
                Vx = 0;
                Vy = 0;
                Vz = 0;


                if(iRight) {
                    //changeentityproperty(p, "velocity", 1, 0, 0);
                    Vx = 2;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }
                if(iRightR) {
                    //changeentityproperty(p, "velocity", 0, 0, 0);
                    Vx = 0;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }

                if(iLeft) {
                    //changeentityproperty(p, "velocity", -1, 0, 0);
                    Vx =-1.5;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);

                }
                if(iLeftR) {
                    //changeentityproperty(p, "velocity", 0, 0, 0);
                    Vx = 0;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }
                if(iUp){
                    //changeentityproperty(p, "velocity", 0, -2, 0);
                    Vy = -2;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }
                if(iUpR) {
                    //changeentityproperty(p, "velocity", 0, 0, 0);
                    Vy = 0;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }
                if(iDown){
                    ///changeentityproperty(p, "velocity", 0, 2, 0);
                    Vy = 2;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }
                if(iDownR) {
                    //changeentityproperty(p, "velocity", 0, 0, 0);
                    Vy = 0;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }
                /// do diagnal 
                if(iDown && iRight) {
                    //changeentityproperty(p, "velocity", 1, 0.5, 0);
                    Vx = 2;
                    Vy = 1.5;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }

                if(iDown && iLeft) {
                    //changeentityproperty(p, "velocity", -1, 0.5, 0);
                    Vx = -1.5;
                    Vy = 1.5;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                   
                }

                if(iUp && iRight){
                    //changeentityproperty(p, "velocity", 1, -0.5, 0);
                    Vx = 2;
                    Vy = -1.5;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                    
                }

                if(iUp && iLeft){
                    //changeentityproperty(p, "velocity", -1, -0.5, 0);
                    Vx = -1.5;
                    Vy = -1.5;
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }


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
