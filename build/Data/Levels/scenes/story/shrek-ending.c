#include "data/scripts/story/story_scroll.c"

void main()
{	
	scrollText("shrek-ending.txt",71,0.3,"back1",0.09);
	drawstring(0360,253,1000,"Press Start To Skip");
  
	float y = getlayerproperty("background", 0, "zoffset");
	int ypos = getlocalvar("stagey");

	void p;
    int i, hp, mp, lv, num;
    for(i=0; i<4; i++){
        p = getplayerproperty(i, "entity");
        num = i+1;
        if(p){
       		changeentityproperty(p, "gfxshadow", 1);
       		changeentityproperty(p, "mp", 0);
            executeanimation(p, openborconstant("ANI_IDLE")); 
		    changedrawmethod(p, "enabled", 1);
		    changedrawmethod(p, "alpha", 6);  
		    changedrawmethod(p, "channelb", 0);
		    changedrawmethod(p, "channelg", 0);
		    changedrawmethod(p, "channelr", 0);
        }
    }
	log(ypos);
	log(" - ypos. ");
	log(y);
	log(" - y. ");
	if(ypos > -47) {
		y -= 1;
		ypos -= 1;
		setlocalvar("stagey", ypos);
	}
	changelayerproperty("background", 0, "zratio", 0.1);
	changelayerproperty("background", 0, "zoffset", y); 
}

void oncreate() {
	setlocalvar("stagey", 0);
	setlocalvar("opacitynum", 0);
	//changedrawmethod(NULL(), "reset", 1);
}
