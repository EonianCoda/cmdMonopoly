#pragma once
#include<cstdlib>
#include<ctime>
#include<string>
#include"Player.h"
using namespace std;

//����� ���^ ����q ���Ѽ� �ʶR����

class Stock
{
public:
	Stock();
	Stock(int initID, wstring initName, double initLastValue, int initLastNum);
	void setStock(int initID, wstring initName, double initLastValue, int initLastNum);
	void setNextStock();

	wstring getName()const;
	double getTradedValue()const;
	double getValueDifference()const;
	int getTradedNum()const;
	int getStatus()const;

	int buyStock(Player& player, int amount);
	int sellStock(Player& player, int amount);

private:
	wstring name;
	double tradedValue, lastTradedValue, valueDifference;
	int tradedNum, lastTradedNum, id;
	int status;//����1 �^��-1 ���`0

	double generateNormalRatio();
	void setTradedNum();
};