
#include "data/scripts/defines.h"

//#import "data/scripts/lib_wall.c"
#import "data/scripts/lib_platform.c"


void spawnsubentity(char ent, float x, float z, float a) {
    void subent;

      clearspawnentry();
      setspawnentry("name", ent);
      setspawnentry("coords", x,z,a);
      subent = spawn();

      changeentityproperty(subent, "position", x, z, a);

    return subent;
}

void spawnsubentity_parent(char ent, float x, float z, float a, void parent) {
    void subent;

      clearspawnentry();
      setspawnentry("name", ent);
      setspawnentry("coords", x,z,a);
      subent = spawn();

      changeentityproperty(subent, "parent", parent);
      changeentityproperty(subent, "position", x, z, a);

    return subent;
}

void spawnsubentity_bind_to_parent(char ent, void parent) {
    void subent = NULL();

      if ( getentityproperty(parent,"exists") ) {
          float x = getentityproperty(parent, "x");
          float z = getentityproperty(parent, "z");
          float a = getentityproperty(parent, "y");
          float base = getentityproperty(parent, "base");

          clearspawnentry();
          setspawnentry("name", ent);
          setspawnentry("coords", x,z,a);
          subent = spawn();

          changeentityproperty(subent, "position", x, z, a);
          changeentityproperty(subent, "base", base);
          changeentityproperty(subent, "parent", parent);
      }

    return subent;
}

void spawnsubentity_relative(char ent, float rx, float rz, float ra, void parent) {
    void subent;
    float x = 0, z = 0, a = 0, base = 0;

    clearspawnentry();
    setspawnentry("name", ent);

      if ( getentityproperty(parent,"exists") ) {
          int p_dir = getentityproperty(parent,"direction");

          if (!p_dir) rx *= -1;

          x = getentityproperty(parent, "x");
          z = getentityproperty(parent, "z");
          a = getentityproperty(parent, "y");
          base = getentityproperty(parent, "base");
          //changeentityproperty(subent, "direction", p_dir);
          //changeentityproperty(subent, "parent", parent);
      } else {
          x = 0;
          z = 0;
          a = 0;
      }

      //setspawnentry("flip", p_dir);
      setspawnentry("coords", x+rx,z+rz,a+ra);
      subent = spawn();

      if ( ra != NULL() ) {
        changeentityproperty(subent, "position", x+rx, z+rz, a+ra);
        changeentityproperty(subent, "base", a+ra);
      } else {
        changeentityproperty(subent, "position", x+rx, z+rz, base);
        changeentityproperty(subent, "base", base);
      }

      if ( getentityproperty(parent,"exists") ) {
          int p_dir = getentityproperty(parent,"direction");

          changeentityproperty(subent, "direction", p_dir);
          changeentityproperty(subent, "parent", parent);
      }

    return subent;
}

void spawnbuttons(char ent, void parent) {
    void subent;

      clearspawnentry();
      setspawnentry("name", ent);
      subent = spawn();

      changeentityproperty(subent, "parent", parent);

    return subent;
}

void get_parent(void ent) {
	void parent = getentityproperty(ent,"parent");
	void owner = getentityproperty(ent,"owner");
  	void subentity = getentityproperty(ent, "subentity");

  	if ( parent == NULL() ) parent = owner;
  	if ( parent == NULL() ) parent = subentity;

  	return parent;
}

int rand_gen(int x, int y, int seed_flag) {
    int num = 0;
    int r;

    if ( seed_flag != NULL() && seed_flag > 0 ) r = get_rand_seed();
    else r = rand();

    if (r < 0) r *= -1;
    num = x+(r%((y-x)+1));

    return num;
}

int get_rand_seed() {
    int seed = 0, tmp = 0;
    char rf = "";

    seed = ov("ticks")&rand();
    tmp = seed;
    seed = seed << 16;
    tmp = tmp >> 16;

    if ( getentity(0) != NULL() ) {
        rf = getentity(0)+"";
        rf = substr(rf,1,strlength(rf)-1);
        rf = to_float(rf);
        seed /= ov("elapsed_time");
        tmp = tmp | rf;
    } else {
        seed /= ov("elapsed_time");
    }

    seed = seed | rand();
    seed = (seed & tmp) | ((!seed) & rf);

    return seed;
}

void setframe(void self, int min_frm, int max_frm) {
    if ( getentityproperty(self,"aiflag","animating") != -1 ) changeentityproperty(self,"animpos",min_frm);
    else changeentityproperty(self,"animpos",max_frm);
}

void remove(void array, int index, int nofree_flag) {
    int i = 0, c = 0;
    void tmp_array;

    if ( typeof(get(array,index)) == openborconstant("VT_PTR") && (nofree_flag == NULL() || nofree_flag <= 0) ) free(get(array,index));
    set(array,index,NULL());

    for ( i = 0; i < size(array); ++i ) {
        void elem = get(array,i);

        if ( elem != NULL() ) set(tmp_array,c++,elem);
    }

    free(array);

    return tmp_array;
}

int stop_last_sound(int snd_id) {
    int playid = openborvariant("sample_play_id");
    int channel;

    if ( snd_id != NULL() ) playid = snd_id;

    //if(playsample(yousampleid)) setentityvar(self, "lastchannel", openborvariant("sample_play_id"));

    if ( playid ) {
        channel = querychannel(playid);
        //drawstring(20,50,0,channel);
        if ( channel >= 0 ) stopchannel(channel);
    }

    return playid;
}

int draw_anim_string(int x, int y, int font_index, char str, int refresh_time, int loop_flag, int sound) {
    int time = openborvariant("elapsed_time");
    char temp_str = "";

    if ( getlocalvar("timing") == NULL() ) setlocalvar("timing", get_next_time(refresh_time));
    if ( getlocalvar("cursor") == NULL() ) setlocalvar("cursor", 0);

    // Check Time-Reset
    if ( check_stored_time(getlocalvar("timing"),9999) ) setlocalvar("timing", openborconstant("MIN_INT"));

    if ( getlocalvar("prev_char") == getchar(str, getlocalvar("cursor")) && getlocalvar("prev_char") == " " ) setlocalvar("timing",time-1); // non contiamo gli spazi consecutivi
    if ( getlocalvar("timing") < time ) {
        if (sound != NULL() && getlocalvar("cursor") < strlength(str) && getchar(str, getlocalvar("cursor")) != " " ) playsample(sound);
        setlocalvar("prev_char",getchar(str, getlocalvar("cursor")));
        setlocalvar("cursor", getlocalvar("cursor")+1);
        setlocalvar("timing", NULL());
        if ( loop_flag == 1 ) {
            if ( getlocalvar("cursor") > strlength(str) ) setlocalvar("cursor", 0); // Oppure 1 se si vuole ricominciare dalla prima lettera
        } else {
            if ( getlocalvar("cursor") > strlength(str) ) setlocalvar("cursor", strlength(str));
        }
    }

    temp_str = strleft(str, getlocalvar("cursor"));

    drawstring(x,y,font_index,temp_str);

    return strlength(temp_str);
}

int clear_draw_anim_string() {
    setlocalvar("cursor", NULL());
    setlocalvar("timing", NULL());
}

int complete_draw_anim_string(char str) {
    setlocalvar("cursor", strlength(str));
}

float digit_num(int num) {
    int i = 0;
    int tmp_num = num;

    if ( num == NULL() ) return 0;
    if ( tmp_num <= 0 ) {
        ++i;
    } else {
        while( tmp_num > 0 ) {
            int digit;

            digit = truncA(tmp_num%10); // ex. 526%100 = 26
            tmp_num = truncA(tmp_num/10); // ex. 526/100 = 5
            ++i;
        } // fine while
    }

    return i;
}

char getchar(char str, int index) {
    if ( index >= strlength(str) ) index = strlength(str)-1;
    else if ( index < 0 ) index = 0;

    if ( strlength(str) > 0 ) {
        str = strright(str, index);

        if ( strlength(str) > 1 ) {
            index = 1;
            str = strleft(str, index);
        }
    } //else str = "";

    return str;
}

char setchar(char str, int index, char substr) {
    char str1, str2;

    if ( index+1 > strlength(str) ) return NULL();

    str1 = substr(str,0,1 + index-1);
    str2 = substr(str,index+1,strlength(str) - (index+1));

    return (str1+substr+str2);
}

char substr(char str, int start_index, int length) {
    if ( start_index+length > strlength(str) ) return NULL();
    if ( start_index < 0 || strlength(str) <= 0 ) return NULL();

    str = strright(str, start_index);
    str = strleft(str, length);

    return str;
}

int strcmp(char A, char B, int case_flag) {
    if ( strlength(A) != strlength(B) ) return 0;
    if ( search_str(A,B,case_flag) != -1 ) return 1;
    else return 0;
}

char search_str(char str, char sub, int case_flag) {
    char substr;

    if ( str == NULL() || sub == NULL() ) return -1;
    if ( case_flag == NULL() ) case_flag = 0;

    if ( case_flag ) substr = strinfirst(lowercase(str), lowercase(sub));
    else substr = strinfirst(str, sub);

    return substr;
}

char replace_str(char txt, char pat, char rep) {
    char str_left = "", str_right = "";
    int len = strlength(pat);

    str_left = strinfirst(txt,pat);
    if ( str_left == -1 ) return -1;
    str_left = substr(txt,0,strlength(txt)-strlength(str_left));

    str_right = strinfirst(txt,pat);
    str_right = substr(str_right,len,strlength(str_right)-len);

    return ( str_left+rep+str_right );
}

int searchstr_index(char txt, char pat, int reverse_flag) {
    char tmp = "";

        tmp = strinfirst(txt,pat);
        if ( tmp == -1 ) return -1;
        else return ( strlength(txt)-strlength(tmp) );

    /*if ( !reverse_flag || reverse_flag == NULL() ) {
        tmp = strinfirst(txt,pat);
        if ( tmp == -1 ) return -1;
        else return ( strlength(txt)-strlength(tmp) );
    } else {
        tmp = strinlast(txt,pat); return tmp;
        if ( tmp == -1 ) return -1;
        else return ( strlength(txt)-strlength(tmp) );
    }*/
}

char uppercase(char str) {
    int i = 0;
    char cstr = "";

    for ( i = 0; i < strlength(str); ++i ) {
        char ch = getchar(str,i);

             if ( ch == "a" ) ch = "A";
        else if ( ch == "b" ) ch = "B";
        else if ( ch == "c" ) ch = "C";
        else if ( ch == "d" ) ch = "D";
        else if ( ch == "e" ) ch = "E";
        else if ( ch == "f" ) ch = "F";
        else if ( ch == "g" ) ch = "G";
        else if ( ch == "h" ) ch = "H";
        else if ( ch == "i" ) ch = "I";
        else if ( ch == "j" ) ch = "J";
        else if ( ch == "k" ) ch = "K";
        else if ( ch == "l" ) ch = "L";
        else if ( ch == "m" ) ch = "M";
        else if ( ch == "n" ) ch = "N";
        else if ( ch == "o" ) ch = "O";
        else if ( ch == "p" ) ch = "P";
        else if ( ch == "q" ) ch = "Q";
        else if ( ch == "r" ) ch = "R";
        else if ( ch == "s" ) ch = "S";
        else if ( ch == "t" ) ch = "T";
        else if ( ch == "u" ) ch = "U";
        else if ( ch == "v" ) ch = "V";
        else if ( ch == "x" ) ch = "X";
        else if ( ch == "y" ) ch = "Y";
        else if ( ch == "w" ) ch = "W";
        else if ( ch == "z" ) ch = "Z";

        cstr += ch;
    }

    return cstr;
}

char lowercase(char str) {
    int i = 0;
    char cstr = "";

    for ( i = 0; i < strlength(str); ++i ) {
        char ch = getchar(str,i);

             if ( ch == "A" ) ch = "a";
        else if ( ch == "B" ) ch = "b";
        else if ( ch == "C" ) ch = "c";
        else if ( ch == "D" ) ch = "d";
        else if ( ch == "E" ) ch = "e";
        else if ( ch == "F" ) ch = "f";
        else if ( ch == "G" ) ch = "g";
        else if ( ch == "H" ) ch = "h";
        else if ( ch == "I" ) ch = "i";
        else if ( ch == "J" ) ch = "j";
        else if ( ch == "K" ) ch = "k";
        else if ( ch == "L" ) ch = "l";
        else if ( ch == "M" ) ch = "m";
        else if ( ch == "N" ) ch = "n";
        else if ( ch == "O" ) ch = "o";
        else if ( ch == "P" ) ch = "p";
        else if ( ch == "Q" ) ch = "q";
        else if ( ch == "R" ) ch = "r";
        else if ( ch == "S" ) ch = "s";
        else if ( ch == "T" ) ch = "t";
        else if ( ch == "U" ) ch = "u";
        else if ( ch == "V" ) ch = "v";
        else if ( ch == "X" ) ch = "x";
        else if ( ch == "Y" ) ch = "y";
        else if ( ch == "W" ) ch = "w";
        else if ( ch == "Z" ) ch = "z";

        cstr += ch;
    }

    return cstr;
}

int KMPSearch(char txt, char pat) { // KMPSearch: Knuth–Morris–Pratt Algorithm
    int M = strlength(pat);
    int N = strlength(txt);

    // create lps[] that will hold the longest prefix suffix values for pattern
    int lps = array(M);
    int j = 0;  // index for pat[]
    int i = 0;  // index for txt[]

    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);

    while( i < N ) {
      if( strcmp(getchar(pat,j),getchar(txt,i)) ) {
        j++;
        i++;
      }

      if ( j == M ) {
        //printf("Found pattern at index %d \n", i-j);
        return ( i-j );

        j = get(lps,j-1);
        // mismatch after j matches
      } else if( !strcmp(getchar(pat,j),getchar(txt,i)) ) {
        // Do not match lps[0..lps[j-1]] characters,
        // they will match anyway
        if (j != 0) j = get(lps,j-1);
        else ++i;
      }
    }
    free(lps); // to avoid memory leak

    return -1;
}

void computeLPSArray(char pat, int M, int lps) {
    int len = 0;  // lenght of the previous longest prefix suffix
    int i;

    set(lps,0,0); // lps[0] is always 0
    i = 1;

    // the loop calculates lps[i] for i = 1 to M-1
    while( i < M ) {
       if( strcmp(getchar(pat,i),getchar(pat,len)) ) {
         len++;
         set(lps,i,len);
         i++;
       } else { // (pat[i] != pat[len])
         if( len != 0 ) {
           // This is tricky. Consider the example AAACAAAA and i = 7.
           len = get(lps,len-1);
           // Also, note that we do not increment i here
         } else { // if (len == 0)
           set(lps,i,0);
           i++;
         }
       }
    }
}

int get_num_entities() { // usare openborvariant("count_entities")
    int i, count = 0;

    for (i = 0; i < openborconstant("MAX_ENTS"); ++i) {
        void ent = getentity(i);

        if ( getentityproperty(ent, "exists") ) {
            ++count;
        }
    }

    return count;
}

int truncA(int num) {
    int sign = 1;
    float min = 0.00000001;

    if ( num < 0 ) {
        sign = -1;
        num *= -1;
        min *= -1;
    }

    num += min;
    num = num%(num+1);
    num *= sign;

    return num;
}

int get_sign(int num) {
    if ( num < 0 ) return -1;
    else return 1;
}

float get_mantix(float num) {
    float mantix = num - truncA(num);

    return mantix;
}

int getFPS() {
    float average = 0;

    if ( ov("in_level") && !ov("game_paused") ) {
        int game_speed = openborvariant("game_speed");
        int time = openborvariant("elapsed_time");

        average = getlocalvar("average");

        if ( average == NULL() ) {
            setlocalvar("average", game_speed);
            average = getlocalvar("average");
        }

        // Check Time-Reset
        //if ( check_stored_time(glv("fps_time"),9999) ) setlocalvar("fps_time", openborconstant("MIN_INT"));

        if ( getlocalvar("fps_time") == NULL() ) {
            setlocalvar("fps_time", time); // start from 1
            setlocalvar("fps", time);
        } else setlocalvar("fps", getlocalvar("fps")+1);

        if ( (getlocalvar("fps_time") >= 0 && time-getlocalvar("fps_time") >= game_speed) || (getlocalvar("fps_time") < 0 && abs(time-getlocalvar("fps_time")) >= game_speed) ) {
            int exceded_frm = time-getlocalvar("fps_time")-game_speed;

            average = abs(truncA((average+getlocalvar("fps"))/2.0));
            setlocalvar("average", average);
            setlocalvar("fps", exceded_frm);
            setlocalvar("fps_time", time);
        }
        /*drawstring(10,190,0,"fps: "+getlocalvar("fps"));
        drawstring(10,200,0,"fps_time: "+getlocalvar("fps_time"));
        drawstring(10,210,0,"time_range: "+(time-getlocalvar("fps_time")+1));*/
    } else if ( !ov("in_level") ) {
        setlocalvar("average", NULL());
        setlocalvar("fps", NULL());
        setlocalvar("fps_time", NULL());
    }

    return average;
}

float abs(float num) {
    if (num < 0) num *= -1.0;

    return num;
}

float diff(float a, float b) {
    if (a >= b) a = a - b;
    else a = b - a;

    return a;
}

float diff_sign(float a, float b) {
    if ( a >= 0 && b >= 0 ) return (a - b);
    else {
        int sign = 1;

        if ( a < b ) sign = -1;

        if ( a < 0 ) {
            a += abs(a);
            b += abs(b);
        }
        if ( b < 0 ) {
            a += abs(b);
            b += abs(b);
        }

        a = (a-b)*sign;
    }

    return a;
}

int diff_dir(float a, float b) {
    if (a >= b) return 1;
    else return -1;
}

int get_ready_players_count(void self) {
    int p, count = 0;

    for (p = 0; p < MAX_PLAYERS; ++p) {
        void player = getplayerproperty(p, "entity");

        if ( getentityproperty(player,"exists") ) {
            int anim_id = getentityproperty(player, "animationid");

            if ( anim_id != openborconstant("ANI_SPAWN") && anim_id != openborconstant("ANI_RESPAWN") ) {
                if ( player != self ) ++count;
                else continue;
            }
        }
    } // fine for

    return count;
}

int living_players() {
    int p, count = 0;

    for (p = 0; p < MAX_PLAYERS; ++p) {
        void player = getplayerproperty(p, "entity");

        if ( getentityproperty(player,"exists") ) {
            if ( !getentityproperty(player,"aiflag","dead") ) ++count;
            else continue;
        }
    } // fine for

    return count;
}

// Generate safe position from a pos.
int generate_safe_pos(void self, float xfinal, float zfinal, float xscale, float zscale, void xmem, void zmem, float threshold) {
    int i;
    float base = gep(self,"base");
    float t_x, t_z;
    float orig_x = xfinal, orig_z = zfinal;
    int hole;
    float wall, platform_base;
    void platform;

    if ( threshold == NULL() ) threshold = 0;

    t_x = threshold;
    t_z = threshold;
    if ( xscale < 0 ) t_x *= -1;
    if ( zscale < 0 ) t_z *= -1;

    /*while( checkhole(xfinal+t_x,zfinal+t_z) || checkwall(xfinal+t_x,zfinal+t_z) > base || find_platform_in_a(xfinal+t_x,zfinal+t_z,base,MAX_ALTITUDE) != NULL() ) {
        xfinal += xscale;
        zfinal += zscale;
    }*/
    hole = checkhole(xfinal+t_x,zfinal);
    wall = checkwall(xfinal+t_x,zfinal);
    platform = find_platform_in_a(xfinal+t_x,zfinal,base,MAX_ALTITUDE);
    platform_base = get_platform_base(platform);
    while( (hole && base <= 0) || wall > base || (platform != NULL() && platform_base != base) ) {
        xfinal += xscale;

        hole = checkhole(xfinal+t_x,zfinal);
        wall = checkwall(xfinal+t_x,zfinal);
        platform = find_platform_in_a(xfinal+t_x,zfinal,base,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
    }
    if ( !is_incam_coords(xfinal,zfinal,0,0,0) ) {
        xfinal = orig_x;
    }

    hole = checkhole(xfinal,zfinal+t_z);
    wall = checkwall(xfinal,zfinal+t_z);
    platform = find_platform_in_a(xfinal,zfinal+t_z,base,MAX_ALTITUDE);
    platform_base = get_platform_base(platform);
    while( (hole && base <= 0) || wall > base || (platform != NULL() && platform_base != base) ) {
        zfinal += zscale;

        hole = checkhole(xfinal,zfinal+t_z);
        wall = checkwall(xfinal,zfinal+t_z);
        platform = find_platform_in_a(xfinal,zfinal+t_z,base,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
    }
    if ( !is_incam_coords(xfinal,zfinal,0,0,0) ) {
        zfinal = orig_z;
    }

    slv(xmem,xfinal);
    slv(zmem,zfinal);
}

// Generate safe position from a pos.
float generate_safe_xpos(float base, float xcoord, float zcoord, float scale, float min, float max, float threshold) {
    int i;
    float orig_x = xcoord, orig_z = zcoord;
    int hole;
    float wall, platform_base;
    void platform;

    if ( threshold == NULL() ) threshold = 0;

    if ( scale < 0 ) threshold *= -1;

    if ( xcoord < min ) xcoord = min;
    else if ( xcoord > max ) xcoord = max;

    hole = checkhole(xcoord+threshold,zcoord);
    wall = checkwall(xcoord+threshold,zcoord);
    platform = find_platform_in_a(xcoord+threshold,zcoord,base,MAX_ALTITUDE);
    platform_base = get_platform_base(platform);
    while( (hole && base <= 0) || wall > base || (platform != NULL() && platform_base != base) ) {
        xcoord += scale;
        if ( scale < 0 && xcoord <= min ) break;
        else if ( scale >= 0 && xcoord >= max ) break;

        hole = checkhole(xcoord+threshold,zcoord);
        wall = checkwall(xcoord+threshold,zcoord);
        platform = find_platform_in_a(xcoord+threshold,zcoord,base,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
    }
    if ( xcoord < min ) xcoord = min;
    else if ( xcoord > max ) xcoord = max;

    if ( !is_incam_coords(xcoord,zcoord,0,0,0) ) {
        xcoord = orig_x;
        zcoord = orig_z;
    }

    return xcoord;
}

// Generate safe position from a pos.
float generate_safe_zpos(float base, float xcoord, float zcoord, float scale, float min, float max, float threshold) {
    int i;
    float orig_x = xcoord, orig_z = zcoord;
    int hole;
    float wall, platform_base;
    void platform;

    if ( threshold == NULL() ) threshold = 0;

    if ( scale < 0 ) threshold *= -1;

    if ( zcoord < min ) zcoord = min;
    else if ( zcoord > max ) zcoord = max;

    hole = checkhole(xcoord,zcoord+threshold);
    wall = checkwall(xcoord,zcoord+threshold);
    platform = find_platform_in_a(xcoord,zcoord+threshold,base,MAX_ALTITUDE);
    platform_base = get_platform_base(platform);
    while( (hole && base <= 0) || wall > base || (platform != NULL() && platform_base != base) ) {
        zcoord += scale;
        if ( scale < 0 && zcoord <= min ) break;
        else if ( scale >= 0 && zcoord >= max ) break;

        hole = checkhole(xcoord,zcoord+threshold);
        wall = checkwall(xcoord,zcoord+threshold);
        platform = find_platform_in_a(xcoord,zcoord+threshold,base,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
    }
    if ( zcoord < min ) zcoord = min;
    else if ( zcoord > max ) zcoord = max;

    if ( !is_incam_coords(xcoord,zcoord,0,0,0) ) {
        xcoord = orig_x;
        zcoord = orig_z;
    }

    return zcoord;
}

// Generate safe position from a pos.: Scan linear pos by a distance (threshold)
float set_safe_xpos(void self, float threshold) {
    float x = getentityproperty(self,"x");
    float z = getentityproperty(self,"z");
    float a = getentityproperty(self,"y");
    float base = getentityproperty(self,"base");
    float min = openborvariant("xpos")+20;
    float max = openborvariant("xpos")+openborvariant("hresolution")-30;
    int i, scale = 1;
    int hole;
    float wall, platform_base;
    void platform;

    hole = checkhole(x+threshold,z);
    wall = checkwall(x+threshold,z);
    platform = find_platform_in_a(x+threshold,z,a,MAX_ALTITUDE);
    platform_base = get_platform_base(platform);
    while( ((hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base)) && x > min ) {
        x -= scale;

        hole = checkhole(x+threshold,z);
        wall = checkwall(x+threshold,z);
        platform = find_platform_in_a(x+threshold,z,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
    }

    hole = checkhole(x-threshold,z);
    wall = checkwall(x-threshold,z);
    platform = find_platform_in_a(x-threshold,z,a,MAX_ALTITUDE);
    platform_base = get_platform_base(platform);
    while( ((hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base)) && x < max ) {
        x += scale;

        hole = checkhole(x-threshold,z);
        wall = checkwall(x-threshold,z);
        platform = find_platform_in_a(x-threshold,z,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
    }
    if ( x < min ) x = min;
    else if ( x > max ) x = max;

    changeentityproperty(self,"position",x,NULL(),NULL());

    return x;
}

// Generate safe position from a pos.: Scan linear pos by a distance (threshold)
float set_safe_zpos(void self, float threshold) {
    float x = getentityproperty(self,"x");
    float z = getentityproperty(self,"z");
    float a = getentityproperty(self,"y");
    float base = getentityproperty(self,"base");
    float min = openborvariant("player_min_z");
    float max = openborvariant("player_max_z");
    int i, scale = 1;
    int hole;
    float wall, platform_base;
    void platform;

    hole = checkhole(x,z+threshold);
    wall = checkwall(x,z+threshold);
    platform = find_platform_in_a(x,z+threshold,a,MAX_ALTITUDE);
    platform_base = get_platform_base(platform);
    while( ((hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base)) && z > min ) {
        z -= scale;

        hole = checkhole(x,z+threshold);
        wall = checkwall(x,z+threshold);
        platform = find_platform_in_a(x,z+threshold,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
    }

    hole = checkhole(x,z-threshold);
    wall = checkwall(x,z-threshold);
    platform = find_platform_in_a(x,z-threshold,a,MAX_ALTITUDE);
    platform_base = get_platform_base(platform);
    while( ((hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base)) && z < max ) {
        z += scale;

        hole = checkhole(x,z-threshold);
        wall = checkwall(x,z-threshold);
        platform = find_platform_in_a(x,z-threshold,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
    }

    if ( z < min ) z = min;
    else if ( z > max ) z = max;

    changeentityproperty(self,"position",NULL(),z,NULL());

    return z;
}

int changedir(void player) {
    if ( getentityproperty(player, "direction") == 1 ) changeentityproperty(player, "direction", 0);
    else changeentityproperty(player, "direction", 1);
}

int disable_keys_for_frame() {
    int i;

    for ( i = 0; i < MAX_PLAYERS; ++i ) {
        changeplayerproperty(i,"keys",0);
        changeplayerproperty(i,"playkeys",0);
        changeplayerproperty(i,"newkeys",0);
    }
}

int keyp(int p, char k, int press_flag) {
    if ( press_flag == NULL() ) press_flag = 1;

	return playerkeys(p, press_flag, k);
}

int keyc(char k, int press_flag) {
	int i;

    if ( press_flag == NULL() ) press_flag = 1;
	for(i=0; i < MAX_PLAYERS; i++) {
		if(playerkeys(i, press_flag, k)) {
			return 1;
		}
	}
	return 0;
}

int keyp_coin(int p, char k, int press_flag) {
    if ( COIN_SYSTEM && getplayerproperty(p,"credits") <= 0 && getplayerproperty(p,"lives") <= 0 ) return 0;

    if ( press_flag == NULL() ) press_flag = 1;

	return playerkeys(p, press_flag, k);
}

int key_coin(char k, int press_flag) {
	int i;

    if ( press_flag == NULL() ) press_flag = 1;

	for(i = 0; i < MAX_PLAYERS; i++) {
        if ( COIN_SYSTEM && getplayerproperty(i,"credits") <= 0 && getplayerproperty(i,"lives") <= 0 ) continue;

		if(playerkeys(i, press_flag, k)) {
			return 1;
		}
	}
	return 0;
}

int anykey_coin() {
	int i;

	for(i = 0; i < MAX_PLAYERS; i++) {
        int keys = getplayerproperty(i, "keys");
        int flag_scr_shot = openborconstant("FLAG_SCREENSHOT");
        //int sdid_scr_shot = openborconstant("SDID_SCREENSHOT");

        if ( COIN_SYSTEM && getplayerproperty(i,"credits") <= 0 && getplayerproperty(i,"lives") <= 0 ) continue;

		if( keys && !is_flag_on(keys,flag_scr_shot) ) { // exclude screenshot
			return 1;
		} else return 0;
	}
	return 0;
}

int pressed_keyc(int p) {
	int i;

	for(i = 0; i < MAX_PLAYERS; i++) {
        int player_key = getplayerproperty(i,"keys");

		if( i == p  ) {
			return player_key;
		}
	}

	return 0;
}

int anykeyc() {
	int i;

	for(i = 0; i < MAX_PLAYERS; i++) {
        int keys = getplayerproperty(i, "keys");
        int flag_scr_shot = openborconstant("FLAG_SCREENSHOT");
        //int sdid_scr_shot = openborconstant("SDID_SCREENSHOT");

		if( keys && !is_flag_on(keys,flag_scr_shot) ) { // exclude screenshot
			return 1;
		} else return 0;
	}
	return 0;
}

/* PARAMS:
 * player_index: player index (ex 0,1,2,3)
 * key: pressed key string (ex. "moveleft", "move right", etc..)
 * time_range: time between 2 key impulses
 * start_press_flag: 1 == press the first time too, 0 == no first time key press
 * start_time_eta: wait time after the first key press (time between 1st and 2nd impulse)
 */
int hold_key_press(int player_index, char key, float time_range, int start_press_flag, float start_time_eta) {

    if ( playerkeys(player_index,0,key) ) {
        int time = openborvariant("elapsed_time");
        int game_speed = openborvariant("game_speed");

        //drawstring(0,20,0,"TEST KEY: "+playerkeys(player_index,0,key));
        if ( time_range == NULL() ) time_range = 0;
        if ( start_time_eta == NULL() ) start_time_eta = 0;
        time_range *= game_speed;
        start_time_eta *= game_speed;
        if ( getlocalvar("hold_time"+key+player_index) == NULL() ) {
            setlocalvar("hold_time"+key+player_index,time);

            if ( start_press_flag > 0 && getlocalvar("first_keypress"+key+player_index) == NULL() ) {
                setlocalvar("first_keypress"+key+player_index,1);
                return playerkeys(player_index,0,key);
            }
        } else if ( time-getlocalvar("hold_time"+key+player_index) >= time_range ) {
            //drawstring(0,30,0,"OK!");
            if ( start_time_eta > 0 && getlocalvar("second_keypress"+key+player_index) == NULL() ) {
                if ( time-getlocalvar("hold_time"+key+player_index) < start_time_eta ) return 0;
            }

            // simulate hold press
            if ( getlocalvar("second_keypress"+key+player_index) == NULL() ) setlocalvar("second_keypress"+key+player_index,1);
            setlocalvar("hold_time"+key+player_index,NULL());
            return playerkeys(player_index,0,key);
        }
    } else {
        if ( getlocalvar("hold_time"+key+player_index) ) setlocalvar("hold_time"+key+player_index,NULL());
        if ( getlocalvar("first_keypress"+key+player_index) ) setlocalvar("first_keypress"+key+player_index,NULL());
        if ( getlocalvar("second_keypress"+key+player_index) ) setlocalvar("second_keypress"+key+player_index,NULL());
    }

    return 0;
}

int get_next_time(int next_time) {
	int time = openborvariant("elapsed_time");
	int MAX_INT = openborconstant("MAX_INT");

	next_time = truncA(next_time);

	if ( next_time != (time+next_time)-time ) {
        changeopenborvariant("elapsed_time", 0);
        time = openborvariant("elapsed_time");
	}

    if ( time < 0 ) {
        changeopenborvariant("elapsed_time", 0);
        time = openborvariant("elapsed_time");
    }

    if ( time+next_time >= MAX_INT ) {
        changeopenborvariant("elapsed_time", 0);
        time = openborvariant("elapsed_time");
    }

    return (time+next_time);
}

int check_stored_time(int stored_time, int threshold) {
	int time = openborvariant("elapsed_time");
	int  MIN_INT = openborconstant("MIN_INT");
	int  MAX_INT = openborconstant("MAX_INT");

    //if ( time < 0 ) changeopenborvariant("elapsed_time", 0);
    if ( stored_time == NULL() ) return 0;
    else if ( time-stored_time <= MIN_INT ) return 1;
    else if ( time > stored_time && abs(time-stored_time) >= threshold ) return 1;
    else return 0;

}

// Positive time
int check_time_reset() {
    int time = openborvariant("elapsed_time");

	if ( time < 0 ) changeopenborvariant("elapsed_time", 0);
}

float tan(float teta) {
    return (sin(teta)/cos(teta));
}

float arcsin(float alpha) {
    float teta = 0;
    float pi = 3.141592653589793;

    //asin(alpha) = 0.5*pi - acos(alpha);
    teta = 0.5*pi - arccos(alpha);
    teta -= 1.57;
    teta += 90;

    return teta;
}

float arccos(float alpha) {
    float teta = 0;
    float pi = 3.141592653589793;

    teta = 3.14159-1.57079*alpha;

    teta *= 180.0/pi; // radiants to degrees
    teta -= 90;

    return teta;
}

float arc_approx(float alpha) {
    float teta = 0;
    float k, n = 80;
    float coeff;

    for ( k = 0; k <= n; k += 1.0 ) {
        coeff = (2.0)/(2.0*k+1.0);
        coeff *= exp(0.5*alpha,2.0*k+1.0);
        coeff *= fact(2.0*k) / ( fact(k) * fact(k) );

        teta += coeff;
    }

    return teta;
}

float asinA(float alpha) {
    float teta = 0;
    float pi = 3.141592653589793;

    teta = arc_approx(alpha);
    teta *= 180.0/pi; // radiants to degrees

    return teta;
}

float acosA(float alpha) {
    float teta = 0;
    float pi = 3.141592653589793;

    teta = arc_approx(alpha);
    teta = 0.5*pi - arc_approx(alpha);
    teta *= 180.0/pi; // radiants to degrees

    return teta;
}

float to_teta(float sin_teta, float cos_teta) {
    float teta = 0;

    if ( sin_teta < -1 || sin_teta > 1 ) return NULL();
    if ( cos_teta < -1 || cos_teta > 1 ) return NULL();

    teta = arccos(cos_teta);
    if ( sin_teta > 0 ) teta = sum_cyclic(teta,+180,359);
    teta = 360 - teta;
    if ( teta == 360 ) teta = 0;

    return teta;
}

float arc(float alpha) {
    float arc = 0;

    if ( alpha < -1 || alpha > 1 ) return 0;

    alpha += 1;
    arc = (180*alpha)/2;
    arc -= 90;

    return arc;
}

float inv_teta(float teta) {
    teta = sum_cyclic(teta,+180,359);

    return teta;
}

float to_arc(float sin_teta, float cos_teta) {
    float arc = 0;

    if ( sin_teta < -1 || sin_teta > 1 ) return 0;
    if ( cos_teta < -1 || cos_teta > 1 ) return 0;

    sin_teta += 1;
    // x:alpha=180:2
    arc = (180*sin_teta)/2;

    if ( cos_teta >= 0 ) arc = sum_cyclic(arc,-90,359);
    else {
        arc = sum_cyclic(arc,-270,359);
        arc = 360 - arc;
        if ( arc == 360 ) arc = 0;
    }

    return arc;
}

float qrt(float num) {
    return (num*num);
}

int get_parent_type(void self, void type) {
    void parent = getentityproperty(self,"parent");

    if ( getentityproperty(parent,"exists") ) {
        if ( getentityproperty(parent,"type") == type ) return 1;
        else return 0;
    } else return 0;
}

/*
 * Controlla che il percorso che si deve fare non è ostacolato. Reimposta le nuove coords x,z nella locazione di memoria indicata dai params
 * Restituisce 1 se è un passaggio libero!!
 */
int set_safe_path(void self, float xfinal, float zfinal, float xscale, float zscale, void xmem, void zmem, float t_bound, int no_bound_flag) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float xpos = ov("xpos");
    float ypos = ov("ypos");
    float hres = ov("hresolution");
    float vres = ov("vresolution");
    float minz = ov("player_min_z");
    float maxz = ov("player_max_z");
    float threshold = 0.99, tx, tz; // precisione
    int hole;
    float wall, platform_base;
    void platform;

    if ( t_bound == NULL() ) t_bound = 20;

    // sappiamo che x,z sono safe.
    // dobbiamo controllare che tra x,z e xfinal,zfinal non ci siano ostacoli
    setlocalvar(xmem,x);
    setlocalvar(zmem,z);

    if ( no_bound_flag == NULL() || no_bound_flag <= 0 ) {
        if ( x <= xpos+t_bound || x >= xpos+hres-t_bound ) return 0;
        if ( z <= ypos+t_bound || z >= ypos+vres-t_bound ) return 0;
    }

         if ( xfinal < x && xscale > 0 ) xscale *= -1;
    else if ( xfinal > x && xscale < 0 ) xscale *= -1;
    if ( xfinal != x && xscale == 0 ) x = xfinal;

         if ( zfinal < z && zscale > 0 ) zscale *= -1;
    else if ( zfinal > z && zscale < 0 ) zscale *= -1;
    if ( zfinal != z && zscale == 0 ) z = zfinal;

    while( x != xfinal || z != zfinal ) {
        // Scorri secondo il fattore di scalatura (diagonale/verticale/orizzontale)
        if ( x != xfinal ) x += xscale;
        if ( z != zfinal ) z += zscale;

        if ( xscale > 0 && x >= xfinal ) x = xfinal; // raggiunto meta. fermiamoci. -> DX
        else if ( xscale < 0 && x <= xfinal ) x = xfinal; // raggiunto meta. fermiamoci. -> SX
        if ( zscale > 0 && z >= zfinal ) z = zfinal; // raggiunto meta. fermiamoci. -> DOWN
        else if ( zscale < 0 && z <= zfinal ) z = zfinal; // raggiunto meta. fermiamoci. -> UP

        tx = threshold;
        tz = threshold;
        if ( xscale < 0 ) tx *= -1;
        if ( zscale < 0 ) tz *= -1;

        hole = checkhole(x+xscale,z+zscale);
        wall = checkwall(x+xscale,z+zscale);
        platform = find_platform_in_a(x+xscale,z+zscale,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
        if ( (hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base) || z < minz+zscale || z > maxz-zscale ) return 0;

        hole = checkhole(x+tx,z+tz);
        wall = checkwall(x+tx,z+tz);
        platform = find_platform_in_a(x+tx,z+tz,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
        if ( (hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base) || z < minz+tz || z > maxz-tz ) return 0;

        hole = checkhole(x,z);
        wall = checkwall(x,z);
        platform = find_platform_in_a(x,z,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
        if ( (hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base) || z < minz+threshold || z > maxz-threshold ) return 0;

        // Conserviamo l'ultima posizione sicura a partire da x,z
        setlocalvar(xmem,x);
        setlocalvar(zmem,z);
    }

    return 1;
}

// special smoothed movements (spped) -> t_bound = 3.0; // distance from final coord
int set_safe_path_smoothed(void self, float xfinal, float zfinal, float xscale, float zscale, void xmem, void zmem, float t_bound, int no_bound_flag) {
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    float a = getentityproperty(self, "y");
    float base = getentityproperty(self, "base");
    float xpos = ov("xpos");
    float ypos = ov("ypos");
    float hres = ov("hresolution");
    float vres = ov("vresolution");
    float minz = ov("player_min_z");
    float maxz = ov("player_max_z");
    float threshold = 0.99, tx, tz; // precisione
    float t_dist = 1.0, t_speed = 0.01; // precisione
    int speed_flag = 1; // cambia velocità (fai il rapporto) una sola volta
    int hole;
    float wall, platform_base;
    void platform;

    if ( t_bound == NULL() ) t_bound = 20;

    // t_dist == 1 perchè altrienti la velocità aumenta
    // sappiamo che x,z sono safe.
    // dobbiamo controllare che tra x,z e xfinal,zfinal non ci siano ostacoli
    setlocalvar(xmem,x);
    setlocalvar(zmem,z);

    if ( xscale > 0 ) xfinal -= t_bound;
    else xfinal += t_bound;
    if ( zscale > 0 ) zfinal -= t_bound;
    else zfinal += t_bound;

    if ( no_bound_flag == NULL() || no_bound_flag <= 0 ) {
        if ( x <= xpos+t_bound || x >= xpos+hres-t_bound ) return 0;
        if ( z <= ypos+t_bound || z >= ypos+vres-t_bound ) return 0;
    }

         if ( xfinal < x && xscale > 0 ) xscale *= -1;
    else if ( xfinal > x && xscale < 0 ) xscale *= -1;
    if ( xfinal != x && xscale == 0 ) x = xfinal;

         if ( zfinal < z && zscale > 0 ) zscale *= -1;
    else if ( zfinal > z && zscale < 0 ) zscale *= -1;
    if ( zfinal != z && zscale == 0 ) z = zfinal;

    while( x != xfinal || z != zfinal ) {
        float max_dist = 0, x_dist = 0, z_dist = 0, mdf_speedx, mdf_speedz;

        if ( speed_flag < 2 ) {
            // Dunque impostiamo tutte le distanze. Se la distanza == 0 allora è stata raggiunta
            if ( xscale > 0 && x < xfinal+threshold) x_dist = xfinal-x;
            else if ( xscale < 0 && x > xfinal-threshold ) x_dist = x-xfinal;
            else x_dist = 0;

            if ( zscale > 0 && z < zfinal+threshold) z_dist = zfinal-z;
            else if ( zscale < 0 && z > zfinal-threshold ) z_dist = z-xfinal;
            else z_dist = 0;

            if ( x_dist == 0 || x_dist <= t_dist ) x = xfinal;
            if ( z_dist == 0 || z_dist <= t_dist ) z = zfinal;

            if ( x_dist != 0 && z_dist != 0 ) {
                // Poi calcolare dist max. da x a dir_x per es.
                if ( x_dist > 0 ) max_dist = x_dist;
                if ( z_dist > max_dist ) max_dist = z_dist;
                if ( max_dist <= 0 ) max_dist = 1;

                // Associare speed a dist min. Stabilire attraverso una percentuale la velocità per le altre distanze.
                // L'associacione avviene tramite proporzioe -> speed:max_dist = new_speed:new_dist
                // Ovviamente la speed == a max_speed rimarrà immodificata se dist/max_dist = 1

                mdf_speedx = (x_dist*xscale)/max_dist;
                mdf_speedz = (z_dist*zscale)/max_dist;
                if ( abs(mdf_speedx) <= t_speed ) x = xfinal;
                if ( abs(mdf_speedz) <= t_speed ) z = zfinal;
                if ( abs(mdf_speedx) > t_speed && abs(mdf_speedz) > t_speed ) {
                    xscale = mdf_speedx;
                    zscale = mdf_speedz;
                    if ( speed_flag == 1 ) speed_flag = 2;
                }
            }
        } // fine if speed_flag

        //setglobalvar("xscale",xscale);
        //setglobalvar("zscale",zscale);
        //return;

        // Scorri secondo il fattore di scalatura (diagonale/verticale/orizzontale)
        if ( x != xfinal ) x += xscale;
        if ( z != zfinal ) z += zscale;

        if ( xscale > 0 && x >= xfinal ) x = xfinal; // raggiunto meta. fermiamoci. -> DX
        else if ( xscale < 0 && x <= xfinal ) x = xfinal; // raggiunto meta. fermiamoci. -> SX
        if ( zscale > 0 && z >= zfinal ) z = zfinal; // raggiunto meta. fermiamoci. -> DOWN
        else if ( zscale < 0 && z <= zfinal ) z = zfinal; // raggiunto meta. fermiamoci. -> UP

        tx = threshold;
        tz = threshold;
        if ( xscale < 0 ) tx *= -1;
        if ( zscale < 0 ) tz *= -1;

        hole = checkhole(x+xscale,z+zscale);
        wall = checkwall(x+xscale,z+zscale);
        platform = find_platform_in_a(x+xscale,z+zscale,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
        if ( (hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base) || z < minz+zscale || z > maxz-zscale ) return 0;

        hole = checkhole(x+tx,z+tz);
        wall = checkwall(x+tx,z+tz);
        platform = find_platform_in_a(x+tx,z+tz,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
        if ( (hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base) || z < minz+tz || z > maxz-tz ) return 0;

        hole = checkhole(x,z);
        wall = checkwall(x,z);
        platform = find_platform_in_a(x,z,a,MAX_ALTITUDE);
        platform_base = get_platform_base(platform);
        if ( (hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base) || z < minz+threshold || z > maxz-threshold ) return 0;

        // Conserviamo l'ultima posizione sicura a partire da x,z
        setlocalvar(xmem,x);
        setlocalvar(zmem,z);
    }

    return 1;
}

int check_safe_pos(void self, float x_threshold, float z_threshold, int screen_bound_flag, int t_bound_custom) {
		int i,j,result = 0;
        float xpos = ov("xpos");
        float ypos = ov("ypos");
        float hres = ov("hresolution");
        float vres = ov("vresolution");
        float minz = ov("player_min_z");
        float maxz = ov("player_max_z");
        float x = getentityproperty(self, "x");
        float z = getentityproperty(self, "z");
        float a = getentityproperty(self, "y");
        float base = getentityproperty(self, "base");
        float t_bound = 20;

        if ( getlocalvar("orig_x") == NULL() ) setlocalvar("orig_x",x);
        if ( getlocalvar("orig_z") == NULL() ) setlocalvar("orig_z",z);
        if ( getlocalvar("orig_a") == NULL() ) setlocalvar("orig_a",a);

        if ( screen_bound_flag == NULL() || screen_bound_flag != 0 ) {
            if ( t_bound_custom != NULL() ) t_bound = t_bound_custom;

            if ( x <= xpos+t_bound ) {
                changeentityproperty(self,"position",xpos+t_bound+1,NULL(),NULL());
                x = getentityproperty(self, "x");
            } else if ( x >= xpos+hres-t_bound ) {
                changeentityproperty(self,"position",xpos+hres-t_bound-1,NULL(),NULL());
                x = getentityproperty(self, "x");
            }
            if ( z-base <= ypos+t_bound ) {
                changeentityproperty(self,"position",NULL(),ypos+t_bound+1,NULL());
                z = getentityproperty(self, "z");
            } else if ( z-base >= ypos+vres-t_bound ) {
                changeentityproperty(self,"position",NULL(),ypos+vres-t_bound-1,NULL());
                z = getentityproperty(self, "z");
            }
        }

        for (i = -1; i < 1+1; ++i) {
            for (j = -1; j < 1+1; ++j) {
                float wall;
                int hole;
                void platform;
                float platform_base = -1;

                if ( i != 0 && j != 0 ) continue; // checkiamo solo su 4 assi

                hole = checkhole(x+(i*x_threshold),z+(j*z_threshold));
                wall = checkwall(x+(i*x_threshold),z+(j*z_threshold));
                platform = find_platform_in_a(x+(i*x_threshold),z+(j*z_threshold),a,MAX_ALTITUDE);
                platform_base = get_platform_base(platform);
                if ( (hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base)
                    || z+(j*z_threshold) < minz || z+(j*z_threshold) > maxz ) {
                    float shiftx = -1*i*x_threshold;
                    float shiftz = -1*j*z_threshold;

                    hole = checkhole(x+shiftx,z+shiftz);
                    wall = checkwall(x+shiftx,z+shiftz);
                    platform = find_platform_in_a(x+shiftx,z+shiftz,a,MAX_ALTITUDE);
                    platform_base = get_platform_base(platform);
                    if ( (!hole || (hole && base > 0)) && wall <= a && (platform == NULL() || (platform != NULL() && platform_base == base))
                        && z+shiftz >= minz && z+shiftz <= maxz ) {

                        x = x+shiftx;
                        z = z+shiftz;

                        result = 1;
                        // Go to final control!!
                        i = 1;
                        j = 1;
                    } else continue;
                } else continue;

                // Check finale
                if ( i == 1 && j == 1 ) {
                    hole = checkhole(x,z);
                    wall = checkwall(x,z);
                    platform = find_platform_in_a(x,z,a,MAX_ALTITUDE);
                    platform_base = get_platform_base(platform);
                    if ( (hole && base <= 0) || wall > a || (platform != NULL() && platform_base != base)
                        || z < minz || z > maxz ) {
                        x = getlocalvar("orig_x");
                        z = getlocalvar("orig_z");
                        a = getlocalvar("orig_a");
                        result = 1;
                        break;
                    }
                } // fine if check finale
            } // fine for1
            if ( result == 1 ) break;
        } // fine for2

        setlocalvar("orig_x",NULL());
        setlocalvar("orig_z",NULL());
        setlocalvar("orig_a",NULL());

        changeentityproperty(self,"position",x,z,a);

        return result;
}

// Se c'è un ostacolo restituisce 1
int check_obstacles(void self, float x_threshold, float z_threshold, int screen_bound_flag, int t_bound_custom) {
		int i,j,result = 0;
        float xpos = ov("xpos");
        float ypos = ov("ypos");
        float hres = ov("hresolution");
        float vres = ov("vresolution");
        float minz = ov("player_min_z");
        float maxz = ov("player_max_z");
        float x = getentityproperty(self, "x");
        float z = getentityproperty(self, "z");
        float a = getentityproperty(self, "y");
        float base = getentityproperty(self, "base");
        float t_bound = 20;

        if ( screen_bound_flag == NULL() || screen_bound_flag != 0 ) {
            if ( t_bound_custom != NULL() ) t_bound = t_bound_custom;

            if ( x <= xpos+t_bound ) {
                result = 1;
            } else if ( x >= xpos+hres-t_bound ) {
                result = 1;
            }
            if ( z-base <= ypos+t_bound ) {
                result = 1;
            } else if ( z-base >= ypos+vres-t_bound ) {
                result = 1;
            }
        }

        for (i = -1; i < 1+1; ++i) {
            for (j = -1; j < 1+1; ++j) {
                float wall;
                int hole;
                void platform;
                float platform_base = -1;

                if ( result == 1 ) break;
                if ( i != 0 && j != 0 ) continue; // checkiamo solo su 4 assi

                hole = checkhole(x+(i*x_threshold),z+(j*z_threshold));
                wall = checkwall(x+(i*x_threshold),z+(j*z_threshold));
                platform = find_platform_in_a(x+(i*x_threshold),z+(j*z_threshold),a,MAX_ALTITUDE);
                platform_base = get_platform_base(platform);
                if ( (hole && base <= 0)
                    || (wall > a) || wall >= MAX_WALL_HEIGHT
                    || (platform != NULL() && platform_base != base)
                    || z+(j*z_threshold) < minz || z+(j*z_threshold) > maxz
                   ) {
                    //float shiftx = -1*i*x_threshold;
                    //float shiftz = -1*j*z_threshold;

					result = 1;
					break;
                } else continue;
            }
            if ( result == 1 ) break;
        }

        return result;
}

int check_obstacles_by_coords(void self, float x, float z, float a, float base, float x_threshold, float z_threshold, int screen_bound_flag, int t_bound_custom) {
		int i,j,result = 0;
        float xpos = ov("xpos");
        float ypos = ov("ypos");
        float hres = ov("hresolution");
        float vres = ov("vresolution");
        float minz = ov("player_min_z");
        float maxz = ov("player_max_z");
        float t_bound = 20;

        if ( screen_bound_flag == NULL() || screen_bound_flag != 0 ) {
            if ( t_bound_custom != NULL() ) t_bound = t_bound_custom;

            if ( x <= xpos+t_bound ) {
                result = 1;
            } else if ( x >= xpos+hres-t_bound ) {
                result = 1;
            }
            if ( z-base <= ypos+t_bound ) {
                result = 1;
            } else if ( z-base >= ypos+vres-t_bound ) {
                result = 1;
            }
        }

        for (i = -1; i < 1+1; ++i) {
            for (j = -1; j < 1+1; ++j) {
                float wall;
                int hole;
                void platform;
                float platform_base = -1;

                if ( result == 1 ) break;
                if ( i != 0 && j != 0 ) continue; // checkiamo solo su 4 assi

                hole = checkhole(x+(i*x_threshold),z+(j*z_threshold));
                wall = checkwall(x+(i*x_threshold),z+(j*z_threshold));
                platform = find_platform_in_a(x+(i*x_threshold),z+(j*z_threshold),a,MAX_ALTITUDE);
                platform_base = get_platform_base(platform);
                if ( (hole && base <= 0)
                    || (wall > a) || wall >= MAX_WALL_HEIGHT
                    || (platform != NULL() && platform_base != base)
                    || z+(j*z_threshold) < minz || z+(j*z_threshold) > maxz
                   ) {
                    //float shiftx = -1*i*x_threshold;
                    //float shiftz = -1*j*z_threshold;

					result = 1;
					break;
                } else continue;
            }
            if ( result == 1 ) break;
        }

        return result;
}

int check_obstacles_2D_level_in_dir(void self, float x_threshold) {
		int i,result = 0;
        float x = getentityproperty(self, "x");
        float z = getentityproperty(self, "z");
        float a = getentityproperty(self, "y");
        float base = getentityproperty(self, "base");
        int dir = getentityproperty(self,"direction");
        float wall;
        int hole;
        void platform;
        float platform_base = -1;

        if ( dir ) i = 1;
        else i = -1;

        hole = checkhole(x+(i*x_threshold),z);
        wall = checkwall(x+(i*x_threshold),z);
        platform = find_platform_in_a(x+(i*x_threshold),z,a,MAX_ALTITUDE);
        if ( platform != NULL() ) {
            platform_base = get_platform_base(platform);
            if ( platform_base > a ) return 1;
        } else if ( (hole && base <= 0) || wall > a ) {
            return 1;
        }

        return result;
}

int is_in_horizontal_bounds(float x, float z) {
    float xpos = ov("xpos");
    float hres = ov("hresolution");

    if ( x <= xpos || x >= xpos+hres ) return 1;

    return 0;
}

int is_flag_on(int flag_byte, int flag_value) {
    flag_byte = flag_byte & flag_value;

    if ( flag_byte == flag_value ) return 1;
    else return 0;
}

int get_flag_value(int flag_byte, int flag_value) {
    int flag;

    flag = exp(2,flag_value); // 1,2,4 etc...
    flag_byte = flag_byte & flag;

    return flag_byte;
}

int get_flag_from_value(int flag_value) {
    int flag;

    flag = exp(2,flag_value); // 1,2,4 etc...

    return flag;
}

int get_flag_position(int flag_value) { // flag_value == 2,4,8 etc...
    int flag = flag_value;
    int count = 0;

    while( flag >= 0 ) {
        flag /= 2;
        ++count;
    }

    return count;
}

// returns the new flag_byte
int add_flag(int flag_byte, int flag_value) {
    int flag;

    flag = exp(2,flag_value); // 1,2,4 etc...

    if ( !is_flag_on(flag_byte,flag) ) flag_byte += flag; // add new flag

    return flag_byte;
}

// returns the new flag_byte
int remove_flag(int flag_byte, int flag_value) {
    int flag;

    flag = exp(2,flag_value); // 1,2,4 etc...

    if ( is_flag_on(flag_byte,flag) ) flag_byte -= flag; // add new flag

    return flag_byte;
}

int exp(int base, int exp) {
    int i = 0;
    int result;
    int sign = 1;

    if ( base < 0 ) sign = -1;
    base = abs(base);
    exp = abs(exp);
    result = base;

    if ( exp == 0 ) return 1;
    for ( i = 0; i < exp-1; ++i ) {
        result *= base;
    }
    result *= sign;

    return result;
}

float sum(float base, int exp) {
    int i = 0;
    float result = 0;

    for ( i = 0; i < exp; ++i ) {
        result += base;
    }

    return result;
}

float sum_cyclic(float base, float value, float max) {
    int i = 0, sum_val = 1;

    if ( value < 0 ) sum_val = -1;
    value = abs(value);

    for ( i = 0; i < value; ++i ) {
        base += sum_val;
        if ( base > max ) base = 0;
        else if ( base < 0 ) base = max;
    }

    return base;
}

float sum_cyclic_sign(float base, float value, float min, float max) {
    int i = 0, sum_val = 1;

    if ( value < 0 ) sum_val = -1;
    value = abs(value);

    for ( i = 0; i < value; ++i ) {
        base += sum_val;
        if ( base > max ) base = min;
        else if ( base < min ) base = max;
    }

    return base;
}

/*
 * PARAMS:
 * t = time [0,1] (from 0 to 1);
 * n = type of bezier curve (ex. cubic = 3, quadratic = 2, linear = 1)
 * c# = coordinates (x OR z OR y) -> Ex.: x0,x1,x2,x3 | x0 is START, x3 is END, x1,x2 are intermediate.
 */
float bezier_curve(float t, int n, float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8, float c9) {
    float i = 0, b = 0;
    float coeff = 0, coord = 0;
    /// x = x0*(1-t)^3 + 3*x1*t*(1-t)^2 + 3*x2*t^2*(1-t) + x3*t^3
    /// x = x0*(1-t)^5 + 5*x1*t*(1-t)^4 + 10*x2*t^2*(1-t)^3 + 10*x3*t^3*(1-t)^2 + 5*x4*t^4*(1-t) + x5*t^5*(1-t)^0
    /*
    Binomial Coefficient: (n k) = n!/(k!*(n-k)!) --> Example: n!/(k!*(n-k)!) -> 5!/(2!*(5-2)! -> 120/2*6 -> 120/12 -> 10
    */

    for (i = 0; i <= n; ++i) {
        if      (i == 0) coord = c0;
        else if (i == 1) coord = c1;
        else if (i == 2) coord = c2;
        else if (i == 3) coord = c3;
        else if (i == 4) coord = c4;
        else if (i == 5) coord = c5;
        else if (i == 6) coord = c6;
        else if (i == 7) coord = c7;
        else if (i == 8) coord = c8;
        else if (i == 9) coord = c9;
        if ( coord == NULL() ) break;

        coeff = fact(n)/(fact(i)*fact(n-i));
        b += coeff*coord * exp(1-t,n-i) * exp(t,i);
    }

    return b;
}

int fact(int n) {
  int f = 1;

  n = abs(n);
  while ( n > 1) {
    f = f*n;
    n--;
  }

  return(f);
}

//to_transparent(self,1);
int to_transparent(void self, float speed, int reset_flag) {
    int time = openborvariant("elapsed_time");
    //float speed = 0.6;
    int max_transp = 256, min_transp = 0;
    float transp, threshold = 0.5;

    if ( reset_flag == NULL() ) reset_flag = 0;

    if ( getlocalvar("transp_start_time") == NULL() ) setlocalvar("transp_start_time",time);

    // Check Time-Reset
    if ( check_stored_time(getlocalvar("transp_start_time"),9999) ) setlocalvar("transp_start_time",openborconstant("MIN_INT"));
    get_next_time(max_transp);

    changedrawmethod(self, "enabled", 1);
    if ( reset_flag ) changedrawmethod(self, "reset", 1);
    //changedrawmethod(self, "flag", 1);

    transp = (time-getlocalvar("transp_start_time"))/speed;
    transp = max_transp - transp;
    if ( transp <= threshold ) transp = 0;
    else if ( transp >= max_transp ) transp = max_transp;

    changedrawmethod(self, "alpha", 6);
    changedrawmethod(self, "channelg", transp);
    changedrawmethod(self, "channelr", transp);
    changedrawmethod(self, "channelb", transp);


    if ( reset_flag ) {
        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);
    }

    if ( transp <= 0 ) {
        setlocalvar("transp_start_time",NULL());
        changedrawmethod(self, "enabled", 0);
        changedrawmethod(self, "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);

        return 1;
    }

    return 0;
}

//reduce_size_intime_to(self,256,40,1.7,0); -> reset 0 non resetta fino alla fine, 1 resetta sempre, 2 non resetta mai
int reduce_size_intime_to(void self, float size_from, float size_to, int speed, int reset_flag) {
    int time = openborvariant("elapsed_time");
    //void sprite = getentityproperty(self, "sprite");
    //int width = getgfxproperty(sprite, "srcwidth");
    //int height = getgfxproperty(sprite, "srcheight");
    //int scale_factor = 1;
    float threshold = 0.5;
    float size;

    if ( reset_flag == NULL() ) reset_flag = 0;

    if ( getlocalvar("reduce_size_start_time") == NULL() ) setlocalvar("reduce_size_start_time",time);

    // Check Time-Reset
    if ( check_stored_time(getlocalvar("reduce_size_start_time"),9999) ) setlocalvar("reduce_size_start_time",openborconstant("MIN_INT"));
    get_next_time(size_from);

    changedrawmethod(self, "enabled", 1);
    if ( reset_flag == 1 ) changedrawmethod(self, "reset", 1);
    //changedrawmethod(self, "flag", 1);

    size = (time-getlocalvar("reduce_size_start_time"))/speed;
    size = size_from - size;

    if ( size <= size_to+threshold ) size = size_to;

    //changedrawmethod(self, "centerx", width/2); // ancoraggio
    //changedrawmethod(self, "centery", height/2);

    changedrawmethod(self, "scalex", size);
    changedrawmethod(self, "scaley", size);

    if ( reset_flag == 1 ) {
        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);
    }

    if ( size <= size_to ) {
        setlocalvar("reduce_size_start_time",NULL());

        if ( reset_flag == 1 || reset_flag == 0 ) {
            changedrawmethod(self, "enabled", 0);
            changedrawmethod(self, "reset", 1);
            changedrawmethod(NULL(), "enabled", 0);
            changedrawmethod(NULL(), "reset", 1);
        }

        return 1;
    }

    return 0;
}

/*
 * PARAMS:
 * self = entity that performs a charge attack
 * key = hold button for charge attack: "attack" or "special" etc..
 * color1,2,3 = flashing colors. set it in RGB format like -> rgbcolor(0x43,0x00,0xDA)
 * max_charge_time = time in frames of whole charge time (from min to max charge)
 * atk_name1,2,3 = ATTACK NAME es. "ANI_FREESPECIAL1" or "ANI_FOLLOW1"
 * sound_fx = sound path+filename (OPTIONAL)
 * sound_duration = sound duration time (OPTIONAL)
 * flashing_ent = flashing entity (OPTIONAL)
 * flash_frame_length = frames number of flashing entity animation (OPTIONAL)
 * on_base_flag = if 1 you can perform a charge atk only on land else set it to 0 (OPTIONAL)
 * first_atk_flag = if 0 you avoid the first attack. so only last 2 attacks are valid (OPTIONAL)
 */
 // Ex. charge_attack(self,"attack",rgbcolor(0x00,0x26,0x88),rgbcolor(0x43,0x00,0xDA),rgbcolor(0x00,0xDA,0x0F),openborvariant("game_speed")*6,"ANI_FREESPECIAL17","ANI_FREESPECIAL21","ANI_FREESPECIAL13","data/sounds/frozen03.wav",openborvariant("game_speed")*0.2,"cflash",3,0,1);
int charge_attack(void self, char key, int color1, int color2, int color3, int max_charge_time, char atk_name1, char atk_name2, char atk_name3, char sound_fx, int sound_duration, char flashing_ent, int flash_frame_length, int on_base_flag, int first_atk_flag) {
    int time = openborvariant("elapsed_time");
    int p = getentityproperty(self,"playerindex");
    int CHARGE_ATK1 = openborconstant(atk_name1);
    int CHARGE_ATK2 = openborconstant(atk_name2);
    int CHARGE_ATK3 = openborconstant(atk_name3);

    if ( playerkeys(self,0,key) ) {
        if ( getlocalvar("charge_time") == NULL() ) { // unsetted time
            if ( time+max_charge_time >= openborconstant("MAX_INT") ) { // time protection
                changeopenborvariant("elapsed_time",0);
                time = openborvariant("elapsed_time");
            }
            setlocalvar("charge_time",time+max_charge_time);
        } else { // setted time
            int time_range = max_charge_time/3;
            int charge_time = time%max_charge_time;

            // FLASHING
            if ( time < getlocalvar("charge_time")-time_range*2 ) {
                flashing_color(self,color1,openborvariant("game_speed")/10,0);
                setlocalvar("charge_atk",0);
            } else if ( time < getlocalvar("charge_time")-time_range*1 ) {
                flashing_color(self,color2,openborvariant("game_speed")/15,0);
                setlocalvar("charge_atk",1);
            } else {
                flashing_color(self,color3,openborvariant("game_speed")/20,0);
                setlocalvar("charge_atk",2);
            }

            // SOUND
            if ( getlocalvar("charge_sound_time") == NULL() ) {
                if ( sound_fx == NULL() || sound_duration == NULL() ) return -1;
                if ( time+max_charge_time >= openborconstant("MAX_INT") ) { // time protection
                    changeopenborvariant("elapsed_time",0);
                    time = openborvariant("elapsed_time");
                }
                playsample(loadsample(sound_fx));
                setlocalvar("charge_sound_time",time+sound_duration);
            } else if ( time > getlocalvar("charge_sound_time") ) setlocalvar("charge_sound_time",NULL());

            // FLASH ENTITY
            if ( getlocalvar("charge_flash_ent") == NULL() ) {
                float x = getentityproperty(self,"x");
                float z = getentityproperty(self,"z");
                float y = getentityproperty(self,"y");
                float base = getentityproperty(self,"base");
                float dir = getentityproperty(self,"direction");
                float height = getentityproperty(self,"height");
                void subent;

                if ( flashing_ent == NULL() ) return -1;

                if ( height == NULL() ) height = 0;
                clearspawnentry();
                setspawnentry("name", flashing_ent);
                subent = spawn();
                changeentityproperty(subent, "no_adjust_base", 1);
                changeentityproperty(subent, "position", x, z, y+(height/2));
                changeentityproperty(subent, "base", y+(height/2));
                changeentityproperty(subent, "direction", dir);
                changeentityproperty(subent, "parent", self);
                setlocalvar("charge_flash_ent",subent);
            } else {
                void subent = getlocalvar("charge_flash_ent");

                if ( getentityproperty(subent,"exists") ) {
                    if ( getentityproperty(subent,"defaultmodel") == flashing_ent ) {
                        if ( flash_frame_length != NULL() ) {
                            if ( getentityproperty(subent,"animpos") >= flash_frame_length ) {
                                killentity(subent);
                                setlocalvar("charge_flash_ent",NULL());
                            }
                        }
                    } else setlocalvar("charge_flash_ent",NULL());
                } else setlocalvar("charge_flash_ent",NULL());
            }
        }
    } else {
        if ( getlocalvar("charge_time") != NULL() ) {
            float y = getentityproperty(self,"y");
            float base = getentityproperty(self,"base");

            if ( first_atk_flag == NULL() ) first_atk_flag = 0;

            // PERFORM AN ATTACK
            if ( getlocalvar("charge_atk") != NULL() ) {
                if ( on_base_flag != NULL() && on_base_flag > 0 && y > base ) return 0;

                if ( getlocalvar("charge_atk") == 0 ) {
                   if ( first_atk_flag ) performattack(self,CHARGE_ATK1,1);
                } else if ( getlocalvar("charge_atk") == 1 ) performattack(self,CHARGE_ATK2,1);
                else performattack(self,CHARGE_ATK3,1);
            }

            // RESET
            clear_flashing(self,0);
            setlocalvar("charge_sound_time",NULL());
            if ( getentityproperty(getlocalvar("charge_flash_ent"),"exists") && getentityproperty(getlocalvar("charge_flash_ent"),"defaultmodel") == flashing_ent ) killentity(getlocalvar("charge_flash_ent"));
            setlocalvar("charge_flash_ent",NULL());
            setlocalvar("charge_atk",NULL());
            setlocalvar("charge_time",NULL());
        }
    }
}

void flashing_color(void self, int colour, int frames, int reset_flag) {
    int time = openborvariant("elapsed_time");
    int flash_frames = frames;
    float time_range = flash_frames/2;
    int no_colour = rgbcolor(0x00,0x00,0x00);
    //int colour1 = rgbcolor(0xFF,0xFF,0x00);
    //int colour2 = rgbcolor(0xFF,0x00,0x00);

    if ( reset_flag == NULL() ) reset_flag = 0;

    time %= flash_frames;

    if ( time <= time_range ) {
        setlocalvar("next_flash_color",colour);
        setlocalvar("step_flash_color", 1);
    } else {
        setlocalvar("next_flash_color", no_colour);
        setlocalvar("step_flash_color", NULL());
    }

    changedrawmethod(self, "enabled", 1);
    if ( reset_flag ) changedrawmethod(self, "reset", 1);

    if ( getlocalvar("step_flash_color") == NULL() ) {
        changedrawmethod(self, "tintmode", 0);
        changedrawmethod(self, "tintcolor", 0);
        changedrawmethod(self, "fillcolor", 0);
    } else if ( getlocalvar("step_flash_color") == 1 ) {
        changedrawmethod(self, "tintmode", 6);
        changedrawmethod(self, "fillcolor", 0);
        changedrawmethod(self, "tintcolor", getlocalvar("next_flash_color"));
    }

    if ( reset_flag ) {
        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);
    }
}

//flashing3(self,rgbcolor(0xFF,0xFF,0x00),rgbcolor(0xFF,0x00,0x00),0);
void flashing3(void self, int colour1, int colour2, int reset_flag) {
    int time = openborvariant("elapsed_time");
    int flash_frames = 8;
    float time_range = truncA(flash_frames/3);
    int no_colour = rgbcolor(0x00,0x00,0x00);
    //int colour1 = rgbcolor(0xFF,0xFF,0x00);
    //int colour2 = rgbcolor(0xFF,0x00,0x00);

    if ( reset_flag == NULL() ) reset_flag = 0;

    time %= flash_frames;

    if ( time <= time_range ) {
        setlocalvar("next_flash_color",colour1);
        setlocalvar("step_flash_color", 1);
    } else if ( time > time_range && time <= time_range*2 ) {
        setlocalvar("next_flash_color", colour2);
        setlocalvar("step_flash_color", 2);
    } else {
        setlocalvar("next_flash_color", no_colour);
        setlocalvar("step_flash_color", NULL());
    }

    changedrawmethod(self, "enabled", 1);
    if ( reset_flag ) changedrawmethod(self, "reset", 1);

    if ( getlocalvar("step_flash_color") == NULL() ) {
        changedrawmethod(self, "tintmode", 0);
        changedrawmethod(self, "tintcolor", 0);
        changedrawmethod(self, "fillcolor", 0);
    } else if ( getlocalvar("step_flash_color") == 1 ) {
        changedrawmethod(self, "tintmode", 6);
        changedrawmethod(self, "fillcolor", 0);
        changedrawmethod(self, "tintcolor", getlocalvar("next_flash_color"));
    } else if ( getlocalvar("step_flash_color") == 2 ) {
        changedrawmethod(self, "tintmode", 1);
        changedrawmethod(self, "tintcolor", 0);
        changedrawmethod(self, "fillcolor", getlocalvar("next_flash_color"));
        // oppure tintmode == 4 + tintcolor
    }

    if ( reset_flag ) {
        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);
    }
}

void clear_flashing(void self, int reset_flag) {
    setlocalvar("set_flash_time",NULL());
    setlocalvar("next_flash_color",NULL());
    setlocalvar("step_flash_color",NULL());

    if ( reset_flag == NULL() ) reset_flag = 0;

    changedrawmethod(self, "tintmode", 0);
    changedrawmethod(self, "tintcolor", 0);
    changedrawmethod(self, "fillcolor", 0);

    if ( reset_flag ) {
        changedrawmethod(self, "enabled", 0);
        changedrawmethod(self, "reset", 1);
        changedrawmethod(NULL(), "enabled", 0);
        changedrawmethod(NULL(), "reset", 1);
    }
}

// in == 0, out == 1 ---> ex. fade(0.9,0);
int fade(float speed, int inout) {
    int time = openborvariant("elapsed_time");
	int vres = ov("vresolution");
	int hres = ov("hresolution");
    //float speed = 0.6;
    int max_transp = 254, min_transp = 0;
    float transp = 0, threshold = 0.1;
    int alpha = 6;

    if ( getlocalvar("fade_start_time") == NULL() ) setlocalvar("fade_start_time",time);

    // Check Time-Reset
    if ( check_stored_time(getlocalvar("fade_start_time"),9999) ) setlocalvar("fade_start_time",openborconstant("MIN_INT"));
    get_next_time(max_transp);

    changedrawmethod(NULL(), "enabled", 1);
    changedrawmethod(NULL(), "reset", 1);
    //changedrawmethod(self, "flag", 1);

    transp = (time-getlocalvar("fade_start_time"))/speed;
    if ( inout <= 0 ) transp = max_transp - transp;
    if ( transp <= threshold && inout <= 0 ) transp = 0;
    else if ( transp >= max_transp-threshold && inout > 0 ) transp = max_transp;

    //transp = 100;
    //changedrawmethod(NULL(), "alpha", alpha);
    changedrawmethod(NULL(), "channelg", transp);
    changedrawmethod(NULL(), "channelr", transp);
    changedrawmethod(NULL(), "channelb", transp);

    drawbox (0, 0, hres, vres, MAX_Z_LAYER, rgbcolor(0x00,0x00,0x00), alpha); //openborconstant("MAX_INT")

    changedrawmethod(NULL(), "enabled", 0);
    changedrawmethod(NULL(), "reset", 1);

    if ( transp <= min_transp && inout <= 0 ) {
        setlocalvar("fade_start_time",NULL());

        return 1;
    } else if ( transp >= max_transp && inout > 0 ) {
        setlocalvar("fade_start_time",NULL());

        return 1;
    }

    return 0;
}

// reset hitbyid opp only if isn't in range or if the entity change to another opp
int reset_opp_by_range(void self, void opp, int range_anim_id) {
    void last_opp = getentityvar(self,"last_opp");

    if ( getentityproperty(opp,"exists") && opp != last_opp ) {
        if ( getentityproperty(last_opp,"exists") ) changeentityproperty(last_opp,"hitbyid",0); // attackid
        setentityvar(self,"last_opp",opp);
    }

	if ( last_opp && getentityproperty(last_opp,"exists") ) {
		int range_flag = checkrange(self,last_opp,range_anim_id);
		//drawstring(10,170,0,":  "+range_flag );

		if ( !range_flag ) {
			changeentityproperty(last_opp,"hitbyid",0); // attackid
			setentityvar(self,"last_opp",NULL());
			last_opp == NULL();
		}
    }
}

int rotate(void self, float radius, float angle) {
	float x = getentityproperty(self, "x");
	//float z = getentityproperty(self, "z");
	float a = getentityproperty(self, "y");
	float base = getentityproperty(self, "base");
	float teta;
	float abs_angle = abs(angle);
	//int dir = 1; // 1 per senso orario, -1 antiorario
	//float radius = 50; // Raggio

	if ( getlocalvar("rotx"+self) == NULL() ) setlocalvar("rotx"+self, x);
	if ( getlocalvar("rota"+self) == NULL() ) setlocalvar("rota"+self, a);

	//if ( abs_angle >= 360 || angle <= -360 ) angle = 0;

	teta = angle;
	//teta %= 360;

	x = getlocalvar("rotx"+self) - radius*cos(teta);
	a = getlocalvar("rota"+self) - radius*sin(teta);

	changeentityproperty(self, "position", x, NULL(), a);
	changeentityproperty(self, "base", a);

	//drawstring(10, 160, 0, "x: "+x);
	//drawstring(10, 170, 0, "z: "+z);
    return teta;
}

int clear_rotate(void self) {
	setlocalvar("rotx"+self,NULL());
	setlocalvar("rota"+self,NULL());
}

int get_lost_frames(char label, int reset) {
		int lost_frames = 0;

		if ( label == NULL() ) label = "lf_start_time";

		if ( getlocalvar(label) == NULL() ) lost_frames = 0;
		else lost_frames = openborvariant("elapsed_time")-getlocalvar(label);

		if ( lost_frames <= 0 ) lost_frames = 0; // assicuriamoci che sia almeno 1
		setlocalvar(label,openborvariant("elapsed_time"));

		if( reset && lost_frames > 0 ) reset_lost_frames(NULL(),label);

		return lost_frames;
}

int reset_lost_frames(int frames, char label) {
    if ( label == NULL() ) label = "lf_start_time";
    if ( frames < 0 ) frames = NULL();
    setlocalvar(label, frames);
}

int autokill_keyc(int p, char key, char attack_type, int back_flag) {
    if ( ov("in_level") ) {
        if ( glvp("type") != 2 ) {
            void player = gpp(p,"entity");

            if ( gep(player,"exists") ) {
                if ( !gep(player,"aiflag","dead") && playerkeys(p,1,key) ) {
                    int dir = gep(player,"direction");
                    float xshift = 50;
                    void enemy;
                    char enemy_name = "fake_enemy"; // "fake_enemy"

                    if ( back_flag == NULL() ) back_flag = 0;
                    if ( back_flag ) { // back kill
                        if ( dir ) xshift *= -1;
                    } else {
                        if ( !dir ) xshift *= -1;
                    }

                    loadmodel(enemy_name);
                    enemy = spawnsubentity_relative(enemy_name,xshift,0,0,player);
                    cep(enemy, "lifespancountdown", ov("game_speed")*1.0);
                    damageentity(player,enemy,gep(player,"health"),1,oc(attack_type));
                }
            }
        }
    }
}

int destroy_all_enemies(int p, int kill_flag) {
    if ( openborvariant("in_level") ) {
        if ( getlevelproperty("type") != 2 ) {
            //int p = getentityproperty(player,"playerindex");
            void player = getplayerproperty(p,"entity");

            if ( !is_in_pain(player) && getentityproperty(player,"aiflag","attacking") && (playerkeys(p,0,"attack") || playerkeys(p,0,"special")) ) { // getentityproperty(player,"attacking") == 1
                int i;

                for ( i = 0; i < openborvariant("count_entities"); ++i ) { // openborconstant("MAX_ENTS")
                    void ent = getentity(i);

                    if ( !getentityproperty(ent,"exists") ) continue;

                    if ( getentityproperty(ent,"type") == openborconstant("TYPE_ENEMY") ) {
                        int health = getentityproperty(ent,"health");
                        int anim_id = getentityproperty(ent,"animationid");

                        if ( getentityvar(ent,"weapon") > 0 && getentityvar(ent,"weapon") != NULL() ) continue;
                        if ( kill_flag != NULL() && kill_flag ) {
                            killentity(ent);
                        } else {
                            if ( getentityproperty(ent,"aiflag","dead") || health <= 0 ) continue;
                            if ( anim_id == openborconstant("ANI_SPAWN") || anim_id == openborconstant("ANI_RESPAWN")  ) continue;
                            // Con "vulnerable" evitiamo anche di uccidere end_level_anim
                            if ( getentityproperty(ent,"vulnerable") ) {
                                float height;
                                damageentity(ent,player,health,1,openborconstant("ATK_NORMAL"));

                                height = getentityproperty(ent,"height");
                                if (height == NULL()) height = 40;
                                void flash = spawnsubentity("cflash",getentityproperty(ent,"x"),getentityproperty(ent,"z"),getentityproperty(ent,"y")+height);
                                changeentityproperty(flash,"no_adjust_base",1);
                                changeentityproperty(flash,"base",getentityproperty(ent,"y")+height);

                                /*if ( !getentityproperty(opp,"aiflag","blocking") ) {
                                    if ( !gep(opp,"flash","noattack") ) show_flash();
                                    playsample(loadsample("data/sounds/punch5.wav"));
                                }*/
                              /*if ( getentityproperty(opp,"aiflag","blocking") ) { // && oa <= obase
                                if ( getentityproperty(opp,"flash","block") ) show_block_flash(opp);
                                changeentityproperty(opp,"aiflag","blocking",1);
                                changeentityproperty(opp,"takeaction","common_block",1);
                                opp = NULL();
                              }*/
                            }
                        }
                    }
                } // fine for
            } // fine if attacking
        }
    } // fine if level
}

int show_enemies() {
    if ( openborvariant("in_level") ) {
        if ( getlevelproperty("type") != 2 ) {
            int i, y = 50;

            drawstring( 10,y,0,"enemies: "+openborvariant("count_enemies"));

            for ( i = 0; i < openborvariant("count_entities"); ++i ) { // openborconstant("MAX_ENTS")
                void ent = getentity(i);

                if ( !getentityproperty(ent,"exists") ) continue;
                if ( getentityproperty(ent,"type") == openborconstant("TYPE_ENEMY") ) {
                    y += 10;
                    drawstring( 10,y,0," "+getentityproperty(ent,"model"));
                }
            }
        }
    }
}

int music_fade(int vol_dec) {

    if ( getlocalvar("music_fade") == NULL() ) {
        if ( getlocalvar("music_vol") == NULL() ) {
            get_lost_frames(); // reset lost frames
            setlocalvar("music_vol",openborvariant("musicvol"));
        }
        vol_dec *= get_lost_frames();

        setlocalvar("music_vol",getlocalvar("music_vol")-vol_dec);
        if ( getlocalvar("music_vol") < 0 ) setlocalvar("music_vol",0);

        setmusicvolume(getlocalvar("music_vol"),getlocalvar("music_vol"));
        if ( getlocalvar("music_vol") <= 0 ) setlocalvar("music_fade",1);
    } else {
        pausemusic();
    }
}

int reset_music_fade() {
    setlocalvar("music_fade",NULL());
    setlocalvar("music_vol",NULL());
    setmusicvolume(openborvariant("musicvol"),openborvariant("musicvol"));
}

void show_flash(char flash_name, void opp, int map, char sndfx) {
	void flash;

    if ( flash_name == NULL() ) flash_name = "cflash";

    // noatflash 1
    if ( getentityproperty(opp,"exists") && opp != NULL() ) {
        if ( getentityproperty(opp,"flash","noattack") ) return;
    }

	flash = spawnsubentity(flash_name,openborvariant("lasthitx"),openborvariant("lasthitz")+1,openborvariant("lasthita"));
	if ( map != NULL() ) changeentityproperty(flash,"map",map);
	if ( sndfx != NULL() ) playsample(loadsample(sndfx));

	if ( getentityproperty(opp,"exists") && opp != NULL() ) {
        int odir = getentityproperty(opp,"direction");

        if ( odir ) changeentityproperty(flash,"direction",1);
        else changeentityproperty(flash,"direction",0);
	}
}

void show_block_flash(char flash_name, void opp, int map, char sndfx) {
	void flash;

	if ( flash_name == NULL() ) flash_name = "dflash";

	//if ( getentityproperty(opp,"flash","block") <= 0 ) return;

	flash = spawnsubentity("dflash",openborvariant("lasthitx"),openborvariant("lasthitz")+1,openborvariant("lasthita"));
	if ( map != NULL() ) changeentityproperty(flash,"map",map);
	if ( sndfx != NULL() ) playsample(loadsample(sndfx));

	if ( getentityproperty(opp,"exists") && opp != NULL() ) {
        int odir = getentityproperty(opp,"direction");

        if ( odir ) changeentityproperty(flash,"direction",1);
        else changeentityproperty(flash,"direction",0);
	}
}

int is_in_pain(void self, int rise_flag) {
    if ( getentityproperty(self,"aiflag","inpain") || getentityproperty(self,"aiflag","falling") || getentityproperty(self,"aiflag","dead") ) {
        return 1;
    }

    if ( rise_flag == NULL() || rise_flag == 0 ) {
        if ( getentityproperty(self,"staydown","rise") || getentityproperty(self,"staydown","riseattack") ) { // getentityproperty(self,"staydown","riseattack_stall")
                return 1;
        }
    }

    return 0;
}

int is_on_base(void self) {
    float a = getentityproperty(self,"y");
    float base = getentityproperty(self,"base");
    float tossv = getentityproperty(self,"tossv");

    if ( a <= base && tossv == 0 ) return 1;
    else return 0;
}

int is_stopped(void self) {
    float xdir = getentityproperty(self,"xdir");
    float zdir = getentityproperty(self,"zdir");
    float tossv = getentityproperty(self,"tossv");

    if ( xdir == 0 && zdir == 0 && tossv == 0 ) return 1;
    else return 0;
}

int is_walking(void self, int max_anim, int run_flag) {
    int anim_id = getentityproperty(self,"animationid");
    int i = 0;

    if ( max_anim == NULL() ) max_anim = 10;
    if ( anim_id == openborconstant("ANI_WALK") || anim_id == openborconstant("ANI_UP") || anim_id == openborconstant("ANI_DOWN") || (anim_id == openborconstant("ANI_RUN") && run_flag > 0) ) return 1;
    for ( i = 2; i <= max_anim; ++i ) {
        if ( anim_id == openborconstant("ANI_WALK"+i) || anim_id == openborconstant("ANI_UP"+i) && anim_id == openborconstant("ANI_DOWN"+i) ) return 1;
    }

    if ( run_flag > 0 && getentityproperty(self,"aiflag","running") ) return 1;
    if ( getentityproperty(self,"aiflag","walking") ) return 1;

    return 0;
}

int is_moving(void self) {
    if ( gep(self,"xdir") != 0 || gep(self,"zdir") != 0 || gep(self,"tossv") != 0 ) return 1;
    else return 0;
}

int is_in_idle(void self, int max_anim) {
    /*int anim_id = getentityproperty(self,"animationid");
    int i = 0;

    if ( max_anim == NULL() ) max_anim = 10;
    if ( anim_id == openborconstant("ANI_IDLE") ) return 1;
    for ( i = 2; i <= max_anim; ++i ) {
        if ( anim_id == openborconstant("ANI_IDLE"+i) ) return 1;
    }*/

    if ( getentityproperty(self,"aiflag","idling") == 3 ) return 1;

    return 0;
}

int is_in_idle_anim(void self, int max_anim) {
    int anim_id = getentityproperty(self,"animationid");
    int i = 0;

    if ( max_anim == NULL() ) max_anim = 10;
    if ( anim_id == openborconstant("ANI_IDLE") ) return 1;
    for ( i = 2; i <= max_anim; ++i ) {
        if ( anim_id == openborconstant("ANI_IDLE"+i) ) return 1;
    }

    if ( getentityproperty(self,"aiflag","idling") == 3 ) return 1;

    return 0;
}

int is_attacking(void self) {
    if ( getentityproperty(self,"aiflag","attacking") ) return 1;
    else if ( getentityproperty(self,"attacking") ) return 1;

    return 0;
}

int is_running(void self) {
    if ( getentityproperty(self,"aiflag","running") ) return 1;

    return 0;
}

int is_name(void self, char model, char name) {
    char model_name = getentityproperty(self, model);

    if ( model_name == name ) return 1;
    else return 0;
}

int is_for(void entity, void orig_owner) {
	void parent = getentityproperty(entity,"parent");
	void owner = getentityproperty(entity,"owner");
  	void subentity = getentityproperty(entity, "subentity");

  	if ( parent == NULL() ) parent = owner;
  	if ( parent == NULL() ) parent = subentity;

  	if ( orig_owner == parent && orig_owner != NULL() ) return 1;
    else return 0;
}

void grabbed_by(void self) {
    void opp = getentityproperty(self,"opponent");

    if ( getentityproperty(opp,"exists") ) {
        void grabbed_opp = getentityproperty(opp,"grabbing");

        if ( grabbed_opp == self ) return opp;
        else return NULL();
    }

    return NULL();
}

int is_grabbing(void self) {
    if ( getentityproperty(self,"animationid") == openborconstant("ANI_GRAB") ) return 1;
    else return 0;
}

int is_grabbed(void self) {
    if ( getentityproperty(self,"animationid") == openborconstant("ANI_GRABBED") ) return 1;
    else return 0;
}

 // threshold to restrict the space
int is_incam(void ent, float threshold) {
    if ( getentityproperty(ent,"exists") && openborvariant("in_level") ) {
        float scr_posx = openborvariant("xpos");
        float scr_posy = openborvariant("ypos");
        int hres = openborvariant("hresolution");
        int vres = openborvariant("vresolution");
        float x = getentityproperty(ent,"x");
        float z = getentityproperty(ent,"z");
        float a = getentityproperty(ent,"a");
        float base = getentityproperty(ent,"base");
        float scrollminx = openborvariant("scrollminx");
        float scrollmaxx = openborvariant("scrollmaxx");
        //float scrollminz = openborvariant("scrollminz");
        //float scrollmaxz = openborvariant("scrollmaxz");
        float minz = openborvariant("player_min_z");
        float maxz = openborvariant("player_max_z");
        //float camx = getlevelproperty("cameraxoffset");
        //float camz = getlevelproperty("camerazoffset");

        if ( threshold == NULL() ) threshold = 0;

        if ( x >= scr_posx+threshold && x <= scr_posx+hres-threshold && z-base >= scr_posy+threshold && z-base <= scr_posy+vres-4 ) {
            if ( x >= scrollminx && x <= scrollmaxx+hres && z >= minz-2.0 && z <= maxz+2.0 ) {
                return 1;
            }
        }
    }

    return 0;
}

int is_incam_coords(float x, float z, float a, float base, float threshold) {
    if ( openborvariant("in_level") ) {
        float scr_posx = openborvariant("xpos");
        float scr_posy = openborvariant("ypos");
        int hres = openborvariant("hresolution");
        int vres = openborvariant("vresolution");
        float scrollminx = openborvariant("scrollminx");
        float scrollmaxx = openborvariant("scrollmaxx");
        //float scrollminz = openborvariant("scrollminz");
        //float scrollmaxz = openborvariant("scrollmaxz");
        float minz = openborvariant("player_min_z");
        float maxz = openborvariant("player_max_z");
        //float camx = getlevelproperty("cameraxoffset");
        //float camz = getlevelproperty("camerazoffset");

        if ( threshold == NULL() ) threshold = 0;

        if ( (x == NULL() || z == NULL() || a == NULL() || base == NULL()) ) return 0;

        if ( x >= scr_posx+threshold && x <= scr_posx+hres-threshold && z-base >= scr_posy+threshold && z-base <= scr_posy+vres-4 ) {
            if ( x >= scrollminx && x <= scrollmaxx+hres && z >= minz-2.0 && z <= maxz+2.0 ) {
                return 1;
            }
        }
    }

    return 0;
}

float get_distance(float x1, float x2) {
    if ( x1 <= x2 ) return (x2-x1);
    else return (x1-x2);
}

int change_walking_anim_noaicontrol(void self) {
    float speed = getentityproperty(self, "speed");
    float xdir = getentityproperty(self, "xdir");
    float zdir = getentityproperty(self, "zdir");
    float tossv = getentityproperty(self, "tossv");
    int anim_id = getentityproperty(self, "animationid");
    float t_zdir = (speed/2-speed/4);

    if ( !getentityproperty(self,"aiflag","attacking") && !getentityproperty(self,"aiflag","inpain") && !getentityproperty(self,"aiflag","falling") && !getentityproperty(self,"aiflag","dead") ) {
        if ( tossv == 0 ) {
            if ( xdir != 0 && abs(zdir) < t_zdir  ) {
                if ( getentityproperty(self,"animvalid",openborconstant("ANI_WALK")) && anim_id != openborconstant("ANI_WALK") ) changeentityproperty(self,"animation",openborconstant("ANI_WALK"));
            }
            if ( zdir < -1*t_zdir ) {
                if ( getentityproperty(self,"animvalid",openborconstant("ANI_UP")) && anim_id != openborconstant("ANI_UP") ) changeentityproperty(self,"animation",openborconstant("ANI_UP"));
            } else if ( zdir > t_zdir ) {
                if ( getentityproperty(self,"animvalid",openborconstant("ANI_DOWN")) && anim_id != openborconstant("ANI_DOWN") ) changeentityproperty(self,"animation",openborconstant("ANI_DOWN"));
            }

            if ( xdir < 0 && getentityproperty(self,"direction") ) changeentityproperty(self,"direction",0);
            else if ( xdir > 0 && !getentityproperty(self,"direction") ) changeentityproperty(self,"direction",1);

            if ( xdir == 0 && zdir == 0 && tossv == 0 && anim_id != openborconstant("ANI_IDLE") ) setidle(self,openborconstant("ANI_IDLE"),1);
        }
    }
}

// se si supera TIME+FRAMES la funzione restituisce vero, altrimenti falso
int time_to_wait(void label, int frames) {
    int time = openborvariant("elapsed_time");

    if ( getlocalvar(label) == NULL() ) {
        setlocalvar(label,get_next_time(frames));
        //return 0;
    }

    if ( getlocalvar(label) != NULL() && time >= getlocalvar(label) ) {
        return 1;
    }

    return 0;
}

int reset_time_to_wait(void label) {
    setlocalvar(label,NULL());
}

//AB = SQRT[(x2-x1)^2 + (y2-y1)^2]
float distAB(float x1, float y1, float x2, float y2) {
    if ( x1 == NULL() || y1 == NULL() || x2 == NULL() || y2 == NULL() ) return -1;

    if ( x1 < 0 ) {
        x1 += abs(x1);
        y1 += abs(x1);
        x2 += abs(x1);
        y2 += abs(x1);
    }
    if ( y1 < 0 ) {
        x1 += abs(y1);
        y1 += abs(y1);
        x2 += abs(y1);
        y2 += abs(y1);
    }
    if ( x2 < 0 ) {
        x1 += abs(x2);
        y1 += abs(x2);
        x2 += abs(x2);
        y2 += abs(x2);
    }
    if ( y2 < 0 ) {
        x1 += abs(y2);
        y1 += abs(y2);
        x2 += abs(y2);
        y2 += abs(y2);
    }

    if ( x1 >= 0 && y1 >= 0 && x2 >= 0 && y2 >= 0 ) return sqrt(exp(diff(x2,x1),2)+exp(diff(y2,y1),2));
    else return -1;
}

int update_coords(void self) {
    float x = getentityproperty(self,"x");
    float z = getentityproperty(self,"z");
    float y = getentityproperty(self,"y");
    float base = getentityproperty(self,"base");
    int dir = getentityproperty(self,"direction");

    setentityvar(self,"x",x);
    setentityvar(self,"z",z);
    setentityvar(self,"y",y);
    setentityvar(self,"base",base);
    setentityvar(self,"direction",dir);
}

int count_enemies() {
    int count = 0;

    if ( openborvariant("in_level") && getlevelproperty("type") != 2 ) {
        int i;

        for ( i = 0; i < openborvariant("count_entities"); ++i ) { // openborconstant("MAX_ENTS")
            void ent = getentity(i);

            if ( !getentityproperty(ent,"exists") ) continue;
            if ( getentityproperty(ent,"type") == openborconstant("TYPE_ENEMY") ) {
                ++count;
            }
        }
    }

    return count;
}

int is_waiting(float ETA, int loop_flag) {
    float xpos = ov("xpos");
    float ypos = ov("ypos");
    float standard_ETA = 2;
    int count_enemies = count_enemies();

    //drawstring( 10,200,0,"time:  "+ov("elapsed_time"));
    //drawstring( 10,210,0,"ETA:  "+glv("is_waiting_time"));

    if ( ETA == NULL() ) ETA = standard_ETA;
    if ( count_enemies <= 0 ) {
        int time = ov("elapsed_time");
        int wait_time = ov("game_speed")*ETA;

        if ( glv("is_waiting_time") == NULL() ) {
            slv("prev_xpos",xpos);
            slv("prev_ypos",ypos);
            slv("prev_count_enemies",count_enemies);

            if ( time+ETA >= oc("MAX_INT") ) {
                cv("elapsed_time",0);
                time = ov("elapsed_time");
            }
            slv("is_waiting_time",time+wait_time);
        } else if ( time >= glv("is_waiting_time") && glv("is_waiting_time") != NULL() ) {
            if ( xpos == glv("prev_xpos") && ypos == glv("prev_ypos") ) {
                if ( loop_flag == NULL() || loop_flag == 0 ) {
                    slv("prev_xpos",NULL());
                    slv("prev_ypos",NULL());
                    slv("prev_count_enemies",NULL());
                    slv("is_waiting_time",NULL());
                }

                return 1;
            } else {
                slv("prev_xpos",NULL());
                slv("prev_ypos",NULL());
                slv("prev_count_enemies",NULL());
                slv("is_waiting_time",NULL());
            }
        }
    } else {
        if ( glv("prev_count_enemies") != NULL() ) {
            slv("prev_xpos",NULL());
            slv("prev_ypos",NULL());
            slv("prev_count_enemies",NULL());
            slv("is_waiting_time",NULL());
        }
    }

    return 0;
}

int jump(void player) {
    performattack(player, openborconstant("ANI_JUMP"), 1);
    changeentityproperty(player, "aiflag", "jumping", 1);
    changeentityproperty(player, "takeaction", "common_jump", 1);
    playsample(loadsample("data/sounds/jump.wav"));
    tossentity(player, getentityproperty(player, "jumpheight"),0,0);
}

int is_jumping(void player) {
    if ( getentityproperty(player,"animationid") == openborconstant("ANI_JUMP") && getentityproperty(player,"aiflag","jumping") > 0 ) return 1;
    else return 0;
}

int is_animation(void player, char animid) {
    if ( getentityproperty(player,"animationid") == openborconstant(animid) ) return 1;
    else return 0;
}

// prendi un punteggio dal target e dallo al self
int give_score(void self, void target) {

    if ( getentityproperty(target,"exists") ) {
        int ohealth = getentityproperty(target,"health");
        int ttype = getentityproperty(target,"type");

        if ( ttype == openborconstant("TYPE_PLAYER") ) return 0;

        if ( ohealth <= 0 && getentityproperty(self,"type") == openborconstant("TYPE_PLAYER") ) {
            int p = getentityproperty(self,"playerindex");
            int player_score = getplayerproperty(p,"score");
            int score_aquired = getentityproperty(target,"score");

            changeplayerproperty(p,"score",player_score+score_aquired);

            return (player_score+score_aquired);
        }
    }

    return 0;
}

void add_hitbyid_list(void self, void entityvar, void opp) {
    void opp_list = getentityvar(self,entityvar);

    if ( opp != NULL() && getentityproperty(opp,"exists") ) {
        if ( opp_list == NULL() ) {
            setentityvar(self,entityvar,array(0));
            opp_list = getentityvar(self,entityvar);
            set(opp_list,0,opp);
        } else {
            int list_size = size(opp_list), i;

            // cerca slot vuoto. non trovi? aggiungi alla fine
            for (i = 0; i < list_size; ++i) {
                void old_opp = get(opp_list,i);

                if ( old_opp == NULL() ) {
                    set(opp_list,i,opp);
                    return opp_list;
                } else if ( old_opp == opp ) {
                    return opp_list;
                }
            }
            set(opp_list,size(opp_list),opp); // no empty slots? set at the end
        }

        //setentityvar(self,entityvar,opp_list);
    }

    return opp_list;
}

void clear_hitbyid_list(void self, void entityvar, int freeflag, int no_delete_flag) {
    int i = 0;
    void opp_list = getentityvar(self,entityvar);

    if ( opp_list != NULL() ) {
        int list_size = size(opp_list)-1;
        //drawstring(10,100,0,opp_list);
        for ( i = list_size; i >= 0; i-- ) {
            void opp = get(opp_list,i);

            //if ( gep(self,"defaultmodel")=="LEO" ) drawstring(10,50+(i)*10,0,"opp("+(i)+"): "+gep(opp,"name")+" id: "+gep(opp,"hitbyid"));

            if ( no_delete_flag == NULL() ) no_delete_flag = 0;

            if ( opp != NULL() && getentityproperty(opp,"exists") ) {
                changeentityproperty(opp,"hitbyid",0);
                if ( !no_delete_flag ) set(opp_list,i,NULL());
            } else set(opp_list,i,NULL());
        }

        if ( freeflag == NULL() || freeflag == 0 ) {
            free(opp_list);
            setentityvar(self,entityvar,NULL());
        }

        opp_list = getentityvar(self,entityvar);
    }

    return opp_list;
}

void destroy_hitbyid_list(char localvar_name) {
    int i = 0;
    void opp_list = getlocalvar(localvar_name);

    if ( opp_list != NULL() ) {
        int list_size = size(opp_list)-1;

        for ( i = list_size; i >= 0; i-- ) {
            set(opp_list,i,NULL());
        }
        free(opp_list);
        setlocalvar(localvar_name,NULL());
        opp_list = getlocalvar(localvar_name);
    }

    return opp_list;
}

float mod(float base, int op) {
    float mantix = base - base%(base+1);

    if ( mantix < 0 ) mantix = 0;
    base = base%op + mantix;

    return base;
}

float mod_sign(float base, int op) {
    float sign = 1;
    float mantix;

    if ( base < 0 ) sign = -1;
    base = abs(base);
    op = abs(op);
    mantix = base - base%(base+1);

    if ( mantix < 0 ) mantix = 0;
    base = base%op + mantix;

    base *= sign;

    return base;
}

int load_model(char modelname) {
    int models_cached = openborvariant("models_cached");
    int i = 0;

    for ( i = 0; i < models_cached; ++i ) {
        char model = getmodelproperty(i,2); // name

        //drawstring(100,0,0,"cached: "+models_cached);
        //drawstring(10,10*i,0,""+getmodelproperty(i,2));
        if ( model == modelname ) {
            loadmodel(modelname);
            //changemodelproperty(i,4,1); // selectable
            return;
        }
    }
}

void finish_level() {
    void end_level, player;
    int i;
    char current_branch = openborvariant("current_branch");
    char current_level = openborvariant("current_level");
    char current_scene = openborvariant("current_scene");
    char current_set = openborvariant("current_set");
    int current_stage = openborvariant("current_stage");

    if ( getlocalvar("end_level")+current_stage+current_set+current_level == NULL() ) {
        loadmodel("endlevel_empty");
        end_level = spawnsubentity("endlevel_empty",-50,-50,0);
        setlocalvar("endlevel"+current_stage+current_set+current_level,end_level);
    }

    for (i = 0; i < MAX_PLAYERS; ++i) {
        player = gpp(i, "ent");

        if ( !gep(player, "exists") ) continue;
        else break;
    }

    bindentity(end_level,player,0,0,0,0);
    if ( gep(player, "noaicontrol") == 1 ) cep(player, "noaicontrol", 0);
}

int offscreenkill(int pixels, void entity, int nokill_flag) {
	float xpos = openborvariant("xpos");
	float ypos = openborvariant("ypos");
	int vres = openborvariant("vresolution");
	int hres = openborvariant("hresolution");
	float x, z, y;

    if ( entity == NULL() ) entity = getlocalvar("self");
    if ( entity == NULL() ) return 0;

	x = getentityproperty(entity,"x");
	z = getentityproperty(entity,"z");
	y = getentityproperty(entity,"y");
	if ( x >= xpos+hres+pixels || x <= xpos-pixels || z-y >= ypos+vres+pixels || z-y <= ypos-pixels ) {
        if ( nokill_flag == 0 || nokill_flag == NULL() ) {
            killentity(entity);
            return 1;
        }
	}

	return 0;
}

int kill_players(int flag) {
    int p = 0;

    for ( p = 0; p < openborvariant("maxplayers"); ++p ) {
        void player = getplayerproperty(p, "entity");

        changeplayerproperty(p, "lives", 0);
        changeplayerproperty(p, "credits", 0);
        changeplayerproperty(p, "name", "");
        if ( getentityproperty(player,"exists") ) {
            killentity(player);
        }
    }

    if ( flag == NULL() || flag == 0 ) changeplayerproperty(0,"lives",1); // to avoid game over
}

int freeze(void self, int eta) {
    changeentityproperty(self,"freezetime",get_next_time(eta));
    changeentityproperty(self,"frozen",1);
}

int ratio(float value, float ratio) {
    return ( (value*ratio)/100 );
}

int ratio_value(float value, float max_value) {
    return ( (value*100)/max_value );
}

int inc_gv(char label, float value, float start_from_value) {
    if ( value == NULL() ) value = 1;

    if ( ggv(label) == NULL() && start_from_value == NULL() ) sgv(label,value);
    else if ( ggv(label) == NULL() ) sgv(label,start_from_value);

    sgv(label,ggv(label)+value);
}

int inc_lv(char label, float value, float start_from_value) {
    if ( value == NULL() ) value = 1;

    if ( glv(label) == NULL() && start_from_value == NULL() ) slv(label,value);
    else if ( glv(label) == NULL() ) slv(label,start_from_value);

    slv(label,glv(label)+value);
}

int inc_ev(void ent, char label, float value, float start_from_value) {
    if ( value == NULL() ) value = 1;

    if ( gev(ent,label) == NULL() && start_from_value == NULL() ) sev(ent,label,value);
    else if ( gev(ent,label) == NULL() ) sev(ent,label,start_from_value);

    sev(ent,label,gev(ent,label)+value);
}

int draw_time_countdown(int ETA, float x, float y, int font, int z, int sortid, int zero_count_flag, int zero_flag, int reset) {
    int i = 0;
    int time = openborvariant("elapsed_time");
    int game_speed = openborvariant("game_speed");
    int countdown_time = getlocalvar("countdown_time");
    int num = 0;
    char num_str = "";

    if ( reset > 0 ) {
        setlocalvar("countdown_time",NULL());
        setlocalvar("countdown_digits",NULL());
        return -1;
    }

    if ( countdown_time != NULL() ) {
        if ( countdown_time >= time ) {
            if ( z == NULL() ) z = 5000;
            if ( sortid == NULL() ) sortid = 0;

            num = ((countdown_time-time)/game_speed)%ETA;
            if ( num == NULL() || num < 0 ) num = 0;

            num_str = num;
            if ( zero_flag > 0 ) {
                for (i = 0; i < getlocalvar("countdown_digits")-digit_count(num); ++i ) num_str = "0"+num_str;
            }
            drawstring(x,y,font,num_str,z,sortid);
        } else {
            if ( zero_count_flag > 0 ) {
                num = 0;

                num_str = num;
                if ( zero_flag > 0 ) {
                    for (i = 0; i < getlocalvar("countdown_digits")-digit_count(num); ++i ) num_str = "0"+num_str;
                }
                drawstring(x,y,font,num_str,z,sortid);
            }
        }
    } else {
        setlocalvar("countdown_time",get_next_time(ETA*game_speed));
        setlocalvar("countdown_digits",digit_count(ETA));
        num = ETA;
        if ( num == NULL() || num < 0 ) num = 0;
    }

    return num;
}

int get_time_countdown(int ETA, int reset) {
    int i = 0;
    int time = openborvariant("elapsed_time");
    int game_speed = openborvariant("game_speed");
    int countdown_time = getlocalvar("countdown_time");
    int num = 0;

    if ( reset > 0 ) {
        setlocalvar("countdown_time",NULL());
        return -1;
    }

    if ( countdown_time != NULL() ) {
        if ( countdown_time >= time ) {
            num = ((countdown_time-time)/game_speed)%ETA;
        } else num = 0;
    } else {
        setlocalvar("countdown_time",get_next_time(ETA*game_speed));
        num = ETA;
    }

    if ( num == NULL() || num < 0 ) num = 0;

    return num;
}

int digit_count(int num) {
    int i = 0, count = 0;

    if ( num == NULL() ) return 0;

    if ( num < 10 && num >= 0 ) return 1;
    else if ( num < 0 && num > -10 ) return 2;

    if ( num < 0 ) ++count;

    for (i = 0; num >= 1; ++i) {
        num = truncA(num/10);
        ++count;
    }

    return count;
}

void set_to_idle(void player, int anim_flag, int control_flag) {
    changeplayerproperty(player, "weapon",0);
    changeentityproperty(player, "antigravity", 0);

    changeentityproperty(player, "subject_to_screen", 0);
    changeentityproperty(player, "velocity", 0,0,0);

    if ( anim_flag > 0 ) {
        changeentityproperty(player, "position",NULL(),NULL(),getentityproperty(player,"base"));
        if ( getentityproperty(player,"animationid") != openborconstant("ANI_IDLE") && !getentityproperty(player,"aiflag","dead") ) setidle(player,openborconstant("ANI_IDLE"),1);
    } else {
        if ( is_on_base(player) && !is_in_pain(player) ) setidle(player,openborconstant("ANI_IDLE"),1);
    }

    if ( control_flag != NULL() ) changeentityproperty(player,"noaicontrol",control_flag);
}

int ent_exists(void ent) {
    if ( ent != NULL() ) {
        if ( getentityproperty(ent, "exists") ) return 1;
    }

    return 0;
}

void set_blackscreen(int value) {
    slv("black_screen",value);
}

int get_blackscreen() {
    return glv("black_screen");
}

int draw_blackscreen() {
	if ( !glv("black_screen") ) {
        drawbox (0, 0, ov("hresolution"), ov("vresolution"), MAX_Z_LAYER, rgbcolor(0x00,0x00,0x00), 0);
	}
}

int ready_to_attack(void self, int land_flag, int atk_flag, char anim) {
    int anim_id = getentityproperty(self,"animationid");

    if ( gep(self,"aiflag","animating") && !is_attacking(self) && !is_in_pain(self) && is_on_base(self) && gep(self,"tossv") == 0 ) return 1;

    if ( anim == "ANI_JUMP" || anim == "ANI_WALKOFF" ) land_flag = 0;

    if ( !is_walking(self) && !is_in_idle(self) ) {
        if ( anim != NULL() ) {
            if ( anim_id != openborconstant(anim) ) return 0;
            else return 1;
        }
        if ( getentityproperty(self,"animpos") > 0 ) {
            if ( anim_id != openborconstant("ANI_JUMP") && anim_id != openborconstant("ANI_WALKOFF") ) return 0;
        }
        if ( atk_flag > 0 && is_attacking(self) ) return 0;
    }
    if ( is_in_pain(self) ) return 0;
    if ( land_flag > 0 ) {
        if ( !is_on_base(self) ) return 0;
    }

    return 1;
}

int set_candamage(void self, char type) {
    int candamage = getentityproperty(self,"candamage");
    int dmg_type = oc(type);

    if ( !is_flag_on(candamage,dmg_type)  ) {
        changeentityproperty(self,"candamage",candamage+dmg_type);

        return 1;
    }

    return 0;
}

int remove_candamage(void self, char type) {
    int candamage = getentityproperty(self,"candamage");
    int dmg_type = oc(type);

    if ( is_flag_on(candamage,dmg_type)  ) {
        changeentityproperty(self,"candamage",candamage-dmg_type);

        return 1;
    }

    return 0;
}

int parse_vars() {
    void self = glv("self");
    char name = gep(self,"name");
    char model = gep(self,"model");
    char str = name, tmp_str = "", param = "";
    char value = "";
    int i = 0, si = 0;

    tmp_str = "";
    while( tmp_str != -1 ) {
        tmp_str = replace_str(str," ","");
        if ( tmp_str != -1 ) str = tmp_str;
    }
    tmp_str = "";
    while( tmp_str != -1 ) {
        tmp_str = replace_str(str," ","");
        if ( tmp_str != -1 ) str = tmp_str;
    }

    tmp_str = "";
    tmp_str = replace_str(str,"[","");
    if ( tmp_str == -1 ) return 0;
    else str = tmp_str;

    tmp_str = "";
    tmp_str = replace_str(str,"]","");
    if ( tmp_str == -1 ) return 0;
    else str = tmp_str;

    si = 0;
    while( si != -1 ) {
        si = searchstr_index(str,"=");
        if ( si == -1 ) break; // no more params

        // get substring
        si = searchstr_index(str,";");
        if ( si != -1 ) param = substr(str,0,si+1);
        else param = str;

        // update string - param
        str = substr(str,strlength(param),strlength(str)-strlength(param));

        // remove all ";"
        tmp_str = "";
        while( tmp_str != -1 ) {
            tmp_str = replace_str(param,";","");
            if ( tmp_str != -1 ) param = tmp_str;
        }

        // parse param and value
        si = searchstr_index(param,"=");
        value = param;
        param = substr(param,0,si);
        value = substr(value,si+1,strlength(value)-si-1);

        if ( is_float(value) ) sev(self,param,to_float(value));
        else sev(self,param,value);
    }

    if ( gev(self,"name") == NULL() ) {
        cep(self,"name",model);
    } else cep(self,"name",gev(self,"name"));

    //drawstring(10,50,0,"alias: "+gev(self,"name"));

    return 1;
}

float to_float(char num) {
    char str_decimal = "", str_mantix = "", str_digit = "", tmp = "";
    int decimal = 0, sign = 1, sign_flag = 0;
    float digit = 0;
    int i = 0;
    float float_num = 0, mantix = 0;

    i = searchstr_index(num,".");
    if ( i != -1 ) {
        str_decimal = substr(num,0,i);
        str_mantix = substr(num,i+1,strlength(num)-i-1);
    } else {
        str_decimal = num;
        str_mantix = "";
    }

    tmp = getchar(str_decimal,0);
         if ( tmp == "+" ) { sign =  1; sign_flag = 1; }
    else if ( tmp == "-" ) { sign = -1; sign_flag = 1; }
    if ( sign_flag ) str_decimal = substr(str_decimal,1,strlength(str_decimal)-1);

    // get decimal part
    for ( i = strlength(str_decimal)-1; i >= 0; --i ) {
        str_digit = getchar(str_decimal,i);

             if ( str_digit == "0" ) digit = 0;
        else if ( str_digit == "1" ) digit = 1;
        else if ( str_digit == "2" ) digit = 2;
        else if ( str_digit == "3" ) digit = 3;
        else if ( str_digit == "4" ) digit = 4;
        else if ( str_digit == "5" ) digit = 5;
        else if ( str_digit == "6" ) digit = 6;
        else if ( str_digit == "7" ) digit = 7;
        else if ( str_digit == "8" ) digit = 8;
        else if ( str_digit == "9" ) digit = 9;
        else continue;

        digit *= exp(10,strlength(str_decimal)-1-i);
        decimal += digit;
    }

    // get mantix part
    for ( i = 0; i < strlength(str_mantix); ++i ) {
        int div = 0;

        str_digit = getchar(str_mantix,i);

             if ( str_digit == "0" ) digit = 0;
        else if ( str_digit == "1" ) digit = 1;
        else if ( str_digit == "2" ) digit = 2;
        else if ( str_digit == "3" ) digit = 3;
        else if ( str_digit == "4" ) digit = 4;
        else if ( str_digit == "5" ) digit = 5;
        else if ( str_digit == "6" ) digit = 6;
        else if ( str_digit == "7" ) digit = 7;
        else if ( str_digit == "8" ) digit = 8;
        else if ( str_digit == "9" ) digit = 9;
        else continue;

        div = exp(10,i+1);
        digit = (digit*1.0)/div; // *1.0 to normalize to float
        mantix += digit;
    }

    float_num = (sign*(decimal+mantix));

    return float_num;
}

int is_float(char num) {
    char str_decimal = "", str_mantix = "", str_digit = "", tmp = "";
    int sign_flag = 0;
    float digit = 0;
    int i = 0;

    i = searchstr_index(num,".");
    if ( i != -1 ) {
        str_decimal = substr(num,0,i);
        str_mantix = substr(num,i+1,strlength(num)-i-1);
    } else {
        str_decimal = num;
        str_mantix = "";
    }

    tmp = getchar(str_decimal,0);
         if ( tmp == "+" ) { sign_flag = 1; }
    else if ( tmp == "-" ) { sign_flag = 1; }
    if ( sign_flag ) str_decimal = substr(str_decimal,1,strlength(str_decimal)-1);

    // get decimal part
    for ( i = strlength(str_decimal)-1; i >= 0; --i ) {
        str_digit = getchar(str_decimal,i);

             if ( str_digit == "0" ) digit = 0;
        else if ( str_digit == "1" ) digit = 1;
        else if ( str_digit == "2" ) digit = 2;
        else if ( str_digit == "3" ) digit = 3;
        else if ( str_digit == "4" ) digit = 4;
        else if ( str_digit == "5" ) digit = 5;
        else if ( str_digit == "6" ) digit = 6;
        else if ( str_digit == "7" ) digit = 7;
        else if ( str_digit == "8" ) digit = 8;
        else if ( str_digit == "9" ) digit = 9;
        else return 0;
    }

    // get mantix part
    for ( i = 0; i < strlength(str_mantix); ++i ) {
        str_digit = getchar(str_mantix,i);

             if ( str_digit == "0" ) digit = 0;
        else if ( str_digit == "1" ) digit = 1;
        else if ( str_digit == "2" ) digit = 2;
        else if ( str_digit == "3" ) digit = 3;
        else if ( str_digit == "4" ) digit = 4;
        else if ( str_digit == "5" ) digit = 5;
        else if ( str_digit == "6" ) digit = 6;
        else if ( str_digit == "7" ) digit = 7;
        else if ( str_digit == "8" ) digit = 8;
        else if ( str_digit == "9" ) digit = 9;
        else return 0;
    }

    return 1;
}

void get_parsed_var(char label) {
    void self = glv("self");
    void value = gev(self,label);

    return value;
}

int reset_hitbyid(void self, int HITBYID_RESET_TIME) {
    void opp = gep(self, "opponent");
    int time = ov("elapsed_time");

    if ( HITBYID_RESET_TIME == NULL() ) HITBYID_RESET_TIME = ov("game_speed")*1.0;

    // Reset hitbyid after a lot of time
    if ( gep(opp,"exists") ) {
        if ( gep(glv("hitbyid_opp"),"exists") ) {
            if ( glv("hitbyid_opp") != opp ) {
                if ( gep(glv("hitbyid_opp"),"hitbyid") != 0 ) {
                    cep(glv("hitbyid_opp"), "hitbyid", 0);
                    slv("hitbyid_opp",NULL());
                }
            }
        }
        slv("hitbyid_opp",opp);

        if ( gep(opp,"hitbyid") != 0 && glv("bind_time") == NULL() ) {
            slv("bind_time", time+HITBYID_RESET_TIME);
        } else if ( gep(opp,"hitbyid") != 0 && glv("bind_time") < time ) {
            cep(opp, "hitbyid", 0); // vedere con vulnerable!! -> resettiamo sempre la possibilità di colpire l'avversario!!
            slv("bind_time",NULL());
        }
    }
}

int check_duck(void player, int p, char anim_name) {
    if ( player == NULL() ) player = gpp(p,"entity");

    //if ( ! isATurtle(player) ) return 0;

    if ( ov("player_min_z") == ov("player_max_z") ) {
        if ( gep(player,"animvalid",oc(anim_name)) ) {
            //drawstring(10,50,0,"TEST");

            if ( ready_to_attack(player,1,1,anim_name) ) {
                if ( p == NULL() ) p = gep(player,"playerindex");

                if ( playerkeys(p,0,"movedown") ) { //  && !playerkeys(p,0,"moveleft") && !playerkeys(p,0,"moveright")
                    int anim_id = gep(player,"animationid");

                    if ( anim_id != oc(anim_name) ) {
                        if ( !gep(player,"aiflag","jumping") ) {
                            cep(player,"velocity",0,0,0);
                            performattack(player,oc(anim_name),1);
                            //cep(player,"animation",oc(anim_name),2);
                            cep(player,"attacking",0);
                            cep(player,"aiflag","attacking",0);
                            return 1;
                        }
                    } else { // in duck
                        if ( playerkeys(p,0,"jump") ) {
                            jump(player);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

void reopengif(void path) {
    void self = getlocalvar("self");
	void gif = openanigif(path);
	int MAX_INT = openborconstant("MAX_INT");
	float factor = 5;
	int time = openborvariant("elapsed_time");

    if ( time*factor >= MAX_INT ) changeopenborvariant("elapsed_time", 1);

	setlocalvar("gif", gif);
	setlocalvar("giftime", time*factor);
	decodeanigif(gif);
	return gif;
}

void playanigif(void path) {
  void self = getlocalvar("self");
  void gif = getlocalvar("gif");
  float factor = 5;
  int time = openborvariant("elapsed_time");

    // Check Time-Reset
    //if ( check_stored_time(getlocalvar("giftime"),9999) ) setlocalvar("giftime", openborconstant("MIN_INT"));
	if(!gif) {
		gif = reopengif(path);
	} else if(getanigifinfo(gif, "nextframe")+getlocalvar("giftime") <= time*factor) { // openborvariant("elapsed_time")*5 = speed
        //finished?
		if ( decodeanigif(gif) || (getlocalvar("skipable_gif") >= 1 && anykeyc() ) ) {
		    destroyanigif();
            gif = reopengif(path);
        }
	}

  //drawstring(10,20,0,getanigifinfo(gif, "nextframe"));
  changedrawmethod(NULL(), "reset", 1);
  drawscreen(getanigifinfo(gif, "buffer"), 0,0,0);
  changedrawmethod(NULL(), "reset", 1);
  changedrawmethod(NULL(), "flag", 0);
}

void set_skipable_gif(int value) {
    setlocalvar("skipable_gif",value);
}

void destroyanigif() { // oppure ondestroy() -> meglio usare questo su un'entity con ondrawscript. Ovviamente non lo chiami più questo metodo. Si chiamerà da solo
	void gif = getlocalvar("gif");
	if(gif) {
		free(gif);
		setlocalvar("gif", NULL());
	}
	setlocalvar("giftime", NULL());
}

int is_proximity(void ent1, void ent2, float range) {
    if ( ent_exists(ent1) && ent_exists(ent2) ) {
        float x1 = getentityproperty(ent1,"x");
        float z1 = getentityproperty(ent1,"z");
        float x2 = getentityproperty(ent2,"x");
        float z2 = getentityproperty(ent2,"z");

        if ( x1 >= x2-range && x1 <= x2+range && z1 >= z1-range && z1 <= z2+range ) {
            return 1;
        } else return 0;
    }
}

int is_proximity_by_coords(float x1, float z1, float x2, float z2, float range) {
    if ( x1 >= x2-range && x1 <= x2+range && z1 >= z1-range && z1 <= z2+range ) {
        return 1;
    } else return 0;
}

int exitotoset(int set, int usesave, int noavoidgameover) {
    int p = 0;

    changelevelproperty("type",2);
    for ( p = 0; p < openborvariant("maxplayers"); ++p ) {
        void player = getplayerproperty(p, "entity");

        changeplayerproperty(p, "lives", 0);
        changeplayerproperty(p, "score", 0);
        changeplayerproperty(p, "credits", 0);
        changeplayerproperty(p, "hasplayed", 0);
        changeplayerproperty(p, "spawnhealth", 0);
        changeplayerproperty(p, "spawnmp", 0);
        changeplayerproperty(p, "score", 0);
        changeplayerproperty(p, "name", "");
        if ( getentityproperty(player,"exists") ) {
            killentity(player);
        }
    }

    if ( noavoidgameover == NULL() || noavoidgameover <= 0 ) changeplayerproperty(0,"lives",1); // to avoid game over

    playgame(set,usesave);
}


