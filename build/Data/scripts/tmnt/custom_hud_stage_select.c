
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/custom_hud_player_select.c"
//#import "data/scripts/custom_hud_versus.c"
#import "data/scripts/custom_hud_commons.c"
#import "data/chars/misc/help/press_any_button.c"

#define BGLAYERS_NUM get_bglayer_num()
#define DELTA_SPEED 60.0
#define DELTA_MIN 0.00001

void stageSelect(int select_type, int invert_flag) {
    if ( select_type <= 0 ) chooseStage1();
    else if ( select_type == 1 ) chooseStage2(invert_flag);
    else if ( select_type == 2 ) chooseStage3();
    else chooseStage4();
}

void get_stage_item(){
    if ( ggv("1VS1") ) return glv("1VS1_stageitem");
    else return glv("stageitem");
}

void get_prev_item() {
    if ( ggv("1VS1") ) return glv("1VS1_stageprev");
    else return glv("stageprev");
}

// motionless pics
void chooseStage1() {
  void ent;
  int i;
  char text  = "STAGE SELECT";
  int selector = glv("mselector");
  void items = get_stage_item();
  void prevs = get_prev_item();
  int se = loadsample("data/sounds/beep2.wav");
  int se2 = loadsample("data/sounds/beep.wav");
  int se3 = loadsample("data/sounds/cancel01.wav");
  int vres = openborvariant("vresolution");
  int hres = openborvariant("hresolution");
  void title_bg = glv("title_bg");
  float size = 1;
  float arrow_dx_shiftx = 0, arrow_sx_rx = 0;

  int time = ov("elapsed_time");
  int game_speed = ov("game_speed");
  int waiting_time = game_speed*0.0;
  int lighting_time = game_speed*0.95;
  int refresh_time = time%(lighting_time + (waiting_time*1.0));
  int tot_color_changes = 12 - 1;
  int black = rgbcolor(0x00,0x00,0x00);
  int white = rgbcolor(0xFF,0xFF,0xFF);
  int color0 = rgbcolor(0xFF,0x84,0x00); // dark
  int color1 = rgbcolor(0xFF,0x95,0x23);
  int color2 = rgbcolor(0xFF,0xA0,0x3B);
  int color3 = rgbcolor(0xFF,0xAA,0x50);
  int color4 = rgbcolor(0xFF,0xB3,0x62);
  int color5 = rgbcolor(0xFF,0xB9,0x6D); // light
  int color6 = rgbcolor(0xFF,0xC6,0x8A);

  draw_blackscreen();

  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "enabled", 1);
  changedrawmethod(NULL(), "tintmode", 4);
       if ( refresh_time <= (lighting_time/tot_color_changes)*0  ) changedrawmethod(NULL(), "tintcolor", color0);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*1  ) changedrawmethod(NULL(), "tintcolor", color1);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*2  ) changedrawmethod(NULL(), "tintcolor", color2);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*3  ) changedrawmethod(NULL(), "tintcolor", color3);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*4  ) changedrawmethod(NULL(), "tintcolor", color4);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*5  ) changedrawmethod(NULL(), "tintcolor", color5);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*6  ) changedrawmethod(NULL(), "tintcolor", color6);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*7  ) changedrawmethod(NULL(), "tintcolor", color5);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*8  ) changedrawmethod(NULL(), "tintcolor", color4);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*9  ) changedrawmethod(NULL(), "tintcolor", color3);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*10 ) changedrawmethod(NULL(), "tintcolor", color2);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*11 ) changedrawmethod(NULL(), "tintcolor", color1);

  if ( glv("pic_stage") == NULL() ) slv("pic_stage",loadsprite("data/sprites/pic_stage.gif"));
  drawsprite(glv("pic_stage"), 0, -24, 2000);
  sprite_swing_horizontal("pic_stage_arrow_dx","data/sprites/pic_stage_arrow.gif",251,102,0,2001,0.5,0.1,0.5,0.02,1,0);

  // CREATE ARROW MIRRORING
  changedrawmethod(NULL(), "flipx", 1);
  //sprite_swing_horizontal("pic_stage_arrow_sx","data/sprites/pic_stage_arrow.gif", 69,100,0,2001,0.5,0.1,0.5,0.02,0,0);
  if ( glv("pic_stage_arrow_sx") == NULL() ) slv("pic_stage_arrow_sx",loadsprite("data/sprites/pic_stage_arrow.gif"));
  arrow_sx_rx = 69.5;
  arrow_dx_shiftx = get_swing_shift("pic_stage_arrow_dx")-251;
  arrow_sx_rx -= arrow_dx_shiftx;
  drawsprite(glv("pic_stage_arrow_sx"),arrow_sx_rx,102,2001,2001);
  //drawstring(10,20,0,arrow_sx_rx);

  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "enabled", 0);

	//drawRotateBg();
	if (!title_bg) slv("title_bg", spawnsubentity("title_bg", 0,0,0));

    drawstring(hres/2-3-strwidth(text, 0)/2, 10, 6, text);

	/*for(i=0; i<size(items); i++) {
		drawstring(160-strwidth(get(items,i),(selector==i))/2, 160+i*12, (selector==i),get(items,i));
	}*/
    drawstring(hres/2-strwidth(get(items,selector),0)/2,198,0,get(items,selector));

    size = 0.939;
    changedrawmethod(NULL(), "reset", 1);
    changedrawmethod(NULL(), "scalex", 256*size); // da 256 in giù (150)
    changedrawmethod(NULL(), "scaley", 256*size);

    drawsprite(get(prevs, selector),hres/2-160/2+5,72,2);


	if ( !get_blackscreen() ) {
        set_blackscreen(1);
        return;
	}

	if( key_coin("moveup") || key_coin("moveleft") ) {
		if(--selector<0) {
			selector = size(items)-1;
		}
		playsample(se2);
	} else if( key_coin("movedown") || key_coin("moveright") ) {
		if(++selector>=size(items)) {
			selector = 0;
		}
		playsample(se2);
	} else if(key_coin("start") || key_coin("attack")) {
		from_stage_to_player_select();
	} else if(key_coin("esc") || key_coin("special")) {
	    if ( ggv("1VS1") && glv("mode") != "1VS1_stage" ) from_stage_to_vs_winner_summary();
	    else from_stage_to_mode_select();
	}
	slv("mselector", selector);
}

// ellipse pics
void chooseStage2(int counterclockwise) {
  int time = openborvariant("elapsed_time");
  int game_speed = openborvariant("game_speed");
  int lost_frame = get_lost_frames()+1;
  int vres = openborvariant("vresolution");
  int hres = openborvariant("hresolution");
  int i;
  void ent;
  float teta,x,y,x_shift = hres/2, y_shift = 107, size, size2;
  int selector = glv("mselector");
  void items = get_stage_item();
  void prevs = get_prev_item();
  int se = loadsample("data/sounds/beep2.wav");
  int se2 = loadsample("data/sounds/beep.wav");
  int se3 = loadsample("data/sounds/cancel01.wav");
  float da = get_delta(0); //, t_da = 0.8, t_da_min = 0.1;
  float da_speed = 2.00, da_smooth = 0.92; // 0.92
  int NUM_SETS = size(get_stage_item());
  float arc = 360.0/NUM_SETS;
  float radius = 90.0, rad_div_x = 1.0, rad_div_y = 2.0, init_degree = 270.0;
  float size_max = 0.94, size_min = 0.35;
  int invert_fact = 1;
  void title_bg = glv("title_bg");
  char text  = "STAGE SELECT";

  int time = ov("elapsed_time");
  int game_speed = ov("game_speed");
  int waiting_time = game_speed*0.0;
  int lighting_time = game_speed*0.95;
  int refresh_time = time%(lighting_time + (waiting_time*1.0));
  int tot_color_changes = 12 - 1;
  int black = rgbcolor(0x00,0x00,0x00);
  int white = rgbcolor(0xFF,0xFF,0xFF);
  int color0 = rgbcolor(0xFF,0x84,0x00); // dark
  int color1 = rgbcolor(0xFF,0x95,0x23);
  int color2 = rgbcolor(0xFF,0xA0,0x3B);
  int color3 = rgbcolor(0xFF,0xAA,0x50);
  int color4 = rgbcolor(0xFF,0xB3,0x62);
  int color5 = rgbcolor(0xFF,0xB9,0x6D); // light
  int color6 = rgbcolor(0xFF,0xC6,0x8A);

  if ( counterclockwise != NULL() && counterclockwise > 0 ) invert_fact = -1;

  draw_blackscreen();

  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "enabled", 1);
  changedrawmethod(NULL(), "tintmode", 4);
       if ( refresh_time <= (lighting_time/tot_color_changes)*0  ) changedrawmethod(NULL(), "tintcolor", color0);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*1  ) changedrawmethod(NULL(), "tintcolor", color1);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*2  ) changedrawmethod(NULL(), "tintcolor", color2);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*3  ) changedrawmethod(NULL(), "tintcolor", color3);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*4  ) changedrawmethod(NULL(), "tintcolor", color4);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*5  ) changedrawmethod(NULL(), "tintcolor", color5);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*6  ) changedrawmethod(NULL(), "tintcolor", color6);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*7  ) changedrawmethod(NULL(), "tintcolor", color5);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*8  ) changedrawmethod(NULL(), "tintcolor", color4);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*9  ) changedrawmethod(NULL(), "tintcolor", color3);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*10 ) changedrawmethod(NULL(), "tintcolor", color2);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*11 ) changedrawmethod(NULL(), "tintcolor", color1);
  if ( glv("pic_stage") == NULL() ) slv("pic_stage",loadsprite("data/sprites/pic_stage.gif"));
  drawsprite(glv("pic_stage"), 0, 0, 2000);
  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "enabled", 0);

  if (!title_bg) slv("title_bg", spawnsubentity("title_bg", 0,0,0));
  // It has 5 items so 360/5=72. The variable da is used to store rotate animation angle.
  // The last line is used to calculate x position, x=radius*cos(a), y=radius*sin(a), plus screen center position. Those + - are usually used to determine clockwise or counter-clockwise layout.

  /// ######### SMOOTH CODE (BEST) #########
  dec_delta(DELTA_SPEED,0);
  da = get_delta(0);
  /// ##################################################

  //drawRotateBg();
  //clp("bglayer", 2, "bgspeedratio", 2);

  //drawOffsetShiftBg();

  //drawstring(10, 100, 0, "delta a: "+da);
  drawstring(hres/2-3-strwidth(text, 0)/2, 10, 6, text);
  drawstring(x_shift-strwidth(get(items,selector),1)/2, 220,0,get(items,selector));
  //drawstring(10, 100, 0, ggv("test"));

  for(i = 0; i < NUM_SETS; i++) { // ############### 3 è il numero di sets (arcade ecc..)
    float degree, coord_x, coord_y, dist;

    if ( da >= 0 ) degree = init_degree;
    else degree = sum_cyclic_sign(init_degree,-360,-359,359);

    teta = invert_fact*arc*(i-selector)+degree+da*invert_fact;
    //if ( teta < 0 ) teta = sum_cyclic_sign(teta,360,-359,359);
    //drawstring(10, 40+i*10, 0, "var: "+teta);
    //drawstring(10, 50, 0, "degree: "+degree);
    //if ( i == selector ) drawstring(10, 40, 0, "teta: "+teta);
    //if ( i == selector ) drawstring(10, 50, 0, "da: "+da);
    //drawstring(10, 60+i*10, 0, "da: "+da);

    coord_x = radius/rad_div_x * cos(teta);
    coord_y = radius/rad_div_y * sin(teta);
    x = x_shift - coord_x;
    y = y_shift - coord_y;

    /*
    Se si vuole simulare un fade dal quadro più in evidenza a quello meno in evidenza:
    il quadro più in evidenza è dato da init_degree. sottraendo o sommando ciclicamente 180 si ottiene quello meno in evidenza. fare dunque una proporzione.

    calcolo della size: x:1.0=coord_y:radius/rad_div_y -> x = (1.0*coord_y)/(radius/rad_div_y)
    se vogliamo inserire un valore minimo di x: x+min = ((max-min)*coord_y)/(radius/rad_div_y)
    tenere conto che coord_y va da -1 a 1 quindi coord_y massimo diventa 2 e non 1!!

    in questo caso il max è 0.93650 e il min è 0.38580

    perchè poi fare size = max-size? perchè in realtà più si va in basso e più la distanza aumenta. quindi bisogna invertire il tutto
    */
    dist = coord_y + (radius/rad_div_y);
    size = ((size_max-size_min)*dist)/((radius/rad_div_y)*2.0);
    size += size_min;
    size = (size_max+size_min)-size; // inverted
    //drawstring(10, 100+i*10, 0, "coord_y: "+coord_y);

    if ( i == selector ) size2 = 1.0;
    else size2 = size;
    //if ( i == selector ) drawstring(10, 50, 0, "size: "+size);
    //drawstring(10, 60+i*10, 0, "var: "+size);

    changedrawmethod(NULL(), "reset", 1);
    changedrawmethod(NULL(), "scalex", 256*size); // da 256 in giù (150)
    changedrawmethod(NULL(), "scaley", 256*size);
    changedrawmethod(NULL(), "alpha", 6*(size2!=1)); // quando è grande al max si setta a 0
    changedrawmethod(NULL(), "channelr", 255*size2); // size*255
    changedrawmethod(NULL(), "channelg", 255*size2);
    changedrawmethod(NULL(), "channelb", 255*size2);
    changedrawmethod(NULL(), "centerx", 160/2.0);
    changedrawmethod(NULL(), "centery", 120/2.0);

    drawsprite(get(prevs, i), x, y, y + 1000);
  }

	if ( !get_blackscreen() ) {
        set_blackscreen(1);
        return;
	}

	if(key_coin("moveleft")) {
		if(--selector<0) {
			selector = size(items)-1;
		}
        set_delta(-1*arc);
        set_delta_time(time);
        reset_lost_frames();
		playsample(se2);
	} else if(key_coin("moveright")) {
		if(++selector>=size(items)) {
			selector = 0;
		}
        set_delta(arc);
        set_delta_time(time);
        reset_lost_frames();
		playsample(se2);
	} else if(key_coin("start") || key_coin("attack")) {
		from_stage_to_player_select();
	} else if(key_coin("esc") || key_coin("special")) {
	    if ( ggv("1VS1") && glv("mode") != "1VS1_stage" ) from_stage_to_vs_winner_summary();
	    else from_stage_to_mode_select();
	}
	slv("mselector", selector);
}

// in row pics
void chooseStage3() {
  int time = openborvariant("elapsed_time");
  int game_speed = openborvariant("game_speed");
  int lost_frame = get_lost_frames()+1;
  int i,p;
  void ent;
  float a,x,y,w,size,size2;
  int selector = glv("mselector");
  void items = get_stage_item();
  void prevs = get_prev_item();
  int se = loadsample("data/sounds/beep2.wav");
  int se2 = loadsample("data/sounds/beep.wav");
  int se3 = loadsample("data/sounds/cancel01.wav");
  int vres = openborvariant("vresolution");
  int hres = openborvariant("hresolution");
  float da = get_delta(DELTA_MIN); //, t_da = 0.8, t_da_min = 0.1;
  int NUM_SETS = size(get_stage_item());
  float max_dist = 120;
  float x_shift = hres/2, y_shift = 151;
  float size_max = 0.94, size_min = 0.25;
  float dist = max_dist/NUM_SETS; // distanza da ricoprire == 120
  void title_bg = glv("title_bg");
  char text  = "STAGE SELECT";

  int time = ov("elapsed_time");
  int game_speed = ov("game_speed");
  int waiting_time = game_speed*0.0;
  int lighting_time = game_speed*0.95;
  int refresh_time = time%(lighting_time + (waiting_time*1.0));
  int tot_color_changes = 12 - 1;
  int black = rgbcolor(0x00,0x00,0x00);
  int white = rgbcolor(0xFF,0xFF,0xFF);
  int color0 = rgbcolor(0xFF,0x84,0x00); // dark
  int color1 = rgbcolor(0xFF,0x95,0x23);
  int color2 = rgbcolor(0xFF,0xA0,0x3B);
  int color3 = rgbcolor(0xFF,0xAA,0x50);
  int color4 = rgbcolor(0xFF,0xB3,0x62);
  int color5 = rgbcolor(0xFF,0xB9,0x6D); // light
  int color6 = rgbcolor(0xFF,0xC6,0x8A);

  draw_blackscreen();

  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "enabled", 1);
  changedrawmethod(NULL(), "tintmode", 4);
       if ( refresh_time <= (lighting_time/tot_color_changes)*0  ) changedrawmethod(NULL(), "tintcolor", color0);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*1  ) changedrawmethod(NULL(), "tintcolor", color1);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*2  ) changedrawmethod(NULL(), "tintcolor", color2);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*3  ) changedrawmethod(NULL(), "tintcolor", color3);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*4  ) changedrawmethod(NULL(), "tintcolor", color4);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*5  ) changedrawmethod(NULL(), "tintcolor", color5);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*6  ) changedrawmethod(NULL(), "tintcolor", color6);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*7  ) changedrawmethod(NULL(), "tintcolor", color5);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*8  ) changedrawmethod(NULL(), "tintcolor", color4);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*9  ) changedrawmethod(NULL(), "tintcolor", color3);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*10 ) changedrawmethod(NULL(), "tintcolor", color2);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*11 ) changedrawmethod(NULL(), "tintcolor", color1);
  if ( glv("pic_stage") == NULL() ) slv("pic_stage",loadsprite("data/sprites/pic_stage.gif"));
  drawsprite(glv("pic_stage"), 0, -1, 2000);
  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "enabled", 0);

  if (!title_bg) slv("title_bg", spawnsubentity("title_bg", 0,0,0));
  // It has 5 items so 360/5=72. The variable da is used to store rotate animation angle.
  // The last line is used to calculate x position, x=radius*cos(a), y=radius*sin(a), plus screen center position. Those + - are usually used to determine clockwise or counter-clockwise layout.

  /// ######### SMOOTH CODE (BEST) #########
  dec_delta(DELTA_SPEED,DELTA_MIN);
  da = get_delta(DELTA_MIN);
  /// ##################################################

  //drawRotateBg();
  //clp("bglayer", 2, "bgspeedratio", 2);

  //drawOffsetShiftBg();

  drawstring(hres/2-3-strwidth(text, 0)/2, 10, 6, text);
  drawstring(160-strwidth(get(items,selector),1)/2, 220,0,get(items,selector));
  //for(i=0;i<NUM_SETS;++i) drawstring(10,30+10*i,0, glv("test"+i) );

  for(i = 0; i < NUM_SETS; i++) { // ############### 3 è il numero di sets (arcade ecc..)
    int queue = i-selector; // prendiamo il selezionato -> in questo modo il selezionato (queue) sarà sempre == 0 e gli altri a seguire

    if ( queue < 0 ) queue = NUM_SETS+queue; // se siamo sotto lo 0 prendiamo l'ultimo
    a = dist*(queue)+da; // posizione dal basso verso l'alto

    x = x_shift;
    y = y_shift - a;

    size = ((size_max-size_min)*a)/(max_dist);
    size += size_min;
    size = (size_max+size_min)-size; // inverted

    if ( i == selector ) size2 = 1.0;
    else size2 = size;

    changedrawmethod(NULL(), "reset", 1);
    changedrawmethod(NULL(), "scalex", 256*size);
    changedrawmethod(NULL(), "scaley", 256*size);
    changedrawmethod(NULL(), "alpha", 6*(size2!=1)); // quando è grande al max si setta a 0
    changedrawmethod(NULL(), "channelr", 255*size2);
    changedrawmethod(NULL(), "channelg", 255*size2);
    changedrawmethod(NULL(), "channelb", 255*size2);
    changedrawmethod(NULL(), "centerx", 80);
    changedrawmethod(NULL(), "centery", 60);

    drawsprite(get(prevs, i), x, y, y + 1000);
  }

	if ( !get_blackscreen() ) {
        set_blackscreen(1);
        return;
	}

	if(key_coin("moveup")) {
		if(--selector<0) {
			selector = size(items)-1;
		}
        set_delta(-1*dist);
        set_delta_time(time);
        reset_lost_frames();
		playsample(se2);
	} else if(key_coin("movedown")) {
		if(++selector>=size(items)) {
			selector = 0;
		}
        set_delta(dist);
        set_delta_time(time);
        reset_lost_frames();
		playsample(se2);
	} else if(key_coin("start") || key_coin("attack")) {
		from_stage_to_player_select();
	} else if(key_coin("esc") || key_coin("special")) {
	    if ( ggv("1VS1") && glv("mode") != "1VS1_stage" ) from_stage_to_vs_winner_summary();
	    else from_stage_to_mode_select();
	}
	slv("mselector", selector);
}

// catalogue pics
void chooseStage4() {
  int time = openborvariant("elapsed_time");
  int game_speed = openborvariant("game_speed");
  int lost_frame = get_lost_frames()+1;
  int i,p;
  void ent;
  float a,x,y,z,w,size,size2;
  int selector = glv("mselector");
  void items = get_stage_item();
  void prevs = get_prev_item();
  int se = loadsample("data/sounds/beep2.wav");
  int se2 = loadsample("data/sounds/beep.wav");
  int se3 = loadsample("data/sounds/cancel01.wav");
  int vres = openborvariant("vresolution");
  int hres = openborvariant("hresolution");
  float da = get_delta(DELTA_MIN); //, t_da = 0.8, t_da_min = 0.1;
  int NUM_SETS = size(get_stage_item());
  float max_dist = 260, half_dist = max_dist/2.0, center = hres/2, y_shift = 120.0, pos_fix = 0; // center centra la pic
  float size_max = 0.94, size_min = 0.35;
  float dist = max_dist/NUM_SETS; // distanza da ricoprire == 120
  void title_bg = glv("title_bg");
  char text  = "STAGE SELECT";

  int time = ov("elapsed_time");
  int game_speed = ov("game_speed");
  int waiting_time = game_speed*0.0;
  int lighting_time = game_speed*0.95;
  int refresh_time = time%(lighting_time + (waiting_time*1.0));
  int tot_color_changes = 12 - 1;
  int black = rgbcolor(0x00,0x00,0x00);
  int white = rgbcolor(0xFF,0xFF,0xFF);
  int color0 = rgbcolor(0xFF,0x84,0x00); // dark
  int color1 = rgbcolor(0xFF,0x95,0x23);
  int color2 = rgbcolor(0xFF,0xA0,0x3B);
  int color3 = rgbcolor(0xFF,0xAA,0x50);
  int color4 = rgbcolor(0xFF,0xB3,0x62);
  int color5 = rgbcolor(0xFF,0xB9,0x6D); // light
  int color6 = rgbcolor(0xFF,0xC6,0x8A);

  draw_blackscreen();

  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "enabled", 1);
  changedrawmethod(NULL(), "tintmode", 4);
       if ( refresh_time <= (lighting_time/tot_color_changes)*0  ) changedrawmethod(NULL(), "tintcolor", color0);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*1  ) changedrawmethod(NULL(), "tintcolor", color1);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*2  ) changedrawmethod(NULL(), "tintcolor", color2);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*3  ) changedrawmethod(NULL(), "tintcolor", color3);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*4  ) changedrawmethod(NULL(), "tintcolor", color4);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*5  ) changedrawmethod(NULL(), "tintcolor", color5);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*6  ) changedrawmethod(NULL(), "tintcolor", color6);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*7  ) changedrawmethod(NULL(), "tintcolor", color5);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*8  ) changedrawmethod(NULL(), "tintcolor", color4);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*9  ) changedrawmethod(NULL(), "tintcolor", color3);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*10 ) changedrawmethod(NULL(), "tintcolor", color2);
  else if ( refresh_time <= (lighting_time/tot_color_changes)*11 ) changedrawmethod(NULL(), "tintcolor", color1);
  if ( glv("pic_stage") == NULL() ) slv("pic_stage",loadsprite("data/sprites/pic_stage.gif"));
  drawsprite(glv("pic_stage"), 0, -32, 2000);
  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "enabled", 0);

  if (!title_bg) slv("title_bg", spawnsubentity("title_bg", 0,0,0));

  /// ######### SMOOTH CODE (BEST) #########
  dec_delta(DELTA_SPEED,DELTA_MIN);
  da = get_delta(DELTA_MIN);
  /// ##################################################

  drawstring(hres/2-3-strwidth(text, 0)/2, 10, 6, text);
  drawstring(160-strwidth(get(items,selector),1)/2, 192,0,get(items,selector));
  //for(i=0;i<NUM_SETS;++i) drawstring(10,30+10*i,0, glv("test"+i) );

  for(i = 0; i < NUM_SETS; i++) { // ############### 3 è il numero di sets (arcade ecc..)
    int queue = i-selector; // prendiamo il selezionato -> in questo modo il selezionato (queue) sarà sempre == 0 e gli altri a seguire

    if ( queue < 0 ) queue = NUM_SETS+queue; // se siamo sotto lo 0 prendiamo l'ultimo

    a = (dist*queue)+da; // posizione
    x = a;
    x += center;
    if ( x > center+half_dist ) { // sistemiamo lato sx (max_dist/2 perchè è la distanza di ognuno dei 2 lati)
        float x_shift = center-half_dist;

        /*
        pos_fix è un fix ed è lo scostamento in pixel del quadro quando supera la half_dist
        per es. se i quadri sono 11 e la dist fra loro è 23, fino a 23*5 (dist*queue) è tutto ok perchè non si supera la half_dist
        ma quando facciamo 23*6 superariamo la half_dist: es. 138 > 130 (half_dist) e dunque quegli 8 pixels (138-130) sono il pos_fix
        */
        pos_fix = ((dist*queue)%half_dist)%dist;
        a += pos_fix;
        x += pos_fix;

        //x = x % (center+half_dist+1);
        x = mod(x,center+half_dist); // % con mantissa, torniamo indietro
        x += x_shift;
        //drawstring(10,40+10*i,0,pos_fix);
    }
    y = y_shift;

    //x:1.0=a:half_dist
    size = ((size_max-size_min)*(a%(half_dist+1)))/(half_dist);
    size += size_min;
    if ( x >= center ) size = (size_max+size_min)-size; // inverted
    if ( i == selector ) size = size_max;

    if ( i == selector ) size2 = 1.0;
    else size2 = size;

    changedrawmethod(NULL(), "reset", 1);
    changedrawmethod(NULL(), "scalex", 256*size);
    changedrawmethod(NULL(), "scaley", 256*size);
    changedrawmethod(NULL(), "alpha", 6*(size2!=1)); // quando è grande al max si setta a 0
    changedrawmethod(NULL(), "channelr", 255*size2);
    changedrawmethod(NULL(), "channelg", 255*size2);
    changedrawmethod(NULL(), "channelb", 255*size2);
    changedrawmethod(NULL(), "centerx", 80);
    changedrawmethod(NULL(), "centery", 60);

    if ( x < center ) queue *= -1.0;
    if ( i == selector ) queue = -1.0*NUM_SETS;
    z = y+1000.0-queue;

    drawsprite(get(prevs, i), x, y, z);
  }

	if ( !get_blackscreen() ) {
        set_blackscreen(1);
        return;
	}

	if(key_coin("moveleft")) {
		if(--selector<0) {
			selector = size(items)-1;
		}
        set_delta(-1*dist);
        set_delta_time(time);
        reset_lost_frames();
		playsample(se2);
	} else if(key_coin("moveright")) {
		if(++selector>=size(items)) {
			selector = 0;
		}
        set_delta(dist);
        set_delta_time(time);
        reset_lost_frames();
		playsample(se2);
	} else if(key_coin("start") || key_coin("attack")) {
		from_stage_to_player_select();
	} else if(key_coin("esc") || key_coin("special")) {
	    if ( ggv("1VS1") && glv("mode") != "1VS1_stage" ) from_stage_to_vs_winner_summary();
	    else from_stage_to_mode_select();
	}
	slv("mselector", selector);
}

void from_stage_to_main_menu() {
    int se = loadsample("data/sounds/beep2.wav");
    int se2 = loadsample("data/sounds/beep.wav");
    int se3 = loadsample("data/sounds/cancel01.wav");

    playsample(se3);
    setglobalvar("menu_mode","main");
    if( glv("pic_stage") ) { free(glv("pic_stage")); slv("pic_stage",NULL()); }
    sprite_swing_horizontal("pic_stage_arrow_dx","data/sprites/pic_stage_arrow.gif",251,100,0,2001,0.5,0.1,0.5,0.02,1,1);
    sprite_swing_horizontal("pic_stage_arrow_sx","data/sprites/pic_stage_arrow.gif",251,100,0,2001,0.5,0.1,0.5,0.02,0,1);
    if (glv("title_bg")) killentity(glv("title_bg"));
    slv("title_bg", NULL());
    clp("background", 0, "enabled", 0);
    change_bglayer(BGLAYERS_NUM,0,1);
    slv("mselector", 0);
    slv("mode_selector", 0);
    set_blackscreen(0);
    slv("start", "menu");
    slv("mode", NULL());
    reset_versus_mode(1);
    set_delta(0);
    set_delta_time(0);
    reset_lost_frames();
}

void from_stage_to_mode_select() {
    int se = loadsample("data/sounds/beep2.wav");
    int se2 = loadsample("data/sounds/beep.wav");
    int se3 = loadsample("data/sounds/cancel01.wav");

    playsample(se3);
    setglobalvar("menu_mode","mode");
    if( glv("pic_stage") ) { free(glv("pic_stage")); slv("pic_stage",NULL()); }
    sprite_swing_horizontal("pic_stage_arrow_dx","data/sprites/pic_stage_arrow.gif",251,100,0,2001,0.5,0.1,0.5,0.02,1,1);
    sprite_swing_horizontal("pic_stage_arrow_sx","data/sprites/pic_stage_arrow.gif",251,100,0,2001,0.5,0.1,0.5,0.02,0,1);
    if (glv("title_bg")) killentity(glv("title_bg"));
    slv("title_bg", NULL());
    clp("background", 0, "enabled", 0);
    change_bglayer(BGLAYERS_NUM,0,1);
    slv("mselector", 0);
    //slv("mode_selector", 0); // <---------- NOT CLEAN LAST STAGE SELECTOR
    set_blackscreen(0);
    slv("start", "mode");
    slv("mode", NULL());
    reset_versus_mode(1);
    set_delta(0);
    set_delta_time(0);
    reset_lost_frames();
}

void from_stage_to_vs_winner_summary() {
    int se = loadsample("data/sounds/beep2.wav");
    int se2 = loadsample("data/sounds/beep.wav");
    int se3 = loadsample("data/sounds/cancel01.wav");

    playsample(se3);
    setglobalvar("menu_mode","main");
    if( glv("pic_stage") ) { free(glv("pic_stage")); slv("pic_stage",NULL()); }
    sprite_swing_horizontal("pic_stage_arrow_dx","data/sprites/pic_stage_arrow.gif",251,100,0,2001,0.5,0.1,0.5,0.02,1,1);
    sprite_swing_horizontal("pic_stage_arrow_sx","data/sprites/pic_stage_arrow.gif",251,100,0,2001,0.5,0.1,0.5,0.02,0,1);
    if (glv("title_bg")) killentity(glv("title_bg"));
    slv("title_bg", NULL());
    clp("background", 0, "enabled", 0);
    change_bglayer(BGLAYERS_NUM,0,1);
    slv("mselector", 0);
    slv("mode_selector", 0);
    set_blackscreen(0);
    slv("start", NULL());
    slv("mode", NULL());
    //reset_versus_mode(1);
    set_delta(0);
    set_delta_time(0);
    reset_lost_frames();
}

void from_stage_to_player_select() {
    int se = loadsample("data/sounds/beep2.wav");
    int se2 = loadsample("data/sounds/beep.wav");
    int se3 = loadsample("data/sounds/cancel01.wav");
    int i = 0;

    playsample(se);
    setglobalvar("menu_mode","player");
    if( glv("pic_stage") ) { free(glv("pic_stage")); slv("pic_stage",NULL()); }
    sprite_swing_horizontal("pic_stage_arrow_dx","data/sprites/pic_stage_arrow.gif",251,100,0,2001,0.5,0.1,0.5,0.02,1,1);
    sprite_swing_horizontal("pic_stage_arrow_sx","data/sprites/pic_stage_arrow.gif",251,100,0,2001,0.5,0.1,0.5,0.02,0,1);
    checkSetupPlayerList(glv("mselector"));
    set_delta(0);
    set_delta_time(0);
    reset_lost_frames();
    if (glv("title_bg")) killentity(glv("title_bg"));
    slv("title_bg", NULL());
    playmusic("data/music/menu.bor", 1);

    for( i = 0; i < MAX_PLAYERS; i++ ) {
        if ( (COIN_SYSTEM && gpp(i,"credits") > 0) || COIN_SYSTEM == 0) {
            if( keyp_coin(i, "start") || keyp_coin(i, "attack") ) slv("player."+i+".join", 1);
        }
    }
    clp("background", 0, "enabled", 0);
    change_bglayer(BGLAYERS_NUM,0,1);

    slv("mselector", 0);
    slv("mode_selector", 0);
    set_blackscreen(0);
    slv("start", "select");
}

float dec_delta(float speed, float min_delta) {
  float da_time = 0, t_delta = 0.9; //min_da = 0.00001, t_da_time = 15.0;
  float da = get_delta(min_delta);
  int time = ov("elapsed_time");

  /*if ( da > 0 ) {
    t_delta += 0.2;
  }*/

  da_time = time-get_delta_time();
  da_time /= speed; // speed
  ++da_time; // da_time >= 1
  //if ( da_time > t_da_time ) da_time = t_da_time;
  da /= da_time;
  set_delta(da);
  if( !get_delta_time() || (da >= -1*t_delta && da <= t_delta) ) { da = min_delta; set_delta(min_delta); set_delta_time(0); } // threshold

  //drawstring(10, 30, 0, "delta time: "+da_time);
  //drawstring(10, 40, 0, "delta: "+da);

  return da;
}

void set_delta(float delta) {
    slv("delta",delta);
}

float get_delta(float min_delta) {
    if ( glv("delta") == NULL() && min_delta != NULL() ) return min_delta;
    else if ( glv("delta") == NULL() ) return 0;
    else return glv("delta");
}

void set_delta_time(int time) {
    slv("da_time",time);
}

int get_delta_time() {
    if ( glv("da_time") == NULL() ) return 0;
    else return glv("da_time");
}

