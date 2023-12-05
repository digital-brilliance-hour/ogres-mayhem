#include "data/scripts/story/story_clear.c"
void main()
{
	void self = getlocalvar("self");
	void p;
	 int i, hp, maxhp, mp, num;
	 int snum = openborvariant("current_stage");
	 int waiting = openborvariant("waiting");
	 int av_enemies = openborvariant("count_enemies");
	 log("count_enemies: " + av_enemies);
	if(getlocalvar("addbreak") == NULL()) {

		changeentityproperty(self, "noaicontrol", 1);
		changeentityproperty(self, "aiflag", "walking", 0);
		//changeentityproperty(self, "aiflag", "idling", 0);
		changeentityproperty(self, "aiflag", "attacking", 0);
		setlocalvar("addbreak", 1);
	}
	 for(i=0; i<4; i++){
		 p = getplayerproperty(i, "entity");
		 if(p){
			if(snum == 9 && av_enemies == 1 && getlocalvar("addout"+i) == NULL()) {
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
			 	int  iETime     = openborvariant("elapsed_time");       //Current time.
		        changeentityproperty(p, "frozen", 1);               //Toggle frozen.
		        changeentityproperty(p, "freezetime", iETime + 400);    //Toggle frozen time.
                setentityvar(subent, "jumpTo", "");

				setlocalvar("addout"+i, 1);
			}

		 }
	 } 
}
