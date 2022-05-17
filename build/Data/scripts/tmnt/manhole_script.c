
#import "data/scripts/lib.c"

void main() {
    void self = getlocalvar("self");

    setlocalvar("opp_list",clear_hitbyid_list(self,"opp_list",1));
    //drawstring(10,100,0,getlocalvar("opp_list"));
}

void ondestroy() {
    destroy_hitbyid_list("opp_list");
}
