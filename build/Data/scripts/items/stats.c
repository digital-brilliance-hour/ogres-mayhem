void main()
{
	void self = getlocalvar("self");
	void player = getlocalvar("damagetaker");
	void subent;
    loadmodel("statsBoost"); // name of the entity to be loaded        
    clearspawnentry(); // clean the spawn entry        
    setspawnentry("name", "statsBoost"); // define the entity to be spawn        
    setspawnentry("coords", -1,-1,-1000); // set the position of the entity       
    subent=spawn();  //  spawn the entity
    bindentity(subent, player, 0, 0, 0, 0, 0);
    changeentityproperty(subent, "parent", player); //Set caller as parent.
}
