
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

/*#import "data/scripts/custom_hud.c"
#import "data/scripts/custom_hud_game_over.c"
#import "data/scripts/custom_hud_hiscores.c"
#import "data/scripts/lib_drawing.c"
#import "data/scripts/loading.c"*/

//#import "data/scripts/var_resetter.c"

void main() {
    //int time = openborvariant("elapsed_time");

            //drawstring( 10,140,0,"Var (lasthitc): "+openborvariant("lasthitc"));
            /*if ( getentityproperty(getglobalvar("boss1"),"exists") && getentityproperty(getglobalvar("boss2"),"exists") ) {
                drawstring( 10,120,0,"Var (boss1ev): "+getentityvar(getglobalvar("boss1"),12));
                drawstring( 10,130,0,"Var (boss2ev): "+getentityvar(getglobalvar("boss2"),12));
            }
            drawstring( 10,140,0,"Var (1xfinal): "+getglobalvar("boss1_xfinal"));
            drawstring( 10,150,0,"Var (1zfinal): "+getglobalvar("boss1_zfinal"));
            drawstring( 10,160,0,"Var (1dirfinal): "+getglobalvar("boss1_dirfinal"));
            drawstring( 10,170,0,"Var (2xfinal): "+getglobalvar("boss2_xfinal"));
            drawstring( 10,180,0,"Var (2zfinal): "+getglobalvar("boss2_zfinal"));
            drawstring( 10,190,0,"Var (2dirfinal): "+getglobalvar("boss2_dirfinal"));*/
            //drawstring( 10,140,0,"Var (1zdir): "+getglobalvar("boss1_zdir"));
            //drawstring( 10,150,0,"Var (2zdir): "+getglobalvar("boss2_zdir"));

            /*drawstring( 10,140,0,"Var (lasthita): "+openborvariant("lasthita"));
            drawstring( 10,150,0,"Var (lasthitc): "+openborvariant("lasthitc"));
            drawstring( 10,160,0,"Var (lasthitt): "+openborvariant("lasthitt"));
            drawstring( 10,170,0,"Var (lasthitx): "+openborvariant("lasthitx"));
            drawstring( 10,180,0,"Var (lasthitz): "+openborvariant("lasthitz"));*/

             //drawstring( 10, 190,0,"Var (P1 Pos):    a =  "+getentityproperty(getplayerproperty(0, "entity"), "y"));
             //drawstring( 10, 200,0,"Var (P1 Pos):    x =  "+getentityproperty(getplayerproperty(0, "entity"), "x")+"   z =  "+getentityproperty(getplayerproperty(0, "entity"), "z") );
             /*drawstring( 10, 210,0,"Var (Camera Z):  "+getlevelproperty("camerazoffset")+"   Camera X:   "+getlevelproperty("cameraxoffset") );
             drawstring( 10, 220,0,"Var (Time):  "+openborvariant("elapsed_time") % 10000 );
             drawstring( 10, 230,0,"Var (animid):  "+getentityproperty(getplayerproperty(0, "entity"), "animationid") + "  dir:  " + getentityproperty(getplayerproperty(0, "entity"),"direction") );
             drawstring( 260,230,0,"FPS:  "+getFPS());*/

            /*if ( getglobalvar("in_menu") != 1 && openborvariant("in_level")&& getentityproperty(getplayerproperty(0,"entity"),"exists") ) {
                 if ( getlevelproperty("type") != 2 ) drawstring( 10,230,0,"is_waiting:  "+is_waiting(2));
            }*/

           /*drawstring( 10,210,0,"TOTAL RAM: "+ov("totalram")+" KB");
           drawstring( 10,220,0,"USED RAM: "+ov("usedram")+" KB");
           drawstring( 10,230,0,"FREE RAM: "+ov("freeram")+" KB");*/

           /*int yy = 50;
           drawstring(10,yy,0,ggv("aa"));
           drawstring(10,yy+10,0,"x: "+ggv("p1respawn_x"));
           drawstring(10,yy+20,0,"z: "+ggv("p1respawn_z"));
           drawstring(10,yy+30,0,"a: "+ggv("p1respawn_a"));
           drawstring(10,yy+40,0,"base: "+ggv("p1respawn_base"));
           drawstring(10,yy+50,0,"platform: "+ggv("p1respawn_platform"));*/

           //int i = 0;for (i = 0; i < MAX_PLAYERS; i++) drawstring( 10,140+i*10,0,"var: "+getglobalvar("current_credits_"+i)+" credits: "+gpp(i,"credits"));






    //check_entity_collision(60,20,40,0,0);
    //set_cam_between_players();

    check_disable_keys_nojoin();
}

void set_cam_between_players() {
    if ( getglobalvar("in_menu") != 1 && openborvariant("in_level") ) {
        if ( getlevelproperty("type") != 2 ) { // 2 is for custom hud level
            float xpos = openborvariant("xpos"), new_xpos = 0;
            float ypos = openborvariant("ypos"), new_ypos = 0;

            new_xpos = truncA(xpos)-get_mantix(xpos);
            new_ypos = truncA(ypos)-get_mantix(ypos);
            if ( new_ypos < 0 ) new_xpos = 0;
            if ( new_ypos < 0 ) new_ypos = 0;

            changeopenborvariant("xpos",new_xpos);
            changeopenborvariant("ypos",new_ypos);
        }
    }
}

// entity_collision v2.3
void check_entity_collision(float width, float depth, float height, int nomove_flag, int opp_move_flag) { //COLLISION FOR UPDATED.C
    int i = 0;

    for (i = 0; i < openborvariant("count_entities"); ++i) {
        void opp = getentity(i);
        int type = getentityproperty(opp, "type");

        if ( type == openborconstant("TYPE_ENEMY") || type == openborconstant("TYPE_PLAYER") ) {
            float ox = getentityproperty(opp, "x");
            float oz = getentityproperty(opp, "z");
            float oa = getentityproperty(opp, "a");
            float obase = getentityproperty(opp, "base");
            int odir = getentityproperty(opp, "direction");
            float t_shift = 0;
            int p;

            for (p = 0; p < openborvariant("maxplayers"); ++p) {
                void player = getplayerproperty(p, "entity");

                if ( getentityproperty(player, "exists") ) {
                    float x = getentityproperty(player, "x");
                    float z = getentityproperty(player, "z");
                    float a = getentityproperty(player, "a");
                    float base = getentityproperty(player, "base");
                    int dir = getentityproperty(player, "direction");
                    float xdir = getentityproperty(player, "xdir");
                    float zdir = getentityproperty(player, "zdir");
                    float tossv = getentityproperty(player, "tossv");
                    float speed = getentityproperty(player, "speed");
                    float runspeed = getentityproperty(player, "running", "speed");
                    float is_running = getentityproperty(player, "aiflag", "running");
                    float new_speed = speed;

                    float left_bound = width/2;
                    float right_bound = width/2;
                    float up_bound = depth/2;
                    float down_bound = depth/2;

                    if ( player != opp ) {
                        //if ( getentityproperty(player,"playerindex") == 0 ) drawstring(10,120,0,"xdir: "+getentityproperty(player,"xdir"));
                        //if ( getentityproperty(player,"playerindex") == 0 ) drawstring(10,130,0,"zdir: "+getentityproperty(player,"zdir"));

                        // COLLIDE
                        if ( x >= ox-left_bound-t_shift && x <= ox+right_bound+t_shift && z >= oz-up_bound-t_shift && z <= oz+down_bound+t_shift && a >= oa-t_shift && a <= oa+height+t_shift) {
                            if ( is_running ) new_speed = runspeed;

                            // reset position
                            //drawstring(10,100,0,"COLLISION!");

                            if ( xdir == 0 && zdir == 0 ) continue;
                            //if ( xdir == 0 && zdir == 0 && tossv == 0 ) continue;
                            if ( getentityproperty(player,"aiflag","attacking") ) continue;
                            if ( getentityproperty(player,"aiflag","inpain") ) continue;
                            if ( getentityproperty(player,"aiflag","falling") ) continue;
                            if ( getentityproperty(player,"aiflag","dead") ) continue;
                            if ( getentityproperty(player,"noaicontrol") ) continue;
                            //if ( getentityproperty(player,"aiflag","jumping") ) continue;
                            //if ( tossv > 0 ) continue; // > or !=

                            //if ( xdir != 0 ) changeentityproperty(player, "velocity", -1*xdir, NULL(), NULL());
                            /*else {
                                if ( x >= ox-left_bound && x <= ox ) changeentityproperty(player, "velocity", -1*new_speed, NULL(), NULL());
                                else if ( x > ox && x <= ox+right_bound) changeentityproperty(player, "velocity", new_speed, NULL(), NULL());
                            }*/

                            //if ( zdir != 0 ) changeentityproperty(player, "velocity", NULL(), -1*zdir, NULL());
                            /*else {
                                if ( z >= oz-up_bound && z <= oz ) changeentityproperty(player, "velocity", NULL(), -1*new_speed/2, NULL());
                                else if ( z > oz && z <= oz+down_bound ) changeentityproperty(player, "velocity", NULL(), new_speed/2, NULL());
                            }*/

                            if ( xdir != 0 ) {
                                if ( x >= ox-left_bound && x <= ox ) changeentityproperty(player, "velocity", -1*new_speed, NULL(), NULL());
                                else if ( x > ox && x <= ox+right_bound) changeentityproperty(player, "velocity", new_speed, NULL(), NULL());
                            }

                            if ( zdir != 0 ) {
                                if ( z >= oz-up_bound && z <= oz ) changeentityproperty(player, "velocity", NULL(), -1*new_speed/2, NULL());
                                else if ( z > oz && z <= oz+down_bound ) changeentityproperty(player, "velocity", NULL(), new_speed/2, NULL());
                            }

                            if ( opp_move_flag > 0 ) {
                                float opp_xdir = xdir;
                                float opp_zdir = zdir;

                                if ( getentityproperty(opp, "xdir") == 0 ) opp_xdir *= 2;
                                if ( getentityproperty(opp, "zdir") == 0 ) opp_zdir *= 2;

                                changeentityproperty(opp, "velocity", opp_xdir, NULL(), NULL());
                                changeentityproperty(opp, "velocity", NULL(), opp_zdir, NULL());
                            }

                            //if ( is_running ) changeentityproperty(player,"aiflag","running",1);

                            // TO SHIFT WHILE MOVING
                            if ( nomove_flag <= 0 || nomove_flag == NULL() ) {
                                new_speed = speed;
                                //if ( is_running ) new_speed = runspeed;

                                //if ( tossv > 0) new_speed /= 2;
                                if ( tossv > 0) new_speed = 0;

                                if ( x >= ox-left_bound && x <= ox-left_bound+(right_bound+left_bound)/2 ) changeentityproperty(player, "velocity", -1*new_speed, NULL(), NULL());
                                else if ( x > ox-left_bound+(right_bound+left_bound)/2 && x <= ox+right_bound) changeentityproperty(player, "velocity", new_speed, NULL(), NULL());
                                //else changeentityproperty(player, "velocity", xdir, NULL(), NULL());

                                if ( z >= oz-up_bound && z <= oz-up_bound+(down_bound+up_bound)/2 ) changeentityproperty(player, "velocity", NULL(), -1*new_speed/2, NULL());
                                else if ( z > oz-up_bound+(down_bound+up_bound)/2 && z <= oz+down_bound ) changeentityproperty(player, "velocity", NULL(), new_speed/2, NULL());
                                //else changeentityproperty(player, "velocity", NULL(), zdir, NULL());

                                /*if ( xdir != 0 || zdir != 0 ) { // to not move the stopped character (if it is stopped all dirs == 0 and so it cant move itsself)
                                    //drawstring(10,110,0,"MOVIN!");

                                    if ( x >= ox-left_bound && x <= ox-left_bound+(right_bound+left_bound)/2 && xdir == 0 ) changeentityproperty(player, "velocity", -1*speed, NULL(), NULL());
                                    else if ( x > ox-left_bound+(right_bound+left_bound)/2 && x <= ox+right_bound && xdir == 0 ) changeentityproperty(player, "velocity", speed, NULL(), NULL());
                                    //else changeentityproperty(player, "velocity", xdir, NULL(), NULL());

                                    if ( z >= oz-up_bound && z <= oz-up_bound+(down_bound+up_bound)/2 && zdir == 0 ) changeentityproperty(player, "velocity", NULL(), -1*speed/2, NULL());
                                    else if ( z > oz-up_bound+(down_bound+up_bound)/2 && z <= oz+down_bound && zdir == 0 ) changeentityproperty(player, "velocity", NULL(), speed/2, NULL());
                                    //else changeentityproperty(player, "velocity", NULL(), zdir, NULL());
                                }*/
                            }
                        }
                    }
                } // fine if player exists
            }
        }
	}
}

void check_disable_keys_nojoin() {
    if ( openborvariant("nojoin") ) {
        int p = 0;

        for (p = 0; p < openborvariant("maxplayers"); ++p) {
            void player = getplayerproperty(p,"entity");

            if ( !getentityproperty(player,"exists") ) {
                changeplayerproperty(p, "keys", 0);
                changeplayerproperty(p, "newkeys", 0);
                changeplayerproperty(p, "playkeys", 0);
            }
        }
    }
}

void ondestroy() {

}
