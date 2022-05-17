
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"

#import "data/scripts/player_grab.c"
#import "data/scripts/player_bind_grab.c"
#import "data/scripts/player_check_entities.c"

#import "data/scripts/ladder_commons.c"
#import "data/scripts/AI_ladder_climbing.c"
#import "data/scripts/AI_ladder_lib.c"
#import "data/scripts/AI_ladder_commons.c"
#import "data/scripts/flying_script.c"

#define THRESHOLD_LADDER_X openborvariant("hresolution")/1 // serching ladder range
#define THRESHOLD_LADDER_Z (openborvariant("scrollmaxz")-openborvariant("scrollminz"))/1
#define T_LADDER_RANGE 5
#define T_LADDER_PROXIMITY 50
#define T_ACTION_RADIUS 20
#define T_INCAM -20
#define T_LADDER_ALT_VISIBILITY get_ladder_alt_visibility()
#define T_LAYER get_T_LAYER()
#define CHANGE_LAYER_FLAG get_CHANGE_LAYER_FLAG()


// DA FARE:
// check check_AI_ladder every N frames
// scrivi search_path_ladder_for_target

void main() {
    //void self = getlocalvar("self");

}

/*int AI_ladder(void self) {

    if ( glv("near_a_ladder") == NULL() ) check_AI_ladder(self); /// check it every N frames

    //goto_ladder_no_ai_control(self,5);
    goto_ladder(self);
    //goto_ladder_waypoints(self);
    check_AI_climb(self);

    //drawstring(10,160,0,"xrange: "+glv("xrange"));
    //drawstring(10,170,0,"arange: "+glv("arange"));
    //drawstring(10,180,0,"no_target: "+glv("no_target"));
    //drawstring(10,190,0,"ladder: "+gev(self,"ladder"));
    //drawstring(10,200,0,"action_flag: "+glv("action_flag"));
    //drawstring(10,210,0,"near a ladder: "+glv("near_a_ladder"));
    //if ( gep(glv("near_a_ladder"),"exists") ) drawstring(10,220,0,"ladder path from: "+gep(glv("near_a_ladder"),"model"));
}*/

// check for a ladder path if the target has a base != self_base (in ondrawscript event)
int check_AI_ladder(void self, void ent) {
    void target = findtarget(self);

    //drawstring(10,230,0,"obstacle: "+getglobalvar("fps"));
    //drawstring(10,220,0,"near_a_ladder: "+glv("near_a_ladder"));

    // check target. target is up/down own base?
    if ( gep(target,"exists") && glv("near_a_ladder") == NULL() ) {
        float base = gep(self,"base");
        float target_base = gep(target,"base");
        float target_x = gep(target,"x");
        float target_z = gep(target,"z");
        int target_anim_id = gep(target,"animationid");

        if ( base != target_base || (base == target_base && is_hole_to_target(self,target)) ||
            ( target_anim_id == oc("ANI_FOLLOW50") || target_anim_id == oc("ANI_FOLLOW51") || target_anim_id == oc("ANI_FOLLOW52") || target_anim_id == oc("ANI_FOLLOW53")
             || target_anim_id == oc("ANI_FOLLOW54") || target_anim_id == oc("ANI_FOLLOW55") || target_anim_id == oc("ANI_FOLLOW56") || target_anim_id == oc("ANI_FOLLOW57")
             || target_anim_id == oc("ANI_FOLLOW58") || target_anim_id == oc("ANI_FOLLOW59") || target_anim_id == oc("ANI_FOLLOW120") || target_anim_id == oc("ANI_FOLLOW121") ) ) {
                void ladder = NULL();

                // 1. USA QUESTO PER UNA LADDER PATH
                //ladder = search_ladder_from_target(NULL(),NULL(),self,target,1);
                //if ( gep(ladder,"exists") ) slv("near_a_ladder",ladder);
                //drawstring(10,220,0,"ladder path from: "+gep(ladder,"model"));

                // 2. OPPURE USA QUESTO PER UNA LADDER PIU' VICINA CHE POTREBBE PORTARE AL TARGET
                ladder = search_ladder_for_target(self,target,1);
                //ladder = search_path_ladder_for_target(self,target);
                //drawstring(10,220,0,"ladder: "+glv("near_a_ladder"));
                if ( gep(ladder,"exists") ) slv("near_a_ladder",ladder);

                //adjustwalkanimation(self, ladder);
                //drawstring(10,220,0,"ladder simple path from: "+gep(ladder,"model"));

        } // fine if target_a
    } // fine if target
}

int is_hole_to_target(void self, void target) {
    if ( ent_exists(self) && ent_exists(target) ) {
        float self_x = gep(self,"x");
        float self_z = gep(self,"z");
        float target_x = gep(target,"x");
        float target_z = gep(target,"z");
        float target_dir_x = 1, target_dir_z = 1;
        float dist_x = 0, dist_z = 0, mid_x = 0, mid_z = 0;
        void plat_self = NULL(), plat_target = NULL();
        float wall_mid = 0;

        if ( target_x < self_x ) target_dir_x = -1;
        if ( target_z < self_z ) target_dir_z = -1;
        dist_x = diff(self_x,target_x)/2;
        dist_z = diff(self_z,target_z)/2;
        mid_x = self_x+target_dir_x*mid_x;
        mid_z = self_z+target_dir_z*mid_z;

        if ( checkhole(mid_x,mid_z) ) return 1;

        plat_self = get_platform_on(self);
        plat_target = get_platform_on(target);
        if ( (plat_self != NULL() || plat_target != NULL()) && plat_self != plat_target ) return 1;

        wall_mid = checkwall(mid_x,mid_z);
        if ( checkwall(self_x,self_z) != wall_mid || checkwall(target_x,target_z) != wall_mid ) return 1;
    }

    return 0;
}

int grab_ladder(void player) {
    //if ( glv("glimbing_a_ladder") == NULL() ) {
        int i;
        void grabbed_opp, grabbing;
        //reset_opp_by_range(player,opp,openborconstant("ANI_IDLE"));

        grabbed_opp = grabbed_by(player);
        grabbing = getentityproperty(player,"grabbing");

        if ( !getentityproperty(grabbed_opp,"exists") && !getentityproperty(grabbing,"exists") && !getentityproperty(get_grabber(player),"exists")
            && !getentityproperty(get_grabbed_opp(player),"exists") && !getentityproperty(get_main_opp(player),"exists") ) {
                void opp = getentityvar(player,"ladder");

                    if ( getentityproperty(opp, "exists") ) {
                        float x = getentityproperty(player, "x");
                        float z = getentityproperty(player, "z");
                        float a = getentityproperty(player, "y");
                        float base = getentityproperty(player, "base");
                        float speed = getentityproperty(player, "speed");

                        if ( getentityvar(opp, "type") == "ladder_v" || getentityvar(opp, "type") == "ladder_h" || getentityvar(opp, "type") == "ladder_l" || getentityvar(opp, "type") == "ladder_r" || getentityvar(opp, "type") == "climbing_area" ) { // cambiare in getentityvar 0 == "ladder_v"
                                float width = getentityvar(opp,"width");
                                float height = getentityvar(opp,"height");
                                float xo = getentityproperty(opp, "x");
                                float zo = getentityproperty(opp, "z");
                                float ao = getentityproperty(opp, "y");
                                float bo = getentityproperty(opp, "base");
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
                                                //drawstring( 10,160,0,"bo:  "+bo+"   "+"  ao:  "+ao+"  a:  "+a);
                                                if (  !is_flying(player)
                                                      && anim_id != openborconstant("ANI_FOLLOW50") && anim_id != openborconstant("ANI_FOLLOW51") && anim_id != openborconstant("ANI_FOLLOW52")
                                                      && anim_id != openborconstant("ANI_FOLLOW57") && anim_id != openborconstant("ANI_FOLLOW120") ) {
                                                        if ( x >= xo-T_ACTION_RADIUS && x <= xo+width+T_ACTION_RADIUS && z >= zo-T_ACTION_RADIUS && z <= zo+T_ACTION_RADIUS ) {
                                                            //float shiftx = width - ((a-bo)*width)/height; // reverse
                                                            //x:(a-bo)=width:height -> (a-bo)*width=x*height -> x = ((a-bo)*width)/height
                                                            //shiftx = width - ((a-bo)*width)/height; // reverse (from 0 (top) to width (land)
                                                            //shiftx += width/3; // sommiamo una parte per shiftare di una costante in base alla scala

                                                            // Significa: da base
                                                            if ( a == bo && bo == ao && anim_id != openborconstant("ANI_JUMP") && anim_id != openborconstant("ANI_WALKOFF") ) {
                                                                   if ( is_flying(player) ) reset_fly(player);
                                                                   reset_lost_frames();

                                                                   /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                        damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                        damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                                   }*/

                                                                   changeentityproperty(player, "velocity", 0,0, 0.0001);
                                                                   if ( find_platform_in_a(x,z,a-5,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY) != NULL() ) { // Siamo su una piattaforma?
                                                                        setentityvar("setlayer", getentityproperty(player, "setlayer"));
                                                                        if ( CHANGE_LAYER_FLAG ) changeentityproperty(player, "setlayer", getentityproperty(opp, "setlayer")+T_LAYER);
                                                                   }
                                                                   changeentityproperty(player, "base", bo+(speed/2) );
                                                                   changeentityproperty(player, "tosstime", get_next_time(openborvariant("game_speed")*1));
                                                                   if ( getentityvar(opp, "type") == "ladder_v" ) {
                                                                        changeentityproperty(player, "position", xo+(width/2),zo+1,a+(speed/2));
                                                                        //changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW50"));
                                                                        performattack(player, openborconstant("ANI_FOLLOW50"), 1);
                                                                   } else if ( getentityvar(opp, "type") == "ladder_h" ) {
                                                                        changeentityproperty(player, "position", NULL(),zo+1,ao-(height/2+player_height));
                                                                        //changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW50"));
                                                                        performattack(player, openborconstant("ANI_FOLLOW120"), 1);
                                                                   } else if ( getentityvar(opp, "type") == "ladder_l" ) {
                                                                        float new_a = a+(speed/2);
                                                                        float shiftx = width - ((new_a-bo)*width)/height; // calcoliamo quando deve stare di lato sulla scala se si aggrappa in volo
                                                                        changeentityproperty(player, "position", xo-shiftx+width-1,zo+1,new_a);
                                                                        if ( !getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 1);
                                                                        performattack(player, openborconstant("ANI_FOLLOW51"), 1);
                                                                   } else if ( getentityvar(opp, "type") == "ladder_r" ) {
                                                                        float new_a = a+(speed/2);
                                                                        float shiftx = width - ((new_a-bo)*width)/height; // calcoliamo quando deve stare di lato sulla scala se si aggrappa in volo
                                                                        changeentityproperty(player, "position", xo+shiftx,zo+1,new_a);
                                                                        if ( getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 0);
                                                                        performattack(player, openborconstant("ANI_FOLLOW52"), 1);
                                                                   } else if ( getentityvar(opp, "type") == "climbing_area" ) {
                                                                       if ( getentityvar(opp, "type") == "climbing_area" ) {
                                                                            changeentityproperty(player, "position", x,zo+1,a+(speed/2));
                                                                            performattack(player, openborconstant("ANI_FOLLOW57"), 1);
                                                                       }
                                                                   }
                                                            } // fine if da base
                                                            else if ( (a <= ao+height && a+(player_height) >= ao)
                                                                   && (anim_id == openborconstant("ANI_JUMP") || anim_id == openborconstant("ANI_WALKOFF")) ) { // Significa: in volo
                                                               /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                    damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                    damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                               }*/

                                                               if ( is_flying(player) ) reset_fly(player);
                                                               reset_lost_frames();

                                                               changeentityproperty(player, "velocity", 0,0, 0.0001);
                                                               if ( is_a_platform_below(opp) == 1 ) { // Siamo su una piattaforma?
                                                                    setentityvar("setlayer", getentityproperty(player, "setlayer"));
                                                                    if ( CHANGE_LAYER_FLAG ) changeentityproperty(player, "setlayer", getentityproperty(opp, "setlayer")+T_LAYER);
                                                               }
                                                               changeentityproperty(player, "base", base+(speed/2) );
                                                               changeentityproperty(player, "tosstime", get_next_time(openborvariant("game_speed")*1));
                                                               if ( getentityvar(opp, "type") == "ladder_v" ) {
                                                                    changeentityproperty(player, "position", xo+(width/2),zo+1,a+(speed/2));
                                                                    performattack(player, openborconstant("ANI_FOLLOW50"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_h" ) {
                                                                    changeentityproperty(player, "position", NULL(),zo+1,ao-(height/2+player_height));
                                                                    performattack(player, openborconstant("ANI_FOLLOW120"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_l" ) {
                                                                    float new_a = a+(speed/2);
                                                                    float shiftx = width - ((new_a-bo)*width)/height; // calcoliamo quando deve stare di lato sulla scala se si aggrappa in volo
                                                                    changeentityproperty(player, "position", xo-shiftx+width-1,zo+1,new_a);
                                                                    if ( !getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 1);
                                                                    performattack(player, openborconstant("ANI_FOLLOW51"), 1);
                                                               } else if ( getentityvar(opp, "type") == "ladder_r" ) {
                                                                    float new_a = a+(speed/2);
                                                                    float shiftx = width - ((new_a-bo)*width)/height; // calcoliamo quando deve stare di lato sulla scala se si aggrappa in volo
                                                                    changeentityproperty(player, "position", xo+shiftx,zo+1,new_a);
                                                                    if ( getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 0);
                                                                    performattack(player, openborconstant("ANI_FOLLOW52"), 1);
                                                               }
                                                            } // fine if in volo



                                                        } // fine if coords
                                                } // fine if playkeys + animations

                                                // dalla cima
                                                if ( is_on_base(player) && !is_flying(player)
                                                    && anim_id != openborconstant("ANI_FOLLOW50") && anim_id != openborconstant("ANI_FOLLOW51") && anim_id != openborconstant("ANI_FOLLOW52")
                                                    && anim_id != openborconstant("ANI_FOLLOW55") && anim_id != openborconstant("ANI_FOLLOW56") && anim_id != openborconstant("ANI_FOLLOW120")
                                                    && anim_id != openborconstant("ANI_FOLLOW121") ) {

                                                            if ( x >= xo-T_ACTION_RADIUS && x <= xo+width+T_ACTION_RADIUS && z >= zo-T_ACTION_RADIUS-10 && z <= zo+T_ACTION_RADIUS+10 ) {
                                                                if ( base >= bo+height-T_ACTION_RADIUS && a <= bo+height+T_ACTION_RADIUS ) { // dalla cima
                                                                       reset_lost_frames();

                                                                       /*if ( getentityproperty(grabbed_opp,"exists") || getentityproperty(grabbing,"exists") ) {
                                                                            damageentity(player,player,0,0,openborconstant("ATK_NORMAL6"));
                                                                            damageentity(grabbed_opp,grabbed_opp,0,0,openborconstant("ATK_NORMAL6"));
                                                                       }*/

                                                                        if ( getentityvar(opp, "type") == "ladder_v" ) {
                                                                            changeentityproperty(player, "velocity", 0,0,0);
                                                                            changeentityproperty(player, "position", xo+width/2,NULL(),NULL());
                                                                            performattack(player, openborconstant("ANI_FOLLOW55"), 1);
                                                                        } else if ( getentityvar(opp, "type") == "ladder_h" ) {
                                                                            changeentityproperty(player, "velocity", 0,0,0);
                                                                            //changeentityproperty(player, "position", NULL(),NULL(),ao-(height/2+player_height));
                                                                            performattack(player, openborconstant("ANI_FOLLOW121"), 1);
                                                                        } else if ( getentityvar(opp, "type") == "ladder_l" ) {
                                                                            changeentityproperty(player, "velocity", 0,0,0);
                                                                            if ( !getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 1);
                                                                            performattack(player, openborconstant("ANI_FOLLOW56"), 1);
                                                                        } else if ( getentityvar(opp, "type") == "ladder_r" ) {
                                                                            changeentityproperty(player, "velocity", 0,0,0);
                                                                            if ( getentityproperty(player, "direction") ) changeentityproperty(player, "direction", 0);
                                                                            performattack(player, openborconstant("ANI_FOLLOW56"), 1);
                                                                        } else if ( getentityvar(opp, "type") == "climbing_area" ) {
                                                                            changeentityproperty(player, "velocity", 0,0,0);
                                                                            performattack(player, openborconstant("ANI_FOLLOW59"), 1);
                                                                        }
                                                                } // fine if dalla cima
                                                            } // fine if coords
                                                } // fine if playkeys + animations

                                        //changeentityproperty(player, "hitbyid", 0); // Resettiamo la pausa attacco
                                } // fine if pains

                        } // fine if wich ladder
                    } // fine if exists
        } // fine if grabbing
    //} // fine if glv("climbing_a_ladder")
}

int goto_ladder_no_ai_control(void self, int blockade_nums) {
    void ladder = glv("near_a_ladder");

    AI_ladder_reset_cond(self);
    if ( ladder != NULL() && gep(ladder,"exists") && gev(self,"near_a_ladder") == NULL()
        && (is_walking(self) || is_running(self) || is_in_idle(self)) && !is_attacking(self) && !is_in_pain(self) )  {
        int minz = ov("player_min_z");
        int maxz = ov("player_max_z");
        void platform = NULL(), platform_on = get_platform_on(self);
        float wall;
        int hole;
        int base = gep(self,"base");
        int a = gep(self,"y");
        int x = gep(self,"x");
        int z = gep(self,"z");
        int ladder_base = gep(ladder,"base");
        int ladder_a = gep(ladder,"y");
        int ladder_x = gep(ladder,"x");
        int ladder_z = gep(ladder,"z");
        char ladder_type = getentityvar(ladder, "type");
        float speed = gep(self,"speed"), xspeed = NULL(), zspeed = NULL();
        float x_distance = get_distance(x,ladder_x), z_distance = get_distance(z,ladder_z);
        int i = 0, i1 = 0, i2 = 0, c1 = 0, c2 = 0, g1 = 0, g2 = 0;
        float t_obstacle = 5, t_space = 0.8;
        float ladder_width = getentityvar(ladder,"width");
        void target = findtarget(self);

        if ( gep(target,"exists") ) {
            float target_base = gep(target,"base");
            if ( base == target_base ) AI_ladder_reset(self);
        }

        if ( ladder_width == NULL() ) ladder_width = 0;
        if ( minz != maxz && (ladder_type == "ladder_v" || ladder_type == "ladder_h" || ladder_type == "climbing_area") ) ladder_z -= T_LADDER_RANGE;

        //cep(self,"position",ladder_x,ladder_z,NULL()); // ---> FOR DEBUGGING

        // AGGIUNGERE SE coords sono in ladder allora performattack + AI_ladder_reset(self)
        if ( x <= ladder_x+T_LADDER_RANGE+ladder_width && x >= ladder_x-T_LADDER_RANGE && z <= ladder_z+T_LADDER_RANGE && z >= ladder_z-T_LADDER_RANGE ) {
            if ( !is_attacking(self) && !is_in_pain(self) && is_on_base(self) )  {
                sev(self,"ladder",glv("near_a_ladder"));

                //AI_ladder_reset_cond(self);
                    if ( ladder_type == "ladder_h" ) {
                        if ( !is_flying(self) ) {
                            if ( ladder_a <= a ) {
                                grab_ladder(self);
                            } else {
                                if ( !is_jumping(self) ) {
                                    if ( x <= ladder_x-T_LADDER_RANGE+ladder_width && x >= ladder_x+T_LADDER_RANGE && z <= ladder_z+T_LADDER_RANGE*2 && z >= ladder_z-T_LADDER_RANGE ) {
                                        jump(self);
                                        slv("ladder_h_ent",glv("near_a_ladder"));

                                        return;
                                    }
                                } else {
                                    grab_ladder(self);
                                }
                                //playsample(loadsample("data/sounds/beep2.wav"));
                            }
                        }
                    } else {
                        if ( !is_flying(self) ) grab_ladder(self);
                        AI_ladder_reset(self);
                    }

                return;
            }
        }

        if ( !gep(self,"noaicontrol") ) cep(self,"noaicontrol",1);

        if ( x_distance > THRESHOLD_LADDER_X || z_distance > THRESHOLD_LADDER_Z ) {
            AI_ladder_reset(self);
            return;
        }

        // set velocity
        if ( ladder_x+ladder_width < x-t_space ) g1 = -1;
        else if ( ladder_x > x+t_space ) g1 = 1;
        else g1 = 0;

        if ( ladder_z < z-t_space ) g2 = -1;
        else if ( ladder_z > z+t_space ) g2 = 1;
        else g2 = 0;

        if ( glv("flagged_xdir") == NULL() ) {
            i1 = g1;
        } else i1 = glv("flagged_xdir");

        if ( glv("flagged_zdir") == NULL() ) {
            i2 = g2;
        } else i2 = glv("flagged_zdir");

        // search for a good path
        // start indexes to right directions (9 dirs compreso stare fermi == 0)

        // PRIORITY DIRECTIONS!!
        platform = find_platform_in_a(x+g1*t_obstacle,z+g2*t_obstacle,a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
        wall = checkwall(x+g1*t_obstacle,z+g2*t_obstacle);
        hole = checkhole(x+g1*t_obstacle,z+g2*t_obstacle);
        if ( (platform == NULL() || platform == platform_on) && wall <= base && hole == 0 ) { // good path!!
            i1 = g1, i2 = g2;

            // set new velocity
            xspeed = i1*speed;
            zspeed = i2*speed/2;

            cep(self,"velocity",xspeed,zspeed,NULL());
            change_walking_anim_noaicontrol(self);
            slv("flagged_xdir",i1); // dirs flaggate: se abbiamo trovato una strada libera allora flagghiamo e proseguiamo sempre per quella strada!
            slv("flagged_zdir",i2);

            //drawstring(10,180,0,"ladder: "+glv("near_a_ladder"));
            //drawstring(10,190,0,"i1: "+i1+",  i2: "+i2);
            return;
        }

        c1 = 0;
        while ( c1 < 3 ) {
            c2 = 0;
            while ( c2 < 3 ) {
                if ( (i1 == 0 && i2 == 0) || ( i1 == -1*glv("flagged_xdir") && i2 == -1*glv("flagged_zdir") ) ) { // escludiamo lo stare fermi e il ritorno nella direzione opposta!!
                    ++c2; ++i2;
                    if ( i2 > 1 ) i2 = -1;
                    continue;
                }
                platform = find_platform_in_a(x+i1*t_obstacle,z+i2*t_obstacle,a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                wall = checkwall(x+i1*t_obstacle,z+i2*t_obstacle);
                hole = checkhole(x+g1*t_obstacle,z+g2*t_obstacle);

                if ( (platform == NULL() || platform == platform_on) && wall <= base && hole == 0 ) { // good path!!
                    // set new velocity
                    xspeed = i1*speed;
                    zspeed = i2*speed/2;

                    cep(self,"velocity",xspeed,zspeed,NULL());
                    change_walking_anim_noaicontrol(self);
                    slv("flagged_xdir",i1);
                    slv("flagged_zdir",i2);

                    //drawstring(10,180,0,"ladder: "+glv("near_a_ladder"));
                    //drawstring(10,190,0,"i1: "+i1+",  i2: "+i2);
                    return;
                } else {
                    if ( glv("blockade_num") == NULL() ) slv("blockade_num",0);
                    else slv("blockade_num",glv("blockade_num")+1);

                    if ( glv("blockade_num") >= blockade_nums && blockade_nums >= 0 ) {
                        AI_ladder_reset_cond(self);
                        return;
                    }
                }
                ++c2; ++i2;
                if ( i2 > 1 ) i2 = -1;
            } // fine while2
            ++c1; ++i1;
            if ( i1 > 1 ) i1 = -1;
        } // fine while1

        AI_ladder_reset_cond(self);

    } else if ( glv("ladder_h_ent") != NULL() ) {
        char ladder_type = getentityvar(glv("ladder_h_ent"), "type");

        if ( ladder_type == "ladder_h" && !is_animation(self,"ANI_FOLLOW120") && !is_animation(self,"ANI_FOLLOW121") ) {
            if ( !is_flying(self) && is_jumping(self) ) {
                //playsample(loadsample("data/sounds/beep2.wav"));
                sev(self,"ladder",glv("ladder_h_ent"));
                grab_ladder(self);
                //AI_ladder_reset(self,0,1);
            }
        }
    } // fine if ladder exists

}

int goto_ladder_waypoints(void self) {
    void ladder = glv("near_a_ladder");

    if ( ladder != NULL() && gep(ladder,"exists") && gev(self,"near_a_ladder") == NULL() )  {
        int minz = ov("player_min_z");
        int maxz = ov("player_max_z");
        int base = gep(self,"base");
        int a = gep(self,"y");
        int x = gep(self,"x");
        int z = gep(self,"z");
        int ladder_base = gep(ladder,"base");
        int ladder_a = gep(ladder,"y");
        int ladder_x = gep(ladder,"x");
        int ladder_z = gep(ladder,"z");
        float speed = gep(self,"speed");
        //void platform_around = NULL(), platform = NULL(), platform_on = NULL();
        //float wall;
        //float t_platform_near = T_ACTION_RADIUS;
        //int dir_timeout = ov("game_speed")*2;
        //int time = ov("elapsed_time");
        float ladder_width = getentityvar(ladder,"width");
        char ladder_type = getentityvar(ladder, "type");
        float t_dist = 5;
        void target = findtarget(self);

        if ( gep(target,"exists") ) {
            float target_base = gep(target,"base");
            if ( base == target_base ) AI_ladder_reset(self);
        }

        if ( ladder_width == NULL() ) ladder_width = 0;
        if ( minz != maxz && (ladder_type == "ladder_v" || ladder_type == "ladder_h" || ladder_type == "climbing_area") ) ladder_z -= T_LADDER_RANGE;
        cep(self,"opponent",ladder);

        if ( ( !is_stopped(self) ) && (!is_in_pain(self) && is_on_base(self)) ) { // !is_attacking(self) && is_walking(self) || is_running(self)) &&
            // AGGIUNGERE SE coords sono in ladder allora performattack + AI_ladder_reset(self)
            if ( x <= ladder_x+T_LADDER_RANGE+ladder_width && x >= ladder_x-T_LADDER_RANGE && z <= ladder_z+T_LADDER_RANGE && z >= ladder_z-T_LADDER_RANGE ) {
                //drawstring(10,180,0,"TEST");
                //if ( !is_attacking(self) && !is_in_pain(self) && is_on_base(self) )  {
                    sev(self,"ladder",glv("near_a_ladder"));

                    //AI_ladder_reset_cond(self);
                    if ( ladder_type == "ladder_h" ) {
                        if ( !is_flying(self) ) {
                            if ( ladder_a <= a ) {
                                grab_ladder(self);
                            } else {
                                if ( !is_jumping(self) ) {
                                    if ( x <= ladder_x-T_LADDER_RANGE+ladder_width && x >= ladder_x+T_LADDER_RANGE && z <= ladder_z+T_LADDER_RANGE*2 && z >= ladder_z-T_LADDER_RANGE ) {
                                        jump(self);
                                        slv("ladder_h_ent",glv("near_a_ladder"));

                                        return;
                                    }
                                } else {
                                    grab_ladder(self);
                                }
                                //playsample(loadsample("data/sounds/beep2.wav"));
                            }
                        }
                    } else {
                        if ( !is_flying(self) ) grab_ladder(self);
                        AI_ladder_reset(self);
                    }

                    return;
                //}
            }

            AI_ladder_reset_cond(self,1);
        } else {
            // ADD WAYPOINTS (only if base entity == base ladder else reset)
            if ( base == ladder_base && !is_in_pain(self) && is_on_base(self) ) {
                if ( ladder_type == "ladder_v" ) {
                    ladder_x += ladder_width/2;
                } else if ( ladder_type == "ladder_l" ) {
                    ladder_x += t_dist;
                } else if ( ladder_type == "ladder_r" ) {
                    ladder_x += t_dist*2;
                } else if ( ladder_type == "climbing_area" || ladder_type == "ladder_h" ) {
                    if ( x > ladder_x && x < ladder_x ) ladder_x = x;
                    else if ( x >= ladder_x ) ladder_x -= t_dist;
                    else if ( x <= ladder_x ) ladder_x += t_dist;
                }

                //if( glv("test")==NULL() ){waypoints(self,ladder_x,ladder_z);slv("test",1);}
                waypoints(self,ladder_x,ladder_z);
                //waypoints(self,20,20);
                //drawstring(10,180,0,"TEST");
                //AI_ladder_reset(self);
            } else {
                AI_ladder_reset(self);
            }
        }
    } else if ( glv("ladder_h_ent") != NULL() ) {
        char ladder_type = getentityvar(glv("ladder_h_ent"), "type");

        if ( ladder_type == "ladder_h" && !is_animation(self,"ANI_FOLLOW120") && !is_animation(self,"ANI_FOLLOW121") ) {
            if ( !is_flying(self) && is_jumping(self) ) {
                //playsample(loadsample("data/sounds/beep2.wav"));
                sev(self,"ladder",glv("ladder_h_ent"));
                grab_ladder(self);
                //AI_ladder_reset(self,0,1);
            }
        }
    }
}

int goto_ladder(void self) {
    void ladder = glv("near_a_ladder");

    if ( ladder != NULL() && gep(ladder,"exists") && gev(self,"near_a_ladder") == NULL() )  {
        int minz = ov("player_min_z");
        int maxz = ov("player_max_z");
        int base = gep(self,"base");
        int a = gep(self,"y");
        int x = gep(self,"x");
        int z = gep(self,"z");
        int ladder_base = gep(ladder,"base");
        int ladder_a = gep(ladder,"y");
        int ladder_x = gep(ladder,"x");
        int ladder_z = gep(ladder,"z");
        float speed = gep(self,"speed")+0.5;
        void platform = NULL(), platform_on = NULL();
        //void platform_around = NULL();
        float wall;
        int hole;
        float t_obstacle = T_ACTION_RADIUS;
        int dir_timeout = ov("game_speed")*2;
        int time = ov("elapsed_time");
        float ladder_width = getentityvar(ladder,"width");
        char ladder_type = getentityvar(ladder, "type");
        void target = findtarget(self);

        if ( glv("ladder_h_ent") != NULL() && ladder_type != "ladder_h" ) slv("ladder_h_ent",NULL());

        if ( gep(target,"exists") ) {
            float target_base = gep(target,"base");
            if ( base == target_base && ladder_type != "ladder_h" ) AI_ladder_reset(self);
        }

        //drawstring(10,100,0,gev(self,"near_a_ladder"));
        if ( ladder_width == NULL() ) ladder_width = 0;
        if ( minz != maxz && (ladder_type == "ladder_v" || ladder_type == "ladder_h" || ladder_type == "climbing_area") ) ladder_z -= T_LADDER_RANGE;
        cep(self,"opponent",ladder);
        //cep(self,"think","common_think",0);


        //cep(self,"noaicontrol",1);
        //cep(self,"velocity",0.0001,NULL(),NULL());
        // niente noaicontrol per cambio animazioni e strada da percorrere. lasciamo fare alla IA originale dell'engine

        // se si è a fianco ad una piattaforma, non abilitare l'IA
        /*platform_around = find_platform_around_in_a(self,t_obstacle,t_obstacle,a+1,MAX_ALTITUDE);
        if ( platform_around != NULL() && platform_around != get_platform_on(self) ) {
            slv("near_a_ladder",NULL());
            return;
        }*/
        //drawstring(10,180,0,"TEST");
        //|| gep(self,"aiflag","idling")
        //drawstring(10,180,0,"TEST LADDER OK");

        if ( !is_stopped(self) && !is_in_pain(self) && is_on_base(self) && !is_jumping(self) ) { // !is_attacking(self) && is_walking(self) || is_running(self)) &&
            platform_on = get_platform_on(self);
            //drawstring(10,190,0,"TEST GOTO OK");

            if ( ladder_x+ladder_width < x ) {
                platform = find_platform_in_a(x-t_obstacle,z,a+1,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                wall = checkwall(x-t_obstacle,z);
                hole = checkhole(x-t_obstacle,z);
                if ( (platform == NULL() || platform == platform_on) && wall <= base && hole == 0 ) {
                    cep(self,"velocity",-1*speed,NULL(),NULL());
                    if ( glv("dir_timeout") == NULL() || time >= glv("dir_timeout") ) {
                        if ( gep(self,"direction") ) cep(self,"direction",0);
                        slv("dir_timeout",time+dir_timeout);
                    }
                } else {
                    AI_ladder_reset_cond(self);
                    slv("near_a_ladder",NULL());
                    return;
                }
            } else if ( ladder_x >= x ) {
                platform = find_platform_in_a(x+t_obstacle,z,a+1,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                wall = checkwall(x+t_obstacle,z);
                hole = checkhole(x+t_obstacle,z);
                if ( (platform == NULL() || platform == platform_on) && wall <= base && hole == 0 ) {
                    cep(self,"velocity",speed,NULL(),NULL());
                    if ( glv("dir_timeout") == NULL() || time >= glv("dir_timeout") ) {
                        if ( !gep(self,"direction") ) cep(self,"direction",1);
                        slv("dir_timeout",time+dir_timeout);
                    }
                } else {
                    AI_ladder_reset_cond(self);
                    slv("near_a_ladder",NULL());
                    return;
                }
            }

            if ( ladder_z >= z ) {
                platform = find_platform_in_a(x,z+t_obstacle,a+1,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                wall = checkwall(x,z+t_obstacle);
                hole = checkhole(x,z+t_obstacle);
                if ( (platform == NULL() || platform == platform_on) && wall <= base && hole == 0 ) {
                    cep(self,"velocity",NULL(),speed/2,NULL());
                } else {
                    AI_ladder_reset_cond(self);
                    slv("near_a_ladder",NULL());
                    return;
                }
            } else if ( ladder_z < z ) {
                platform = find_platform_in_a(x,z-t_obstacle,a+1,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY);
                wall = checkwall(x,z-t_obstacle);
                hole = checkhole(x,z-t_obstacle);
                if ( (platform == NULL() || platform == platform_on) && wall <= base && hole == 0 ) {
                    cep(self,"velocity",NULL(),-1*(speed/2),NULL());
                    /*if ( glv("dir_timeout") == NULL() || time >= glv("dir_timeout") ) {
                        //if ( gep(self,"animationid") != oc("ANI_UP") ) cep(self,"animation",oc("ANI_UP"),1); // 1 flag == reset??
                        slv("dir_timeout",time+dir_timeout);
                    }*/
                } else {
                    AI_ladder_reset_cond(self);
                    slv("near_a_ladder",NULL());
                    return;
                }
            }

            //cep(self,"position",ladder_x,ladder_z,NULL()); // ---> FOR DEBUGGING

            // AGGIUNGERE SE coords sono in ladder allora performattack + AI_ladder_reset(self)
            if ( x <= ladder_x+T_LADDER_RANGE+ladder_width && x >= ladder_x-T_LADDER_RANGE && z <= ladder_z+T_LADDER_RANGE && z >= ladder_z-T_LADDER_RANGE ) {
                //sev(self,"ladder",glv("near_a_ladder"));grab_ladder(self);return;
                //drawstring(10,180,0,"TEST");
                if ( !is_in_pain(self) )  { //  && is_on_base(self) && !gep(self,"aiflag","attacking")
                    sev(self,"ladder",glv("near_a_ladder"));
                    //AI_ladder_reset_cond(self);

                    if ( ladder_type == "ladder_h" ) {
                        if ( !is_flying(self) ) {
                            if ( ladder_a <= a ) {
                                grab_ladder(self);
                            } else {
                                if ( !is_jumping(self) ) {
                                    if ( x <= ladder_x-T_LADDER_RANGE+ladder_width && x >= ladder_x+T_LADDER_RANGE && z <= ladder_z+T_LADDER_RANGE*2 && z >= ladder_z-T_LADDER_RANGE ) {
                                        jump(self);
                                        slv("ladder_h_ent",glv("near_a_ladder"));

                                        return;
                                    }
                                } else {
                                    grab_ladder(self);
                                }
                                //playsample(loadsample("data/sounds/beep2.wav"));
                            }
                        }
                    } else {
                        if ( !is_flying(self) ) grab_ladder(self);
                        AI_ladder_reset(self);
                    }

                    return;
                }
            }

            AI_ladder_reset_cond(self,1);
        }

        //change_walking_anim_noaicontrol(self);

        //drawstring(10,180,0,"ladder: "+glv("near_a_ladder"));
    } else if ( glv("ladder_h_ent") != NULL() ) {
        char ladder_type = getentityvar(glv("ladder_h_ent"), "type");

        if ( ladder_type == "ladder_h" && !is_animation(self,"ANI_FOLLOW120") && !is_animation(self,"ANI_FOLLOW121") ) {
            if ( !is_flying(self) && is_jumping(self) ) {
                //playsample(loadsample("data/sounds/beep2.wav"));
                sev(self,"ladder",glv("ladder_h_ent"));
                grab_ladder(self);
                //AI_ladder_reset(self,0,1);
            }
        }
    }
}

void search_path_ladder_for_target(void self, void target) {
    int i = 0, cl = 0;
    void ladder_stack = array(0);
    void ladder_stack_direct_link = array(0);
    int ls_i = 0, lasl_i = 0;
    int check_num = 0;
    void choosen_ladder = NULL();
    float self_x = gep(self,"x");
    float self_z = gep(self,"z");
    float self_base = gep(self,"base");
    float target_base = gep(target,"base");
    int path_flag = 0; // 1 = UP, 0 = DOWN

    if ( target_base > self_base ) path_flag = 1;
    do {

        check_interact_entities(self,0,1,1,1);
        for (cl = 0; cl < get_interact_ent_size(); ++cl) { // openborvariant("count_entities") --- openborconstant("MAX_ENTS")
            void ent = get_interact_ent(cl);

            if ( gep(ent, "exists") ) { // USE THE ENTITYVAR NOT DEFAULT MODEL
                void ent_type = getentityvar(ent, "type");
                char substr1, substr2;
                void ladder = NULL();

                if ( typeof(ent_type) == oc("VT_STR") ) {
                    char cText = ent_type;
                    char cFind1 = "ladder";
                    char cFind2 = "climbing_area";
                    substr1 = strinfirst(cText, cFind1);
                    substr2 = strinfirst(cText, cFind2);
                } else continue;

                ladder = ent;
                if ( substr1 != -1 || substr2 != -1 ) {
                    // target base is > or < than sel base?
                    /*
                     * - al primo ciclo vedi se ci sono solo link diretto al target (o al self+target) e riempi stack_new
                     * - dal 2nd ciclo: prendi la scala nuova. controlla link tra vecchia scala e nuova
                     * - se link è ok vedi se è candidata e se c'è un link diretto dal target al self
                     * - salva tutto su stack_new e stack_direct.
                     * - dopo il for fai un check: se nn ci sono stack_direct svuota vecchio ladder_stack e riempilo con stack_new
                     * - se ci sono stack_direct svuota tutti gli stack. scegli la scala più vicina al self (range_flag)
                     */
                    if ( check_num == 0 ) {
                        int is_candidate_flag = is_candidate_ladder(self,target,ladder);

                        if ( is_candidate_flag == 2 ) set(ladder_stack,ls_i++,ladder);
                        else if ( is_candidate_flag == 4 ) set(ladder_stack_direct_link,lasl_i++,ladder);

                        //log( check_num+") "+gep(ladder,"defaultname")+" ("+ent_type+"): "+is_candidate_flag+"\n" );
                    } else {
                        int c2 = 0;
                        void tmp_ladder_stack = array(0);
                        int tmp_ladder_i = 0;

                        for ( c2 = 0; c2 < size(ladder_stack); ++c2 ) {
                            int is_linked_flag, is_candidate_flag;
                            void old_ladder = get(ladder_stack,c2);

                            if ( old_ladder == NULL() ) continue;

                            /* ladder is in ladder1 param because the path is reversed in relation to the basic logic.
                               The path from self to target is UP, but from target to self the path is DOWN
                             */
                            is_linked_flag = is_linked_ladder(ladder,old_ladder,path_flag);
                            if ( is_linked_flag == 1 ) {
                                set(tmp_ladder_stack,tmp_ladder_i++,ladder);
                                is_candidate_flag = is_candidate_ladder(self,target,ladder);
                                if ( is_candidate_flag == 3 ) set(ladder_stack_direct_link,lasl_i++,ladder);
                            }
                        }

                        if ( ladder_stack != NULL() ) free(ladder_stack);
                        ladder_stack = tmp_ladder_stack; // <--- copy array address
                        //if ( tmp_ladder_stack != NULL() ) free(tmp_ladder_stack);
                    }
                } // is_a ladder
            } // fine if ent exists
        } // fine for
        ++check_num; // loop count
    } while ( size(ladder_stack) > 1 && size(ladder_stack_direct_link) <= 0 ); // finisci se non trovi niente o ne trovi 1

    // CHOOSE NEAREST LADDER TO SELF
    for ( i = 0; i < size(ladder_stack_direct_link); ++i ) {
        void candidate_ladder = get(ladder_stack_direct_link,i);
        float ladder_x = gep(candidate_ladder,"x");
        float ladder_z = gep(candidate_ladder,"z");
        float ladder_width = getentityvar(candidate_ladder,"width");

        if ( ladder_width == NULL() ) ladder_width = 0;
        if ( self_x <= ladder_x+T_LADDER_PROXIMITY+ladder_width && self_x >= ladder_x-T_LADDER_PROXIMITY
            && self_z >= ladder_z-T_LADDER_PROXIMITY && self_z <= ladder_z+T_LADDER_PROXIMITY ) {
            choosen_ladder = candidate_ladder;
            break;
        }
    }

    if ( ladder_stack != NULL() ) free(ladder_stack);
    if ( ladder_stack_direct_link != NULL() ) free(ladder_stack_direct_link);

    return choosen_ladder;
}

// cerca una scala sulla base di self. se ent è una scala fai un check su ent. se non trova nulla restituisci NULL(). se ent è in input controlla solo quell'entità
void search_ladder_for_target(void self, void target, int range_flag, void ent) {
    void ladder = NULL();
    int cl = 0;
    int count_entities;
    int ent_flag = 0;

    check_interact_entities(self,0,1,1,1);
    count_entities = get_interact_ent_size(); // openborvariant("count_entities") --- openborconstant("MAX_ENTS")

    if ( ent != NULL() ) {
        count_entities = 1;
        ent_flag = 1;
    }

    // target base is > or < than sel base?
    for (cl = 0; cl < count_entities; ++cl) {
        if ( !ent_flag ) ent = get_interact_ent(cl);

        if ( gep(ent, "exists") ) { // USE THE ENTITYVAR NOT DEFAULT MODEL
            void ent_type = getentityvar(ent, "type");
            char substr1, substr2;

            if ( typeof(ent_type) == oc("VT_STR") ) {
                char cText = ent_type;
                char cFind1 = "ladder";
                char cFind2 = "climbing_area";
                substr1 = strinfirst(cText, cFind1);
                substr2 = strinfirst(cText, cFind2);
            } else continue;

            ladder = ent;
            if ( substr1 != -1 || substr2 != -1 ) {
                if ( is_incam(ladder,T_INCAM) ) {
                    int minz = ov("player_min_z");
                    int maxz = ov("player_max_z");
                    float target_base = gep(target,"base");
                    float self_base = gep(self,"base");
                    float self_x = gep(self,"x");
                    float self_z = gep(self,"z");
                    float ladder_base = gep(ladder,"base");
                    float ladder_a = gep(ladder,"y");
                    float ladder_x = gep(ladder,"x");
                    float ladder_z = gep(ladder,"z");
                    float ladder_height = getentityvar(ladder,"height");
                    //void target_platform = get_platform_on(target);
                    void platform_end, platform_begin;
                    float wall_height;
                    float ladder_width = getentityvar(ladder,"width"), climb_area_width = 0;
                    void platform_on = get_platform_on(self);
                    float wall_on = checkwall(self_x,self_z);
                    float side_ladder_shift = 0, front_ladder_shift = 0;
                    int i = 0, t_inc = 1;

                    if ( ladder_width == NULL() ) ladder_width = 0;

                    if ( ent_type == "ladder_v" || ent_type == "ladder_h" || ent_type == "climbing_area" ) front_ladder_shift = -10;
                    else if ( ent_type == "ladder_l" ) side_ladder_shift = 30;
                    else if ( ent_type == "ladder_r" ) side_ladder_shift = -10;

                    if ( ent_type == "climbing_area" ) climb_area_width = ladder_width;
                    else climb_area_width = 0;

                    // ladder_h priority
                    if ( ent_type == "ladder_h" ) {
                        //drawstring(10,50,0,"ladder: "+ent_type);
                        if ( (range_flag) && self_x <= ladder_x+T_LADDER_PROXIMITY+ladder_width && self_x >= ladder_x-T_LADDER_PROXIMITY
                            && self_z >= ladder_z-T_LADDER_PROXIMITY && self_z <= ladder_z+T_LADDER_PROXIMITY ) {
                            //playsample(loadsample("data/sounds/beep.wav"));
                            return ladder;
                        } else continue;
                    }

                    if ( target_base > self_base ) {
                        for ( i = ladder_x; i <= ladder_x+climb_area_width; i += t_inc ) { // to check climbable area too
                            platform_end = find_platform_in_a(i+side_ladder_shift,ladder_z+front_ladder_shift,ladder_a+ladder_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma alla fine della scala
                            platform_begin = find_platform_in_a(i+side_ladder_shift,ladder_z+front_ladder_shift,ladder_a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma sulla base della scala
                            wall_height = checkwall(i+side_ladder_shift,ladder_z+front_ladder_shift);

                            if ( (platform_begin != platform_on || wall_on != wall_height) && self_base > 0 ) continue; // self è più in alto della base e non ci sono piattaforme/muri per questa scala? salta!
                            //if ( ladder_a > target_base ) continue;

                            if ( gep(platform_end, "exists") ) {
                                float platform_a =  gep(platform_end,"y");
                                float platform_height =  gep(platform_end,"height");
                                float platform_walkable_base = platform_a+platform_height;

                                if ( platform_walkable_base > self_base && platform_walkable_base <= target_base ) {
                                    if ( (range_flag) && self_x <= ladder_x+T_LADDER_PROXIMITY+ladder_width && self_x >= ladder_x-T_LADDER_PROXIMITY
                                        && self_z >= ladder_z-T_LADDER_PROXIMITY && self_z <= ladder_z+T_LADDER_PROXIMITY ) {
                                        return ladder;
                                    } else continue;
                                } else continue;
                            }

                            if ( wall_height > self_base && wall_height <= target_base ) { // funziona anche per la base 0
                                if ( (range_flag) && self_x <= ladder_x+T_LADDER_PROXIMITY+ladder_width && self_x >= ladder_x-T_LADDER_PROXIMITY
                                    && self_z >= ladder_z-T_LADDER_PROXIMITY && self_z <= ladder_z+T_LADDER_PROXIMITY ) {
                                    return ladder;
                                } else continue;
                            } else continue;
                        } // fine for
                    } else if ( target_base < self_base ) {
                        for ( i = ladder_x; i <= ladder_x+climb_area_width; i += t_inc ) { // to check climbable area too
                            platform_end = find_platform_in_a(i+side_ladder_shift,ladder_z+front_ladder_shift,ladder_a+ladder_height,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma alla fine della scala
                            platform_begin = find_platform_in_a(i+side_ladder_shift,ladder_z+front_ladder_shift,ladder_a,MAX_ALTITUDE,T_LADDER_ALT_VISIBILITY); // cerco una piattaforma sulla base della scala
                            wall_height = checkwall(i+side_ladder_shift,ladder_z+front_ladder_shift);

                            //drawstring(10,180,0,"TEST");
                            //drawstring(10,200,0,"TEST LADDER CERCANDO");
                            if ( (platform_end != platform_on || wall_on != wall_height) && self_base > 0 ) continue; // self è più in alto della base e non ci sono piattaforme/muri per questa scala? salta!
                            //drawstring(10,210,0,"TEST LADDER TROVATA");
                            if ( ladder_a <= 0 && target_base <= 0 ) {
                                if ( (range_flag) && self_x <= ladder_x+T_LADDER_PROXIMITY+ladder_width && self_x >= ladder_x-T_LADDER_PROXIMITY
                                    && self_z >= ladder_z-T_LADDER_PROXIMITY && self_z <= ladder_z+T_LADDER_PROXIMITY ) {
                                    return ladder;
                                } else continue;
                            }

                            if ( gep(platform_begin, "exists") ) {
                                float platform_a =  gep(platform_begin,"y");
                                float platform_height =  gep(platform_begin,"height");
                                float platform_walkable_base = platform_a+platform_height;

                                if ( platform_walkable_base < self_base && platform_walkable_base >= target_base ) {
                                    if ( (range_flag) && self_x <= ladder_x+T_LADDER_PROXIMITY+ladder_width && self_x >= ladder_x-T_LADDER_PROXIMITY
                                        && self_z >= ladder_z-T_LADDER_PROXIMITY && self_z <= ladder_z+T_LADDER_PROXIMITY ) {
                                        return ladder;
                                    } else continue;
                                } else continue;
                            }

                            if ( wall_height < self_base && wall_height >= target_base ) {
                                if ( (range_flag) && self_x <= ladder_x+T_LADDER_PROXIMITY+ladder_width && self_x >= ladder_x-T_LADDER_PROXIMITY
                                    && self_z >= ladder_z-T_LADDER_PROXIMITY && self_z <= ladder_z+T_LADDER_PROXIMITY ) {
                                    return ladder;
                                } else continue;
                            } else continue;
                        } // fine for
                    } else { // target_base == self_base
                        return NULL();
                    } // fine if target base comparison
                } else continue; // fine if is_incam
            } else continue; // fine if is_a_ladder
        } else continue; // fine if ent exists
    } // fine for

    return NULL();
}
