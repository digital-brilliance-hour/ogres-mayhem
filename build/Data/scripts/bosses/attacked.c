void main(){
	void self = getlocalvar("self");
	void hp = getentityproperty(self, "health");
	void maxhp = getentityproperty(self, "maxhealth");
	log(maxhp + ": Max");
	float percentage = maxhp*0.3;
	log(hp + ": HP");
	if(hp <= percentage){
			log("ready to transform");
			executeanimation(self, openborconstant("ANI_FOLLOW1"));
	}
}