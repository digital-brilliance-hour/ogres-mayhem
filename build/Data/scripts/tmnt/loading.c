
#import "data/scripts/lib.c"
//#import "data/scripts/var_resetter.c"

// clear_loading_stuffs() è in update.c

void main() {
    int hres = openborvariant("hresolution");
    int vres = openborvariant("vresolution");
	int value = getlocalvar("value");
	int max = getlocalvar("max");
	int ratio = get_ratio(value, max);
	char branch_name = openborvariant("current_branch");
	char text1 = "NOW LOADING...";

	/*if ( getlocalvar("reset_vars") == NULL() ) {
        reset_vars(); // reset vars for next levels
        setlocalvar("reset_vars",1);
	}*/

    //drawstring(10,100,0,"Var(): "+openborvariant("current_branch"));
    if ( value == -1 && max == 1 ) return 0;
    if ( branch_name != NULL() ) {
        if ( strinfirst(branch_name, "text_intro") != -1 || max <= -1 ) return 0;
    }

    //drawstring((hres/2-strwidth(text1, 6)/2)-1,50,6,text1);
    draw_loading_ratio(ratio);
    draw_loading_icon(ratio);
}

/*void ondestroy() {
    if ( getglobalvar("loading_icon") != NULL() ) { free(getglobalvar("loading_icon")); setglobalvar("loading_icon",NULL()); }
}*/

void draw_loading_icon(int ratio) {
    char path = "data/bgs/loading_icon.gif";
    int hres = openborvariant("hresolution");
    int vres = openborvariant("vresolution");
    void icon;
    int hicon, wicon;
    int clipx, clipy;
    int icon_ratio;
    int transp = 120;

    if ( getglobalvar("loading_icon") == NULL() ) {
        void spr = loadsprite(path);

        setglobalvar("loading_icon", spr);
    }

    icon = getglobalvar("loading_icon");
    if ( icon != NULL() ) {
        float min_ratio = 90, max_ratio = 150, diff_ratio; // 90 completo, 150 no (90+60)

        hicon = getgfxproperty(icon, "srcheight");
        wicon = getgfxproperty(icon, "srcwidth");

        diff_ratio = max_ratio-min_ratio;
        icon_ratio = (diff_ratio*ratio)/100;
        icon_ratio %= diff_ratio+1; // trunc
        icon_ratio += min_ratio;
        clipy = icon_ratio;

        // ratio:100=x:256 -> 100x=256*ratio -> x = (256*ratio)/100

        changedrawmethod(NULL(), "enabled", 1);
        changedrawmethod(NULL(), "reset", 1);
        //changedrawmethod(NULL(), "clip", 10, 20);
        changedrawmethod(NULL(), "cliph", 356);
        changedrawmethod(NULL(), "clipw", 256);
        //changedrawmethod(NULL(), "clipx", 256-clipx);
        changedrawmethod(NULL(), "clipy", (max_ratio+min_ratio)-clipy); //256-clipy
        changedrawmethod(NULL(), "alpha", 6);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);

        drawsprite(icon, hres/2-wicon/2, vres/2-hicon/2, 0, 0); // drawsprite(sprite, int x, int y, int z, int sortid)
        //drawstring(160,180,0,icon_ratio);
        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);
    }
}

void draw_loading_ratio(int ratio) {
	//void dot_string;
	//void dot;
    int hres = openborvariant("hresolution");
    int vres = openborvariant("vresolution");
    //int ticks = openborvariant("ticks");
    char text1, text2 = "Loading...";

	/*dot = ratio%3;
	if (dot == 0) dot_string=".";
	else if (dot == 1) dot_string="..";
	else dot_string="...";*/

    //text1 = ratio+"% Complete"+dot_string;
    text1 = ratio+"%";
	drawstring((hres/2-strwidth(text1, 0)/2)-2,(vres/2)-4,0,text1);
	//drawstring((hres/2-strwidth(text2, 0)/2)+2,(vres/2)-50,0,text2);

}

int get_ratio(int value, int max) {
	int ratio;

    if ( max == 1 && getglobalvar("ratio_flag") == NULL() ) {
        value = 0;
    }  else if ( max == 1 && getglobalvar("ratio_flag") != NULL() ) {
        value = 1;
    }

    if ( ratio > 0 && getglobalvar("ratio_flag") == NULL() ) setglobalvar("ratio_flag", ratio);

    // x:100=value:max -> 100*value=x*max -> x=(100*value)/max
    ratio = (100*value)/max;
    ratio %= 100+1; // trunc

	return ratio;
}

// called by update.c
void clear_loading_stuffs() {
    if ( openborvariant("in_level") || getglobalvar("in_menu") ) {
        if ( getglobalvar("ratio_flag") != NULL() ) {
            setglobalvar("ratio_flag", NULL());
        }

        if ( getglobalvar("loading_icon") != NULL() ) {
            free(getglobalvar("loading_icon"));
            setglobalvar("loading_icon", NULL());
        }
    }
}

