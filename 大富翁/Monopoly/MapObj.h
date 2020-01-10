#pragma once
#include<cstdlib>
#include<cwchar>
#include"Player.h"

using namespace std;

class MapObj {
public:
	MapObj();
	MapObj(const wstring& input);

	int getPos()const;
	int getType()const;
	int getVal()const;
	int getToll(int tempLv)const;
	int getLv()const;
	int getOwner()const;
	wstring getName()const;
	wstring getTypeName()const;
	int getUpgradeFee()const;
	int getTotalVal()const;
	int getBarrier() const;

	void setOwner(int ownerID);
	void setLV(int Lv);
	void setBarrier(int input);
	void Buy(Player& playerPassBy);
	void upgrade();
	void changeType(int desiredType);
	int payTollPassBy(Player& playerPasserby);
	pair<wstring,int> getChance(Player&);



protected:
	int pos, type;
	int val, toll[4], lv, ownerID;
	static int loan[4];
	int barrier;
	wstring name;
};