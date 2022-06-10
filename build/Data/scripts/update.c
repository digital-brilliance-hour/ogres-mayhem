#include "data/scripts/traileru.c"
#include "data/scripts/levelup/lvup.c"

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

int scoreExists(int pindex, int sindex) {
  return getsaveinfo(sindex, "SCORE", pindex);
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
             //settextobj(10*i, 6+120*i, 221, 0, -10, getentityproperty(p,"offense",openborconstant("ATK_NORMAL"))+"/"+getglobalvar("offense_"+num+"P"));
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
      setUpdate();
    }

    if (openborvariant("game_paused")) {
      //will place code to add background soon.
      
       /*void black_scr = getglobalvar("blackscreen");
       if(!black_scr){
          black_scr = allocscreen(openborvariant("hResolution"),openborvariant("vResolution"));
          setglobalvar("blackscreen",black_scr);
       }
       clearscreen(black_scr);
      void blackscreencounter = getlocalvar("blackscreencounter");
      if(blackscreencounter != 1){
      blackscreencounter = setlocalvar("blackscreencounter",1);  
      } 
      int ls = getglobalvar("blackpause");
      void background = loadsprite("data/bgs/black.gif");
      drawspritetoscreen(background, black_scr, 0, 0);
      setdrawmethod(NULL(), 0, 256, 256, 0, 0, 0, 1, 0);
      drawscreen(black_scr,0,0,1000,0);

      //changedrawmethod(NULL(), "alpha", 1);
      void blackscreencounter = getlocalvar("blackscreencounter"); 
      while(blackscreencounter!=1)
      {
        void subent;
        loadmodel("blackScreen"); // name of the entity to be loaded
        clearspawnentry(); // clean the spawn entry
        setspawnentry("name", "blackScreen"); // define the entity to be spawn
        //setspawnentry("coords", 1,1,10000); // set the position of the entity
        subent=spawn();  //  spawn the entity
        //changeentityproperty(subent, "position", 1,1,10000); //for safe, set again the position
        setglobalvar("bckground", subent);
        blackscreencounter = setlocalvar("blackscreencounter",1); // turn on the variable, blocking a new spawn to be made
      }*/

      /*void p;
      int i, hp, mp, lv, num;
      for(i=0; i<4; i++){
         p = getplayerproperty(i, "entity");
         num = i+1;
         if(p){
           if("Travel"!=getentityproperty(p, "model")) {
             drawstring(6+120*i, 33, 1, "attack", 10);
             drawstring(6+120*i, 49, 1, "u,u or d,d", 10);
             drawstring(6+120*i, 65, 1, "action", 10);
             drawstring(6+120*i, 89, 1, "up,action", 10);
             drawstring(6+120*i, 113, 1, "down,action", 10);
             drawstring(6+120*i, 137, 1, "power", 10);
             drawstring(6+120*i, 161, 1, "up,power", 10);
             drawstring(6+120*i, 185, 1, "down,power", 10);
           }
         }
      }*/
}
  afterImg();
}

void setUpdate() {
  if(getglobalvar("setupdate") > 0) {
    int setnumber = 1;
    int currentStage = getglobalvar("setupdate");
    if(currentStage == 6) {
      setnumber = 2;
    }
    log(getsaveinfo(setnumber, "SCORE", 0));
    int checkScore = getglobalvar("checkSet1Score");
    if(checkScore != 1) {
        void p;
        int i, hp, mp, lv, num;
        for(i=0; i<4; i++){
            p = getplayerproperty(i, "entity");
            num = i+1;
            if(p){
                if("Travel"!=getentityproperty(p, "model")) {
                  if(currentStage == 4) {
                    if(scoreExists(i, 1) < 1) {
                      if(scoreExists(i, 0) < 21000) {
                        changeplayerproperty(i, "score", 21500);
                        lvup(i);

                      }
                      else {
                        changeplayerproperty(i, "score", scoreExists(i, 0));
                      lvup(i);

                      }
                    }
                  }
                  if(currentStage == 6) {
                    if(scoreExists(i, 2) < 1) {
                      if(scoreExists(i, 1) < 45000) {
                        changeplayerproperty(i, "score", 45500);
                        lvup(i);

                      }
                      else {
                        changeplayerproperty(i, "score", scoreExists(i, 0));
                      lvup(i);

                      }
                    }
                  }
                }
            }
        }
        setglobalvar("checkSet1Score", 1);
        setglobalvar("setupdate", NULL());
    }
  }
}

void oncreate(){
  void Stage1, Stage2, Stage3, Stage4, Stage5; 
  void Stage6, Stage7, Stage8, Stage9, Stage10;    
  void Title2;

  Title2 = loadsprite("data/bgs/black.gif");

  Stage1 = loadsprite("data/sprites/stages/stage1.gif");
  Stage2 = loadsprite("data/sprites/stages/stage2.gif");
  Stage3 = loadsprite("data/sprites/stages/stage3.gif");
  Stage4 = loadsprite("data/sprites/stages/stage4.gif");
  Stage5 = loadsprite("data/sprites/stages/stage5.gif");
  Stage6 = loadsprite("data/sprites/stages/stage6.gif");
  Stage7 = loadsprite("data/sprites/stages/stage7.gif");
  Stage8 = loadsprite("data/sprites/stages/stage8.gif");
  Stage9 = loadsprite("data/sprites/stages/stage9.gif");
  Stage10 = loadsprite("data/sprites/stages/stage10.gif");
      
  setglobalvar("Title2", Title2);

  setglobalvar("Stage1", Stage1);
  setglobalvar("Stage2", Stage2);
  setglobalvar("Stage3", Stage3);
  setglobalvar("Stage4", Stage4);
  setglobalvar("Stage5", Stage5);
  setglobalvar("Stage6", Stage6);
  setglobalvar("Stage7", Stage7);
  setglobalvar("Stage8", Stage8);
  setglobalvar("Stage9", Stage9);
  setglobalvar("Stage10", Stage10);
  
    void mTitle;
    void black;
    void AButton, DButton, JButton, SButton, PButton;

    mTitle = loadsprite("data/sprites/movelist/movelist-title.gif");
    black = loadsprite("data/bgs/black.gif");
    AButton = loadsprite("data/sprites/movelist/a-letter.gif");
    DButton = loadsprite("data/sprites/movelist/d-letter.gif");
    JButton = loadsprite("data/sprites/movelist/j-letter.gif");
    SButton = loadsprite("data/sprites/movelist/s-letter.gif");
    PButton = loadsprite("data/sprites/movelist/p-letter.gif");

    setglobalvar("mTitle", mTitle); 
    setglobalvar("black_screen", black);
    setglobalvar("attack-button", AButton);
    setglobalvar("dash-button", DButton);
    setglobalvar("jump-button", JButton);
    setglobalvar("special-button", SButton);
    setglobalvar("power-button", PButton);
    setglobalvar("bossInterlude", 0);
}

void ondestroy(){
  void Stage1 = getglobalvar("Stage1");    
  void Stage2 = getglobalvar("Stage2"); 
  void Stage3 = getglobalvar("Stage3");    
  void Stage4 = getglobalvar("Stage4");
  void Stage5 = getglobalvar("Stage5");    
  void Stage6 = getglobalvar("Stage6");
  void Stage7 = getglobalvar("Stage7");    
  void Stage8 = getglobalvar("Stage8");
  void Stage9 = getglobalvar("Stage9");    
  void Stage10 = getglobalvar("Stage10"); 
      
  void Title2 = getglobalvar("Title2");  
  void mTitle = getglobalvar("mTitle"); 
  void black = getglobalvar("black_screen");  
  void AButton = getglobalvar("attack-button"); 
  void DButton = getglobalvar("dash-button"); 
  void JButton = getglobalvar("jump-button"); 
  void SButton = getglobalvar("special-button"); 
  void PButton = getglobalvar("power-button"); 
  free(Stage1);    
  free(Stage2);   
  free(Stage3);    
  free(Stage4);   
  free(Stage5);    
  free(Stage6);   
  free(Stage7);    
  free(Stage8);   
  free(Stage9);    
  free(Stage10);    
  free(Title2);  
  free(mTitle);   
  free(black);
  free(AButton);
  free(DButton);
  free(JButton);
  free(SButton);
  free(PButton); 
  setglobalvar("Stage1", NULL());
  setglobalvar("Stage2", NULL());
  setglobalvar("Stage3", NULL());
  setglobalvar("Stage4", NULL());
  setglobalvar("Stage5", NULL());
  setglobalvar("Stage6", NULL());
  setglobalvar("Stage7", NULL());
  setglobalvar("Stage8", NULL());
  setglobalvar("Stage9", NULL());
  setglobalvar("Stage10", NULL());      
  setglobalvar("Title2", NULL()); 
  setglobalvar("mTitle", NULL()); 
  setglobalvar("black_screen", NULL()); 
    setglobalvar("attack-button", NULL());
    setglobalvar("dash-button", NULL());
    setglobalvar("jump-button", NULL());
    setglobalvar("special-button", NULL());
    setglobalvar("power-button", NULL());
  setglobalvar("bossInterlude", NULL());
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
