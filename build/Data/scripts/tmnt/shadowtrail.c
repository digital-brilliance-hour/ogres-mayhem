
#include "data/scripts/defines.h"
#import "data/scripts/lib.c"

#define MAX_TRAILS 50

/*
Per pulire piano tutta la trail.
Impostare un flag ON come parametro nella func. Poi ad ogni aggiornamento dello sprite cancella la coda (localvar spr) 2 sprites per volta.
Quindi cancella 2 sprites e aggiorna di 1. Si cancellerà uno sprites per volta. Quando sono tutti cancellati pulisci la trail.
Se il flag è on e se tutta la coda è pulita allora non chiamare più il metodo draw_trail.
In alternativa si puà usare updated.c ed eseguire il metodo fintanto che la coda è piena...
*/


int clear_shadowtrail(void ent) {
    int i;

        for( i = 0; i < MAX_TRAILS; ++i) {
            setlocalvar("xpos"+ent+"_"+i, NULL());
            setlocalvar("zpos"+ent+"_"+i, NULL());
            setlocalvar("apos"+ent+"_"+i, NULL());
            setlocalvar("facing"+ent+"_"+i, NULL());
            setlocalvar("spr"+ent+"_"+i, NULL());
            setlocalvar(ent+"_k",NULL());
            reset_lost_frames(NULL(),"st_lost_frm");
        }

   return 0;
}

int get_oldest(void ent, int maxtrails) {
    if ( getlocalvar(ent+"_k") == NULL() ) return NULL();
    else {
        int k,i;

        if ( getlocalvar(ent+"_k") != NULL() ) k = getlocalvar(ent+"_k")-1; else k = 0;
        if (k < 0 ) k = maxtrails-1;

        for( i = k+1; i < maxtrails; ++i) { // dal k+1 c'è possibilità di trovare il k-vecchio
            if ( getlocalvar("spr"+ent+"_"+i) != NULL() ) return i;
        }
        for( i = 0; i <= k; ++i) { // ricominciamo dall'inizio fino a k compreso (evidentemente 1 solo sprite in tutto)
            if ( getlocalvar("spr"+ent+"_"+i) != NULL() ) return i;
        }

        return NULL();
    }
}

int draw_shadowtrail(void ent, int maxtrails, int refresh_frm, int min_transp, int max_transp, int swap_flag, int on_off_switch, int alpha, int color, int new_map) {
     int i = 0, k = 0, c = 0;
	 void spr = getentityproperty(ent, "sprite");
	 float x = getentityproperty(ent, "x");
     float z = getentityproperty(ent, "z");
     float a = getentityproperty(ent, "y");
	 int facing = getentityproperty(ent, "direction");
	 int colourmap = getentityproperty(ent, "colourmap");
     int elapsed_time = openborvariant("elapsed_time");
     float xpos, zpos, apos;
     int lost_frame = get_lost_frames("st_lost_frm",1);


     if ( new_map != NULL() ) {
         int map = getentityproperty(ent, "map");

         changeentityproperty(ent,"map",new_map);
         colourmap = getentityproperty(ent, "colourmap");
         changeentityproperty(ent,"map",map);
     }

     if ( alpha == NULL() ) alpha = 6;

	 if ( facing == 1 ) facing = 0;
	 else facing = 1;

	 if ( maxtrails > MAX_TRAILS ) maxtrails = MAX_TRAILS;

     //drawstring( 10,200,0,"k:  "+getlocalvar(ent+"_k")+"  oldk:  "+get_oldest(ent, maxtrails));
     refresh_frm -= lost_frame;
     if (refresh_frm < 0) refresh_frm = 0;
     if ( time_to_wait("st_refresh_frm",refresh_frm) == 1 && openborvariant("in_level") ) { // memorizziamo sprites e posizioni nel tempo
            if ( getlocalvar(ent+"_k") != NULL() ) k = getlocalvar(ent+"_k"); else k = 0;

            reset_time_to_wait("st_refresh_frm");

            // SPRITES SWAP
            if ( swap_flag && getlocalvar(ent+"_k") != NULL() ) {
                int tk = k;
                void tspr = NULL();

                for( i = 0; i < maxtrails; ++i) {
                    --tk;
                    if ( tk < 0 ) tk = maxtrails-1;
                    tspr = getlocalvar("spr"+ent+"_"+tk);
                    --tk;
                    if ( tk < 0 ) tk = maxtrails-1;
                    setlocalvar("spr"+ent+"_"+tk, tspr); // inseriamo lo sprite precedente nello slot ancora precedente
                }
            }
            // END SPRITES SWAP

            if ( on_off_switch && on_off_switch != NULL() ) {
                setlocalvar("xpos"+ent+"_"+k, x);
                setlocalvar("zpos"+ent+"_"+k, z);
                setlocalvar("apos"+ent+"_"+k, a);
                setlocalvar("facing"+ent+"_"+k, facing);
                setlocalvar("spr"+ent+"_"+k, spr);
                ++k;
                if ( k >= maxtrails ) k = 0;
                setlocalvar(ent+"_k", k); // Impostiamo il k recente
            } else {
                int old_k = get_oldest(ent, maxtrails);

                if ( getlocalvar(ent+"_k") != NULL() ) k = getlocalvar(ent+"_k")-1; else k = 0;
                if (k < 0 ) k = maxtrails-1;

                if ( old_k != NULL() ) setlocalvar("spr"+ent+"_"+old_k, NULL()); // cancelliamo lo sprite più vecchio

                if ( old_k != NULL() && old_k == k ) {
                        clear_shadowtrail(ent); // se abbiamo cancellato anche l'ultimo sprite, puliamo tutto!!
                        return 0;
                }
            } // fine on/off switch
     } // if time_to_wait()
     //reset_lost_frames(0,"st_lost_frm");



     if ( openborvariant("in_level") ) { // draw trails
         for( i = 0; i < maxtrails; ++i) {
                int transp, gradient;
                int layer = getentityproperty(ent, "setlayer");

                if ( getlocalvar("spr"+ent+"_"+i) != NULL() ) {
                     x = getlocalvar("xpos"+ent+"_"+i);
                     z = getlocalvar("zpos"+ent+"_"+i);
                     a = getlocalvar("apos"+ent+"_"+i);
                     facing = getlocalvar("facing"+ent+"_"+i);
                     spr = getlocalvar("spr"+ent+"_"+i);

                     // naturalmente il più recente deve essere il meno trasparente, ecco perchè usiamo k e non i!!
                     if ( getlocalvar(ent+"_k") != NULL() ) k = getlocalvar(ent+"_k")-1; else k = 0;
                     if (k < 0 ) k = maxtrails-1;
                     // Presa la k più recente impostiamo la k corrispondente alla i dalla più recente alla meno recente
                     // Dobbiamo sottrarre quindi maxtrails-i a k
                     /*for ( c = 0; c < i; ++c ) {
                        --k;
                        if ( k < 0 ) k = maxtrails-1;
                     }
                     gradient = maxtrails-1;
                     for ( c = 0; c < k; ++c ) {
                        --gradient;
                        if ( gradient < 0 ) gradient = maxtrails-1;
                     }*/

                     c = maxtrails-1-k;
                     gradient = (i+c)%maxtrails;

                     changedrawmethod(NULL(), "enabled", 1);
                     changedrawmethod(NULL(), "reset", 1);
                     //changedrawmethod(NULL(), "flag", 1);

                     changedrawmethod(NULL(), "alpha", alpha);
                     if ( color != NULL() ) {
                         changedrawmethod(NULL(), "tintmode", alpha);
                         changedrawmethod(NULL(), "tintcolor", color); // rgbcolor(0x00,0x00,0xFF)
                     }

                     transp = ( (max_transp-min_transp)*(gradient+1)/maxtrails )+min_transp;
                     changedrawmethod(NULL(), "channelg", transp);
                     changedrawmethod(NULL(), "channelr", transp);
                     changedrawmethod(NULL(), "channelb", transp);

                     // (entity, int flag, int scalex, int scaley, int flipx, int flipy, int shiftx, int alpha, int remap, int fillcolor, int rotate, int fliprotate, int transparencybg, void* colourmap, int centerx, int centery);
                     setdrawmethod(NULL(), 1, 256, 256, facing, 0, 0, alpha, 1, 0, 0, 0, 0, colourmap); // map: -1 = Use entity's colormap.

                     if ( x != NULL() ) drawsprite(spr, x-openborvariant("xpos"), z-a-openborvariant("ypos")-4, z-(maxtrails-1-gradient), layer);
                     changedrawmethod(NULL(), "enabled", 0);
                     changedrawmethod(NULL(), "reset", 1);
                     setdrawmethod(NULL(), 0);
                } // fine if spr != NULL()

         } // fine for
     } // if in_level

    return 1;
}


