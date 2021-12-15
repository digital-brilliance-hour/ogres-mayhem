#import "data/scripts/levelup/lvup.c"

void main() {
	if (openborvariant("in_level")) {
		void self = getlocalvar("self"); // get caller
		void parent = getentityproperty(self, "parent"); // get parent
		if(parent!=NULL()){ // is parent valid?
			int x = getentityproperty(parent, "x"); // get parent X pos
	        int y = getentityproperty(parent, "y"); // get parent Z pos
			void pindex = getentityproperty(parent, "playerindex"); //get current player index
			void pname = getentityproperty(parent, "name"); //get current player name
			if(pindex!=NULL()) {
				int Level=getglobalvar("level."+pindex);
				log(x);
				if(Level>1) {
					switch(Level) {
						case 2:
						//settextobj(2*(Level+1), 200, 200, 4, 999999999,  "Reached Level:"+Level+"!!", openborvariant("elapsed_time")+400);
				        settextobj(2*(Level+1), 102, 50, 4, 999999999,  "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", openborvariant("elapsed_time")+400);
				        settextobj(2*(Level+2), 62, 200, 4, 999999999,  "New Move Unlocked! Tap Action to Use", openborvariant("elapsed_time")+400);
				        settextobj(2*(Level+3), 182, 225, 4, 999999999,  "Drains 10 MP", openborvariant("elapsed_time")+400);
						break;
						case 3:
						if(pname=="Gon") {}
						else if(pname=="Ichigo") {}
						else if(pname=="Rock_Lee") {}
						else {
					        settextobj(2*(Level+1), 102, 50, 4, 999999999,  "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+2), 62, 200, 4, 999999999,  "New Move Unlocked! Up Up Action to Use", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+3), 182, 225, 4, 999999999,  "Drains 20 MP", openborvariant("elapsed_time")+400);
					    }
						break;
						case 4:
						if(pname=="Luffy") {}
						else {
					        settextobj(2*(Level+1), 102, 50, 4, 999999999,  "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+2), 65, 200, 4, 999999999,  "New Move Unlocked! Tap Power to Use", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+3), 177, 225, 4, 999999999,  "Drains All MP", openborvariant("elapsed_time")+400);
					    }
						break;
						case 5:
						if(pname=="Naruto") {}
						else if(pname=="Luffy") {}
						else if(pname=="Goku_Normal") {}
						else if(pname=="Ichigo") {}
						else if(pname=="Rock_Lee") {}
						else {
					        settextobj(2*(Level+1), 102, 50, 4, 999999999,  "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+2), 27, 200, 4, 999999999,  "New Move Unlocked! Down Down Action to Use", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+3), 182, 225, 4, 999999999,  "Drains 30 MP", openborvariant("elapsed_time")+400);
					    }
						break;
						case 6:
						if(pname=="Naruto") {}
						else if(pname=="Luffy") {}
						else if(pname=="Deku") {}
						else if(pname=="Goku_Normal") {}
						else if(pname=="Ichigo") {}
						else if(pname=="Rock_Lee") {}
						else {
					        settextobj(2*(Level+1), 102, 50, 4, 999999999,  "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+2), 65, 200, 4, 999999999,  "New Move Unlocked! Up Up Power to Use", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+3), 182, 225, 4, 999999999,  "Drains All MP", openborvariant("elapsed_time")+400);
					    }
						break;
						case 7:
						if(pname=="Goku_Normal" || pname=="Deku" || pname=="Luffy") {}
						else {
					        settextobj(2*(Level+1), 102, 50, 4, 999999999,  "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+2), 62, 200, 4, 999999999,  "New Move Unlocked! Down Down Power to Use", openborvariant("elapsed_time")+400);
					        settextobj(2*(Level+3), 177, 225, 4, 999999999,  "Drains All MP", openborvariant("elapsed_time")+400);
						}
						break;
					}
					setlocalvar("helptextbox1", 2*(Level+1));
				}

			}
		}
	}
	else {
		void self = getlocalvar("self"); // get caller
		void parent = getentityproperty(self, "parent"); // get parent
		if(parent!=NULL()){ // is parent valid?
			int x = getentityproperty(parent, "x"); // get parent X pos
	        int y = getentityproperty(parent, "y"); // get parent Z pos
			void pindex = getentityproperty(parent, "playerindex"); //get current player index
			void pname = getentityproperty(parent, "name"); //get current player name
			if(pindex!=NULL()) {
				int Level=getglobalvar("level."+pindex);
				log(x);
				if(Level>1) {
					switch(Level) {
						case 2:
						//settextobj(2*(Level+1), 200, 200, 4, 999999999,  "Reached Level:"+Level+"!!", openborvariant("elapsed_time")+400);
				        drawstring(102, 50, 4, "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", 999999999);
				        drawstring(62, 200, 4, "New Move Unlocked! Tap Action to Use", 999999999);
				        drawstring(182, 225, 4, "Drains 10 MP", 999999999);
						break;
						case 3:
						if(pname=="Gon") {}
						else if(pname=="Ichigo") {}
						else if(pname=="Rock_Lee") {}
						else {
					        drawstring(102, 50, 4, "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", 999999999);
					        drawstring(62, 200, 4, "New Move Unlocked! Up Up Action to Use", 999999999);
					        drawstring(182, 225, 4, "Drains 20 MP", 999999999);
					    }
						break;
						case 4:
						if(pname=="Luffy") {}
						else {
					        drawstring(102, 50, 4, "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", 999999999);
					        drawstring(65, 200, 4, "New Move Unlocked! Tap Power to Use", 999999999);
					        drawstring(177, 225, 4, "Drains All MP", 999999999);
					    }
						break;
						case 5:
						if(pname=="Naruto") {}
						else if(pname=="Luffy") {}
						else if(pname=="Goku_Normal") {}
						else if(pname=="Ichigo") {}
						else if(pname=="Rock_Lee") {}
						else {
					        drawstring(102, 50, 4, "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", 999999999);
					        drawstring(27, 200, 4, "New Move Unlocked! Down Down Action to Use", 999999999);
					        drawstring(182, 225, 4, "Drains 30 MP", 999999999);
					    }
						break;
						case 6:
						if(pname=="Naruto") {}
						else if(pname=="Luffy") {}
						else if(pname=="Deku") {}
						else if(pname=="Goku_Normal") {}
						else if(pname=="Ichigo") {}
						else if(pname=="Rock_Lee") {}
						else {
					        drawstring(102, 50, 4, "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", 999999999);
					        drawstring(65, 200, 4, "New Move Unlocked! Up Up Power to Use", 999999999);
					        drawstring(182, 225, 4, "Drains All MP", 999999999);
					    }
						break;
						case 7:
						if(pname=="Goku_Normal" || pname=="Deku" || pname=="Luffy") {}
						else{
					        drawstring(102, 50, 4, "Player "+(pindex+1)+" Has Reached Level "+Level+"!!", 999999999);
					        drawstring(62, 200, 4, "New Move Unlocked! Down Down Power to Use", 999999999);
					        drawstring(177, 225, 4, "Drains All MP", 999999999);
						}
						break;
					}
					setlocalvar("helptextbox1", 2*(Level+1));
				}

			}
		}

	}
}