#include "data/scripts/traileru.c"

#define   MODEL_NAME 2
#define   SELECTABLE 4
#define   MAXENTCOUNT openborvariant("models_cached")
#define obv openborvariant
void setValid(void name,void isselectable)
{
   int i;
   for(i=0;i<MAXENTCOUNT;i++)
   {
      if(getmodelproperty(i,MODEL_NAME)==name)
      {
         //log(getmodelproperty(i,MODEL_NAME));
         changemodelproperty(i,SELECTABLE,isselectable);
         return;
      }
   }
}

int getScoreLevel(int lvl){
 if (lvl%2 == 1){
 return lvl * (lvl/2) * 1000;
 }else{
 return (lvl - 1) * (lvl/2) * 1000;
 }
}

void lockBehavior(char player) {
  int count = 0; 
  int i;
  //set your array
  void items = array(6);
  set(items,0,"Hiei");
  set(items,1,"Sasuke");
  set(items,2,"Renji");
  set(items,3,"Toshiro");
  set(items,4,"Kenpachi");
  for(i=0; i<size(items); i++) 
  {
    
  }
   
}

void main()
{
    void scene=openborvariant("current_scene");
    loadgamefile();
    //log(getsaveinfo(0, "playername", 0));
    if(getsaveinfo(0, "times_completed") > 1) 
    {
      
    }
    if(scene == "intro.txt")
    {
      //log("at intro");
      setglobalvar("TitleMusic", 0);
    }
    if(scene == "ending.txt")
    {
      setValid("Broly",1);
    }
    // Update script for locking certain player
    if(openborvariant("in_selectscreen")==1){
      //setValid("Broly",1);

      int S = getglobalvar("SelectMusic");
      if(S != 1) {
        void bg = "data/music/select.ogg";
        playmusic(bg, 1);
        setglobalvar("SelectMusic", 1);
      }
    } else if(openborvariant("in_menuscreen")==1){
      setindexedvar(0, NULL());
        setglobalvar("SelectMusic", 0);
    }
    else {

        setglobalvar("SelectMusic", 0);
    }
    if (openborvariant("in_titlescreen")) {
      loadgamefile();

      int j, num;
      for(j=0; j<4; j++){
        num = j+1;
        setglobalvar("maxmp_"+num+"P", NULL());
        setglobalvar("maxhealth_"+num+"P",  NULL());
        setglobalvar("offense_"+num+"P", NULL());
        setglobalvar("defense_"+num+"P", NULL());
        setglobalvar("score"+j,0);
        setglobalvar("level."+j,1);
      }
      //log(getsaveinfo(0, "name"));
      //log(getglobalvar("TitleMusic"));
      //log(getsaveinfo(0, "times_completed"));
      int T = getglobalvar("TitleMusic");
      if(T != 1) {
        void bg = "data/music/menu.ogg";
        playmusic(bg, 1, 1400919);
        setglobalvar("TitleMusic", 1);
      }
    }
    else {
        setglobalvar("TitleMusic", 0);
    }
    if (openborvariant("in_halloffamescreen")) {
      int H = getglobalvar("HOFMusic");
      if(H != 1) {
        void hbg = "data/music/hiscore.ogg";
        playmusic(hbg, 1, 644145);
        setglobalvar("HOFMusic", 1);
      }
    }
    else {
        setglobalvar("HOFMusic", 0);
    }


    if (openborvariant("in_showcomplete")) {
      int CO = getglobalvar("CompMusic");
      if(CO != 1) {
        void cbg = "data/music/complete.ogg";
        playmusic(cbg, 1);
        setglobalvar("CompMusic", 1);
      }
    }
    else {
        setglobalvar("CompMusic", 0);
    }

    if(getglobalvar("punt_ant_1P") == NULL()){
     setglobalvar("punt_ant_1P", 0);
    }
     if(getglobalvar("punt_ant_2P") == NULL()){
     setglobalvar("punt_ant_2P", 0);
    }
     if(getglobalvar("punt_ant_3P") == NULL()){
     setglobalvar("punt_ant_3P", 0);
    }
     if(getglobalvar("punt_ant_4P") == NULL()){
     setglobalvar("punt_ant_4P", 0);
    }
    if (openborvariant("in_level") && openborvariant("current_stage") < 12) {
      // settextobj(150, 102, 50, 4, 99999999,  "Player 1 Has Reached Level 1!!", openborvariant("elapsed_time")+400);
      // settextobj(151, 27, 200, 4, 99999999,  "New Move Unlocked! Down Down Action to Use", openborvariant("elapsed_time")+400);
      // settextobj(152, 177, 225, 4, 99999999,  "Drains All MP", openborvariant("elapsed_time")+400);
       void p;
       int i, hp, mp, lv, num;
       for(i=0; i<4; i++){
         p = getplayerproperty(i, "entity");
         num = i+1;
         if(p){
           if("Travel"!=getentityproperty(p, "model")) {
             //HP text script
             hp = getentityproperty(p, "health"); if(hp<0) hp=0;
             settextobj(10*i, 6+120*i, 221, 0, -10, getentityproperty(p,"offense",openborconstant("ATK_NORMAL"))+"/"+getglobalvar("offense_"+num+"P"));
             //settextobj(10*i, 6+120*i, 221, 0, -10, hp+"/"+getentityproperty(p, "maxhealth"));
             //MP text script
             mp = getentityproperty(p, "mp"); if(mp<0) mp=0;
             //settextobj(11*i, 6+120*i, 236, 0, -10,  mp+"/"+getentityproperty(p, "maxmp"));
             //Lv
             lv = getglobalvar("level."+i);
             if(!lv) lv=1;
             settextobj(12*i, 6+120*i, 250, 3, -10,  "Power LVL:"+lv);
             //Muestra la barra de nivel
             //if(getplayerproperty(i, "score")<=190000)
             //drawbox(6+120*i, 258, 50, 10, 9999, rgbcolor(0,255,255), 0);
             //Muestra las vidas para que muestre la vida 0
             //drawstring(96+120*i, 217, 0, getplayerproperty(i, "lives") - 1);
           }
         }
      }
      show_enemy_health();
    }
  afterImg();
}

void player_used(char playername)
{
       void p;
       int i, hp, mp, lv;
       for(i=0; i<4; i++){
         p = getplayerproperty(i, "name");
         if(p){
            if(p == playername) {
              return 1;
            }
         }
       }
}

void oncreate()
{
    int C = fileskim("Rise", "Test.txt");
    setglobalvar("Load", C);

    setindexedvar(1, 0);
}

void ondestroy(){
}

void fileskim(char Line, void Path)
{
      void file = openfilestream(Path , 1);
      void vLoad;

      if(file != -1){
        setfilestreamposition(file, 0);
        vLoad = getfilestreamargument(file, 0, "string");

        while(vLoad != Line && vLoad != "#End"){
          filestreamnextline(file);
          vLoad = getfilestreamargument(file, 0, "string");          
        }
        if( vLoad == Line ){
          filestreamnextline(file);
          vLoad = getfilestreamargument(file, 0, "int");
          return vLoad;

          closefilestream(file);
        } else if(vLoad == "#End"){
          closefilestream(file);
          return 0;
        }
      } else {
        return 0;
      }
}