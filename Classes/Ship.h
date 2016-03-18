#ifndef  __SHIP_H__
#define  __SHIP_H__

struct Ship
{
	Ship(const float _mhp, const float _d) : maxHP(_mhp), crnHP(_mhp), dmg(_d){}
	float maxHP;
	float crnHP;
	float dmg;

};

#endif // __SHIP_H__
