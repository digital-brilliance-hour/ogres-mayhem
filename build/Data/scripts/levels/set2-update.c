#include "data/scripts/levelup/lvup.c"

void main() {
	log(getsaveinfo(1, "SCORE", 0));
	int checkScore = getglobalvar("checkSet1Score");
	if(checkScore != 1) {
	    void p;
	    int i, hp, mp, lv, num;
	    for(i=0; i<4; i++){
	        p = getplayerproperty(i, "entity");
	        num = i+1;
	        if(p){
	            if("Travel"!=getentityproperty(p, "model")) {
	            	if(scoreExists(i, 1) < 1) {
		            	if(scoreExists(i, 0) < 21000) {
				            changeplayerproperty(i, "score", 21500);
				            lvup(i);

		            	}
			            else {
			            	changeplayerproperty(i, "score", scoreExists(i, 0));
					        lvup(i);

			            }
		            }
	            }
	        }
	    }
	    setglobalvar("checkSet1Score", 1);
	}
}

int scoreExists(int pindex, int sindex) {
	return getsaveinfo(sindex, "SCORE", pindex);
}