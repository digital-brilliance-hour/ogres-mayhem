
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/lib_platform.c"

#define T_OB_RANGE_Y 10
#define T_TOSSV_FROM_BASE 0.1
#define T_ALT_FROM_BASE 2.0

// BUGS:

int check_flying(void self) {
    int anim_id = gep(self,"animationid");
    int type = gep(self,"type");
    char model = gep(self,"model");

    // reset on climbing: if getglobalvar("p"+p+"ladder") != NULL() reset fly
    if ( type == oc("TYPE_PLAYER") ) {
        int p = gep(self,"playerindex");

        if ( getglobalvar("p"+p+"ladder") != NULL() ) { // se stai su scala annulla volo
            if ( is_flying(self) ) {
                reset_fly(self);
                //set_flying(self,NULL());
            }

            return 0;
        }
    }

    if ( !is_in_pain(self) &&
        (   (model == "efly_baxter" && anim_id != oc("ANI_FREESPECIAL10")) || (model == "player_fly_baxter" && anim_id != oc("ANI_FREESPECIAL10")) || (model == "efly_baxter_handshot" && anim_id != oc("ANI_FREESPECIAL10"))
         || (model == "ebaxter" && anim_id == oc("ANI_FREESPECIAL10")) || (model == "BAXTER" && anim_id == oc("ANI_FREESPECIAL10")) || (model == "ebaxter_handshot" && anim_id == oc("ANI_FREESPECIAL10"))   ) ) {
        check_flying_movement(self,80,8);
        if ( !is_flying(self) ) set_flying(self,1);

        return 1;
    } else {
        if ( is_flying(self) ) {
            reset_fly(self);
            //set_flying(self,NULL());
        }
    }

    return 0;
}

int check_flying_movement(void self, float ashift, float bound_shift) { // 80, 8
    int lost_frames = get_lost_frames();
    float x = gep(self, "x");
    float z = gep(self, "z");
    float a = gep(self, "y");
    float base = gep(self, "base");
    float height = gep(self, "height");
    float max_vel = 0.6, vel_factor = 0.01, vel = glv("vel"), dec_shift, orig_ashift = ashift;
    float mid_bound, down_bound, up_bound;
    float threshold = 0.01;
    int game_speed = ov("game_speed");
    int time = ov("elapsed_time");

    if ( get_flying_base(self) != NULL() ) base = get_flying_base(self);

    if ( gep(self,"no_adjust_base") ) cep(self, "no_adjust_base", 0); // ----- ANTI NO_ADJUST_BASE BUG -----
    if ( gep(self,"subject_to_hole") ) { slv("temp_subject_to_hole",gep(self,"subject_to_hole")); cep(self, "subject_to_hole", 0); }

    if ( glv("fly_ashift") != NULL() ) ashift = glv("fly_ashift");
    if ( glv("fly_bound_shift") != NULL() ) bound_shift = glv("fly_bound_shift");

    mid_bound = base+ashift;
    down_bound = mid_bound-bound_shift;
    up_bound = mid_bound+bound_shift;
    if ( height == NULL() ) height = T_ENT_HEIGHT;

    //cep(self,"subject_to_gravity",0);
    //cep(self,"no_adjust_base",1);
    //cep(self,"subject_to_platform",0);

    if ( lost_frames > 0 ) vel_factor *= lost_frames; // aggiustiamo eventuali rallentamenti del dispositivo

    // imprimiamo velocità iniziale
    if ( vel == NULL() ) vel = max_vel; // andrà su

    //drawstring(10,150,0,"a: "+a);
    if ( a >= up_bound ) {
        // VEDI SE CI SONO OSTACOLI NEL CAMMINO
        // v(t) = v0 +a*(s/v0) -> 0 = v0 + a*s/v0 -> v0 = d*s/v0 -> v0^2 = d*s -> s = (v0^2)/d
        dec_shift = exp(max_vel,2)/vel_factor;
        //drawstring(10,150,0,"dec_shift: "+dec_shift);

        // C'è un ostacolo nella parte più alta dove volerai (base+orig_ashift)? se no allora resetta tutto e torna a volare senza limiti
        // cerca tra a+height e +o- orig_ashift+dec_shift+T_OB_RANGE_Y
        if ( !obstacle_above_by_coords(x,z,a,base,height,orig_ashift+dec_shift+T_OB_RANGE_Y) ) {
            slv("fly_ashift",NULL());
            slv("fly_bound_shift",NULL());
        }

        // Se c'è un ostacolo inverti il movimento
        if ( obstacle_above(self,T_OB_RANGE_Y) ) {
            a = up_bound-threshold;
            vel = -1*vel_factor;
            if ( vel < -1*max_vel ) vel = -1*max_vel;

            if ( glv("fly_ashift") != NULL() ) slv("fly_ashift",glv("fly_ashift")/2);
            else slv("fly_ashift",ashift/2);
            if ( glv("fly_bound_shift") != NULL() ) slv("fly_bound_shift",glv("fly_bound_shift")/2);
            else slv("fly_bound_shift",bound_shift/2);

            //drawstring(10,110,0,"AAAA");
        } else {
            vel -= vel_factor;
            if ( vel < -1*max_vel ) vel = -1*max_vel;
            //drawstring(10,110,0,"BBBB");
        }
    } else if ( a <= down_bound ) {
        // Se c'è un ostacolo inverti il movimento
        if ( obstacle_below(self) ) {
            a += T_ALT_FROM_BASE;
            tossentity(self,T_TOSSV_FROM_BASE,0,0); // to avoid stay on platform "ground-0" bug
            //a = down_bound+threshold;
            vel = vel_factor;
            if ( vel > max_vel ) vel = max_vel;
            //drawstring(10,110,0,"CCCC");
        } else {
            vel += vel_factor;
            if ( vel > max_vel ) vel = max_vel;
            //drawstring(10,110,0,"DDDD");
        }
    } else if ( vel <= 0 ) { // raggiungiamo le velocità max
        // Se c'è un ostacolo inverti il movimento
        if ( obstacle_below(self) ) {
            a += T_ALT_FROM_BASE;
            tossentity(self,T_TOSSV_FROM_BASE,0,0); // to avoid stay on platform "ground-0" bug
            vel = vel_factor;
            //drawstring(10,110,0,"EEEE");
        } else {
            vel -= vel_factor;
            //drawstring(10,110,0,"FFFF");
        }
        if ( vel < -1*max_vel ) vel = -1*max_vel;
    } else if ( vel >= 0 ) {
        // VEDI SE CI SONO OSTACOLI NEL CAMMINO
        // v(t) = v0 +a*(s/v0) -> 0 = v0 + a*s/v0 -> v0 = d*s/v0 -> v0^2 = d*s -> s = (v0^2)/d
        dec_shift = exp(max_vel,2)/vel_factor;
        //drawstring(10,150,0,"dec_shift: "+dec_shift);

        // C'è un ostacolo nella parte più alta dove volerai (base+orig_ashift)? se no allora resetta tutto e torna a volare senza limiti
        // cerca tra a+height e +o- orig_ashift+dec_shift+T_OB_RANGE_Y
        if ( !obstacle_above_by_coords(x,z,a,base,height,orig_ashift+dec_shift+T_OB_RANGE_Y) ) {
            slv("fly_ashift",NULL());
            slv("fly_bound_shift",NULL());
        }

        // Se c'è un ostacolo inverti il movimento
        if ( obstacle_above(self,T_OB_RANGE_Y) ) {
            void platform = find_platform_in_a(x,z,a+height+threshold,MAX_ALTITUDE);

            if ( platform != NULL() && getentityproperty(platform,"exists") ) {
                a = getentityproperty(platform,"y");
            }

            vel = -1*vel_factor;

            if ( glv("fly_ashift") != NULL() ) slv("fly_ashift",glv("fly_ashift")/2);
            else slv("fly_ashift",ashift/2);
            if ( glv("fly_bound_shift") != NULL() ) slv("fly_bound_shift",glv("fly_bound_shift")/2);
            else slv("fly_bound_shift",bound_shift/2);

            //drawstring(10,110,0,"GGGG");
        } else {
            vel += vel_factor;
            //drawstring(10,110,0,"HHHH");
        }
        if ( vel > max_vel ) vel = max_vel;
    }

    /*drawstring(10,160,0,"base: "+base);
    drawstring(10,120,0,"vel: "+vel);
    drawstring(10,130,0,"new a: "+a);
    drawstring(10,140,0,"down_bound: "+down_bound);*/

    //if ( lost_frames > 0 ) vel *= lost_frames;
    slv("vel",vel);

    //cep(self,"no_adjust_base",1);
    cep(self,"tosstime",get_next_time(game_speed/20)); // 10 teniamo in volo l'entità
    cep(self,"position",NULL(),NULL(),a+vel);
    cep(self,"base",gep(self,"y")); // perchè altrimenti non puoi attaccare
}

int check_flying_movement_vert(void self, float ashift, float bound_shift, float max_vel, float vel_factor) { // 80, 8
    int lost_frames = get_lost_frames();
    float x = gep(self, "x");
    float z = gep(self, "z");
    float a = gep(self, "y");
    float base = gep(self, "base");
    float vel = glv("vel");
    float mid_bound, down_bound, up_bound;
    float threshold = 0.01;
    int game_speed = ov("game_speed");
    int time = ov("elapsed_time");

    if ( max_vel == NULL() ) max_vel = 0.6;
    if ( vel_factor == NULL() ) vel_factor = 0.01,

    if ( get_flying_base(self) != NULL() ) base = get_flying_base(self);

    if ( gep(self,"no_adjust_base") ) cep(self, "no_adjust_base", 0); // ----- ANTI NO_ADJUST_BASE BUG -----
    if ( gep(self,"subject_to_hole") ) { slv("temp_subject_to_hole",gep(self,"subject_to_hole")); cep(self, "subject_to_hole", 0); }

    mid_bound = base+ashift;
    down_bound = mid_bound-bound_shift;
    up_bound = mid_bound+bound_shift;

    if ( lost_frames > 0 ) vel_factor *= lost_frames; // aggiustiamo eventuali rallentamenti del dispositivo

    // imprimiamo velocità iniziale
    if ( vel == NULL() ) vel = max_vel; // andrà su

    //drawstring(10,150,0,"a: "+a);
    if ( a >= up_bound ) {
        vel -= vel_factor;
        if ( vel < -1*max_vel ) vel = -1*max_vel;
    } else if ( a <= down_bound ) {
        vel += vel_factor;
        if ( vel > max_vel ) vel = max_vel;
    } else if ( vel <= 0 ) { // raggiungiamo le velocità max
        vel -= vel_factor;
        if ( vel < -1*max_vel ) vel = -1*max_vel;
    } else if ( vel >= 0 ) {
        vel += vel_factor;
        if ( vel > max_vel ) vel = max_vel;
    }

    //if ( lost_frames > 0 ) vel *= lost_frames;
    slv("vel",vel);

    //cep(self,"no_adjust_base",1);
    cep(self,"tosstime",get_next_time(game_speed/20)); // 10 teniamo in volo l'entità
    cep(self,"position",NULL(),NULL(),a+vel);
    cep(self,"base",gep(self,"y")); // perchè altrimenti non puoi attaccare
}

int check_flying_movement_horiz(void self, float ashift, float bound_shift, float max_vel, float vel_factor) { // 80, 8
    int lost_frames = get_lost_frames();
    float x = gep(self, "x");
    float z = gep(self, "z");
    float a = gep(self, "y");
    float base = gep(self, "base");
    float vel = glv("vel");
    float mid_bound, down_bound, up_bound;
    float threshold = 0.01;
    int game_speed = ov("game_speed");
    int time = ov("elapsed_time");

    if ( max_vel == NULL() ) max_vel = 0.6;
    if ( vel_factor == NULL() ) vel_factor = 0.01,

    if ( getlocalvar("original_pos") == NULL() ) setlocalvar("original_pos",x);
    mid_bound = getlocalvar("original_pos")+ashift;
    down_bound = mid_bound-bound_shift;
    up_bound = mid_bound+bound_shift;

    if ( lost_frames > 0 ) vel_factor *= lost_frames; // aggiustiamo eventuali rallentamenti del dispositivo

    // imprimiamo velocità iniziale
    if ( vel == NULL() ) vel = max_vel; // andrà su

    //drawstring(10,150,0,"a: "+a);
    if ( x >= up_bound ) {
        vel -= vel_factor;
        if ( vel < -1*max_vel ) vel = -1*max_vel;
    } else if ( x <= down_bound ) {
        vel += vel_factor;
        if ( vel > max_vel ) vel = max_vel;
    } else if ( vel <= 0 ) { // raggiungiamo le velocità max
        vel -= vel_factor;
        if ( vel < -1*max_vel ) vel = -1*max_vel;
    } else if ( vel >= 0 ) {
        vel += vel_factor;
        if ( vel > max_vel ) vel = max_vel;
    }

    //if ( lost_frames > 0 ) vel *= lost_frames;
    slv("vel",vel);

    //cep(self,"no_adjust_base",1);
    cep(self,"position",x+vel,NULL(),NULL());
}

// da chiamare su inpain o falling o cambio arma
int reset_fly(void self) {
    slv("vel",NULL());
    slv("orig_base",NULL());
    slv("fly_ashift",NULL());
    slv("fly_bound_shift",NULL());
    cep(self,"tosstime",0);
    set_flying_base(self,NULL());
    if ( glv("temp_subject_to_hole") != NULL() ) { cep(self, "subject_to_hole", glv("temp_subject_to_hole")); slv("temp_subject_to_hole",NULL()); }
    reset_lost_frames(); // reset lost frames!!
    //cep(self,"tosstime",0); // <------ OPTIONAL
    //cep(self,"subject_to_gravity",1);

    set_flying(self,NULL());
}

int obstacle_below(void self) {
    if ( is_a_wall_below(self) || is_a_platform_below(self) ) return 1;
    else return 0;
}

int obstacle_above(void self, float range_y) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float height = getentityproperty(self, "height");
    float threshold = 1;

    if ( height == NULL() ) height = 0;

    // is_a_wall_below(self) ||
    if ( find_platform_in_a(x,z,a+height+threshold,MAX_ALTITUDE,range_y) ) return 1;
    else return 0;
}

int obstacle_below_by_coords(float x, float z, float a, float base) {
    if ( is_a_wall_below_by_coords(x,z,a,base) || is_a_platform_below_by_coords(x,z,a,base) ) return 1;
    else return 0;
}

int obstacle_above_by_coords(float x, float z, float a, float base, float height, float range_y) {
    float threshold = 1;

    if ( height == NULL() ) height = 0;

    // is_a_wall_below_by_coords(x,z,a,base) ||
    if ( find_platform_in_a(x,z,a+height+threshold,MAX_ALTITUDE,range_y) ) return 1;
    else return 0;
}

int is_flying(void self) {
    if ( getentityvar(self,"flying") > 0 ) return 1;
    else return 0;
}

int set_flying(void self, int value) {
    setentityvar(self,"flying",value);
}

int set_flying_base(void self, int value) {
    setentityvar(self,"flying_base",value);
}

int get_flying_base(void self) {
    return getentityvar(self,"flying_base");
}

