#pragma once
#include<string>
#include<vector>
#include<map>
#include<utility>
#include<sstream>
#include<iostream>
#include<algorithm>
#include"Music.h"
using namespace std;

namespace Motion {
	enum PlayerStatus {
		Sleep, //睡眠狀態(無法收稅)
		Stay, //膠著狀態(停留在原地無法移動)
		Car, //使用汽車
		Bear_skill,//向著蘿○全速的前進
		Bear_The_World, //陷入薩瓦魯多
		Glasses_sit, //絕食靜坐
	};
}

typedef struct { int ID; int num; } Item;
typedef struct { int num; double averageCost; } StockData;
typedef struct {
	int status;
	int statusTimes;
}PlayerStatus;
typedef struct{
	pair<wstring, wstring> firstSkill;
	int firstSpend;
	pair<wstring, wstring> SecondSkill;
	int SecondSpend;
}Skill;


wstring getCardContent(int index);

class Player {
public:
	static Player* Instance;
	Player();
	~Player();
	void setPlayer(const wstring& input);

	wstring getName()const;
	int getID()const;
	int getPos()const;
	int getMoney()const;
	int getDeposit()const;

	vector<PlayerStatus> getStatus()const;

	int getRole()const;
	int getColor()const;
	int getMana()const;
	int getHouseNum() const;
	int getStockNum(int stockID)const;
	double getStockCost(int stockID)const;
	vector<Item> getHouse() const;
	int getCardNum() const;
	const vector<Item>& getCard()const;
	const Skill&  getSkill() const;
	int getLoan() const;
	int getLoan_Round() const;
	bool isbankrupt() const;

	void setName(const wstring& input);
	void setID(int input);
	void setPos(int input);
	void setMoney(int input);
	void setDeposit(int input);
	void setStatus(const PlayerStatus &);
	void setHouse(const Item &);
	void setRole(int input);
	void setColor(int input);
	void setMana(int input);
	void SetCard(Item);
	void setStockNum(int stockID, int input);
	void setStockCost(int stockID, double input);
	void setBankrupt(bool input);
	void setLoan(int);
	void setLoan_Round(int);
	//升級房子
	void upgradeHouse(int housePos);
	void clearHouse(int housePos);
	//付現金(若現金不足，直接扣存款)
	void payMoney(int amount);
	//扣存款
	void withdrawMoney(int amount);
	void addHouse(int pos);
	
private:
	//|名字 銀行存款 (銀行貸款 剩餘還款日期)  角色 顏色 魔力 現在狀態|股票|卡片
	wstring name;
	int id, pos, money, deposit, role, color;
	int loan,loan_round;
	//魔力值
	int mana;
	static int randColor;
	vector<PlayerStatus> status;
	//是否破產
	bool bankrupt;
	vector<Item> houses, card;
	StockData stock[6];
};

