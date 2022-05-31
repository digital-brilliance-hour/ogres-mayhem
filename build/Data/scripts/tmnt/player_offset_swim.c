
#include "data/scripts/defines.h"


void check_zmax_swim_distance(void self) {

    if ( openborvariant("in_level") && openborvariant("count_players") > 1 ) {
        //int camerazoffset = getlevelproperty("camerazoffset");
        //float x = getentityproperty(self, "x");
        float z = getentityproperty(self, "z");
        //float y = getentityproperty(self, "y");
        float base = getentityproperty(self, "base");
        float zdir = getentityproperty(self, "zdir");
        //float zspeed = getentityproperty(self, "speed")/2;
        float zmin_pos = NULL(), zmax_pos = NULL(), base_pos = NULL(), p, zmin_bound, zmax_bound;
        float zmin_threshold = 20, zmax_threshold = 20;
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
        if ( getglobalvar("zbound_"+pindex) != NULL() ) setglobalvar("zbound_"+pindex, NULL());
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
            setglobalvar("zbound_"+pindex, NULL());
            if ( z-base >= zmax_bound ) {
                //drawstring( 10,180,0,"ATTIVO BASSO!!" );
                if ( playerkeys(pindex,0,"movedown") == 32 ) changeentityproperty(self, "velocity", NULL(), -10, NULL());
                setglobalvar("zbound_"+pindex, 1);
                setglobalvar("gravity_"+pindex, 0);
            } else if ( z-base <= zmin_bound ) {
                //drawstring( 10,170,0,"ATTIVO ALTO!!" );
                if ( playerkeys(pindex,0,"moveup") == 16 ) changeentityproperty(self, "velocity", NULL(), 10, NULL());
                setglobalvar("zbound_"+pindex, 1);
                setglobalvar("gravity_"+pindex, 0);
            } else if ( zmax_pos-base_pos >= zmax_bound && z < zmax_pos ) { // cioè non è il player corrente ad essere in fallo...
                if ( playerkeys(pindex,0,"moveup") == 16 ) changeentityproperty(self, "velocity", NULL(), 10, NULL());
                setglobalvar("zbound_"+pindex, 1);
                setglobalvar("gravity_"+pindex, 0);
            } else if ( zmin_pos-base_pos <= zmin_bound && z > zmin_pos ) {
                if ( playerkeys(pindex,0,"movedown") == 32 ) changeentityproperty(self, "velocity", NULL(), -10, NULL());
                setglobalvar("zbound_"+pindex, 1);
                setglobalvar("gravity_"+pindex, 0);
            } else setglobalvar("zbound_"+pindex, NULL());
        } //else setglobalvar("zbound_"+pindex, NULL());

    } // fine if in_level

}


