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
		Sleep, //�ίv���A(�L�k���|)
		Stay, //���۪��A(���d�b��a�L�k����)
		Car, //�ϥΨT��
		Bear_skill,//�V���ڡ����t���e�i
		Bear_The_World, //���J�ĥ˾|�h
		Glasses_sit, //�����R��
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
	//�ɯũФl
	void upgradeHouse(int housePos);
	void clearHouse(int housePos);
	//�I�{��(�Y�{�������A�������s��)
	void payMoney(int amount);
	//���s��
	void withdrawMoney(int amount);
	void addHouse(int pos);
	
private:
	//|�W�r �Ȧ�s�� (�Ȧ�U�� �Ѿl�ٴڤ��)  ���� �C�� �]�O �{�b���A|�Ѳ�|�d��
	wstring name;
	int id, pos, money, deposit, role, color;
	int loan,loan_round;
	//�]�O��
	int mana;
	static int randColor;
	vector<PlayerStatus> status;
	//�O�_�}��
	bool bankrupt;
	vector<Item> houses, card;
	StockData stock[6];
};

