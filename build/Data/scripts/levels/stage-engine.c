void main() {
	
    void p;
    int i, hp, mp, lv, num;
    for(i=0; i<4; i++){
        p = getplayerproperty(i, "entity");
        if(p){
            if(getglobalvar("bossInterlude")) {
                if(openborvariant("game_paused") || openborvariant("textbox")) {
                    changeplayerproperty(i, "disablekeys", 0);
                }
                else {
                    changeplayerproperty(i, "disablekeys", openborconstant("FLAG_SPECIAL")+openborconstant("FLAG_ATTACK")+openborconstant("FLAG_ATTACK2")+openborconstant("FLAG_ATTACK3")+openborconstant("FLAG_ATTACK4")+openborconstant("FLAG_JUMP")+openborconstant("FLAG_MOVEUP")+openborconstant("FLAG_MOVEDOWN")+openborconstant("FLAG_MOVELEFT")+openborconstant("FLAG_MOVERIGHT"));
                }

                int pX, pY = 0; 
                if(getentityproperty(p, "x") < getglobalvar("bossStopX")) {
                    pX = 1;
                    void walkcounter = getlocalvar("walkcounter"+i);
                    while(walkcounter != 1) {
                    executeanimation(p, openborconstant("ANI_WALK"));
                        //setidle(p, openborconstant("ANI_WALK"));
                        log("walked");
                        walkcounter = setlocalvar("walkcounter"+i, 1);
                    }
                }
                /*else if(getentityproperty(p, "x") > getglobalvar("bossStopX")) {
                    pX = -1;
                    changeentityproperty(p, "direction", 0);
                }*/
                else {
                    pX = 0;
                    //changeentityproperty(p, "direction", 1);
                    changeentityproperty(p, "x", getglobalvar("bossStopX"));
                    void idlecounter = getlocalvar("idlecounter"+i);
                    while(idlecounter != 1) {
                        setidle(p,  openborconstant("ANI_IDLE"));
                        idlecounter = setlocalvar("idlecounter"+i, 1);
                    }
                    scrollcam(120, 0, 2);
                }

                if(getlevelproperty("cameraxoffset") == 120) {
                    int holder = getlocalvar("holdcounter");
                    if(holder < 220) {
                        holder += 1;
                        setlocalvar("holdcounter", holder);
                    }
                    if(holder == 220) {
                        if(getglobalvar("bossDialog") != "") {
                            void dialogcounter = getlocalvar("dialogcounter");  
                            while(dialogcounter!=1)      {        
                                void subent; 
                                loadmodel("story"); // name of the entity to be loaded        
                                clearspawnentry(); // clean the spawn entry        
                                setspawnentry("name", "story"); // define the entity to be spawn        
                                subent=spawn();  //  spawn the entity   
                                changeentityproperty(subent, "name", getglobalvar("bossDialog"));    
                                setglobalvar("bossInterlude", 0); 
                                dialogcounter = setlocalvar("dialogcounter",1); // turn on the variable, blocking a new spawn to be made      
                            }
                        }
                    }
                }

                /*if(getentityproperty(p, "z") < getglobalvar("bossStopY"+i)) {
                    pY = 1;
                }
                else if(getentityproperty(p, "z") > getglobalvar("bossStopY"+i)) {
                    pY = -1;
                }
                else {
                    pY = 0;
                    changeentityproperty(p, "z", getglobalvar("bossStopY"+i));
                }*/

                log(getentityproperty(p, "x") + ": X; ");
                //log(getlevelproperty("basemap", 0, "x"));
                /*log(getentityproperty(p, "z") + ": Y; ");
                log(getglobalvar("bossStopX"));*/

                changeentityproperty(p, "velocity", pX, pY, 0);
                //changeentityproperty(p, "position", getglobalvar("bossStopX"), getglobalvar("bossStopY"+i), 0);
            } 
            else {
                setlocalvar("dialogcounter", 0); 
                setlocalvar("walkcounter"+i, 0);
                setlocalvar("idlecounter"+i, 0);
                changeplayerproperty(i, "disablekeys", 0);
                int holder = getlocalvar("holdcounter");
                if(holder == 220 && getlevelproperty("cameraxoffset") == 120) {
                    int  iEntity;
                    void vEntity;
                    int  iMax        = openborvariant("ent_max");
                               
                    for(iEntity=0; iEntity<iMax; iEntity++)
                    {       
                        vEntity = getentity(iEntity);
                        if(getentityproperty(vEntity, "type")==openborconstant("TYPE_enemy"))
                        {       
                            log(getentityproperty(vEntity, "boss"));
                            if(getentityproperty(vEntity, "boss")) {
                                log("I made it");
                                changeentityproperty(vEntity, "noaicontrol", 0);
                                changeentityproperty(vEntity, "subject_to_screen", 1);
                                changeentityproperty(vEntity, "aiflag", "walking", 1);
                                changeentityproperty(vEntity, "aiflag", "idling", 1);
                                changeentityproperty(vEntity, "aiflag", "attacking", 1);
                                changeentityproperty(vEntity, "destx", getentityproperty(p, "x")+10);
                                changeentityproperty(vEntity, "destz", getentityproperty(p, "z"));
                                changeentityproperty(vEntity, "takeaction", "common_attack_proc");
                                setlocalvar("holdcounter", 0);
                                //executeanimation(vEntity, openborconstant("ANI_RUN"));
                            }   
                        }
                    } 
                }  
            }
        }
    }
}

void oncreate() {
    setglobalvar("bossStopX", 0);
    setglobalvar("bossStopY0", 0);
    setglobalvar("bossStopY1", 0);
    setglobalvar("bossStopY2", 0);
    setglobalvar("bossStopY3", 0);
    setlocalvar("dialogcounter", 0);
    setlocalvar("holdcounter", 0);
    setlocalvar("walkcounter0", 0);
    setlocalvar("idlecounter0", 0);
    setlocalvar("walkcounter1", 0);
    setlocalvar("idlecounter1", 0);
    setlocalvar("walkcounter2", 0);
    setlocalvar("idlecounter2", 0);
    setlocalvar("walkcounter3", 0);
    setlocalvar("idlecounter3", 0);
}

void ondestroy() {
    setglobalvar("bossStopX", NULL());
    setglobalvar("bossStopY0", NULL());
    setglobalvar("bossStopY1", NULL());
    setglobalvar("bossStopY2", NULL());
    setglobalvar("bossStopY3", NULL());
    setlocalvar("dialogcounter", NULL());
    setlocalvar("holdcounter", NULL());
    setlocalvar("walkcounter0", NULL());
    setlocalvar("idlecounter0", NULL());
    setlocalvar("walkcounter1", NULL());
    setlocalvar("idlecounter1", NULL());
    setlocalvar("walkcounter2", NULL());
    setlocalvar("idlecounter2", NULL());
    setlocalvar("walkcounter3", NULL());
    setlocalvar("idlecounter3", NULL());

}

void scrollcam(float xoff, float zoff, float spd) {
  if( openborvariant("in_level") ) {
    changelevelproperty("cameraxoffset", xoff); //X offset for camera
    changelevelproperty("camerazoffset", zoff); //Z offset for camera
    changelevelproperty("scrollspeed", spd); //Scrolling camera speed
  }
}