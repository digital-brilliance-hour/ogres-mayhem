
//#import "data/scripts/enemy_script.c"
#import "data/scripts/lib.c"
#import "data/scripts/lib_wall.c"
#import "data/scripts/player_didhit.c"

void main() {
  void self = getlocalvar("self");
  //void plane = getlocalvar("plane");
  //void height = getlocalvar("height"); Height of blocking wall.
  /*onblockascript
        Each update while entity is prevented from moving upward through platform (i.e. "hitting head").
        self: Caller.
        obstacle: Entity blocking caller.
   */

    check_wallsplat(self, 0);

    // drawstring( 20,180,0,"MURO!!" );



}

int get_e_oppx(void player) {
    return getentityvar(player, 6);
}

int check_wallsplat(void player, float a_threshold) {
    int anim_id = getentityproperty(player,"animationid");
    void opp = getentityproperty(player,"opponent");
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

    /*if ( anim_id != 2333 && getentityproperty(player, "invincible") == 0  && getentityproperty(player, "y") >= 0 && getentityproperty(player, "aiflag", "running") == 1
                    && anim_id != openborconstant("ANI_FOLLOW30") && anim_id != openborconstant("ANI_FOLLOW34") && anim_id != openborconstant("ANI_FOLLOW31")
                    && anim_id != openborconstant("ANI_FOLLOW35") && anim_id != openborconstant("ANI_FOLLOW32") && anim_id != openborconstant("ANI_RISE9")
                    && anim_id != openborconstant("ANI_DIE9") && checksplattablewall(player,1,1,1001) == 1 ) {*/


    // getentityproperty(self, "invincible") == 0 && getentityproperty(self,"animationid") == FALL (IN ARIA) e anche "y" >= threshold  ||||||||||  getentityproperty(self, "running", "speed")
    // a_threshold: if (a > base ) a_threshold = a-base; --> if ( t_diff >= a_threshold )
    // CAMBIARE LE PRIME 2!!
    if ( getentityproperty(player, "invincible") == 0  && a >= base && t_diff >= a_threshold
                    && anim_id != openborconstant("ANI_FOLLOW30") && anim_id != openborconstant("ANI_FOLLOW34") && anim_id != openborconstant("ANI_FOLLOW31")
                    && anim_id != openborconstant("ANI_FOLLOW35") && anim_id != openborconstant("ANI_FOLLOW32") && anim_id != openborconstant("ANI_RISE10")
                    && anim_id != openborconstant("ANI_DIE10") && anim_id != openborconstant("ANI_SHOCK") && checksplattablewall(player,t_dist,t_dist) == 1 &&
                    ( anim_id == openborconstant("ANI_FALL") || anim_id == openborconstant("ANI_FALL2") || anim_id == openborconstant("ANI_PAIN20") || anim_id == openborconstant("ANI_FALL11")
                      || anim_id == openborconstant("ANI_FALL12") || anim_id == openborconstant("ANI_FALL14") || anim_id == openborconstant("ANI_FALL70")
                      || anim_id == openborconstant("ANI_FALL33") || anim_id == openborconstant("ANI_FALL34") || anim_id == openborconstant("ANI_FALL35")
                      || (anim_id == openborconstant("ANI_PAIN36") && getentityproperty(player, "animpos") >= 21 ) ) ) {

        // IMPORTANTISSIMO: noaicontrol + cambio d'animazione funziona solo se lo script viene ripetuto a ciclo ad ogni frame!!
        changeentityproperty(player, "noaicontrol", 1); // lo facciamo perchè con qualche mossa speciale altrimenti può evitare di cadere
        changeentityproperty(player, "antigravity", 1);
        changeentityproperty(player, "position", x, z, a);
        //changeentityproperty(self, "base", a);

        bindentity(player,NULL());
        if (!opp || !getentityproperty(opp,"exists")) {
            if ( prev_dir == dir ) damageentity(player,player,30,0,openborconstant("ATK_NORMAL10"));
            else damageentity(player,player,30,0,openborconstant("ATK_NORMAL50"));
        } else {
            if ( prev_dir == dir ) damageentity(player,opp,30,0,openborconstant("ATK_NORMAL10"));
            else damageentity(player,opp,30,0,openborconstant("ATK_NORMAL50"));
        }
        check_versus_mark(opp,player);

        // SUPER IMPORTANTISSIMO: Se non imposti la velocity a 0 su tutti i parametri, il player continuerà a camminare anche dopo noaicontrol impostato a vero!!
        changeentityproperty(player, "velocity", 0, 0, 0);

        if (opp && getentityproperty(opp,"exists")) {
            if ( getentityproperty(player, "x") < get_e_oppx(player) ) setentityvar(player,0,0);
            else setentityvar(player,0,1);
        }

        if ( anim_id == openborconstant("ANI_FALL34") || anim_id == openborconstant("ANI_FALL33") || anim_id == openborconstant("ANI_FALL11")
            || anim_id == openborconstant("ANI_FALL14") || anim_id == openborconstant("ANI_PAIN20") || anim_id == openborconstant("ANI_PAIN36") ) { // BACKPAINS
                if ( getentityproperty(player, "direction") == 0 ) {
                    changeentityproperty(player, "direction", 1);
                    setentityvar(player,0,1);
                } else {
                    changeentityproperty(player, "direction", 0);
                    setentityvar(player,0,0);
                }
                dir = getentityproperty(player, "direction");
        }

        if ( getentityvar(player,0) == dir ) {
            if ( !dir ) {
                    changeentityproperty(player, "direction", 1);
                    setentityvar(player,0,1);
            } else {
                    changeentityproperty(player, "direction", 0);
                    setentityvar(player,0,0);
            }
            changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW34"));
        } else {
            changeentityproperty(player, "animation", openborconstant("ANI_FOLLOW30"));
        }

    }

    return 1;
}

