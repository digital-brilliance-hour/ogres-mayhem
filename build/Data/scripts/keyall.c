#include "data/scripts/story/story_keys.c"

void main()
{
	storyKeys();

	int iPlIndex = getlocalvar("player"); //Get calling player
	log(iPlIndex);
    void vSelf = getplayerproperty(iPlIndex , "entity"); //Get calling entity
    void vAniID = getentityproperty(vSelf,"animationID"); //Get current animation ID
    void vAniPos = getentityproperty(vSelf, "animpos"); //Get current animation frame
    int iDir = getentityproperty(vSelf, "direction");  //Get current facing direction

    void iUp = playerkeys(iPlIndex, 1, "moveup"); // New key status of "Up"
    void iDown = playerkeys(iPlIndex, 1, "movedown"); // New key status of "Down"
    void iLeft = playerkeys(iPlIndex, 1, "moveleft"); // New key status of "Left"
    void iRight = playerkeys(iPlIndex, 1, "moveright"); // New key status of "Right"
    void iJump = playerkeys(iPlIndex, 1, "jump"); //New key status of "Jump"
    void iSpecial = playerkeys(iPlIndex, 1, "Special"); //New key status of "Special"
    void iAttack = playerkeys(iPlIndex, 1, "attack"); //New key status of "Attack"
    void iAttack2 = playerkeys(iPlIndex, 1, "attack2"); // New key status of "Attack 2"
    void iAttack3 = playerkeys(iPlIndex, 1, "attack3"); // New key status of "Attack 3"
    void iAttack4 = playerkeys(iPlIndex, 1, "attack4"); // New key status of "Attack 4"

    void iLeftR = playerkeys(iPlIndex, 2, "moveleft"); // Release status of "Left"
    void iRightR = playerkeys(iPlIndex, 2, "moveright"); // Release status of "Right"
    void iAttackR = playerkeys(iPlIndex, 2, "attack"); //Release status of "Attack"
    void iAttack2R = playerkeys(iPlIndex, 2, "attack2"); // Release status of "Attack 2"
    void iAttack3R = playerkeys(iPlIndex, 2, "attack3"); // Release status of "Attack 3"
    void iAttack4R = playerkeys(iPlIndex, 2, "attack4"); // Release status of "Attack 4"

    void iDownH = playerkeys(iPlIndex, 0, "movedown");
    void iUpH = playerkeys(iPlIndex, 0, "moveup");
    void iLeftH = playerkeys(iPlIndex, 0, "moveleft");
    void iRightH = playerkeys(iPlIndex, 0, "moveright");
    if(vSelf) {

	    if(getlocalvar("currentPlayerMoveList") != NULL()) {
	    	if(iAttack4R) {
	    		void currPlay = getplayerproperty(getlocalvar("currentPlayerMoveList"), "entity");
	    		if(currPlay == vSelf) {
	    			setlocalvar("currentPlayerMoveList", NULL());
	    			setentityvar(vSelf, "movepressed", NULL());
					setlocalvar("movelistcounter", 0);
	    			changeentityproperty(getentityvar(vSelf, "movelistobj"), "animation", openborconstant("ANI_FOLLOW1"));
				}
	    	}
	    }

	    //Toggle Move List
	    if(iAttack4) {    
	    	if(!getentityvar(vSelf, "movepressed")){
		       void movelistcounter = getlocalvar("movelistcounter");  
		       while(movelistcounter!=1)      {        
		            void subent;   

		            loadmodel("movelist"); // name of the entity to be loaded        
		            clearspawnentry(); // clean the spawn entry        
		             setspawnentry("name", "movelist"); // define the entity to be spawn        
		             setspawnentry("coords", 240, 136, 0); // set the position of the entity       
		             subent=spawn();  //  spawn the entity        
		             setglobalvar("bckground", subent);        
		             changeentityproperty(subent, "position", 240, 136, 0); //for safe, set again the position               
		             setentityvar(vSelf, "movepressed", 1);
		             setentityvar(vSelf, "movelistobj", subent);
		             setlocalvar("currentPlayerMoveList", iPlIndex);
		             movelistcounter = setlocalvar("movelistcounter",1); // turn on the variable, blocking a new spawn to be made      
		       }

	    	}
	        
	    }
	}

	/*if(getglobalvar("bossInterlude")) {
		if(vSelf) {
			changeplayerproperty(iPlIndex, "disablekeys", openborconstant("FLAG_SPECIAL")+openborconstant("FLAG_ATTACK")+openborconstant("FLAG_ATTACK2")+openborconstant("FLAG_ATTACK3")+openborconstant("FLAG_ATTACK4")+openborconstant("FLAG_JUMP")+openborconstant("FLAG_MOVEUP")+openborconstant("FLAG_MOVEDOWN")+openborconstant("FLAG_MOVELEFT")+openborconstant("FLAG_MOVERIGHT"));
		}
	}
	else {
		if(vSelf) {
			changeplayerproperty(iPlIndex, "disablekeys",0);
		}
	}*/

    /*if(getentityvar(vSelf, "movepressed" && iAttack4)) {
    }*/
}

void oncreate() {
	setlocalvar("movelistcounter", 0);
	setlocalvar("currentPlayerMoveList", NULL());
}