
#include "data/scripts/defines.h"

#import "data/scripts/lib.c"

int return_to_noweapon(void self) {
    if ( gev(self,"return_to_noweapon") == 1 ) {
        char model = gep(self,"model");
        int type = gep(self,"type");
        int animid = gep(self,"animationid");

        if ( type == oc("TYPE_ENEMY") ) {
            if ( gev(self,"return_to_noweapon") == 1 && is_on_base(self) ) {
                sev(self,"return_to_noweapon",NULL());
                cep(self,"weapon",1);
            }
        } else if ( type == oc("TYPE_PLAYER") ) {
            if ( gev(self,"return_to_noweapon") == 1 && is_on_base(self) ) {
                sev(self,"return_to_noweapon",NULL());
                cpp(gep(self,"playerindex"),"weapon",0);
            }
        }
    }
}

int check_custom_attack(void self, char name, char atk_anim, float atk_ratio, float custom_time, int force_custom_ratio, char opp_anim_id) {
    char model = gep(self,"model");

    if ( !gep(self,"animvalid",oc(atk_anim)) ) return 0;
    if ( name != NULL() && name != "" ) {
        if ( model != name ) return 0;
    }
    if ( gep(self,"type") == oc("TYPE_ENEMY") ) {
        char id = "_"+atk_anim;
        int time = ov("elapsed_time");
        int ATK_TYPE = oc(atk_anim);
        void target = findtarget(self,ATK_TYPE);

        // Se sei stato custom grabbed...
        if ( getentityvar(self,28) != NULL() ) {
            slv("queued_atk"+id,NULL());
            slv("queued_atk_time"+id,NULL());

            return 0;
        }

        if ( opp_anim_id != NULL() ) {
            if ( oc(opp_anim_id) != gep(target,"animationid") ) {
                slv("queued_atk"+id,NULL());
                slv("queued_atk_time"+id,NULL());

                return 0;
            }
        }

        //drawstring(10,100,0,"aggr: "+gep(self,"aggression"));
        if ( target != NULL() && gep(target,"exists") ) {
            int range_flag = checkrange(self, target, ATK_TYPE);

            if ( range_flag && !is_grabbed(self) && !is_grabbing(self)
                && !gep(self,"aiflag","walking") && !gep(self,"aiflag","running") && !gep(self,"aiflag","jumping")
                && !gep(self,"aiflag","inpain") && !gep(self,"aiflag","falling") && !gep(self,"aiflag","dead")
                && !gep(self,"aiflag","drop")&& !gep(self,"aiflag","turning") && !gep(self,"aiflag","frozen")
                && gep(target,"vulnerable") && !gep(target,"invincible") ) { // && gep(self, "aiflag", "idling") && !gep(self,"attacking") && !gep(self,"aiflag","attacking")

                if ( glv("queued_atk_time"+id) == NULL() ) {
                    int game_speed = ov("game_speed");
                    int ETA;
                    int r = rand_gen(0,100);
                    float aggression = gep(self,"aggression")*-1; // normalizziamo l'aggression
                    float atk_range = getentityproperty(self,"attackthrottle");
                    int atk_time = getentityproperty(self,"attackthrottletime")+1; // mai == 0

                    if ( atk_ratio == NULL() ) atk_ratio = 92;
                    if ( custom_time == NULL() ) custom_time = 1;
                    ETA = game_speed*custom_time;

                    if ( aggression > 0 ) ETA *= aggression;
                    else if ( aggression < 0 ) ETA /= abs(aggression);

                    if ( atk_range >= 0 && (force_custom_ratio <= 0 || force_custom_ratio == NULL()) ) {
                        int rand_atk = rand_gen(0,100);

                        // atk_range == 30%? abbiamo il 30% di possibilità di una cancel. L'atk va in porto se atk_range > rand_atk. 0% == attacca sicuro!
                        if ( atk_range*100 > rand_atk || atk_range == 0 ) {
                            ETA = game_speed*atk_time;
                            r = atk_ratio; // force next step to put atk in queue
                        } else {
                            slv("queued_atk"+id,NULL());
                            slv("queued_atk_time"+id,get_next_time(ETA));

                            return 0;
                        }
                    }

                    if ( r <= atk_ratio ) {
                        slv("queued_atk"+id,ATK_TYPE);
                        slv("queued_atk_time"+id,get_next_time(ETA));

                        return 1;
                    } else { // Attiviamo l'attacco comunque ma a NULL()
                        slv("queued_atk"+id,NULL());
                        slv("queued_atk_time"+id,get_next_time(ETA));

                        return 0;
                    }
                } else {
                    if ( time >= glv("queued_atk_time"+id) ) {

                        // Condizioni necessarie e sufficienti affinchè si verifichi l'attacco
                        if ( !gep(self,"aiflag","attacking") ) { // sempre per ora
                                if ( glv("queued_atk"+id) != NULL() ) {
                                    cep(self,"velocity",0,0,0);
                                    performattack(self,glv("queued_atk"+id),1);
                                    slv("queued_atk"+id,NULL());
                                    slv("queued_atk_time"+id,NULL());
                                } else {
                                    //if ( glv("queued_atk") != NULL() ) slv("queued_atk",NULL());

                                    if ( glv("queued_atk_time"+id) != NULL() ) slv("queued_atk_time"+id,NULL());
                                } // fine if queued_atk
                        } else {
                            if ( glv("queued_atk_time"+id) == NULL() || time >= glv("queued_atk_time"+id)  ) {
                                if ( glv("queued_atk"+id) != NULL() ) slv("queued_atk"+id,NULL());
                                if ( glv("queued_atk_time"+id) != NULL() ) slv("queued_atk_time"+id,NULL());
                            }
                        }
                    } // fine if queued_atk_time??
                } // fine if queued_time == NULL()??
            } /*else {
                if ( glv("queued_atk_time") == NULL() || glv("queued_atk_time") >= time  ) {
                    if ( glv("queued_atk") != NULL() ) slv("queued_atk",NULL());
                    if ( glv("queued_atk_time") != NULL() ) slv("queued_atk_time",NULL());
                }
            }*/ // fine if range
        } else {
            if ( glv("queued_atk_time") == NULL() || time >= glv("queued_atk_time")  ) {
                if ( glv("queued_atk") != NULL() ) slv("queued_atk",NULL());
                if ( glv("queued_atk_time") != NULL() ) slv("queued_atk_time",NULL());
            }
        } // fine if target
    } // fine if model

    return 0;
}

/*int check_hook_reset(void self) {
    char model = getentityproperty(self,"model");

    if ( model == "player_foot_hook" || model == "foot_hook" || model == "board_foot_hook" ) {
        int anim_id = getentityproperty(self, "animationid");
        int anim_pos = getentityproperty(self, "animpos");

        if ( anim_id == openborconstant("ANI_FOLLOW49") ) {
            void hook = getentityvar(self,13);

            if ( !getentityproperty(hook,"exists") && anim_pos > 0 ) {
                performattack(self,openborconstant("ANI_FOLLOW48"),1);
                //setidle(self,openborconstant("ANI_IDLE"),1);
            }
        }

    }
}*/

