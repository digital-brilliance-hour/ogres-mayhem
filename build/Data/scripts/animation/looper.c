void looper(int Limit)
{// Loops current animation
    void self = getlocalvar("self");
    void loop = getlocalvar("Loop" + self);

    if(loop==NULL()){ // Localvar empty?
      setlocalvar("Loop" + self, 0);
      loop = 0;
    }
  
    if(loop < Limit){ // loops reach limit?
      setlocalvar("Loop" + self, loop+1); // Increment number of loops
      //updateframe(self, Frame); //Change frame
    } else if(loop==Limit){ // loops reach limit?
      setlocalvar("Loop" + self, NULL());
      killentity(self);
    }
}