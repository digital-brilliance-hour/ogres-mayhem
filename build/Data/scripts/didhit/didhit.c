void main()
{//Used by SOR4 juggle system, who makes all main attacks to have a lower dropv height
	
	//if(getglobalvar("juggleSystem") == "sor4"){
		void self	= getlocalvar("self");
		void target	= getlocalvar("damagetaker");
		void sType	= getentityproperty(target,"subtype");
		int health	= getentityproperty(target,"health");
		int dmg 	= getlocalvar("damage");
		int blocked	= getlocalvar("blocked");
		int drop	= getlocalvar("drop");
		int height	= getentityproperty(target,"y");
		int base	= getentityproperty(target,"base");
		float xVel	= getentityproperty(target,"xdir");
		float xMult	= 4;
		float yVel	= 1;

		if(health >= 1){
			if(dmg > 0){
				if(blocked == 0){
					if(drop == 0){
						if(sType != openborconstant("SUBTYPE_NOTGRAB")){
							if(height <= base){yVel = 0;}
							changeentityproperty(target, "velocity", xVel/xMult, 0, yVel);
						}
					}
				}
			}
		}
	//}
}