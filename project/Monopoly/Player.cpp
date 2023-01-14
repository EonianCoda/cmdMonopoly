#include "Player.h"
Player* Player::Instance = new Player[4];
int Player::randColor = 9 * rand() / RAND_MAX;
int randRole = rand() % 4;
int randColor = rand() % 8;
namespace {
	const int ColorCode[8] = {
		//(15 + 1 * 16), //��
		(15 + 1 * 16 + 128), //�G��
		(15 + 2 * 16), //��
		(15 + 4 * 16), //��
		(15 + 4 * 16 + 128), //�G��
		(15 + 5 * 16), //��
		(15 + 5 * 16 + 128), //�G��
		(0 + 6 * 16), //��
		(0 + 6 * 16 + 128), //�G��
	};

	const wstring Card[] = {
		L"�ޱ���l�G�i�ޱ���l��X���I��",
		L"���ٳ]�m�G�\�����",
		L"���J���ۡG���w�����a���d�@�^�X",
		L"�M��,�g�a,�ѥ]�G�Ҧ��H���{������",
		L"�T���d�G�C�^�X�i�H�������l�A����T�^�X",
		L"�ۭ��d�G��@�ӤH�մ���m",
	};



	const Skill skill_list[] = {
		//���Y�ޯ�
		{{L"�V���ڡ��e�i",L"�Ĥ@�^�X��l�I��X2�A�ĤG�^�X��l�I��X3�A�ĤT�^�X��l�I��X4"},35,
			{L"�����N�b�e��",L"������e�誺�����m"},35},

			//���Y�ޯ�
			{{L"�ߨ��x�W�A���h�j��",L"�ɯū��w�ؿv"},55,
				{L"�񲴥@�ɡA���A�t�z",L"���ʷ�U��m���Фl"},35},

				//����ѧB�ޯ�
				{{L"�����R��",L"��a���d�@�^�X"},45,
					{L"�h��H���u�f�s��",L"���骱�a����Ȧ�Q��18%"},65},

					//�������ޯ�
					{{L"�ĥ˾|�h",L"�Ϩ�L�H���h�@�^�X���ɶ�"},65,
						{L"�Y�ڪ�������wryyyyy",L"��@���a�i�����a"},60},
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
//�O�_�}��
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

	//�Фl
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

		//�ɴ�
		loan = 0;
		loan_round = 0;

		//�Y�S������h�H���]�w�@�ب���
		role = (randRole++) % 4;


		//�Y�S���C��A�h�H���]�m�@���C��
		color = ColorCode[randColor];
		randColor++;
		randColor %= 8;

		return;
	}
	temp.get();
	//      |�W�r     �Ȧ�s�� ���� �C�� �]�O �{�b���A|�Ѳ�|�d��
	temp >> name >> deposit >> loan >> loan_round >> role >> color >> mana;

	//�{�b���A
	int statusID, stautsTimes;
	status.clear();
	while (!temp.eof() && temp.peek() != L'|')
	{
		temp >> statusID >> stautsTimes;
		setStatus({ statusID ,stautsTimes });
	}temp >> dummy;

	tempID = 0;
	//�Ѳ�
	while (!temp.eof() && temp.peek() != L'|')
	{
		temp >> tempNum >> tempAverage;
		stock[tempID].num = tempNum;
		stock[tempID].averageCost = tempAverage;
		tempID++;
	}temp >> dummy;
	//�d
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

//���o���a�Фl�M��
vector<Item> Player::getHouse() const
{
	return houses;
}
//���o�Фl�`��
int Player::getHouseNum() const
{
	return houses.size();
}

//���o���a�m�W
wstring Player::getName()const
{
	return name;
}
//���o���aID�G��0�}�l
int Player::getID() const
{
	return id;
}
//���o���a��m�G0 ~ 27
int Player::getPos() const
{
	return pos;
}
//���o���a����
int Player::getMoney() const
{
	return money;
}
//���o���a�s��
int Player::getDeposit() const
{
	return deposit;
}
//���o���a���A��
vector<PlayerStatus> Player::getStatus() const
{
	return status;
}


//���o���a����s��
int Player::getRole() const
{
	return role;
}
//���o���a�C��s�X
int Player::getColor() const
{
	return color;
}
//���o��e�]�O��
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

//�]�m���a�W�r
void Player::setName(const wstring& input)
{
	name = input;
}
//�]�m���aID
void Player::setID(int input)
{
	id = input;
}
//�]�m���a��m
void Player::setPos(int input)
{
	pos = input;
}
//�]�m���a������
void Player::setMoney(int input)
{
	money = input;
}
//�]�m���a�s��
void Player::setDeposit(int input)
{
	deposit = input;
}
//�]�m���a���A
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

//�]�m���a����
void Player::setRole(int input)
{
	role = input;
}
//�]�m���a�C��X
void Player::setColor(int input)
{
	color = input;
}
//�]�m���a�]�O��
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
//���ܪ��a���Фl�ݩʤ�������
void Player::upgradeHouse(int housePos)
{
	for (int i = 0; i < houses.size(); i++)
	{
		if (houses[i].ID == housePos)
		{
			//�Y���Ťw�F�̰����A�h�ˬd
			if (houses[i].num == 3)
			{
				cout << "upgradeHouse�G����MAX";
				return;
			}
			houses[i].num++;
			return;
		}
	}
}

//�I�{��(�Y�{�������A�������s��)
void Player::payMoney(int amount)
{
	money -= amount;
	//�Y�{�������I
	if (money < 0)
	{
		//���s��
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

//���s��
void Player::withdrawMoney(int amount)
{
	deposit -= amount;

}

//�W�[���a�֦����Фl�G�Ѽ�pos�F�ĴX��
void Player::addHouse(int pos)
{
	houses.push_back({ pos, 0 });
}

