
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"

#import "data/scripts/player_grab.c"
#import "data/scripts/player_bind_grab.c"

#import "data/scripts/ladder_commons.c"
#import "data/scripts/flying_script.c"

#define T_ACTION_RADIUS 20
#define T_TOP_EDGE 10
#define MIN_LOST_FRAME 0
#define T_TOSSTIME get_T_TOSSTIME()
#define T_DIST get_T_DIST()
#define MAX_LOST_FRAMES truncA(ov("game_speed")*1.5)
#define T_LAYER get_T_LAYER()
#define CHANGE_LAYER_FLAG get_CHANGE_LAYER_FLAG()

/*
 * Line Changes:
 *
 */

int check_climb(void player) {
    int anim_id = getentityproperty(player, "animationid");
    int p = getentityproperty(player, "playerindex");

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
        void ladder = getglobalvar("p"+p+"ladder");
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

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( playerkeys(p,0,"moveup") == 16 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x,z,a+mul_shifta);
                //reset_lost_frames();
            } else if ( playerkeys(p,0,"movedown") == 32 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x,z,a-mul_shifta);
                //reset_lost_frames();
            } else if ( playerkeys(p,1,"moveleft") == 4 || playerkeys(p,1,"moveright") == 8 ) {
                if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+(shifta*2));
                jump_from_climbing(player);
                player_ladder_reset(player);
            } else if (  (getplayerproperty(p, "keys") <= 2 || getplayerproperty(p, "keys") == 512)  ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            } else if ( playerkeys(p,1,"jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                player_ladder_reset(player);
            } else {
                changeplayerproperty(p, "keys", 0);
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            // SALI
            if ( a+(player_height) > ao+height && getglobalvar("p"+p+"ladder") != NULL() && playerkeys(p,0,"moveup") == 16 ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a(x,z-t_dist,a+T_TOP_EDGE,MAX_ALTITUDE);
                float w_altitude = checkwall(x,z-t_dist);

                if (platform) {
                    float altitude = getentityproperty(platform, "y");
                    if ( getentityproperty(platform, "height") != NULL() ) altitude += getentityproperty(platform, "height");

                    // Però potrebbe esserci una piattaforma in più che ostruisce. verifichiamo!
                    void platform_up = find_platform_in_a(x,z-t_dist,altitude+player_height,MAX_ALTITUDE);
                    //drawstring( 10,200,0,"PLAT:  "+platform_up+"   "+platform);
                    if ( (!platform_up || platform_up == platform) && a <= altitude && altitude-a-player_height <= max_t_overclimb) { // SALE SULLA PIATTAFORMA
                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x,z-t_dist,altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW53"), 1);
                        player_ladder_reset(player);
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
                        player_ladder_reset(player);
                } else {  // NON TROVA SPORGENZA QUINDI CADE
                    if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    player_ladder_reset(player);
                }
            } else if ( getglobalvar("p"+p+"ladder") != NULL() ) { // SCENDI A TERRA
                if ( a+(player_height+player_height/4) < ao && ao > bo ) { // SCALA SOSPESA QUINDI CADI: se la scala è in aria e non c'è una base su cui la scala si appoggia...
                    if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    player_ladder_reset(player);
                } else if ( a <= getentityproperty(ladder, "base") && ao <= bo ) { // SCENDI A TERRA
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    setidle(player, openborconstant("ANI_IDLE"), 1);
                    player_ladder_reset(player);
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
        void ladder = getglobalvar("p"+p+"ladder");
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

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( playerkeys(p,0,"moveleft") == 4 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x-mul_shifta,z,a);
                //reset_lost_frames();
            } else if ( playerkeys(p,0,"moveright") == 8 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x+mul_shifta,z,a);
                //reset_lost_frames();
            } else if ( playerkeys(p,0,"moveup") == 16 || playerkeys(p,0,"movedown") == 32 ) {
                if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+(shifta*2));
                jump_from_climbing(player);
                player_ladder_reset(player);
            } else if (  (getplayerproperty(p, "keys") <= 2 || getplayerproperty(p, "keys") == 512)  ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            } else if ( playerkeys(p,1,"jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                player_ladder_reset(player);
            } else {
                changeplayerproperty(p, "keys", 0);
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            // NON TROVA SPORGENZA QUINDI CADE
            if ( (x > xo+width || x < xo) && getglobalvar("p"+p+"ladder") != NULL() && (playerkeys(p,0,"moveleft") == 4 || playerkeys(p,0,"moveright") == 8) ) {
                    if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    player_ladder_reset(player);
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
        void ladder = getglobalvar("p"+p+"ladder");
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

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( playerkeys(p,0,"moveup") == 16 || playerkeys(p,0,"moveright") == 8 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                // calcoliamo di quando il player deve andare avanti
                //float shiftx = 0.01*(width/2); // oppure width/height;
                float new_a = a+shifta;
                float shiftx = (width - ((new_a-bo)*width)/height)/openborvariant("game_speed"); // 200 è costante d'inclinazione in base alla velocità del gioco

                changeentityproperty(player, "position", x+shiftx,z,a+mul_shifta);
                //reset_lost_frames();
            } else if ( playerkeys(p,0,"movedown") == 32 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                float new_a = a-shifta;
                float shiftx = (width - ((new_a-bo)*width)/height)/openborvariant("game_speed");

                changeentityproperty(player, "position", x-shiftx,z,a-mul_shifta );
                //reset_lost_frames();
            } else if ( playerkeys(p,1,"moveleft") == 4 ) {
                changeentityproperty(player, "direction", 0);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+(shifta*2)); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                jump_from_climbing(player);
                player_ladder_reset(player);
            } else if (  (getplayerproperty(p, "keys") <= 2 || getplayerproperty(p, "keys") == 512)  ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            } else if ( playerkeys(p,1,"jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                player_ladder_reset(player);
            } else {
                changeplayerproperty(p, "keys", 0);
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            if ( a+(player_height) > bo+height && getglobalvar("p"+p+"ladder") != NULL() && (playerkeys(p,0,"moveup") == 16 || playerkeys(p,0,"moveright") == 8) ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a(x+t_dist,z,a+T_TOP_EDGE,MAX_ALTITUDE);
                float w_altitude = checkwall(x+t_dist,z);

                if (platform) {
                    float altitude = getentityproperty(platform, "y");
                    if ( getentityproperty(platform, "height") != NULL() ) altitude += getentityproperty(platform, "height");

                    // Però potrebbe esserci una piattaforma in più che ostruisce. verifichiamo!
                    void platform_up = find_platform_in_a(x+t_dist,z,altitude+player_height,MAX_ALTITUDE);
                    //drawstring( 10,200,0,"PLAT:  "+platform_up+"   "+platform);
                    if ( (!platform_up || platform_up == platform) && a <= altitude && altitude-a-player_height <= max_t_overclimb) {
                        float z_dist = 0;
                        if ( find_platform_in_a(x+t_dist,z-z_dist,a,MAX_ALTITUDE) == platform ) z_dist = t_dist/2;

                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x+t_dist,z-z_dist,altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW54"), 1);
                        player_ladder_reset(player);
                    } else {
                        // FARE CHE NON SALE
                        changeentityproperty(player, "tosstime", T_TOSSTIME);
                        changeentityproperty(player, "position", x,z,a-mul_shifta);
                        reset_lost_frames();
                    }
                } else if ( a+T_TOP_EDGE <= w_altitude && a+T_TOP_EDGE >= w_altitude-max_t_overclimb ) { // FARE MURO
                        float z_dist = 0;
                        if ( find_platform_in_a(x+t_dist,z-z_dist,a,MAX_ALTITUDE) == platform ) z_dist = t_dist/2;

                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x+t_dist,z-z_dist,w_altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW54"), 1);
                        player_ladder_reset(player);
                } else { // else finale fai come move left/right
                    if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    player_ladder_reset(player);
                }
            } else if ( getglobalvar("p"+p+"ladder") != NULL() ) {
                if ( a+(player_height+player_height/4) < ao && ao > bo ) { // se la scala è in aria e non c'è una base su cui la scala si appoggia...
                    if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    player_ladder_reset(player);
                } else if ( a <= getentityproperty(ladder, "base") && ao <= bo ) {
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    setidle(player, openborconstant("ANI_IDLE"), 1);
                    player_ladder_reset(player);
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
        void ladder = getglobalvar("p"+p+"ladder");
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

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( playerkeys(p,0,"moveup") == 16 || playerkeys(p,0,"moveleft") == 4 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                // calcoliamo di quando il player deve andare avanti
                float new_a = a+shifta;
                float shiftx = (width - ((new_a-bo)*width)/height)/openborvariant("game_speed");

                changeentityproperty(player, "position", x-shiftx,z,a+mul_shifta);
                //reset_lost_frames();
            } else if ( playerkeys(p,0,"movedown") == 32 ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                float new_a = a-shifta;
                float shiftx = (width - ((new_a-bo)*width)/height)/openborvariant("game_speed");

                changeentityproperty(player, "position", x+shiftx,z,a-mul_shifta );
                //reset_lost_frames();
            } else if ( playerkeys(p,1,"moveright") == 8 ) {
                changeentityproperty(player, "direction", 1);
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                changeentityproperty(player, "position", x,z,a+(shifta*2)); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                jump_from_climbing(player);
                player_ladder_reset(player);
            } else if (  (getplayerproperty(p, "keys") <= 2 || getplayerproperty(p, "keys") == 512)  ) {
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            } else if ( playerkeys(p,1,"jump") == 128 ) {
                changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                jump(player);
                player_ladder_reset(player);
            } else {
                changeplayerproperty(p, "keys", 0);
                changeentityproperty(player, "tosstime", T_TOSSTIME);
            }

            if ( a+(player_height) > bo+height && getglobalvar("p"+p+"ladder") != NULL() && (playerkeys(p,0,"moveup") == 16 || playerkeys(p,0,"moveleft") == 4) ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a(x-t_dist,z,a+T_TOP_EDGE,MAX_ALTITUDE);
                float w_altitude = checkwall(x-t_dist,z);

                if (platform) {
                    float altitude = getentityproperty(platform, "y");
                    if ( getentityproperty(platform, "height") != NULL() ) altitude += getentityproperty(platform, "height");

                    // Però potrebbe esserci una piattaforma in più che ostruisce. verifichiamo!
                    void platform_up = find_platform_in_a(x-t_dist,z,altitude+player_height,MAX_ALTITUDE);
                    //drawstring( 10,200,0,"PLAT:  "+platform_up+"   "+platform);
                    if ( (!platform_up || platform_up == platform) && a <= altitude && altitude-a-player_height <= max_t_overclimb) {
                        float z_dist = 0;
                        if ( find_platform_in_a(x-t_dist,z-z_dist,a,MAX_ALTITUDE) == platform ) z_dist = t_dist/2;

                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x-t_dist,z-z_dist,altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW54"), 1);
                        player_ladder_reset(player);
                    } else {
                        // FARE CHE NON SALE
                        changeentityproperty(player, "tosstime", T_TOSSTIME);
                        changeentityproperty(player, "position", x,z,a-mul_shifta);
                        reset_lost_frames();
                    }
                } else if ( a+T_TOP_EDGE <= w_altitude && a+T_TOP_EDGE >= w_altitude-max_t_overclimb ) { // FARE MURO
                        float z_dist = 0;
                        if ( find_platform_in_a(x-t_dist,z-z_dist,a,MAX_ALTITUDE) == platform ) z_dist = t_dist/2;

                        changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                        changeentityproperty(player, "position", x-t_dist,z-z_dist,w_altitude);
                        //jump(player);
                        performattack(player, openborconstant("ANI_FOLLOW54"), 1);
                        player_ladder_reset(player);
                } else { // else finale fai come move left/right
                    if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    player_ladder_reset(player);
                }
            } else if ( getglobalvar("p"+p+"ladder") != NULL() ) {
                if ( a+(player_height+player_height/4) < ao && ao > bo ) { // se la scala è in aria e non c'è una base su cui la scala si appoggia...
                    if ( playerkeys(p,1,"moveright") == 8 ) changeentityproperty(player, "direction", 1);
                    else if ( playerkeys(p,1,"moveleft") == 4 ) changeentityproperty(player, "direction", 0);
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    changeentityproperty(player, "position", x,z,a+shifta); // se cambiamo la "y" non accade il bug quando fa landing con aiflag_jump su on
                    jump_from_climbing(player);
                    player_ladder_reset(player);
                } else if ( a <= getentityproperty(ladder, "base") && ao <= bo ) {
                    changeentityproperty(player, "tosstime", openborvariant("elapsed_time")-1);
                    setidle(player, openborconstant("ANI_IDLE"), 1);
                    player_ladder_reset(player);
                }
            }
        } // fine if exists

    } // fine if follow#

    if ( getglobalvar("p"+p+"ladder") != NULL()
        && anim_id != openborconstant("ANI_FOLLOW46") && anim_id != openborconstant("ANI_FOLLOW47") && anim_id != openborconstant("ANI_FOLLOW48") && anim_id != openborconstant("ANI_FOLLOW49")
        && anim_id != openborconstant("ANI_FOLLOW50") && anim_id != openborconstant("ANI_FOLLOW51") && anim_id != openborconstant("ANI_FOLLOW52") && anim_id != openborconstant("ANI_FOLLOW55")
        && anim_id != openborconstant("ANI_FOLLOW56") && anim_id != openborconstant("ANI_FOLLOW57") && anim_id != openborconstant("ANI_FOLLOW58") && anim_id != openborconstant("ANI_FOLLOW59")
        && anim_id != openborconstant("ANI_FOLLOW120") && anim_id != openborconstant("ANI_FOLLOW121") ) {

        player_ladder_reset(player);
        reset_lost_frames();
    }
}

int check_ladder(void player, void opp) {
    //void opp = getentityproperty(player, "opponent");
    int i;
    void grabbed_opp, grabbing;
    int count_entities = ov("count_entities"); // openborconstant("MAX_ENTS")
    int ent_flag = 0;

    if ( opp != NULL() ) {
        count_entities = 1;
        ent_flag = 1;
    }

    //reset_opp_by_range(player,opp,openborconstant("ANI_IDLE"));

    grabbed_opp = grabbed_by(player);
    grabbing = getentityproperty(player,"grabbing");

    if ( !getentityproperty(grabbed_opp,"exists") && !getentityproperty(grabbing,"exists") && !getentityproperty(get_grabber(player),"exists")
        && !getentityproperty(get_grabbed_opp(player),"exists") && !getentityproperty(get_main_opp(player),"exists") ) {
        for (i = 0; i < count_entities; ++i) {
            if ( !ent_flag ) opp = getentity(i);

                if ( getentityproperty(opp, "exists") ) {
                    float x = getentityproperty(player, "x");
                    float z = getentityproperty(player, "z");
                    float a = getentityproperty(player, "y");
                    float base = getentityproperty(player, "base");
                    float speed = getentityproperty(player, "speed");

                    if ( getentityvar(opp, "type") == "ladder_v" || getentityvar(opp, "type") == "ladder_h" || getentityvar(opp, "type") == "ladder_l" || getentityvar(opp, "type") == "ladder_r" ) { // cambiare in getentityvar 0 == "ladder_v"
                            float width = getentityvar(opp,"width");
                            float height = getentityvar(opp,"height");
                            float xo = getentityproperty(opp, "x");
                            float zo = getentityproperty(opp, "z");
                            float ao = getentityproperty(opp, "y");
                            float bo = getentityproperty(opp, "base");
                            int p = getentityproperty(player, "playerindex");
                            float player_height = getentityproperty(player, "height");
                            int anim_id = getentityproperty(player, "animationid");

                            //drawstring( 10,100,0,"base: "+base+"  bo: "+bo+"  height: "+height);

                            if ( player_height < T_ENT_HEIGHT || player_height == NULL() ) player_height = T_ENT_HEIGHT; // height protection

                                //drawstring( 10,160,0,"bo+h:  "+(bo+height));
                                /*
                                drawstring( 10,160,0,"opp:  "+opp);
                                drawstring( 10,170,0,"a:  "+getentityproperty(player, "y"));
                                drawstring( 10,200,0,"base_ladder:  "+bo);
                                drawstring( 10,210,0,"a_ladder:  "+ao);
                                */
                            if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1 ) {
                                            //drawstring( 10,160,0,"bo:  "+bo+"   "+"  ao:  "+ao+"  a:  "+a);
                                            if ( (is_action_btn_pressed_ladder_by_down(p) || is_action_btn_pressed_ladder_by_air(p))
                                                && anim_id != openborconstant("ANI_FOLLOW50") && anim_id != openborconstant("ANI_FOLLOW51") && anim_id != openborconstant("ANI_FOLLOW52")
                                                && anim_id != openborconstant("ANI_FOLLOW120") ) {
                                                    if ( x >= xo-T_ACTION_RADIUS && x <= xo+width+T_ACTION_RADIUS && z >= zo-T_ACTION_RADIUS && z <= zo+T_ACTION_RADIUS ) {
                                                        //float shiftx = width - ((a-bo)*width)/height; // reverse
                                                        //x:(a-bo)=width:height -> (a-bo)*width=x*height -> x = ((a-bo)*width)/height
                                                        //shiftx = width - ((a-bo)*width)/height; // reverse (from 0 (top) to width (land)
                                                        //shiftx += width/3; // sommiamo una parte per shiftare di una costante in base alla scala

                                                        if ( !gep(player,"animvalid",oc("ANI_FOLLOW50")) || !gep(player,"animvalid",oc("ANI_FOLLOW51")) || !gep(player,"animvalid",oc("ANI_FOLLOW52"))
                                                            || !gep(player,"animvalid",oc("ANI_FOLLOW120")) ) return 0;

                                                        // Significa: da base
                                                        if ( is_action_btn_pressed_ladder_by_down(p) && a == bo && bo == ao
                                                            && anim_id != openborconstant("ANI_JUMP") && anim_id != openborconstant("ANI_WALKOFF") ) {
                                                               /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                    damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                    damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                               }*/

                                                               if ( is_flying(player) ) reset_fly(player);
                                                               reset_lost_frames();
                                                               setglobalvar("climb_press_button", 1);

                                                               changeentityproperty(player, "velocity", 0,0, 0.0001);
                                                               setglobalvar("p"+p+"ladder", opp);
                                                               setentityvar(player,"ladder",opp);
                                                               if ( find_platform_in_a(x,z,a-5,MAX_ALTITUDE) != NULL() ) { // Siamo su una piattaforma?
                                                                    setglobalvar("p"+p+"layer", getentityproperty(player, "setlayer"));
                                                                    if ( CHANGE_LAYER_FLAG ) changeentityproperty(player, "setlayer", getentityproperty(opp, "setlayer")+T_LAYER);
                                                               }
                                                               changeentityproperty(player, "base", bo+(speed/2) );
                                                               changeentityproperty(player, "tosstime", get_next_time(openborvariant("game_speed")*1));
                                                               if ( getentityvar(opp, "type") == "ladder_v" ) {
                                                                    changeentityproperty(player, "position", xo+(width/2),zo+1,a+(speed/2));
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    //changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW50"));
                                                                    performattack(player, openborconstant("ANI_FOLLOW50"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_h" ) {
                                                                    changeentityproperty(player, "position", NULL(),zo+1,ao-(height/2+player_height));
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    //changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW50"));
                                                                    performattack(player, openborconstant("ANI_FOLLOW120"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_l" ) {
                                                                    float new_a = a+(speed/2);
                                                                    float shiftx = width - ((new_a-bo)*width)/height; // calcoliamo quando deve stare di lato sulla scala se si aggrappa in volo
                                                                    changeentityproperty(player, "position", xo-shiftx+width-1,zo+1,new_a);
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    performattack(player, openborconstant("ANI_FOLLOW51"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_r" ) {
                                                                    float new_a = a+(speed/2);
                                                                    float shiftx = width - ((new_a-bo)*width)/height; // calcoliamo quando deve stare di lato sulla scala se si aggrappa in volo
                                                                    changeentityproperty(player, "position", xo+shiftx,zo+1,new_a);
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    performattack(player, openborconstant("ANI_FOLLOW52"), 1);
                                                               }
                                                        } else if ( is_action_btn_pressed_ladder_by_air(p) && (a <= ao+height && a+(player_height) >= ao)
                                                                   && (anim_id == openborconstant("ANI_JUMP") || anim_id == openborconstant("ANI_WALKOFF")) ) { // Significa: in volo
                                                               /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                    damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                    damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                               }*/

                                                               if ( is_flying(player) ) reset_fly(player);
                                                               reset_lost_frames();
                                                               setglobalvar("climb_press_button", 1);

                                                               changeentityproperty(player, "velocity", 0,0, 0.0001);
                                                               setglobalvar("p"+p+"ladder", opp);
                                                               setentityvar(player,"ladder",opp);
                                                               if ( is_a_platform_below(opp) == 1 ) { // Siamo su una piattaforma?
                                                                    setglobalvar("p"+p+"layer", getentityproperty(player, "setlayer"));
                                                                    if ( CHANGE_LAYER_FLAG ) changeentityproperty(player, "setlayer", getentityproperty(opp, "setlayer")+T_LAYER);
                                                               }
                                                               changeentityproperty(player, "base", base+(speed/2) );
                                                               changeentityproperty(player, "tosstime", get_next_time(openborvariant("game_speed")*1));
                                                               if ( getentityvar(opp, "type") == "ladder_v" ) {
                                                                    changeentityproperty(player, "position", xo+(width/2),zo+1,a+(speed/2));
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    performattack(player, openborconstant("ANI_FOLLOW50"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_h" ) {
                                                                    changeentityproperty(player, "position", NULL(),zo+1,ao-(height/2+player_height));
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    performattack(player, openborconstant("ANI_FOLLOW120"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_l" ) {
                                                                    float new_a = a+(speed/2);
                                                                    float shiftx = width - ((new_a-bo)*width)/height; // calcoliamo quando deve stare di lato sulla scala se si aggrappa in volo
                                                                    changeentityproperty(player, "position", xo-shiftx+width-1,zo+1,new_a);
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    if ( !getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 1);
                                                                    performattack(player, openborconstant("ANI_FOLLOW51"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_r" ) {
                                                                    float new_a = a+(speed/2);
                                                                    float shiftx = width - ((new_a-bo)*width)/height; // calcoliamo quando deve stare di lato sulla scala se si aggrappa in volo
                                                                    changeentityproperty(player, "position", xo+shiftx,zo+1,new_a);
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    if ( getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 0);
                                                                    performattack(player, openborconstant("ANI_FOLLOW52"), 1);
                                                               }
                                                        } // fine if in volo

                                                    } // fine if coords
                                            } // fine if playkeys

                                            // dalla cima
                                            if ( is_action_btn_pressed_ladder_by_up(p) && is_on_base(player)
                                                && anim_id != openborconstant("ANI_FOLLOW50") && anim_id != openborconstant("ANI_FOLLOW51") && anim_id != openborconstant("ANI_FOLLOW52")
                                                && anim_id != openborconstant("ANI_FOLLOW55") && anim_id != openborconstant("ANI_FOLLOW56") && anim_id != openborconstant("ANI_FOLLOW120")
                                                && anim_id != openborconstant("ANI_FOLLOW121") ) {

                                                        if ( !gep(player,"animvalid",oc("ANI_FOLLOW55")) || !gep(player,"animvalid",oc("ANI_FOLLOW56")) || !gep(player,"animvalid",oc("ANI_FOLLOW121")) ) return 0;

                                                        if ( x >= xo-T_ACTION_RADIUS && x <= xo+width+T_ACTION_RADIUS && z >= zo-T_ACTION_RADIUS-10 && z <= zo+T_ACTION_RADIUS+10 ) {
                                                            if ( base >= bo+height-T_ACTION_RADIUS && a <= bo+height+T_ACTION_RADIUS ) { // dalla cima
                                                                   /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                        damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                        damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                                   }*/

                                                                   if ( is_flying(player) ) reset_fly(player);
                                                                   reset_lost_frames();
                                                                   setglobalvar("climb_press_button", 1);

                                                                    if ( getentityvar(opp, "type") == "ladder_v" ) {
                                                                        changeentityproperty(player, "velocity", 0,0,0);
                                                                        setglobalvar("p"+p+"ladder", opp);
                                                                        setentityvar(player,"ladder",opp);
                                                                        changeentityproperty(player, "position", xo+width/2,NULL(),NULL());
                                                                        performattack(player, openborconstant("ANI_FOLLOW55"), 1);
                                                                    } else if ( getentityvar(opp, "type") == "ladder_h" ) {
                                                                        changeentityproperty(player, "velocity", 0,0,0);
                                                                        setglobalvar("p"+p+"ladder", opp);
                                                                        setentityvar(player,"ladder",opp);
                                                                        //changeentityproperty(player, "position", NULL(),NULL(),ao-(height/2+player_height));
                                                                        performattack(player, openborconstant("ANI_FOLLOW121"), 1);
                                                                    } else if ( getentityvar(opp, "type") == "ladder_l" ) {
                                                                        changeentityproperty(player, "velocity", 0,0,0);
                                                                        if ( !getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 1);
                                                                        setglobalvar("p"+p+"ladder", opp);
                                                                        setentityvar(player,"ladder",opp);
                                                                        performattack(player, openborconstant("ANI_FOLLOW56"), 1);
                                                                    } else if ( getentityvar(opp, "type") == "ladder_r" ) {
                                                                        changeentityproperty(player, "velocity", 0,0,0);
                                                                        if ( getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 0);
                                                                        setglobalvar("p"+p+"ladder", opp);
                                                                        setentityvar(player,"ladder",opp);
                                                                        performattack(player, openborconstant("ANI_FOLLOW56"), 1);
                                                                    }
                                                            } // fine if dalla cima
                                                        } // fine if coords
                                            } // fine if playkeys
                                    //changeentityproperty(player, "hitbyid", 0); // Resettiamo la pausa attacco
                            } // fine if pains

                    } // fine if ladder
                } // fine if exists
        } // fine for getentity
    } // fine if grabbing

    return 0;
}


