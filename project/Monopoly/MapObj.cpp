#include"MapObj.h"

namespace {
	const wstring TypeName[13] = { L"",
		L"地皮",L"地皮",L"地皮",
		L"鐵皮屋",L"路邊攤",L"聚賭公園",
		L"公寓大樓",L"便利商店",L"地下賭場",
		L"帝寶",L"好市多",L"線上賭場"
	};

	vector<wstring> chance = {
		L"超爽derrrrr 撿到一百塊哩...",
		L"把你阿嬤賣掉，獲取現金 ¥ 3000",
		L"高雄發大財，所有玩家獲得現金 ¥ 2000",
		L"南北菜蟲一起串聯!! 損失 10% 現金",
		L"被傳染了手抖的毛病，一秒鐘會抖6.6下，魔力歸零",
		L"被席主席懷念，精神抖擻，魔力回滿",
		L"基德為你的貓咪討回公道，成功地拿到了一大筆補償費 ¥ 2000",
		L"嘎啦嘎啦，損失 ¥ 2500",
		L"進入平偉的機車行換煞車皮，損失 20% 現金",


		L"參與韓總造勢晚會，聆聽韓總精神，魔力恢復50%",
		L"搭訕小妹妹，被抓進監牢3回合",
		L"遭仙人跳，損失第一桶金  $，魔力歸零",
		L"存款利息，獲得10%總資產",
		L"遇到周年慶，損失50%手頭現金",
		//L"E"
	};
}

pair<wstring, int> MapObj::getChance(Player& player)
{
	int Index = rand() % 9;


	pair<wstring, int> result;
	playChanceMusic(Index);
	switch (Index)
	{
		//撿到100元
	case 0:
		player.payMoney(-100);
		result.second = 3500;
		break;
		//把你阿嬤賣掉
	case 1:
		result.second = 1500;
		player.payMoney(-3000);
		break;
		//高雄發大財
	case 2:
		result.second = 1000;
		for (int i = 0; i < 4; i++)
		{
			//沒有破產
			if (!Player::Instance[i].isbankrupt())
			{
				Player::Instance[i].payMoney(-2000);
			}
		}
		result.second = 1500;
		break;
		//損失 10 % 現金
	case 3:

		result.second = 1500;
		player.payMoney((player.getMoney() * 0.1));
		break;
		//魔力歸零
	case 4:
		result.second = 1500;
		player.setMana(0);
		break;
		//魔力回滿
	case 5:
		result.second = 1000;
		player.setMana(100);
		break;

		//補償費 $2000
	case 6:
		result.second = 2500;
		player.payMoney(-2000);
		break;
		//損失 $2500
	case 7:
		result.second = 1000;
		player.payMoney(2500);
		break;
		//損失20%現金
	case 8:
		result.second = 1000;
		player.payMoney((player.getMoney() * 0.2));
		break;
	}
	result.first = chance[Index];
	return result;
}

MapObj::MapObj()
{
}

MapObj::MapObj(const wstring& input) :lv(0), ownerID(-1)
{
	wstringstream ss(input);
	ss >> pos >> name >> type;
	barrier = 0;
	if (!ss.eof() && type <= 0)
	{
		ss >> barrier;
	}
	if (!ss.eof())
	{
		ss >> val;
		ss >> toll[0] >> toll[1] >> toll[2] >> toll[3];
		if (!ss.eof())
		{
			ss >> barrier;
		}
	}

}
void MapObj::setBarrier(int input)
{
	barrier = input;
}
//設置等級
void MapObj::setLV(int Lv)
{
	this->lv = Lv;
}

//設置所有人
void  MapObj::setOwner(int ownerID)
{
	this->ownerID = ownerID;
}


int MapObj::getBarrier() const
{
	return barrier;
}
//取得該地圖物件的位置
int MapObj::getPos()const
{
	return pos;
}
//取得建築種類
//參數：-2,-1：命運、機會、0：起點、1：民宅、2：商店、3：賭場
int MapObj::getType()const
{
	return type;
}
//取得地價
int MapObj::getVal()const
{
	return val;
}
//取得過路費：參數：傳入欲取得的等級
int MapObj::getToll(int tempLv)const
{
	return toll[tempLv] * ((type == 1) ? 1 : ((type == 2) ? 0.2 : 1.5));
}
int MapObj::getLv()const
{
	return lv;
}
int MapObj::getOwner()const
{
	return ownerID;
}
wstring MapObj::getName()const
{
	return name;
}
//取得建築在不同等級下顯示的名稱
wstring MapObj::getTypeName() const
{
	return TypeName[3 * lv + type];;
}

//取得升級費用
int MapObj::getUpgradeFee() const
{
	if ((type == 1 || type == 2 || type == 3) && lv < 4)
		return (int)(1.5 * toll[lv + 1]);
	return 0;
}

//取得土地總價值
int MapObj::getTotalVal() const
{
	double sum = val;
	for (int i = 1; i <= lv; i++)
	{
		sum += toll[i] * 1.5;
	}
	return (int)sum;
}

//買地
//參數：playerPassBy：變更為誰
void MapObj::Buy(Player& playerPassBy)
{
	ownerID = playerPassBy.getID();
	playerPassBy.addHouse(pos);
	playerPassBy.payMoney(val);
}

//升級某塊地，並付錢
void MapObj::upgrade()
{
	lv++;
	Player::Instance[ownerID].upgradeHouse(pos);
	Player::Instance[ownerID].payMoney(1.5 * toll[lv]);
}

//變更建築種類
void MapObj::changeType(int desiredType)
{
	type = desiredType;
}

int MapObj::payTollPassBy(Player& playerPasserby)
{
	int tempToll = 0;
	if (~ownerID)
	{
		if (ownerID != playerPasserby.getID())
		{
			switch (type)
			{
			case 1:
				tempToll = toll[lv];
				break;
			case 2:
				tempToll = 0.2 * toll[lv];
				break;
			case 3:
				tempToll = 1.5 * toll[lv];
				break;
			}
			playerPasserby.payMoney(tempToll);
			//擁有者獲得金錢
			Player::Instance[ownerID].payMoney(-tempToll);

		}
		return tempToll;
	}
}