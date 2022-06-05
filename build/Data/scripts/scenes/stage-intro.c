
void main()
{
	int r = rand()%2;
	int ry = rand()%2;
	changelayerproperty("background", 0, "xoffset", r);
	changelayerproperty("background", 0, "zoffset", ry);

	int y = getlocalvar("stageY");
	if(y > 120) {
		y -= 1;
		setlocalvar("stageY", y);
	}
	else {
		y = 120;
	}

	int T = getlocalvar("stageTimer");

	if(T < 6000) {
		T += 1;
		setlocalvar("stageTimer", T);
	}
	else {
		jumptobranch("", 1);
	}

	int snum = openborvariant("current_stage");
	log(snum);

	void StageText = getglobalvar("Stage"+(snum-20)); 
	changedrawmethod(NULL(), "reset", 1);
	drawsprite(StageText, 128, y, 2); 
	void p;
	int i, hp, mp, lv, num;
	for(i=0; i<4; i++){
	    p = getplayerproperty(i, "entity");
	    if(p) {
	    	changedrawmethod(p, "enabled", 1);
	    	changedrawmethod(p, "alpha", 6);
	    	changedrawmethod(p, "channelr", 0);
	    	changedrawmethod(p, "channelg", 0);
	    	changedrawmethod(p, "channelb", 0);

	    }
	}
	if(playerkeys(0, 1, "start") || playerkeys(1, 1, "start") || playerkeys(2, 1, "start") || playerkeys(3, 1, "start")) {
		jumptobranch("", 1);
	}

	drawstring(0360,253,1000,"Press Start To Skip");

}

void oncreate() {
	setlocalvar("stageY", 275);
	setlocalvar("stageTimer", 0);
}
