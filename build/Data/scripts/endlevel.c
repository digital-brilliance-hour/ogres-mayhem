#include "data/scripts/story/story_clear.c"
void main()
{
	clearStory();
	setglobalvar("zoomentity", NULL());

	void p;
	 int i, hp, maxhp, mp, num;
	 int snum = openborvariant("current_stage");
	 for(i=0; i<4; i++){
		 p = getplayerproperty(i, "entity");
		 hp = getentityproperty(p, "health");
		 maxhp = getentityproperty(p, "maxhealth");
		 num = i+1;
		 if(p){
			setglobalvar("saved_health_"+num+"P", hp);
			if(getentityproperty(p, "maxmp") < 1) {
				setglobalvar("maxmp_"+num+"P", 100);
			}
			else {
				setglobalvar("maxmp_"+num+"P", getentityproperty(p, "maxmp"));
			}
			setglobalvar("offense_"+num+"P", getentityproperty(p,"offense",openborconstant("ATK_NORMAL")));

			setglobalvar("defense_"+num+"P", getentityproperty(p,"defense",openborconstant("ATK_NORMAL")));

			setglobalvar("maxhealth_"+num+"P", getentityproperty(p, "maxhealth"));
			if(snum == 9) {
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
			}

		 }
	 } 
}
