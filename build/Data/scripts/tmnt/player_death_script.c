
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"
#import "data/scripts/endlevel_anim.c"
#import "data/scripts/player_commons.c"

void main() { // entityvar usate (indexes): 0(getwalkingdir), 1(time), 2(combo counter),
    void self = getlocalvar("self");

    show_icondie(self);
    show_gameover(self);
    reset_on_die(self);
}

void show_icondie(void self) {
    int hres = openborvariant("hresolution");
    int vres = openborvariant("vresolution");
    float xpos = openborvariant("xpos");
    float ypos = openborvariant("ypos");
    int type = getentityproperty(self,"type");
    void ent;

    if ( type == openborconstant("TYPE_PLAYER") ) {
        int p = getentityproperty(self, "playerindex");
        void player = getplayerproperty(p, "entity");
        char name = getplayerproperty(p, "name");
        int map = getentityproperty(self,"map");

        //if ( getplayerproperty(p, "lives") <= 1 ) {
            if ( isATurtle(self) ) {
                if ( getglobalvar("icondie_"+p) != NULL() && getentityproperty(getglobalvar("icondie_"+p) ,"exists") ) killentity(getglobalvar("icondie_"+p));
                ent = spawnsubentity("crying_turtle",xpos+p*82+15,ypos+14,NULL());
                changeentityproperty(ent,"parent",self);
                set_board_map(self,ent);
                setglobalvar("icondie_"+p,ent);
                setentityvar(ent,"player_index",p);
                setentityvar(ent,"prev_lives",getplayerproperty(p,"lives"));
            }
        //}
    }

    // No need becouse when an entity dies, the entityvars are resetted
    /*if ( gev(self,"subject_to_stair") != NULL() ) sev(self,"subject_to_stair",NULL());
    if ( gev(self,"subject_to_wave") != NULL() ) sev(self,"subject_to_wave",NULL());
    if ( gev(self,"subject_to_waterpuddle") != NULL() ) sev(self,"subject_to_waterpuddle",NULL());*/

    //if ( !getentityproperty(player, "exists") && name == "" ) { // name == "" trick
}

void show_gameover(void self) {
    int hres = openborvariant("hresolution");
    int vres = openborvariant("vresolution");
    float xpos = openborvariant("xpos");
    float ypos = openborvariant("ypos");
    int type = getentityproperty(self,"type");
    void ent;

    if ( type == openborconstant("TYPE_PLAYER") ) {
        int p = getentityproperty(self, "playerindex");
        void player = getplayerproperty(p, "entity");
        char name = getplayerproperty(p, "name");

        if ( getplayerproperty(p, "lives") <= 1 ) {
            ent = spawnsubentity("player_gameover",xpos+p*82+6,ypos+34,NULL());
            changeentityproperty(ent,"parent",self);
            setentityvar(ent,"player_index",p);
            setentityvar(ent,"prev_lives",getplayerproperty(p,"lives"));
        }
    }

    //if ( !getentityproperty(player, "exists") && name == "" ) { // name == "" trick
    /*if ( getentityproperty(player, "health") <= 0 && getplayerproperty(p, "lives") <= 0 ) {
        spawnsubentity("player_gameover",xpos+p*82+6,ypos+34,NULL()); // 6,88,170,252
    }*/
}

void reset_on_die(void self) {
    int p = getentityproperty(self, "playerindex");

    // disabilitare questo se non si vogliono visualizzare i tasti dopo la morte
    //if ( getplayerproperty(p,"lives") <= 0 ) { // change at continue
        setglobalvar("any_button"+p, NULL());
        setglobalvar("attack_button"+p, NULL());
        setglobalvar("attack_button_escape"+p, NULL());
        setglobalvar("any_button_active"+p, NULL());
        setglobalvar("attack_button_active"+p, NULL());
        setglobalvar("atk_btn_escape_active"+p, NULL());
        setglobalvar("rrodney_button_escape"+p, NULL());
        setglobalvar("rrodney_active"+p, NULL());
        setglobalvar("frozen_button_escape"+p, NULL());
        setglobalvar("frozen_active"+p, NULL());
        setglobalvar("bubbled_button_escape"+p, NULL());
        setglobalvar("bubbled_active"+p, NULL());
        setglobalvar("muddied_button_escape"+p, NULL());
        setglobalvar("muddied_active"+p, NULL());
        setglobalvar("roped_button_escape"+p, NULL());
        setglobalvar("roped_active"+p, NULL());
    //}
}
