#pragma once
struct product {
	char	type[10];
	int		amount;
	char	name[20];
	int		priceSell;
	int		priceBuy;
	char	description[256];
};

struct product LlistaItems[] = {
	{ "Item",5,"Chimaera Wing",70,35,"Returns you to the castle"},
	{ "Item",10,"Dragon scale",20,10,"Increases Defense by 2 when equipped."},
	{ "Item",15,"Holy water",40,7,"Keeps enemies away when used outised of battle. Damages enemies if used during a battle."},
	{ "Item",3,"Magic Key",100,100,"Single-use key used to a locked door before it breaks."},
	{ "Item",20,"Medicinal Herb",25,5,"Restores 15 HP."},
	{ "Item",25,"Torch",8,1,"Temporarily illuminates caves. Its radius is lower compared to the Glow spell. Can be chucked to enemies to inflict fire damage."},
	{ "Armor",4,"Leather suit",70,30,"Defense +4"},
	{ "Armor",2,"Iron armour",1000,700,"Defense +16"},
	{ "Armor",1,"Magic armour",7700,5000,"Defense +24"},
	{ "Armor",1,"Hero’s armour",10000,6000,"Defense +28"},
	{ "Weapon",3,"Oaken club",60,20,"Attack +4"},
	{ "Weapon",6,"Copper sword",180,70,"Attack +10"},
	{ "Weapon",2,"Hand Axe",560,250,"Attack +15"},
	{ "Weapon",1,"Flame Sword",9800,6000,"Attack +28"},
	{ "Weapon",0,"Hero’s Sword",20000,15000,"Attack +40"},
	{ "Shield",5,"Small Shield",90,95,"Defense +4"},
	{ "Shield",2,"Iron Shield",800,350,"Defense +10"},
	{ "Shield",1,"Silver Shield",14000,10000,"Defense +20"}
};