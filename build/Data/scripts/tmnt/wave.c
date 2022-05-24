
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"
#import "data/scripts/lib_platform.c"
#import "data/scripts/ladder_commons.c"
#import "data/scripts/flying_script.c"

#define T_WALKOFF 4.0 // 2 -> 4
#define BOOST_FACTOR 0.47
#define CUSHONING_FACTOR 0.025 // 0.02 -> 0.065 -> 0.015
#define CUSH_LIMIT_FACTOR 1.2 // 1 -> 2 -> 1.3
//#define T_TOSSTIME get_next_time(10)

// DA FARE:

void main() {
    void self = getlocalvar("self");

    //drawstring( 10,170,0,"W!");

    //check_wave_ents(self);

}

int is_on_wave() {
    if ( glv("is_on_wave_c") || glv("is_on_wave_r") || glv("is_on_wave_l") ) return 1;
    else return 0;
}

int check_wave_ents(void self) {
    //void stair = getentityproperty(self, "opponent");
    int i, found_flag = 0, is_on = 0, result = 0;
    //reset_opp_by_range(self,stair,openborconstant("ANI_IDLE"));

    if ( getglobalvar("in_menu") != 1 && getlevelproperty("type") != 2 ) { // Waiting, Select
        for (i = 0; i < openborvariant("count_entities"); ++i) { // openborconstant("MAX_ENTS")
            void ent = getentity(i);

            if ( getentityproperty(ent, "exists") ) {
                if ( getentityvar(ent, "type") == "wave_r" || getentityvar(ent, "type") == "wave_l" || getentityvar(ent, "type") == "wave_c" ) {
                    found_flag = 1;
                    is_on = check_wave(self, ent);
                    if (is_on == 1) {
                        result = 1;
                        break;
                    }
                } // fine if wave
            } // fine if exists
        } // fine for all_ents

        if ( !found_flag || is_on <= 0 ) {
            reset_wave(self);
        }
    }

    return result;
}

int reset_cushoning() {
    setlocalvar("cush_tossv", NULL());
    setlocalvar("cush_ashift", NULL());
    setlocalvar("cush_new_base", NULL());
}

int reset_wave(void self) {
    if ( getlocalvar("is_on_wave_r") == 1 || getlocalvar("is_on_wave_l") == 1 || getlocalvar("is_on_wave_c") == 1 ) {
        if ( getlocalvar("stored_nab") != NULL() ) changeentityproperty(self, "no_adjust_base", getlocalvar("stored_nab"));
        else changeentityproperty(self, "no_adjust_base", 0);
        reset_lost_frames();
        reset_cushoning();
    }

    if ( getlocalvar("is_on_wave_r") == 1 ) {
        setlocalvar("is_on_wave_r", NULL());
    }
    if ( getlocalvar("is_on_wave_l") == 1 ) {
        setlocalvar("is_on_wave_l", NULL());
    }
    if ( getlocalvar("is_on_wave_c") == 1 ) {
        setlocalvar("is_on_wave_c", NULL());
    }
    setentityvar(self,"wave",NULL());
}

int check_wave(void self, void ent) {
                float x = getentityproperty(self, "x");
                float z = getentityproperty(self, "z");
                float a = getentityproperty(self, "y");
                float base = getentityproperty(self, "base");
                float ex = getentityproperty(ent, "x");
                float ez = getentityproperty(ent, "z");
                float ea = getentityproperty(ent, "y");
                float ebase = getentityproperty(ent, "base");
                float height = getentityvar(ent, "height");
                float width = getentityvar(ent, "width");
                float depth = getentityvar(ent, "depth");
                float new_base, tmp_new_base, xshift, zshift;
                int subject_to_wave = getentityvar(self,"subject_to_wave");
                //int lost_frame = get_lost_frames()+1;

                if ( subject_to_wave <= 0 ) {
                    //reset_wave(self);
                    return 0;
                }

                // Lunghezza dell'onda
                if (x > ex) xshift = x-ex;
                else xshift = ex-x;
                xshift = xshift;


                // Aggiungere la "y"
                if ( getentityvar(ent, "type") == "wave_r" ) {
                    // Cambiare zs in base a x del personaggio
                    /*
                     * Calcoliamo lo zshift. L'ultimo gradino avrà zs = height - depth
                     * ?:x = zs:w -> ? = (x*zs)/w
                     */
                     //zshift = xshift*(height/7+depth)/width;

                    new_base = (height*xshift)/width;
                    new_base += ea;
                    tmp_new_base = new_base;

                    if ( x <= ex+width && x >= ex && z <= ez+2+depth/2 && z >= ez-depth/2 ) { // a >= new_base-2
                        if ( a <= new_base+T_WALKOFF ) new_base = get_newbase_cushoning(self,ent,new_base,tmp_new_base,xshift);

                        if ( base != new_base && a <= new_base+T_WALKOFF+get_walkoff_wave(self,ent) ) {
                            set_entity_pos(self,ent,new_base);
                        } else if ( base < new_base && a > new_base && a != base && is_flying(self) ) { // && is_flying(self)
                            if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                            changeentityproperty(self, "no_adjust_base", 1);
                            changeentityproperty(self, "base", new_base);
                            setlocalvar("is_on_wave_r", 1);
                            setentityvar(self,"wave",ent);

                            return 2; // possono esserci più scale in alto a queste coords. continuiamo la ricerca.
                        }

                        if ( a <= new_base+T_WALKOFF ) return 1;
                    } // fine if coords
                } else if ( getentityvar(ent, "type") == "wave_l" ) {
                    new_base = (height*xshift)/width;
                    new_base += ea;
                    tmp_new_base = new_base;

                    if ( x <= ex+width && x >= ex && z <= ez+2+depth/2 && z >= ez-depth/2 ) {
                        if ( a <= new_base+T_WALKOFF ) new_base = get_newbase_cushoning(self,ent,new_base,tmp_new_base,xshift);

                        if ( base != new_base && a <= new_base+T_WALKOFF+get_walkoff_wave(self,ent) ) {
                            set_entity_pos(self,ent,new_base);
                        } else if ( base < new_base && a > new_base && a != base && is_flying(self) ) { // && is_flying(self)
                            if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                            changeentityproperty(self, "no_adjust_base", 1);
                            changeentityproperty(self, "base", new_base);
                            setlocalvar("is_on_wave_l", 1);
                            setentityvar(self,"wave",ent);

                            return 2; // possono esserci più scale in alto a queste coords. continuiamo la ricerca.
                        }

                        if ( a <= new_base+T_WALKOFF ) return 1;
                    } // fine if coords
                } else if ( getentityvar(ent, "type") == "wave_c" ) {
                    new_base = height;
                    new_base += ea;
                    tmp_new_base = new_base;

                    if ( x <= ex+width && x >= ex && z <= ez+2+depth/2 && z >= ez-depth/2 ) {
                        xshift = width;
                        if ( a <= new_base+T_WALKOFF ) new_base = get_newbase_cushoning(self,ent,new_base,tmp_new_base,xshift);

                        //drawstring(10,160,0,"base: "+base);
                        //drawstring(10,170,0,"newbase: "+new_base);
                        //drawstring(10,180,0,"a: "+a);
                        if ( base != new_base && a <= new_base+T_WALKOFF+get_walkoff_wave(self,ent) ) {
                            set_entity_pos(self,ent,new_base);
                        } else if ( base < new_base && a > new_base && a != base && is_flying(self) ) { // solo se stai volando
                            if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
                            changeentityproperty(self, "no_adjust_base", 1);
                            changeentityproperty(self, "base", new_base);
                            setlocalvar("is_on_wave_c", 1);
                            setentityvar(self,"wave",ent);

                            return 2; // possono esserci più scale in alto a queste coords. continuiamo la ricerca.
                        }

                        if ( a <= new_base+T_WALKOFF ) return 1;
                    } // fine if coords
                } // fine if waves

    return 0;
}

float get_walkoff_wave(void self, void wave) {
    float adjust_wave_base_value = gev(wave,"adjust_wave_base_value");

    //adjust_wave_base_value = 20.0;
    if ( adjust_wave_base_value != NULL() ) {
        float a = getentityproperty(self, "y");
        float base = getentityproperty(self, "base");
        float tossv = getentityproperty(self, "tossv");

        if ( (!gep(self, "aiflag", "jumping") && tossv == 0) || (tossv < 0 && !gep(self, "aiflag", "jumping") && !gep(self, "aiflag", "falling")) || (a <= base) ) {
            return adjust_wave_base_value;
        } else return 0;
    } else return 0;
}

float set_entity_pos(void self, void ent, float new_base) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float xs = getentityproperty(ent, "x");
    float zs = getentityproperty(ent, "z");
    float as = getentityproperty(ent, "y");
    float bases = getentityproperty(ent, "base");
    float zdir = getentityproperty(self, "zdir");
    float tossv = getentityproperty(self, "tossv");
    //float height = getentityproperty(stair, "height");
    //float width = getentityvar(stair, 1);
    //float depth = getentityvar(stair, 3);

    if ( getlocalvar("stored_nab") == NULL() ) setlocalvar("stored_nab",getentityproperty(self,"no_adjust_base"));
    changeentityproperty(self, "no_adjust_base", 1);
    changeentityproperty(self, "base", new_base);
    //drawstring( 10,180,0,"base:  "+base+" new_base: "+new_base);
    if ( getentityvar(ent, "type") == "wave_c" ) setlocalvar("is_on_wave_c", 1);
    else if ( getentityvar(ent, "type") == "wave_r" ) setlocalvar("is_on_wave_r", 1);
    else if ( getentityvar(ent, "type") == "wave_l" ) setlocalvar("is_on_wave_l", 1);
    setentityvar(self,"wave",ent);

    if ( (!getentityproperty(self, "aiflag", "jumping") && tossv == 0) || (tossv < 0) || (a <= base) ) {
        //changeentityproperty(self, "tosstime", T_TOSSTIME);
        //changeentityproperty(self, "position", NULL(), z+2, new_base); // il +2 è per mostrare il pg anche quando va UP
        //finish_anim(self); // for landframes
        if ( !is_flying(self) ) changeentityproperty(self, "position", NULL(), NULL(), new_base);
        reset_jumping(self);
    } // fine if jump, zdir

    // se il pg è già sulla scala (infatti non cade) facciamolo rimanere sospeso così appena
    // tocca su o giù può agire la depth per capire se il pg può cadere dalla scala o meno
    /*else if ( getentityproperty(self, "tossv") == 0 ) {
        changeentityproperty(self, "tosstime", T_TOSSTIME);
    }*/

    return new_base;
}

float get_newbase_cushoning(void self, void ent, float new_base, float tmp_new_base, float xshift) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float height = getentityvar(ent, "height");
    float ea = getentityproperty(ent, "y");
    float width = getentityvar(ent, "width");
    //float depth = getentityvar(ent, 3);
    int lost_frame = get_lost_frames()+1;
    int game_speed = openborvariant("game_speed");

    if ( a < new_base-2.0 ) {
        // first time cushoning
        if ( (getentityvar(ent, "type") == "wave_c" && getlocalvar("is_on_wave_c") == NULL())
            || (getentityvar(ent, "type") == "wave_l" && getlocalvar("is_on_wave_l") == NULL())
            || (getentityvar(ent, "type") == "wave_r" && getlocalvar("is_on_wave_r") == NULL()) ) {
            lost_frame = 1;
        }

        new_base = base + sum(BOOST_FACTOR,lost_frame); // se ti avvicini all'onda sali di BOOST_FACTOR per volta
        if ( new_base > tmp_new_base ) new_base = tmp_new_base;
    }

    // new_base ~= new_base -> new_base < new_base -> new_base > base
    // evitare che a == new_base se il tossv != 0
    if ( CUSHONING_FACTOR != 0 && (getentityproperty(self, "tossv") < 0 || getlocalvar("cush_tossv") != NULL()) ) {
        float cush_tossv = 4;
        float cush_limit = ((height*xshift)/width) * 2 * CUSH_LIMIT_FACTOR; // half hight based on xshift space. è l'altezza minima che si può raggiungere. s'imposta come metà altezza. -> 70*2 = 140
        float aspeed;

        // quando tossv < 0 allora si comincia il cushoning!!
        if ( getlocalvar("cush_tossv") == NULL() && getentityproperty(self, "tossv") < 0 ) setlocalvar("cush_tossv", getentityproperty(self, "tossv"));
        else if ( getlocalvar("cush_tossv") == NULL() ) setlocalvar("cush_tossv", cush_tossv);

        // FARE CHE NON SI SUPERA cush_limit!!
        /// quanti CUSHONING_FACTOR servono per portare a 0 la cush_ashift? -> CUSHONING_FACTOR*x = aspeed -> x = aspeed/CUSHONING_FACTOR
        /// sapendo che aspeed viene decrementata costantemente di CUSHONING_FACTOR ogni frame, quale sarà il valore minimo di new_base se viene decrementato di x ogni frame?
        /// v = s/t -> s = vt -> t = s/v
        /// 0 = v(t0) +a*t -> 0 = v(t0) +a*(s/v(t0)) -> v(t0) = -1*a*s/v(t0) -> v(t0)^2 = -1*a*s -> v(t0)^2 = d*s -> v(t0) = sqrt(a*s)
        /// moto uniformemente accelerato: spazio = cush_limit, velocità finale = 0, de/accelerazione = CUSHONING_FACTOR, calcolare velocità iniziale!
        //aspeed = getlocalvar("cush_tossv")/5;
        // 140*0.02 = sqrt(2.8) = 1.67
        aspeed = sqrt(abs(sum(CUSHONING_FACTOR,lost_frame)*cush_limit)/lost_frame); // velocità per arrivare dall'alto verso cush_limit (il basso) ed assicurarsi di non oltrepassare il cush_limit

        /*drawstring(10,100,0,"cush_limit: "+cush_limit);
        drawstring(10,110,0,"aspeed: "+aspeed);
        drawstring(10,120,0,"cush_ashift: "+getlocalvar("cush_ashift"));
        drawstring(10,130,0,"height: "+height);
        drawstring(10,140,0,"new_base: "+new_base);
        drawstring(10,150,0,"cush_new_base: "+getlocalvar("cush_new_base"));*/

        // Ammortizziamo la spinta verso il basso fino a tendere a 0
        if ( getlocalvar("cush_ashift") == NULL() ) setlocalvar("cush_ashift", aspeed - CUSHONING_FACTOR*lost_frame);
        else setlocalvar("cush_ashift", getlocalvar("cush_ashift") - CUSHONING_FACTOR*lost_frame);

        // cush_ashift è lo scostamento verticale. rispetto all'altezza max dell'onda (tmp_new_base) di quanto ci dobbiamo scostare?
        if ( abs(getlocalvar("cush_ashift")) > height ) setlocalvar("cush_ashift", getlocalvar("cush_ashift")%(height+1.0) ); // <--- PREVENT BUGS

        // Se si sottrae lo scostamento verso il basso (tendente a 0) ad una velocità tendente verso l'alto, otteniamo l'ammortizzamento in salita
        if ( getlocalvar("cush_new_base") == NULL() ) new_base = tmp_new_base - getlocalvar("cush_ashift"); // + BOOST_FACTOR*lost_frame;
        else new_base = getlocalvar("cush_new_base") - getlocalvar("cush_ashift");
        new_base += sum((BOOST_FACTOR/(game_speed/10)),lost_frame); // fix for slowly devices
        if ( abs(new_base) > ea+height ) new_base = ea+height; // <--- PREVENT BUGS
        setlocalvar("cush_new_base",new_base);

        if ( lost_frame > 1 ) reset_lost_frames();

        /*if ( new_base >= tmp_new_base ) {
            //new_base = tmp_new_base;
            reset_cushoning();
            reset_lost_frames();
        }*/
        if ( new_base >= tmp_new_base-2.0 ) {
            /// ---------- Reset with threshold ----------
            reset_cushoning();
            reset_lost_frames();
            //changeentityproperty(self, "velocity", 0,0,0);
            if ( getentityproperty(self, "aiflag", "jumping") ) changeentityproperty(self, "aiflag", "jumping", 0);
            //if ( !getentityproperty(self, "aiflag", "inpain") && !getentityproperty(self, "aiflag", "falling") && !getentityproperty(self, "aiflag", "dead") ) setidle(self, openborconstant("ANI_IDLE"), 1);
        }
    }

    return new_base;
}

