
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

void main() {
    void self = getlocalvar("self");

    if ( !gep(self,"no_adjust_base") ) cep(self,"no_adjust_base",1);

    spawn_dust(self);
}

int spawn_dust(void self) {
        float x = gep(self,"x");
        float z = gep(self,"z");
        float y = gep(self,"y");
        float base = gep(self,"base");
        int dir = gep(self,"direction");
        char defaultmodel = gep(self,"defaultmodel");
        char dust_name = "board_dust";
        void ent;

        if ( defaultmodel == "surf" ) {

        } else if ( defaultmodel == "esurf" ) {

        } else if ( defaultmodel == "skate" ) {

        } else if ( defaultmodel == "eskate" ) {

        } else if ( defaultmodel == "ufo" ) {

        } else if ( defaultmodel == "eufo" ) {

        } else if ( defaultmodel == "snow" ) {
            float xshift = 36;

            if ( !dir ) xshift *= -1;

            //ent = spawnsubentity_relative(dust_name,0,-2,0,self);
            ent = spawnsubentity_parent(dust_name,x,z-1,y,self);
            bindentity(ent,self,xshift,-1,0,-1,0);
            cep(ent,"map",3);
            //cep(ent,"direction",dir);
        } else if ( defaultmodel == "esnow" ) {
            float xshift = 36;

            if ( !dir ) xshift *= -1;

            //ent = spawnsubentity_relative(dust_name,0,-2,0,self);
            ent = spawnsubentity_parent(dust_name,x,z-1,y,self);
            bindentity(ent,self,xshift,-1,0,-1,0);
            cep(ent,"map",3);
            //cep(ent,"direction",dir);
        }
}
