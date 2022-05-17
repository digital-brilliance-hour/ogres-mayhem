
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

// it returns: 0 == not completed, 1 rocking complete, 2 half completed rocking
int move_platform_vert(void self, int init_dir, float maxspeed, float acc, float dist) {
    float x = gep(self,"x");
    float z = gep(self,"z");
    float y = gep(self,"y");
    float base = gep(self,"base");
    float vel = glv("vel");
    float original_pos = glv("original_pos");
    int step = 0;

    if ( original_pos == NULL() ) {
        slv("original_pos",y);
        original_pos = glv("original_pos");
    }

    if ( vel == NULL() ) {
        if ( init_dir ) vel = maxspeed;
        else vel = -1*maxspeed;
    }

    if ( init_dir ) {
        if ( y >= original_pos+dist ) {
            vel -= acc;
            if ( vel < -1*maxspeed ) vel = -1*maxspeed;
            if ( vel <= 0 && vel != -1*maxspeed ) step = 2; // half complete
        } else if ( y <= original_pos ) {
            vel += acc;
            if ( vel > maxspeed ) vel = maxspeed;
            if ( vel >= 0 && vel != maxspeed ) step = 1; // full complete
        } else if ( vel <= 0 ) { // raggiungiamo le velocità max
            vel -= acc;
            if ( vel < -1*maxspeed ) vel = -1*maxspeed;
        } else if ( vel >= 0 ) {
            vel += acc;
            if ( vel > maxspeed ) vel = maxspeed;
        }
    } else {
        if ( y <= original_pos-dist ) {
            vel += acc;
            if ( vel > maxspeed ) vel = maxspeed;
            if ( vel >= 0 && vel != maxspeed ) step = 2; // half complete
        } else if ( y >= original_pos ) {
            vel -= acc;
            if ( vel < -1*maxspeed ) vel = -1*maxspeed;
            if ( vel <= 0 && vel != -1*maxspeed ) step = 1; // full complete
        } else if ( vel <= 0 ) { // raggiungiamo le velocità max
            vel -= acc;
            if ( vel < -1*maxspeed ) vel = -1*maxspeed;
        } else if ( vel >= 0 ) {
            vel += acc;
            if ( vel > maxspeed ) vel = maxspeed;
        }
    }

    slv("vel",vel);
    cep(self,"velocity",NULL(),NULL(),vel);

    //cep(self,"tosstime",get_next_time(ov("game_speed")/20));
    //cep(self, "no_adjust_base", 1);
    //cep(self,"base",gep(self,"y"));

    if ( step > 0 ) return step;
    else return 0;
}

// it returns: 0 == not completed, 1 rocking complete, 2 half completed rocking
int move_platform_horiz(void self, int init_dir, float maxspeed, float acc, float dist) {
    float x = gep(self,"x");
    float z = gep(self,"z");
    float y = gep(self,"y");
    float base = gep(self,"base");
    float vel = glv("vel");
    float original_pos = glv("original_pos");
    int step = 0;

    if ( original_pos == NULL() ) {
        slv("original_pos",x);
        original_pos = glv("original_pos");
    }

    if ( vel == NULL() ) {
        if ( init_dir ) vel = maxspeed;
        else vel = -1*maxspeed;
    }

    if ( init_dir ) {
        if ( x >= original_pos+dist ) {
            vel -= acc;
            if ( vel < -1*maxspeed ) vel = -1*maxspeed;
            if ( vel <= 0 && vel != -1*maxspeed ) step = 2; // half complete
        } else if ( x <= original_pos ) {
            vel += acc;
            if ( vel > maxspeed ) vel = maxspeed;
            if ( vel >= 0 && vel != maxspeed ) step = 1; // full complete
        } else if ( vel <= 0 ) { // raggiungiamo le velocità max
            vel -= acc;
            if ( vel < -1*maxspeed ) vel = -1*maxspeed;
        } else if ( vel >= 0 ) {
            vel += acc;
            if ( vel > maxspeed ) vel = maxspeed;
        }
    } else {
        if ( x <= original_pos-dist ) {
            vel += acc;
            if ( vel > maxspeed ) vel = maxspeed;
            if ( vel >= 0 && vel != maxspeed ) step = 2; // half complete
        } else if ( x >= original_pos ) {
            vel -= acc;
            if ( vel < -1*maxspeed ) vel = -1*maxspeed;
            if ( vel <= 0 && vel != -1*maxspeed ) step = 1; // full complete
        } else if ( vel <= 0 ) { // raggiungiamo le velocità max
            vel -= acc;
            if ( vel < -1*maxspeed ) vel = -1*maxspeed;
        } else if ( vel >= 0 ) {
            vel += acc;
            if ( vel > maxspeed ) vel = maxspeed;
        }
    }

    slv("vel",vel);
    cep(self,"velocity",vel,NULL(),NULL());

    if ( step > 0 ) return step;
    else return 0;
}

