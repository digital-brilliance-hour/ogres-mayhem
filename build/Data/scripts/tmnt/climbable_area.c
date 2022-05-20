
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

int check_climb_area(void player) {
    int anim_id = getentityproperty(player, "animationid");
    int p = getentityproperty(player, "playerindex");

    if ( anim_id == openborconstant("ANI_FOLLOW57") || anim_id == openborconstant("ANI_FOLLOW58") ) {
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
        void ladder = getglobalvar("p"+p+"ladder");
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

            changeentityproperty(player, "velocity", 0,0,NULL());
            changeentityproperty(player, "aiflag", "jumping", 0);
            changeentityproperty(player, "aiflag", "running", 0);
            if ( playerkeys(p,0,"moveup") == 16 ) {
                if ( anim_id != openborconstant("ANI_FOLLOW57") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW57"));
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x,z,a+mul_shifta);
                //reset_lost_frames();
            } else if ( playerkeys(p,0,"movedown") == 32 ) {
                if ( anim_id != openborconstant("ANI_FOLLOW57") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW57"));
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x,z,a-mul_shifta);
                //reset_lost_frames();
            } else if ( playerkeys(p,0,"moveleft") == 4 ) { // CAMMINA DI LATO L
                if ( anim_id != openborconstant("ANI_FOLLOW58") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW58"));
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x-mul_shiftx,z,a);
                //reset_lost_frames();
            } else if ( playerkeys(p,0,"moveright") == 8 ) { // CAMMINA DI LATO R
                if ( anim_id != openborconstant("ANI_FOLLOW58") ) changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW58"));
                changeentityproperty(player, "tosstime", T_TOSSTIME);
                changeentityproperty(player, "position", x+mul_shiftx,z,a);
                //reset_lost_frames();
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

            //drawstring( 10,160,0,"hole:  "+checkhole(x,z));
            // VERIFICA PRESENZA LATERALE DI PIATTAFORME/MURI
            if ( getglobalvar("p"+p+"ladder") != NULL() && playerkeys(p,0,"moveright") == 8 ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a_with_height(x+x_dist,z,a,player_height,MAX_ALTITUDE);
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
                    player_ladder_reset(player);
                }
            } else if ( getglobalvar("p"+p+"ladder") != NULL() && playerkeys(p,0,"moveleft") == 4 ) {
                // Prendiamo l'altitudine della piattaforma/muro in su
                void platform = find_platform_in_a_with_height(x-x_dist,z,a,player_height,MAX_ALTITUDE);
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
                    player_ladder_reset(player);
                }
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

int check_climbing(void player, void opp) {
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

                    if ( getentityvar(opp, "type") == "climbing_area" ) { // cambiare in getentityvar 0 == "climbing_area"
                            float width = getentityvar(opp,"width");
                            float height = getentityvar(opp,"height");
                            float xo = getentityproperty(opp, "x");
                            float zo = getentityproperty(opp, "z");
                            float ao = getentityproperty(opp, "y");
                            float bo = getentityproperty(opp, "base");
                            int p = getentityproperty(player, "playerindex");
                            float player_height = getentityproperty(player, "height");
                            int anim_id = getentityproperty(player, "animationid");

                            if ( player_height < T_ENT_HEIGHT || player_height == NULL() ) player_height = T_ENT_HEIGHT; // height protection

                                //drawstring( 10,160,0,"bo+h:  "+(bo+height));
                                /*
                                drawstring( 10,160,0,"opp:  "+opp);
                                drawstring( 10,170,0,"a:  "+getentityproperty(player, "y"));
                                drawstring( 10,200,0,"base_ladder:  "+bo);
                                drawstring( 10,210,0,"a_ladder:  "+ao);
                                */
                            if ( getentityproperty(player, "aiflag", "dead") != 1 && getentityproperty(player, "aiflag", "falling") != 1 && getentityproperty(player, "aiflag", "inpain") != 1 ) {
                                            if ( (is_action_btn_pressed_ladder_by_down(p) || is_action_btn_pressed_ladder_by_air(p))
                                                && anim_id != openborconstant("ANI_FOLLOW57") ) {

                                                    if ( !gep(player,"animvalid",oc("ANI_FOLLOW57")) ) return 0;

                                                    if ( x >= xo-T_ACTION_RADIUS && x <= xo+width+T_ACTION_RADIUS && z >= zo-T_ACTION_RADIUS && z <= zo+T_ACTION_RADIUS ) {
                                                        // Significa: da base
                                                        if ( is_action_btn_pressed_ladder_by_down(p) && a == bo && bo == ao && anim_id != openborconstant("ANI_JUMP") && anim_id != openborconstant("ANI_WALKOFF") ) {

                                                               if ( is_flying(player) ) reset_fly(player);
                                                               reset_lost_frames();
                                                               setglobalvar("climb_press_button", 1);

                                                               /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                    damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                    damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                               }*/

                                                               changeentityproperty(player, "velocity", 0,0, 0.0001);
                                                               setglobalvar("p"+p+"ladder", opp);
                                                               setentityvar(player,"ladder",opp);
                                                               if ( find_platform_in_a(x,z,a-5,MAX_ALTITUDE) != NULL() ) { // Siamo su una piattaforma?
                                                                    setglobalvar("p"+p+"layer", getentityproperty(player, "setlayer"));
                                                                    if ( CHANGE_LAYER_FLAG ) changeentityproperty(player, "setlayer", getentityproperty(opp, "setlayer")+T_LAYER);
                                                               }
                                                               changeentityproperty(player, "base", bo+(speed/2) );
                                                               changeentityproperty(player, "tosstime", get_next_time(openborvariant("game_speed")*1));
                                                               if ( getentityvar(opp, "type") == "climbing_area" ) {
                                                                    changeentityproperty(player, "position", x,zo+1,a+(speed/2));
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    performattack(player, openborconstant("ANI_FOLLOW57"), 1);
                                                               }
                                                        } else if ( is_action_btn_pressed_ladder_by_air(p) && (a <= ao+height && a+(player_height) >= ao) && (anim_id == openborconstant("ANI_JUMP") || anim_id == openborconstant("ANI_WALKOFF")) ) { // Significa: in volo

                                                               if ( is_flying(player) ) reset_fly(player);
                                                               reset_lost_frames();
                                                               setglobalvar("climb_press_button", 1);

                                                               /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                    damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                    damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                               }*/

                                                               changeentityproperty(player, "velocity", 0,0, 0.0001);
                                                               setglobalvar("p"+p+"ladder", opp);
                                                               setentityvar(player,"ladder",opp);
                                                               if ( is_a_platform_below(opp) == 1 ) { // Siamo su una piattaforma?
                                                                    setglobalvar("p"+p+"layer", getentityproperty(player, "setlayer"));
                                                                    if ( CHANGE_LAYER_FLAG ) changeentityproperty(player, "setlayer", getentityproperty(opp, "setlayer")+T_LAYER);
                                                               }
                                                               changeentityproperty(player, "base", base+(speed/2) );
                                                               changeentityproperty(player, "tosstime", get_next_time(openborvariant("game_speed")*1));
                                                               if ( getentityvar(opp, "type") == "climbing_area" ) {
                                                                    changeentityproperty(player, "position", x,zo+1,a+(speed/2));
                                                                    setglobalvar("p"+p+"climbing", 0);
                                                                    performattack(player, openborconstant("ANI_FOLLOW57"), 1);
                                                               }
                                                        } // fine if in volo

                                                    } // fine if coords
                                            } // fine if playkeys

                                            // dalla cima
                                            if ( is_action_btn_pressed_ladder_by_up(p) && is_on_base(player)
                                                && anim_id != openborconstant("ANI_FOLLOW57") && anim_id != openborconstant("ANI_FOLLOW59") ) {

                                                    if ( !gep(player,"animvalid",oc("ANI_FOLLOW59")) ) return 0;

                                                        if ( x >= xo-T_ACTION_RADIUS && x <= xo+width+T_ACTION_RADIUS && z >= zo-T_ACTION_RADIUS-10 && z <= zo+T_ACTION_RADIUS+10 ) {
                                                            if ( base >= bo+height-T_ACTION_RADIUS && a <= bo+height+T_ACTION_RADIUS ) { // dalla cima

                                                                   if ( is_flying(player) ) reset_fly(player);
                                                                   reset_lost_frames();
                                                                   setglobalvar("climb_press_button", 1);

                                                                   /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                        damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                        damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                                   }*/

                                                                   if ( getentityvar(opp, "type") == "climbing_area" ) {
                                                                       changeentityproperty(player, "velocity", 0,0,0);
                                                                       setglobalvar("p"+p+"ladder", opp);
                                                                       setentityvar(player,"ladder",opp);
                                                                       performattack(player, openborconstant("ANI_FOLLOW59"), 1);
                                                                   }
                                                            } // fine if dalla cima
                                                        } // fine if coords
                                            } // fine if playkeys
                            } // fine if pains
                    } // fine if ladder
                } // fine if exists
        } // fine for getentity
    } // fine if grabbing

    return 0;
}



