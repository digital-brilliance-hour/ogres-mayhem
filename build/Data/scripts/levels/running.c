void main() {

    log("Current X value "+openborvariant("xpos")+".  ");
    int lwidth = openborvariant("levelwidth");
    int lpos = openborvariant("xpos");
    void p;
    int i, hp, mp, lv, num;
    for(i=0; i<4; i++){
        p = getplayerproperty(i, "entity");
        num = i+1;
        if(p){
            if("Travel"!=getentityproperty(p, "model")) {
                int xpos = getentityproperty(p, "x");
                int plives = getplayerproperty(i, "lives");
                changeentityproperty(p, "mp", 0);
                if(xpos >= lwidth-120) {
                    void vEntity;                                       //Target entity placeholder.
                    int  iEntity; 
                    char iName;                                         //Entity Name.
                    int  iMax      = openborvariant("count_entities");  //Entity count.

                    //Enumerate and loop through entity collection.
                    for(iEntity=0; iEntity<iMax; iEntity++){
                      vEntity = getentity(iEntity);                 //Get target entity from current loop.
                      iName   = getentityproperty(vEntity, "name"); //Get target name.
                      
                      if(iName == "bLargeFireball"){ //Same alias?
                        changeentityproperty(vEntity, "candamage", openborconstant("TYPE_ENEMY"));
                      }
                    }
                    executeanimation(p, openborconstant("ANI_IDLE"));
                    //jumptobranch("", 1);
                    if(getlocalvar("removing"+i) == NULL()) {
                        void subent;
                        loadmodel("pspawn"); // name of the entity to be loaded
                        clearspawnentry(); // clean the spawn entry
                        setspawnentry("name", "pspawn"); // define the entity to be spawn
                        setspawnentry("coords", -1,-1,200); // set the position of the entity
                        subent=spawn();  //  spawn the entity
                        bindentity(subent, p, 0, 0, 0, 0, 0);
                        changeentityproperty(subent, "parent", p); //Set caller as parent.
                        changeentityproperty(subent, "setlayer", 200);
                        changeentityproperty(subent, "position", 0, 0, 200);
                        setentityvar(subent, "jumpTo", "Ending");
                        setlocalvar("removing"+i, subent);
                    }
                }
                hp = getentityproperty(p, "health");
                if(getlocalvar("removing"+i) == NULL()) {
                    if(hp > 0) {
                        executeanimation(p, openborconstant("ANI_RUN"));
                    }
                    else {
                        if(getlocalvar("player"+i+"die") == NULL()) {
                            if(plives > 1) {
                                executeanimation(p, openborconstant("ANI_FALL"));
                                changeplayerproperty(i, "lives", plives-1);
                                killentity(p);
                                jumptobranch(openborvariant("current_branch"), 1);
                                setlocalvar("player"+i+"die", 1);
                            }
                            else {
                                executeanimation(p, openborconstant("ANI_FALL"));
                                killentity(p);
                                gameover();
                                setlocalvar("player"+i+"die", 1);
                            }
                        }
                    }
                }
                else {
                    executeanimation(p, openborconstant("ANI_SPAWN"));
                }
                void iUp = playerkeys(i, 0, "moveup"); // New key status of "Up"
                void iDown = playerkeys(i, 0, "movedown"); // New key status of "Down"
                void iLeft = playerkeys(i, 0, "moveleft"); // New key status of "Left"
                void iRight = playerkeys(i, 0, "moveright"); // New key status of "Right"
                void iUpR = playerkeys(i, 2, "moveup"); // New key status of "Up"
                void iDownR = playerkeys(i, 2, "movedown"); // New key status of "Down"
                void iLeftR = playerkeys(i, 2, "moveleft"); // New key status of "Left"
                void iRightR = playerkeys(i, 2, "moveright"); // New key status of "Right"
                int Vxbase = 8;
                int Vxback = 4;
                int Vx, Vy, Vz;
                Vx = Vxbase;
                Vy = 0;
                Vz = 0;


                /*if(iRight) {
                    //changeentityproperty(p, "velocity", 1, 0, 0);
                    Vx += 4;
                }
                if(iRightR) {
                    //changeentityproperty(p, "velocity", 0, 0, 0);
                    Vx = Vxbase;
                }

                if(iLeft) {
                    //changeentityproperty(p, "velocity", -1, 0, 0);
                    Vx = Vxback;

                }
                if(iLeftR) {
                    //changeentityproperty(p, "velocity", 0, 0, 0);
                    Vx = Vxbase;
                }*/
                if(iUp){
                    //changeentityproperty(p, "velocity", 0, -2, 0);
                    Vy = -2;
                }
                if(iUpR) {
                    //changeentityproperty(p, "velocity", 0, 0, 0);
                    Vy = 0;
                }
                if(iDown){
                    ///changeentityproperty(p, "velocity", 0, 2, 0);
                    Vy = 2;
                }
                if(iDownR) {
                    //changeentityproperty(p, "velocity", 0, 0, 0);
                    Vy = 0;
                }
                /// do diagnal 
                /*if(iDown && iRight) {
                    //changeentityproperty(p, "velocity", 1, 0.5, 0);
                    Vx += 4;
                    Vy = 1.5;
                }

                if(iDown && iLeft) {
                    //changeentityproperty(p, "velocity", -1, 0.5, 0);
                    Vx = Vxback;
                    Vy = 1.5;
                   
                }

                if(iUp && iRight){
                    //changeentityproperty(p, "velocity", 1, -0.5, 0);
                    Vx += 4;
                    Vy = -1.5;
                    
                }

                if(iUp && iLeft){
                    //changeentityproperty(p, "velocity", -1, -0.5, 0);
                    Vx = Vxback;
                    Vy = -1.5;
                }*/
                if(getlocalvar("removing"+i) == NULL()) {
                    changeentityproperty(p, "velocity", Vx, Vy, Vz);
                }
                else {
                    changeentityproperty(p, "velocity", 0, 0, 0);
                    changeentityproperty(p, "speed", 0, 0, 0);
                }


            }
        }
    }

}
