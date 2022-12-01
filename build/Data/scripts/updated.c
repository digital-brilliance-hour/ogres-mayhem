#include "data/scripts/common/constants.h"
#include "data/scripts/story/story_define.h"
#include "data/scripts/story.c"
#include "data/scripts/gettick.c"

void zoom()
{
   void vscreen = openborvariant("vscreen");
   int maxz=openborvariant("PLAYER_MAX_Z")+1000;
   int zoom_value=getglobalvar("zoomvalue");
   int zoom_x=getglobalvar("zoomx");
   int zoom_y=getglobalvar("zoomy");
   void ent=getglobalvar("zoomentity");
   int px=getentityproperty(ent,"x") +  zoom_x - openborvariant("xpos");
   int py=getentityproperty(ent,"z") + zoom_y - openborvariant("ypos"); //- getentityproperty(ent,"a");
   void zoom_scr = getglobalvar("zoomscreen");
   if(!zoom_scr){
      zoom_scr = allocscreen(openborvariant("hResolution"),openborvariant("vResolution"));
      setglobalvar("zoomscreen",zoom_scr);
   }
   clearscreen(zoom_scr);

   //draw what we need 
   drawspriteq(zoom_scr,0,openborconstant("MIN_INT"),maxz,0,0);
   //setup drawMethod
   changedrawmethod(NULL(),"reset",1);
   changedrawmethod(NULL(),"enabled",1);
   changedrawmethod(NULL(),"scalex",zoom_value);
   changedrawmethod(NULL(),"scaley",zoom_value);
   changedrawmethod(NULL(),"centerx",px);
   changedrawmethod(NULL(),"centery",py);
   //Draw the resized customized screen to main screen.
   drawscreen(zoom_scr,px,py, maxz+1);
   changedrawmethod(NULL(),"enabled", 0);
   drawspriteq(vscreen, 0, maxz+1,maxz+1, 0, 0);
   drawspriteq(vscreen, 0, maxz+2,openborconstant("MAX_INT"), 0, 0);
   clearspriteq();
}

void main()
{
  if(getglobalvar("zoomentity"))
  {
      zoom();        
  }
	mainLoop();
}

void mainLoop()
{
	if(SYS_INLEVEL)
	{
		inLevelLoop();
	}
	else if (SYS_SCENE != "intro.txt"){
		/*if(SYS_INMENU) {
			void background = loadsprite("data/bgs/title.png");
			drawsprite(background, 0, 0, 0, 0);
		}
		else if(SYS_INTITLE) {
			void background = loadsprite("data/bgs/title.png");
			drawsprite(background, 0, 0, 0, 0);

		}
		if(SYS_INMENU || SYS_INTITLE){
			drawstring(110,40,3,"Story System EdItIon");
			drawstring(124,212,0,"CRxTRDude 2014");
		}*/
	}
  if (SYS_SCENE != NULL()){
      //log("intro played");
      setglobalvar("TitleMusic", 0);
  }

  if(openborvariant("in_halloffamescreen")==1){
    //highScoreMusic();
  }
  if(SYS_INMENU || SYS_INTITLE){
      drawstring(400,10,1,"Version: 0.9.4");
  } 

	if(openborvariant("in_selectscreen")==1){
      int models_cached = openborvariant("models_cached");
      int i = 0;
      int C = getglobalvar("Load");

      for( i = 0; i < models_cached; ++i ) {
        char model = getmodelproperty(i,2);

        if( model == "Royce" && C != 39 ) {
            changemodelproperty(i,4,0);
            return;
        }
      }
    } else if(openborvariant("in_menuscreen")==1){
      setindexedvar(0, NULL());
    }
}

void highScoreMusic()
{
  void bg = "data/music/highscore.bor";
  //playmusic(bg, 1);

}

void inLevelLoop()
{
	turnWhite();
	storySystem();
  showMoves();
  int j, num, maxmp;
  for(j=0; j<4; j++){
    num = j + 1;
    void p = getplayerproperty(j, "entity"); 
    maxmp = getentityproperty(p, "maxmp");
    if(maxmp != 0) {
      fullmp(p, num);
    }
  }
  //log(openborvariant("viewportx"));
  //log(openborvariant("xpos"));
  //("\n");
}

void oncreate()
{
    int C = fileskim("Rise");
    setglobalvar("Load", C);

    setindexedvar(1, 0);
}

void resetMethod(char Name, void self)
{/*
    Kills all entities with defined alias 
  
    Name: Name of entity to be killed
    */

    void vEntity;                                       //Target entity placeholder.
    int  iEntity;                                       //Entity enumeration holder.
    char iName;                                         //Entity Name.
    int  iMax      = openborvariant("count_entities");  //Entity count.

    //Enumerate and loop through entity collection.
    for(iEntity=0; iEntity<iMax; iEntity++){
      vEntity = getentity(iEntity);                 //Get target entity from current loop.
      iName   = getentityproperty(vEntity, "name"); //Get target name.
      
      if(iName == Name){ //Same alias?
        if(Name == "tint") {
          changedrawmethod(self, "tintmode", 0);
          changedrawmethod(self, "tintcolor", 0);
        }
        killentity(vEntity);
      }
    }
}

void fullmp(void p, int num) {
  int mp, maxmp;
  void subent;
  mp = getentityproperty(p, "mp");
  maxmp = getentityproperty(p, "maxmp");
  //log("mp = " + mp + ". maxmp = " + maxmp + ". ");
  int tintmode = getdrawmethod(p, "tintmode");
  void blinking = getglobalvar("fullmp" + num);
  if(mp == maxmp && blinking != 1) {
    log("madeithere ");
    loadmodel("tint"); // name of the entity to be loaded        
        clearspawnentry(); // clean the spawn entry        
         setspawnentry("name", "tint"); // define the entity to be spawn        
         setspawnentry("coords", -1,-1,-1000); // set the position of the entity       
         subent=spawn();  //  spawn the entity
         bindentity(subent, p, 0, 0, 0, 0, 0);
    changeentityproperty(subent, "parent", p); //Set caller as parent.
    setentityvar(subent, "blink", 1);
        setentityvar(subent, "tintSpeed", 10);
        setentityvar(subent, "tintMode", 1);
        setentityvar(subent, "tintTo", rgbcolor(11, 222, 67));
        setentityvar(subent, "blinkStart", rgbcolor(67, 140, 13));
        setglobalvar("fullmp" + num, 1);
  }
  else if (mp < maxmp && blinking == 1){
    resetMethod("tint", p);
    setglobalvar("fullmp" + num, NULL());
  }
  else {
    return 0;
  }
}

void showMoves() {

    int showM = getglobalvar("showM");
    if(showM) {
      void mTitle = getglobalvar("mTitle");
      void black = getglobalvar("black_screen"); 
      void AButton = getglobalvar("attack-button"); 
      void DButton = getglobalvar("dash-button"); 
      void JButton = getglobalvar("jump-button"); 
      void SButton = getglobalvar("special-button"); 
      void PButton = getglobalvar("power-button"); 
      //log(openborconstant("FLAG_ATTACK"));
      //SDL_GetKeyName(openborconstant("FLAG_ATTACK"));
      //mTitle = loadsprite("data/sprites/movelist/movelist-title.png");
      //Start of drawmethod enablement
      changedrawmethod(NULL(), "enabled", 1);
      changedrawmethod(NULL(), "flag", 1);

      //Title
      changedrawmethod(NULL(), "reset", 1);
      changedrawmethod(NULL(), "centerx", 108);
      changedrawmethod(NULL(), "centery", 12);
      drawsprite(mTitle, 240, 30, 99999999999999);
      
      //Buttons
      changedrawmethod(NULL(), "reset", 1);
      changedrawmethod(NULL(), "centerx", 8);
      changedrawmethod(NULL(), "centery", 8);
      drawsprite(AButton, 109, 60, 99999999999999); 
      drawsprite(DButton, 177, 60, 99999999999999);
      drawsprite(JButton, 229, 60, 99999999999999);
      drawsprite(SButton, 281, 60, 99999999999999);
      drawsprite(PButton, 351, 60, 99999999999999);

      //Buttons for Moves

      drawsprite(AButton, 132, 103, 99999999999999); 
      drawsprite(AButton, 412, 103, 99999999999999);

      //Fonts
      changedrawmethod(NULL(), "reset", 1);
      settextobj(11, 117, 57, 0, 99999999999999,  ": Attack");
      settextobj(21, 186, 57, 0, 99999999999999,  ": Dash");
      settextobj(31, 239, 57, 0, 99999999999999,  ": Jump");
      settextobj(41, 291, 57, 0, 99999999999999,  ": Special");
      settextobj(51, 361, 57, 0, 99999999999999,  ": Power");
      settextobj(61, 40, 85, 1, 99999999999999,  "Attack Combo");
      settextobj(71, 40, 100, 2, 99999999999999,  "Repeatedly Press");
      settextobj(81, 180, 85, 1, 99999999999999,  "Dash");
      settextobj(91, 180, 100, 2, 99999999999999,  "Parry or Dash through attacks");
      settextobj(101, 370, 85, 1, 99999999999999,  "Jump Attack");
      settextobj(111, 370, 100, 2, 99999999999999,  "Jump + ");

      //Special Moves
      settextobj(121, 30, 133, 1, 99999999999999,  "LvL2 Move - 10MP");
      drawsprite(SButton, 30, 143, 99999999999999);

      settextobj(131, 175, 133, 1, 99999999999999,  "LvL3 Move  - 20MP");
      settextobj(141, 175, 148, 2, 99999999999999,  "Up + ");
      drawsprite(SButton, 195, 143, 99999999999999);

      settextobj(151, 320, 133, 1, 99999999999999,  "LvL4 Move  - ALL MP");
      drawsprite(PButton, 320, 143, 99999999999999);

      settextobj(161, 30, 173, 1, 99999999999999,  "LvL5 Move  - 30MP");
      settextobj(171, 30, 188, 2, 99999999999999,  "Down + ");
      drawsprite(SButton, 60, 183, 99999999999999);

      settextobj(181, 175, 173, 1, 99999999999999,  "LvL6 Move  - ALL MP");
      settextobj(191, 175, 188, 2, 99999999999999,  "Up + ");
      drawsprite(PButton, 195, 183, 99999999999999);

      settextobj(201, 320, 173, 1, 99999999999999,  "LvL7 Move  - ALL MP");
      settextobj(211, 320, 188, 2, 99999999999999,  "Down + ");
      drawsprite(PButton, 350, 183, 99999999999999);

      //Black Overlay
      changedrawmethod(NULL(), "reset", 1);
      changedrawmethod(NULL(), "alpha", 6);
      //changedrawmethod(NULL(), "channelr", 0.5);
      //changedrawmethod(NULL(), "channelg", 0.5);
      //changedrawmethod(NULL(), "channelb", 0.5);
      drawsprite(black, 0, 0, 99999999999998);
      //changedrawmethod(NULL(), "enabled", 1);
      //changedrawmethod(NULL(), "flag", 1);
    }
    else {
      cleartextobj(11);
      cleartextobj(21);
      cleartextobj(31);
      cleartextobj(41);
      cleartextobj(51);
      cleartextobj(61);
      cleartextobj(71);
      cleartextobj(81);
      cleartextobj(91);
      cleartextobj(101);
      cleartextobj(111);

      //Special Moves
      cleartextobj(121);

      cleartextobj(131);
      cleartextobj(141);

      cleartextobj(151);

      cleartextobj(161);
      cleartextobj(171);

      cleartextobj(181);
      cleartextobj(191);

      cleartextobj(201);
      cleartextobj(211);
    }
}

void fileskim(char Line)
{
      void file = openfilestream("Test.txt", 1);
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