
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/lib_platform.c"

int check_bouncing(void self, float threshold, float bounce_factor) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float tossv = get_tossv(self);
    float tossv_abs = abs(tossv);
    float true_tossv = getentityproperty(self, "tossv");
    int lost_frame = get_lost_frames()+1;
    float t_toss = 0.1;

    if ( !getentityproperty(self,"subject_to_gravity") ) return 0;

    if ( tossv == 0 ) {
        setentityvar(self,"altitude",a); // new altitude
    }

    /*if ( checkwall(x,z) > a ) {
        changeentityproperty(self,"position",NULL(),NULL(),base);
        return 1;
    }*/

    if ( a <= base && tossv != NULL() && tossv_abs > threshold ) {
        float altitude = getentityvar(self,"altitude");
        float new_toss = tossv_abs/bounce_factor; // *lost_frame

        //new_toss = 5;
        if ( new_toss > t_toss ) {
            setentityvar(self,"altitude",altitude/bounce_factor);
            tossentity(self, new_toss, NULL(), NULL());
            set_tossv_value(self,new_toss);
        } else {
            setentityvar(self,"altitude",0);
            set_tossv_value(self,0);
        }

        if ( true_tossv == 0 && new_toss <= t_toss ) return 1; // change to tossv = 0??
        else return 2; // bouncing incomplete
    } else if ( a <= base ) return 1; // change to tossv = 0??

    return 0;
}

int check_vbouncing(void self, float t_dist) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    int dir = getentityproperty(self,"direction");
    float xdir = getentityproperty(self, "xdir");
    float zdir = getentityproperty(self, "zdir");

    if ( a >= base ) {
        float wall_h = checkwall(x,z);
        float wall_h_sx = checkwall(x-t_dist,z);
        float wall_h_dx = checkwall(x+t_dist,z);
        float wall_h_up = checkwall(x,z-t_dist);
        float wall_h_down = checkwall(x,z+t_dist);

        //if ( !getentityproperty(self,"subject_to_gravity") ) return 0;

        if ( wall_h_sx > a || wall_h_dx > a || wall_h > a ) {
            set_velx(self,-1*xdir);
            changeentityproperty(self, "velocity", get_velx(self), NULL(), NULL());
            if ( dir ) changeentityproperty(self, "direction", 0);
            else changeentityproperty(self, "direction", 1);
        } else {
            void platform = find_platform_in_a(x,z,a,MAX_ALTITUDE);
            void platform_sx = find_platform_in_a(x-t_dist,z,a,MAX_ALTITUDE);
            void platform_dx = find_platform_in_a(x+t_dist,z,a,MAX_ALTITUDE);
            float platform_height = 0;

                 if ( platform    != NULL() && platform    != self ) { platform_height = getentityproperty(platform,    "height"); }
            else if ( platform_sx != NULL() && platform_sx != self ) { platform_height = getentityproperty(platform_sx, "height"); platform = platform_sx; }
            else if ( platform_dx != NULL() && platform_dx != self ) { platform_height = getentityproperty(platform_dx, "height"); platform = platform_dx; }
            if ( platform_height == NULL() ) platform_height = 0;

            if ( platform != NULL() && platform != self ) {
                float platform_a = getentityproperty(platform, "y");

                set_velx(self,-1*xdir);
                changeentityproperty(self, "velocity", get_velx(self), NULL(), NULL());
                if ( dir ) changeentityproperty(self, "direction", 0);
                else changeentityproperty(self, "direction", 1);
            }
        }

        if ( wall_h_up > a || wall_h_down > a || wall_h > a ) {
            set_velz(self,-1*zdir);
            changeentityproperty(self, "velocity", NULL(), get_velz(self), NULL());
        } else {
            void platform = find_platform_in_a(x,z,a,MAX_ALTITUDE);
            void platform_sx = find_platform_in_a(x,z-t_dist,a,MAX_ALTITUDE);
            void platform_dx = find_platform_in_a(x,z+t_dist,a,MAX_ALTITUDE);
            float platform_height = 0;

                 if ( platform    != NULL() && platform    != self ) { platform_height = getentityproperty(platform,    "height"); }
            else if ( platform_sx != NULL() && platform_sx != self ) { platform_height = getentityproperty(platform_sx, "height"); platform = platform_sx; }
            else if ( platform_dx != NULL() && platform_dx != self ) { platform_height = getentityproperty(platform_dx, "height"); platform = platform_dx; }
            if ( platform_height == NULL() ) platform_height = 0;

            if ( platform != NULL() && platform != self ) {
                float platform_a = getentityproperty(platform, "y");

                set_velz(self,-1*zdir);
                changeentityproperty(self, "velocity", NULL(), get_velz(self), NULL());
            }
        }
    }
}

int check_friction(void self, float threshold, float friction_factor, int toss_flag) {
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float velx = get_velx(self);
    float velz = get_velz(self);
    //float friction_factor = 1.035; //1.025
    int lost_frame = get_lost_frames();

    if ( lost_frame <= 0 ) lost_frame = 1;

    if ( velx != NULL() && abs(velx) > threshold ) {
        if ( a <= base && getlocalvar("begin_frictionx") == NULL() ) {
            setlocalvar("begin_frictionx",1);
        }
        if ( getlocalvar("begin_frictionx") == 1 ) {
            changeentityproperty(self, "velocity", velx/(friction_factor*lost_frame), NULL(), NULL());
            set_velx(self,velx/(friction_factor*lost_frame));
        }
    } else if ( abs(velx) <= threshold ) {
        changeentityproperty(self, "velocity", 0, NULL(), NULL());
        set_velx(self,0);
    }
    if ( getentityproperty(self,"xdir") && (get_velx(self) == NULL() || get_velx(self) == 0) ) changeentityproperty(self, "velocity", 0, NULL(), NULL());

    if ( velz != NULL() && abs(velz) > threshold ) {
        if ( a <= base && getlocalvar("begin_frictionz") == NULL() ) {
            setlocalvar("begin_frictionz",1);
        }
        if ( getlocalvar("begin_frictionz") == 1 ) {
            changeentityproperty(self, "velocity", NULL(), velz/(friction_factor*lost_frame), NULL());
            set_velz(self,velz/(friction_factor*lost_frame));
        }
    } else if ( abs(velz) <= threshold ) {
        changeentityproperty(self, "velocity", NULL(), 0, NULL());
        set_velz(self,0);
    }
    if ( getentityproperty(self,"zdir") && (get_velz(self) == NULL() || get_velz(self) == 0) ) changeentityproperty(self, "velocity", NULL(), 0, NULL());

    if ( get_tossv(self) == 0 && (toss_flag == NULL() || toss_flag <= 0) ) {
        set_velx(self,0);
        set_velz(self,0);
    }
    reset_lost_frames();

    return 0;
}

int self_rotate(void self, float speed, int dir) {
	void sprite = getentityproperty(self, "sprite");

	if ( sprite != NULL() ) {
		int lost_frames = get_lost_frames();
		int width = getgfxproperty(sprite, "srcwidth");
		int height = getgfxproperty(sprite, "srcheight");
		int time = openborvariant("elapsed_time");
		float roto_speed = (time/speed);

		//if ( lost_frames > 0 ) speed *= lost_frames;
		if ( !dir ) speed *= -1;

		roto_speed = roto_speed%360 + roto_speed-(roto_speed%(roto_speed+1));
		//drawstring(10,100,0,"roto_speed: "+roto_speed);
		if ( roto_speed >= 360 ) roto_speed = 0; // not %360 that trunc the num!
		else if ( roto_speed < 0 ) roto_speed = 359;

		changedrawmethod(self, "reset", 1);
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "centerx", width/2);
		changedrawmethod(self, "centery", height/2);
		changedrawmethod(self, "rotate", roto_speed); // per disegnare una barra e ruotare anche gli oggetti

        	//changedrawmethod(self, "enabled", 0);
        	//changedrawmethod(NULL(), "enabled", 1);
       		//changedrawmethod(NULL(), "reset", 1);
	}
}

float get_velx(void self) {
    return getentityvar(self,"xdir");
}

void set_velx(void self, float value) {
    setentityvar(self,"xdir",value);
}

float get_velz(void self) {
    return getentityvar(self,"zdir");
}

void set_velz(void self, float value) {
    setentityvar(self,"zdir",value);
}

float get_tossv(void self) {
    return getentityvar(self,"tossv");
}

void set_tossv(void self) {
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float tossv = getentityproperty(self, "tossv");

    if ( a > base ) setentityvar(self,"tossv",tossv);
}

void set_tossv_value(void self, float value) {
    setentityvar(self,"tossv",value);
}

