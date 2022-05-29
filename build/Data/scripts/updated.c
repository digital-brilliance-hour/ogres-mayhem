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