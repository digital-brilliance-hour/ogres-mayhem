
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"

float check_camera_type(float camerax, float cameraz_base, int basecam_flag, float scrollspeed, int zcam_flag, int xcam_flag, int y_dynamic_flag) {
    int i;
    float cameraz = 0, tot_y = 0, tot_z = 0, tot_x = 0, tot_base = 0;
    //float ypos = ov("ypos");
    int vres = ov("vresolution");
    float minz = ov("player_min_z");
    //float scrollspeed = glvp("scrollspeed");
    int players_in_game = ov("count_players");
    int player_count = 0;
    float div_fact = 1.5;

    if ( scrollspeed != NULL() ) {
        clvp("scrollspeed",scrollspeed);
    }

    if ( players_in_game <= 0 ) {
        clvp("camerazoffset",cameraz_base);
        clvp("cameraxoffset",camerax);
        return cameraz_base;
    }
    for (i = 0; i < MAX_PLAYERS; ++i) {
        void player = gpp(i, "entity");

        if ( gep(player, "exists") ) {
            float x = gep(player,"x");
            float z = gep(player,"z");
            float y = gep(player,"y");
            float base = gep(player,"base");
            //int health = gep(player,"health");

            if ( gep(player,"aiflag","dead") && players_in_game >= 1 ) continue;

            if ( basecam_flag != NULL() && basecam_flag > 0 ) {
                if ( y_dynamic_flag != NULL() && y_dynamic_flag > 0 ) {
                    if ( base <= 0 && checkhole(x,z) && players_in_game > 1 ) {
                        if ( player_count > 0 ) continue;
                    }
                    if ( players_in_game > 1 && player_count > 0 && diff(base,tot_base/player_count) >= vres/div_fact ) { // is_in_pain(player) &&
                        if ( player_count > 0 ) continue;
                    }

                    if ( y-base >= vres/div_fact || y-base < 0 ) tot_y += y;
                    else tot_y += base;
                } else {
                    tot_y += base;
                }
            } else {
                tot_y += y;
            }

            tot_base += base;

            if ( zcam_flag != NULL() && zcam_flag > 0 ) {
                z -= minz;
                tot_z += z;
            }

            if ( xcam_flag != NULL() && xcam_flag > 0 ) {
                x -= minz;
                tot_x += x;
            }

            ++player_count;
        } else continue;
    }
    if ( player_count <= 0 ) {
        clvp("camerazoffset",cameraz_base);
        clvp("cameraxoffset",camerax);
        return cameraz_base;
    }

    tot_y /= player_count;
    cameraz_base -= tot_y;

    if ( zcam_flag != NULL() && zcam_flag > 0 ) {
        //if ( player_count <= 1 ) {
            tot_z /= player_count;
            cameraz_base += tot_z;
        //}
    }
    if ( xcam_flag != NULL() && xcam_flag > 0 ) {
        //if ( player_count <= 1 ) {
            tot_x /= player_count;
            camerax += tot_x;
            camerax = truncA(camerax);
        //}
    }
    cameraz_base = truncA(cameraz_base);
    //cv("ypos",tot_z+minz+cameraz_base-tot_y); // alternative

    clvp("camerazoffset",cameraz_base);
    clvp("cameraxoffset",camerax);

    /*drawstring(10,50,0,"cameraz: "+cameraz_base);
    drawstring(10,50+10,0,"tot_y: "+tot_y);
    drawstring(10,50+20,0,"tot_z: "+tot_z);
    drawstring(10,50+30,0,"ypos: "+ypos);*/

    return cameraz;
}

void set_min_level_bound(float scroll_pos, float scroll_speed, void localvar_name) {
	// Trova xmin di un player esistente, poi sottrai a xmin hres/2.
	if ( openborvariant("xpos") > scroll_pos ) { if ( getlocalvar(localvar_name) != 1 ) setlocalvar(localvar_name,1); }
	if ( getlocalvar(localvar_name) == 1 && openborvariant("xpos") < scroll_pos && getlevelproperty("scrollspeed") != 0 ) {
		float xmin = NULL();
		int hres = openborvariant("hresolution");
		int i;

		for (i = 0; i < MAX_PLAYERS; ++i) {
			void player = getplayerproperty(i, "entity");

			if ( !getentityproperty(player, "exists") ) continue;
			if ( getentityproperty(player, "x") < xmin || xmin == NULL() ) xmin = getentityproperty(player, "x") ;
		}
		if ( xmin < scroll_pos+(hres/2) ) changelevelproperty("scrollspeed",0);
	} else if ( getlocalvar(localvar_name) == 1 && openborvariant("xpos") <= scroll_pos && getlevelproperty("scrollspeed") != scroll_speed ) {
		float xmax = NULL();
		int hres = openborvariant("hresolution");
		int i;

		for (i = 0; i < MAX_PLAYERS; ++i) {
			void player = getplayerproperty(i, "entity");

			if ( !getentityproperty(player, "exists") ) continue;
			if ( getentityproperty(player, "x") > xmax || xmax == NULL() ) xmax = getentityproperty(player, "x") ;
		}

		if ( xmax >= scroll_pos+(hres/2) ) changelevelproperty("scrollspeed",scroll_speed);
	}
}

void set_max_level_bound(float scroll_pos, float scroll_speed, void localvar_name) {
	// Trova xmin di un player esistente, poi sottrai a xmin hres/2.
	if ( openborvariant("xpos") > scroll_pos ) { if ( getlocalvar(localvar_name) != 1 ) setlocalvar(localvar_name,1); }
	if ( getlocalvar(localvar_name) == 1 && openborvariant("xpos") > scroll_pos && getlevelproperty("scrollspeed") != 0 ) {
		float xmax = NULL();
		int hres = openborvariant("hresolution");
		int i;

		for (i = 0; i < MAX_PLAYERS; ++i) {
			void player = getplayerproperty(i, "entity");

			if ( !getentityproperty(player, "exists") ) continue;
			if ( getentityproperty(player, "x") > xmax || xmax == NULL() ) xmax = getentityproperty(player, "x") ;
		}

		if ( xmax >= scroll_pos+(hres/2) ) changelevelproperty("scrollspeed",0);
	} else if ( getlocalvar(localvar_name) == 1 && openborvariant("xpos") >= scroll_pos && getlevelproperty("scrollspeed") != scroll_speed ) {
		float xmin = NULL();
		int hres = openborvariant("hresolution");
		int i;

		for (i = 0; i < MAX_PLAYERS; ++i) {
			void player = getplayerproperty(i, "entity");

			if ( !getentityproperty(player, "exists") ) continue;
			if ( getentityproperty(player, "x") < xmin || xmin == NULL() ) xmin = getentityproperty(player, "x") ;
		}
		if ( xmin < scroll_pos+(hres/2) ) changelevelproperty("scrollspeed",scroll_speed);
	}
}

void change_basemap(int map_index, float rx, float rz, float x_size, float z_size, float min_y, float max_y, int x_cont) {
    float x,z;
	float delta,y,tmp;
	int dir = 0;

	changelevelproperty("basemap", map_index, "x", rx);
	changelevelproperty("basemap", map_index, "xsize", x_size+2);
	changelevelproperty("basemap", map_index, "z", rz);
	changelevelproperty("basemap", map_index, "zsize", z_size+2);

    if (min_y <= max_y) dir = 1;
    else
    {
        dir = 0;
        tmp = min_y;
        min_y = max_y;
        max_y = tmp;
    }

	delta = (max_y - min_y) / ( (x_size <= 0) ? 1 : (x_size-1) );

	for( x = 0; x < x_size; x++) {
		if ( dir == NULL() || dir > 0 ) {
            if ( x == x_size-1 ) y = max_y;
            else y = x*delta + min_y;
		} else y = max_y - (x*delta); // + min_a

		if ( x_cont != NULL() ) {
            if ( dir == NULL() || dir > 0 ) {
                if ( x+rx >= x_cont ) y = max_y; // connect with the wall more smoothly
            } else {
                if ( x+rz <= x_cont ) y = max_y;
            }
		}

		for ( z = 0; z < z_size; z++) {
			changelevelproperty("basemap", map_index, "map", x, z, y);
		}
	}
}

void check_t_walkoff(void ent, float t_walkoff) {
        if ( getentityproperty(ent, "exists") ) {
            int type = getentityproperty(ent,"type");

			if ( type == openborconstant("TYPE_ENEMY") || type == openborconstant("TYPE_PLAYER") || type == openborconstant("TYPE_NPC")
                || type == openborconstant("TYPE_NONE") || type == openborconstant("TYPE_OBSTACLE") || type == openborconstant("TYPE_TRAP") ) {
                if ( !is_in_pain(ent) && !getentityproperty(ent,"noaicontrol") ) {
                    float y = getentityproperty(ent,"y");
                    float base = getentityproperty(ent,"base");

                    if ( y > base && y-base <= t_walkoff ) changeentityproperty(ent,"position",NULL(),NULL(),base);
                }
            }
        }

    return;
}

void check_zpos_platform_in_level(void ent) {
        if ( getentityproperty(ent, "exists") ) {
            int type = getentityproperty(ent,"type");

			if ( type == openborconstant("TYPE_ENEMY") || type == openborconstant("TYPE_PLAYER") || type == openborconstant("TYPE_NPC")
                || type == openborconstant("TYPE_NONE") || type == openborconstant("TYPE_OBSTACLE") || type == openborconstant("TYPE_TRAP") ) {

                check_zpos_platform(ent);
			}
        }

    return;
}

void check_zpos_platform(void self) {
    float x = gep(self,"x");
    float z = gep(self,"z");
    float y = gep(self,"y");
    //float base = getentityproperty(self,"base");
    int layer = gep(self,"setlayer");
    int base_layer = MAX_ONPLATFORM_LAYER;
    //void platform = get_platform_on(ent,2.0);
    void platform = NULL();

    if ( gep(self,"subject_to_platform") ) platform = get_platform_in_range(self);
    else platform = checkplatform(x,z,y+T_GENERIC_WALKOFF);

    if ( ent_exists(platform) ) {
        if ( gev(platform,"big_platform") > 0 ) {
            int new_layer = base_layer+truncA(z);

            //drawstring(10,50,0,"is_on");
            if ( layer != new_layer ) {
                if ( gev(self,"layer_level") == NULL() ) sev(self,"layer_level",layer);
                cep(self,"setlayer",new_layer);
            }
        }
    } else if ( platform == NULL() ) {
        int prev_layer = gev(self,"layer_level");

        if ( prev_layer != NULL() && layer != prev_layer && layer < MAX_OVERFOREGROUND_LAYER ) cep(self,"setlayer",prev_layer);
    }

    return;
}

void check_shadow(void ent, int transp, float xshift, float yshift, int scalex, int scaley, int shiftx, int base_flag, int black_flag, int altitude_flag) {
        if ( getentityproperty(ent, "exists") ) {
            int type = getentityproperty(ent,"type");
            int subject_to_c_shadow = getentityvar(ent,"subject_to_custom_shadow");

			if ( type == openborconstant("TYPE_ENEMY") || type == openborconstant("TYPE_PLAYER") || type == openborconstant("TYPE_NPC")
                || type == openborconstant("TYPE_NONE") || type == openborconstant("TYPE_OBSTACLE") || type == openborconstant("TYPE_TRAP") ) {
                if ( subject_to_c_shadow == NULL() ) subject_to_c_shadow = 1;
                if ( !subject_to_c_shadow ) return;

				if ( getentityproperty(ent,"gfxshadow") || getentityvar(ent,"gfxshadow_level") ) {
					draw_shadow(ent,transp,xshift,yshift, scalex, scaley, shiftx, base_flag, black_flag, altitude_flag);
					if ( !getentityvar(ent,"gfxshadow_level") ) setentityvar(ent,"gfxshadow_level",1);
				}
				changeentityproperty(ent,"gfxshadow",0);
			}
        }

    return;
}

int draw_shadow(void self, int transp, float xshift, float yshift, int scalex, int scaley, int shiftx, int base_flag, int black_flag, int altitude_flag) {
    if ( openborvariant("in_level") && !openborvariant("game_paused") ) { // !ov("game_paused") is to avoid overlapping bug
        void spr = getentityproperty(self, "sprite");
        float x = getentityvar(self, "x");
        float z = getentityvar(self, "z");
        float a = getentityvar(self, "y");
        float base = getentityvar(self, "base");
        int facing = getentityvar(self, "direction");
        int layer = getentityproperty(self, "setlayer");
        //int sortid = oc("MIN_INT");
        //float z_min = 31;
        float xpos = openborvariant("xpos");
        float ypos = openborvariant("ypos");
        int colourmap = getentityproperty(self, "colourmap");
        float new_y;
        int tintmode = 1, alpha = 6;
        //float transp = 40; //255
        int shadow_color = rgbcolor(0x00,0x00,0x08);
        int black_color = rgbcolor(0x00,0x00,0x00);

        if ( transp > 254 ) transp = 254;
        else if ( transp < 0 ) transp = 0;
        if ( scalex == NULL() ) scalex = 256;
        if ( scaley == NULL() ) scaley = 50;
        if ( shiftx == NULL() ) shiftx = 220;
        if ( altitude_flag == NULL() ) altitude_flag = 0;

        if ( black_flag > 0 ) {
            tintmode = 2;
            alpha = 2;
            shadow_color = black_color;
        }

        // && is_on_platform(self)
        if ( layer > 0 ) {
            if ( is_on_platform_in_range(self) ) layer = MAX_ONPLATFORM_LAYER; // trick for shadown on platform
            else if ( altitude_flag ) layer = 0;
        }
        //else layer = -1;

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

            //changedrawmethod(NULL(), "tintmode", tintmode);
            //changedrawmethod(NULL(), "fillcolor", shadow_color); // tintcolor
            //changedrawmethod(NULL(), "tintcolor", shadow_color);

            changedrawmethod(NULL(), "alpha", alpha);
            changedrawmethod(NULL(), "channelg", transp);
            changedrawmethod(NULL(), "channelr", transp);
            changedrawmethod(NULL(), "channelb", transp);

            //changedrawmethod(NULL(), "scalex", 255);
            //changedrawmethod(NULL(), "scaley", 50);
            //changedrawmethod(NULL(), "shiftx", 100);

            // (entity, int flag, int scalex, int scaley, int flipx, int flipy, int shiftx, int alpha, int remap, int fillcolor, int rotate, int fliprotate, int transparencybg, void* colourmap, int centerx, int centery);
            setdrawmethod(NULL(), 1, scalex, scaley, facing, 0, shiftx, alpha, 1, shadow_color, 0, 0, 0, colourmap); // map: -1 = Use entity's colormap.

            new_y = z-a-ypos-4 + (a-base) + yshift;
            if ( base_flag == NULL() || base_flag == 0 ) new_y -= (a-base)/4;
            //z += 6;
            //z += 43; // on platforms

            if ( altitude_flag ) drawsprite(spr, x-xpos + xshift, new_y, layer+z-1, layer);
            else drawsprite(spr, x-xpos + xshift, new_y, layer+z-6, layer);

            changedrawmethod(NULL(), "enabled", 0);
            changedrawmethod(NULL(), "reset", 1);
            setdrawmethod(NULL(), 0);
        }
    }
}

int check_go_arrow(float x1, float x2, char arrow_dir, float z1, float z2, float wait_time) {
    float xpos = openborvariant("xpos");

	if ( xpos >= x1 && xpos <= x2 ) {
        //int game_speed = openborvariant("game_speed");
        float ypos = openborvariant("ypos");
        int hres = openborvariant("hresolution");
        //int vres = openborvariant("vresolution");
        void GO_ARROW = getglobalvar("go_arrow");
        float ETA = 1.0; // secs

        if ( wait_time != NULL() ) ETA = wait_time;

	    //drawstring( 10,200,0,"time:  "+openborvariant("elapsed_time"));

	    if ( z1 != NULL() || z2 != NULL() ) {
            if ( ypos < z1 || ypos > z2 ) {
                return 0;
            }
	    }

	    if ( GO_ARROW == NULL() ) {
            if ( count_enemies() <= 0 ) changeopenborvariant("waiting",0); // 110 = GO; 111 = WAIT
	    }

        if ( is_waiting(ETA) && GO_ARROW == NULL() ) {
            void ent;

            ent = spawnsubentity("go_arrow",xpos+hres-50,ypos+110,0);

                 if ( arrow_dir == "RIGHT" ) performattack(ent,openborconstant("ANI_FOLLOW1"),1);
            else if ( arrow_dir == "LEFT"  ) performattack(ent,openborconstant("ANI_FOLLOW2"),1);
            else if ( arrow_dir == "DOWN"  ) performattack(ent,openborconstant("ANI_FOLLOW3"),1);
            else if ( arrow_dir == "UP"    ) performattack(ent,openborconstant("ANI_FOLLOW4"),1);

            setglobalvar("go_arrow",ent);
            setlocalvar("prev_xpos",xpos);
            setlocalvar("prev_ypos",ypos);
        } else if ( GO_ARROW != NULL() && getentityproperty(GO_ARROW,"exists") ) {
            if ( xpos != getlocalvar("prev_xpos") || ypos != getlocalvar("prev_ypos") ) {
                killentity(GO_ARROW);
                setglobalvar("go_arrow",NULL());
            }

            return 0;
        }

    }

    return 1;
}

int set_diagonal_level_bounds(void ent, float z1, float z2, float xpos1, float xpos2) {
    if ( getentityproperty(ent,"exists") ) {
        int type = getentityproperty(ent,"type");
        float x = getentityproperty(ent,"x");
        float z = getentityproperty(ent,"z");
        float x1 = 0, x2 = openborvariant("hresolution");
        float rx = x, z3 = 0;

        if ( xpos1 != NULL() && xpos2 != NULL() ) {
            if ( x < xpos1 || x > xpos2 ) return 0;
            else {
                x2 = diff(xpos2,xpos1);
                rx -= xpos1;
            }
        }

        //z1 = 500; z2 = 600;
        //&& type != openborconstant("TYPE_NONE")
        if ( type != openborconstant("TYPE_PLAYER") && type != openborconstant("TYPE_ENEMY") && type != openborconstant("TYPE_NPC") ) return 0;
        if ( getentityproperty(ent,"noaicontrol") ) return 0;
        if ( !getentityproperty(ent,"subject_to_wall") ) return 0;

        //x:h=320:(600-500) -> x:h=320:100 -> (x*100)/320 = h
        z3 = ((z2-z1)*rx)/(x2-x1);
        z3 += z1;

        if ( z <= z3 ) changeentityproperty(ent,"position",NULL(),z3+0.1,NULL());
        //drawstring( 10,100,0,"x3: "+z3);

        return 1;
    }

    return 0;
}
