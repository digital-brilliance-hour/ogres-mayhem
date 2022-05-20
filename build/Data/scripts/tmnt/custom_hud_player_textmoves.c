
#import "data/scripts/lib.c"
#import "data/scripts/lib_drawing.c"

void get_btn(char btn) {
         if ( btn == "UP"       ) return "à";
    else if ( btn == "DOWN"     ) return "â";
    else if ( btn == "SX" || btn == "LEFT" ) return "ä";
    else if ( btn == "DX" || btn == "RIGHT" ) return "ã";
    else if ( btn == "ATK" || btn == "ATTACK" ) return "å";
    else if ( btn == "BLK" || btn == "BLOCK" ) return "ç";
    else if ( btn == "SPECIAL"  ) return "ç";
    else if ( btn == "JMP" || btn == "JUMP" ) return "é";
    else if ( btn == "EMPTY"    ) return "è";
    else if ( btn == "TAUNT"    ) return "ê";
    else if ( btn == "SEP"      ) return "               ";
    else if ( btn == "ARROW_SX" ) return "Ö";
    else if ( btn == "ARROW_DX" ) return "á";
}

void show_player_moves(void player, int width_box, int height_box) {
    //int vres = openborvariant("vresolution");
    int hres = openborvariant("hresolution");
    int dir = getentityproperty(player, "direction");
    char defaultmodel = getentityproperty(player, "defaultmodel");
    int p = getentityproperty(player, "playerindex");
    char text1 = "", FORWARD_BTN = "DX", BACKWARD_BTN = "SX";
    int xpos = 140, ypos = 152, x_f_pos = 0;
    int space_y = 10, font_width = 8, font_index = 0;
    int count = 0, max_count = 7;

    if ( !dir ) {
        FORWARD_BTN = "SX";
        BACKWARD_BTN = "DX";
    }

    if ( defaultmodel == "LEO" ) {
        int tot_pages = 2;

        text1 = "Leonardo Special Moves";
        drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

        // ---------- PAGE SETUP ----------
        setup_pages(p,tot_pages);
        // ---------- PAGE SETUP ----------

        if ( get_current_page() == 0 ) {
            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(dragon sword)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spinning sword)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spinning dash sword)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jumping spin sword)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+" , ("+get_btn("BLOCK")+") (on jump)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(sword cutter)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(up slasher)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("JUMP");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(slide sweep)",width_box,ypos+space_y*count);
            ++count;

            draw_pages(tot_pages,ypos,space_y,max_count,hres);
            ++count;
        } else {
            text1 = "hold "+get_btn("ATK")+" 3 sec.";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(dash kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK")+" , "+get_btn("ATK")+" , "+get_btn("ATK")+" ,... "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(moon slasher)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" +) "+get_btn("ATK")+" , "+get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" or "+get_btn("UP")+") + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            draw_pages(tot_pages,ypos,space_y,max_count,hres);
            ++count;
        }
    } else if ( defaultmodel == "MIKE" ) {
        int tot_pages = 2;

        text1 = "Michaelangelo Special Moves";
        drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

        // ---------- PAGE SETUP ----------
        setup_pages(p,tot_pages);
        // ---------- PAGE SETUP ----------

        if ( get_current_page() == 0 ) {
            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(speedy kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jumping kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jumping double kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("DOWN")+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jumping shell shock)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+" (on jump)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(kick snip)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK")+" , (hold) "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rolling nunchak.)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(cannon kick)",width_box,ypos+space_y*count);
            ++count;

            draw_pages(tot_pages,ypos,space_y,max_count,hres);
            ++count;
        } else {
            text1 = "hold "+get_btn("ATK")+" 3 sec.";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(power shell shock)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" , ("+get_btn("ATK")+")";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(turtle hook)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("JUMP")+" , ("+get_btn("ATK")+")";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(lariat turtle hook)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" +) "+get_btn("ATK")+" , "+get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" or "+get_btn("UP")+") + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            draw_pages(tot_pages,ypos,space_y,max_count,hres);
            ++count;
        }
    } else if ( defaultmodel == "DON" ) {
        int tot_pages = 2;

        text1 = "Donatello Special Moves";
        drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

        // ---------- PAGE SETUP ----------
        setup_pages(p,tot_pages);
        // ---------- PAGE SETUP ----------

        if ( get_current_page() == 0 ) {
            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(twirling bo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (on foot)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(slamming grab)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+" (on jump)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(whisking bo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("BLOCK")+" , (hold) "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(land whisking bo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(bo blow)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(home run)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(back swindle)",width_box,ypos+space_y*count);
            ++count;

            draw_pages(tot_pages,ypos,space_y,max_count,hres);
            ++count;
        } else {
            text1 = "hold "+get_btn("ATK")+" 3 sec.";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(nail shooter)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(up kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("JUMP");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(fresh kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" +) "+get_btn("ATK")+" , "+get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" or "+get_btn("UP")+") + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            draw_pages(tot_pages,ypos,space_y,max_count,hres);
            ++count;
        }
    } else if ( defaultmodel == "RAPH" ) {
        int tot_pages = 2;

        text1 = "Raphael Special Moves";
        drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

        // ---------- PAGE SETUP ----------
        setup_pages(p,tot_pages);
        // ---------- PAGE SETUP ----------

        if ( get_current_page() == 0 ) {
            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(turtle drill)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(slashing fan)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+" (on jump)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(pneumatic drill)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(boomerang sai)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(slide cutting)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(twin somersault)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("JUMP");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(double cut)",width_box,ypos+space_y*count);
            ++count;

            draw_pages(tot_pages,ypos,space_y,max_count,hres);
            ++count;
        } else {
            text1 = "hold "+get_btn("ATK")+" 3 sec.";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(fast pricker)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(sai cut)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" +) "+get_btn("ATK")+" , "+get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" or "+get_btn("UP")+") + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            draw_pages(tot_pages,ypos,space_y,max_count,hres);
            ++count;
        }
    } else if ( defaultmodel == "BAXTER" ) {
        char model = getentityproperty(player, "model");

        if ( model == "BAXTER" ) {
            text1 = "Baxter Stockman Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = "("+get_btn(FORWARD_BTN)+" or "+get_btn("DOWN")+") + "+get_btn("ATK")+" (on jump)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rifle shot)",width_box,ypos+space_y*count);
            ++count;

            text1 = "hold "+get_btn("DOWN")+" or "+get_btn(FORWARD_BTN)+" (in air)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rifle dir.)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(flying stance)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+" (while jumping)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(flying stance)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_fly_baxter" ) {
            text1 = "Baxter (Flying Stance) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = "("+get_btn(FORWARD_BTN)+" or "+get_btn("DOWN")+") + "+get_btn("ATK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rifle shot)",width_box,ypos+space_y*count);
            ++count;

            text1 = "hold "+get_btn("DOWN")+" or "+get_btn(FORWARD_BTN)+" (in air)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rifle dir.)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" + "+get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(land stance)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        }
    } else if ( defaultmodel == "ROCKSTEADY" ) {
        char model = getentityproperty(player, "model");

        if ( model == "ROCKSTEADY" ) {
            text1 = "Rocksteady Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(uppercut)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(shoulder run)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(zig-zag shoulder)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(counter attack)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jump kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "rockste_rifle" ) {
            text1 = "Rocksteady (Rifle) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rifle shot)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK")+" (hold)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(up rifle shot)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jump kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        }
    } else if ( defaultmodel == "BEBOP" ) {
        text1 = "Bebop Special Moves";
        drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

        text1 = get_btn("BLOCK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(shoulder run)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(kick)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn("UP")+" + "+get_btn("BLOCK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(up shoulder run)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn("DOWN")+" + "+get_btn("BLOCK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(down shoulder run)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(shocking rifle)",width_box,ypos+space_y*count);
        ++count;
    } else if ( defaultmodel == "TOKKA" ) {
        text1 = "Tokka Special Moves";
        drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

        text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(stinging slap)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn("BLOCK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(shoulder run)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(stinging half shell)",width_box,ypos+space_y*count);
        ++count;
    } else if ( defaultmodel == "RAHZAR" ) {
        text1 = "Rahzar Special Moves";
        drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

        text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(kick)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn("BLOCK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(shoulder run)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn("DOWN")+" + "+get_btn("BLOCK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(down shoulder run)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn("UP")+" + "+get_btn("BLOCK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(up shoulder run)",width_box,ypos+space_y*count);
        ++count;

        text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
        x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
        print_movename("(jumping claw)",width_box,ypos+space_y*count);
        ++count;
    } else if ( defaultmodel == "FOOT_SOLDIER" ) {
        char model = getentityproperty(player, "model");

        if ( model == "FOOT_SOLDIER" ) {
            int tot_pages = 4;

            text1 = "Foot Soldier Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            // ---------- PAGE SETUP ----------
            setup_pages(p,tot_pages);
            // ---------- PAGE SETUP ----------

            if ( get_current_page() == 0 ) {
                text1 = get_btn("UP")+" "+get_btn("DOWN")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(boomerang)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(uppercut)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(jump kick)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(low kick)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(up kick)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" "+get_btn("DOWN")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(overturned kick)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" "+get_btn("UP")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(middle kick)",width_box,ypos+space_y*count);
                ++count;

                draw_pages(tot_pages,ypos,space_y,max_count,hres);
                ++count;
            } else if ( get_current_page() == 1 ) {
                // SOMERSAULT
                text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
                ++count;
                // SOMERSAULT
                text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn(FORWARD_BTN)+" "+get_btn("DOWN")+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(dust launch)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(frontal kunai)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(jumping kunai)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(frontal dynamite)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(jumping dynamite)",width_box,ypos+space_y*count);
                ++count;

                draw_pages(tot_pages,ypos,space_y,max_count,hres);
                ++count;
            } else if ( get_current_page() == 2 ) {
                text1 = get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(shuriken)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(jumping stars)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" "+get_btn("DOWN")+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(manhole launch)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(flame launch)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("JUMP");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(snowball)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("JUMP");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(air snowball)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("ATK")+" + "+get_btn("JUMP")+" OR "+get_btn(FORWARD_BTN)+", "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(sliding kick)",width_box,ypos+space_y*count);
                ++count;

                draw_pages(tot_pages,ypos,space_y,max_count,hres);
                ++count;
            } else if ( get_current_page() == 3 ) {
                text1 = get_btn("DOWN")+" + "+get_btn("BLOCK")+" (on jump)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(air kunai)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" + "+get_btn("BLOCK")+" (on jump)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(air dynamite)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK")+" (on jump)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(air stars)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn(FORWARD_BTN)+" + "+get_btn("JUMP")+" (on jump)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(air snowball)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+", "+get_btn("UP")+", "+get_btn("UP")+" OR "+get_btn("DOWN")+", "+get_btn("DOWN")+", "+get_btn("DOWN");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(dodge)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("BLOCK")+"";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(change weapon)",width_box,ypos+space_y*count);
                ++count;

                draw_pages(tot_pages,ypos,space_y,max_count,hres);
                ++count;
            }
        } else if ( model == "player_foot_sword" ) {
            text1 = "Foot Soldier (Sword) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("UP")+" "+get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(air hi-kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(air sword stick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jump kick)",width_box,ypos+space_y*count);
            ++count;

            // SOMERSAULT
            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
            ++count;
            // SOMERSAULT
            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" +) "+get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_axe" ) {
            text1 = "Foot Soldier (Axe) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(axe air cut)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(axe hewer)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_bow" ) {
            text1 = "Foot Soldier (Bow) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(arrow)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(air arrow)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(fire air arrow)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_fan" ) {
            int tot_pages = 2;

            text1 = "Foot Soldier (Fan) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            // ---------- PAGE SETUP ----------
            setup_pages(p,tot_pages);
            // ---------- PAGE SETUP ----------

            if ( get_current_page() == 0 ) {
                text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(shocking fan)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(knocking fan)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" + "+get_btn("ATK")+" (hold)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(ice fan)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" + "+get_btn("ATK")+" (hold)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(bubble fan)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" "+get_btn("UP")+" + "+get_btn("ATK")+" (hold)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(burning fan)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" "+get_btn("DOWN")+" + "+get_btn("ATK")+" (hold)";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(clay fan)",width_box,ypos+space_y*count);
                ++count;

                draw_pages(tot_pages,ypos,space_y,max_count,hres);
                ++count;
            } else {
                // SOMERSAULT
                text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
                ++count;
                // SOMERSAULT
                text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("BLOCK")+"";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(change weapon)",width_box,ypos+space_y*count);
                ++count;

                draw_pages(tot_pages,ypos,space_y,max_count,hres);
                ++count;
            }
        } else if ( model == "player_foot_fish" ) {
            text1 = "Foot Soldier (Fish) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(air fish stick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jump kick)",width_box,ypos+space_y*count);
            ++count;

            // SOMERSAULT
            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
            ++count;
            // SOMERSAULT
            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , ("+get_btn(FORWARD_BTN)+" +) "+get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_fork" ) {
            text1 = "Foot Soldier (Fork) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(attack up)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(attack mid)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(attack down)",width_box,ypos+space_y*count);
            ++count;

            // SOMERSAULT
            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
            ++count;
            // SOMERSAULT
            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_hook" ) {
            text1 = "Foot Soldier (Hook) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(hook launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(heavy blade)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_knife" ) {
            int tot_pages = 2;

            text1 = "Foot Soldier (Knife) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            // ---------- PAGE SETUP ----------
            setup_pages(p,tot_pages);
            // ---------- PAGE SETUP ----------

            if ( get_current_page() == 0 ) {
                text1 = get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(knife stuck)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("UP")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(knife up slash)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(knife down slash)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(knife launch)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(up kick)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(jump kick)",width_box,ypos+space_y*count);
                ++count;

                draw_pages(tot_pages,ypos,space_y,max_count,hres);
                ++count;
            } else {
                // SOMERSAULT
                text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
                ++count;
                // SOMERSAULT
                text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
                ++count;

                text1 = get_btn("BLOCK")+"";
                x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
                print_movename("(change weapon)",width_box,ypos+space_y*count);
                ++count;

                draw_pages(tot_pages,ypos,space_y,max_count,hres);
                ++count;
            }
        } else if ( model == "player_foot_lasergun" ) {
            text1 = "Foot Soldier (Lasergun) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(laser shot)",width_box,ypos+space_y*count);
            ++count;

            // JUMPKICK
            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(middle kick)",width_box,ypos+space_y*count);
            ++count;

            // SOMERSAULT
            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
            ++count;
            // SOMERSAULT
            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_nunchaku" ) {
            text1 = "Foot Soldier (Nunchaku) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(air slash)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(forkful)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(nunchacku beat)",width_box,ypos+space_y*count);
            ++count;

            // SOMERSAULT
            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
            ++count;
            // SOMERSAULT
            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_rifle" ) {
            text1 = "Foot Soldier (Rifle) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rifle smack)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rifle shot)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_spear" ) {
            text1 = "Foot Soldier (Spear) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spear stuck)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spear air shore)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spear beating)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spear launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_tonfa" ) {
            text1 = "Foot Soldier (Tonfa) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK")+" (hold)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(block)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(attack up)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(attack mid)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(attack down)",width_box,ypos+space_y*count);
            ++count;

            // SOMERSAULT
            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
            ++count;
            // SOMERSAULT
            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_crescent_blade" ) {
            text1 = "Foot Soldier (Crescent B.) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(upper cut)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(thrust cut)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(double cut)",width_box,ypos+space_y*count);
            ++count;

            // JUMPKICK
            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(jump kick)",width_box,ypos+space_y*count);
            ++count;

            // SOMERSAULT
            text1 = get_btn("UP")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant wheel)",width_box,ypos+space_y*count);
            ++count;
            // SOMERSAULT
            text1 = get_btn("DOWN")+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(avoidant somersault)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK")+" , "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(combo)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_bomb" ) {
            text1 = "Foot Soldier (Bomb) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(frontal bomb)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(air bomb)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" (on jump)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(air bomb)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_frisbee" ) {
            text1 = "Foot Soldier (Frisbee) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(frontal frisbee)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(air frisbee)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_surf" ) {
            text1 = "Foot Soldier (Surf) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(surf launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" (in air)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(surf air launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_tire" ) {
            text1 = "Foot Soldier (Tire) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(tire launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" (in air)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(tire air launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_torpedo" ) {
            text1 = "Foot Soldier (Torpedo) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(torpedo launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" (in air)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(torpedo air launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_hammer" ) {
            text1 = "Foot Soldier (Hammer) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(hammer)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_junk" ) {
            text1 = "Foot Soldier (Junk) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(junk launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" (in air)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(junk air launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_snowflood" ) {
            text1 = "Foot Soldier (Snowflood) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(snowflood launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" (in air)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(snowf. air launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_foot_leadball" ) {
            text1 = "Foot Soldier (Leadball) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(leadball launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("ATK")+" (in air)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(leadball air launch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else {
            text1 = "- SECRET -";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);
        }
    } else if ( defaultmodel == "ROCK_SOLDIER" ) {
        char model = getentityproperty(player, "model");

        if ( model == "ROCK_SOLDIER" ) {
            text1 = "Rock Soldier Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(middle kick)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(uppercut)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spinning top)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(shoulder run)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn(FORWARD_BTN)+" + "+get_btn("BLOCK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(outstretched arms)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" "+get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(tremor)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(lower punch)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_rock_soldier_mg" ) {
            text1 = "Rock Soldier (Machinegun) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(bullet shot)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_rock_soldier_lasergun" ) {
            text1 = "Rock Soldier (Lasergun) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(laser shot)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_rock_soldier_hammer" ) {
            text1 = "Rock Soldier (Hammer) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn("DOWN")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(hammer)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_rock_soldier_fl" ) {
            text1 = "Rock Soldier (Flame Launcher) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(flame launcher)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_rock_soldier_electro" ) {
            text1 = "Rock Soldier (Electro) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(electric launcher)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_rock_soldier_beam" ) {
            text1 = "Rock Soldier (Beam) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spinning top)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_rock_soldier_bazooka" ) {
            text1 = "Rock Soldier (Bazooka) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(bazooka bullet)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spinning top)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else if ( model == "player_rock_soldier_rifle" ) {
            text1 = "Rock Soldier (Rifle) Special Moves";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);

            text1 = get_btn(FORWARD_BTN)+" + "+get_btn("ATK")+" (hold)";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(rifle shot)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn(FORWARD_BTN)+" "+get_btn("UP")+" + "+get_btn("ATK");
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(spinning top)",width_box,ypos+space_y*count);
            ++count;

            text1 = get_btn("BLOCK")+"";
            x_f_pos = (hres/2-(strlength(text1)*font_width)/2)-1;
			print_move(text1,-1,ypos+space_y*count);
            print_movename("(change weapon)",width_box,ypos+space_y*count);
            ++count;
        } else {
            text1 = "- SECRET -";
            drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);
        }
    } else {
        text1 = "- SECRET -";
        drawstring((hres/2-strwidth(text1, 0)/2),ypos-10,0,text1);
    }
}

void print_move(char text1, int x, int y) {
    int hres = openborvariant("hresolution");
	char font_mini = "data/sprites/font.gif", mem_font = "font_pause_move";

    draw_fontstring(hres/2+x-fontstring_width(text1,font_mini,128,128,mem_font,1)/2,y,9999,9999,text1,font_mini,128,128,mem_font,0,NULL(),1);
}

void print_movename(char text1, int width_box, int y) {
    int hres = openborvariant("hresolution");
	char font_mini = "data/sprites/special_fonts/font_mini.gif", mem_font = "font_pause_movename";
    int margin_dx = 4;

    draw_fontstring(hres/2+width_box/2-margin_dx-fontstring_width(text1,font_mini,96,96,mem_font,1),y,9999,9999,text1,font_mini,96,96,mem_font,0,NULL(),1);
}

int get_current_page() {
    return getlocalvar("pause_showmoves_page");
}

void draw_pages(int tot_pages, int ypos, int space_y, int max_count, int hres) {
    //tot_pages = 33;
    int cur_page = getlocalvar("pause_showmoves_page");
    char page_txt = " "+(cur_page+1)+"/"+tot_pages+" ";
    char page_tot_txt = " "+(tot_pages+1)+"/"+tot_pages+" ";
    char text1 = "";
    int page_txt_x = (hres/2-strwidth(page_tot_txt,0)/2)-1;
    int middle_pos = (hres/2-strwidth("/",0)/2)-1;
    int char_width = 8, max_num_space = char_width+digit_count(tot_pages)*char_width;
    int page_font_sx = 0, page_font_dx = 0;

    if ( getlocalvar("page_font_time_sx") != NULL() ) page_font_sx = 1;
    if ( getlocalvar("page_font_time_dx") != NULL() ) page_font_dx = 1;

    //drawstring(page_txt_x,ypos+space_y*max_count,0,page_txt);
    //draw_fontstring(page_txt_x,ypos+space_y*max_count,9999,9999,page_txt,"data/sprites/font.gif",128,128,"font_pause_movename",0,NULL(),0);
    drawstring(middle_pos,ypos+space_y*max_count,0,"/");
    drawstring(middle_pos+1+char_width,ypos+space_y*max_count,0,tot_pages);
    drawstring(middle_pos-1-strwidth(((cur_page+1)+""),0),ypos+space_y*max_count,0,cur_page+1);

    text1 = get_btn("ARROW_SX");
    drawstring(middle_pos-2-max_num_space,ypos+space_y*max_count,page_font_sx,text1);
    text1 = get_btn("ARROW_DX");
    drawstring(middle_pos+3+max_num_space,ypos+space_y*max_count,page_font_dx,text1);

    return;
}

void setup_pages(int p, int tot_pages) {
    int game_speed = openborvariant("game_speed");
    //int time = openborvariant("elapsed_time");
    int ticks = openborvariant("ticks");
    int pagefontETA = game_speed*0.5;
    int cur_page = getlocalvar("pause_showmoves_page");
	//int se = loadsample("data/sounds/beep2.wav");
	int se2 = loadsample("data/sounds/beep.wav");
	//int se3 = loadsample("data/sounds/cancel01.wav");

    // ---------- PAGE SETUP ----------
    if( keyp(p,"moveleft") ) {
        playsample(se2);
        if ( cur_page <= 0 ) setlocalvar("pause_showmoves_page",tot_pages-1);
        else setlocalvar("pause_showmoves_page",cur_page-1);
        setlocalvar("page_font_time_sx",ticks+pagefontETA);
    } else if ( keyp(p,"moveright") ) {
        playsample(se2);
        if ( cur_page >= tot_pages-1 ) setlocalvar("pause_showmoves_page",0);
        else setlocalvar("pause_showmoves_page",cur_page+1);
        setlocalvar("page_font_time_dx",ticks+pagefontETA);
    }

    if ( getlocalvar("page_font_time_sx") < ticks ) setlocalvar("page_font_time_sx",NULL());
    if ( getlocalvar("page_font_time_dx") < ticks ) setlocalvar("page_font_time_dx",NULL());
}

void clear_pause_fonts() {
    if ( getlocalvar("font_pause_movename") != NULL() ) {
        free(getlocalvar("font_pause_movename"));
        setlocalvar("font_pause_movename",NULL());
    }
    if ( getlocalvar("font_pause_move") != NULL() ) {
        free(getlocalvar("font_pause_move"));
        setlocalvar("font_pause_move",NULL());
    }
}
