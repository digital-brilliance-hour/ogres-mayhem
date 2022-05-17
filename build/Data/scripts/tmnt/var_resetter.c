
#include "data/scripts/defines.h"

#import "data/scripts/custom_hud.c"
#import "data/scripts/custom_hud_game_over.c"
#import "data/scripts/custom_hud.c"

void main() {
    void self = getlocalvar("self");

    if ( self != NULL() && getentityproperty(self,"exists") && openborvariant("in_level") ) {
        char name = gep(self,"name");
        if ( name == "reset_all" ) reset_vars(self,0,1);
    } else reset_vars(self);
}

int reset_vars(void self, int menu_vars_flag, int reset_all) {
    int i;

    if ( COIN_SYSTEM ) {
        for (i = 0; i < MAX_PLAYERS; ++i) {
            setlocalvar("current_credits_"+i,getglobalvar("current_credits_"+i));
        }
    }
    if ( reset_all == NULL() || reset_all <= 0 ) {
        if ( getglobalvar("1VS1") != NULL() ) setlocalvar("1VS1",getglobalvar("1VS1"));
        if ( getglobalvar("skip_intro") != NULL() ) setlocalvar("skip_intro",getglobalvar("skip_intro"));
        if ( getglobalvar("num_stages") != NULL() ) setlocalvar("num_stages",getglobalvar("num_stages"));
        if ( getglobalvar("num_versus_stages") != NULL() ) setlocalvar("num_versus_stages",getglobalvar("num_versus_stages"));

        if ( getglobalvar("in_menu") == NULL() && openborvariant("in_level") ) {
            // save need vars
            setlocalvar("current_music",getglobalvar("current_music"));
            setlocalvar("level_music",getglobalvar("level_music"));
            setlocalvar("player_nograb",getglobalvar("player_nograb"));
            setlocalvar("enemy_nograb",getglobalvar("enemy_nograb"));
            setlocalvar("board_level",getglobalvar("board_level"));

            setlocalvar("vs_winner",getglobalvar("vs_winner"));
            setlocalvar("versus_time",getglobalvar("versus_time"));
            for (i = 0; i < MAX_PLAYERS; ++i) {
                setlocalvar("versus_wins_"+i,getglobalvar("versus_wins_"+i));
                setlocalvar("versus_mark_"+i,getglobalvar("versus_mark_"+i));
                setlocalvar("versus_mark_tot_"+i,getglobalvar("versus_mark_tot_"+i));
                if ( COIN_SYSTEM == 0 ) setlocalvar("current_credits_"+i,getglobalvar("current_credits_"+i));
            }
        } else {
            setlocalvar("in_menu",getglobalvar("in_menu"));
        }
    }

    if ( getglobalvar("in_menu") == NULL() && openborvariant("in_level") ) {
        // reset all used vars
        if ( menu_vars_flag != 1 ) {
            changedrawmethod(NULL(), "enabled", 0);
            changedrawmethod(NULL(), "reset", 1);
            setdrawmethod(NULL(), 0);

            changeopenborvariant("nofadeout", 0);
            changeopenborvariant("nojoin", 0);
        }
    }

    // ##### CUSTOM HUD #####
    unload_sp_characters(); // IMPORTANT TO CLEAR LOADED FILES IN "UPDATE.C"!!
    unload_continue_nums();
    //clear_color_font();

    setglobalvar("in_menu", NULL());
    setglobalvar("menu_mode",NULL());
    setglobalvar("playing_gif", NULL());
    setglobalvar("playing_wildcards", NULL());
    if ( getglobalvar("wild_card_array") != NULL() ) { free(getglobalvar("wild_card_array")); setglobalvar("wild_card_array", NULL()); }
    setglobalvar("clock", NULL());
    setglobalvar("last_z", NULL());
    setglobalvar("last_player_spawned", NULL());
    setglobalvar("beep", NULL());
    setglobalvar("beeping_ent", NULL());
    setglobalvar("player_nograb", NULL());
    setglobalvar("enemy_nograb", NULL());
    setglobalvar("boss1",NULL());
    setglobalvar("boss2",NULL());
    setglobalvar("ratio_flag", NULL());
    setglobalvar("current_music", NULL());
    setglobalvar("level_music", NULL());
    setglobalvar("prev_music",NULL());
    setglobalvar("countdown_time",NULL());
    setglobalvar("board_level",NULL());
    setglobalvar("go_arrow", NULL());
    setglobalvar("vs_fight_flag",NULL());
    setglobalvar("climb_press_button", NULL());
    setglobalvar("climb_press_active", NULL());
    setglobalvar("add_credits_master", NULL());
    setglobalvar("fake_credit", NULL());

    if ( getglobalvar("loading_icon") != NULL() ) { free(getglobalvar("loading_icon")); setglobalvar("loading_icon", NULL()); }

    /*for (i = 0; i < MAX_PLAYERS; ++i) {
        setglobalvar("v"+(i+1)+"Player", NULL());
    }*/

    for (i = 0; i < MAX_PLAYERS; ++i) {
        setglobalvar("nosame"+i, NULL());
        setglobalvar("nosamecolour"+i, NULL());
        setglobalvar("p"+(i+1)+"respawn_x", NULL());
        setglobalvar("p"+(i+1)+"respawn_z", NULL());
        setglobalvar("p"+(i+1)+"respawn_a", NULL());
        setglobalvar("p"+(i+1)+"respawn_base", NULL());
        setglobalvar("p"+(i+1)+"respawn_platform", NULL());
        setglobalvar("p"+(i+1)+"spawning", NULL());
        setglobalvar("p"+(i+1)+"spawn_time", NULL());
        //setglobalvar("p"+(i+1)+"respawn_xo", NULL());
        //setglobalvar("p"+(i+1)+"respawn_zo", NULL());
        //setglobalvar("p"+(i+1)+"respawn_ao", NULL());
        //setglobalvar("surfp"+(i+1), NULL());

        setglobalvar("add_credits_"+i, NULL());
        setglobalvar("any_button"+i, NULL());
        setglobalvar("attack_button"+i, NULL());
        setglobalvar("attack_button_escape"+i, NULL());
        setglobalvar("any_button_active"+i, NULL());
        setglobalvar("attack_button_active"+i, NULL());
        setglobalvar("atk_btn_escape_active"+i, NULL());
        setglobalvar("rrodney_button_escape"+i, NULL());
        setglobalvar("rrodney_active"+i, NULL());
        setglobalvar("frozen_button_escape"+i, NULL());
        setglobalvar("frozen_active"+i, NULL());
        setglobalvar("bubbled_button_escape"+i, NULL());
        setglobalvar("bubbled_active"+i, NULL());
        setglobalvar("muddied_button_escape"+i, NULL());
        setglobalvar("muddied_active"+i, NULL());
        setglobalvar("roped_button_escape"+i, NULL());
        setglobalvar("roped_active"+i, NULL());
        setglobalvar("bind_grab_ready"+i, NULL());
        setglobalvar("tmp_no_bind_grab"+i, NULL());
        setglobalvar("tmp_no_new_grab"+i,NULL());
        setglobalvar("custom_grab_active_flag"+i,NULL());

        //if ( getglobalvar("sc_parrow"+(i+1)) != NULL() ) { free(getglobalvar("sc_parrow"+(i+1))); setglobalvar("sc_parrow"+(i+1), NULL()); }
        //if ( getglobalvar("sc_parrow"+(i+1)+"b") != NULL() ) { free(getglobalvar("sc_parrow"+(i+1)+"b")); setglobalvar("sc_parrow"+(i+1)+"b", NULL()); }

        setglobalvar("p"+i+"ladder", NULL());
        setglobalvar("p"+i+"climbing", NULL());
        setglobalvar("p"+i+"layer", NULL());
        setglobalvar("timer_"+i, NULL());
        setglobalvar("gravity_"+i, NULL());
        setglobalvar("xspeed_"+i, NULL());
        setglobalvar("zbound_"+i, NULL());
        setglobalvar("timing_"+i, NULL());
        setglobalvar("bombtiming_"+i, NULL());
        setglobalvar("combostall_"+i, NULL());
        setglobalvar("combotiming_"+i, NULL());
        setglobalvar("gdoublegun_"+i, NULL());
        setglobalvar("bind_type"+i, NULL());
        setglobalvar("bind_partner"+i, NULL());
        setglobalvar("tmp_candamage"+i, NULL());
        setglobalvar("tmp_nograb"+i, NULL());
        setglobalvar("versus_mark_"+i,NULL());
        setglobalvar("icondie_"+i,NULL());
    } // fine for

    /*for (i = 0; i < 10; ++i) {
        setglobalvar( "e_surf"+i, NULL() );
        setglobalvar( "ex_surf"+i, NULL() );
    }*/

    clearglobalvar(); // puliamo tutto

    if ( COIN_SYSTEM ) {
        for (i = 0; i < MAX_PLAYERS; ++i) {
            setglobalvar("current_credits_"+i,getlocalvar("current_credits_"+i));
        }
    }
    if ( reset_all == NULL() || reset_all <= 0 ) {
        // re-save need vars
        if ( getlocalvar("in_menu") == NULL() ) {
            setglobalvar("current_music",getlocalvar("current_music"));
            setglobalvar("level_music",getlocalvar("level_music"));
            setglobalvar("player_nograb",getlocalvar("player_nograb"));
            setglobalvar("enemy_nograb",getlocalvar("enemy_nograb"));
            setglobalvar("board_level",getlocalvar("board_level"));
        } else {
            setglobalvar("in_menu",getlocalvar("in_menu"));
        }

        // restore non-resettable vars
        setglobalvar("1VS1",getlocalvar("1VS1"));
        setglobalvar("skip_intro",getlocalvar("skip_intro"));
        setglobalvar("num_stages",getlocalvar("num_stages"));
        setglobalvar("num_versus_stages",getlocalvar("num_versus_stages"));
        setglobalvar("vs_winner",getlocalvar("vs_winner"));
        setglobalvar("versus_time",getlocalvar("versus_time"));
        for (i = 0; i < MAX_PLAYERS; ++i) {
            setglobalvar("versus_wins_"+i,getlocalvar("versus_wins_"+i));
            setglobalvar("versus_mark_"+i,getlocalvar("versus_mark_"+i));
            setglobalvar("versus_mark_tot_"+i,getlocalvar("versus_mark_tot_"+i));
            if ( COIN_SYSTEM == 0 ) setglobalvar("current_credits_"+i,getlocalvar("current_credits_"+i));
        }
    }

    clearlocalvar(); // puliamo tutto

    if ( self != NULL() && getentityproperty(self,"exists") && openborvariant("in_level") ) killentity(self);

    return;
}

