
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

// Imposta una distanza minima e massima sull'asse Z
int check_zbounds(void self, float min_threshold, float max_threshold) {
    float scr_posy = openborvariant("ypos");
    int vres = openborvariant("vresolution");
    int zdir = getentityproperty(self, "zdir");
    float z = getentityproperty(self, "z");
    //float y = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    int anim_id = getentityproperty(self, "animationid");
    /*float zmin = ov("player_min_z");
    float zmax = ov("player_max_z");
    float cameraz = glvp("camerazoffset");*/

    if ( openborvariant("in_level") ) {
        if ( anim_id != openborconstant("ANI_SPAWN") && anim_id != openborconstant("ANI_RESPAWN") ) {
            if ( z-base <= scr_posy+min_threshold ) {
                zdir = abs(zdir);
                changeentityproperty(self, "velocity", NULL(), zdir, NULL());
            } else if ( z-base >= scr_posy+vres+max_threshold ) {
                zdir = -1*abs(zdir);
                changeentityproperty(self, "velocity", NULL(), zdir, NULL());
            }
        }
    }
    /*drawstring(10,50,0,"ypos: "+scr_posy);
    drawstring(10,50+10,0,"z: "+z);
    drawstring(10,50+20,0,"y: "+gep(self, "y"));
    drawstring(10,50+30,0,"ybound: "+(scr_posy+vres));
    drawstring(10,50+40,0,"lv_height: "+ov("levelheight"));
    drawstring(10,50+50,0,"cameraz: "+glvp("camerazoffset"));*/
}

// Imposta una distanza massima tra 2 o più giocatori
void check_zmax_distance(void self) {

    if ( openborvariant("in_level") && openborvariant("count_players") > 1 ) {
        //int camerazoffset = getlevelproperty("camerazoffset");
        //float x = getentityproperty(self, "x");
        float z = getentityproperty(self, "z");
        //float y = getentityproperty(self, "y");
        float base = getentityproperty(self, "base");
        float zdir = getentityproperty(self, "zdir");
        //int zspeed = getentityproperty(self, "speed")/2;
        float zmin_pos = NULL(), zmax_pos = NULL(), base_pos = NULL(), p, zmin_bound, zmax_bound;
        float zmin_threshold = ZBOUND__, zmax_threshold = ZBOUND__;
        //float scr_posx = openborvariant("xpos");
        float scr_posy = openborvariant("ypos");
        //int hres = openborvariant("hresolution");
        int vres = openborvariant("vresolution");
        int pindex = getentityproperty(self, "playerindex");
        /*float zmin = ov("player_min_z");
        float zmax = ov("player_max_z");
        float cameraz = glvp("camerazoffset");*/

        if ( getentityproperty(self,"noaicontrol") ) return;

        // Cerchiamo la distanza minima in z
        zmin_pos = NULL();
        for (p = 0; p < MAX_PLAYERS; ++p) {
            void player = getplayerproperty(p, "entity");

            if ( getentityproperty(player, "exists") ) { // comprendiamo il proprio z
                if ( zmin_pos == NULL() ) {
                    zmin_pos = getentityproperty(player, "z");
                    base_pos = getentityproperty(player, "base");
                } else if ( getentityproperty(player, "z") < zmin_pos ) {
                    zmin_pos = getentityproperty(player, "z");
                    base_pos = getentityproperty(player, "base");
                }
            } else continue;
        }

        // Cerchiamo la distanza massima in z
        zmax_pos = NULL();
        for (p = 0; p < MAX_PLAYERS; ++p) {
            void player = getplayerproperty(p, "entity");

            if ( getentityproperty(player, "exists") ) {
                if ( zmax_pos == NULL() ) {
                    zmax_pos = getentityproperty(player, "z");
                    base_pos = getentityproperty(player, "base");
                } else if ( getentityproperty(player, "z") > zmax_pos ) {
                    zmax_pos = getentityproperty(player, "z");
                    base_pos = getentityproperty(player, "base");
                }
            } else continue;
        }

        if ( zmin_pos != NULL() && zmax_pos != NULL() ) {
            zmax_bound = scr_posy+vres+zmax_threshold; //media -> parte rimanente in basso rispetto ai players
            zmin_bound = scr_posy+zmin_threshold; //vres-camerazoffset parte rimanente in alto

            // Per arrotondare un valore z: z = z%(z*z)
            if ( z-base >= zmax_bound ) {
                //z = z%(z*z);
                //drawstring( 10,180,0,"ATTIVO BASSO!!" );
                //if ( playerkeys(pindex,0,"movedown") == 32 ) changeentityproperty(self, "position", x, zmax_bound, a); //zmax_bound-zmax_threshold
                if ( playerkeys(pindex,0,"movedown") == 32 ) changeentityproperty(self, "velocity", NULL(), -1*zdir, NULL());
            } else if ( z-base <= zmin_bound ) {
                //drawstring( 10,170,0,"ATTIVO ALTO!!" );
                if ( playerkeys(pindex,0,"moveup") == 16 ) changeentityproperty(self, "velocity", NULL(), -1*zdir, NULL());
            } else if ( zmax_pos-base_pos >= zmax_bound && z < zmax_pos ) { // cioè non è il player corrente ad essere in fallo...
                //if ( playerkeys(pindex,0,"moveup") == 16 ) changeentityproperty(self, "position", x, z+zspeed, a);
                if ( playerkeys(pindex,0,"moveup") == 16 ) changeentityproperty(self, "velocity", NULL(), -1*zdir, NULL());
            } else if ( zmin_pos-base_pos <= zmin_bound && z > zmin_pos ) {
                //if ( playerkeys(pindex,0,"movedown") == 32 ) changeentityproperty(self, "position", x, z-zspeed, a);
                if ( playerkeys(pindex,0,"movedown") == 32 ) changeentityproperty(self, "velocity", NULL(), -1*zdir, NULL());
            }
        }

    } // fine if in_level

}

float get_zmin_distance() { // or player_min_z
    float scr_posy = openborvariant("ypos");
    int zmin_threshold = ZBOUND__;
    float zmin_bound = scr_posy+zmin_threshold;

    return zmin_bound;
}

float get_zmax_distance() { // or player_max_z
    float scr_posy = openborvariant("ypos");
    int vres = openborvariant("vresolution");
    int zmax_threshold = ZBOUND__;
    float zmax_bound = scr_posy+vres+zmax_threshold;

    return zmax_bound;
}


