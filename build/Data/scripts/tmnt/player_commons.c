
int set_board_map(void player, void board) {
    int map = get_board_map(player);

    changeentityproperty(board, "map", map);
    return map;
}

int get_board_map(void player) {
    int original_map = getentityproperty(player, "map");
    int map = 0;

    if ( getentityproperty(player, "defaultmodel") == "LEO" ) {
        if ( original_map == 0 ) map = 1;
        else if ( original_map == 2 ) map = 0;
        else if ( original_map == 3 ) map = 3;
        else if ( original_map == 4 ) map = 2;
        else if ( original_map == 5 ) map = 4;
        else if ( original_map == 6 ) map = 4+4*0+4;
        else if ( original_map == 7 ) map = 4+4*0+3;
        else if ( original_map == 8 ) map = 4+4*0+2;
        else if ( original_map == 9 ) map = 4+4*0+1;
        else if ( original_map == 1 ) map = 4+4*4+1;
    } else if ( getentityproperty(player, "defaultmodel") == "MIKE" ) {
        if ( original_map == 0 ) map = 2;
        else if ( original_map == 2 ) map = 0;
        else if ( original_map == 3 ) map = 3;
        else if ( original_map == 4 ) map = 4;
        else if ( original_map == 5 ) map = 1;
        else if ( original_map == 6 ) map = 4+4*1+4;
        else if ( original_map == 7 ) map = 4+4*1+3;
        else if ( original_map == 8 ) map = 4+4*1+2;
        else if ( original_map == 9 ) map = 4+4*1+1;
        else if ( original_map == 1 ) map = 4+4*4+2;
    } else if ( getentityproperty(player, "defaultmodel") == "DON" ) {
        if ( original_map == 0 ) map = 3;
        else if ( original_map == 2 ) map = 0;
        else if ( original_map == 3 ) map = 1;
        else if ( original_map == 4 ) map = 2;
        else if ( original_map == 5 ) map = 4;
        else if ( original_map == 6 ) map = 4+4*2+4;
        else if ( original_map == 7 ) map = 4+4*2+3;
        else if ( original_map == 8 ) map = 4+4*2+2;
        else if ( original_map == 9 ) map = 4+4*2+1;
        else if ( original_map == 1 ) map = 4+4*4+3;
    } else if ( getentityproperty(player, "defaultmodel") == "RAPH" ) {
        if ( original_map == 0 ) map = 4;
        else if ( original_map == 2 ) map = 0;
        else if ( original_map == 3 ) map = 3;
        else if ( original_map == 4 ) map = 2;
        else if ( original_map == 5 ) map = 1;
        else if ( original_map == 6 ) map = 4+4*3+4;
        else if ( original_map == 7 ) map = 4+4*3+3;
        else if ( original_map == 8 ) map = 4+4*3+2;
        else if ( original_map == 9 ) map = 4+4*3+1;
        else if ( original_map == 1 ) map = 4+4*4+4;
    } else map = 0;

    return map;
}

int check_board_level() {
    char branch_name = openborvariant("current_branch");
    int board_level = 0;

    if ( branch_name != NULL() ) { //  && getglobalvar("board_level") == NULL()
        char surf_lv = "surf_lv";
        char skate_lv = "skate_lv";
        char ufo_lv = "ufo_lv";
        char snow_lv = "snowboard_lv";
        char surf_branch = strinfirst(branch_name, surf_lv);
        char skate_branch = strinfirst(branch_name, skate_lv);
        char ufo_branch = strinfirst(branch_name, ufo_lv);
        char snow_branch = strinfirst(branch_name, snow_lv);

        if ( surf_branch != -1 ) {
            board_level = 1;
        } else if ( skate_branch != -1 ) {
            board_level = 2;
        } else if ( ufo_branch != -1 ) {
            board_level = 3;
        } else if ( snow_branch != -1 ) {
            board_level = 4;
        } else board_level = 0;
    } else {
        if ( getglobalvar("board_level") != NULL() ) board_level = getglobalvar("board_level");
    }

    if ( getglobalvar("board_level") == NULL() ) setglobalvar("board_level",board_level);
    if ( board_level == 0 ) setglobalvar("board_level",NULL());

    return board_level;
}

