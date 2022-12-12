void main()
{
	void self	= getlocalvar("self");
	int drop	= getentityproperty(self, "aiflag", "drop");
	int Vx = getentityproperty(self,"xdir");
    int Vy = getentityproperty(self,"tossv");
    int Vz = getentityproperty(self,"zdir");

	if(drop == 1){
		changeentityproperty(self, "velocity", -Vx*0.5, -Vz*0.5, Vy+1);
		//changeentityproperty(self, "velocity", -2, -Vz*0.5, 2);
	}
}
