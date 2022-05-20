/*
aggiungere:
    load water_splash
    load water_ripple_front
    load water_ripple_back

    ondrawscript    data/scripts/player_ondrawscript.c
*/

#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

#define SPLASH_SOUNDFX "data/sounds/water_splash01.wav"
#define ENT_WATER_SPLASH "water_splash"
#define ENT_RIPPLE_FRONT "water_ripple_front"
#define ENT_RIPPLE_BACK  "water_ripple_back"
#define RIPPLE_SPAWNING_FRAMES_PAUSE 2
#define SPLASH_DOWN_START_FRAME 2
#define RIPPLE_XSHIFT 3
#define RIPPLE_ASHIFT 0
#define WATER_H_FIX 0

/*void main() {
  void self = getlocalvar("self");

    // usare ondrawscript
    //draw_clipping_color(self, 70, 15, 0xFF8000);
    //draw_clipping_color(self, 70, 15, rgbcolor(0x00,0x80,0xFF));
}*/

int check_water_puddle_ents(void self) {
    int i;
    int anim_id = getentityproperty(self,"animationid");

    if ( anim_id == oc("ANI_SPAWN") || anim_id == oc("ANI_RESPAWN") ) {
        int anim_pos = getentityproperty(self,"animpos");
        if ( anim_pos <= 1 ) return;
    }

    if ( getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 ) { // Waiting, Select
        //int water_puddle_flag = 0;

        for (i = 0; i < openborvariant("count_entities"); ++i) { // openborconstant("MAX_ENTS")
            void ent = getentity(i);

            if ( getentityproperty(ent, "exists") ) {
                if ( getentityvar(ent, "type") == "water_puddle" ) {
                    //water_puddle_flag = 1;
                    check_water_puddle(self, ent);
                }
            } // fine if exists
        } // fine for all_ents
        /*if ( !water_puddle_flag ) {
            // Prevent No-Shadow to next level BUG!
            //if ( !getentityproperty(self,"gfxshadow") ) changeentityproperty(self,"gfxshadow",1);
            drawstring(10,100,0,"aaaa: "+getentityproperty(self,"gfxshadow"));
        }*/
    }
}

int check_ripple(void self) {
    int is_on_puddle;
    int is_on_base;
    int was_on_puddle = glv("was_on_puddle");
    int was_on_base = glv("was_on_base");
    void ripple = glv("ripple");
    void splash = glv("splash");
    int ripple_frame = RIPPLE_SPAWNING_FRAMES_PAUSE;

    is_on_puddle = is_on_puddle(self); // check if this ent is on puddle (like stairs)
    is_on_base = is_on_base(self); // or is in "y" of puddle

    // CHECK EXISTENCE
    if ( !is_name(ripple,"defaultmodel",ENT_RIPPLE_FRONT) || !is_for(ripple,self) ) { slv("ripple",NULL()); ripple = glv("ripple"); }
    if ( !is_name(splash,"defaultmodel",ENT_WATER_SPLASH) || !is_for(splash,self) ) { slv("splash",NULL()); splash = glv("splash"); }

    if ( is_on_puddle ) {
        if ( is_on_base ) {
            if ( is_stopped(self) ) { // Se il pg è fermo crea un ripple per volta
                if ( ripple == NULL() || !gep(ripple,"exists") ) ripple = ripple_spawn(self,"ripple_front","ripple_back",ENT_RIPPLE_FRONT,ENT_RIPPLE_BACK,get_water_map()); // spawn ent if not exists (mem ripple in a localvar)
                //else if ( !is_name(ripple,"defaultmodel",ENT_RIPPLE_FRONT) ) { slv("ripple",NULL()); ripple = glv("ripple"); }
            } else {
                // Se il pg è in movimento crea nuovi ripple dopo il frame X
                if ( ripple == NULL() || !gep(ripple,"exists") ) ripple = ripple_spawn(self,"ripple_front","ripple_back",ENT_RIPPLE_FRONT,ENT_RIPPLE_BACK,get_water_map());
                else {
                    //if ( !is_name(ripple,"defaultmodel",ENT_RIPPLE_FRONT) ) { slv("ripple",NULL()); ripple = glv("ripple"); }
                    if ( ripple != NULL() ) {
                        if ( gep(ripple,"animpos") >= ripple_frame ) ripple = ripple_spawn(self,"ripple_front","ripple_back",ENT_RIPPLE_FRONT,ENT_RIPPLE_BACK,get_water_map());
                    }
                }  // fine if ripple exists
            } // fine if is_in_movement
        } // fine if is_on_base

        if ( !was_on_puddle && !was_on_base ) {
            slv("splash",spawn_splash(self,ENT_WATER_SPLASH,SPLASH_SOUNDFX,SPLASH_DOWN_START_FRAME,get_water_map())); // splash down
            splash = glv("splash");
        }

        // adjust splash position for speedy moves
        if ( !check_splash_pos(self,glv("splash"),ENT_WATER_SPLASH,SPLASH_DOWN_START_FRAME) ) slv("splash",NULL());
    } else {
        //if ( was_on_puddle ) spawn_splash(self); // splash up
        if ( is_stopped(self) ) {
            //delete_ripple(self,"ripple_front","ripple_back",ENT_RIPPLE_FRONT,ENT_RIPPLE_BACK); // <---- optional
            slv("ripple",NULL());
            ripple == glv("ripple");
        }
    }

    if ( was_on_puddle && !is_on_base && was_on_base ) {
        slv("splash",spawn_splash(self,ENT_WATER_SPLASH,SPLASH_SOUNDFX,NULL(),get_water_map())); // splash up
        splash = glv("splash");
    }
    //if ( !is_on_base && was_on_base ) spawn_splash(self); // splash up

    slv("was_on_puddle",is_on_puddle);
    slv("was_on_base",is_on_base);
    slv("ripple",ripple);
    slv("splash",splash);
}

int is_on_puddle(void self) {
    if ( getlocalvar("is_on_water_puddle") > 0 ) return 1;
    else return 0;
}

void ripple_spawn(void self, char ripple_front, char ripple_back, char front_ripple_name, char back_ripple_name, int map) {
    if ( map == NULL() ) map = 0;

    slv(ripple_front,spawnsubentity_relative(front_ripple_name,RIPPLE_XSHIFT,0.1,RIPPLE_ASHIFT+get_water_height()-WATER_H_FIX,self));
    slv(ripple_back,spawnsubentity_relative(back_ripple_name,0,-1.0,0,glv(ripple_front)));

    //changeentityproperty(glv(ripple_front),"parent",self);
    changeentityproperty(glv(ripple_back),"parent",self);

    changeentityproperty(glv(ripple_front),"map",map);
    changeentityproperty(glv(ripple_back),"map",map);

    return glv(ripple_front);
}

int delete_ripple(void self, char ripple_front, char ripple_back, char front_ripple_name, char back_ripple_name) {
    if ( gep(glv(ripple_front),"exists") ) {
        if ( is_name(glv(ripple_front),"defaultmodel",front_ripple_name) && is_for(glv(ripple_front),self) ) { killentity(glv(ripple_front)); }
    }
    slv(ripple_front,NULL());

    if ( gep(glv(ripple_back),"exists") ) {
        if ( is_name(glv(ripple_back),"defaultmodel",back_ripple_name) && is_for(glv(ripple_back),self) ) { killentity(glv(ripple_back)); }
    }
    slv(ripple_back,NULL());

    return 1;
}

void spawn_splash(void self, char water_splash_name, char splash_sound_name, int frame, int map) {
    void splash;
    float base = gep(self,"base");

    if ( map == NULL() ) map = 0;

    splash = spawnsubentity_relative(water_splash_name,RIPPLE_XSHIFT,1,NULL(),self);
    changeentityproperty(splash,"position",NULL(),NULL(),base+RIPPLE_ASHIFT+get_water_height()-WATER_H_FIX);
    changeentityproperty(splash,"parent",self);
    changeentityproperty(splash,"map",map);

    if ( frame != NULL() ) {
        changeentityproperty(splash,"animpos",frame);
    }
    if ( splash_sound_name != NULL() ) playsample(loadsample(splash_sound_name));

    return splash;
}

// return 0 if it doesn't exists
int check_splash_pos(void self, void splash, char model_name, int frame) {
    if ( gep(splash,"exists") ) {
        int anim_pos = gep(splash,"animpos");

        if ( !is_name(splash,"defaultmodel",model_name) || !is_for(splash,self) ) return 0;

        if ( anim_pos <= frame ) {
            float x = gep(self,"x");
            float z = gep(self,"z");
            float a = gep(self,"y");
            float base = gep(self,"base");
            int dir = gep(self,"direction");
            float ex = gep(splash,"x");
            float xshift = RIPPLE_XSHIFT;

            if ( !dir ) xshift *= -1;
            if ( ex != x+xshift ) {
                cep(splash,"position",x+xshift,z+1,base+RIPPLE_ASHIFT+get_water_height()-WATER_H_FIX);
            }
        } // fine if animpos

        return 1;
    } else return 0; // fine if exists
}

int get_water_map() {
    return glv("water_map");
}

int set_water_map(int value) {
    return slv("water_map",value);
}

int get_water_height() {
    return glv("water_height");
}

int set_water_height(float value) {
    slv("water_height",value);
}

int check_puddle(void self, int no_a_flag) {
    int i;

    if ( getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 ) {
        for (i = 0; i < openborvariant("count_entities"); ++i) { // openborconstant("MAX_ENTS")
            void ent = getentity(i);

            if ( getentityproperty(ent, "exists") ) {
                if ( getentityvar(ent, "type") == "water_puddle" ) {
                    if ( no_a_flag && no_a_flag != NULL() ) return check_water_puddle_no_a(self, ent);
                    else return check_water_puddle(self, ent, 1);
                }
            } // fine if exists
        } // fine for all_ents

        return 0;
    }
}

int check_water_puddle(void self, void ent, int check_only_flag) {
                int p = getentityproperty(self, "playerindex");
                /*float x = getentityproperty(self, "x");
                float z = getentityproperty(self, "z");
                float a = getentityproperty(self, "y");
                float base = getentityproperty(self, "base");
                float ex = getentityproperty(ent, "x");
                float ez = getentityproperty(ent, "z");
                float ea = getentityproperty(ent, "y");
                float ebase = getentityproperty(ent, "base");*/
                float height = getentityvar(ent, "height");
                //float width = getentityvar(ent, "width");
                //float depth = getentityvar(ent, "depth");
                int subject_to_waterpuddle = getentityvar(self, "subject_to_water_puddle");

                if ( subject_to_waterpuddle <= 0 ) {
                    reset_water_puddle(self,1,0);
                    return 1;
                }

                if ( height == NULL() ) height = getentityproperty(ent, "height");
                if ( height == NULL() ) height = 0;
                set_water_height(height);
                set_water_map(getentityvar(ent,"map"));

                if ( getentityvar(ent, "type") == "water_puddle" ) {
                    if ( is_in_water_puddle(self,ent) ) {
                        if ( !check_only_flag || check_only_flag == NULL() ) {
                            draw_clipping_transp(self, 140, height, getentityvar(ent,"transp"), ent);
                            clip_character(self, -190, height, ent);
                            setlocalvar("is_on_water_puddle", 1);
                            setentityvar(self,"water_puddle",ent);
                        } else return 1;
                    } else {
                        if ( !check_only_flag || check_only_flag == NULL() ) {
                            if ( getlocalvar("is_on_water_puddle") == 1 ) {
                                reset_water_puddle_clipping(self);
                                setlocalvar("is_on_water_puddle", NULL());
                                setentityvar(self,"water_puddle",NULL());
                            }
                        } else return 0;
                    } // fine if coords
                }
}

int is_in_water_puddle(void self, void ent) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float ex = getentityproperty(ent, "x");
    float ez = getentityproperty(ent, "z");
    float ea = getentityproperty(ent, "y");
    float ebase = getentityproperty(ent, "base");
    float height = getentityvar(ent, "height");
    float width = getentityvar(ent, "width");
    float depth = getentityvar(ent, "depth");
    float shift_horiz = getentityvar(ent, "shift_horizontal");
    float shift_vert = getentityvar(ent, "shift_vertical");

    if ( shift_horiz == NULL() ) shift_horiz = 0;
    if ( shift_vert  == NULL() ) shift_vert = 0;

    //if ( x <= ex+width && x >= ex && z <= ez+2+depth/2 && z >= ez-depth/2 && a <= ebase+2+height && a >= ebase-2 )

    //shift_horiz = 110; //110
    //shift_vert = -110;
    //depth = 40;
    //width = 120;
    if ( a <= ebase+2+height && a >= ebase-2 ) {
        float rx, rz, check_x, check_z, invert_check_x, invert_check_z;

        //if ( x <= ex+width && x >= ex && z <= ez+2+depth && z >= ez ) return 1;

        // effettiva distanza (relativa) tra char e water puddle
        rz = diff(z,ez);
        rx = diff(x,ex);

        check_x = (abs(shift_horiz)*rz)/depth;
        check_z = (abs(shift_vert)*rx)/width;
        if ( shift_horiz < 0 ) check_x = abs(check_x-abs(shift_horiz)); // invert
        if ( shift_vert  < 0 ) check_z = abs(check_z-abs(shift_vert));  // invert
        invert_check_x = abs(check_x-abs(shift_horiz)); // invert
        invert_check_z = abs(check_z-abs(shift_vert));  // invert

        //if ( x >= ex-check_x+shift_horiz && x <= ex+width+invert_check_x-shift_horiz && z >= ez-check_z+invert_check_z && z <= ez+depth+2+invert_check_z-invert_check_z ) return 1; // no additional shift
        if ( x >= ex-check_x && x <= ex+width+invert_check_x && z >= ez-check_z && z <= ez+depth+2+invert_check_z ) return 1;
    }

    return 0;
}

int check_water_puddle_no_a(void self, void ent) {
                float x = getentityproperty(self, "x");
                float z = getentityproperty(self, "z");
                float a = getentityproperty(self, "y");
                float ex = getentityproperty(ent, "x");
                float ez = getentityproperty(ent, "z");
                float ea = getentityproperty(ent, "y");
                float width = getentityvar(ent, "width");
                float depth = getentityvar(ent, "depth");

                if ( getentityvar(ent, "type") == "water_puddle" ) {
                    if ( x <= ex+width && x >= ex && z <= ez+2+depth/2 && z >= ez-depth/2 ) {
                        return 1;
                    } else {
                        return 0;
                    } // fine if coords
                }
}

int reset_water_puddle_clipping(void self, int noshadow_flag) {
    changedrawmethod(NULL(), "enabled", 0);
    changedrawmethod(NULL(), "reset", 1);
    setdrawmethod(NULL(), 0);
    changedrawmethod(self, "enabled", 0);
    changedrawmethod(self, "reset", 1);
    setdrawmethod(self, 0);

    if ( !noshadow_flag || noshadow_flag == NULL() ) {
        int orig_gfx_shadow = getlocalvar("mem_wp_shadow");

        if ( orig_gfx_shadow == NULL() ) {
            setlocalvar("mem_wp_shadow",getentityproperty(self,"gfxshadow"));
            orig_gfx_shadow = getlocalvar("mem_wp_shadow");
        }

        if ( !getentityproperty(self,"gfxshadow") ) changeentityproperty(self,"gfxshadow",orig_gfx_shadow);
    }
}

int reset_water_puddle(void self, int noshadow_flag, int noclipping_reset) {
    if ( glv("was_on_puddle") != NULL() || glv("was_on_base") != NULL() || glv("is_on_water_puddle") != NULL() ) {
        if ( noclipping_reset <= 0 || noclipping_reset == NULL() ) reset_water_puddle_clipping(self,noshadow_flag);
        setlocalvar("water_puddle_found",NULL());
        setlocalvar("was_on_puddle",NULL());
        setlocalvar("was_on_base",NULL());
        setlocalvar("is_on_water_puddle",NULL());
        setentityvar(self,"water_puddle",NULL());
    }
}

int clip_character(void self, float clipx, float clipy, void ent) {
    /*int anim_id = getentityproperty(self,"animationid");
    int anim_pos = getentityproperty(self,"animpos");

    if ( (anim_id == oc("ANI_SPAWN") || anim_id == oc("ANI_RESPAWN")) && anim_pos <= 1 ) return;*/

    if ( openborvariant("in_level") ) {
        //void spr = getentityproperty(self, "sprite");
        float x = getentityproperty(self, "x");
        float z = getentityproperty(self, "z");
        float a = getentityproperty(self, "y");
        float base = getentityproperty(self, "base");
        int layer = getentityproperty(self, "setlayer");
        float xpos = openborvariant("xpos");
        float ypos = openborvariant("ypos");
        int colourmap = getentityproperty(self, "colourmap");
        int no_shadow = getentityvar(ent,"no_shadow");
        int alpha = getentityproperty(self, "alpha");
        //int channelr = getdrawmethod(self, "channelr");
        //int channelg = getdrawmethod(self, "channelg");
        //int channelb = getdrawmethod(self, "channelb");


        if ( no_shadow == NULL() ) no_shadow = 1;

            changedrawmethod(NULL(), "enabled", 1);
            changedrawmethod(NULL(), "reset", 1);

            //drawstring( 10,190,0,"Var (): "+truncA(xpos));
            if ( getlocalvar("mem_wp_shadow") == NULL() ) setlocalvar("mem_wp_shadow",getentityproperty(self,"gfxshadow"));
            if ( getentityproperty(self,"gfxshadow") && no_shadow ) {
                changeentityproperty(self,"gfxshadow",0);
            }

            //changedrawmethod(NULL(), "clip", 10, 20);
            changedrawmethod(self, "cliph", 256); // 75 ----> sono pixel che compongono il quadrato
            changedrawmethod(self, "clipw", 512); // 256 -> se aumento clipw poi devo aumentare anche clipx
            changedrawmethod(self, "clipx", clipx); // -100
            changedrawmethod(self, "clipy", -256-clipy+a-base); // -100  -> più abbassi e più tagli
            //changedrawmethod(self, "clipy", -100+a);
            // ####### TOGLI -base se vuoi che cambi il clipy in base a quello!!!

            //changedrawmethod(self, "channelr", channelr);
            //changedrawmethod(self, "channelg", channelg);
            //changedrawmethod(self, "channelb", channelb);

            // (entity, int flag, int scalex, int scaley, int flipx, int flipy, int shiftx, int alpha, int remap, int fillcolor, int rotate, int fliprotate, int transparencybg, void* colourmap, int centerx, int centery);
            //setdrawmethod(self, 1, 256, 256, facing, 0, 0, 6, 1, 0, 0, 0, 0, colourmap); // map: -1 = Use entity's colormap.
            setdrawmethod(self, 1, 256, 256, 0, 0, 0, alpha, 1, 0, 0, 0, 0, colourmap); // map: -1 = Use entity's colormap.

            changedrawmethod(NULL(), "enabled", 0);
            changedrawmethod(NULL(), "reset", 1);
            setdrawmethod(NULL(), 0);
    }
}

int draw_clipping_transp(void self, float clipx, float clipy, int transp, void ent) {
    /*int anim_id = getentityproperty(self,"animationid");
    int anim_pos = getentityproperty(self,"animpos");

    if ( (anim_id == oc("ANI_SPAWN") || anim_id == oc("ANI_RESPAWN")) && anim_pos <= 1 ) return;*/

    if ( openborvariant("in_level") ) {
        void spr = getentityproperty(self, "sprite");
        float x = getentityvar(self, "x");
        float z = getentityvar(self, "z");
        float a = getentityvar(self, "y");
        float base = getentityvar(self, "base");
        int facing = getentityvar(self, "direction");
        int layer = getentityproperty(self, "setlayer");
        float xpos = openborvariant("xpos");
        float ypos = openborvariant("ypos");
        int colourmap = getentityproperty(self, "colourmap");
        int time = openborvariant("elapsed_time");
        int game_speed = openborvariant("game_speed");
        int blink_flag = getentityproperty(self, "blink");
        //float transp = 40; //255

        if ( x == NULL() ) x = getentityproperty(self, "x");
        if ( z == NULL() ) z = getentityproperty(self, "z");
        if ( a == NULL() ) a = getentityproperty(self, "y");
        if ( base == NULL() ) base = getentityproperty(self, "base");
        if ( facing == NULL() ) facing = getentityproperty(self, "direction");
        if ( transp == NULL() ) transp = 0;

        if ( spr != NULL() ) {
            if (facing == 1) facing = 0;
            else facing = 1;

            changedrawmethod(NULL(), "enabled", 1);
            changedrawmethod(NULL(), "reset", 1);

            //drawstring( 10,190,0,"Var (): "+truncA(xpos));

            //changedrawmethod(NULL(), "clip", 10, 20);
            changedrawmethod(NULL(), "cliph", 356);
            changedrawmethod(NULL(), "clipw", 512); // se aumento clipw poi devo aumentare anche clipx
            changedrawmethod(NULL(), "clipx", x-xpos-clipx);
            changedrawmethod(NULL(), "clipy", z-a-ypos-4-clipy);

            changedrawmethod(NULL(), "channelg", transp);
            changedrawmethod(NULL(), "channelr", transp);
            changedrawmethod(NULL(), "channelb", transp);

            // (entity, int flag, int scalex, int scaley, int flipx, int flipy, int shiftx, int alpha, int remap, int fillcolor, int rotate, int fliprotate, int transparencybg, void* colourmap, int centerx, int centery);
            setdrawmethod(NULL(), 1, 256, 256, facing, 0, 0, 6, 1, 0, 0, 0, 0, colourmap); // map: -1 = Use entity's colormap.

            if ( (time%(game_speed/10) >= (game_speed/10)/2 && blink_flag) || !blink_flag ) drawsprite(spr, x-xpos, z-a-ypos-4, z+2, layer);

            changedrawmethod(NULL(), "enabled", 0);
            changedrawmethod(NULL(), "reset", 1);
            setdrawmethod(NULL(), 0);
        }

    }
}

int draw_clipping_color(void self, float clipx, float clipy, int color) {
    if ( openborvariant("in_level") ) {
        void spr = getentityproperty(self, "sprite");
        float x = getentityvar(self, "x");
        float z = getentityvar(self, "z");
        float a = getentityvar(self, "y");
        float base = getentityvar(self, "base");
        int facing = getentityvar(self, "direction");
        int layer = getentityproperty(self, "setlayer");
        float xpos = openborvariant("xpos");
        float ypos = openborvariant("ypos");
        int colourmap = getentityproperty(self, "colourmap");
        float exhift;
        //float transp = 40; //255

        if ( x == NULL() ) x = getentityproperty(self, "x");
        if ( z == NULL() ) z = getentityproperty(self, "z");
        if ( a == NULL() ) a = getentityproperty(self, "y");
        if ( base == NULL() ) base = getentityproperty(self, "base");
        if ( facing == NULL() ) facing = getentityproperty(self, "direction");

        if ( spr != NULL() ) {
            if (facing == 1) facing = 0;
            else facing = 1;

            changedrawmethod(NULL(), "enabled", 1);
            changedrawmethod(NULL(), "reset", 1);

            //drawstring( 10,190,0,"Var (): "+truncA(xpos));
            changedrawmethod(NULL(), "tintmode", 1);
            changedrawmethod(NULL(), "tintcolor", color);

            //changedrawmethod(NULL(), "clip", 10, 20);
            changedrawmethod(NULL(), "cliph", 356);
            changedrawmethod(NULL(), "clipw", 256); // se aumento clipw poi devo aumentare anche clipx
            changedrawmethod(NULL(), "clipx", x-xpos-clipx);
            changedrawmethod(NULL(), "clipy", z-a-ypos-4-clipy);

            //changedrawmethod(NULL(), "channelg", transp);
            //changedrawmethod(NULL(), "channelr", transp);
            //changedrawmethod(NULL(), "channelb", transp);

            // (entity, int flag, int scalex, int scaley, int flipx, int flipy, int shiftx, int alpha, int remap, int fillcolor, int rotate, int fliprotate, int transparencybg, void* colourmap, int centerx, int centery);
            setdrawmethod(NULL(), 1, 256, 256, facing, 0, 0, 6, 1, 0, 0, 0, 0, colourmap); // map: -1 = Use entity's colormap.

            drawsprite(spr, x-xpos, z-a-ypos-4, z+2, layer);

            changedrawmethod(NULL(), "enabled", 0);
            changedrawmethod(NULL(), "reset", 1);
            setdrawmethod(NULL(), 0);
        }

    }
}

