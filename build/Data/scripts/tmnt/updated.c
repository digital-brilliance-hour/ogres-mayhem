
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/update.c"

#import "data/scripts/custom_hud.c"
#import "data/scripts/custom_hud_game_over.c"
#import "data/scripts/custom_hud_hiscores.c"
#import "data/scripts/custom_hud_player_textmoves.c"
#import "data/scripts/lib_drawing.c"
#import "data/scripts/loading.c"

#define T_LAYERS_NUM 3000

void main() {
    int p = 0;

   /*if ( openborvariant("in_level") ) {
        if ( getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 ) splitscreen(2);
   }*/

    clear_loading_stuffs();

    for (p = 0; p < MAX_PLAYERS; ++p) {
        set_hpbar(p);
        draw_credits_ps(p);
    }

    check_pause();
    show_inserted_credits();
    if ( getglobalvar("1VS1") == NULL() ) check_continue();
    overlap_hiscore_scr();

   //setglobalvar("zoomentity",getplayerproperty(0,"entity"));
   if( getglobalvar("zoomentity") != NULL() ) {
        setglobalvar("zoomx",0);
        setglobalvar("zoomy",0);
        setglobalvar("zoomvalue",256*2);
        zoom();
   }

   /*if ( playerkeys(0,1,"attack") ) {
        take_screenshot("td_screenshot");
   }
   if ( ggv("td_screenshot") != NULL() ) {
        drawscreen(ggv("td_screenshot"), 100, 100, openborconstant("MAX_INT")-2);
   }*/

   quake();
   //rocking();
   //draw_with_tint(2, rgbcolor(0x00,0x00,0x00));


   //drawstring(10,220,0,"FPS: "+getFPS());
   //if ( time_to_wait("aaa",400) ) { int r=get_rand_seed(); slv("r",r); reset_time_to_wait("aaa"); }
   //drawstring(10,220,0,"r: "+glv("r"));
}

void ondestroy() {
    unload_sp_characters();
    unload_continue_nums();
    clear_loading_stuffs();
    clear_pause_fonts();
    clear_color_font();
    destroy_overlap_hiscore_scr();
}

void splitscreen(int scr_num) {
   void vscreen = openborvariant("vscreen"); // Screenshot dello schermo
   int maxz = openborvariant("PLAYER_MAX_Z")+T_LAYERS_NUM; // Lo z della stage. Quindi la risoluzione. Prendiamo quindi lo z + 1000! Stiamo considerando ogni valore di z come se fosse un layer uno sopra l'altro.
   int zoom_value = 256;
   int player_xshift = 0;
   int player_yshift = 0;
   int hres = openborvariant("hresolution");
   int vres = openborvariant("vresolution");
   int xpos = openborvariant("xpos");
   int ypos = openborvariant("ypos");
   int i = 0;
   int fixed_flag = 0;

   for ( i = 0; i < scr_num; ++i ) {
       void ent;
       int px, py;
       void split_scr = getglobalvar("split_screen"+i);
       float scr_pos;
       int fix_x = 0;
       float pos_error = 1.0/scr_num;
       int p = 0;

       // store screens
       if( !split_scr ){
          split_scr = allocscreen(hres,vres);
          setglobalvar("split_screen"+i, split_scr);
       }

       ent = getplayerproperty(i,"entity");
       if ( ent == NULL() ) { // find 1st existing player
           for ( p = 0; p < MAX_PLAYERS; ++p ) {
                ent = getplayerproperty(p,"entity");
                if ( getentityproperty(ent,"exists") ) break;
           }
       }
       px = getentityproperty(ent,"x") + player_xshift - xpos;
       py = getentityproperty(ent,"z") + player_yshift - ypos - getentityproperty(ent,"y");

       drawspriteq(split_scr, 0, openborconstant("MIN_INT"), maxz, 0, 0); // Openbor constants MIN_INT and MAX_INT are like their names, smallest and biggest integer value possible.

       //setup drawMethod
       changedrawmethod(NULL(), "reset", 1); // resets the drawmethod, clearing all values you changed before
       changedrawmethod(NULL(), "enabled", 1); // is the switch to turn it on/off, this is optional but just in case
       changedrawmethod(NULL(), "scalex", zoom_value/scr_num);
       changedrawmethod(NULL(), "scaley", zoom_value);
       changedrawmethod(NULL(), "centerx", px);
       changedrawmethod(NULL(), "centery", py);

        // FIX SCR
       scr_pos = px/scr_num + (hres/scr_num)*i + 1*i;
       if ( get_mantix(scr_pos) >= pos_error && !fixed_flag ) {  // if ( i == 0 && get_mantix(scr_pos) >= 0.5 ) fix_x = 1;
            fix_x = 1;
            fixed_flag = 1;
       }
       /*if ( i == 0 ) {
            drawstring(10,100,0,"scr_pos_mantix:  "+get_mantix(scr_pos));
            drawstring(10,110,0,"pos_error:  "+pos_error);
            drawstring(10,120,0,"pos:  "+(scr_pos));
       }*/

       //Draw the resized customized screen to main screen.
       drawscreen(split_scr, scr_pos+fix_x, py, maxz+1);
       changedrawmethod(NULL(), "enabled", 0);
   } // fine for

    drawspriteq(vscreen, 0, maxz+1, maxz+1, 0, 0); // 0,maxz+1 sono i layers, e gli ultimi 0 0  sono gli offset
    drawspriteq(vscreen, 0, maxz+2, openborconstant("MAX_INT"), 0, 0);

    /// ----------- CLEAR -----------
   clearspriteq();

   //drawstring(10,100,0,"scr_pos_mantix: "+string);
}

void rocking() { // ship simulating
    int loop = getglobalvar("rocking_loop");
    int refresh_time = getglobalvar("rocking_refreshtime");
    int intensity = getglobalvar("rocking_intensity");

    /*intensity = 1;
    refresh_time = 1;
    loop = 22;*/
    if ( loop != NULL() && refresh_time != NULL() && intensity != NULL() && openborvariant("in_level") ) {
        int time = openborvariant("elapsed_time");
        int ETA = refresh_time*loop;

        // Check Time-Reset
        if ( check_stored_time(getlocalvar("finish_rocking_time"),9999) ) setlocalvar("finish_rocking_time", openborconstant("MIN_INT"));

        if ( getlocalvar("finish_rocking_time") == NULL() ) setlocalvar("finish_rocking_time",get_next_time(ETA));

        if ( time%refresh_time == 0 ) {
            changelevelproperty("rocking", intensity);
            if ( loop > 0 ) --loop;
            else loop = NULL();
            if ( time > getlocalvar("finish_rocking_time") ) loop = NULL();

            setglobalvar("rocking_loop", loop);

            if ( loop == NULL() ) {
                setglobalvar("rocking_refreshtime", NULL());
                setglobalvar("rocking_intensity", NULL());
                setlocalvar("finish_rocking_time", NULL());
            }
        }
        if ( time > getlocalvar("finish_rocking_time") ) {
            setglobalvar("rocking_loop", NULL());
            setglobalvar("rocking_refreshtime", NULL());
            setglobalvar("rocking_intensity", NULL());
            setlocalvar("finish_rocking_time", NULL());
        }
    } // fine if loop

}

void quake() {
    int loop = getglobalvar("quake_loop");
    int refresh_time = getglobalvar("quake_refreshtime");
    int intensity = getglobalvar("quake_intensity");

    /*intensity = 5;
    refresh_time = 2;
    loop = 220;*/
    /*if ( openborvariant("in_level") ) {
        drawstring(10,60,0,"cameraz:  " + getlevelproperty("camerazoffset"));
        //drawstring(10,70,0,"basemap:  " + getlevelproperty("basemap"));
        drawstring(10,80,0,"bgspeed:  " + getlevelproperty("bgspeed"));
        drawstring(10,90,0,"maxfallspeed:  " + getlevelproperty("maxfallspeed"));
        drawstring(10,100,0,"maxtossspeed:  " + getlevelproperty("maxtossspeed"));
        drawstring(10,110,0,"quake:  " + getlevelproperty("quake"));
        drawstring(10,120,0,"scrollspeed:  " + getlevelproperty("scrollspeed"));
    }*/

    if ( loop != NULL() && refresh_time != NULL() && intensity != NULL() && openborvariant("in_level") ) {
        int time = openborvariant("elapsed_time");
        int ETA = refresh_time*loop;

        // Check Time-Reset
        if ( check_stored_time(getlocalvar("finish_quake_time"),9999) ) setlocalvar("finish_quake_time", openborconstant("MIN_INT"));

        if ( getlocalvar("finish_quake_time") == NULL() ) setlocalvar("finish_quake_time",get_next_time(ETA));

        if ( time%refresh_time == 0 ) {
            changelevelproperty("quake", intensity);
            /*if ( getlocalvar("camerazoffset") == NULL() ) setlocalvar("camerazoffset", getlevelproperty("camerazoffset"));
            if ( getlevelproperty("camerazoffset") <= getlocalvar("camerazoffset") ) changelevelproperty("camerazoffset", getlocalvar("camerazoffset")+intensity);
            else changelevelproperty("camerazoffset", getlocalvar("camerazoffset")-intensity);*/

            if ( loop > 0 ) --loop;
            else loop = NULL();
            if ( time > getlocalvar("finish_quake_time") ) loop = NULL();

            setglobalvar("quake_loop", loop);

            if ( loop == NULL() ) {
                setglobalvar("quake_refreshtime", NULL());
                setglobalvar("quake_intensity", NULL());
                setlocalvar("finish_quake_time", NULL());

                //if ( getlocalvar("camerazoffset") != NULL() ) changelevelproperty("camerazoffset", getlocalvar("camerazoffset"));
                //setlocalvar("camerazoffset", NULL());
            }
        }
        if ( time > getlocalvar("finish_quake_time") ) {
            setglobalvar("quake_loop", NULL());
            setglobalvar("quake_refreshtime", NULL());
            setglobalvar("quake_intensity", NULL());
            setlocalvar("finish_quake_time", NULL());

            //if ( getlocalvar("camerazoffset") != NULL() ) changelevelproperty("camerazoffset", getlocalvar("camerazoffset"));
            //setlocalvar("camerazoffset", NULL());
        }
    } // fine if loop
}

void draw_with_tint(int tintmode, int tintcolor) {
    if ( openborvariant("in_level") ) {
        int i;
        int vres = openborvariant("vResolution");
        int hres = openborvariant("hResolution");
        //void screen = allocscreen(hres,vres);

        //clearscreen(screen);
        changeshadowcolor(tintcolor, 1);
        for (i = 0; i < openborvariant("count_entities"); ++i) { // openborconstant("MAX_ENTS")
            void ent = getentity(i);

            if ( getentityproperty(ent, "exists") ) {
                void ent_name = getentityproperty(ent, "defaultmodel");

                changedrawmethod(ent, "reset", 1); // resets the drawmethod, clearing all values you changed before
                changedrawmethod(ent, "enabled", 1); // is the switch to turn it on/off, this is optional but just in case
                if ( ent_name != "parrow1" && ent_name != "parrow2" && ent_name != "parrow3" && ent_name != "parrow4"
                     && ent_name != "press_any_arrow" && ent_name != "press_any_button" && ent_name != "press_attack_button" && ent_name != "press_attack_button_escape" && ent_name != "bind_grab_ready"
                     && ent_name != "cflash" && ent_name != "dflash" && ent_name != "death_stars" && ent_name != "say_atk" && ent_name != "say_cave" && ent_name != "say_erad" && ent_name != "shellshock"
                     && ent_name != "dust" && ent_name != "climb_dust" && ent_name != "bind_grab_ready" && ent_name != "bsplat_flash" && ent_name != "tr_splat_flash" && ent_name != "arrow_flame"
                     && ent_name != "land_sparkles" && ent_name != "tr_splat_flash" && ent_name != "boom_foot" && ent_name != "boom01" && ent_name != "boom02" && ent_name != "boom03" ) {
                    changedrawmethod(ent, "flag", 1);
                    //changedrawmethod(ent, "scalex", 256);
                    //changedrawmethod(ent, "scaley", 256);
                    //changedrawmethod(ent, "remap", -1);
                    //changedrawmethod(ent, "rotate", 30-1);

                    changedrawmethod(ent, "tintmode", tintmode);
                    changedrawmethod(ent, "tintcolor", tintcolor);
                    //changeentityproperty(ent, "gfxshadow", 0);

                    //entity, int flag, int scalex, int scaley, int flipx, int flipy, int shiftx, int alpha, int colourmap, int fillcolour, int rotate, int rotateflip, int transparencybg)
                    ///setdrawmethod(ent, 1, 256, 256, 0, 0, 0, 0, -1, 0, 0, 0, 0);
                    // scalex = scaley = 256, colourmap -1
                }

            } // fine if exists
        }
        changedrawmethod(NULL(), "flag", 0);
        changedrawmethod(NULL(), "enabled", 0);
        setdrawmethod(NULL(), 0);


    } // fine if in_level

}

void zoom() {
   void vscreen = openborvariant("vscreen"); // Screenshot dello schermo
   int maxz = openborvariant("PLAYER_MAX_Z")+T_LAYERS_NUM; // Lo z della stage. Quindi la risoluzione. Prendiamo quindi lo z + 1000! Stiamo considerando ogni valore di z come se fosse un layer uno sopra l'altro.
   int zoom_value = getglobalvar("zoomvalue");
   int zoom_x = getglobalvar("zoomx");
   int zoom_y = getglobalvar("zoomy");
   void ent = getglobalvar("zoomentity");
   int px = getentityproperty(ent,"x") + zoom_x - openborvariant("xpos");
   int py = getentityproperty(ent,"z") + zoom_y - openborvariant("ypos") - getentityproperty(ent,"y");
   void zoom_scr = getglobalvar("zoomscreen");

   if( !zoom_scr ){
      zoom_scr = allocscreen(openborvariant("hresolution"),openborvariant("vresolution"));
      setglobalvar("zoomscreen", zoom_scr);
   }
   //clearscreen(zoom_scr); // Puliamo lo schermo da rimasugli tipo drawbox o drawlines ecc...

   //draw what we need
   /*
    screen -  a screen or leave it NULL for your main screen
    0 - reserved
    min - min layer (aka min z)
    max - max layer (aka max z)
    dx - x offset
    dy - y offset
   */
   drawspriteq(zoom_scr, 0, openborconstant("MIN_INT"), maxz, 0, 0); // Openbor constants MIN_INT and MAX_INT are like their names, smallest and biggest integer value possible.

   //setup drawMethod
   changedrawmethod(NULL(), "reset", 1); // resets the drawmethod, clearing all values you changed before
   changedrawmethod(NULL(), "enabled", 1); // is the switch to turn it on/off, this is optional but just in case
   changedrawmethod(NULL(), "scalex", zoom_value);
   changedrawmethod(NULL(), "scaley", zoom_value);
   changedrawmethod(NULL(), "centerx", px);
   changedrawmethod(NULL(), "centery", py);

   //Draw the resized customized screen to main screen.
   drawscreen(zoom_scr, px, py, maxz+1);
   changedrawmethod(NULL(), "enabled", 0);
   drawspriteq(vscreen, 0, maxz+1, maxz+1, 0, 0);
   drawspriteq(vscreen, 0, maxz+2, openborconstant("MAX_INT"), 0, 0);

   clearspriteq();
}

void take_screenshot(void label) {
   //int maxz = openborvariant("PLAYER_MAX_Z")+T_LAYERS_NUM; // Lo z della stage. Quindi la risoluzione. Prendiamo quindi lo z + 1000! Stiamo considerando ogni valore di z come se fosse un layer uno sopra l'altro.
   void scr = getglobalvar(label);

   if ( scr ) {
      clearscreen(scr);
      setglobalvar(label,NULL());
      scr = getglobalvar(label);
   }

   if( !scr ){
      scr = allocscreen(openborvariant("hresolution"),openborvariant("vresolution"));
      drawspriteq(scr, 0, openborconstant("MIN_INT"), openborconstant("MAX_INT"), 0, 0); // Openbor constants MIN_INT and MAX_INT are like their names, smallest and biggest integer value possible.
      setglobalvar(label, scr);
   }

   return scr;
}

/*
 if( getentityproperty(self, "animationid") == openborconstant("ANI_ATTACK1") || getentityproperty(self, "animationid") == openborconstant("ANI_ATTACK2")
      || getentityproperty(self, "animationid") == openborconstant("ANI_ATTACK3") || getentityproperty(self, "animationid") == openborconstant("ANI_ATTACK4")
      || getentityproperty(self, "animationid") == openborconstant("ANI_ATTACK5")  ) {
      setglobalvar ("zoomentity", self);
      setglobalvar ("zoomvalue", 384);
      setglobalvar ("zoomx", 0);
      setglobalvar ("zoomy", -80);
      //zoom();
 } else {
     setglobalvar ("zoomentity", NULL());
 }
*/

