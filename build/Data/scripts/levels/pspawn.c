void main() {
	void p;
	int i, hp, mp, lv, num, stage, maxhp, newhp, score;
	int c_stage = openborvariant("current_stage");
	for(i=0; i<4; i++){
		p = getplayerproperty(i, "entity");
		num = i+1;
		if(p){
			if(c_stage < 10) {
			 	setglobalvar("p"+num+"_spawned", 1);
			 	int  iETime     = openborvariant("elapsed_time");       //Current time.
				changedrawmethod(p, "enabled", 1);
			    changedrawmethod(p, "alpha", 6);
			    changedrawmethod(p, "channelr", 0);
			    changedrawmethod(p, "channelg", 0);
			    changedrawmethod(p, "channelb", 0);
			    changeentityproperty(p, "gfxshadow", 1);
			    changeentityproperty(p, "shadowbase", 0);
			    void subent;
			    loadmodel("ispawn"); // name of the entity to be loaded
			    clearspawnentry(); // clean the spawn entry
			    setspawnentry("name", "ispawn"); // define the entity to be spawn
			    setspawnentry("coords", -1,-1,200); // set the position of the entity
			    subent=spawn();  //  spawn the entity
			    bindentity(subent, p, 0, 0, 0, 0, 0);
			    changeentityproperty(subent, "parent", p); //Set caller as parent.
			    changeentityproperty(subent, "setlayer", 200);
			    changeentityproperty(subent, "position", 0, 0, 200);
		        changeentityproperty(p, "frozen", 1);               //Toggle frozen.
		        changeentityproperty(p, "freezetime", iETime + 400);    //Toggle frozen time.
		    }
		    if(c_stage == 10) {
			 	setglobalvar("p"+num+"_spawned", 1);
			 	int  iETime     = openborvariant("elapsed_time");       //Current time.
				changedrawmethod(p, "enabled", 1);
			    changedrawmethod(p, "alpha", 6);
			    changedrawmethod(p, "channelr", 0);
			    changedrawmethod(p, "channelg", 0);
			    changedrawmethod(p, "channelb", 0);
			    changeentityproperty(p, "gfxshadow", 1);
			    changeentityproperty(p, "shadowbase", 0);
			    void subent;
			    loadmodel("fspawn"); // name of the entity to be loaded
			    clearspawnentry(); // clean the spawn entry
			    setspawnentry("name", "fspawn"); // define the entity to be spawn
			    setspawnentry("coords", -1,-1,200); // set the position of the entity
			    subent=spawn();  //  spawn the entity
			    bindentity(subent, p, 0, 0, 0, 0, 0);
			    changeentityproperty(subent, "parent", p); //Set caller as parent.
			    changeentityproperty(subent, "setlayer", 200);
			    changeentityproperty(subent, "position", 0, 0, 200);
		        changeentityproperty(p, "frozen", 1);               //Toggle frozen.
		        changeentityproperty(p, "freezetime", iETime + 400);    //Toggle frozen time.
		    }
		}
	}
}