
#import "data/scripts/lib.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/player_didhit.c"
//#import "data/scripts/player_backpain.c"

void main() {
  void self = getlocalvar("self");
  //int plane = getlocalvar("plane");
  //float height = getlocalvar("height"); Height of blocking wall.
  //int index = getlocalvar("index");
  /*onblockascript
        Each update while entity is prevented from moving upward through platform (i.e. "hitting head").
        self: Caller.
        obstacle: Entity blocking caller.
   */

    //drawstring( 20,180,0,"WALL!!" );return;

    check_wallsplat(self, 0);
    check_hold_wall(self);
}

int getoppx(void player) {
    return getentityvar(player, 6);
}

int check_hold_wall_anim(void player) {
    int anim_id = getentityproperty(player,"animationid");

    if ( anim_id == openborconstant("ANI_FOLLOW45") && getentityproperty(player, "animpos") <= 0 ) {
        int p = getentityproperty(player, "playerindex");
        int game_speed = openborvariant("game_speed");
        float xdir = 1;

        // In questo modo il toss durerà tanto quanto dura il frame 0 di FOLLOW45
        changeentityproperty(player, "tosstime", getentityproperty(player,"tosstime")+game_speed/10);
        changeentityproperty(player, "aiflag", "jumping", 0);
        changeentityproperty(player, "aiflag", "running", 0);
        changeentityproperty(player, "velocity", 0,0,0);
        if ( getentityproperty(player, "direction") == 1 ) xdir *= -1;

        if ( playerkeys(p,0,"jump") ) {
            //changeentityproperty(player,"tosstime",0);
            xdir *= 2;
            tossentity(player, 3, xdir, 0);
            xdir /= 1.5;

            changeentityproperty(player, "aiflag", "running", 1);
            setentityvar(player,5,xdir);
            //changeentityproperty(player, "aiflag", "jumping", 1);
            //performattack(player, openborconstant("ANI_JUMP"), 1);
        } else setentityvar(player,5,xdir);
    }
}

int set_hold_wall_anim(void player) {
    int anim_id = getentityproperty(player,"animationid");
    int p = getentityproperty(player, "playerindex");
    int game_speed = openborvariant("game_speed");
    float xdir = 1;

    changeentityproperty(player, "tosstime", getentityproperty(player,"tosstime")+game_speed/10);
    changeentityproperty(player, "aiflag", "jumping", 0);
    changeentityproperty(player, "aiflag", "running", 0);
    changeentityproperty(player, "velocity", 0,0,0);
    if ( getentityproperty(player, "direction") == 1 ) xdir *= -1;

    if ( playerkeys(p,0,"jump") ) {
        xdir *= 2;
        tossentity(player, 3, xdir, 0);
        xdir /= 1.5;

        changeentityproperty(player, "aiflag", "running", 1);
        setentityvar(player,5,xdir);
    } else setentityvar(player,5,xdir);
}

int check_hold_wall(void player) {
    int anim_id = getentityproperty(player,"animationid");
    int p = getentityproperty(player, "playerindex");
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float t_dist = 3;

    if ( anim_id == openborconstant("ANI_JUMP") ) { // && anim_id != openborconstant("ANI_FOLLOW45") && getentityproperty(player, "aiflag", "jumping")
        if ( checksplattablewall(player,t_dist,t_dist,1) ) {
            if ( checkwall(x+t_dist,z+0) >= a && playerkeys(p,0,"moveright") == 8 && playerkeys(p,0,"moveleft") != 4 ) {
                changeentityproperty(player, "aiflag", "jumping", 0); // se è a 1 possiamo muoverci
                changeentityproperty(player, "aiflag", "running", 0);
                performattack(player, openborconstant("ANI_FOLLOW45"), 1);
                //set_hold_wall_anim(player);
            } else if ( checkwall(x-t_dist,z+0) >= a && playerkeys(p,0,"moveleft") == 4 && playerkeys(p,0,"moveright") != 8 ) {
                changeentityproperty(player, "aiflag", "jumping", 0);
                changeentityproperty(player, "aiflag", "running", 0);
                performattack(player, openborconstant("ANI_FOLLOW45"), 1);
                //set_hold_wall_anim(player);
            }
        }
    } // fine se sta saltando

    // La funzione di perform holdjump viene fatta attraverso check_hold_platform()
}

int check_wallsplat(void player, float a_threshold) {
    void opp = getentityproperty(player, "opponent");
    int anim_id = getentityproperty(player,"animationid");
    float x = getentityproperty(player, "x");
    float z = getentityproperty(player, "z");
    float a = getentityproperty(player, "y");
    float base = getentityproperty(player, "base");
    float t_diff = 0, t_dist = 1;
    int dir = getentityproperty(player, "direction");
    int prev_dir = getentityvar(player,0);
    float wall_height, wall_r, wall_l;

    if ( !is_in_pain(player) ) return 0;

    wall_height = getwallaround_in_a(player,t_dist,0,1);

    if ( wall_height <= a ) return 0;

    wall_r = checkwall(x+t_dist,z);
    wall_l = checkwall(x-t_dist,z);
    if ( wall_r > a && prev_dir != 1 ) return 0;
    if ( wall_l > a && prev_dir != 0 ) return 0;

    if ( a > base ) t_diff = a-base;

    /*if ( anim_id != 2333 && getentityproperty(player, "invincible") == 0  && a >= base && getentityproperty(player, "aiflag", "running") == 1
                    && anim_id != openborconstant("ANI_FOLLOW30") && anim_id != openborconstant("ANI_FOLLOW34") && anim_id != openborconstant("ANI_FOLLOW31")
                    && anim_id != openborconstant("ANI_FOLLOW35") && anim_id != openborconstant("ANI_FOLLOW32") && anim_id != openborconstant("ANI_RISE10")
                    && anim_id != openborconstant("ANI_DIE10") && checksplattablewall(player,1,1,1001) == 1 ) {*/

    // getentityproperty(self, "invincible") == 0 && getentityproperty(self,"animationid") == FALL (IN ARIA) e anche "y" >= threshold  ||||||||||  getentityproperty(self, "running", "speed")
    // a_threshold: if (a > base ) a_threshold = a-base; --> if ( t_diff >= a_threshold )
    // CAMBIARE LE PRIME 2!!
    if ( getentityproperty(player, "invincible") == 0  && a >= base && t_diff >= a_threshold
                    && anim_id != openborconstant("ANI_FOLLOW30") && anim_id != openborconstant("ANI_FOLLOW34") && anim_id != openborconstant("ANI_FOLLOW31")
                    && anim_id != openborconstant("ANI_FOLLOW35") && anim_id != openborconstant("ANI_FOLLOW32") && anim_id != openborconstant("ANI_RISE10")
                    && anim_id != openborconstant("ANI_DIE10") && anim_id != openborconstant("ANI_SHOCK") && checksplattablewall(player,t_dist,t_dist) == 1 &&
                    ( anim_id == openborconstant("ANI_FALL") || anim_id == openborconstant("ANI_FALL2") || anim_id == openborconstant("ANI_FALL3")
                      || anim_id == openborconstant("ANI_FALL50") || anim_id == openborconstant("ANI_FALL51") || anim_id == openborconstant("ANI_FALL52") || anim_id == openborconstant("ANI_FALL71")) ) {

        // IMPORTANTISSIMO: noaicontrol + cambio d'animazione funziona solo se lo script viene ripetuto a ciclo ad ogni frame!!
        changeentityproperty(player, "noaicontrol", 1); // lo facciamo perchè con qualche mossa speciale altrimenti può evitare di cadere
        changeentityproperty(player, "antigravity", 1);
        changeentityproperty(player, "position", x, z, a);
        //changeentityproperty(self, "base", a);

        bindentity(player,NULL());
        if (!opp || !getentityproperty(opp,"exists")) {
                damageentity(player,player,30,0,openborconstant("ATK_NORMAL10")); // <--------------
        } else {
                damageentity(player,opp,30,0,openborconstant("ATK_NORMAL10")); // <--------------
        }
        check_versus_mark(opp,player);

        // SUPER IMPORTANTISSIMO: Se non imposti la velocity a 0 su tutti i parametri, il player continuerà a camminare anche dopo noaicontrol impostato a vero!!
        changeentityproperty(player, "velocity", 0, 0, 0);

		if (opp && getentityproperty(opp,"exists")) {
			if ( getentityproperty(player, "x") < getoppx(player) ) setentityvar(player,0,0);
            else setentityvar(player,0,1);
		}

        if ( getentityvar(player,0) != 2 && getentityvar(player,0) != NULL() ) {
            if ( getentityvar(player,0) == dir ) {
                changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW30"));
            } else {
                changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW34"));
            }
        } else { // use prev dir
            if ( getentityvar(player, 4) == dir ) {
                changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW30"));
            } else {
                changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW34"));
            }
        }
    }

    return 1;
}
