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

//�Ѳ��W��
wstring Stock::getName() const
{
	return name;
}
//�����
double Stock::getTradedValue() const
{
	return tradedValue;
}
//���t
double Stock::getValueDifference() const
{
	return valueDifference;
}
//����q
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
	//�ʶR�q�W�L����q�ο�����������R(�^��0)
	if (amount > tradedNum || player.getMoney() < amount * tradedValue)
	{
		return 0;
	}
	//�p�G�i�H�R ��s�ʶR�������� ����q��� ���a�I��
	player.setStockCost(id, (player.getStockCost(id) * player.getStockNum(id) + amount * tradedValue) / (player.getStockNum(id) + amount));
	tradedNum -= amount;
	player.setMoney(player.getMoney() - amount * tradedValue);
	player.setStockNum(id, player.getStockNum(id) + amount);
	return 1;
}

int Stock::sellStock(Player& player, int amount)
{
	//�ƶq�S���W�L���a�֦��ƶq�~�� ��
	if (amount > player.getStockNum(id))return 0;
	if (amount == player.getStockNum(id))player.setStockCost(id, 0);
	tradedNum += amount;
	player.setMoney(player.getMoney() + amount * tradedValue);
	player.setStockNum(id, player.getStockNum(id) - amount);
	return 1;
}

//��s�ѥ�
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
