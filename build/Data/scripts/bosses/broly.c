void main() {
	void self = getlocalvar("self");
	int hp = getentityproperty(self, "health");
	if(hp < 30) {
		if(getlocalvar("addbreak") == NULL()) {
			changeentityproperty(self, "noaicontrol", 1);
			changeentityproperty(self, "aiflag", "walking", 0);
			changeentityproperty(self, "aiflag", "idling", 0);
			changeentityproperty(self, "aiflag", "attacking", 0);
			executeanimation(self, openborconstant("ANI_FOLLOW1"));
			setlocalvar("addbreak", 1);
		}

		void p;
        int i, hp, mp, lv, num;
        for(i=0; i<4; i++){
            p = getplayerproperty(i, "entity");
            num = i+1;
            if(p){
            	changeentityproperty(p, "mp", 0);
                executeanimation(p, openborconstant("ANI_IDLE"));
            }
        }
	}
}