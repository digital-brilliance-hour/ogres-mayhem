void main() {
void p = getplayerproperty(3, "entity");
changeentityproperty(p, "maxmp",getglobalvar("maxmp_4P"));
changeentityproperty(p, "mp", 0);
changeentityproperty(p, "maxhealth", getglobalvar("maxhealth_4P"));
changeentityproperty(p, "health", getglobalvar("maxhealth_4P"));
	changeentityproperty(p,"offense",openborconstant("ATK_NORMAL"),getglobalvar("offense_4P"));
	changeentityproperty(p,"defense",openborconstant("ATK_NORMAL"),getglobalvar("defense_4P"));
}
