void main() {
	void self = getlocalvar("self");
	int pauseval = getlocalvar("pauseadd");                 //Caller   
    int  iETime     = openborvariant("elapsed_time");       //Current time.
    void vParent = getentityproperty(self, "parent"); 
    if(pauseval > 0) {

        	if(vParent != self) {
            changeentityproperty(vParent, "frozen", 1);               //Toggle frozen.
            changeentityproperty(vParent, "freezetime", iETime + pauseval);    //Toggle frozen time.
			}      
    }

}