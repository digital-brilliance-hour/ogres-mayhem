
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"

#import "data/scripts/player_grab.c"

#import "data/scripts/AI_climbing_criteria.c"
#import "data/scripts/AI_ladder_commons.c"
#import "data/scripts/ladder_commons.c"

#define THRESHOLD_LADDER_X openborvariant("hresolution")/1
#define THRESHOLD_LADDER_Z (openborvariant("scrollmaxz")-openborvariant("scrollminz"))/1
#define T_LADDER_RANGE 5
#define T_TARGET_RANGE 1
#define T_ACTION_RADIUS 20
#define T_TOP_EDGE 10
#define MIN_LOST_FRAME 0
#define T_TOSSTIME get_T_TOSSTIME()
#define T_DIST get_T_DIST()
#define T_LADDER_ALT_VISIBILITY get_ladder_alt_visibility()
#define MAX_LOST_FRAMES truncA(ov("game_speed")*1.5)

// DA FARE:
//

void main() {
  //void self = getlocalvar("self");

}

int check_AI_climb(void player) {
    int anim_id = getentityproperty(player, "animationid");

    //drawstring(10,210,0,"action_flag: "+get_action("moveup"));
    //drawstring(10,220,0,"action_flag: "+retrieve_flag_value_pos("moveup"));
    if ( anim_id == openborconstant("ANI_FOLLOW50") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        float speed = getentityproperty(player, "speed");
        float player_height = getentityproperty(player, "height");
        float shifta = speed/2, mul_shifta;
        float t_dist = T_DIST;
        float max_t_overclimb = T_ACTION_RADIUS;
        void ladder = getentityvar(player,"ladder");
        int lost_frames = get_lost_frames()+MIN_LOST_FRAME;

        if ( lost_frames == 0 ) lost_frames = 1;
        if ( lost_frames > MAX_LOST_FRAMES ) { lost_frames = 0; reset_lost_frames(); }
        mul_shifta = sum(shifta,lost_frames);

        if ( player_height < T_ENT_HEIGHT || player_height == NULL() ) player_height = T_ENT_HEIGHT; // height protection

        check_subjects(player);
        if ( getentityproperty(ladder, "exists") ) {
            float width = getentityvar(ladder,"width");
            float height = getentityvar(ladder,"height");
            float bo = getentityproperty(ladder,"base");
            float ao = getentityproperty(ladder,"y");

            generate_action_for_ladder(player);

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( get_action("moveup") == 16 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x,z,a+mul_shifta);
                //reset_lost_frames();
            } else if ( get_action("movedown") == 32 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x,z,a-mul_shifta);
                //reset_lost_frames();
            } else if ( get_action("moveleft") == 4 || get_action("moveright") == 8 ) {
                if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+(shifta*2));
                jump_from_climbing(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("start") == 2 ) { // walkoff
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                setidle(player,openborconstant("ANI_IDLE"),1);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            // SALI
            if ( a+(player_height) > ao+height && getentityvar(player,"ladder") != NULL() && get_action("moveup") == 16 ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a(x,z-t_dist,a+T_TOP_EDGE,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                float w_altitude = checkwall(x,z-t_dist);

                if (platform) {
                    float altitude = getentityproperty(platform, "y");
                    if ( getentityproperty(platform, "height") != NULL() ) altitude += getentityproperty(platform, "height");

                    // Però potrebbe esserci una piattaforma in più che ostruisce. verifichiamo!
                    void platform_up = find_platform_in_a(x,z-t_dist,altitude+player_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                    //drawstring( 10,200,0,"PLAT:  "+platform_up+"   "+platform);
                    if ( (!platform_up || platform_up == platform) && a <= altitude && altitude-a-player_height <= max_t_overclimb) { // SALE SULLA PIATTAFORMA
                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x,z-t_dist,altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW53"), 1);
                        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                        AI_ladder_reset(player,1);
                    } else { // NON SALE MA CONTINUA AD ARRAMPICARSI
                        changeentityproperty(player, "tosstime", T_TOSSTIME);
                        changeentityproperty(player, "position", x,z,a-mul_shifta);
                        reset_lost_frames();
                    }
                } else if ( a+T_TOP_EDGE <= w_altitude && a+T_TOP_EDGE >= w_altitude-max_t_overclimb ) { //  // SALE SUL MURO
                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x,z-t_dist,w_altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW53"), 1);
                        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                        AI_ladder_reset(player,1);
                } else {  // NON TROVA SPORGENZA QUINDI CADE
                    if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            } else if ( getentityvar(player,"ladder") != NULL() ) { // SCENDI A TERRA
                if ( a+(player_height+player_height/4) < ao && ao > bo ) { // SCALA SOSPESA QUINDI CADI: se la scala è in aria e non c'è una base su cui la scala si appoggia...
                    if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                } else if ( a <= getentityproperty(ladder, "base") && ao <= bo ) { // SCENDI A TERRA
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    setidle(player, openborconstant("ANI_IDLE"), 1);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            }
        } // fine if exists

    } else if ( anim_id == openborconstant("ANI_FOLLOW120") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        float speed = getentityproperty(player, "speed");
        float player_height = getentityproperty(player, "height");
        float shifta = speed/2, mul_shifta;
        float t_dist = T_DIST;
        float max_t_overclimb = T_ACTION_RADIUS;
        void ladder = getentityvar(player,"ladder");
        int lost_frames = get_lost_frames()+MIN_LOST_FRAME;

        if ( lost_frames == 0 ) lost_frames = 1;
        if ( lost_frames > MAX_LOST_FRAMES ) { lost_frames = 0; reset_lost_frames(); }
        mul_shifta = sum(shifta,lost_frames);

        if ( player_height < T_ENT_HEIGHT || player_height == NULL() ) player_height = T_ENT_HEIGHT; // height protection

        check_subjects(player);
        if ( getentityproperty(ladder, "exists") ) {
            float width = getentityvar(ladder,"width");
            float height = getentityvar(ladder,"height");
            float bo = getentityproperty(ladder,"base");
            float ao = getentityproperty(ladder,"y");
            float xo = getentityproperty(ladder,"x");

            generate_action_for_ladder_h(player);

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( get_action("moveleft") == 4 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x-mul_shifta,z,a);
                //reset_lost_frames();
            } else if ( get_action("moveright") == 8 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x+mul_shifta,z,a);
                //reset_lost_frames();
            } else if ( get_action("moveup") == 16 || get_action("movedown") == 32 ) {
                if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+(shifta*2));
                jump_from_climbing(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("start") == 2 ) { // walkoff
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                setidle(player,openborconstant("ANI_IDLE"),1);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            // NON TROVA SPORGENZA QUINDI CADE
            if ( (x > xo+width || x < xo) && getentityvar(player,"ladder") != NULL() && (get_action("moveleft") == 4 || get_action("moveright") == 8) ) {
                if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                jump_from_climbing(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            }
        } // fine if exists

    } else if ( anim_id == openborconstant("ANI_FOLLOW51") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        float speed = getentityproperty(player, "speed");
        float player_height = getentityproperty(player, "height");
        float shifta = speed/2, mul_shifta;
        float t_dist = T_DIST;
        float max_t_overclimb = T_ACTION_RADIUS;
        void ladder = getentityvar(player,"ladder");
        int lost_frames = get_lost_frames()+MIN_LOST_FRAME;

        if ( lost_frames == 0 ) lost_frames = 1;
        if ( lost_frames > MAX_LOST_FRAMES ) { lost_frames = 0; reset_lost_frames(); }
        mul_shifta = sum(shifta,lost_frames);

        if ( player_height < T_ENT_HEIGHT || player_height == NULL() ) player_height = T_ENT_HEIGHT; // height protection

        check_subjects(player);
        if ( getentityproperty(player, "direction") != 1 ) changeentityproperty(player, "direction", 1);
        if ( getentityproperty(ladder, "exists") ) {
            float width = getentityvar(ladder,"width");
            float height = getentityvar(ladder,"height");
            float bo = getentityproperty(ladder,"base");
            float ao = getentityproperty(ladder,"y");

            generate_action_for_ladder(player);

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( get_action("moveup") == 16 || get_action("moveright") == 8 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                // calcoliamo di quando il player deve andare avanti
                //float shiftx = 0.01*(width/2); // oppure width/height;
                float new_a = a+shifta;
                float shiftx = (((new_a-bo)*width)/height)/openborvariant("game_speed"); // 200 è costante d'inclinazione in base alla velocità del gioco

                changeentityproperty(player, "position", x+shiftx,z,a+mul_shifta);
                //reset_lost_frames();
            } else if ( get_action("movedown") == 32 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                float new_a = a-shifta;
                float shiftx = (width - ((new_a-bo)*width)/height)/openborvariant("game_speed");

                changeentityproperty(player, "position", x-shiftx,z,a-mul_shifta );
                //reset_lost_frames();
            } else if ( get_action("moveleft") == 4 ) {
                changeentityproperty(player, "direction", 0);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+(shifta*2)); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                jump_from_climbing(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("start") == 2 ) { // walkoff
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                setidle(player,openborconstant("ANI_IDLE"),1);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            if ( a+(player_height) > bo+height && getentityvar(player,"ladder") != NULL() && (get_action("moveup") == 16 || get_action("moveright") == 8) ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a(x+t_dist,z,a+T_TOP_EDGE,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                float w_altitude = checkwall(x+t_dist,z);

                if (platform) {
                    float altitude = getentityproperty(platform, "y");
                    if ( getentityproperty(platform, "height") != NULL() ) altitude += getentityproperty(platform, "height");

                    // Però potrebbe esserci una piattaforma in più che ostruisce. verifichiamo!
                    void platform_up = find_platform_in_a(x+t_dist,z,altitude+player_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                    //drawstring( 10,200,0,"PLAT:  "+platform_up+"   "+platform);
                    if ( (!platform_up || platform_up == platform) && a <= altitude && altitude-a-player_height <= max_t_overclimb) {
                        float z_dist = 0;
                        if ( find_platform_in_a(x+t_dist,z-z_dist,a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY) == platform ) z_dist = t_dist/2;

                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x+t_dist,z-z_dist,altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW54"), 1);
                        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                        AI_ladder_reset(player,1);
                    } else {
                        // FARE CHE NON SALE
                        changeentityproperty(player, "tosstime", T_TOSSTIME);
                        changeentityproperty(player, "position", x,z,a-mul_shifta);
                        reset_lost_frames();
                    }
                } else if ( a+T_TOP_EDGE <= w_altitude && a+T_TOP_EDGE >= w_altitude-max_t_overclimb ) { // FARE MURO
                        float z_dist = 0;
                        if ( find_platform_in_a(x+t_dist,z-z_dist,a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY) == platform ) z_dist = t_dist/2;

                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x+t_dist,z-z_dist,w_altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW54"), 1);
                        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                        AI_ladder_reset(player,1);
                } else { // else finale fai come move left/right
                    if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            } else if ( getentityvar(player,"ladder") != NULL() ) {
                if ( a+(player_height+player_height/4) < ao && ao > bo ) { // se la scala è in aria e non c'è una base su cui la scala si appoggia...
                    if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                } else if ( a <= getentityproperty(ladder, "base") && ao <= bo ) {
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    setidle(player, openborconstant("ANI_IDLE"), 1);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            }
        } // fine if exists

    } else if ( anim_id == openborconstant("ANI_FOLLOW52") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        float speed = getentityproperty(player, "speed");
        float player_height = getentityproperty(player, "height");
        float shifta = speed/2, mul_shifta;
        float t_dist = T_DIST;
        float max_t_overclimb = T_ACTION_RADIUS;
        void ladder = getentityvar(player,"ladder");
        int lost_frames = get_lost_frames()+MIN_LOST_FRAME;

        if ( lost_frames == 0 ) lost_frames = 1;
        if ( lost_frames > MAX_LOST_FRAMES ) { lost_frames = 0; reset_lost_frames(); }
        mul_shifta = sum(shifta,lost_frames);

        if ( player_height < T_ENT_HEIGHT || player_height == NULL() ) player_height = T_ENT_HEIGHT; // height protection

        check_subjects(player);
        if ( getentityproperty(player, "direction") != 0 ) changeentityproperty(player, "direction", 0);
        if ( getentityproperty(ladder, "exists") ) {
            float width = getentityvar(ladder,"width");
            float height = getentityvar(ladder,"height");
            float bo = getentityproperty(ladder,"base");
            float ao = getentityproperty(ladder,"y");

            generate_action_for_ladder(player);

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( get_action("moveup") == 16 || get_action("moveleft") == 4 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                // calcoliamo di quando il player deve andare avanti
                float new_a = a+shifta;
                float shiftx = (width - ((new_a-bo)*width)/height)/openborvariant("game_speed");

                changeentityproperty(player, "position", x-shiftx,z,a+mul_shifta);
                //reset_lost_frames();
            } else if ( get_action("movedown") == 32 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                float new_a = a-shifta;
                float shiftx = (((new_a-bo)*width)/height)/openborvariant("game_speed");

                changeentityproperty(player, "position", x+shiftx,z,a-mul_shifta );
                //reset_lost_frames();
            } else if ( get_action("moveright") == 8 ) {
                changeentityproperty(player, "direction", 1);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+(shifta*2)); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                jump_from_climbing(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("start") == 2 ) { // walkoff
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                setidle(player,openborconstant("ANI_IDLE"),1);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            if ( a+(player_height) > bo+height && getentityvar(player,"ladder") != NULL() && (get_action("moveup") == 16 || get_action("moveleft") == 4) ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a(x-t_dist,z,a+T_TOP_EDGE,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                float w_altitude = checkwall(x-t_dist,z);

                if (platform) {
                    float altitude = getentityproperty(platform, "y");
                    if ( getentityproperty(platform, "height") != NULL() ) altitude += getentityproperty(platform, "height");

                    // Però potrebbe esserci una piattaforma in più che ostruisce. verifichiamo!
                    void platform_up = find_platform_in_a(x-t_dist,z,altitude+player_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                    //drawstring( 10,200,0,"PLAT:  "+platform_up+"   "+platform);
                    if ( (!platform_up || platform_up == platform) && a <= altitude && altitude-a-player_height <= max_t_overclimb) {
                        float z_dist = 0;
                        if ( find_platform_in_a(x-t_dist,z-z_dist,a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY) == platform ) z_dist = t_dist/2;

                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x-t_dist,z-z_dist,altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW54"), 1);
                        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                        AI_ladder_reset(player,1);
                    } else {
                        // FARE CHE NON SALE
                        changeentityproperty(player, "tosstime", T_TOSSTIME);
                        changeentityproperty(player, "position", x,z,a-mul_shifta);
                        reset_lost_frames();
                    }
                } else if ( a+T_TOP_EDGE <= w_altitude && a+T_TOP_EDGE >= w_altitude-max_t_overclimb ) { // FARE MURO
                        float z_dist = 0;
                        if ( find_platform_in_a(x-t_dist,z-z_dist,a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY) == platform ) z_dist = t_dist/2;

                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x-t_dist,z-z_dist,w_altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW54"), 1);
                        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                        AI_ladder_reset(player,1);
                } else { // else finale fai come move left/right
                    if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            } else if ( getentityvar(player,"ladder") != NULL() ) {
                if ( a+(player_height+player_height/4) < ao && ao > bo ) { // se la scala è in aria e non c'è una base su cui la scala si appoggia...
                    if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                } else if ( a <= getentityproperty(ladder, "base") && ao <= bo ) {
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    setidle(player, openborconstant("ANI_IDLE"), 1);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            }
        } // fine if exists

    } else if ( anim_id == openborconstant("ANI_FOLLOW57") || anim_id == openborconstant("ANI_FOLLOW58") ) {
        float x = getentityproperty(player, "x");
        float z = getentityproperty(player, "z");
        float a = getentityproperty(player, "y");
        float base = getentityproperty(player, "base");
        float speed = getentityproperty(player, "speed");
        float player_height = getentityproperty(player, "height");
        int anim_id = getentityproperty(player, "animationid");
        float shifta = speed/2, mul_shifta;
        float shiftx = speed/2, mul_shiftx;
        float t_dist = T_DIST;
        float x_dist = shiftx;
        float max_t_overclimb = T_ACTION_RADIUS;
        void ladder = getentityvar(player,"ladder");
        int lost_frames = get_lost_frames()+MIN_LOST_FRAME;

        if ( lost_frames == 0 ) lost_frames = 1;
        if ( lost_frames > MAX_LOST_FRAMES ) { lost_frames = 0; reset_lost_frames(); }
        mul_shifta = sum(shifta,lost_frames);
        mul_shiftx = sum(shiftx,lost_frames);

        if ( player_height < T_ENT_HEIGHT || player_height == NULL() ) player_height = T_ENT_HEIGHT; // height protection

        check_subjects(player);
        if ( getentityproperty(ladder, "exists") ) {
            float width = getentityvar(ladder,"width");
            float height = getentityvar(ladder,"height");
            float bo = getentityproperty(ladder,"base");
            float ao = getentityproperty(ladder,"y");

            generate_action_for_climbable_area(player);

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( get_action("moveup") == 16 ) {
                if ( anim_id != openborconstant("ANI_FOLLOW57") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW57"));
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x,z,a+mul_shifta);
                //reset_lost_frames();
            } else if ( get_action("movedown") == 32 ) {
                if ( anim_id != openborconstant("ANI_FOLLOW57") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW57"));
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x,z,a-mul_shifta);
                //reset_lost_frames();
            } else if ( get_action("moveleft") == 4 ) { // CAMMINA DI LATO L
                if ( anim_id != openborconstant("ANI_FOLLOW58") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW58"));
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x-mul_shiftx,z,a);
                //reset_lost_frames();
            } else if ( get_action("moveright") == 8 ) { // CAMMINA DI LATO R
                if ( anim_id != openborconstant("ANI_FOLLOW58") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW58"));
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x+mul_shiftx,z,a);
                //reset_lost_frames();
            } else if ( get_action("jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else if ( get_action("start") == 2 ) { // walkoff
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                setidle(player,openborconstant("ANI_IDLE"),1);
                if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                AI_ladder_reset(player,1);
            } else {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            //drawstring( 10,160,0,"hole:  "+checkhole(x,z));
            // VERIFICA PRESENZA LATERALE DI PIATTAFORME/MURI
            if ( getentityvar(player,"ladder") != NULL() && get_action("moveright") == 8 ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a_with_height(x+x_dist,z,a,player_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                float w_altitude = checkwall(x+x_dist,z);

                if ( platform || w_altitude > 0 ) { // RIMANE BLOCCATO DALLA PIATTAFORMA
                    changeentityproperty(player, "tosstime", T_TOSSTIME);
                    changeentityproperty(player, "position", x-mul_shiftx,z,a);
                    reset_lost_frames();
                } else if ( x > getentityproperty(ladder, "x")+width ) { // CADERE PER WIDTH: Se raggiunta la width allora cadi
                    changeentityproperty(player, "direction", 1);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+(shifta*2)); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            } else if ( getentityvar(player,"ladder") != NULL() && get_action("moveleft") == 4 ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a_with_height(x-x_dist,z,a,player_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                float w_altitude = checkwall(x-x_dist,z);

                if ( platform || w_altitude > 0 ) { // RIMANE BLOCCATO DALLA PIATTAFORMA
                    changeentityproperty(player, "tosstime", T_TOSSTIME);
                    changeentityproperty(player, "position", x+mul_shiftx,z,a);
                    reset_lost_frames();
                } else if ( x < getentityproperty(ladder, "x") ) { // CADERE PER WIDTH: Se raggiunta la width allora cadi
                    changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+(shifta*2)); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            }

            // SALI
            if ( a+(player_height) > ao+height && getentityvar(player,"ladder") != NULL() && get_action("moveup") == 16 ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a(x,z-t_dist,a+T_TOP_EDGE,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                float w_altitude = checkwall(x,z-t_dist);

                if (platform) {
                    float altitude = getentityproperty(platform, "y");
                    if ( getentityproperty(platform, "height") != NULL() ) altitude += getentityproperty(platform, "height");

                    // Però potrebbe esserci una piattaforma in più che ostruisce. verifichiamo!
                    void platform_up = find_platform_in_a(x,z-t_dist,altitude+player_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                    //drawstring( 10,200,0,"PLAT:  "+platform_up+"   "+platform);
                    if ( (!platform_up || platform_up == platform) && a <= altitude && altitude-a-player_height <= max_t_overclimb) { // SALE SULLA PIATTAFORMA
                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x,z-t_dist,altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW53"), 1);
                        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                        AI_ladder_reset(player,1);
                    } else { // NON SALE MA CONTINUA AD ARRAMPICARSI
                        changeentityproperty(player, "tosstime", T_TOSSTIME);
                        changeentityproperty(player, "position", x,z,a-mul_shifta);
                        //reset_lost_frames();
                    }
                } else if ( a+T_TOP_EDGE <= w_altitude && a+T_TOP_EDGE >= w_altitude-max_t_overclimb ) { //  // SALE SUL MURO
                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x,z-t_dist,w_altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW53"), 1);
                        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                        AI_ladder_reset(player,1);
                } else {  // NON TROVA SPORGENZA QUINDI CADE
                    if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            } else if ( getentityvar(player,"ladder") != NULL() ) { // SCENDI A TERRA
                if ( a+(player_height+player_height/4) < ao && ao > bo ) { // SCALA SOSPESA QUINDI CADI: se la scala è in aria e non c'è una base su cui la scala si appoggia...
                    if ( get_action("moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( get_action("moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                } else if ( a <= getentityproperty(ladder, "base") && ao <= bo ) { // SCENDI A TERRA
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    setidle(player, openborconstant("ANI_IDLE"), 1);
                    if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
                    AI_ladder_reset(player,1);
                }
            }
        } // fine if exists
        /*else {
            changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
            setidle(player,openborconstant("ANI_IDLE"),1);
            if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
            AI_ladder_reset(player,1);
        }*/
    } // fine if follow#

    if ( getentityvar(player,"ladder") != NULL()
        && anim_id != openborconstant("ANI_FOLLOW46") && anim_id != openborconstant("ANI_FOLLOW47") && anim_id != openborconstant("ANI_FOLLOW48") && anim_id != openborconstant("ANI_FOLLOW49")
        && anim_id != openborconstant("ANI_FOLLOW50") && anim_id != openborconstant("ANI_FOLLOW51") && anim_id != openborconstant("ANI_FOLLOW52") && anim_id != openborconstant("ANI_FOLLOW55")
        && anim_id != openborconstant("ANI_FOLLOW56") && anim_id != openborconstant("ANI_FOLLOW57") && anim_id != openborconstant("ANI_FOLLOW58") && anim_id != openborconstant("ANI_FOLLOW59")
        && anim_id != openborconstant("ANI_FOLLOW120") && anim_id != openborconstant("ANI_FOLLOW121") ) {

        if ( getentityproperty(player, "setlayer") != getentityvar(player,"setlayer") ) changeentityproperty(player, "setlayer", getentityvar(player,"setlayer"));
        AI_ladder_reset(player,1);
        reset_lost_frames();
    }
}

int set_falling_dir(void player) {
    float speed = getentityproperty(player, "speed");
    float xdir = NULL(), zdir = NULL();

    if ( get_action("moveup") == 16 ) zdir = -1*(speed/2);
    else if ( get_action("movedown") == 32 ) zdir = speed/2;
    if ( get_action("moveright") == 8 ) xdir = speed;
    else if ( get_action("moveleft") == 4 ) xdir = -1*speed;

    changeentityproperty(player, "velocity", xdir, zdir, NULL());
}

