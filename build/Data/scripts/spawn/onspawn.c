void main() {
	void p;
    int i, hp, mp, lv, num;
    for(i=0; i<4; i++){
        p = getplayerproperty(i, "entity");
        num = i+1;
        if(p){
            //setentityvar(subent, "jumpTo", "SIntro");
        }
    }
	void self = getlocalvar("self");
	int pindex = getentityproperty(self, "playerindex");
	num = pindex + 1;
	log("p"+num+"_spawned: " + getglobalvar("p"+num+"_spawned"));
	if(getglobalvar("p"+num+"_spawned") > 0) {                  //Caller   
    	int  iETime     = openborvariant("elapsed_time");       //Current time.
		changedrawmethod(self, "enabled", 1);
	    changedrawmethod(self, "alpha", 6);
	    changedrawmethod(self, "channelr", 0);
	    changedrawmethod(self, "channelg", 0);
	    changedrawmethod(self, "channelb", 0);
	    changeentityproperty(self, "gfxshadow", 1);
	    changeentityproperty(self, "shadowbase", 0);
	    void subent;
	    loadmodel("ispawn"); // name of the entity to be loaded
	    clearspawnentry(); // clean the spawn entry
	    setspawnentry("name", "ispawn"); // define the entity to be spawn
	    setspawnentry("coords", -1,-1,200); // set the position of the entity
	    subent=spawn();  //  spawn the entity
	    bindentity(subent, self, 0, 0, 0, 0, 0);
	    changeentityproperty(subent, "parent", self); //Set caller as parent.
	    changeentityproperty(subent, "setlayer", 200);
	    changeentityproperty(subent, "position", 0, 0, 200);
        changeentityproperty(self, "frozen", 1);               //Toggle frozen.
        changeentityproperty(self, "freezetime", iETime + 400);    //Toggle frozen time.
	    setglobalvar("p"+num+"_spawned", 0);
	}
}