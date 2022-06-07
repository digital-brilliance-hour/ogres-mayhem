void main() {
	void self = getlocalvar("self");
	void hp = getentityproperty(self, "health");
	void maxhp = getentityproperty(self, "maxhealth");
	float percentage = (hp/maxhp)*100;
	void animationcount = getlocalvar("animationcount");
	if(hp < 50) {
		if(animationcount < 1) {
			changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW1"));
			animationcount = setlocalvar("animationcount", 1);
		}
	}

}

void oncreate() {
	setlocalvar("animationcount", 0);
}

void ondestroy() {
	setlocalvar("animationcount", NULL());
}