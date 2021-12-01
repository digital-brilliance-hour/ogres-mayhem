
void main() {
	void vSelf      = getlocalvar("self");  
	void vParent = getentityproperty(vSelf, "parent");
	int damage = getlocalvar("damage"); 
	if(vParent) {
		void p;
	    int i, hp, mp, lv, num;
	    for(i=0; i<4; i++){
	        p = getplayerproperty(i, "entity");
	        num = i+1;
	        if(p){
	            if("Travel"!=getentityproperty(p, "model")) {
		           if(p == vParent) {
		           	 int curScore = getplayerproperty(i, "score");
		           	 int newScore = curScore + (damage*5);
		           	 changeplayerproperty(i, "score", newScore);
		           }
	            }
	        }
	    }
	}
}