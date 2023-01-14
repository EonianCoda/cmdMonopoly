#include "Player.h"
Player* Player::Instance = new Player[4];
int Player::randColor = 9 * rand() / RAND_MAX;
int randRole = rand() % 4;
int randColor = rand() % 8;
namespace {
	const int ColorCode[8] = {
		//(15 + 1 * 16), //藍
		(15 + 1 * 16 + 128), //亮藍
		(15 + 2 * 16), //綠
		(15 + 4 * 16), //紅
		(15 + 4 * 16 + 128), //亮紅
		(15 + 5 * 16), //紫
		(15 + 5 * 16 + 128), //亮紫
		(0 + 6 * 16), //黃
		(0 + 6 * 16 + 128), //亮黃
	};

	const wstring Card[] = {
		L"操控骰子：可操控骰子投出的點數",
		L"路障設置：擺放路障",
		L"陷入蕉著：指定角色原地停留一回合",
		L"和平,土地,麵包：所有人的現金均分",
		L"汽車卡：每回合可以投兩顆骰子，持續三回合",
		L"旋風卡：跟一個人調換位置",
	};



	const Skill skill_list[] = {
		//熊頭技能
		{{L"向著蘿○前進",L"第一回合骰子點數X2，第二回合骰子點數X3，第三回合骰子點數X4"},35,
			{L"○莉就在前方",L"瞬移到前方的角色位置"},35},

			//兔頭技能
			{{L"立足台灣，胸懷大陸",L"升級指定建築"},55,
				{L"放眼世界，征服宇宙",L"收購當下位置的房子"},35},

				//眼鏡老伯技能
				{{L"絕食靜坐",L"原地停留一回合"},45,
					{L"退休人員優惠存款",L"全體玩家獲取銀行利息18%"},65},

					//玩偶熊技能
					{{L"薩瓦魯多",L"使其他人失去一回合的時間"},65,
						{L"吃我的壓路機wryyyyy",L"把一塊地夷為平地"},60},
	};

}
const Skill& Player::getSkill() const
{
	return skill_list[role];
}
wstring getCardContent(int index)
{
	return Card[index];
}

Player::Player()
{

}

int Player::getLoan() const
{
	return loan;
}
int Player::getLoan_Round() const
{
	return loan_round;
}

int Player::getCardNum() const
{
	return card.size();
}

const vector<Item>& Player::getCard()const
{
	return card;
}
//是否破產
bool Player::isbankrupt() const
{
	return bankrupt;
}

void Player::setBankrupt(bool input)
{
	bankrupt = input;
}

void Player::SetCard(Item c)
{
	for (int i = 0; i < card.size(); i++)
	{
		if (card[i].ID == c.ID)
		{
			if (c.num == 0)
			{
				card.erase(card.begin() + i);
			}
			else
			{
				card[i].num = c.num;
			}


			return;
		}
	}
	card.push_back(c);
}

void Player::setPlayer(const wstring& input)
{
	bankrupt = false;
	wstringstream temp(input);
	wstring dummy;

	temp >> id >> pos >> money;

	//房子
	int tempID, tempLv, tempNum;
	double tempAverage;



	while (!temp.eof() && temp.peek() != L'|')
	{
		temp >> tempID >> tempLv;
		houses.push_back({ tempID, tempLv });
	}

	if (temp.eof())
	{
		for (auto i : { 0,1,2,3,4,5 })
		{
			stock[i].num = 0;
			stock[i].averageCost = 0;
		}
		name = L"Player" + to_wstring(id + 1);
		deposit = 0;
		status.clear();
		mana = 100;
		card.clear();

		//借款
		loan = 0;
		loan_round = 0;

		//若沒有角色則隨機設定一種角色
		role = (randRole++) % 4;


		//若沒有顏色，則隨機設置一種顏色
		color = ColorCode[randColor];
		randColor++;
		randColor %= 8;

		return;
	}
	temp.get();
	//      |名字     銀行存款 角色 顏色 魔力 現在狀態|股票|卡片
	temp >> name >> deposit >> loan >> loan_round >> role >> color >> mana;

	//現在狀態
	int statusID, stautsTimes;
	status.clear();
	while (!temp.eof() && temp.peek() != L'|')
	{
		temp >> statusID >> stautsTimes;
		setStatus({ statusID ,stautsTimes });
	}temp >> dummy;

	tempID = 0;
	//股票
	while (!temp.eof() && temp.peek() != L'|')
	{
		temp >> tempNum >> tempAverage;
		stock[tempID].num = tempNum;
		stock[tempID].averageCost = tempAverage;
		tempID++;
	}temp >> dummy;
	//卡
	while (!temp.eof() && temp.peek() != L'|')
	{
		temp >> tempID >> tempLv;
		SetCard({ tempID, tempLv });
	}
}

void Player::setLoan(int loan)
{
	this->loan = loan;
}
void Player::setLoan_Round(int loan_round)
{
	this->loan_round = loan_round;
}

Player::~Player()
{
}

//取得玩家房子清單
vector<Item> Player::getHouse() const
{
	return houses;
}
//取得房子總數
int Player::getHouseNum() const
{
	return houses.size();
}

//取得玩家姓名
wstring Player::getName()const
{
	return name;
}
//取得玩家ID：由0開始
int Player::getID() const
{
	return id;
}
//取得玩家位置：0 ~ 27
int Player::getPos() const
{
	return pos;
}
//取得玩家金錢
int Player::getMoney() const
{
	return money;
}
//取得玩家存款
int Player::getDeposit() const
{
	return deposit;
}
//取得玩家狀態值
vector<PlayerStatus> Player::getStatus() const
{
	return status;
}


//取得玩家角色編號
int Player::getRole() const
{
	return role;
}
//取得玩家顏色編碼
int Player::getColor() const
{
	return color;
}
//取得當前魔力值
int Player::getMana() const
{
	return mana;
}

int Player::getStockNum(int stockID) const
{
	return stock[stockID].num;
}

double Player::getStockCost(int stockID) const
{
	return stock[stockID].averageCost;
}

void Player::setStockNum(int stockID, int input)
{
	stock[stockID].num = input;
}

void Player::setStockCost(int stockID, double input)
{
	stock[stockID].averageCost = input;
}

//設置玩家名字
void Player::setName(const wstring& input)
{
	name = input;
}
//設置玩家ID
void Player::setID(int input)
{
	id = input;
}
//設置玩家位置
void Player::setPos(int input)
{
	pos = input;
}
//設置玩家金錢數
void Player::setMoney(int input)
{
	money = input;
}
//設置玩家存款
void Player::setDeposit(int input)
{
	deposit = input;
}
//設置玩家狀態
void Player::setStatus(const PlayerStatus& s)
{
	for (int i = 0; i < status.size(); i++)
	{
		if (status[i].status == s.status)
		{
			if (s.statusTimes == 0)
			{
				status.erase(status.begin() + i);
			}
			else
			{
				status[i].statusTimes = s.statusTimes;
			}


			return;
		}
	}
	status.push_back(s);
	sort(status.begin(), status.end(), [](const PlayerStatus& a, const PlayerStatus& b) {
		return a.statusTimes < b.statusTimes;
		});
}

//設置玩家角色
void Player::setRole(int input)
{
	role = input;
}
//設置玩家顏色碼
void Player::setColor(int input)
{
	color = input;
}
//設置玩家魔力值
void Player::setMana(int input)
{
	mana = input;
}





void Player::setHouse(const Item& item)
{
	for (auto& i : houses)
	{
		if (i.ID == item.ID)
		{
			i.num = item.num;
			return;
		}
	}
	houses.push_back(item);
}
//改變玩家的房子屬性中的等級
void Player::upgradeHouse(int housePos)
{
	for (int i = 0; i < houses.size(); i++)
	{
		if (houses[i].ID == housePos)
		{
			//若等級已達最高等，則檢查
			if (houses[i].num == 3)
			{
				cout << "upgradeHouse：等級MAX";
				return;
			}
			houses[i].num++;
			return;
		}
	}
}

//付現金(若現金不足，直接扣存款)
void Player::payMoney(int amount)
{
	money -= amount;
	//若現金不夠付
	if (money < 0)
	{
		//扣存款
		deposit += money;
		money = 0;
		if (deposit < 0)
		{
			bankrupt = true;
		}
	}
}

void Player::clearHouse(int housePos)
{
	for (int i = 0; i < houses.size(); i++)
	{
		if (houses[i].ID == housePos)
		{
			houses.erase(houses.begin() + i);
			return;
		}
	}
}

//扣存款
void Player::withdrawMoney(int amount)
{
	deposit -= amount;

}

//增加玩家擁有的房子：參數pos；第幾格
void Player::addHouse(int pos)
{
	houses.push_back({ pos, 0 });
}

