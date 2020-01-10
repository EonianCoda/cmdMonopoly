#include "StockMarket.h"

Stock::Stock() :id(-1), name(L""), tradedValue(0), lastTradedValue(0), tradedNum(0), lastTradedNum(0), valueDifference(0)
{
}

Stock::Stock(int initID, wstring initName, double initLastValue, int initLastNum) : id(initID), name(initName), tradedValue(0), lastTradedValue(initLastValue), tradedNum(0), lastTradedNum(initLastNum), valueDifference(0), status(0)
{
}

void Stock::setStock(int initID, wstring initName, double initLastValue, int initLastNum)
{
	id = initID;
	name = initName;
	tradedValue = 0;
	lastTradedValue = initLastValue;
	tradedNum = 0;
	lastTradedNum = initLastNum;
	valueDifference = 0;
}

//股票名稱
wstring Stock::getName() const
{
	return name;
}
//成交價
double Stock::getTradedValue() const
{
	return tradedValue;
}
//價差
double Stock::getValueDifference() const
{
	return valueDifference;
}
//成交量
int Stock::getTradedNum() const
{
	return tradedNum;
}

int Stock::getStatus() const
{
	return status;
}

int Stock::buyStock(Player& player, int amount)
{
	//購買量超過交易量或錢不夠都不能買(回傳0)
	if (amount > tradedNum || player.getMoney() < amount * tradedValue)
	{
		return 0;
	}
	//如果可以買 更新購買平均價格 交易量減少 玩家付錢
	player.setStockCost(id, (player.getStockCost(id) * player.getStockNum(id) + amount * tradedValue) / (player.getStockNum(id) + amount));
	tradedNum -= amount;
	player.setMoney(player.getMoney() - amount * tradedValue);
	player.setStockNum(id, player.getStockNum(id) + amount);
	return 1;
}

int Stock::sellStock(Player& player, int amount)
{
	//數量沒有超過玩家擁有數量才能 賣
	if (amount > player.getStockNum(id))return 0;
	if (amount == player.getStockNum(id))player.setStockCost(id, 0);
	tradedNum += amount;
	player.setMoney(player.getMoney() + amount * tradedValue);
	player.setStockNum(id, player.getStockNum(id) - amount);
	return 1;
}

//更新股市
void Stock::setNextStock()
{
	double ratio = generateNormalRatio();
	setTradedNum();

	if (ratio >= 0.1)
	{
		tradedValue = lastTradedValue * 1.1;
		valueDifference = tradedValue - lastTradedValue;
		lastTradedValue = tradedValue;
		status = 1;
	}
	else if (ratio <= -0.1)
	{
		tradedValue = lastTradedValue * 0.9;
		valueDifference = tradedValue - lastTradedValue;
		lastTradedValue = tradedValue;
		status = -1;
	}
	else
	{
		tradedValue = lastTradedValue * (1 + ratio);
		valueDifference = tradedValue - lastTradedValue;
		lastTradedValue = tradedValue;
		status = 0;
	}

}

double Stock::generateNormalRatio()
{
	double temp = 0;
	for (int i = 0; i < 50; i++)
	{
		temp += 2 * (double)rand() / RAND_MAX;
	}
	temp /= 50;
	temp -= 1;
	return temp;
}
void Stock::setTradedNum()
{
	tradedNum = 0;
	for (int i = 0; i < 20; i++)
	{
		tradedNum += 1000 * rand() / RAND_MAX;
	}
	tradedNum /= 20;
	tradedNum = tradedNum - 500 + lastTradedNum;
	lastTradedNum = tradedNum;
}
