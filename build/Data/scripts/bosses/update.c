void main() {
	void self = getlocalvar("self");
	void hp = getentityproperty(self, "health");
	void maxhp = getentityproperty(self, "maxhealth");
	float percentage = (hp/maxhp)*100;
	if(percentage <= 30) {
		changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW1"));
	}

}