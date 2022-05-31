
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_data.c"

int get_sprite_intime(void sprite_array, float delay) {
    int time = ov("elapsed_time");
    int game_speed = ov("game_speed");
    int refresh_time = truncA(game_speed*delay);
    int tot_time = refresh_time*(size(sprite_array));
    int index = 0;

    index = time%tot_time;
    index /= refresh_time;

    //drawstring(10,50,0,"index:  "+index);

    return index;
}

char draw_fontstring(int x, int y, int z, int sortid, char str, char font_path, int wfont, int hfont, char mem_var, int reset_flag, void screen, int monospace_flag, int space_pixel, char complete_font_path, char mem_var2) {
    int i = 0;
    int wchar = wfont/16;
    int hchar = hfont/16;
    int down_space = 0, xshift = 0, escape_n = 0;

    slv("__prev_dx_space",NULL());
    slv("__prev_sx_space",NULL());

    if ( sortid == NULL() ) sortid = 0;
    if ( z == NULL() ) z = 2000;

    str += ""; // format to string
    for ( i = 0; i < strlength(str); ++i ) {
        char chara = getchar(str,i);
        int chara_int = byte_to_int(chara);

        if ( chara == "\n" ) {
            down_space += hchar;
            xshift = (i-escape_n)*wchar;
            ++escape_n;
        }

        draw_fontchar(x + i*wchar - xshift,y+down_space,z,sortid,font_path,wfont,hfont,chara_int,mem_var,reset_flag,screen,monospace_flag,space_pixel,chara,complete_font_path,mem_var2);

        //drawstring(x+i*wchar,y,0,chara);
        //drawstring(x,y+i*hchar,0,reset_flag);
    }
    slv("__prev_dx_space",NULL());
    slv("__prev_sx_space",NULL());
}

char draw_fontchar(int x, int y, int z, int sortid, char font_path, int wfont, int hfont, int value, char mem_var, int reset_flag, void screen, int monospace_flag, int space_pixel, char chara, char complete_font_path, char mem_var2) {
    void font = getlocalvar(mem_var);
    void complete_font = NULL();

    if ( mem_var2 != NULL() ) complete_font = getlocalvar(mem_var2);

    if ( font == NULL() ) { font = loadsprite(font_path); setlocalvar(mem_var,font); font = getlocalvar(mem_var); }
    if ( complete_font == NULL() && complete_font_path != NULL() ) { complete_font = loadsprite(complete_font_path); setlocalvar(mem_var2,complete_font); complete_font = getlocalvar(mem_var2); }
    if ( reset_flag > 0 ) {
        if ( font ) { free(font); setlocalvar(mem_var,NULL()); }
        if ( complete_font ) { free(complete_font); setlocalvar(mem_var2,NULL()); }
        return("");
    }
    if ( font ) {
        //int wfont = getgfxproperty(font, "srcwidth");
        //int hfont = getgfxproperty(font, "srcheight");
        int wchar = wfont/16;
        int hchar = hfont/16;
        int row = 0, column = 0;
        int sx_space = 0, dx_space = 0;
        int xcoord, ycoord, prev_sx_space = 0, prev_dx_space = 0;

        row = value%16;
        column = truncA(value/16);
        xcoord = row*wchar;
        ycoord = column*hchar;

        //changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        // shift the single chara to (0,0) coords. then shift it to x,y coords
        x = 0 - xcoord + x;
        y = 0 - ycoord + y;

        // ### MONOSPACE ###
        if ( monospace_flag != NULL() && monospace_flag > 0 ) {
            int i1 = 0, i2 = 0;
            int tmp_space = wchar;
            int space_flag = 0;

            // scan sx -> dx
            i1 = 0; i2 = 0; tmp_space = wchar;
            for ( i2 = ycoord; i2 < ycoord+hchar; ++i2 ) {
                sx_space = 0;
                for ( i1 = xcoord; i1 < xcoord+wchar; ++i1 ) {
                    int pixel;

                    if ( complete_font ) pixel = getgfxproperty(complete_font,"pixel",i1,i2);
                    else pixel = getgfxproperty(font,"pixel",i1,i2);
                    if ( pixel == 0 ) ++sx_space;
                    else break;
                }
                if ( sx_space < tmp_space ) tmp_space = sx_space;
                if ( tmp_space == 0 ) break;
            }
            if ( tmp_space != wchar ) sx_space = tmp_space;
            else { sx_space = 0; space_flag = 1; }

            // scan dx -> sx
            i1 = 0; i2 = 0; tmp_space = wchar;
            for ( i2 = ycoord; i2 < ycoord+hchar; ++i2 ) {
                dx_space = 0;
                for ( i1 = xcoord+wchar-1; i1 >= xcoord; --i1 ) {
                    int pixel;

                    if ( complete_font ) pixel = getgfxproperty(complete_font,"pixel",i1,i2);
                    else pixel = getgfxproperty(font,"pixel",i1,i2);
                    if ( pixel == 0 ) ++dx_space;
                    else break;
                }
                if ( dx_space < tmp_space ) tmp_space = dx_space;
                if ( tmp_space == 0 ) break;
            }
            if ( tmp_space != wchar ) dx_space = tmp_space;
            else { dx_space = 0; space_flag = 1; }

            if ( space_flag == 1 ) {
                sx_space = 0;
                if ( space_pixel == NULL() ) space_pixel = 2;
                dx_space = wchar-space_pixel;
                wchar = space_pixel;
            }

            if ( glv("__prev_dx_space") == NULL() ) slv("__prev_dx_space",0);
            prev_dx_space = glv("__prev_dx_space");
            slv("__prev_dx_space",glv("__prev_dx_space")+dx_space);

            if ( glv("__prev_sx_space") == NULL() ) slv("__prev_sx_space",0);
            prev_sx_space = glv("__prev_sx_space");
            slv("__prev_sx_space",glv("__prev_sx_space")+sx_space);

            /*drawstring(20,50,0,"sx_space: "+sx_space);
            drawstring(20,60,0,"dx_space: "+dx_space);
            drawstring(20,70,0,"xcoord: "+xcoord);
            drawstring(20,80,0,"ycoord: "+ycoord);
            drawstring(20,90,0,"pixel: "+getgfxproperty(font,"pixel",0,16));
            drawstring(20,100,0,"space: "+wchar);*/
        }
        // ### MONOSPACE ###

        changedrawmethod(NULL(), "clipx", xcoord + x-prev_dx_space-prev_sx_space);
        changedrawmethod(NULL(), "clipy", ycoord + y);
        changedrawmethod(NULL(), "cliph", hchar);
        changedrawmethod(NULL(), "clipw", wchar);

        //drawstring(20,20,0, getgfxproperty(font,"pixel",6,0));

        if ( screen == NULL() ) drawsprite(font, x-prev_dx_space-prev_sx_space, y, z, sortid);
        else drawspritetoscreen(font, screen, x-prev_dx_space-prev_sx_space, y);

        //changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);

        //drawstring(10,200,0,wfont+" - "+hfont);
        //drawstring(10,210,0,wchar+" - "+hchar);
        //drawstring(10,220,0,row+" - "+column);

        //free(font);
    }

    return ("");
}

int fontstring_width(char str, char font_path, int wfont, int hfont, char mem_var, int monospace_flag, int space_pixel) {
    int i = 0;
    int font_width = 0;

    str += ""; // format to string
    for ( i = 0; i < strlength(str); ++i ) {
        char chara = getchar(str,i);
        int chara_int = byte_to_int(chara);

        font_width += fontchar_width(chara,font_path,wfont,hfont,chara_int,mem_var,monospace_flag,space_pixel);
    }

    return font_width;
}

int fontchar_width(char chara, char font_path, int wfont, int hfont, int value, char mem_var, int monospace_flag, int space_pixel) {
    int font_width = 0;

    void font = getlocalvar(mem_var);

    if ( font == NULL() ) { font = loadsprite(font_path); setlocalvar(mem_var,font); font = getlocalvar(mem_var); }
    if ( font ) {
        //int wfont = getgfxproperty(font, "srcwidth");
        //int hfont = getgfxproperty(font, "srcheight");
        int wchar = wfont/16;
        int hchar = hfont/16;
        int row = 0, column = 0;
        int sx_space = 0, dx_space = 0;
        int xcoord, ycoord, prev_sx_space = 0, prev_dx_space = 0;
        int original_wchar = wchar, space_width = NULL();

        row = value%16;
        column = truncA(value/16);
        xcoord = row*wchar;
        ycoord = column*hchar;

        // ### MONOSPACE ###
        if ( monospace_flag != NULL() && monospace_flag > 0 ) {
            int i1 = 0, i2 = 0;
            int tmp_space = wchar;
            int space_flag = 0;

            // scan sx -> dx
            i1 = 0; i2 = 0; tmp_space = wchar;
            for ( i2 = ycoord; i2 < ycoord+hchar; ++i2 ) {
                sx_space = 0;
                for ( i1 = xcoord; i1 < xcoord+wchar; ++i1 ) {
                    int pixel;

                    pixel = getgfxproperty(font,"pixel",i1,i2);
                    if ( pixel == 0 ) ++sx_space;
                    else break;
                }
                if ( sx_space < tmp_space ) tmp_space = sx_space;
                if ( tmp_space == 0 ) break;
            }
            if ( tmp_space != wchar ) sx_space = tmp_space;
            else { sx_space = 0; space_flag = 1; }

            // scan dx -> sx
            i1 = 0; i2 = 0; tmp_space = wchar;
            for ( i2 = ycoord; i2 < ycoord+hchar; ++i2 ) {
                dx_space = 0;
                for ( i1 = xcoord+wchar-1; i1 >= xcoord; --i1 ) {
                    int pixel;

                    pixel = getgfxproperty(font,"pixel",i1,i2);
                    if ( pixel == 0 ) ++dx_space;
                    else break;
                }
                if ( dx_space < tmp_space ) tmp_space = dx_space;
                if ( tmp_space == 0 ) break;
            }
            if ( tmp_space != wchar ) dx_space = tmp_space;
            else { dx_space = 0; space_flag = 1; }

            if ( space_flag == 1 ) {
                sx_space = 0;
                if ( space_pixel == NULL() ) space_pixel = 2;
                dx_space = wchar-space_pixel;
                wchar = space_pixel;
                space_width = space_pixel;
            }
        }
        // ### MONOSPACE ###

        if ( space_width != NULL() ) {
            return space_pixel;
        }

        font_width = original_wchar-sx_space-dx_space;
    }

    return font_width;
}

void drawSprite(void logo, float x, float y, void screen) {
	if ( logo ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        if ( screen == NULL() ) drawsprite(logo, x, y, 9000);
        else drawspritetoscreen(logo, screen, x, y);

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

void drawOrizScaledRouletteBg(void sprite1, void sprite2, void sprite3, void sprite4, int width, int height, float div_width, float div_height, int direction_flag, void screen) {
	int hshift, vshift;
	int t = ov("elapsed_time");
	float refresh_time = 2.2;
	int vres = ov("vresolution");
	int hres = ov("hresolution");
	int i, i_h, x, y, z = 1, hsize = 0;
	int opacity = 100; // opacity perc
	float transp = 254*opacity/100;
	int sprite_num = 4;
	int flipx = 0, max_size = 256, h_div = 14, xshift = 2;
	float f_width = width/div_width, f_height = height/div_height, middle_scr = ((hres+width/h_div)/2);

	if ( sprite1 && sprite2 && sprite3 && sprite4 ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        changedrawmethod(NULL(), "alpha", 6);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);
        changedrawmethod(NULL(), "flipx", flipx);
        //changedrawmethod(NULL(), "scalex", max_size);
        changedrawmethod(NULL(), "scaley", max_size);
        //changedrawmethod(NULL(), "centerx", (width/2));
        //changedrawmethod(NULL(), "centery", (height/2));

        if ( direction_flag == NULL() || direction_flag == 0 ) direction_flag = 1;
        else direction_flag = -1;
        hshift = (t/refresh_time)%(f_width*sprite_num);
        vshift = (t/refresh_time)%(f_height*sprite_num); // in questo caso lo shifting dello sprite deve percorrere l'intera vres*sprite_num (da cima a fondo per tutti gli sprites)
        x = hres-hshift*direction_flag; // posizione
        y = vres-vshift*direction_flag;

        /*
         * Quanti sprites devo disegnare? Tanti quanti ne entrano in basso e in alto a seconda della loro altezza variabile (variano in scala).
         */
        hsize = 0; i_h = 0;
        while (hsize < hres) { // hres*sprite_num
            int new_x, space_ratio = 100, space_ratio_tot = 100;

            /*
             * Calcoliamo la percentuale di scala verticale. Da cima a metà schermo la scala va da 0% a 100%, da metà a fondo si va da 100% a 0%
             * Quindi vres/2 : 100 = new_y : x -> (vres/2)*x = 100*new_y -> x = (100*new_y)/(vres/2)
             * Poi vres/2 : 100 = new_y-vres/2 : x; da metà a fondo (da middle_scr a vres si va da 0% a 100%).
             */

            // create sx
            new_x = x-hsize;

            if ( new_x <= middle_scr ) space_ratio = (100*(new_x))/(middle_scr);
            else space_ratio = 100 - (100*(new_x-middle_scr))/(middle_scr);
            if ( space_ratio > 100 ) space_ratio = 100;
            else if ( space_ratio < 0 ) space_ratio = 0;
            space_ratio_tot = (100*new_x)/(middle_scr*2.0); if ( space_ratio_tot > 100 ) space_ratio_tot = 100; else if ( space_ratio_tot < 0 ) space_ratio_tot = 0;

            changedrawmethod(NULL(), "scalex", max_size*space_ratio/100.0);
            //changedrawmethod(NULL(), "centerx", ((width/2)*space_ratio/100));
            //changedrawmethod(NULL(), "centerx", (width/2));
            changedrawmethod(NULL(), "centerx", width*(100.0-space_ratio_tot)/100.0); // in sostanza l'ancoraggio va da height a 0 (0 nel punto più basso)

            new_x -= width/(h_div*2.0); // fix for under 0 position (see middle_scr)
            new_x += xshift;

            //if      ( i_h == 0 ) drawstring(10,20,0,"ratio: "+space_ratio+"  x: "+new_x);
            if      ( i_h%sprite_num == 0 ) { if ( screen == NULL() ) drawsprite(sprite1, new_x, (vres/2-height/2), z+0); else drawspritetoscreen(sprite1, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 1 ) { if ( screen == NULL() ) drawsprite(sprite2, new_x, (vres/2-height/2), z+1); else drawspritetoscreen(sprite2, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 2 ) { if ( screen == NULL() ) drawsprite(sprite3, new_x, (vres/2-height/2), z+2); else drawspritetoscreen(sprite3, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 3 ) { if ( screen == NULL() ) drawsprite(sprite4, new_x, (vres/2-height/2), z+3); else drawspritetoscreen(sprite4, screen, new_x, (vres/2-height/2)); }
            hsize += f_width; // frequency
            ++i_h;
        }

        hsize = f_width; i_h = 0;
        while (hsize < hres) { // hres*sprite_num
            int new_x, space_ratio = 100, space_ratio_tot = 100;

            // create dx
            new_x = x+hsize;

            if ( new_x <= middle_scr ) space_ratio = (100*(new_x))/(middle_scr);
            else space_ratio = 100 - (100*(new_x-middle_scr))/(middle_scr);
            if ( space_ratio > 100 ) space_ratio = 100;
            else if ( space_ratio < 0 ) space_ratio = 0;
            space_ratio_tot = (100*new_x)/(middle_scr*2.0); if ( space_ratio_tot > 100 ) space_ratio_tot = 100; else if ( space_ratio_tot < 0 ) space_ratio_tot = 0;

            changedrawmethod(NULL(), "scalex", max_size*space_ratio/100.0);
            //changedrawmethod(NULL(), "centerx", ((width/2)*space_ratio/100));
            //changedrawmethod(NULL(), "centerx", (width/2));
            changedrawmethod(NULL(), "centerx", width*(100.0-space_ratio_tot)/100.0);

            new_x -= width/(h_div*2.0); // fix for under 0 position (see middle_scr)
            new_x += xshift;

            //if      ( i_h == 0 ) drawstring(10,20,0,"ratio: "+space_ratio+"  x: "+new_x);
            if      ( i_h%sprite_num == 0 ) { if ( screen == NULL() ) drawsprite(sprite4, new_x, (vres/2-height/2), z+3); else drawspritetoscreen(sprite4, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 1 ) { if ( screen == NULL() ) drawsprite(sprite3, new_x, (vres/2-height/2), z+2); else drawspritetoscreen(sprite3, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 2 ) { if ( screen == NULL() ) drawsprite(sprite2, new_x, (vres/2-height/2), z+1); else drawspritetoscreen(sprite2, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 3 ) { if ( screen == NULL() ) drawsprite(sprite1, new_x, (vres/2-height/2), z+0); else drawspritetoscreen(sprite1, screen, new_x, (vres/2-height/2)); }
            hsize += f_width; // frequency
            ++i_h;
        }

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

void drawVericalScaledRouletteBg(void sprite1, void sprite2, void sprite3, void sprite4, int width, int height, float div_width, float div_height, int direction_flag, void screen) {
	int hshift, vshift;
	int t = ov("elapsed_time");
	float refresh_time = 2.2;
	int vres = ov("vresolution");
	int hres = ov("hresolution");
	int i, i_v, x, y, z = 1, vsize = 0;
	int opacity = 100; // opacity perc
	float transp = 254*opacity/100;
	int sprite_num = 4;
	int flipx = 0, max_size = 256, h_div = 14, yshift = 4;
	float f_height = height/div_height, middle_scr = ((vres+height/h_div)/2); // f_height è l'altezza dello sprite divisa per una certa quantità div_height

	if ( sprite1 && sprite2 && sprite3 && sprite4 ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        changedrawmethod(NULL(), "alpha", 6);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);
        changedrawmethod(NULL(), "flipx", flipx);
        changedrawmethod(NULL(), "scalex", max_size);
        //changedrawmethod(NULL(), "scaley", max_size);
        //changedrawmethod(NULL(), "centerx", (width/2));
        //changedrawmethod(NULL(), "centery", (height/2));

        if ( direction_flag == NULL() || direction_flag == 0 ) direction_flag = 1;
        else direction_flag = -1;
        hshift = (t/refresh_time)%(width*sprite_num);
        vshift = (t/refresh_time)%(f_height*sprite_num); // in questo caso lo shifting dello sprite deve percorrere l'intera vres*sprite_num (da cima a fondo per tutti gli sprites)
        x = hres-hshift*direction_flag; // posizione
        y = vres-vshift*direction_flag;


        /*
        if ( glv("xx") == NULL() ) slv("xx",0);if ( glv("yy") == NULL() ) slv("yy",0);if ( glv("zz") == NULL() ) slv("zz",9);
        int sr = 100, sr_tot = 100;
        //middle_scr = (vres+height)/2;
        if ( glv("yy") <= middle_scr ) sr = (100*(glv("yy")))/(middle_scr);
        else sr = 100 - (100*(glv("yy")-middle_scr))/(middle_scr);
        if ( sr > 100 ) sr = 100;
        else if ( sr < 0 ) sr = 0;
        sr_tot = (100*(glv("yy")))/(middle_scr*2);
        if ( sr_tot > 100 ) sr_tot = 100;
        else if ( sr_tot < 0 ) sr_tot = 0;

        if ( playerkeys(0,0,"moveleft") ) slv("xx",glv("xx")-1);
        if ( playerkeys(0,0,"moveright") ) slv("xx",glv("xx")+1);
        if ( playerkeys(0,0,"movedown") ) slv("yy",glv("yy")+1);
        if ( playerkeys(0,0,"moveup") ) slv("yy",glv("yy")-1);
        changedrawmethod(NULL(), "scaley", max_size*sr/100);
        //changedrawmethod(NULL(), "centery", ((height/2)));
        //changedrawmethod(NULL(), "centery", ((height/2)*(sr/100)));
        changedrawmethod(NULL(), "centery", height*(100-sr_tot)/100);
        drawsprite(sprite1, glv("xx"), glv("yy")-height/(h_div*2), glv("zz"));
        drawstring(10,20,0,"ratio: "+sr+"  y: "+glv("yy"),9999);
        drawstring(10,30,0,"sr_tot: "+sr_tot,9999);
        //return;
        */


        /*
         * Quanti sprites devo disegnare? Tanti quanti ne entrano in basso e in alto a seconda della loro altezza variabile (variano in scala).
         */
        vsize = 0; i_v = 0;
        while (vsize < vres) { // vres*sprite_num
            int new_y, space_ratio = 100, space_ratio_tot = 100;

            /*
             * Calcoliamo la percentuale di scala verticale. Da cima a metà schermo la scala va da 0% a 100%, da metà a fondo si va da 100% a 0%
             * Quindi vres/2 : 100 = new_y : x -> (vres/2)*x = 100*new_y -> x = (100*new_y)/(vres/2)
             * Poi vres/2 : 100 = new_y-vres/2 : x; da metà a fondo (da middle_scr a vres si va da 0% a 100%).
             */

            // create up
            new_y = y-vsize;

            if ( new_y <= middle_scr ) space_ratio = (100*(new_y))/(middle_scr);
            else space_ratio = 100 - (100*(new_y-middle_scr))/(middle_scr);
            if ( space_ratio > 100 ) space_ratio = 100;
            else if ( space_ratio < 0 ) space_ratio = 0;
            space_ratio_tot = (100*new_y)/(middle_scr*2.0); if ( space_ratio_tot > 100 ) space_ratio_tot = 100; else if ( space_ratio_tot < 0 ) space_ratio_tot = 0;

            changedrawmethod(NULL(), "scaley", max_size*space_ratio/100.0);
            //changedrawmethod(NULL(), "centery", ((height/2)*space_ratio/100));
            //changedrawmethod(NULL(), "centery", (height/2));
            changedrawmethod(NULL(), "centery", height*(100.0-space_ratio_tot)/100.0); // in sostanza l'ancoraggio va da height a 0 (0 nel punto più basso)

            new_y -= height/(h_div*2.0); // fix for under 0 position (see middle_scr)
            new_y += yshift;

            //if      ( i_v == 0 ) drawstring(10,20,0,"ratio: "+space_ratio+"  y: "+new_y);
            if      ( i_v%sprite_num == 0 ) { if ( screen == NULL() ) drawsprite(sprite1, (hres/2-width/2), new_y, z+0); else drawspritetoscreen(sprite1, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 1 ) { if ( screen == NULL() ) drawsprite(sprite2, (hres/2-width/2), new_y, z+1); else drawspritetoscreen(sprite2, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 2 ) { if ( screen == NULL() ) drawsprite(sprite3, (hres/2-width/2), new_y, z+2); else drawspritetoscreen(sprite3, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 3 ) { if ( screen == NULL() ) drawsprite(sprite4, (hres/2-width/2), new_y, z+3); else drawspritetoscreen(sprite4, screen, (hres/2-width/2), new_y); }
            vsize += f_height; // frequency
            ++i_v;
        }

        vsize = f_height; i_v = 0;
        while (vsize < vres) { // vres*sprite_num
            int new_y, space_ratio = 100, space_ratio_tot = 100;

            // create down
            new_y = y+vsize;

            if ( new_y <= middle_scr ) space_ratio = (100*(new_y))/(middle_scr);
            else space_ratio = 100 - (100*(new_y-middle_scr))/(middle_scr);
            if ( space_ratio > 100 ) space_ratio = 100;
            else if ( space_ratio < 0 ) space_ratio = 0;
            space_ratio_tot = (100*new_y)/(middle_scr*2.0); if ( space_ratio_tot > 100 ) space_ratio_tot = 100; else if ( space_ratio_tot < 0 ) space_ratio_tot = 0;

            changedrawmethod(NULL(), "scaley", max_size*space_ratio/100.0);
            //changedrawmethod(NULL(), "centery", ((height/2)*space_ratio/100));
            //changedrawmethod(NULL(), "centery", (height/2));
            changedrawmethod(NULL(), "centery", height*(100.0-space_ratio_tot)/100.0);

            new_y -= height/(h_div*2.0); // fix for under 0 position (see middle_scr)
            new_y += yshift;

            //if      ( i_v == 0 ) drawstring(10,20,0,"ratio: "+space_ratio+"  y: "+new_y);
            if      ( i_v%sprite_num == 0 ) { if ( screen == NULL() ) drawsprite(sprite4, (hres/2-width/2), new_y, z+3); else drawspritetoscreen(sprite4, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 1 ) { if ( screen == NULL() ) drawsprite(sprite3, (hres/2-width/2), new_y, z+2); else drawspritetoscreen(sprite3, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 2 ) { if ( screen == NULL() ) drawsprite(sprite2, (hres/2-width/2), new_y, z+1); else drawspritetoscreen(sprite2, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 3 ) { if ( screen == NULL() ) drawsprite(sprite1, (hres/2-width/2), new_y, z+0); else drawspritetoscreen(sprite1, screen, (hres/2-width/2), new_y); }
            vsize += f_height; // frequency
            ++i_v;
        }

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

void drawOrizontalRouletteBg(void sprite1, void sprite2, void sprite3, void sprite4, int width, int height, int direction_flag, void screen) {
	int hshift, vshift;
	int t = ov("elapsed_time");
	float refresh_time = 2.2;
	int vres = ov("vresolution");
	int hres = ov("hresolution");
	int i, i_h, x, y, z = 1;
	int opacity = 100; // opacity perc
	float transp = 254*opacity/100;
	int sprite_num = 4;

	if ( sprite1 && sprite2 && sprite3 && sprite4 ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        changedrawmethod(NULL(), "alpha", 6);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);

        if ( direction_flag == NULL() || direction_flag == 0 ) direction_flag = 1;
        else direction_flag = -1;
        hshift = (t/refresh_time)%(width*sprite_num);
        vshift = (t/refresh_time)%(height*sprite_num); // in questo caso lo shifting dello sprite deve percorrere l'intera vres*sprite_num (da cima a fondo per tutti gli sprites)
        x = hres-hshift*direction_flag; // posizione
        y = vres-vshift*direction_flag;

        /*
         * Quanti sprites devo disegnare? Tanti quanti ne entrano in basso e in alto a seconda della loro altezza variabile (variano in scala).
         */
        for (i_h = 0, i_h < (hres/(width)*sprite_num+sprite_num); ++i_h) {
            int new_x;

            // create up
            new_x = x-(i_h*width);
            if      ( i_h%sprite_num == 0 ) { if ( screen == NULL() ) drawsprite(sprite1, new_x, (vres/2-height/2), z+0); else drawspritetoscreen(sprite1, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 1 ) { if ( screen == NULL() ) drawsprite(sprite2, new_x, (vres/2-height/2), z+1); else drawspritetoscreen(sprite2, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 2 ) { if ( screen == NULL() ) drawsprite(sprite3, new_x, (vres/2-height/2), z+2); else drawspritetoscreen(sprite3, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 3 ) { if ( screen == NULL() ) drawsprite(sprite4, new_x, (vres/2-height/2), z+3); else drawspritetoscreen(sprite4, screen, new_x, (vres/2-height/2)); }

            // create down
            new_x = x+((i_h+1)*width);
            if      ( i_h%sprite_num == 0 ) { if ( screen == NULL() ) drawsprite(sprite4, new_x, (vres/2-height/2), z+3); else drawspritetoscreen(sprite4, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 1 ) { if ( screen == NULL() ) drawsprite(sprite3, new_x, (vres/2-height/2), z+2); else drawspritetoscreen(sprite3, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 2 ) { if ( screen == NULL() ) drawsprite(sprite2, new_x, (vres/2-height/2), z+1); else drawspritetoscreen(sprite2, screen, new_x, (vres/2-height/2)); }
            else if ( i_h%sprite_num == 3 ) { if ( screen == NULL() ) drawsprite(sprite1, new_x, (vres/2-height/2), z+0); else drawspritetoscreen(sprite1, screen, new_x, (vres/2-height/2)); }
        }

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

void drawVericalRouletteBg(void sprite1, void sprite2, void sprite3, void sprite4, int width, int height, int direction_flag, void screen) {
	int hshift, vshift;
	int t = ov("elapsed_time");
	float refresh_time = 2.2;
	int vres = ov("vresolution");
	int hres = ov("hresolution");
	int i, i_v, x, y, z = 1;
	int opacity = 100; // opacity perc
	float transp = 254*opacity/100;
	int sprite_num = 4;

	if ( sprite1 && sprite2 && sprite3 && sprite4 ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        changedrawmethod(NULL(), "alpha", 6);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);

        if ( direction_flag == NULL() || direction_flag == 0 ) direction_flag = 1;
        else direction_flag = -1;
        hshift = (t/refresh_time)%(width*sprite_num);
        vshift = (t/refresh_time)%(height*sprite_num); // in questo caso lo shifting dello sprite deve percorrere l'intera vres*sprite_num (da cima a fondo per tutti gli sprites)
        x = hres-hshift*direction_flag; // posizione
        y = vres-vshift*direction_flag;

        /*
         * Quanti sprites devo disegnare? Tanti quanti ne entrano in basso e in alto a seconda della loro altezza variabile (variano in scala).
         */
        for (i_v = 0, i_v < (vres/(height)*sprite_num+sprite_num); ++i_v) {
            int new_y;

            // create up
            new_y = y-(i_v*height);
            if      ( i_v%sprite_num == 0 ) { if ( screen == NULL() ) drawsprite(sprite1, (hres/2-width/2), new_y, z+0); else drawspritetoscreen(sprite1, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 1 ) { if ( screen == NULL() ) drawsprite(sprite2, (hres/2-width/2), new_y, z+1); else drawspritetoscreen(sprite2, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 2 ) { if ( screen == NULL() ) drawsprite(sprite3, (hres/2-width/2), new_y, z+2); else drawspritetoscreen(sprite3, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 3 ) { if ( screen == NULL() ) drawsprite(sprite4, (hres/2-width/2), new_y, z+3); else drawspritetoscreen(sprite4, screen, (hres/2-width/2), new_y); }

            // create down
            new_y = y+((i_v+1)*height);
            if      ( i_v%sprite_num == 0 ) { if ( screen == NULL() ) drawsprite(sprite4, (hres/2-width/2), new_y, z+3); else drawspritetoscreen(sprite4, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 1 ) { if ( screen == NULL() ) drawsprite(sprite3, (hres/2-width/2), new_y, z+2); else drawspritetoscreen(sprite3, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 2 ) { if ( screen == NULL() ) drawsprite(sprite2, (hres/2-width/2), new_y, z+1); else drawspritetoscreen(sprite2, screen, (hres/2-width/2), new_y); }
            else if ( i_v%sprite_num == 3 ) { if ( screen == NULL() ) drawsprite(sprite1, (hres/2-width/2), new_y, z+0); else drawspritetoscreen(sprite1, screen, (hres/2-width/2), new_y); }
        }

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

void drawOrizShiftBg(void logobg, int width, int height, int fix_y, int direction_flag, int forced_z, int sortid, void screen) {
	int hshift, vshift;
	int t = ov("elapsed_time");
	float refresh_time = 2.2;
	int vres = ov("vresolution");
	int hres = ov("hresolution");
	int i, i_s, i_d, x, y, z = 1; // i_s == i di sx e rispettivamente i_d i di destra
	int t_fix = 1; // Soglia di aggiustamento. Aggiunge un numero x di patterns
	int opacity = 100; // opacity perc
	float transp = 254*opacity/100;
    int horiz_num = 0, vert_num = 0;

	if ( logobg ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        changedrawmethod(NULL(), "alpha", 6);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);

        if ( forced_z != NULL() ) z = forced_z;
        if ( sortid == NULL() ) sortid = 0;
        if ( direction_flag == NULL() || direction_flag == 0 ) direction_flag = 1;
        else direction_flag = -1;
        hshift = (t/refresh_time)%(width);
        vshift = (t/refresh_time)%(height);
        x = hres-hshift*direction_flag; // posizione
        y = vres-vshift*direction_flag;

        //drawsprite(logobg, x, fix_y, z, 9999);

        horiz_num = truncA(x/width) + (x%width > 0)+t_fix;
        vert_num = truncA(y/height) + (y%height > 0)+t_fix;
        for (i_s = 0, i_s < horiz_num; ++i_s) {
            int new_x = x-(i_s*width);

            if ( screen == NULL() ) drawsprite(logobg, new_x, fix_y, z, sortid);
            else drawspritetoscreen(logobg, screen, new_x, fix_y);
        }
        /*for (i_d = 0, i_d < ((hres-x+width)/width)+t_fix; ++i_d) {
            int new_x = x+(i_d*width);

            drawsprite(logobg, new_x, fix_y, z);
        }*/

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

void drawOrizShiftBgAlpha(void logobg, int width, int height, int fix_y, int direction_flag, int forced_z, int sortid, void screen, int alpha, int opacity, float speed) {
	int hshift, vshift;
	int t = ov("elapsed_time");
	float refresh_time = speed;
	int vres = ov("vresolution");
	int hres = ov("hresolution");
	int i, i_s, i_d, x, y, z = 1; // i_s == i di sx e rispettivamente i_d i di destra
	int t_fix = 1; // Soglia di aggiustamento. Aggiunge un numero x di patterns
	//int opacity = 100; // opacity perc
	float transp = 254*opacity/100;
    int horiz_num = 0;
    //int vert_num = 0;

	if ( logobg ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);

        changedrawmethod(NULL(), "alpha", alpha);
        changedrawmethod(NULL(), "channelg", transp);
        changedrawmethod(NULL(), "channelr", transp);
        changedrawmethod(NULL(), "channelb", transp);

        if ( refresh_time == 0 ) refresh_time = t;

        if ( forced_z != NULL() ) z = forced_z;
        if ( sortid == NULL() ) sortid = 0;
        if ( direction_flag == NULL() || direction_flag == 0 ) direction_flag = 1;
        else direction_flag = -1;
        hshift = (t/refresh_time)%(width);
        vshift = (t/refresh_time)%(height);
        x = hres-hshift*direction_flag; // posizione
        y = vres-vshift*direction_flag;

        //drawsprite(logobg, x, fix_y, z, 9999);
        horiz_num = truncA(x/width) + (x%width > 0)+t_fix;
        //vert_num = truncA(y/height) + (y%height > 0)+t_fix;
        for (i_s = 0, i_s < horiz_num; ++i_s) {
            int new_x = x-(i_s*width);

            if ( screen == NULL() ) drawsprite(logobg, new_x, fix_y, z, sortid);
            else drawspritetoscreen(logobg, screen, new_x, fix_y);
        }
        /*for (i_d = 0, i_d < ((hres-x+width)/width)+t_fix; ++i_d) {
            int new_x = x+(i_d*width);

            drawsprite(logobg, new_x, fix_y, z);
        }*/

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

void drawOffsetShiftBg(void logobg, int width, int height, int direction_flag, int forced_z, int sortid, void screen) {
	int hshift, vshift, shift;
	int t = openborvariant("elapsed_time");
	float refresh_time = 2.2;
	int vres = openborvariant("vresolution");
	int hres = openborvariant("hresolution");
	int i, i_s, i_d, x, y, z = 1; // i_s == i di sx e rispettivamente i_d i di destra
	int t_fix = 1; // Soglia di aggiustamento. Aggiunge un numero x di patterns
    int horiz_num = 0, vert_num = 0;
    int horiz_num = 0, vert_num = 0;

	if ( logobg ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);
        //changedrawmethod(NULL(), "centerx", width/2); // ancoraggio
        //changedrawmethod(NULL(), "centery", height/2);

        // Dobbiamo scostare il pattern da hres a 0 e da vres a 0 ricordando che l'ancoraggio e offset(0,0)
        // Poi dopo che si è oltre vres/hres si ricomincia da capo (ecco perchè il modulo)
        // Facciamo ((hres/width)*width) e non semplicemente hres per ricominciare da capo nel giusto modo
        // Ovvero non dobbiamo fare solo %hres ma dobbiamo considerare la lunghezza del pattern. cioè width finchè non ricopre la hres.

        if ( forced_z != NULL() ) z = forced_z;
        if ( sortid == NULL() ) sortid = 0;
        if ( direction_flag == NULL() || direction_flag == 0 ) direction_flag = 1;
        else direction_flag = -1;
        //hshift = (t/refresh_time)%((hres/width)*width);
        //vshift = (t/refresh_time)%((vres/height)*height);
        hshift = (t/refresh_time)%(width);
        vshift = (t/refresh_time)%(height);
        x = hres-hshift*direction_flag; // posizione
        y = vres-vshift*direction_flag;
        //drawstring(160, 120,0,x);

        /*
         * Dobbiamo ricoprire l'intero schermo SEMPRE. Quindi quanti pattern disegnamo?
         * Vediamo la riga horizzontale a sinistra dello sprite. Sappiamo che è alla coordinata x e che è lungo width.
         * Quanti ne dobbiamo disegnare per coprire tutta la parte sinistra? Dobbiamo coprire una distanza x (da 0 a x).
         * Quindi dobbiamo disegnarme x/width + qualche altro (forse 2 ) per coprire le mezze parti.
         */
        //shift = 0;

        // Occupiamoci di riempire la parte su (e poi sx/dx)
        horiz_num = truncA(x/width)  + (x%width  > 0)+t_fix;
        vert_num  = truncA(y/height) + (y%height > 0)+t_fix;
        for (i = 0, i < vert_num; ++i) {
            int new_y = y-(i*height);

            //drawsprite(logobg, x, new_y, z);
            for (i_s = 0, i_s < horiz_num; ++i_s) {
                int new_x = x-(i_s*width);

                if ( screen == NULL() ) drawsprite(logobg, new_x, new_y, z, sortid);
                else drawspritetoscreen(logobg, screen, new_x, new_y);
            }
            /*for (i_d = 0, i_d < ((hres-x+width)/width)+t_fix; ++i_d) {
                int new_x = x+(i_d*width);
                drawsprite(logobg, new_x, new_y, z);
            }*/
        }

        // Occupiamoci di riempire la parte giù (e poi sx/dx)
        /*for (i = 0, i < ((vres-y)/height)+t_fix; ++i) {
            int new_y = y+(i*height);

            //drawsprite(logobg, x, new_y, z);
            for (i_s = 0, i_s < (x/width)+t_fix; ++i_s) {
                int new_x = x-(i_s*width);
                drawsprite(logobg, new_x, new_y, z);
            }
            for (i_d = 0, i_d < ((hres-x+width)/width)+t_fix; ++i_d) {
                int new_x = x+(i_d*width);
                drawsprite(logobg, new_x, new_y, z);
            }
        }*/

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

void drawRotate(void sprite, int width, int height, int z, int sortid, int speed, int xshift, int yshift, void screen) {
	int vres = openborvariant("vresolution");
	int hres = openborvariant("hresolution");
	int shift = 0;
	int t = openborvariant("elapsed_time");
	float refresh_time = speed;
	float arc_vel;

    if ( z == NULL() ) z = 0;
    if ( sortid == NULL() ) sortid = 0;
    if ( xshift == NULL() ) xshift = 0;
    if ( yshift == NULL() ) yshift = 0;
    if ( speed == NULL() ) speed = 0;

    /*if(glv("aaa")==NULL())slv("aaa",0.1);
    slv("aaa",0.1+glv("aaa"));
    arc_vel = glv("aaa");*/

    arc_vel = (t/refresh_time);
    arc_vel = arc_vel%360 + get_mantix(arc_vel);
    if ( arc_vel > 359 ) arc_vel = (t/refresh_time) - 359;
    else if ( arc_vel < -359 ) arc_vel = (t/refresh_time) + 359;

	if ( sprite ) {
        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 1);
        changedrawmethod(NULL(), "rotate", (t/refresh_time)%360); //(t/refresh_time)%360
        changedrawmethod(NULL(), "centerx", width/2); // ancoraggio
        changedrawmethod(NULL(), "centery", height/2);
        //changedrawmethod(NULL(), "alpha", 6);
        //changedrawmethod(NULL(), "channelr", 160);
        //changedrawmethod(NULL(), "channelg", 160);
        //changedrawmethod(NULL(), "channelb", 160);

        //shift = (t/(refresh_time*2))%50;
        //if( (t/(refresh_time*2))%(50*2) >= 50 ) shift = (50-1)-shift;

        shift = 0;

        if ( screen == NULL() ) drawsprite(sprite, (hres/2)+xshift, (vres/2)-yshift, z, sortid);
        else drawspritetoscreen(sprite, screen, (hres/2)+xshift, (vres/2)-yshift);

        changedrawmethod(NULL(), "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
	}
}

