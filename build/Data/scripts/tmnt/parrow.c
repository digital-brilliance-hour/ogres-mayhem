
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/player_commons.c"

void main() { // aggiorna sempre fino alla morte dell'entità che chiama SCRIPT ...parrows.c
    void self = getlocalvar("self");
    void parent = getentityproperty(self, "parent");

    if ( getentityproperty(parent, "exists") ) {
        slv("candie",1);
        if ( getglobalvar("in_menu") ) {
            float x = getentityproperty(parent, "x");
            float z = getentityproperty(parent, "z");
            float a = getentityproperty(parent, "y");

            changeentityproperty(self, "position", x+140, z+117, a);
            changeentityproperty(self, "base", a);

            set_board_map(parent,self);
        } else {
            float x = getentityproperty(parent, "x");
            float z = getentityproperty(parent, "z");
            float a = getentityproperty(parent, "y");
            int dir = getentityproperty(parent, "direction");
            char pdefaultmodel = getentityproperty(parent, "defaultmodel");
            char pmodel = getentityproperty(parent, "model");
            int layer = getentityproperty(parent, "setlayer");
            float xshift = 100, zshift = 1, yshift = -10;

            if ( !dir ) xshift = 80;

                 if ( search_str(pmodel,"GLIDER",0) != -1 ) yshift = -30;
            else if ( search_str(pmodel,"SWIM",0)   != -1 ) yshift = -20;
            else if ( pdefaultmodel == "TOKKA" || pdefaultmodel == "RAHZAR" ) yshift = 10;

            changeentityproperty(self, "position", x+xshift, z+zshift, a+yshift);
            changeentityproperty(self, "base", a+yshift);
            changeentityproperty(self, "setlayer", layer+z);

            set_board_map(parent,self);

            if ( getentityproperty(self, "lifespancountdown") <= 0 ) killentity(self);
        }
    } else {
        if ( glv("candie") == 1 ) killentity(self);
    }
}

