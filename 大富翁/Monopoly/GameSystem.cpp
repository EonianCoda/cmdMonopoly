#include "GameSystem.h"

namespace {
	pair<Coordinate, Coordinate> Button[] = {
		{ {18, 31},{50, 35} }, //投擲骰子
	};
}

GameSystem* GameSystem::Instance = NULL;
HANDLE GameSystem::hInput = GetStdHandle(STD_INPUT_HANDLE);

GameSystem* GameSystem::getInstance()
{
	if (GameSystem::Instance == NULL)
	{
		GameSystem::Instance = new GameSystem;
	}
	return GameSystem::Instance;
}


GameSystem::GameSystem() : playerNum(0), nowPlayer(0), Round(0), mapName(L"")
{
	srand(time(NULL));
	for (int i = 0; i < 28; i++)
	{
		Map[i] = NULL;
	}

	//測試用
	screen = Display::Screen::MainMenu;
	nowStatus = System::Status::MainBeginning;


	//設定console的模式以進行滑鼠事件的操作
	DWORD prev_mode;
	GetConsoleMode(GameSystem::hInput, &prev_mode);
	SetConsoleMode(GameSystem::hInput, (ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE)) | ENABLE_MOUSE_INPUT);
}


GameSystem::~GameSystem()
{
	for (int i = 0; i < 28; i++)
	{
		if (Map[i] != NULL)
		{
			delete Map[i];
		}
	}
}

//將地圖、玩家資料、系統資訊初始化
void GameSystem::initGame()
{
	for (int i = 0; i < 28; i++)
	{
		if (Map[i] != NULL)
		{
			delete Map[i];
		}
	}

	playerNum = 0;
	nowPlayer = 0;
	Round = 0;
	mapName = L"";
}

void GameSystem::MouseInput()
{
	static INPUT_RECORD record;
	static DWORD length;
	ReadConsoleInput(GameSystem::hInput, &record, 1, &length);
	int X = record.Event.MouseEvent.dwMousePosition.X;
	int Y = record.Event.MouseEvent.dwMousePosition.Y;

	//按下ESC鍵
	if (record.EventType == KEY_EVENT && record.Event.KeyEvent.uChar.AsciiChar == 27)
	{

		if (screen == Display::Screen::MainMenu)
		{
			if (nowStatus == System::Status::MainSelectMode)
			{
				nowStatus = System::Status::MainBeginning;
				UI::getInstance()->ClearMainBottom();
				UI::getInstance()->ShowBlock(screen, Display::Block::MainBeginning);
			}
			else if (nowStatus == System::Status::MainSelectFile)
			{
				nowStatus = System::Status::MainSelectMode;
				UI::getInstance()->ClearMainBottom();
				UI::getInstance()->ShowBlock(screen, Display::Block::MainSelectMode);
				UI::getInstance()->gotoXY(0, 50);
			}
			else if (nowStatus == System::Status::MainNewMap)
			{
				nowStatus = System::Status::MainSelectMode;
				UI::getInstance()->ClearMainBottom();
				UI::getInstance()->ShowBlock(screen, Display::Block::MainSelectMode);
				Sleep(100);
			}
			else if (nowStatus == System::Status::MainNewCharater)
			{
				nowStatus = System::Status::MainNewMap;
				UI::getInstance()->ClearMainBanner();
				UI::getInstance()->ClearMainBottom();
				UI::getInstance()->ShowBlock(screen, Display::Block::MainBanner);
				UI::getInstance()->ShowBlock(screen, Display::Block::MainNewMap);
				scanMapFiles();
				if (mapFiles.empty())
				{
					UI::getInstance()->gotoXY(59, 30);
					wcout << L"這裡什麼都沒有";
					UI::getInstance()->gotoXY(60, 32);
					wcout << L"快去玩吧！！";
				}
				else
				{
					for (int i = 0; i < mapFiles.size(); i++)
					{
						UI::getInstance()->gotoXY(25 + (i / 9) * 21, 26 + 2 * (i % 9));
						wfstream fs("MapData//" + mapFiles[i] + ".txt");
						wstring temp;
						fs >> temp;
						wcout << temp;
						fs.close();
					}
				}
				UI::getInstance()->gotoXY(0, 50);
			}
			else if (nowStatus == System::Status::MainSetNameColor)
			{
				UI::getInstance()->ClearMainNameColor();
				UI::getInstance()->gotoXY(18 + setPlayerNameColor * 25, 5);
				UI::getInstance()->SetColor(Player::Instance[setPlayerNameColor].getColor());
				wcout << L" " << Player::Instance[setPlayerNameColor].getName() << L" ";
				UI::getInstance()->SetColor(15);
				nowStatus = System::Status::MainNewCharater;
				Sleep(100);
			}
			else if (nowStatus == System::Status::MainInputTotalMoney)
			{
				UI::getInstance()->gotoXY(85, 30);
				cout << "        ";
				UI::getInstance()->gotoXY(85, 30);
				cout << Player::Instance[0].getMoney();
				UI::getInstance()->gotoXY(0, 50);
				nowStatus = System::Status::MainNewCharater;
				Sleep(100);
			}
			else if (nowStatus == System::Status::MainInputDays)
			{
				UI::getInstance()->gotoXY(112, 30);
				cout << "        ";
				UI::getInstance()->gotoXY(112, 30);
				cout << Round;
				UI::getInstance()->gotoXY(0, 50);
				nowStatus = System::Status::MainNewCharater;
				Sleep(100);
			}
			else if (nowStatus == System::Status::MainSetSound)
			{
				nowStatus = System::Status::MainBeginning;
				UI::getInstance()->ClearMainBottom();
				UI::getInstance()->ShowBlock(screen, Display::Block::MainBeginning);
			}
			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
		}

		else if (screen == Display::Screen::Game)
		{
			//不能按ESC的情形
			if (nowStatus == System::askBuy ||
				nowStatus == System::askSelectBuilding ||
				nowStatus == System::askLVup ||
				nowStatus == System::Casino ||
				nowStatus == System::IKnow
				)
			{
				return;
			}
			else if (nowStatus == System::Status::Menu)
			{
				ContinueGame();

				return;
			}
			else if (nowStatus == System::Status::GameSave)
			{
				UI::getInstance()->SetColor(15);
				for (int i = 0; i < 7; i++)
				{
					UI::getInstance()->DrawHorizontalLine({ 58,21 + i }, 24, L' ');
				}

				UI::getInstance()->SetColor(Color::blue, Color::black, true);
				UI::getInstance()->DrawEllipseButton({ 56,20 }, 25, 9, true);
				UI::getInstance()->SetColor(Color::black, Color::white, false, true);
				UI::getInstance()->gotoXY(65, 24);
				cout << "儲 存 遊 戲";
				nowStatus = System::Status::Menu;
				UI::getInstance()->SetColor(15);
				UI::getInstance()->gotoXY(0, 50);
				Sleep(100);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
				return;
			}

			LastStatus = nowStatus;
			nowStatus = System::Status::Menu;
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowBlock(screen, Display::Block::Menu);
			Sleep(100);
			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
		}

	}
	else if (nowStatus == System::Status::MainSetNameColor)
	{
		if (record.EventType == MOUSE_EVENT && record.Event.MouseEvent.dwButtonState == 1)
		{
			if (Y == 28)
			{
				if (X >= 18 && X <= 29) Player::Instance[setPlayerNameColor].setColor(159);
				else if (X >= 30 && X <= 41) Player::Instance[setPlayerNameColor].setColor(47);
				else if (X >= 42 && X <= 53) Player::Instance[setPlayerNameColor].setColor(79);
				else if (X >= 54 && X <= 65) Player::Instance[setPlayerNameColor].setColor(207);
				else if (X >= 66 && X <= 77) Player::Instance[setPlayerNameColor].setColor(95);
				else if (X >= 78 && X <= 89) Player::Instance[setPlayerNameColor].setColor(223);
				else if (X >= 90 && X <= 101) Player::Instance[setPlayerNameColor].setColor(96);
				else if (X >= 102 && X <= 113) Player::Instance[setPlayerNameColor].setColor(224);
				if (X >= 18 && X <= 113)
				{
					UI::getInstance()->gotoXY(18 + setPlayerNameColor * 25, 5);
					UI::getInstance()->SetColor(Player::Instance[setPlayerNameColor].getColor());
					wcout << L" " << Player::Instance[setPlayerNameColor].getName() << L" ";
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(36 + input.size(), 29);
				}
			}
		}
		char c = record.Event.KeyEvent.uChar.AsciiChar;
		if (record.EventType == KEY_EVENT)
		{
			//一般輸入
			if ((isdigit(c) || c >= 'a' && c <= 'z' || c >= 'a' && c <= 'Z') && input.size() <= 20)
			{
				cout << c;
				input.push_back(c);
			}
			//backspace
			else if (c == 8 && input.size() > 0)
			{
				UI::getInstance()->gotoXY(35 + input.size(), 29);
				cout << ' ';
				UI::getInstance()->gotoXY(35 + input.size(), 29);
				input.pop_back();
				Sleep(100);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			//enter
			else if (c == 13 && input.size() != 0)
			{
				UI::getInstance()->ClearMainNameColor();
				Player::Instance[setPlayerNameColor].setName(stringToWstring(input));
				UI::getInstance()->gotoXY(18 + setPlayerNameColor * 25, 5);
				UI::getInstance()->SetColor(15);
				cout << "                    ";
				UI::getInstance()->gotoXY(18 + setPlayerNameColor * 25, 5);
				UI::getInstance()->SetColor(Player::Instance[setPlayerNameColor].getColor());
				wcout << L" " << Player::Instance[setPlayerNameColor].getName() << L" ";
				UI::getInstance()->SetColor(15);
				nowStatus = System::Status::MainNewCharater;
				UI::getInstance()->gotoXY(0, 50);
			}
		}
		//清除緩衝區
		FlushConsoleInputBuffer(GameSystem::hInput);
	}
	else if (nowStatus == System::Status::MainInputTotalMoney)
	{
		char c = record.Event.KeyEvent.uChar.AsciiChar;
		if (record.EventType == KEY_EVENT)
		{
			//一般輸入
			if (isdigit(c) && input.size() < 7 && !(input.size() == 0 && c == '0'))
			{
				cout << c;
				input.push_back(c);
			}
			//backspace
			else if (c == 8 && input.size() > 0)
			{
				UI::getInstance()->gotoXY(84 + input.size(), 30);
				cout << ' ';
				UI::getInstance()->gotoXY(84 + input.size(), 30);
				input.pop_back();
				Sleep(100);
			}
			//enter
			else if (c == 13 && input.size() > 0 && input.size() <= 9)
			{
				unsigned long long amount = stoull(input);
				Player::Instance[0].setMoney(amount);
				Player::Instance[1].setMoney(amount);
				Player::Instance[2].setMoney(amount);
				Player::Instance[3].setMoney(amount);
				input.clear();
				nowStatus = System::Status::MainNewCharater;
				UI::getInstance()->gotoXY(85, 29);
				cout << "設定成功";
				UI::getInstance()->gotoXY(0, 50);
				Sleep(1000);
				UI::getInstance()->gotoXY(85, 29);
				cout << "        ";
				UI::getInstance()->gotoXY(0, 50);
			}
		}
		//清除緩衝區
		FlushConsoleInputBuffer(GameSystem::hInput);
	}
	else if (nowStatus == System::Status::MainInputDays)
	{
		char c = record.Event.KeyEvent.uChar.AsciiChar;
		if (record.EventType == KEY_EVENT)
		{
			//一般輸入
			if (isdigit(c) && input.size() < 3 && !(input.size() == 0 && c == '0'))
			{
				cout << c;
				input.push_back(c);
			}
			//backspace
			else if (c == 8 && input.size() > 0)
			{
				UI::getInstance()->gotoXY(111 + input.size(), 30);
				cout << ' ';
				UI::getInstance()->gotoXY(111 + input.size(), 30);
				input.pop_back();
				Sleep(100);
			}
			//enter
			else if (c == 13 && input.size() > 0 && input.size() <= 9)
			{
				unsigned long long amount = stoull(input);
				Round = amount;
				input.clear();
				nowStatus = System::Status::MainNewCharater;
				UI::getInstance()->gotoXY(107, 29);
				cout << "設定成功";
				UI::getInstance()->gotoXY(0, 50);
				Sleep(1000);
				UI::getInstance()->gotoXY(107, 29);
				cout << "        ";
				UI::getInstance()->gotoXY(0, 50);
			}
		}
		//清除緩衝區
		FlushConsoleInputBuffer(GameSystem::hInput);
	}
	//買股票
	else if (nowStatus == System::Status::BuyStock)
	{
		if (record.EventType == MOUSE_EVENT && record.Event.MouseEvent.dwButtonState == 1)
		{
			//取消
			if (Y <= 39 && Y >= 37 && X >= 70 && X <= 80)
			{
				nowStatus = System::Status::StockMarket;
				UI::getInstance()->ClearButton();
				UI::getInstance()->ShowBlock(screen, Display::Block::StockMarket);
				input.clear();
				//清除緩衝區
				Sleep(100);
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			//選股
			else if (X >= 18 && X <= 86 && (Y == 18 || Y == 20 || Y == 22 || Y == 24 || Y == 26 || Y == 28))
			{
				selectedStockID = (Y - 18) / 2;
				if (stock[selectedStockID].getStatus() == 1)
				{
					UI::getInstance()->gotoXY(23, 34);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 34);
					UI::getInstance()->SetColor(Color::white, Color::red);
					wcout << L"漲停中，不可購買！";
					UI::getInstance()->gotoXY(0, 50);
					Sleep(1000);
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(23, 34);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
					FlushConsoleInputBuffer(GameSystem::hInput);
					selectedStockID = -1;
					return;
				}
				UI::getInstance()->gotoXY(23, 34);
				wcout << L"                    ";
				UI::getInstance()->gotoXY(23, 34);
				UI::getInstance()->SetColor(Color::white, Color::blue, true);
				wcout << stock[selectedStockID].getName();
				UI::getInstance()->SetColor(15);
				UI::getInstance()->gotoXY(23, 38);
				wcout << L"                                        ";
				UI::getInstance()->gotoXY(23, 38);
				input.clear();
				//清除緩衝區
				Sleep(100);
				FlushConsoleInputBuffer(GameSystem::hInput);
			}

		}
		char c = record.Event.KeyEvent.uChar.AsciiChar;

		if (record.EventType == KEY_EVENT)
		{
			//一般輸入
			if (isdigit(c) && input.size() <= 19 && !(input.size() == 0 && c == '0'))
			{
				cout << c;
				input.push_back(c);
			}
			//backspace
			else if (c == 8 && input.size() > 0)
			{
				UI::getInstance()->gotoXY(22 + input.size(), 38);
				cout << ' ';
				UI::getInstance()->gotoXY(22 + input.size(), 38);
				input.pop_back();
				Sleep(120);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			//enter
			else if (c == 13 && input.size() != 0)
			{
				unsigned long long amount = stoull(input);
				//沒選
				if (selectedStockID == -1)
				{
					UI::getInstance()->gotoXY(23, 38);
					UI::getInstance()->SetColor(Color::red, Color::black);
					wcout << L"你不選個股票是要買沙小？低能兒？";
					UI::getInstance()->gotoXY(0, 50);
					Sleep(1000);
					UI::getInstance()->gotoXY(23, 38);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
					UI::getInstance()->SetColor(15);
				}
				//買太多
				else if (amount > stock[selectedStockID].getTradedNum())
				{
					UI::getInstance()->gotoXY(23, 38);
					UI::getInstance()->SetColor(Color::red, Color::black);
					wcout << L"你想買？沒人賣你啊傻屌！";
					UI::getInstance()->gotoXY(0, 50);
					Sleep(1000);
					UI::getInstance()->gotoXY(23, 38);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
					UI::getInstance()->SetColor(15);
				}
				//沒錢
				else if (amount * stock[selectedStockID].getTradedValue() > Player::Instance[nowPlayer].getDeposit())
				{
					UI::getInstance()->gotoXY(23, 38);
					UI::getInstance()->SetColor(Color::red, Color::black);
					wcout << L"就算在遊戲中，你的帳戶還是沒錢";
					UI::getInstance()->gotoXY(0, 50);
					Sleep(1000);
					UI::getInstance()->gotoXY(23, 38);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
					UI::getInstance()->SetColor(15);
				}
				//可買
				else
				{
					stock[selectedStockID].buyStock(Player::Instance[nowPlayer], amount);
					selectedStockID = -1;

					nowStatus = System::Status::StockMarket;
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBlock(screen, Display::Block::StockMarket);
					UI::getInstance()->drawStockMarket(nowPlayer, stock);
				}
				//清除緩衝區
				input.clear();
				Sleep(100);
			}
		}


	}
	//賣股票
	else if (nowStatus == System::Status::SellStock)
	{
		if (record.EventType == MOUSE_EVENT && record.Event.MouseEvent.dwButtonState == 1)
		{	//取消
			if (Y <= 39 && Y >= 37 && X >= 70 && X <= 80)
			{
				nowStatus = System::Status::StockMarket;
				UI::getInstance()->ClearButton();
				UI::getInstance()->ShowBlock(screen, Display::Block::StockMarket);
				input.clear();
			}
			//選股
			else if (X >= 18 && X <= 86 && (Y == 18 || Y == 20 || Y == 22 || Y == 24 || Y == 26 || Y == 28))
			{
				selectedStockID = (Y - 18) / 2;
				if (stock[selectedStockID].getStatus() == -1)
				{
					UI::getInstance()->gotoXY(23, 34);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 34);
					UI::getInstance()->SetColor(Color::white, Color::green);
					wcout << L"跌停中，不可賣出！";
					UI::getInstance()->gotoXY(0, 50);
					Sleep(1000);
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(23, 34);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
					FlushConsoleInputBuffer(GameSystem::hInput);
					selectedStockID = -1;
					return;
				}
				UI::getInstance()->gotoXY(23, 34);
				wcout << L"                    ";
				UI::getInstance()->gotoXY(23, 34);
				UI::getInstance()->SetColor(Color::white, Color::blue, true);
				wcout << stock[selectedStockID].getName();
				UI::getInstance()->SetColor(15);
				UI::getInstance()->gotoXY(23, 38);
				wcout << L"                                        ";
				UI::getInstance()->gotoXY(23, 38);
				input.clear();
			}
			//清除緩衝區
			Sleep(300);
			FlushConsoleInputBuffer(GameSystem::hInput);
		}

		char c = record.Event.KeyEvent.uChar.AsciiChar;
		if (record.EventType == KEY_EVENT)
		{
			//一般輸入
			if (isdigit(c) && input.size() <= 19 && !(input.size() == 0 && c == '0'))
			{
				cout << c;
				input.push_back(c);
			}
			//backspace
			else if (c == 8 && input.size() > 0)
			{
				UI::getInstance()->gotoXY(22 + input.size(), 38);
				cout << " ";
				UI::getInstance()->gotoXY(22 + input.size(), 38);
				input.pop_back();
				Sleep(120);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			//enter
			else if (c == 13 && input.size() != 0)
			{
				unsigned long long amount = stoull(input);
				//沒選
				if (selectedStockID == -1)
				{
					UI::getInstance()->gotoXY(23, 38);
					UI::getInstance()->SetColor(Color::red, Color::black);
					wcout << L"你不選個股票你要賣沙小？賣台灣？";
					UI::getInstance()->gotoXY(0, 50);
					UI::getInstance()->SetColor(15);
					Sleep(1000);
					UI::getInstance()->gotoXY(23, 38);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
				}
				//賣太多
				else if (amount > Player::Instance[nowPlayer].getStockNum(selectedStockID))
				{
					UI::getInstance()->gotoXY(23, 38);
					UI::getInstance()->SetColor(Color::red, Color::black);
					wcout << L"你賣這谷關空氣？秀燕4ni？";
					UI::getInstance()->gotoXY(0, 50);
					UI::getInstance()->SetColor(15);
					Sleep(1000);
					UI::getInstance()->gotoXY(23, 38);
					wcout << L"                                        ";
					UI::getInstance()->gotoXY(23, 38);
				}
				//可賣
				else
				{
					stock[selectedStockID].sellStock(Player::Instance[nowPlayer], amount);
					selectedStockID = -1;

					nowStatus = System::Status::StockMarket;
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBlock(screen, Display::Block::StockMarket);
					UI::getInstance()->drawStockMarket(nowPlayer, stock);
				}
				//清除緩衝區
				input.clear();
				Sleep(100);
			}
		}

	}
	//存款
	else if (nowStatus == System::Status::SaveMoney || nowStatus == System::Status::WithDraw || nowStatus == System::Status::Loan)
	{
		//返回
		if (record.Event.MouseEvent.dwButtonState == 1 && (Y <= 32 && Y >= 30 && X >= 25 && X <= 34))
		{
			nowStatus = System::Status::Bank;
			UI::getInstance()->ClearButton();
			UI::getInstance()->DrawHorizontalLine({ 20,29 }, 65, L' ');
			UI::getInstance()->DrawHorizontalLine({ 20,30 }, 65, L' ');
			UI::getInstance()->ShowBlock(screen, Display::Block::Bank);
			Sleep(300);
			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
		}
		char c = record.Event.KeyEvent.uChar.AsciiChar;
		if (isdigit(c) && input.size() <= 19 && !(input.size() == 0 && c == '0'))
		{
			cout << c;
			input.push_back(c);
			Sleep(100);
			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
		}
		//backspace鍵
		else if (c == 8 && input.size() > 0)
		{
			UI::getInstance()->gotoXY(42 + input.size(), 38);
			cout << " ";
			UI::getInstance()->gotoXY(42 + input.size(), 38);
			input.pop_back();
			Sleep(100);
			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
		}
		//enter鍵
		else if (c == 13 && input.size() != 0)
		{
			unsigned long long number = stoull(input);
			if (nowStatus == System::Status::Loan)
			{
				//大於一千萬
				if (number > 10000000)
				{
					UI::getInstance()->gotoXY(43, 38);
					for (int i = 0; i < input.size(); i++)
					{
						cout << " ";
					}
					input.clear();
					UI::getInstance()->gotoXY(43, 38);
					UI::getInstance()->SetColor(Color::red, Color::black);
					playMusic(Music::music::Error);
					cout << "借款金額不得超過一千萬，請重新輸入!!";
					UI::getInstance()->SetColor(Color::yellow, Color::black);
					Sleep(1800);
					UI::getInstance()->gotoXY(43, 38);
					cout << "                                    ";
					UI::getInstance()->gotoXY(43, 38);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				else
				{
					Player::Instance[nowPlayer].setLoan(number);
					Player::Instance[nowPlayer].setLoan_Round(3);
					Player::Instance[nowPlayer].payMoney(-1 * number);
					UI::getInstance()->ClearButton();
					UI::getInstance()->DrawHorizontalLine({ 20,29 }, 65, L' ');
					UI::getInstance()->DrawHorizontalLine({ 20,30 }, 65, L' ');
					UI::getInstance()->ShowBlock(screen, Display::Block::Bank);
					UI::getInstance()->UpdatePlayerStatus(nowPlayer);
					nowStatus = System::Status::Bank;
				}
			}
			//錢夠存、錢夠領
			else if ((nowStatus == System::Status::SaveMoney && number <= Player::Instance[nowPlayer].getMoney()) || (nowStatus == System::Status::WithDraw && number <= Player::Instance[nowPlayer].getDeposit()))
			{
				if (nowStatus == System::Status::SaveMoney)
				{
					Player::Instance[nowPlayer].setMoney(Player::Instance[nowPlayer].getMoney() - number);
					Player::Instance[nowPlayer].setDeposit(Player::Instance[nowPlayer].getDeposit() + number);
					UI::getInstance()->UpdatePlayerStatus(nowPlayer);
				}
				else if (nowStatus == System::Status::WithDraw)
				{
					Player::Instance[nowPlayer].setMoney(Player::Instance[nowPlayer].getMoney() + number);
					Player::Instance[nowPlayer].setDeposit(Player::Instance[nowPlayer].getDeposit() - number);
					UI::getInstance()->UpdatePlayerStatus(nowPlayer);
				}
				UI::getInstance()->ClearButton();
				UI::getInstance()->DrawHorizontalLine({ 20,29 }, 65, L' ');
				UI::getInstance()->DrawHorizontalLine({ 20,30 }, 65, L' ');
				UI::getInstance()->ShowBlock(screen, Display::Block::Bank);
				nowStatus = System::Status::Bank;
			}
			else
			{
				UI::getInstance()->gotoXY(43, 38);
				for (int i = 0; i < input.size(); i++)
				{
					cout << " ";
				}
				input.clear();
				UI::getInstance()->gotoXY(45, 38);
				UI::getInstance()->SetColor(Color::red, Color::black);
				playMusic(Music::music::Error);
				cout << "餘額不足，請重新輸入!!";
				UI::getInstance()->SetColor(Color::yellow, Color::black);
				Sleep(1800);
				UI::getInstance()->gotoXY(43, 38);
				cout << "                          ";
				UI::getInstance()->gotoXY(43, 38);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
		}
	}
	//還款
	else if (nowStatus == System::Status::Repay)
	{
		//取消
		if (record.EventType == MOUSE_EVENT && record.Event.MouseEvent.dwButtonState == 1 && (Y <= 32 && Y >= 30 && X >= 25 && X <= 34))
		{
			nowStatus = System::Status::Bank;
			UI::getInstance()->ClearButton();
			UI::getInstance()->DrawHorizontalLine({ 20,29 }, 65, L' ');
			UI::getInstance()->DrawHorizontalLine({ 20,30 }, 65, L' ');
			UI::getInstance()->ShowBlock(screen, Display::Block::Bank);
			Sleep(300);
			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
		}
		else if (record.Event.KeyEvent.uChar.AsciiChar == 13)
		{
			auto& player = Player::Instance[nowPlayer];
			//錢不足以支付
			if (player.getDeposit() + player.getMoney() < player.getLoan())
			{
				UI::getInstance()->SetColor(Color::red, Color::black, true);
				UI::getInstance()->gotoXY(42, 36);
				wcout << L"金錢不足，無法還款           ";
				UI::getInstance()->gotoXY(0, 50);
				playMusic(Music::music::Error);
				Sleep(2000);
				UI::getInstance()->SetColor(15);
			}
			else
			{
				player.payMoney(player.getLoan());
				player.setLoan(0);
				player.setLoan_Round(0);
				UI::getInstance()->UpdatePlayerStatus(nowPlayer);
			}
			nowStatus = System::Status::Bank;
			UI::getInstance()->ClearButton();
			UI::getInstance()->DrawHorizontalLine({ 20,29 }, 65, L' ');
			UI::getInstance()->DrawHorizontalLine({ 20,30 }, 65, L' ');
			UI::getInstance()->ShowBlock(screen, Display::Block::Bank);
			Sleep(300);
			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
		}
	}

	else if (nowStatus == System::Status::GameSave)
	{
		char c = record.Event.KeyEvent.uChar.AsciiChar;
		if (record.EventType == KEY_EVENT)
		{
			//一般輸入
			if ((isdigit(c) || c >= 'a' && c <= 'z' || c >= 'a' && c <= 'Z') && input.size() <= 20)
			{
				cout << c;
				input.push_back(c);
				Sleep(100);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			//backspace
			else if (c == 8 && input.size() > 0)
			{
				UI::getInstance()->gotoXY(60 + input.size(), 25);
				cout << ' ';
				UI::getInstance()->gotoXY(60 + input.size(), 25);
				input.pop_back();
				Sleep(100);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			//enter
			else if (c == 13 && input.size() != 0)
			{
				UI::getInstance()->SetColor(15);
				for (int i = 0; i < 7; i++)
				{
					UI::getInstance()->DrawHorizontalLine({ 58,21 + i }, 24, L' ');
				}
				gameSave(input);
				UI::getInstance()->gotoXY(65, 24);
				cout << "儲 存 成 功";
				Sleep(1000);

				UI::getInstance()->SetColor(Color::blue, Color::black, true);
				UI::getInstance()->DrawEllipseButton({ 56,20 }, 25, 9, true);
				UI::getInstance()->gotoXY(65, 24);
				UI::getInstance()->SetColor(Color::black, Color::white, 0, 1);
				cout << "儲 存 遊 戲";
				nowStatus = System::Status::Menu;
				UI::getInstance()->gotoXY(0, 50);
			}
		}
		//清除緩衝區
		FlushConsoleInputBuffer(GameSystem::hInput);
	}

	else if (record.Event.MouseEvent.dwButtonState == 1)
	{
		UI::getInstance()->gotoXY(0, 49);
		//cout << "(" << Y << "," << X << ")" << endl;
		//dcout << "(" << Y << "," << X << ")" << endl;
		if (screen == Display::Screen::MainMenu)
		{
			if (nowStatus == System::Status::MainBeginning)
			{
				//開始遊戲
				if (X >= 52 && X <= 75 && Y >= 20 && Y <= 25)
				{
					nowStatus = System::Status::MainSelectMode;
					UI::getInstance()->ClearMainBottom();
					UI::getInstance()->ShowBlock(screen, Display::Block::MainSelectMode);
				}
				//音效
				else if (X >= 52 && X <= 75 && Y >= 26 && Y <= 31)
				{
					nowStatus = System::Status::MainSetSound;
					UI::getInstance()->ClearMainBottom();
					UI::getInstance()->ShowBlock(screen, Display::Block::MainSetSound);
					UI::getInstance()->gotoXY(0, 50);
				}
				//離開遊戲
				else if (X >= 52 && X <= 75 && Y >= 32 && Y <= 37)
				{
					UI::getInstance()->SetColor(15);
					system("CLS");
					exit(1);
				}
				Sleep(100);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			else if (nowStatus == System::Status::MainSelectMode)
			{
				//新遊戲
				if (X >= 52 && X <= 75 && Y >= 20 && Y <= 25)
				{
					nowStatus = System::Status::MainNewMap;
					UI::getInstance()->ClearMainBottom();
					UI::getInstance()->ShowBlock(screen, Display::Block::MainNewMap);
					scanMapFiles();
					if (mapFiles.empty())
					{
						UI::getInstance()->gotoXY(59, 30);
						wcout << L"這裡什麼都沒有";
						UI::getInstance()->gotoXY(60, 32);
						wcout << L"快去玩吧！！";
					}
					else
					{
						for (int i = 0; i < mapFiles.size(); i++)
						{
							UI::getInstance()->gotoXY(25 + (i / 9) * 21, 26 + 2 * (i % 9));
							wfstream fs("MapData//" + mapFiles[i] + ".txt");
							wstring temp;
							fs >> temp;
							wcout << temp;
							fs.close();
						}
					}
					UI::getInstance()->gotoXY(0, 50);
				}
				//開啟舊檔
				else if (X >= 52 && X <= 75 && Y >= 26 && Y <= 31)
				{
					nowStatus = System::Status::MainSelectFile;
					UI::getInstance()->ClearMainBottom();
					UI::getInstance()->ShowBlock(screen, Display::Block::MainSelectFile);
					scanSavedFiles();
					if (savedFiles.empty())
					{
						UI::getInstance()->gotoXY(59, 30);
						wcout << L"這裡什麼都沒有";
						UI::getInstance()->gotoXY(60, 32);
						wcout << L"快去玩吧！！";
					}
					else
					{
						for (int i = 0; i < savedFiles.size(); i++)
						{
							UI::getInstance()->gotoXY(25 + (i / 9) * 21, 26 + 2 * (i % 9));
							cout << savedFiles[i];
						}
					}
					UI::getInstance()->gotoXY(0, 50);
				}
				Sleep(100);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			else if (nowStatus == System::Status::MainNewMap)
			{
				if (mapFiles.empty())
				{
					nowStatus = System::Status::MainSelectMode;
					UI::getInstance()->ClearMainBottom();
					UI::getInstance()->ShowBlock(screen, Display::Block::MainSelectMode);
				}
				else if (X >= 25 && X <= 105 && Y >= 26 && Y <= 42)
				{
					int tempX = (X - 25) / 21, tempY = (Y - 26) / 2;
					mapID = tempX * 9 + tempY;
					if (mapID < mapFiles.size())
					{
						fileName = mapFiles[mapID];
						UI::getInstance()->gotoXY(0, 51);
						cout << "                    ";
						UI::getInstance()->gotoXY(0, 51);
						cout << mapFiles[mapID];
						nowStatus = System::Status::MainNewCharater;
						for (int i = 0; i < 4; i++)
						{
							wstring temp = to_wstring(i) + L" 0 30000";
							Player::Instance[i].setPlayer(temp);
						}
						Round = 20;
						playerNum = 4;
						UI::getInstance()->ClearMainBanner();
						UI::getInstance()->ClearMainBottom();
						UI::getInstance()->ShowBlock(screen, Display::Block::MainNewCharater);
					}
				}
				Sleep(100);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			else if (nowStatus == System::Status::MainNewCharater)
			{
				if (Y == 30)
				{
					if (X >= 73 && X <= 91)
					{
						nowStatus = System::Status::MainInputTotalMoney;
						UI::getInstance()->gotoXY(85, 30);
						cout << "        ";
						UI::getInstance()->gotoXY(85, 30);
						input.clear();
					}
					else if (X >= 98 && X <= 114)
					{
						nowStatus = System::Status::MainInputDays;
						UI::getInstance()->gotoXY(112, 30);
						cout << "        ";
						UI::getInstance()->gotoXY(112, 30);
						input.clear();
					}
				}
				//開始遊戲
				else if (X >= 96 && X <= 107 && Y >= 42 && Y <= 44)
				{
					if (!checkMainColorRepeat())
					{
						for (int i = 0; i < 13; i++)
						{
							UI::getInstance()->gotoXY(20, 33 + i);
							cout << "                                                                           ";
						}
						UI::getInstance()->gotoXY(20, 33);
						cout << "顏色請勿重複";
						UI::getInstance()->gotoXY(20, 35);
						cout << "如果你色弱請看醫生";
						UI::getInstance()->gotoXY(20, 37);
						cout << "或是先找朋友幫忙看一下，如果你有朋友...";
						UI::getInstance()->gotoXY(0, 50);
					}
					else
					{
						playerNum = 0;
						for (int i = 0; i < 4; i++)
						{
							if (Player::Instance[i].getRole() != 4) { playerNum++; continue; }
							for (int j = i + 1; j < 4; j++)
							{
								if (Player::Instance[j].getRole() != 4)
								{
									playerNum++;
									wstring tempName = Player::Instance[i].getName();
									int tempRole = Player::Instance[i].getRole(), tempColor = Player::Instance[i].getColor();

									Player::Instance[i].setName(Player::Instance[j].getName());
									Player::Instance[i].setRole(Player::Instance[j].getRole());
									Player::Instance[i].setColor(Player::Instance[j].getColor());
									Player::Instance[j].setName(tempName);
									Player::Instance[j].setRole(tempRole);
									Player::Instance[j].setColor(tempColor);
									break;
								}
							}
						}
						if (!playerNum)
						{
							for (int i = 0; i < 13; i++)
							{
								UI::getInstance()->gotoXY(20, 33 + i);
								cout << "                                                                           ";
							}
							UI::getInstance()->gotoXY(20, 33);
							cout << "好啊都不要角色阿";
							UI::getInstance()->gotoXY(20, 35);
							cout << "你就嫌我們角色醜阿";
							UI::getInstance()->gotoXY(20, 37);
							cout << "都不要玩遊戲啊:D";
							UI::getInstance()->gotoXY(0, 50);
						}
						else
						{
							for (int i = 0; i < 13; i++)
							{
								UI::getInstance()->gotoXY(20, 33 + i);
								cout << "                                                                           ";
							}
							UI::getInstance()->gotoXY(20, 33);
							cout << "遊戲即將開始";
							UI::getInstance()->gotoXY(20, 35);
							cout << "G L H F";
							Sleep(1000);
							UI::getInstance()->SetColor(15);
							system("CLS");

							screen = Display::Screen::Game;
							nowStatus = System::Status::GameTop;
							loadNewMap();

						}
					}
				}
				//玩家0
				else if (X >= 17 && X <= 39)
				{
					if (Y >= 4 && Y <= 6 && Player::Instance[0].getRole() != 4)
					{
						UI::getInstance()->ShowMainNameColor();
						nowStatus = System::Status::MainSetNameColor;
						setPlayerNameColor = 0;
						input.clear();
					}
					else if (Y >= 7 && Y <= 9)
					{
						Player::Instance[0].setRole((Player::Instance[0].getRole() + 1) % 5);
						UI::getInstance()->ShowAvatar(Player::Instance[0].getRole(), 17, 10);
						UI::getInstance()->SetColor(15);

					}
					else if (Y >= 10 && Y <= 24)
					{
						UI::getInstance()->showAvatarIntro(Player::Instance[0].getRole());
					}
					else if (Y >= 25 && Y <= 27)
					{
						if (Player::Instance[0].getRole() == 0)
							Player::Instance[0].setRole(4);
						else
							Player::Instance[0].setRole((Player::Instance[0].getRole() - 1) % 5);
						UI::getInstance()->ShowAvatar(Player::Instance[0].getRole(), 17, 10);
						UI::getInstance()->SetColor(15);
					}
				}
				//玩家1
				else if (X >= 42 && X <= 64)
				{
					if (Y >= 4 && Y <= 6 && Player::Instance[1].getRole() != 4)
					{
						UI::getInstance()->ShowMainNameColor();
						nowStatus = System::Status::MainSetNameColor;
						setPlayerNameColor = 1;
						input.clear();
					}
					else if (Y >= 7 && Y <= 9)
					{
						Player::Instance[1].setRole((Player::Instance[1].getRole() + 1) % 5);
						UI::getInstance()->ShowAvatar(Player::Instance[1].getRole(), 42, 10);
						UI::getInstance()->SetColor(15);
					}
					else if (Y >= 10 && Y <= 24)
					{
						UI::getInstance()->showAvatarIntro(Player::Instance[1].getRole());
					}
					else if (Y >= 25 && Y <= 27)
					{
						if (Player::Instance[1].getRole() == 0)
							Player::Instance[1].setRole(4);
						else
							Player::Instance[1].setRole((Player::Instance[1].getRole() - 1) % 5);
						UI::getInstance()->ShowAvatar(Player::Instance[1].getRole(), 42, 10);
						UI::getInstance()->SetColor(15);
					}
				}
				//玩家2
				else if (X >= 67 && X <= 89)
				{
					if (Y >= 4 && Y <= 6 && Player::Instance[3].getRole() != 4)
					{
						UI::getInstance()->ShowMainNameColor();
						nowStatus = System::Status::MainSetNameColor;
						setPlayerNameColor = 2;
						input.clear();
					}
					else if (Y >= 7 && Y <= 9)
					{
						Player::Instance[2].setRole((Player::Instance[2].getRole() + 1) % 5);
						UI::getInstance()->ShowAvatar(Player::Instance[2].getRole(), 67, 10);
						UI::getInstance()->SetColor(15);

					}
					else if (Y >= 10 && Y <= 24)
					{
						UI::getInstance()->showAvatarIntro(Player::Instance[2].getRole());
					}
					else if (Y >= 25 && Y <= 27)
					{
						if (Player::Instance[2].getRole() == 0)
							Player::Instance[2].setRole(4);
						else
							Player::Instance[2].setRole((Player::Instance[2].getRole() - 1) % 5);
						UI::getInstance()->ShowAvatar(Player::Instance[2].getRole(), 67, 10);
						UI::getInstance()->SetColor(15);
					}
				}
				//玩家3
				else if (X >= 92 && X <= 114)
				{
					if (Y >= 4 && Y <= 6 && Player::Instance[3].getRole() != 4)
					{
						UI::getInstance()->ShowMainNameColor();
						nowStatus = System::Status::MainSetNameColor;
						setPlayerNameColor = 3;
						input.clear();
					}
					else if (Y >= 7 && Y <= 9)
					{
						Player::Instance[3].setRole((Player::Instance[3].getRole() + 1) % 5);
						UI::getInstance()->ShowAvatar(Player::Instance[3].getRole(), 92, 10);
						UI::getInstance()->SetColor(15);

					}
					else if (Y >= 10 && Y <= 24)
					{
						UI::getInstance()->showAvatarIntro(Player::Instance[3].getRole());
					}
					else if (Y >= 25 && Y <= 27)
					{
						if (Player::Instance[3].getRole() == 0)
							Player::Instance[3].setRole(4);
						else
							Player::Instance[3].setRole((Player::Instance[3].getRole() - 1) % 5);
						UI::getInstance()->ShowAvatar(Player::Instance[3].getRole(), 92, 10);
						UI::getInstance()->SetColor(15);
					}
				}
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			else if (nowStatus == System::Status::MainSelectFile)
			{
				if (savedFiles.empty())
				{
					nowStatus = System::Status::MainSelectMode;
					UI::getInstance()->ClearMainBottom();
					UI::getInstance()->ShowBlock(screen, Display::Block::MainSelectMode);

				}
				else if (X >= 25 && X <= 105 && Y >= 26 && Y <= 42)
				{
					int tempX = (X - 25) / 21, tempY = (Y - 26) / 2, temp;
					temp = tempX * 9 + tempY;
					if (temp < savedFiles.size())
					{
						fileName = savedFiles[temp];
						UI::getInstance()->gotoXY(0, 51);
						cout << "                    ";
						UI::getInstance()->gotoXY(0, 51);
						cout << savedFiles[temp];

						UI::getInstance()->SetColor(15);
						system("CLS");
						screen = Display::Screen::Game;
						nowStatus = System::Status::GameTop;
						LoadMap();
						Sleep(100);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
						return;

					}
				}
				Sleep(100);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			else if (nowStatus == System::Status::MainSetSound)
			{
				if (X >= 52 && X <= 75 && Y >= 26 && Y <= 30)
				{
					UI::getInstance()->gotoXY(63, 28);
					if (getMusicMode() == Music::MusicMode::normal)
					{
						wcout << L"關";
						SetMusicMode(Music::MusicMode::quiet);
					}
					else
					{
						wcout << L"開";
						SetMusicMode(Music::MusicMode::normal);
					}
				}
			}
		}
		//遊戲
		else if (screen == Display::Screen::Game)
		{
			//遊戲的最上層(即控制板)
			if (nowStatus == System::Status::GameTop)
			{
				//投擲骰子
				if (X >= 18 && X <= 50 && Y >= 31 && Y <= 35)
				{
					nowStatus = System::Status::RollDice;
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBlock(screen, Display::Block::RollDice);
					//汽車狀態
					for (auto s : Player::Instance[nowPlayer].getStatus())
					{
						if (s.status == Motion::PlayerStatus::Car)
						{
							UI::getInstance()->rollDiceAnimation(0, 0, 1, 2, true);
							UI::getInstance()->rollDiceAnimation(0, 0, 2, 2, true);
							//清除緩衝區
							FlushConsoleInputBuffer(GameSystem::hInput);
							return;
						}
					}
					//只有一顆骰子
					UI::getInstance()->rollDiceAnimation(0, 0, 1, 1, true);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				//股票市場
				else if (Y <= 35 && X >= 55 && X <= 87 && Y >= 31)
				{
					nowStatus = System::Status::StockMarket;
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowBlock(screen, Display::Block::StockMarket);
					UI::getInstance()->SetColor(15);
					UI::getInstance()->drawStockMarket(nowPlayer, stock);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				//使用卡片
				else if (Y <= 40 && X >= 18 && X <= 50 && Y >= 36)
				{
					//沒有卡片
					if ((int)(Player::Instance[nowPlayer].getCard().size()) - 1 < 0)
					{
						UI::getInstance()->gotoXY(29, 38);
						UI::getInstance()->SetColor(Color::red, Color::black, true);
						playMusic(Music::music::Error);
						cout << "沒有卡片！";
						UI::getInstance()->gotoXY(0, 50);
						Sleep(1800);
						UI::getInstance()->SetColor(15);
						UI::getInstance()->gotoXY(29, 38);
						cout << " 使用卡片 ";
						UI::getInstance()->gotoXY(0, 50);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);

					}
					//有卡片
					else
					{
						SumPage = ((Player::Instance[nowPlayer].getCard().size() - 1) / 6) + 1;
						nowPage = 0;
						selectedCard.ID = -1;
						selectedCard.num = 0;
						UI::getInstance()->ClearControl();
						nowStatus = System::Status::UseCard;
						UI::getInstance()->ShowCard(nowPlayer, nowPage);
						UI::getInstance()->ShowBlock(screen, Display::Block::UseCard);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
					}

				}
				//使用技能
				else if (Y <= 40 && X >= 55 && X <= 87 && Y >= 36)
				{
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBlock(screen, Display::Block::UseSkill);
					nowStatus = System::Status::UseSkill;
					UI::getInstance()->ShowUseSkill(nowPlayer);
					Sleep(50);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				else
				{
					DetectInformationClick(X, Y);
				}
			}
	
			//投擲骰子狀態
			else if (nowStatus == System::Status::RollDice)
			{
				//取消
				if (Y <= 33 && Y >= 31 && X >= 71 && X <= 80)
				{
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					nowStatus = System::Status::GameTop;
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				//投骰子
				else
				{
					for (auto s : Player::Instance[nowPlayer].getStatus())
					{
						if (s.status == Motion::PlayerStatus::Car)
						{
							if ((Y <= 40 && Y >= 34 && X >= 37 && X <= 48) || (Y <= 40 && Y >= 34 && X >= 57 && X <= 68))
							{
								//清除取消的按鈕
								UI::getInstance()->SetColor(15);
								UI::getInstance()->DrawHorizontalLine({ 70,31 }, 12, L' ');
								UI::getInstance()->DrawHorizontalLine({ 70,32 }, 12, L' ');
								UI::getInstance()->DrawHorizontalLine({ 70,33 }, 12, L' ');
								//進行玩家移動的動作
								playMusic(Music::music::RollDice);
								PlayerMove(rollDice(2));
							}
							return;
						}
					}
					//只有一顆骰子
					if (Y <= 40 && Y >= 34 && X >= 47 && X <= 58)
					{
						//清除取消的按鈕
						UI::getInstance()->SetColor(15);
						UI::getInstance()->DrawHorizontalLine({ 70,31 }, 12, L' ');
						UI::getInstance()->DrawHorizontalLine({ 70,32 }, 12, L' ');
						UI::getInstance()->DrawHorizontalLine({ 70,33 }, 12, L' ');
						//進行玩家移動的動作
						playMusic(Music::music::RollDice);
						PlayerMove(rollDice(1));
						return;
					}
				}
			}
			//使用技能
			else if (nowStatus == System::Status::UseSkill)
			{
				//取消
				if (Y <= 32 && Y >= 30 && X >= 75 && X <= 84)
				{
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					nowStatus = System::Status::GameTop;
					Sleep(50);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				//第一個技能
				else if (Y <= 40 && Y >= 33 && X >= 18 && X <= 50)
				{
					Player& player = Player::Instance[nowPlayer];
					//魔力不足
					if (player.getMana() < player.getSkill().firstSpend)
					{
						UI::getInstance()->SetColor(Color::white, Color::red, true, true);
						UI::getInstance()->gotoXY(39, 31);
						cout << "      魔  力  不  足 ！！    ";
						UI::getInstance()->gotoXY(0, 50);
						playMusic(Music::music::Error);
						Sleep(1500);

						UI::getInstance()->gotoXY(39, 31);
						UI::getInstance()->SetColor(15);
						cout << "請 點 選 要 使 用 的 技 能 ！";
						UI::getInstance()->gotoXY(0, 50);
						Sleep(50);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
					}
					else
					{
						//發動技能
						UseSkill(0);
						Sleep(50);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
						return;
					}
				}
				//第二個技能
				else if (Y <= 40 && Y >= 33 && X >= 55 && X <= 87)
				{
					Player& player = Player::Instance[nowPlayer];
					//魔力不足
					if (player.getMana() < player.getSkill().SecondSpend)
					{
						UI::getInstance()->SetColor(Color::white, Color::red, true, true);
						UI::getInstance()->gotoXY(39, 31);
						cout << "      魔  力  不  足 ！！    ";
						UI::getInstance()->gotoXY(0, 50);
						playMusic(Music::music::Error);
						Sleep(1500);

						UI::getInstance()->gotoXY(39, 31);
						UI::getInstance()->SetColor(15);
						cout << "請 點 選 要 使 用 的 技 能 ！";
						UI::getInstance()->gotoXY(0, 50);
						Sleep(50);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
					}
					else
					{
						UseSkill(1);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
						return;
					}
				}
			}

			//購買地或升級房子
			else if (nowStatus == System::Status::askBuy || nowStatus == System::Status::askLVup)
			{
				//購買 / 升級
				if (Y <= 40 && Y >= 34 && X >= 22 && X <= 47)
				{
					MapObj* item = Map[Player::Instance[nowPlayer].getPos()];
					//買地
					if (nowStatus == System::Status::askBuy)
					{
						item->Buy(Player::Instance[nowPlayer]);
					}
					//升級
					else if (nowStatus == System::Status::askLVup)
					{
						item->upgrade();
					}
					playMusic(Music::music::SpendMoney);
					//更新右邊的玩家資訊
					UI::getInstance()->UpdatePlayerStatus(nowPlayer);
					UI::getInstance()->UpdateMapObj(*item);
					nowStatus = System::Status::GameTop;
					ChangePlayer();
				}
				//放棄
				else if (Y <= 40 && Y >= 34 && X >= 57 && X <= 82)
				{
					nowStatus = System::Status::GameTop;
					ChangePlayer();
				}
			}
			//選擇建築類型事件
			else if (nowStatus == System::Status::askSelectBuilding)
			{
				//民房
				if (Y <= 40 && Y >= 34 && X >= 19 && X <= 38)
				{
					Map[Player::Instance[nowPlayer].getPos()]->changeType(1);
					Map[Player::Instance[nowPlayer].getPos()]->upgrade();
					nowStatus = System::Status::GameTop;

					//更新右邊的玩家資訊
					UI::getInstance()->UpdatePlayerStatus(nowPlayer);
					UI::getInstance()->UpdateMapObj(*Map[Player::Instance[nowPlayer].getPos()]);
					playMusic(Music::music::SpendMoney);
					//輪到下個玩家
					ChangePlayer();
				}
				//商店
				else if (Y <= 40 && Y >= 34 && X >= 43 && X <= 62)
				{
					Map[Player::Instance[nowPlayer].getPos()]->changeType(2);
					Map[Player::Instance[nowPlayer].getPos()]->upgrade();
					nowStatus = System::Status::GameTop;
					//更新右邊的玩家資訊
					UI::getInstance()->UpdatePlayerStatus(nowPlayer);
					UI::getInstance()->UpdateMapObj(*Map[Player::Instance[nowPlayer].getPos()]);
					playMusic(Music::music::SpendMoney);
					//輪到下個玩家
					ChangePlayer();
				}
				//賭場
				else if (Y <= 40 && Y >= 34 && X >= 67 && X <= 86)
				{
					Map[Player::Instance[nowPlayer].getPos()]->changeType(3);
					Map[Player::Instance[nowPlayer].getPos()]->upgrade();
					nowStatus = System::Status::GameTop;
					//更新右邊的玩家資訊
					UI::getInstance()->UpdatePlayerStatus(nowPlayer);
					UI::getInstance()->UpdateMapObj(*Map[Player::Instance[nowPlayer].getPos()]);
					playMusic(Music::music::SpendMoney);
					//輪到下個玩家
					ChangePlayer();
				}
			}
			//細看某個格子的資訊
			else if (nowStatus == System::Status::MapObj)
			{
				//回選單
				if (Y <= 19 && Y >= 15 && X >= 73 && X <= 86)
				{
					nowStatus = System::Status::GameTop;
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					return;
				}
				this->DetectInformationClick(X, Y);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			//我知道了
			else if (nowStatus == System::Status::IKnow)
			{
				if (Y <= 40 && Y >= 34 && X >= 67 && X <= 86)
				{
					UI::getInstance()->gotoXY(0, 50);
					cout << "我知道了";
					nowStatus = System::Status::GameTop;
					//輪到下個玩家
					ChangePlayer();
				}

			}
			//銀行
			else if (nowStatus == System::Status::Bank)
			{
				//提款
				if (Y <= 39 && Y >= 32 && X >= 19 && X <= 32)
				{
					UI::getInstance()->SetColor(15);
					UI::getInstance()->DrawHorizontalLine({ 37,30 }, 35, L' ');
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBankWork(0, Player::Instance[nowPlayer]);
					nowStatus = System::Status::WithDraw;
					input.clear();
				}
				//存款
				else if (Y <= 39 && Y >= 32 && X >= 36 && X <= 50)
				{
					UI::getInstance()->SetColor(15);
					UI::getInstance()->DrawHorizontalLine({ 37,30 }, 35, L' ');
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBankWork(1, Player::Instance[nowPlayer]);
					nowStatus = System::Status::SaveMoney;
					input.clear();
				}
				//借款
				else if (Y <= 39 && Y >= 33 && X >= 55 && X <= 69)
				{
					//已有借款
					if (Player::Instance[nowPlayer].getLoan() != 0)
					{
						UI::getInstance()->gotoXY(56, 36);
						UI::getInstance()->SetColor(Color::red, Color::black, true);
						cout << "不得重複借款";
						UI::getInstance()->gotoXY(0, 50);
						playMusic(Music::music::Error);
						Sleep(1800);
						UI::getInstance()->SetColor(15);
						UI::getInstance()->gotoXY(56, 36);
						cout << "    借款    ";
						UI::getInstance()->gotoXY(0, 50);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
						return;
					}
					UI::getInstance()->SetColor(15);
					UI::getInstance()->DrawHorizontalLine({ 37,30 }, 35, L' ');
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBankWork(2, Player::Instance[nowPlayer]);
					nowStatus = System::Status::Loan;
					input.clear();
				}
				//還款
				else if (Y <= 39 && Y >= 33 && X >= 72 && X <= 86)
				{
					if (Player::Instance[nowPlayer].getLoan() == 0)
					{
						UI::getInstance()->gotoXY(77, 36);
						UI::getInstance()->SetColor(Color::red, Color::black, true);
						cout << "無借款";
						playMusic(Music::music::Error);
						UI::getInstance()->gotoXY(0, 50);
						Sleep(1300);
						UI::getInstance()->SetColor(15);
						UI::getInstance()->gotoXY(77, 36);
						cout << " 還款 ";
						UI::getInstance()->gotoXY(0, 50);
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
						return;
					}
					UI::getInstance()->SetColor(15);
					UI::getInstance()->DrawHorizontalLine({ 37,30 }, 35, L' ');
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBankWork(3, Player::Instance[nowPlayer]);
					nowStatus = System::Status::Repay;
					input.clear();
				}
				//離開
				else if (Y <= 31 && Y >= 29 && X >= 74 && X <= 83)
				{
					nowStatus = System::Status::RollDice;
				}
			}
			//使用卡片
			else if (nowStatus == System::Status::UseCard)
			{
				static Coordinate pos[6] = {
					{ 19,15 },{ 43,15 },{ 66,15 },{ 19,26 },{ 43,26 },{ 66,26 },
				};
				//選擇卡片
				for (int i = 0; i < Player::Instance[nowPlayer].getCardNum() - (nowPage * 6); i++)
				{
					if (Y <= (pos[i].Y + 10) && Y >= pos[i].Y && X >= pos[i].X && X <= (pos[i].X + 19))
					{
						selectedCard = Player::Instance[nowPlayer].getCard()[nowPage * 6 + i];
						UseCard(selectedCard.ID);

						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
						return;
					}
				}
				//上一頁
				if (Y <= 40 && Y >= 38 && X >= 37 && X <= 46)
				{
					if (nowPage - 1 >= 0)
					{
						nowPage--;
						UI::getInstance()->ShowCard(nowPlayer, nowPage);
					}
					else
					{
						cout << '\a';
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
					}
				}
				//下一頁
				else if (Y <= 40 && Y >= 38 && X >= 60 && X <= 69)
				{
					if (nowPage + 1 < SumPage)
					{
						nowPage++;
						UI::getInstance()->ShowCard(nowPlayer, nowPage);
					}
					else
					{
						cout << '\a';
						//清除緩衝區
						FlushConsoleInputBuffer(GameSystem::hInput);
					}
				}
				//取消
				else if (Y <= 40 && Y >= 38 && X >= 19 && X <= 26)
				{
					nowStatus = System::Status::GameTop;
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);

					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
			}
			//股票市場
			else if (nowStatus == System::Status::StockMarket)
			{
				//取消
				if (Y <= 33 && Y >= 31 && X >= 71 && X <= 80)
				{
					nowStatus = System::Status::GameTop;
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(15, 31);
					cout << "│";
					UI::getInstance()->gotoXY(89, 31);
					cout << "│";
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				//買股
				else if (Y <= 40 && X >= 18 && X <= 50 && Y >= 36)
				{
					UI::getInstance()->ShowBlock(screen, Display::Block::BuyStock);
					nowStatus = System::Status::BuyStock;
					input.clear();
					selectedStockID = -1;
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);

				}
				//賣股
				else if (Y <= 40 && X >= 55 && X <= 87 && Y >= 36)
				{
					UI::getInstance()->ShowBlock(screen, Display::Block::SellStock);
					nowStatus = System::Status::SellStock;
					input.clear();
					selectedStockID = -1;
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);

				}
			}
			//使用控骰卡
			else if (nowStatus == System::Status::Card_Dice)
			{
				//取消
				if (Y <= 17 && Y >= 15 && X >= 76 && X <= 86)
				{
					nowStatus = System::Status::UseCard;
					UI::getInstance()->ClearControl();
					nowStatus = System::Status::UseCard;
					UI::getInstance()->ShowCard(nowPlayer, nowPage);
					UI::getInstance()->ShowBlock(screen, Display::Block::UseCard);
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				//一點
				else if (Y <= 30 && Y >= 26 && X >= 24 && X <= 37)
				{
					UI::getInstance()->ClearControl();
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);

					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					//進行玩家移動的動作
					PlayerMove(1, false);
				}
				//二點
				else if (Y <= 30 && Y >= 24 && X >= 48 && X <= 59)
				{
					UI::getInstance()->ClearControl();
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					//進行玩家移動的動作
					PlayerMove(2, false);
				}
				//三點
				else if (Y <= 30 && Y >= 26 && X >= 69 && X <= 80)
				{
					UI::getInstance()->ClearControl();
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					//進行玩家移動的動作
					PlayerMove(3, false);
				}
				//四點
				else if (Y <= 38 && Y >= 32 && X >= 26 && X <= 37)
				{
					UI::getInstance()->ClearControl();
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					//進行玩家移動的動作
					PlayerMove(4, false);
				}
				//五點
				else if (Y <= 38 && Y >= 32 && X >= 48 && X <= 59)
				{
					UI::getInstance()->ClearControl();
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					//進行玩家移動的動作
					PlayerMove(5, false);
				}
				//六點
				else if (Y <= 38 && Y >= 32 && X >= 69 && X <= 80)
				{
					UI::getInstance()->ClearControl();
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					//進行玩家移動的動作
					PlayerMove(6, false);
				}
			}
			//使用陷入蕉著
			else if (nowStatus == System::Status::Card_banana)
			{
				//取消
				if (Y <= 17 && Y >= 15 && X >= 76 && X <= 86)
				{
					nowStatus = System::Status::UseCard;
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowCard(nowPlayer, nowPage);
					UI::getInstance()->ShowBlock(screen, Display::Block::UseCard);
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				int n = DetectInformationClick(X, Y, false);
				if (n == -5) return;
				//選取玩家
				else if (n < 0)
				{
					n *= -1;
					n--;
					Player::Instance[n].setStatus({ Motion::PlayerStatus::Stay,1 });
					UI::getInstance()->UpdatePlayerStatus(n);
					//用掉卡片
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);
					selectedCard.num = -1;

					//重新顯示GameTop
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					nowStatus = System::Status::GameTop;

					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);

					//選取自己
					if (n == nowPlayer)
					{
						UI::getInstance()->ClearButton();
						PlayerMove(0, false);
					}
				}
			}
			//使用路障卡
			else if (nowStatus == System::Status::Card_barrier)
			{
				//取消
				if (Y <= 17 && Y >= 15 && X >= 76 && X <= 86)
				{
					nowStatus = System::Status::UseCard;
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowCard(nowPlayer, nowPage);
					UI::getInstance()->ShowBlock(screen, Display::Block::UseCard);
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				int n = DetectInformationClick(X, Y, false);
				if (n == -5) return;
				//選取地圖上的方格
				else if (n >= 0)
				{
					//設置路障
					Map[n]->setBarrier(1);
					UI::getInstance()->UpdateMapObj(*Map[n]);

					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					nowStatus = System::Status::GameTop;

					//用掉卡片
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);
					selectedCard.num = -1;

					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
			}
			//使用旋風卡
			else if (nowStatus == System::Status::Card_wind)
			{
				//取消
				if (Y <= 17 && Y >= 15 && X >= 76 && X <= 86)
				{
					nowStatus = System::Status::UseCard;
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowCard(nowPlayer, nowPage);
					UI::getInstance()->ShowBlock(screen, Display::Block::UseCard);
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				int n = DetectInformationClick(X, Y, false);
				if (n == -5) return;
				//選取玩家
				else if (n < 0)
				{
					n *= -1;
					n--;
					//選取自己
					if (n == nowPlayer)
					{
						UI::getInstance()->gotoXY(43, 24);
						UI::getInstance()->SetColor(Color::white, Color::red, true);
						cout << "無法選取自己！請重新選取！";
						UI::getInstance()->gotoXY(0, 50);
						playMusic(Music::music::Error);
						Sleep(1500);
						UI::getInstance()->SetColor(15);
						UI::getInstance()->gotoXY(43, 24);
						cout << "                          ";
						UI::getInstance()->gotoXY(0, 50);
						return;
					}

					int originPos_else = Player::Instance[n].getPos();
					int originPos_now = Player::Instance[nowPlayer].getPos();

					//更新UI上玩家的位置
					UI::getInstance()->UpdatePlayerPos(originPos_else, n, true);
					UI::getInstance()->UpdatePlayerPos(originPos_now, nowPlayer, true);
					UI::getInstance()->UpdatePlayerPos(originPos_now, n);
					UI::getInstance()->UpdatePlayerPos(originPos_else, nowPlayer);

					//交換位置
					Player::Instance[n].setPos(originPos_now);
					Player::Instance[nowPlayer].setPos(originPos_else);


					//用掉卡片
					selectedCard.num--;
					Player::Instance[nowPlayer].SetCard(selectedCard);
					selectedCard.num = -1;

					//重新顯示GameTop
					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					nowStatus = System::Status::GameTop;

					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);

				}
			}
			//立足台灣，胸懷大陸
			else if (nowStatus == System::Status::Skill_upgrade)
			{
				//取消
				if (Y <= 17 && Y >= 15 && X >= 76 && X <= 86)
				{
					nowStatus = System::Status::GameTop;

					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				else
				{
					int n = DetectInformationClick(X, Y, false);
					//為地圖上的格子
					if (n >= 0)
					{
						auto& MapItem = *Map[n];
						int owner = MapItem.getOwner();
						//沒有所有人(無法升級)
						if (owner == -1)
						{
							UI::getInstance()->gotoXY(40, 24);
							UI::getInstance()->SetColor(Color::red, Color::black, true);
							playMusic(Music::music::Error);
							cout << "此 為 黨 有 地 不 可 選 取！！";
							UI::getInstance()->gotoXY(0, 50);
							Sleep(1500);
							UI::getInstance()->gotoXY(40, 24);
							UI::getInstance()->SetColor(15);
							cout << "                                  ";
							UI::getInstance()->gotoXY(0, 50);
							Sleep(50);
							//清除緩衝區
							FlushConsoleInputBuffer(GameSystem::hInput);
						}
						else
						{
							//已達最高等級
							if (MapItem.getLv() == 3)
							{
								UI::getInstance()->gotoXY(40, 24);
								UI::getInstance()->SetColor(Color::red, Color::black, true);
								playMusic(Music::music::Error);
								cout << "已  達  最  高  等  級！！";
								UI::getInstance()->gotoXY(0, 50);
								Sleep(1500);
								UI::getInstance()->gotoXY(39, 24);
								UI::getInstance()->SetColor(15);
								cout << "                              ";

								Sleep(50);
								//清除緩衝區
								FlushConsoleInputBuffer(GameSystem::hInput);
							}
							else
							{
								Player::Instance[owner].upgradeHouse(n);
								MapItem.setLV(MapItem.getLv() + 1);
								UI::getInstance()->UpdateMapObj(MapItem);
								UI::getInstance()->ClearControl();

								//消耗魔力
								Player::Instance[nowPlayer].setMana(Player::Instance[nowPlayer].getMana() - Player::Instance[nowPlayer].getSkill().firstSpend);
								UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
								UI::getInstance()->ShowBlock(screen, Display::Block::Button);
								nowStatus = System::Status::GameTop;
								Sleep(50);
								//清除緩衝區
								FlushConsoleInputBuffer(GameSystem::hInput);
							}
						}
					}
				}
			}
			//吃我的壓路機
			else if (nowStatus == System::Status::Skill_roller)
			{
				//取消
				if (Y <= 17 && Y >= 15 && X >= 76 && X <= 86)
				{
					nowStatus = System::Status::GameTop;

					UI::getInstance()->ClearControl();
					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
				else
				{
					int n = DetectInformationClick(X, Y, false);
					//為地圖上的格子
					if (n >= 0)
					{
						auto& MapItem = *Map[n];
						int owner = MapItem.getOwner();
						//沒有所有人(無法摧毀)
						if (owner == -1)
						{
							UI::getInstance()->gotoXY(40, 24);
							UI::getInstance()->SetColor(Color::red, Color::black, true);
							playMusic(Music::music::Error);
							cout << "此 為 黨 有 地 不 可 摧 毀！！";
							UI::getInstance()->gotoXY(0, 50);
							Sleep(1500);
							UI::getInstance()->gotoXY(39, 24);
							UI::getInstance()->SetColor(15);
							cout << "                              ";
							UI::getInstance()->gotoXY(0, 50);
							Sleep(50);
							//清除緩衝區
							FlushConsoleInputBuffer(GameSystem::hInput);
						}
						else
						{
							//原本就為空地
							if (MapItem.getLv() == 0)
							{
								UI::getInstance()->gotoXY(40, 24);
								UI::getInstance()->SetColor(Color::red, Color::black, true);
								playMusic(Music::music::Error);
								cout << "無  建  築  ， 無  法  摧  毀！！";
								UI::getInstance()->gotoXY(0, 50);
								Sleep(1500);
								UI::getInstance()->gotoXY(39, 24);
								UI::getInstance()->SetColor(15);
								cout << "                                ";

								Sleep(50);
								//清除緩衝區
								FlushConsoleInputBuffer(GameSystem::hInput);
							}
							else
							{
								Player::Instance[owner].setHouse({ n,0 });
								MapItem.setLV(0);
								UI::getInstance()->UpdateMapObj(MapItem);
								UI::getInstance()->ClearControl();

								//消耗魔力
								Player::Instance[nowPlayer].setMana(Player::Instance[nowPlayer].getMana() - Player::Instance[nowPlayer].getSkill().SecondSpend);

								//更新UI
								UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
								UI::getInstance()->ShowBlock(screen, Display::Block::Button);
								nowStatus = System::Status::GameTop;
								Sleep(50);
								//清除緩衝區
								FlushConsoleInputBuffer(GameSystem::hInput);
							}
						}
					}
				}
			}
			//進入賭場
			else if (nowStatus == System::Status::Casino)
			{
				//投骰子
				if (Y <= 40 && Y >= 34 && X >= 47 && X <= 58)
				{
					playMusic(Music::music::RollDice);
					if (rollDice(1) % 2 == 0) WinCasino = true;
					else WinCasino = false;
					nowStatus = System::Status::RollDice;
				}
			}
			//選單
			else if (nowStatus == System::Status::Menu)
			{
				//繼續遊戲
				if (Y <= 27 && Y >= 20 && X >= 24 && X <= 49)
				{
					ContinueGame();
				}
				//儲存遊戲
				else if (Y <= 27 && Y >= 20 && X >= 57 && X <= 82)
				{
					UI::getInstance()->SetColor(15);
					for (int i = 0; i < 7; i++)
					{
						UI::getInstance()->DrawHorizontalLine({ 58,21 + i }, 24, L' ');
					}
					UI::getInstance()->ShowBlock(Display::Screen::Game, Display::Block::GameSave);
					nowStatus = System::Status::GameSave;
				}
				//回主畫面
				else if (Y <= 36 && Y >= 29 && X >= 24 && X <= 49)
				{
					initGame();
					UI::getInstance()->SetColor(15);
					system("CLS");
					//設定console的模式以進行滑鼠事件的操作
					DWORD prev_mode;
					GetConsoleMode(GameSystem::hInput, &prev_mode);
					SetConsoleMode(GameSystem::hInput, (ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE)) | ENABLE_MOUSE_INPUT);
					screen = Display::Screen::MainMenu;
					nowStatus = System::Status::MainBeginning;
					UI::getInstance()->ShowScreen(Display::Screen::MainMenu);

				}
				//離開遊戲
				else if (Y <= 36 && Y >= 29 && X >= 57 && X <= 82)
				{
					UI::getInstance()->SetColor(7);
					system("CLS");
					exit(1);
				}
			}
			//勝利
			else if (nowStatus == System::Status::Win)
			{
				//我知道了
				if (Y <= 40 && Y >= 34 && X >= 67 && X <= 86)
				{
					UI::getInstance()->SetColor(15);
					system("CLS");
					screen = Display::Screen::MainMenu;
					nowStatus = System::Status::MainBeginning;


					//設定console的模式以進行滑鼠事件的操作
					DWORD prev_mode;
					GetConsoleMode(GameSystem::hInput, &prev_mode);
					SetConsoleMode(GameSystem::hInput, (ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE)) | ENABLE_MOUSE_INPUT);

					UI::getInstance()->ShowScreen(Display::Screen::MainMenu);
					Sleep(100);
					//清除緩衝區
					FlushConsoleInputBuffer(GameSystem::hInput);
				}
			}
		}

	}
}

int GameSystem::determinWinner()
{
	int winner = 0;
	double big = 0, sum = 0;
	for (int i = 0; i < playerNum; i++)
	{
		Player& player = Player::Instance[i];
		//沒破產
		if (!player.isbankrupt())
		{
			sum = 0;
			sum += player.getMoney();
			sum += player.getDeposit();
			for (auto s : player.getHouse())
			{
				sum += Map[s.ID]->getTotalVal();
			}
			for (int j = 0; j < 6; j++)
			{
				sum += stock[j].getTradedValue() * player.getStockNum(j);
			}
			if (sum > big)
			{
				winner = i;
				big = sum;
			}
		}
	}

	return winner;
}

void GameSystem::ContinueGame()
{
	UI::getInstance()->ClearControl();
	switch (LastStatus)
	{
	case System::GameTop:
		nowStatus = System::GameTop;
		UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
		UI::getInstance()->ShowBlock(screen, Display::Block::Button);
		break;
	case System::RollDice:
		nowStatus = System::Status::RollDice;
		UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
		UI::getInstance()->ShowBlock(screen, Display::Block::RollDice);
		//汽車狀態
		for (auto s : Player::Instance[nowPlayer].getStatus())
		{
			if (s.status == Motion::PlayerStatus::Car)
			{
				UI::getInstance()->rollDiceAnimation(0, 0, 1, 2, true);
				UI::getInstance()->rollDiceAnimation(0, 0, 2, 2, true);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
				return;
			}
		}
		//只有一顆骰子
		UI::getInstance()->rollDiceAnimation(1, 0, 1, 1, true);
		break;
	case System::UseCard:
		nowStatus = System::Status::UseCard;
		UI::getInstance()->ShowCard(nowPlayer, nowPage);
		UI::getInstance()->ShowBlock(screen, Display::Block::UseCard);
		break;
	case System::UseSkill:
		nowStatus = System::Status::UseSkill;
		UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
		UI::getInstance()->ShowBlock(screen, Display::Block::UseSkill);
		UI::getInstance()->ShowUseSkill(nowPlayer);
		break;


	case System::Bank:
		nowStatus = System::Status::Bank;
		UI::getInstance()->ShowBlock(screen, Display::Block::Bank);

		break;
	case System::WithDraw:
		nowStatus = System::Status::WithDraw;
		UI::getInstance()->ShowBlock(screen, Display::Block::Bank);
		UI::getInstance()->SetColor(15);
		UI::getInstance()->DrawHorizontalLine({ 37,30 }, 35, L' ');
		UI::getInstance()->ClearButton();
		UI::getInstance()->ShowBankWork(0, Player::Instance[nowPlayer]);
		cout << input;

		break;
	case System::SaveMoney:
		nowStatus = System::Status::SaveMoney;
		UI::getInstance()->ShowBlock(screen, Display::Block::Bank);
		UI::getInstance()->SetColor(15);
		UI::getInstance()->DrawHorizontalLine({ 37,30 }, 35, L' ');
		UI::getInstance()->ClearButton();
		UI::getInstance()->ShowBankWork(1, Player::Instance[nowPlayer]);
		cout << input;
		break;
	case System::StockMarket:
		nowStatus = System::Status::StockMarket;
		UI::getInstance()->ShowBlock(screen, Display::Block::StockMarket);
		UI::getInstance()->SetColor(15);
		UI::getInstance()->drawStockMarket(nowPlayer, stock);
		break;
	case System::BuyStock:
		nowStatus = System::Status::BuyStock;
		UI::getInstance()->ShowBlock(screen, Display::Block::StockMarket);
		UI::getInstance()->SetColor(15);
		UI::getInstance()->drawStockMarket(nowPlayer, stock);
		UI::getInstance()->ShowBlock(screen, Display::Block::BuyStock);

		if (selectedStockID != -1)
		{
			UI::getInstance()->gotoXY(22, 34);
			UI::getInstance()->SetColor(Color::white, Color::blue, true);
			wcout << stock[selectedStockID].getName();
		}

		UI::getInstance()->SetColor(15);
		UI::getInstance()->gotoXY(22, 38);
		cout << input;

		break;
	case System::SellStock:
		nowStatus = System::Status::SellStock;
		UI::getInstance()->ShowBlock(screen, Display::Block::StockMarket);
		UI::getInstance()->SetColor(15);
		UI::getInstance()->drawStockMarket(nowPlayer, stock);
		UI::getInstance()->ShowBlock(screen, Display::Block::SellStock);
		if (selectedStockID != -1)
		{
			UI::getInstance()->gotoXY(22, 34);
			UI::getInstance()->SetColor(Color::white, Color::blue, true);
			wcout << stock[selectedStockID].getName();
		}
		UI::getInstance()->SetColor(15);

		UI::getInstance()->gotoXY(22, 38);
		cout << input;

		break;
	case System::Card_Dice:
		nowStatus = System::Card_Dice;
		UseCard(0);
		break;
	case System::Card_banana:
		nowStatus = System::Card_banana;
		UseCard(2);
		break;
	case System::Card_barrier:
		nowStatus = System::Card_banana;
		UseCard(1);
		break;
	case System::Card_wind:
		nowStatus = System::Card_wind;
		UseCard(5);
		break;
	case System::Skill_roller:
		nowStatus = System::Skill_roller;
		UseSkill(1);
		break;
	case System::Skill_upgrade:
		nowStatus = System::Skill_upgrade;
		UseSkill(0);
		break;
	case System::MapObj:
		nowStatus = System::MapObj;
		if (selectedItem < 0)
		{
			int temp = selectedItem * -1;
			temp--;
			UI::getInstance()->ShowPlayerInformation(Player::Instance[temp]);
		}
		else
		{
			UI::getInstance()->ShowMapObjInformation(*Map[selectedItem]);
		}
		break;
	}
	Sleep(50);
	//清除緩衝區
	FlushConsoleInputBuffer(GameSystem::hInput);
}

//使用技能
void GameSystem::UseSkill(int skillID)
{
	Player& player = Player::Instance[nowPlayer];
	int roleID = player.getRole();
	//熊頭
	if (roleID == 0)
	{
		//向著蘿○全速的前進
		if (skillID == 0)
		{
			player.setStatus({ Motion::PlayerStatus::Bear_skill,3 });
			//消耗魔力
			player.setMana(player.getMana() - player.getSkill().firstSpend);
			//更新畫面
			UI::getInstance()->UpdatePlayerStatus(nowPlayer);
			UI::getInstance()->ShowControlPlayer(player);
			UI::getInstance()->ClearButton();
			UI::getInstance()->ShowBlock(screen, Display::Block::Button);
			nowStatus = System::Status::GameTop;
		}
		//○莉就在前方
		else if (skillID == 1)
		{
			int Pos = (player.getPos() + 1) % 28;
			bool Flag = false;
			for (;; Pos = (Pos + 1) % 28)
			{
				for (int i = 0; i < playerNum; i++)
				{
					if (i != nowPlayer && !Player::Instance[i].isbankrupt())
					{
						if (Player::Instance[i].getPos() == Pos)
						{
							Flag = true;
							break;
						}
					}
				}
				if (Flag) break;
			}
			UI::getInstance()->UpdatePlayerPos(player.getPos(), nowPlayer, true);
			UI::getInstance()->UpdatePlayerPos(Pos, nowPlayer);
			player.setPos(Pos);

			//消耗魔力
			player.setMana(player.getMana() - player.getSkill().SecondSpend);
			//更新畫面
			UI::getInstance()->ClearButton();
			UI::getInstance()->ShowBlock(screen, Display::Block::Button);
			UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
			nowStatus = System::Status::GameTop;
		}
	}
	//兔頭
	else if (roleID == 1)
	{
		//立足台灣，胸懷大陸：升級指定建築
		if (skillID == 0)
		{
			UI::getInstance()->ClearControl();

			UI::getInstance()->gotoXY(39, 22);
			UI::getInstance()->SetColor(15);
			cout << "使用技能： 立足";
			UI::getInstance()->SetColor(Color::green, Color::black, true);
			cout << "台灣";
			UI::getInstance()->SetColor(15);
			cout << "，胸懷";
			UI::getInstance()->SetColor(Color::red, Color::black, true);
			cout << "大陸";
			UI::getInstance()->SetColor(15);

			UI::getInstance()->gotoXY(37, 27);

			cout << "請  選  擇  想  升  級  的  建  築";

			UI::getInstance()->gotoXY(37, 30);
			cout << "( 點  選  地  圖  上  的  格  子 )";

			UI::getInstance()->gotoXY(75, 15);
			cout << "┌─────────┐ ";
			UI::getInstance()->gotoXY(75, 16);
			cout << "│   取消  │ ";
			UI::getInstance()->gotoXY(75, 17);
			cout << "└─────────┘ ";
			nowStatus = System::Status::Skill_upgrade;
			UI::getInstance()->gotoXY(0, 50);
		}
		//放眼世界，征服宇宙
		else if (skillID == 1)
		{
			auto& MapItem = *Map[player.getPos()];
			//沒有所有人
			if (MapItem.getOwner() == -1)
			{
				UI::getInstance()->SetColor(Color::white, Color::red, true, true);
				UI::getInstance()->gotoXY(39, 31);
				cout << "    黨 有 地 無 法 收 購！  ";
				UI::getInstance()->gotoXY(0, 50);
				playMusic(Music::music::Error);
				Sleep(1500);

				UI::getInstance()->gotoXY(39, 31);
				UI::getInstance()->SetColor(15);
				cout << "請 點 選 要 使 用 的 技 能 ！";
				UI::getInstance()->gotoXY(0, 50);
				Sleep(50);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
			}
			else
			{
				if (MapItem.getOwner() == nowPlayer)
				{
					UI::getInstance()->SetColor(Color::white, Color::red, true, true);
					UI::getInstance()->gotoXY(39, 31);
					cout << " 無 法 收 購 自 己 的 土 地！";
					UI::getInstance()->gotoXY(0, 50);
					playMusic(Music::music::Error);
					Sleep(1500);

					UI::getInstance()->gotoXY(39, 31);
					UI::getInstance()->SetColor(15);
					cout << "請 點 選 要 使 用 的 技 能 ！";
					UI::getInstance()->gotoXY(0, 50);
				}
				else if (MapItem.getTotalVal() > player.getMoney())
				{
					UI::getInstance()->SetColor(Color::white, Color::red, true, true);
					UI::getInstance()->gotoXY(39, 31);
					cout << "  還 敢 收 購 啊 ! 窮 鬼 !   ";
					UI::getInstance()->gotoXY(0, 50);
					playMusic(Music::music::Error);
					Sleep(1500);

					UI::getInstance()->gotoXY(39, 31);
					UI::getInstance()->SetColor(15);
					cout << "請 點 選 要 使 用 的 技 能 ！";
					UI::getInstance()->gotoXY(0, 50);
				}
				else
				{
					auto house = Player::Instance[MapItem.getOwner()].getHouse();
					Item item;
					for (auto i : house)
					{
						if (i.ID == MapItem.getPos())
						{
							item = i;
							break;
						}
					}
					//消耗魔力
					player.setMana(player.getMana() - player.getSkill().SecondSpend);
					//消耗金錢
					player.payMoney(MapItem.getTotalVal());
					Player::Instance[MapItem.getOwner()].payMoney(-1 * MapItem.getTotalVal());


					UI::getInstance()->UpdatePlayerStatus(MapItem.getOwner());
					Player::Instance[MapItem.getOwner()].clearHouse(MapItem.getPos());
					Player::Instance[MapItem.getOwner()].setHouse(item);
					MapItem.setOwner(nowPlayer);
					UI::getInstance()->UpdateMapObj(MapItem);
					UI::getInstance()->UpdatePlayerStatus(nowPlayer);


					UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
					UI::getInstance()->ClearButton();
					UI::getInstance()->ShowBlock(screen, Display::Block::Button);

					nowStatus = System::Status::GameTop;
				}
			}
		}
	}
	//眼鏡老伯
	else if (roleID == 2)
	{
		//絕食靜坐
		if (skillID == 0)
		{
			//設置絕食靜坐
			Player::Instance[nowPlayer].setStatus({ Motion::PlayerStatus::Glasses_sit,1 });
			//消耗魔力
			player.setMana(player.getMana() - player.getSkill().firstSpend);
			//更新畫面
			UI::getInstance()->UpdatePlayerStatus(nowPlayer);
			UI::getInstance()->ClearButton();
			PlayerMove(0, false);
		}
		//18%
		else if (skillID == 1)
		{
			for (int i = 0; i < playerNum; i++)
			{
				//如果沒有破產
				if ((!Player::Instance[i].isbankrupt()))
				{
					Player::Instance[i].setDeposit(Player::Instance[i].getDeposit() * 1.18);
					UI::getInstance()->UpdatePlayerStatus(i);
				}
			}
			//消耗魔力
			player.setMana(player.getMana() - player.getSkill().SecondSpend);
			//更新畫面
			UI::getInstance()->UpdatePlayerStatus(nowPlayer);
			UI::getInstance()->ShowControlPlayer(player);
			UI::getInstance()->ClearButton();
			UI::getInstance()->ShowBlock(screen, Display::Block::Button);
			nowStatus = System::Status::GameTop;
		}
	}
	//玩偶熊
	else if (roleID == 3)
	{
		//薩瓦魯多
		if (skillID == 0)
		{
			for (int i = 0; i < playerNum; i++)
			{
				//如果沒有破產
				if ((!Player::Instance[i].isbankrupt()) && i != nowPlayer)
				{
					Player::Instance[i].setStatus({ Motion::PlayerStatus::Bear_The_World,1 });
					UI::getInstance()->UpdatePlayerStatus(i);
				}
			}
			//消耗魔力
			player.setMana(player.getMana() - player.getSkill().firstSpend);
			//更新畫面
			UI::getInstance()->UpdatePlayerStatus(nowPlayer);
			UI::getInstance()->ShowControlPlayer(player);
			UI::getInstance()->ClearButton();
			UI::getInstance()->ShowBlock(screen, Display::Block::Button);
			nowStatus = System::Status::GameTop;
		}
		//吃我的壓路機ryyyy
		else if (skillID == 1)
		{
			UI::getInstance()->ClearControl();

			UI::getInstance()->gotoXY(39, 22);
			UI::getInstance()->SetColor(15);
			cout << "使用技能： ";
			UI::getInstance()->SetColor(Color::green, Color::black, true);
			cout << "吃我的壓路機WRRYYYY";

			UI::getInstance()->SetColor(15);
			UI::getInstance()->gotoXY(37, 27);

			cout << "請  選  擇  想  摧  毀  的  建  築";

			UI::getInstance()->gotoXY(37, 30);
			cout << "( 點  選  地  圖  上  的  格  子 )";

			UI::getInstance()->gotoXY(75, 15);
			cout << "┌─────────┐ ";
			UI::getInstance()->gotoXY(75, 16);
			cout << "│   取消  │ ";
			UI::getInstance()->gotoXY(75, 17);
			cout << "└─────────┘ ";
			nowStatus = System::Status::Skill_roller;
			UI::getInstance()->gotoXY(0, 50);
		}
	}
}

//使用卡片
void GameSystem::UseCard(int cardID)
{
	UI::getInstance()->ClearControl();

	//遙控骰子
	if (cardID == 0)
	{
		UI::getInstance()->gotoXY(45, 16);
		UI::getInstance()->SetColor(15);
		cout << "使用卡片：";
		UI::getInstance()->SetColor(Color::white, Color::blue, true);
		cout << "遙控骰子";
		UI::getInstance()->SetColor(15);

		UI::getInstance()->gotoXY(35, 20);

		cout << "請  選  擇  想  擲  出  的  點  數 ";
		UI::getInstance()->gotoXY(75, 15);
		cout << "┌─────────┐ ";
		UI::getInstance()->gotoXY(75, 16);
		cout << "│   取消  │ ";
		UI::getInstance()->gotoXY(75, 17);
		cout << "└─────────┘ ";
		UI::getInstance()->Dice({ 25,24 }, 0, 1, true);
		UI::getInstance()->Dice({ 47,24 }, 1, 1, true);
		UI::getInstance()->Dice({ 68,24 }, 2, 1, true);
		UI::getInstance()->Dice({ 25,32 }, 3, 1, true);
		UI::getInstance()->Dice({ 47,32 }, 4, 1, true);
		UI::getInstance()->Dice({ 68,32 }, 5, 1, true);
		nowStatus = System::Status::Card_Dice;

	}
	//路障
	else if (cardID == 1)
	{
		UI::getInstance()->gotoXY(44, 23);
		UI::getInstance()->SetColor(15);
		cout << "使用卡片：";
		UI::getInstance()->SetColor(Color::white, Color::blue, true);
		cout << "路障設置";
		UI::getInstance()->SetColor(15);

		UI::getInstance()->gotoXY(36, 27);
		wcout << L"請  選  擇  想  放  置  的  位  置";


		UI::getInstance()->gotoXY(37, 30);
		cout << "( 點  選  地  圖  上  的  格  子 )";

		UI::getInstance()->gotoXY(75, 15);
		cout << "┌─────────┐ ";
		UI::getInstance()->gotoXY(75, 16);
		cout << "│   取消  │ ";
		UI::getInstance()->gotoXY(75, 17);
		cout << "└─────────┘ ";
		UI::getInstance()->gotoXY(0, 50);
		nowStatus = System::Status::Card_barrier;
	}
	//陷入蕉著
	else if (cardID == 2)
	{
		UI::getInstance()->gotoXY(44, 23);
		UI::getInstance()->SetColor(15);
		cout << "使用卡片：";
		UI::getInstance()->SetColor(Color::white, Color::blue, true);
		cout << "陷入蕉著";
		UI::getInstance()->SetColor(15);
		_setmode(_fileno(stdout), _O_U16TEXT);

		UI::getInstance()->gotoXY(34, 27);
		wcout << L"請  選  擇  想  蕉  ♂  著  的  對  象";
		_setmode(_fileno(stdout), _O_TEXT);

		UI::getInstance()->gotoXY(39, 30);
		cout << "( 點  選  右  方  角  色  框 )";

		UI::getInstance()->gotoXY(75, 15);
		cout << "┌─────────┐ ";
		UI::getInstance()->gotoXY(75, 16);
		cout << "│   取消  │ ";
		UI::getInstance()->gotoXY(75, 17);
		cout << "└─────────┘ ";
		UI::getInstance()->gotoXY(0, 50);
		nowStatus = System::Status::Card_banana;

	}
	//和平,土地,麵包
	else if (cardID == 3)
	{
		Player& player = Player::Instance[nowPlayer];
		//計算所有人金錢和
		int sum = 0;
		int num = 0;
		for (int i = 0; i < playerNum; i++)
		{
			//若玩家沒有破產
			if (!Player::Instance[i].isbankrupt())
			{
				sum += Player::Instance[i].getMoney();
				num++;
			}
		}
		sum /= num;
		for (int i = 0; i < playerNum; i++)
		{
			if (!Player::Instance[i].isbankrupt())
			{
				Player::Instance[i].setMoney(sum);
				UI::getInstance()->UpdatePlayerStatus(i);

			}
		}
		UI::getInstance()->showUnionFlag();
		playMusic(Music::music::PeaceBread);
		Sleep(5000);

		//UI回歸GameTop
		UI::getInstance()->ClearControl();
		UI::getInstance()->ShowControlPlayer(player);
		UI::getInstance()->ShowBlock(screen, Display::Block::Button);
		nowStatus = System::Status::GameTop;
		//卡片用掉
		selectedCard.num--;
		player.SetCard(selectedCard);
		selectedCard.ID = -1;

	}
	//汽車卡
	else if (cardID == 4)
	{
		Player& player = Player::Instance[nowPlayer];
		//設置駕駛汽車狀態
		player.setStatus({ Motion::PlayerStatus::Car, 3 });
		//UI回歸GameTop
		UI::getInstance()->ClearControl();
		UI::getInstance()->ShowBlock(screen, Display::Block::Button);
		nowStatus = System::Status::GameTop;
		//更新狀態
		UI::getInstance()->ShowControlPlayer(player);
		UI::getInstance()->UpdatePlayerStatus(nowPlayer);

		//卡片用掉
		selectedCard.num--;
		player.SetCard(selectedCard);
		selectedCard.ID = -1;
	}
	//旋風卡
	else if (cardID == 5)
	{
		UI::getInstance()->gotoXY(46, 22);
		UI::getInstance()->SetColor(15);
		cout << "使用卡片：";
		UI::getInstance()->SetColor(Color::white, Color::blue, true);
		cout << "旋風卡";
		UI::getInstance()->SetColor(15);


		UI::getInstance()->gotoXY(34, 27);
		wcout << L"請  選  擇  想  交  換  位  置  的  對  象";

		UI::getInstance()->gotoXY(39, 30);
		cout << "( 點  選  右  方  角  色  框 )";

		UI::getInstance()->gotoXY(75, 15);
		cout << "┌─────────┐ ";
		UI::getInstance()->gotoXY(75, 16);
		cout << "│   取消  │ ";
		UI::getInstance()->gotoXY(75, 17);
		cout << "└─────────┘ ";
		UI::getInstance()->gotoXY(0, 50);
		nowStatus = System::Status::Card_wind;

	}
}

//投骰子
int GameSystem::rollDice(int diceNum)
{
	int n = rand() % 6;
	int result;
	int* lastNumber = new int[diceNum];
	for (int i = 0; i < diceNum; lastNumber[i++] = n);

	for (int i = 0; i < 40; i++)
	{
		result = 0;
		for (int j = 1; j <= diceNum; j++)
		{
			n = rand() % 6;
			UI::getInstance()->rollDiceAnimation(n, lastNumber[j - 1], j, diceNum);
			result += n;
			lastNumber[j - 1] = n;
		}
		Sleep(15);
	}


	delete[] lastNumber;
	//清除緩衝區
	FlushConsoleInputBuffer(GameSystem::hInput);
	return result + diceNum;
}

void GameSystem::PlayerMove(int step, bool sleepable)
{
	nowStatus = System::Status::RollDice;
	int delay = 500;
	//讓骰子的動畫停留
	if (sleepable)
	{
		Sleep(800);
	}


	int originPos = Player::Instance[nowPlayer].getPos();
	Player* player = &Player::Instance[nowPlayer];
	int OutputY = 14;
	bool barrier = false;
	vector<PlayerStatus>::iterator status_result;

	//檢查有沒有受到薩瓦魯多影響
	if (step == 0)
	{
		auto container = player->getStatus();
		//檢查有沒有受到薩瓦魯多影響
		status_result = find_if(container.begin(), container.end(), [](const PlayerStatus& p) {
			return (p.status == Motion::PlayerStatus::Bear_The_World);
			});
		//有受到薩瓦魯多影響
		if (status_result != container.end())
		{
			UI::getInstance()->ClearPlayerInformation();
			UI::getInstance()->ClearButton();

			OutputY += 2;
			UI::getInstance()->gotoXY(45, OutputY);
			UI::getInstance()->SetColor(player->getColor());
			wcout << L" " << player->getName() << L" ";

			UI::getInstance()->SetColor(15);
			wcout << L" 受到";
			UI::getInstance()->SetColor(Color::red, Color::black, true);
			wcout << L" 薩瓦魯多";
			UI::getInstance()->SetColor(15);
			wcout << L" 影響";

			delay *= 2;
			Sleep(delay);
			OutputY += 2;
			UI::getInstance()->gotoXY(45, OutputY);
			wcout << L" 一秒過去了............";

			Sleep(delay);
			OutputY += 2;
			UI::getInstance()->gotoXY(45, OutputY);
			wcout << L" 兩秒過去了............";

			Sleep(delay);
			OutputY += 2;
			UI::getInstance()->gotoXY(45, OutputY);
			wcout << L" 三秒過去了............";

			Sleep(delay);
			OutputY += 2;
			UI::getInstance()->gotoXY(45, OutputY);
			wcout << L" 時間重新開始運轉";

			Sleep(delay / 2);
			nowStatus = System::Status::IKnow;
			UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);

			//讓狀態減少一回合
			for (auto s : Player::Instance[nowPlayer].getStatus())
			{
				s.statusTimes--;
				Player::Instance[nowPlayer].setStatus(s);
			}

			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
			return;
		}
	}
	else if (step != 0 && Player::Instance[nowPlayer].getRole() == 0)
	{
		auto container = player->getStatus();
		//檢查熊頭有沒有發動蘿O就在前方
		status_result = find_if(container.begin(), container.end(), [](const PlayerStatus& p) {
			return (p.status == Motion::PlayerStatus::Bear_skill);
			});
		//有發動蘿O就在前方
		if (status_result != container.end())
		{
			step *= (5 - status_result->statusTimes);
		}
	}




	//檢查有沒有路障
	for (int i = 1, nowPos = originPos; i <= step; i++)
	{
		nowPos = (nowPos + 1) % 28;
		//路障
		if (Map[nowPos]->getBarrier() == 1)
		{
			Map[nowPos]->setBarrier(0);
			barrier = true;
			step = i;
			UI::getInstance()->UpdateMapObj(*Map[nowPos]);
			break;
		}
	}

	//若會經過起點
	if (!barrier && (originPos + step) >= 28)
	{
		UI::getInstance()->ClearControl();
		UI::getInstance()->ShowBlock(screen, Display::Block::Bank);
		nowStatus = System::Status::Bank;
		while (nowStatus != System::Status::RollDice)
		{
			MouseInput();
		}
		UI::getInstance()->ClearControl();
		UI::getInstance()->SetColor(Color::white, Color::black, true);
		UI::getInstance()->DrawHorizontalLine({ 16,30 }, 24, L'─');
		UI::getInstance()->DrawVerticalLine({ 40,15 }, 15, L'│');
		UI::getInstance()->gotoXY(40, 30);
		cout << "┘ ";
		UI::getInstance()->gotoXY(40, 14);
		cout << "┬";
		UI::getInstance()->gotoXY(15, 30);
		cout << "├";
		UI::getInstance()->ShowAvatar(player->getRole(), 17, 15);
	}
	else
	{
		UI::getInstance()->ClearPlayerInformation();
		UI::getInstance()->ClearButton();
	}


	//陷入蕉著
	if (step == 0)
	{
		OutputY += 2;
		UI::getInstance()->gotoXY(45, OutputY);
		UI::getInstance()->SetColor(player->getColor());
		wcout << L" " << player->getName() << L" ";

		UI::getInstance()->SetColor(15);
		for (auto s : player->getStatus())
		{
			if (s.status == Motion::PlayerStatus::Glasses_sit)
			{
				UI::getInstance()->SetColor(Color::green, Color::black, true);
				wcout << L" 絕食靜坐";
				UI::getInstance()->SetColor(15);
				wcout << L" 中";
			}
			else if (s.status == Motion::PlayerStatus::Stay)
			{
				UI::getInstance()->SetColor(Color::red, Color::black, true);
				wcout << L" 陷入蕉著";
				UI::getInstance()->SetColor(15);
				break;
			}
		}

		UI::getInstance()->SetColor(15);
		OutputY += 2;
		UI::getInstance()->gotoXY(45, OutputY);
		wcout << L"停留在 " << Map[originPos]->getName() << L" ";
	}
	//碰到路障
	else if (barrier)
	{
		OutputY += 2;
		UI::getInstance()->gotoXY(45, OutputY);
		UI::getInstance()->SetColor(player->getColor());
		wcout << L" " << player->getName() << L" ";
		UI::getInstance()->SetColor(Color::white, Color::black, true);
		wcout << L" 碰到路障";

		OutputY += 2;
		UI::getInstance()->gotoXY(45, OutputY);
		wcout << L"由 " << Map[originPos]->getName() << L" 移動至 " << Map[(originPos + step) % 28]->getName();
		//角色位置前進
		player->setPos((originPos + step) % 28);
		UI::getInstance()->UpdatePlayerPos(originPos, nowPlayer, true);

		//更新位置
		UI::getInstance()->UpdatePlayerPos(player->getPos(), nowPlayer);
	}
	else
	{
		OutputY += 2;
		UI::getInstance()->gotoXY(45, OutputY);
		UI::getInstance()->SetColor(player->getColor());
		wcout << L" " << player->getName() << L" ";

		UI::getInstance()->SetColor(Color::white, Color::black, true);
		wcout << L" 擲到了 " << step << L" 點";


		Sleep(delay);

		OutputY += 2;
		UI::getInstance()->gotoXY(45, OutputY);
		wcout << L"由 " << Map[originPos]->getName() << L" 移動至 " << Map[(originPos + step) % 28]->getName();

		//角色位置前進
		player->setPos((originPos + step) % 28);
		UI::getInstance()->UpdatePlayerPos(originPos, nowPlayer, true);

		//更新位置
		UI::getInstance()->UpdatePlayerPos(player->getPos(), nowPlayer);
	}



	//現在玩家所在位置的實體
	MapObj* Mapitem = Map[player->getPos()];
	int ownerID = Mapitem->getOwner();
	//機會、命運
	if (Mapitem->getType() < 0)
	{
		Sleep(delay);
		OutputY += 2;
		UI::getInstance()->SetColor(15);
		UI::getInstance()->gotoXY(45, OutputY);
		//執行機會命運
		auto Chance = Mapitem->getChance(*player);
		//輸出機會、命運的字
		_setmode(_fileno(stdout), _O_U16TEXT);
		int length = 0;
		for (int i = 0; i < Chance.first.size(); i++)
		{
			wcout << Chance.first[i];

			length += ((Chance.first[i] <= 122) ? 1 : 2);
			if (length >= 42)
			{
				length = 0;
				UI::getInstance()->gotoXY(45, OutputY + 1);
			}
		}
		_setmode(_fileno(stdout), _O_TEXT);

		Sleep(Chance.second);
		nowStatus = System::Status::IKnow;
		UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
		for (int i = 0; i < playerNum; i++)
		{
			//沒有破產
			if (!Player::Instance[i].isbankrupt())
			{
				UI::getInstance()->UpdatePlayerStatus(i);
			}
		}
		//破產事件觸發
		if (player->getMoney() == 0 && player->getDeposit() <= 0)
		{
			UI::getInstance()->UpdatePlayerStatus(nowPlayer);
			//遊戲結束
			if (Bankrupt())
			{
				Win(determinWinner());
				return;
			}
		}
	}
	//起點
	else if (Mapitem->getType() == 0)
	{
		nowStatus = System::Status::IKnow;
		UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
	}
	else
	{
		//可升級或購買
		if (ownerID == nowPlayer || ownerID == -1)
		{
			//買地事件
			if (ownerID == -1)
			{
				//金錢不足
				if (Mapitem->getVal() > player->getMoney())
				{
					Sleep(delay);
					OutputY += 2;
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(45, OutputY);
					wcout << Mapitem->getName() << L" 沒有所有人，價格為";
					UI::getInstance()->OutputMoney(Mapitem->getVal());

					Sleep(delay);
					OutputY += 2;
					UI::getInstance()->SetColor(Color::red, Color::black, true);
					UI::getInstance()->gotoXY(45, OutputY);
					wcout << L"雖然很便宜，但是你仍然買不起！";

					//叫出我知道了
					Sleep(delay);
					nowStatus = System::Status::IKnow;
					UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
				}
				else
				{
					UI::getInstance()->ShowAskSpend(*Mapitem);
					nowStatus = System::Status::askBuy;
				}
			}
			//選擇建築事件
			else if (ownerID == nowPlayer && Mapitem->getLv() == 0)
			{
				//現金不足
				if (Mapitem->getUpgradeFee() > player->getMoney())
				{
					Sleep(delay);
					OutputY += 2;
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(46, OutputY);
					wcout << Mapitem->getName() << L" 為您的所有地..........";

					Sleep(delay);
					OutputY += 2;
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(46, OutputY);
					UI::getInstance()->SetColor(Color::red, Color::black, true);
					wcout << L"但你沒錢，所以無法升級!";

					//叫出我知道了
					Sleep(delay);
					nowStatus = System::Status::IKnow;
					UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
				}
				else
				{
					UI::getInstance()->ShowAskSpend(*Mapitem);
					nowStatus = System::Status::askSelectBuilding;
				}
			}
			//升級事件
			else
			{
				//金錢不足
				if (Mapitem->getUpgradeFee() > player->getMoney())
				{

					Sleep(delay);
					OutputY += 2;
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(43 + (5 - Mapitem->getName().size()), OutputY);
					wcout << Mapitem->getName() << L" 為您的所有地......";

					Sleep(delay);
					OutputY += 2;
					UI::getInstance()->gotoXY(45, OutputY);
					wcout << L"但你沒錢，所以無法升級!";

					//叫出我知道了
					Sleep(delay);
					nowStatus = System::Status::IKnow;
					UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
				}
				else if (Mapitem->getLv() == 3)
				{

					Sleep(delay);
					OutputY += 2;
					UI::getInstance()->SetColor(15);
					UI::getInstance()->gotoXY(43 + (5 - Mapitem->getName().size()), OutputY);
					wcout << Mapitem->getName() << L" 為您的所有地";

					Sleep(delay);
					OutputY += 2;
					UI::getInstance()->gotoXY(45, OutputY);
					wcout << L"但由於等級已達上限，無法再升級!";

					//叫出我知道了
					Sleep(delay);
					nowStatus = System::Status::IKnow;
					UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
				}
				//詢問是否要升級房子
				else
				{
					UI::getInstance()->ShowAskSpend(*Mapitem);
					nowStatus = System::Status::askLVup;
				}

			}
		}
		//呼叫過路事件
		else
		{
			int originMoney = player->getMoney();
			Player* Owner = &Player::Instance[Mapitem->getOwner()];
			UI::getInstance()->SetColor(15);
			Sleep(delay);
			OutputY += 2;
			UI::getInstance()->gotoXY(45, OutputY);
			wcout << Mapitem->getName() << L" 為 ";
			UI::getInstance()->SetColor(Owner->getColor());
			wcout << L" " << Owner->getName() << L" ";
			UI::getInstance()->SetColor(15);
			wcout << L" 的所有地......";


			//付錢給擁有者
			int pay = Mapitem->payTollPassBy(*player);

			Sleep(delay);
			OutputY += 2;
			UI::getInstance()->gotoXY(45, OutputY);
			UI::getInstance()->SetColor(15);
			wcout << L"需支付";
			UI::getInstance()->OutputMoney(pay);
			wcout << L" 給 ";

			UI::getInstance()->SetColor(Owner->getColor());
			wcout << L" " << Owner->getName() << L" ";


			//賭場
			if (Mapitem->getType() == 3)
			{
				//更新右邊的玩家狀態
				UI::getInstance()->UpdatePlayerStatus(nowPlayer);
				UI::getInstance()->UpdatePlayerStatus(Mapitem->getOwner());
				UI::getInstance()->ShowBlock(screen, Display::Block::Casino);
				nowStatus = System::Status::Casino;
				Sleep(200);
				//清除緩衝區
				FlushConsoleInputBuffer(GameSystem::hInput);
				while (nowStatus == System::Status::Casino)
				{
					MouseInput();
				}
				pay = Mapitem->getToll(Mapitem->getLv()) * 0.75;
				Sleep(1000);
				//贏
				if (WinCasino)
				{
					OutputY += 2;
					player->payMoney(-1 * pay);
					Player::Instance[Mapitem->getOwner()].payMoney(pay);

					player->setMoney(player->getMoney() + Mapitem->getToll(Mapitem->getLv()));
					UI::getInstance()->gotoXY(45, OutputY);
					UI::getInstance()->SetColor(15);
					wcout << L"贏得遊戲，獲得";
					UI::getInstance()->OutputMoney(pay);
				}
				//輸
				else
				{
					player->payMoney(pay);
					Player::Instance[Mapitem->getOwner()].payMoney(-1 * pay);
					OutputY += 2;
					UI::getInstance()->gotoXY(45, OutputY);
					UI::getInstance()->SetColor(15);
					wcout << L"輸掉遊戲，花費";
					UI::getInstance()->OutputMoney(pay);
				}
				//更新右邊的玩家狀態
				UI::getInstance()->UpdatePlayerStatus(nowPlayer);
				UI::getInstance()->UpdatePlayerStatus(Mapitem->getOwner());
				UI::getInstance()->ClearButton();
			}
			else
			{
				//更新右邊的玩家狀態
				UI::getInstance()->UpdatePlayerStatus(nowPlayer);
				UI::getInstance()->UpdatePlayerStatus(Mapitem->getOwner());
			}



			//破產事件觸發
			if (player->getMoney() == 0 && player->getDeposit() <= 0)
			{

				//遊戲結束
				if (Bankrupt())
				{
					Win(determinWinner());
					return;
				}
			}
			//錢夠付
			else
			{
				Sleep(delay);
				nowStatus = System::Status::IKnow;
				UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
			}
		}
	}

	//讓狀態減少一回合
	for (auto s : Player::Instance[nowPlayer].getStatus())
	{
		s.statusTimes--;
		Player::Instance[nowPlayer].setStatus(s);
	}

	//清除緩衝區
	FlushConsoleInputBuffer(GameSystem::hInput);
}

//觸發破產：回傳是否遊戲結束(即其他玩家皆破產)
bool GameSystem::Bankrupt()
{
	Player* player = &Player::Instance[nowPlayer];
	Sleep(1500);

	UI::getInstance()->ClearControl();
	UI::getInstance()->ShowFail({ 26, 15 });
	UI::getInstance()->gotoXY(63, 15);
	cout << "┌─────────────────────┐ ";
	UI::getInstance()->DrawVerticalLine({ 63,16 }, 5, L'│');
	UI::getInstance()->DrawVerticalLine({ 85,16 }, 5, L'│');
	UI::getInstance()->gotoXY(68, 18);
	UI::getInstance()->SetColor(Color::red, Color::black, true);
	wcout << L"恭喜你！破產啦~~";
	UI::getInstance()->SetColor(15);
	UI::getInstance()->gotoXY(63, 21);
	cout << "└─────────────────────┘ ";
	UI::getInstance()->gotoXY(0, 50);
	playMusic(Music::music::Bankrupt);
	Sleep(6000);
	player->setBankrupt(true);
	UI::getInstance()->UpdatePlayerStatus(nowPlayer, false);
	UI::getInstance()->UpdatePlayerPos(player->getPos(), nowPlayer, true);
	player->setDeposit(0);
	for (auto s : player->getHouse())
	{
		Map[s.ID]->setLV(0);
		Map[s.ID]->setOwner(-1);
		UI::getInstance()->UpdateMapObj(*Map[s.ID]);
		player->clearHouse(s.ID);
	}

	int times = 0;
	for (int i = 0; i < playerNum; i++)
	{
		if (Player::Instance[i].isbankrupt()) times++;
	}
	if (times == playerNum - 1) return true;

	nowStatus = System::Status::IKnow;
	UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
	return false;
}

//檢查是否有額外資訊(地圖上的某個地點、某個玩家的資訊)的點擊發生
int GameSystem::DetectInformationClick(int X, int Y, bool achieve)
{
	//玩家1
	if (Y <= 12 && X >= 108 && X <= 131 && !Player::Instance[0].isbankrupt())
	{
		if (achieve)
		{
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowPlayerInformation(Player::Instance[0]);
			nowStatus = System::Status::MapObj;
		}
		selectedItem = -1;
		return -1;
	}
	//玩家2
	else if (Y >= 13 && Y <= 23 && X >= 108 && X <= 131 && playerNum >= 2 && !Player::Instance[1].isbankrupt())
	{
		if (achieve)
		{
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowPlayerInformation(Player::Instance[1]);
			nowStatus = System::Status::MapObj;
		}
		selectedItem = -2;
		return -2;
	}
	//玩家3
	else if (Y >= 24 && Y <= 35 && X >= 108 && X <= 131 && playerNum >= 3 && !Player::Instance[2].isbankrupt())
	{
		if (achieve)
		{
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowPlayerInformation(Player::Instance[2]);
			nowStatus = System::Status::MapObj;
		}
		selectedItem = -3;
		return -3;
	}
	//玩家4
	else if (Y >= 36 && Y <= 48 && X >= 108 && X <= 131 && playerNum >= 4 && !Player::Instance[3].isbankrupt())
	{
		if (achieve)
		{
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowPlayerInformation(Player::Instance[3]);
			nowStatus = System::Status::MapObj;
		}
		selectedItem = -4;
		return -4;
	}
	//上半部地圖
	else if (Y <= 6 && X <= 104 && X >= 1)
	{
		if (achieve)
		{
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowMapObjInformation(*Map[(X - 1) / 13]);
			nowStatus = System::Status::MapObj;
		}
		selectedItem = (X - 1) / 13;
		return (X - 1) / 13;
	}
	//右半部地圖
	else if (X >= 92 && X <= 104 && Y >= 7 && Y <= 48)
	{
		if (achieve)
		{
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowMapObjInformation(*Map[8 + ((Y - 7) / 6)]);
			nowStatus = System::Status::MapObj;
		}
		selectedItem = 8 + ((Y - 7) / 6);
		return 8 + ((Y - 7) / 6);
	}
	//下半部地圖
	else if (Y >= 42 && Y <= 48 && X <= 91 && X >= 1)
	{
		if (achieve)
		{
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowMapObjInformation(*Map[15 + 6 - ((X - 1) / 13)]);
			nowStatus = System::Status::MapObj;
		}
		selectedItem = 15 + 6 - ((X - 1) / 13);
		return 15 + 6 - ((X - 1) / 13);
	}
	//左半部地圖
	else if (X <= 13 && Y >= 7 && Y <= 41 && X >= 1)
	{
		if (achieve)
		{
			UI::getInstance()->ClearControl();
			UI::getInstance()->ShowMapObjInformation(*Map[22 + 5 - ((Y - 7) / 6)]);
			nowStatus = System::Status::MapObj;
		}
		selectedItem = 22 + 5 - ((Y - 7) / 6);
		return 22 + 5 - ((Y - 7) / 6);
	}
	//selectedItem = -5;
	return -5;
}

//讀取地圖
bool GameSystem::LoadMap()
{
	UI::getInstance()->ShowScreen(Display::Screen::Game);
	wifstream input;
	wstring information;
	int type;

	input.imbue(locale(""));
	fileName = "SavedData//" + fileName + ".txt";
	input.open(fileName, ios::in);
	input >> mapName >> Round >> playerNum;

	input.ignore(256, '\n');

	for (int i = 0; i < 28; i++)
	{
		getline(input, information);
		Map[i] = new MapObj(information);
		information.clear();
	}

	getline(input, information);
	nowPlayer = information[information.find(L' ') + 1] - '0';

	for (int i = 0; i < playerNum; i++)
	{
		getline(input, information);
		Player::Instance[i].setPlayer(information);
		//設定地圖上的物件的所有人與等級
		for (auto c : Player::Instance[i].getHouse())
		{
			Map[c.ID]->setOwner(i);
			Map[c.ID]->setLV(c.num);
		}
		UI::getInstance()->UpdatePlayerStatus(i);
	}
	//玩家數量未達4
	for (int i = playerNum; i < 4; i++)
	{
		UI::getInstance()->UpdatePlayerStatus(i, false);
	}

	//顯示地圖上所有物件
	for (int i = 0; i < 28; i++)
	{
		UI::getInstance()->UpdateMapObj(*(Map[i]));
	}

	//顯示玩家位置
	for (int i = 0; i < playerNum; i++)
	{
		UI::getInstance()->UpdatePlayerPos(Player::Instance[i].getPos(), i);
	}

	UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
	UI::getInstance()->UpdateNowInformation(Player::Instance[nowPlayer].getColor(), Player::Instance[nowPlayer].getName(), Round);

	input.close();

	//初始化股票
	stock[0].setStock(0, L"CSIE", 50, 2000);
	stock[1].setStock(1, L"GAME LAB", 50, 2000);
	stock[2].setStock(2, L"老山東小雞", 50, 2000);
	stock[3].setStock(3, L"七月半", 50, 2000);
	stock[4].setStock(4, L"英雄協會", 50, 2000);
	stock[5].setStock(5, L"膏雄大含總", 50, 2000);
	for (auto i : { 0,1,2,3,4,5 }) stock[i].setNextStock();
	selectedStockID = -1;
	//設定console的模式以進行滑鼠事件的操作
	DWORD prev_mode;
	GetConsoleMode(GameSystem::hInput, &prev_mode);
	SetConsoleMode(GameSystem::hInput, (ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE)) | ENABLE_MOUSE_INPUT);
	return true;
}

bool GameSystem::loadNewMap()
{
	UI::getInstance()->ShowScreen(Display::Screen::Game);
	wifstream input;
	wstring information;
	int type;

	input.imbue(locale(""));
	fileName = "MapData//" + fileName + ".txt";
	input.open(fileName, ios::in);
	getline(input, information);

	for (int i = 0; i < 28; i++)
	{
		getline(input, information);
		Map[i] = new MapObj(information);
		information.clear();
	}
	nowPlayer = 0;

	for (int i = 0; i < playerNum; i++)
	{
		UI::getInstance()->UpdatePlayerStatus(i);
	}
	//玩家數量未達4
	for (int i = playerNum; i < 4; i++)
	{
		UI::getInstance()->UpdatePlayerStatus(i, false);
	}

	//顯示地圖上所有物件
	for (int i = 0; i < 28; i++)
	{
		UI::getInstance()->UpdateMapObj(*(Map[i]));
	}

	//顯示玩家位置
	for (int i = 0; i < playerNum; i++)
	{
		UI::getInstance()->UpdatePlayerPos(Player::Instance[i].getPos(), i);
	}
	


	UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
	UI::getInstance()->UpdateNowInformation(Player::Instance[nowPlayer].getColor(), Player::Instance[nowPlayer].getName(), Round);
	input.close();

	for (int player = 0; player < playerNum; player++)
	{
		for (int i = 0; i < 6; i++)
		{
			Player::Instance[player].SetCard({ i,1 });
		}
	}

	//初始化股票
	stock[0].setStock(0, L"CSIE", 50, 2000);
	stock[1].setStock(1, L"GAME LAB", 50, 2000);
	stock[2].setStock(2, L"老山東小雞", 50, 2000);
	stock[3].setStock(3, L"七月半", 50, 2000);
	stock[4].setStock(4, L"英雄協會", 50, 2000);
	stock[5].setStock(5, L"膏雄大含總", 50, 2000);
	for (auto i : { 0,1,2,3,4,5 }) stock[i].setNextStock();
	selectedStockID = -1;
	//設定console的模式以進行滑鼠事件的操作
	DWORD prev_mode;
	GetConsoleMode(GameSystem::hInput, &prev_mode);
	SetConsoleMode(GameSystem::hInput, (ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE)) | ENABLE_MOUSE_INPUT);
	return true;
}

//遊戲勝利
void GameSystem::Win(int index)
{
	UI::getInstance()->SetColor(15);

	Player& winner = Player::Instance[index];
	UI::getInstance()->ClearControl();
	UI::getInstance()->ShowFail({ 26, 15 });
	UI::getInstance()->gotoXY(63, 15);
	cout << "┌─────────────────────┐ ";
	UI::getInstance()->DrawVerticalLine({ 63,16 }, 5, L'│');
	UI::getInstance()->DrawVerticalLine({ 85,16 }, 5, L'│');
	UI::getInstance()->gotoXY(68, 17);

	wcout << L"恭喜 ";
	UI::getInstance()->SetColor(winner.getColor());
	wcout << L" " << winner.getName() << L" ";
	UI::getInstance()->SetColor(15);

	UI::getInstance()->gotoXY(71, 19);
	wcout << L"贏得遊戲";

	UI::getInstance()->SetColor(15);
	UI::getInstance()->gotoXY(63, 21);
	cout << "└─────────────────────┘ ";
	UI::getInstance()->gotoXY(0, 50);
	playMusic(Music::music::Bankrupt);
	Sleep(6000);
	nowStatus = System::Status::Win;
	UI::getInstance()->ShowBlock(screen, Display::Block::Iknow);
	Sleep(50);
	//清除緩衝區
	FlushConsoleInputBuffer(GameSystem::hInput);
	while (screen != Display::Screen::MainMenu)
	{
		MouseInput();
	}

	Sleep(50);
	//清除緩衝區
	FlushConsoleInputBuffer(GameSystem::hInput);
}

void GameSystem::ChangePlayer()
{
	bool Flag = false;
	//若玩家並非破產
	if (!Player::Instance[nowPlayer].isbankrupt())
	{
		//更新右邊角色(更新狀態)
		UI::getInstance()->UpdatePlayerStatus(nowPlayer);
	}

	UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);
	//下一位玩家
	do
	{
		nowPlayer++;
		if (nowPlayer == playerNum)
		{
			Flag = true;
			nowPlayer = 0;
			if (Round - 1 == 0)
			{
				break;
			}
		}
	} while (Player::Instance[nowPlayer].isbankrupt());


	//輪過一輪
	if (Flag)
	{
		Round--;
		UI::getInstance()->ClearControl();
		for (int i = 0; i < playerNum; i++)
		{
			auto& player = Player::Instance[i];
			//沒有破產
			if (!player.isbankrupt())
			{
				//魔力回復
				if ((player.getMana() + 10) > 100)
				{
					player.setMana(100);
				}
				else
				{
					player.setMana(player.getMana() + 10);
				}
				//利息
				if (player.getDeposit() - player.getLoan() > 0)
				{
					Player::Instance[i].setDeposit((player.getDeposit() - player.getLoan()) * 1.03);
				}

				//商店賺取
				for (auto s : Player::Instance[i].getHouse())
				{
					if (Map[s.ID]->getType() == 2)
					{
						Player::Instance[i].payMoney(-1 * Map[s.ID]->getToll(Map[s.ID]->getLv()) * 0.2);
					}

				}
				if (player.getLoan() != 0)
				{
					player.setLoan_Round(player.getLoan_Round() - 1);
					//時間到，要還款了
					if (player.getLoan_Round() == 0)
					{
						player.payMoney(player.getLoan());

						//破產事件觸發
						if (player.getMoney() == 0 && player.getDeposit() <= 0)
						{
							UI::getInstance()->UpdatePlayerStatus(i);
							//遊戲結束
							if (Bankrupt())
							{
								Win(determinWinner());
								return;
							}
						}
						player.setLoan(0);
						player.setLoan_Round(0);
					}
				}

				UI::getInstance()->UpdatePlayerStatus(i);
			}
		}
		//遊戲結束
		if (Round == 0)
		{
			nowStatus = System::Status::Game_End;
			Win(determinWinner());
			return;
		}
		//nowPlayer = 0;
		for (auto i : { 0,1,2,3,4,5 })
			stock[i].setNextStock();

	}


	//顯示下位玩家
	UI::getInstance()->ClearControl();
	UI::getInstance()->UpdateNowInformation(Player::Instance[nowPlayer].getColor(), Player::Instance[nowPlayer].getName(), Round);
	UI::getInstance()->ShowControlPlayer(Player::Instance[nowPlayer]);

	for (auto s : Player::Instance[nowPlayer].getStatus())
	{
		//陷入蕉著或薩瓦魯多
		if (s.status == Motion::PlayerStatus::Stay ||
			s.status == Motion::PlayerStatus::Bear_The_World ||
			s.status == Motion::PlayerStatus::Glasses_sit
			)
		{
			PlayerMove(0, false);
			//清除緩衝區
			FlushConsoleInputBuffer(GameSystem::hInput);
			return;
		}
	}

	UI::getInstance()->ShowBlock(screen, Display::Block::Button);



	//清除緩衝區
	FlushConsoleInputBuffer(GameSystem::hInput);
}



bool GameSystem::checkMainColorRepeat()
{
	bool ans = 1;
	for (int i = 0; i < 4; i++)
	{
		if (Player::Instance[i].getRole() == 4)continue;
		for (int j = 0; j < 4; j++)
		{
			if (i == j || Player::Instance[j].getRole() == 4)continue;
			else if (Player::Instance[i].getColor() == Player::Instance[j].getColor())ans = 0;
		}
	}
	return ans;
}


void GameSystem::scanMapFiles()
{
	mapFiles.clear();
	system("dir .\\MapData\\*.txt /b > map.rec");
	fstream fs;
	fs.open("map.rec", ios::in);
	string input;
	while (fs >> input)
	{
		input.erase(input.end() - 4, input.end());
		mapFiles.push_back(input);
	}
	sort(mapFiles.begin(), mapFiles.end());
	//設定console的模式以進行滑鼠事件的操作
	DWORD prev_mode;
	GetConsoleMode(GameSystem::hInput, &prev_mode);
	SetConsoleMode(GameSystem::hInput, (ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE)) | ENABLE_MOUSE_INPUT);

}

void GameSystem::scanSavedFiles()
{
	savedFiles.clear();
	system("dir .\\SavedData\\*.txt /b > temp.rec");
	fstream fs;
	fs.open("temp.rec", ios::in);
	string input;
	while (fs >> input)
	{
		input.erase(input.end() - 4, input.end());
		savedFiles.push_back(input);
	}
	fs.close();
	sort(savedFiles.begin(), savedFiles.end());
	//設定console的模式以進行滑鼠事件的操作
	DWORD prev_mode;
	GetConsoleMode(GameSystem::hInput, &prev_mode);
	SetConsoleMode(GameSystem::hInput, (ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE)) | ENABLE_MOUSE_INPUT);
}

wstring GameSystem::stringToWstring(string input)
{
	wstring wstr;
	wstr.clear();
	wstr.resize(input.size(), L' ');
	char* temp1 = (char*)& wstr[0], * temp2 = &input[0];
	for (int i = 0; i < input.size(); i++)
	{
		*temp1 = *temp2;
		temp1++;
		*temp1 = 0;
		temp1++;
		temp2++;
	}
	*temp1 = 0;
	temp1++;
	*temp1 = 0;
	return wstr;
}

void GameSystem::gameSave(string input)
{
	//|名字 銀行存款 (銀行貸款 剩餘還款日期)  角色 顏色 魔力 現在狀態|股票|卡片
	wofstream wofs;
	wofs.imbue(locale(""));

	wofs.open("SavedData//" + input + ".txt", ios::out);
	mapName = stringToWstring(input);
	wofs << mapName << L" " << Round << L" " << playerNum << endl;
	for (int i = 0; i < 28; i++)
	{
		if (i < 10)wofs << 0;
		wofs << i << L" " << Map[i]->getName() << L" " << Map[i]->getType();
		if (Map[i]->getType() == 1 || Map[i]->getType() == 2 || Map[i]->getType() == 3)
		{
			wofs << L" " << to_wstring(Map[i]->getVal()) << L" " << to_wstring(Map[i]->getToll(0)) << L" " << to_wstring(Map[i]->getToll(1)) << L" " << to_wstring(Map[i]->getToll(2)) << L" " << to_wstring(Map[i]->getToll(3));
		}
		//是否有障礙物
		wofs << L" " << Map[i]->getBarrier() << endl;
	}
	wofs << L"playerstate " << nowPlayer << endl;
	for (int i = 0; i < playerNum; i++)
	{
		wofs << i << L" ";
		if (Player::Instance[i].getPos() < 10) wofs << 0;
		wofs << Player::Instance[i].getPos() << L" " << to_wstring(Player::Instance[i].getMoney());
		//房子
		for (auto j : Player::Instance[i].getHouse())
		{
			wofs << L" ";
			if (j.ID < 10)wofs << 0;
			wofs << j.ID << L" " << j.num;
		}
		wofs << L"|" << Player::Instance[i].getName() << L" " << to_wstring(Player::Instance[i].getDeposit()) << L" " << to_wstring(Player::Instance[i].getLoan()) << L" " << to_wstring(Player::Instance[i].getLoan_Round());//貸款 還款日
		wofs << L" " << Player::Instance[i].getRole() << L" " << to_wstring(Player::Instance[i].getColor()) << L" " << to_wstring(Player::Instance[i].getMana());
		//狀態
		for (auto j : Player::Instance[i].getStatus())
		{
			wofs << L" " << j.status << L" " << j.statusTimes;
		}
		wofs << "|";
		//股票
		for (auto j : { 0,1,2,3,4,5 })
		{
			wofs << L" " << to_wstring(Player::Instance[i].getStockNum(j)) << L" " << to_wstring(Player::Instance[i].getStockCost(j));
		}
		wofs << "|";
		//卡片
		for (auto j : Player::Instance[i].getCard())
		{
			wofs << L" " << j.ID << L" " << to_wstring(j.num);
		}
		wofs << endl;
	}
	_setmode(_fileno(stdout), _O_TEXT);
	wofs.close();
}
