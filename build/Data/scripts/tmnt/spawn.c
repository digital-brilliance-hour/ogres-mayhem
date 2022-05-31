
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#import "data/scripts/player_commons.c"
#import "data/scripts/endlevel_anim.c"
//#import "data/scripts/player_respawn.c"

void main() {
    void self = getlocalvar("self");
    int anim_id = getentityproperty(self, "animationid");
    int p = getentityproperty(self, "playerindex");
    int inv_eta = openborvariant("game_speed")*3;
    /*char cText = getentityproperty(self, "model");
    char cFind = "STOP";
    char substr = strinfirst(cText, cFind);*/

    // Check Time-Reset
    changeentityproperty(self, "invincible", 1);
    changeentityproperty(self, "invinctime", get_next_time(inv_eta)); //makes entity invincible for 1.5 seconds, change decimal number to change time length
    changeentityproperty(self, "aiflag", "blink", 1); // makes entity blink until invinctime is reached

    if ( getglobalvar("in_menu") != 1 && openborvariant("in_level") ) { // Waiting, Select
        if ( getlevelproperty("type") != 2 ) { // 2 is for custom hud level
            set_parrow(self);
            set_board(self);
        }
    }

    if ( getglobalvar("1VS1") ) {
        if ( p == 1 || p == 3 ) {
            changeentityproperty(self, "direction", 0);
        }
    }

    set_candamage(self,"TYPE_NPC"); // to allow to hit the weapons

    show_icon(self);
    setglobalvar("nosame"+p, NULL());
    setglobalvar("nosamecolour"+p, NULL());
}

void show_icon(void self) {
    if ( getglobalvar("in_menu") != 1 && openborvariant("in_level") ) { // Waiting, Select
        if ( getlevelproperty("type") != 2 ) { // 2 is for custom hud level
            int type = gep(self,"type");
            int hres = ov("hresolution");
            int vres = ov("vresolution");
            float xpos = ov("xpos");
            float ypos = ov("ypos");
            void ent;

            if ( type == oc("TYPE_PLAYER") ) {
                int p = gep(self,"playerindex");
                char defaultmodel = gep(self,"defaultmodel");

                if ( gpp(p,"weapnum") == 3 && isATurtle(self) ) { // glider
                    ent = spawnsubentity("turtle_icon",xpos+p*82+15,ypos+14,NULL());
                    changeentityproperty(ent,"parent",self);
                    set_board_map(self,ent);
                    setentityvar(ent,"player_index",p);
                    setentityvar(ent,"prev_lives",getplayerproperty(p,"lives"));

                         if ( defaultmodel == "LEO" ) setidle(ent,oc("ANI_FOLLOW1"),1);
                    else if ( defaultmodel == "MIKE" ) setidle(ent,oc("ANI_FOLLOW2"),1);
                    else if ( defaultmodel == "DON" ) setidle(ent,oc("ANI_FOLLOW3"),1);
                    else if ( defaultmodel == "RAPH" ) setidle(ent,oc("ANI_FOLLOW4"),1);
                }
            }
        }
    }
}

int set_board(void self) {
    void subent;
    char board = "";
    int type = getentityproperty(self,"type");
    int board_level = check_board_level();

    //board_level = 3;

    if ( type == openborconstant("TYPE_PLAYER") ) {
        char defaultmodel = getentityproperty(self,"defaultmodel");

        if ( defaultmodel == "RAPH" || defaultmodel == "LEO" || defaultmodel == "DON" || defaultmodel == "MIKE" ) {
            if ( board_level == 1 ) {
                board = "surf";
            } else if ( board_level == 2 ) {
                board = "skate";
            } else if ( board_level == 3 ) {
                board = "ufo";
            } else if ( board_level == 4 ) {
                board = "snow";
            } else return 0;
        } else {
            if ( board_level == 1 ) {
                board = "esurf";
            } else if ( board_level == 2 ) {
                board = "eskate";
            } else if ( board_level == 3 ) {
                board = "eufo";
            } else if ( board_level == 4 ) {
                board = "esnow";
            } else return 0;
        }

        if ( board_level > 0 ) {
            setglobalvar("player_nograb",1);
            changeentityproperty(self,"antigrab",999999);
            //changeentityproperty(self,"nograb",1);
        } else if ( getglobalvar("player_nograb") > 0 ) setglobalvar("player_nograb",NULL());
    } else if ( type == openborconstant("TYPE_ENEMY") ) {
        if ( board_level == 1 ) {
            board = "esurf";
        } else if ( board_level == 2 ) {
            board = "eskate";
        } else if ( board_level == 3 ) {
            board = "eufo";
        } else if ( board_level == 4 ) {
            board = "esnow";
        } else return 0;

        if ( board_level > 0 ) {
            setglobalvar("enemy_nograb",1);
            changeentityproperty(self,"antigrab",999999);
            //changeentityproperty(self,"nograb",1);
        } else if ( getglobalvar("enemy_nograb") > 0 ) setglobalvar("enemy_nograb",NULL());
    } else return 0;

    clearspawnentry();
    setspawnentry("name", board);
    subent = spawn();
    changeentityproperty(subent, "parent", self);
    changeentityproperty(subent, "sortid", getentityproperty(self,"sortid")-1);

    //changeentityproperty(subent, "setlayer", getentityproperty(self,"setlayer")-1); // fix with shadowcoords
    if ( type == openborconstant("TYPE_PLAYER") ) set_board_map(self,subent);
    //setentityvar(self,"board",subent);
}

int set_parrow(void player) {
 int i;
 void parrow;
 int p = getentityproperty(player, "playerindex");
 int lifespan = openborvariant("game_speed")*3.0; // 3 secs.
 int defaultmodel = getentityproperty(player,"defaultmodel");

    if ( defaultmodel == "LEO" || defaultmodel == "MIKE" || defaultmodel == "DON" || defaultmodel == "RAPH" ) {
        setentityvar(player,1,NULL());
        setentityvar(player,2,NULL());
        setentityvar(player,3,NULL());
    }

    //setglobalvar("surfp"+(p+1), NULL()); // resettiamo la var

    if (p==0) {
       clearspawnentry();
       setspawnentry("name", "parrow1");
       parrow = spawn();
       changeentityproperty(parrow, "parent", player);
       changeentityproperty(parrow, "lifespancountdown", lifespan);
    }
    else if (p==1) {
       clearspawnentry();
       setspawnentry("name", "parrow2");
       parrow = spawn();
       changeentityproperty(parrow, "parent", player);
       changeentityproperty(parrow, "lifespancountdown", lifespan);
    }
    else if (p==2) {
       clearspawnentry();
       setspawnentry("name", "parrow3");
       parrow = spawn();
       changeentityproperty(parrow, "parent", player);
       changeentityproperty(parrow, "lifespancountdown", lifespan);
    }
    else if (p==3) {
       clearspawnentry();
       setspawnentry("name", "parrow4");
       parrow = spawn();
       changeentityproperty(parrow, "parent", player);
       changeentityproperty(parrow, "lifespancountdown", lifespan);
    }

}

