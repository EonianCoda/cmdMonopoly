#include "UI.h"

namespace {
	//計算wstring的長度
	int getTextLength(const wstring& str)
	{
		int length = 0;
		for (auto c : str)
		{
			if (c <= 122)
			{
				length++;
			}
			else
			{
				length += 2;
			}
		}
		return length;
	}

	wstring Number = L"０１２３４５６７８９";
	static Coordinate list[] = {
		{2,1},{15,1},{28,1},{41,1},{54,1},{67,1},{80,1},{93,1},
		{93,7},{93,13},{93,19},{93,25},{93,31},{93,37},{93,43},
		{80,43},{67,43},{54,43},{41,43},{28,43},{15,43},{2,43},
		{2,37},{2,31},{2,25},{2,19},{2,13},{2,7}
	};
	static wstring TypeName[] = {
	L"民宅",L"商店",L"賭場"
	};


	vector<wstring> unionFlag = {
	L"                !#########       #",
	L"             !########!          ##!",
	L"          !########!               ###",
	L"       !##########                  ####",
	L"     ######### #####                ######",
	L"      !###!      !####!              ######",
	L"        #           #####            ######!",
	L"                      !####!         #######",
	L"                         #####       #######",
	L"                           !####!   #######!",
	L"                              ####!########",
	L"           ##                   ##########",
	L"         ,######!          !#############",
	L"       ,#### ########################!####!",
	L"     ,####'     ##################!'    #####",
	L"   ,####'            #######              !####!",
	L"  ####'                                      #####",
	L"  ~##                                          ##~"
	};
	

	vector<wstring> fail = {
		L"            _ _",
		L"          ./-_-`-'-.",
		L"         (;'/ `\\()  '.",
		L"          `'    |     '.",
		L"                \\()    (\\",
		L"             __/    ()   \\  __",
		L"          .~'_ `'.;-.___.~'` _'~.",
		L"          { (_`.'         '.`_) }",
		L"           \\ .'  _ 0_._0 _  '. /     .,_",
		L"            } -.'   (_)   '.- {    _{   `\\",
		L"         _ {_ / '.___|___.' \\  }  //`._   |",
		L"       /` \\  \\     |   |     }  }:'-.  ```'\"--..==,",
		L"      {    \\ ,}    \\-\"-/   .'  } {,`-'.       //>`\>",
		L"     {`   _./|\\._.  '-'  ._ .~` /`    ;'.    //>  |>",
		L"     {     {///(  `-.-.-`  ) _.'     /   '. ||>   />",
		L"      \\     \\|\\);--`( )`--`(`       }      `\\\\>_.'>",
		L"       ;  _/`/(__.'/`-'.,__/`,    .`         `\"\"\"`",
		L"      .-'`     ;-.(     \\_(;  \\ .'     .--,",
		L"     (`-._   ./   `       '.   `-._..~` /o\\\\",
		L"       `'-;/``.              `;-\"`:    |oo||",
		L" .--._ _.' .  \\      o       ;  .      |  /|",
		L"/.-.  `     .  '._        _.'  '       \\_//",
		L"||oo\\        `.   `'-----`  _.~`--..__,..'",
		L"|\\o  |       .~`'--......--'",
		L" \\'._/   _.~`",
		L"  `.__.-'",
	};


	wstring bearHead[15] = {
		L"　　　　　             ",
		L"　　 ∩＿＿＿＿∩      ",
		L"　　 |ノ　　　  ヽ     ",
		L"　　/　 ●　  ● |     ",
		L"　 |　　 ( _●_) ミ    ",
		L"　彡､　   |∪|     ＼  ",
		L" / _＿　  ヽノ  /＼  ）",
		L"(＿＿＿）　　  /  ノ / ",
		L"　 |　　　    / （_／  ",
		L"　 |　　　    \\        ",
		L"　 |　／￣＼　 \\       ",
		L"　 | /　    )　 )      ",
		L"　 ∪　 　 （  〈      ",
		L"　　　　  　＼__)      " ,
		L"　　　　　             " };
	wstring rabbitHead[15] = {
		L"　　 |⌒|　　|⌒|      ",
		L"　　 |┃ |　　|┃ |      ",
		L"　　 |┃ |＿＿|┃ |      ",
		L"　　 |ノ　　　　 ヽ    ",
		L"　　/　 ●　　 ● |    ",
		L"　 |　　 ( _●_)　 ミ  ",
		L"　彡､     |Ш|　　 ＼  ",
		L" / ＿＿　 ヽノ　/´〉 ) ",
		L"(＿＿＿）　　　/　(_／ ",
		L"　 |　　　　　/        ",
		L"　 |　 ／ ＼　＼       ",
		L"　 | / 　  )　 )       ",
		L"　 ∪　　 （　〈       ",
		L"　　　　　 ＼＿)       ",
		L"　　　　　             " };
	wstring rabbitGlass[15] = {
		L"　　 |⌒|　　|⌒|      ",
		L"　　 |┃ |　　|┃ |      ",
		L"　　 |┃ |＿＿|┃ |_     ",
		L"　　 |ノ  _, ,_ 　ヽ   ",
		L"　　/   －□―□-  |   ",
		L"　 |　   ( _●_)　 ミ  ",
		L"　彡､        　　   ＼ ",
		L" / ＿＿　     　/´〉  )",
		L"(＿＿＿）　　　/　(_／ ",
		L"　 |　　　　　/        ",
		L"　 |　 ／ ＼　＼       ",
		L"　 | / 　  )　 )       ",
		L"　 ∪　　 （　〈       ",
		L"　　　　　 ＼＿)       ",
		L"　　　　　             " };
	wstring bearSuit[15] = {
		L"　  　　　　           ",
		L"   ∩＿＿＿＿＿＿_∩   ",
		L"   |ノ　　　　     ヽ  ",
		L"   |  　　　　      |  ",
		L"   |　  ●　  ●    |  ",
		L"  彡    *(_,●_)*   ミ ",
		L"   |   │ ´・ω・|   |  ",
		L"   |     ￣￣￣     |  ",
		L"   |   ＿          _|  ",
		L"   |   ∪　　　    ∪  ",
		L"   |               |   ",
		L"   ＼___ 　　   ___/   ",
		L"　   　 ||￣￣||       ",
		L"   　　 ∪    ∪       ",
		L" 　  　　　　          " };
	wstring roleName[] = {
		L"Pedo熊",L"Pedo兔",L"眼鏡老伯",L"玩偶熊"
	};

	wstring noPlayer[15] = {
		L"                      ",
		L"                      ",
		L"                      ",
		L"                      ",
		L"                      ",
		L"                      ",
		L"                      ",
		L"         N / A        ",
		L"                      ",
		L"                      ",
		L"                      ",
		L"                      ",
		L"                      ",
		L"                      ",
		L"                      ",
	};

	vector<wstring> Bank = {
		L"                    _._._                       _._._                   ",
		L"                   _|   |_                     _|   |_                  ",
		L"                   | ... |_._._._._._._._._._._| ... |                  ",
		L"                   | ||| |  o NATIONAL BANK o  | ||| |                  ",
		L"                   | \"\"\" |  \"\"\"    \"\"\"    \"\"\"  | \"\"\" |                  ",
		L"              ())  |[-|-]| [-|-]  [-|-]  [-|-] |[-|-]|  ())             ",
		L"             (())) |     |---------------------|     | (()))            ",
		L"            (())())| \"\"\" |  \"\"\"    \"\"\"    \"\"\"  | \"\"\" |(())())           ",
		L"            (()))()|[-|-]|  :::   .-\"-.   :::  |[-|-]|(()))()           ",
		L"            ()))(()|     | |~|~|  |_|_|  |~|~| |     |()))(()           ",
		L"               ||  |_____|_|_|_|__|_|_|__|_|_|_|_____|  ||              ",
		L"            ~ ~^^ @@@@@@@@@@@@@@/=======\\@@@@@@@@@@@@@@ ^^~ ~           ",
		L"                                                                        "
	};


	wstring ground[7] = {
			L"~~~~~~~~~\\ / ~~~~~~~~~~~~~~~\\ / ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\ / ~~~~~",
			L"  \\/..__..--  . \\/  \\/  .  ..  \\/..__..--  . \\/  \\/  .  ..            ",
			L"        .  \\/              \/            .  \/              \\/  .       ",
			L". \\/             .   \\/     .                                         ",
			L"   __...--..__..__       .     \\/   __...--..__..__       .     \\/    ",
			L"\\/  .   .    \\/     \\/    __..--..\\/  .   .    \\/     \\/    __..-     ",
			L"~~~~~~~~\\/~~~~~~~~~~~~~~\/~~~~~~~~~~~~~~~~\\/~~~~~~~\\/~~~~~~~~~~~~~~~\\/ ",
	};

	wstring ground2[7] = {
		L" /\\ /\\ /\\     /\\ /\\ /\\     /\\ /\\ /\\    /\\ /\\ /\\     /\\ /\\ /\\  ",
		L"//\\\\/\\\\/\\\\   //\\\\/\\\\/\\\\   //\\\\/\\\\/\\\\  //\\\\/\\\\/\\\\   //\\\\/\\\\/\\\\ ",
		L"//\\\\/\\\\/\\\\   //\\\\/\\\\/\\\\   //\\\\/\\\\/\\\\  //\\\\/\\\\/\\\\   //\\\\/\\\\/\\\\ ",
		L" || || ||     || || ||     || || ||    || || ||     || || ||  ",
		L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
		L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
		L"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
	};

	wstring banner[7] = {
		//aaaa1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
			L" ___      ___     ______    _____  ___      ______    _______    ______    ___       ___  ___",
			L"|\"  \\    /\"  |   /    \" \\  (\\\"   \\|\"  \\    /    \" \\  |   __ \"\\  /    \" \\  |\"  |     |\"  \\/\" | ",
			L" \\   \\  //   |  // ____  \\ |.\\\\   \\    |  // ____  \\ (. |__) :)// ____  \\ ||  |      \\   \\  /",
			L" /\\\\  \\/.    | /  /    ) :)|: \\.   \\\\  | /  /    ) :)|:  ____//  /    ) :)|:  |       \\\\  \\/",
			L"|: \\.        |(: (____/ // |.  \\    \\. |(: (____/ // (|  /   (: (____/ //  \\  |___    /   /",
			L"|.  \\    /:  | \\        /  |    \\    \\ | \\        / /|__/ \\   \\        /  ( \\_|:  \\  /   /",
			L"|___|\\__/|___|  \\\"_____/    \\___|\\____\\)  \\\"_____/ (_______)   \\\"_____/    \\_______)|___/" };
}
void UI::ShowBuilding(int type, int ID, const int X, const int Y)
{
	SetColor(Color::black, Color::white, false, true);
	for (int y = 0; y < 7; y++)
	{
		gotoXY(X, Y + y);
		wcout << ground2[y];
	}
}

void UI::showUnionFlag()
{
	int X = 28, Y = 19;
	_setmode(_fileno(stdout), _O_U16TEXT);

	SetColor(Color::yellow, Color::red, true, false);
	DrawHorizontalLine({ X,Y - 1 }, 53, L' ');
	for (int y = 0; y < unionFlag.size() + 1; y++)
	{
		DrawHorizontalLine({ X,Y + y }, 53, L' ');
	}

	for (auto n : unionFlag)
	{
		gotoXY(X, Y++);
		wcout << n;
	}
	gotoXY(0, 50);
	_setmode(_fileno(stdout), _O_TEXT);
}

void UI::ShowFail(const Coordinate& start)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	int Y = start.Y;
	for (auto n : fail)
	{
		gotoXY(start.X, Y++);
		wcout << n;
	}
	_setmode(_fileno(stdout), _O_TEXT);
}
//staitc 變數
HANDLE UI::OUTPUT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
UI* UI::Instance = NULL;

UI* UI::getInstance()
{
	if (UI::Instance == NULL)
	{
		UI::Instance = new UI;
	}
	return UI::Instance;
}


UI::UI() :colorCode(Color::black * 16 + Color::white)
{
	//讓中文字可以正常輸出
	setlocale(LC_ALL, "zh-TW");

	RECT r;
	GetWindowRect(GetConsoleWindow(), &r); //stores the console's current dimensions
	MoveWindow(GetConsoleWindow(), r.left, r.top, 1400, 1100, TRUE); // 800 width, 100 height
}


//顯示角色圖像
void UI::ShowAvatar(const int avatarID, const int X, const int Y)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	SetColor(Color::black, Color::white, false, true);
	switch (avatarID)
	{
	case 0:
		for (int i = 0; i < 15; i++)
		{
			gotoXY(X, Y + i);
			wcout << bearHead[i] << endl;
		}
		break;
	case 1:
		for (int i = 0; i < 15; i++)
		{
			gotoXY(X, Y + i);
			wcout << rabbitHead[i] << endl;
		}
		break;
	case 2:
		for (int i = 0; i < 15; i++)
		{
			gotoXY(X, Y + i);
			wcout << rabbitGlass[i] << endl;
		}
		break;
	case 3:
		for (int i = 0; i < 15; i++)
		{
			gotoXY(X, Y + i);
			wcout << bearSuit[i] << endl;
		}
		break;
	case 4:
		for (int i = 0; i < 15; i++)
		{
			gotoXY(X, Y + i);
			wcout << noPlayer[i] << endl;
		}
		break;
	}
	_setmode(_fileno(stdout), _O_TEXT);
}

void UI::ShowScreen(Display::Screen screen)
{
	switch (screen)
	{
	case Display::Screen::MainMenu:
		SetColor(15);
		//清空
		gotoXY(0, 0);
		for (int i = 0; i < 50; i++)
		{
			wcout << L"                                                                                                                                            " << endl;
		}
		ShowBlock(screen, Display::Block::MainBanner);
		ShowBlock(screen, Display::Block::MainBeginning);

		break;
	case Display::Screen::Game:
		ShowMap();
		ShowBlock(Display::Screen::Game, Display::Block::PlayerStatus);
		ShowBlock(Display::Screen::Game, Display::Block::GameStatus);
		ShowBlock(Display::Screen::Game, Display::Block::Control);
		ShowBlock(Display::Screen::Game, Display::Block::Button);
		break;
	}
}

void UI::ShowBlock(Display::Screen screen, Display::Block block)
{
	if (screen == Display::Screen::MainMenu)
	{
		if (block == Display::Block::MainBanner)
		{
			DrawFrame({ 0,0 }, 132, 48);
			for (int i = 0; i < 7; i++)
			{
				gotoXY(20, i + 10);
				wcout << banner[i] << endl;
			}
		}
		else if (block == Display::Block::MainBeginning)
		{
			SetColor(15);
			DrawFrame({ 52, 20 }, 23, 5);
			gotoXY(60, 22);
			wcout << L"開始遊戲";
			DrawFrame({ 52,26 }, 23, 5);
			gotoXY(62, 28);
			wcout << L"聲音";
			DrawFrame({ 52,32 }, 23, 5);
			gotoXY(60, 34);
			wcout << L"離開遊戲";
			gotoXY(0, 50);
		}
		else if (block == Display::Block::MainSelectMode)
		{
			SetColor(15);
			DrawFrame({ 52, 20 }, 23, 5);
			gotoXY(61, 22);
			wcout << L"新遊戲";
			DrawFrame({ 52,26 }, 23, 5);
			gotoXY(60, 28);
			wcout << L"開啟舊檔";
			gotoXY(0, 50);
		}
		else if (block == Display::Block::MainSelectFile)
		{
			SetColor(15);
			DrawFrame({ 20,20 }, 93, 25);
			gotoXY(61, 22);
			wcout << L"請選擇舊檔";
			DrawHorizontalLine({ 23,24 }, 88);
			gotoXY(0, 50);

		}
		else if (block == Display::Block::MainNewMap)
		{
			SetColor(15);
			DrawFrame({ 20,20 }, 93, 25);
			gotoXY(61, 22);
			wcout << L"請選擇地圖";
			DrawHorizontalLine({ 23,24 }, 88);
			gotoXY(0, 50);

		}
		else if (block == Display::Block::MainNewCharater)
		{
			for (int i = 0; i < 4; i++)
			{
				ShowAvatar(Player::Instance[i].getRole(), 17 + i * 25, 10);
				SetColor(15);

				gotoXY(16 + i * 25, 4);
				cout << "┌──────────────────────┐ ";
				gotoXY(16 + i * 25, 5);
				cout << "│ ";
				SetColor(Player::Instance[i].getColor());
				wcout << L" " << Player::Instance[i].getName() << L" ";
				SetColor(15);
				cout << "            │ ";
				gotoXY(16 + i * 25, 6);
				cout << "└──────────────────────┘ ";

				gotoXY(16 + i * 25, 7);
				cout << "┌──────────────────────┐ ";
				gotoXY(16 + i * 25, 8);
				cout << "│          ▲          │ ";
				gotoXY(16 + i * 25, 9);
				cout << "└──────────────────────┘ ";
				gotoXY(16 + i * 25, 25);
				cout << "┌──────────────────────┐ ";
				gotoXY(16 + i * 25, 26);
				cout << "│          ▼          │ ";
				gotoXY(16 + i * 25, 27);
				cout << "└──────────────────────┘ ";
			}
			gotoXY(73, 30);
			cout << "初始金額：  30000        初始回合數：  20";
			DrawFrame({ 16,31 }, 99, 16);
			gotoXY(20, 33);
			cout << "請點選設定玩家名稱（20個英數字以內）及玩家角色";
			gotoXY(20, 35);
			cout << "並設定初始金錢(1~9,999,999)與遊玩回合數(1~100)";
			gotoXY(20, 37);
			cout << "完成後點選右下開始遊戲";

			gotoXY(20, 40);
			cout << "點選角色圖像可觀看角色介紹";

			gotoXY(95, 42);
			cout << "┌───────────┐ ";
			gotoXY(95, 43);
			cout << "│  開始遊戲 │ ";
			gotoXY(95, 44);
			cout << "└───────────┘ ";

			gotoXY(0, 50);

		}
		else if (block == Display::Block::MainSetSound)
		{
			SetColor(15);
			DrawFrame({ 52,26 }, 23, 5);
			gotoXY(63, 28);
			if (getMusicMode() == Music::MusicMode::normal)
			{
				wcout << L"開";
			}
			else
			{
				wcout << L"關";
			}
			gotoXY(0, 50);
		}

	}
	else if (screen == Display::Screen::Game)
	{
		switch (block)
		{
		case Display::Block::Menu:
			DrawFrame({ 20,16 }, 66, 23);
			gotoXY(50, 18);
			SetColor(Color::green, Color::black, true);
			cout << "選   單";

			SetColor(Color::blue, Color::black, true);
			DrawEllipseButton({ 23,20 }, 25, 9, true);
			DrawEllipseButton({ 56,20 }, 25, 9, true);
			DrawEllipseButton({ 23,29 }, 25, 9, true);
			DrawEllipseButton({ 56,29 }, 25, 9, true);
			SetColor(Color::black, Color::white, false, true);
			//SetColor(Color::yellow, Color::white,false, true);
			gotoXY(32, 24);
			cout << "繼 續 遊 戲";
			gotoXY(65, 24);
			cout << "儲 存 遊 戲";
			gotoXY(32, 33);
			cout << "回 主 畫 面";
			gotoXY(65, 33);
			cout << "離 開 遊 戲";
			gotoXY(0, 50);
			break;
		case Display::Block::PlayerStatus:
			DrawFrame({ 107,0 }, 25, 49);
			SetColor(Color::white, Color::black, true);
			for (int y = 12; y < 48; y += 12)
			{
				gotoXY(107, y);
				cout << "╠";
				DrawHorizontalLine({ 108,y }, 23);
				cout << "╣ ";
			}
			break;
		case Display::Block::GameStatus:
			SetColor(Color::black, Color::white, false, true);
			DrawHorizontalLine({ 15,7 }, 75, L'─');
			DrawHorizontalLine({ 15, 12 }, 75, L'─');
			DrawVerticalLine({ 15,8 }, 4, L'│');
			DrawVerticalLine({ 89,8 }, 4, L'│');

			DrawVerticalLine({ 16,8 }, 4, L' ');

			gotoXY(15, 7);
			cout << "┌";
			gotoXY(89, 7);
			cout << "┐ ";
			gotoXY(15, 12);
			cout << "└";
			gotoXY(89, 12);
			cout << "┘ ";

			for (int y = 8; y < 12; y++)
			{
				DrawHorizontalLine({ 16,y }, 73, L' ');
			}
			gotoXY(17, 8);
			cout << "目前遊戲者：";
			gotoXY(17, 10);
			cout << "剩餘回合數：";
			break;
		case Display::Block::Control:
			SetColor(Color::white, Color::black, true);

			DrawHorizontalLine({ 15,14 }, 75, L'─');
			DrawHorizontalLine({ 15,41 }, 75, L'─');
			DrawVerticalLine({ 15,15 }, 26, L'│');
			DrawVerticalLine({ 89,15 }, 26, L'│');
			gotoXY(15, 14);
			cout << "┌";
			gotoXY(89, 14);
			cout << "┐ ";
			gotoXY(15, 41);
			cout << "└";
			gotoXY(89, 41);
			cout << "┘ ";
			gotoXY(17, 31);
			break;

		case Display::Block::Button:

			SetColor(Color::white, Color::black, true);
			DrawEllipseButton({ 17,31 }, 32, 5);
			gotoXY(30, 33);
			cout << "投擲骰子";


			DrawEllipseButton({ 54,31 }, 32, 5);
			gotoXY(67, 33);
			cout << "股票市場";

			DrawEllipseButton({ 17,36 }, 32, 5);
			gotoXY(30, 38);
			cout << "使用卡片";

			DrawEllipseButton({ 54,36 }, 32, 5);
			gotoXY(67, 38);
			cout << "使用技能";

			gotoXY(0, 50);
			break;
		case Display::Block::SelectDice:
			SetColor(Color::white, Color::black, true);

			gotoXY(35, 32);
			cout << "請 選 擇 要 使 用 的 骰 子 ！ ！";
			gotoXY(70, 31);
			cout << "┌───────────┐ ";
			gotoXY(70, 32);
			cout << "│    取消   │ ";
			gotoXY(70, 33);
			cout << "└───────────┘ ";

			DrawEllipseButton({ 20,34 }, 30, 7);
			gotoXY(32, 36);
			cout << "一般骰子";
			gotoXY(25, 38);
			cout << "(１、２、３、４、５、６)";

			DrawEllipseButton({ 53,34 }, 30, 7);
			gotoXY(64, 36);
			cout << "角色專屬骰子";
			gotoXY(58, 38);
			cout << "(１、２、３、４、５、６)";

			break;
		case Display::Block::RollDice:
			SetColor(Color::white, Color::black, true);
			gotoXY(35, 32);
			cout << "請 點 選 骰 子 以 投 擲 骰 子 ！！";
			gotoXY(70, 31);
			cout << "┌─────────┐ ";
			gotoXY(70, 32);
			cout << "│   取消  │ ";
			gotoXY(70, 33);
			cout << "└─────────┘ ";
			break;
			//我知道了
		case Display::Block::Iknow:
			SetColor(Color::white, Color::black, true);
			DrawEllipseButton({ 65,34 }, 20, 7, true);
			gotoXY(73, 37);
			SetColor(Color::black, Color::white, false, true);
			cout << "我知道了";
			gotoXY(0, 50);
			break;
			//銀行
		case Display::Block::Bank:
			SetColor(Color::black, Color::white, false, true);
			for (int i = 0; i < Bank.size(); i++)
			{
				gotoXY(17, 15 + i);
				wcout << Bank[i];
			}
			SetColor(15);
			DrawHorizontalLine({ 17,28 }, 71, L'─');
			gotoXY(15, 28);
			cout << "├";
			gotoXY(89, 28);
			cout << "┤";

			gotoXY(36, 30);
			cout << "歡迎來到銀行，請問要辦理什麼業務？";
			gotoXY(73, 29);
			cout << "┌─────────┐ ";
			gotoXY(73, 30);
			cout << "│   離開  │ ";
			gotoXY(73, 31);
			cout << "└─────────┘ ";

			DrawEllipseButton({ 17,32 }, 15, 9);
			DrawEllipseButton({ 35,32 }, 15, 9);
			DrawEllipseButton({ 53,32 }, 15, 9);
			DrawEllipseButton({ 71,32 }, 15, 9);


			gotoXY(24, 36);
			cout << "提款";
			gotoXY(42, 36);
			cout << "存款";
			gotoXY(60, 36);
			cout << "借款";
			gotoXY(78, 36);
			cout << "還款";

			gotoXY(0, 50);
			break;
		case Display::Block::UseCard:
			SetColor(15);
			gotoXY(18, 38);
			cout << "┌───────┐ ";
			gotoXY(18, 39);
			cout << "│  取消 │ ";
			gotoXY(18, 40);
			cout << "└───────┘ ";


			gotoXY(36, 38);
			cout << "┌─────────┐ ";
			gotoXY(36, 39);
			cout << "│  上一頁 │ ";
			gotoXY(36, 40);
			cout << "└─────────┘ ";

			gotoXY(49, 39);
			cout << "１　/　１";

			gotoXY(59, 38);
			cout << "┌─────────┐ ";

			gotoXY(59, 39);
			cout << "│  下一頁 │ ";
			gotoXY(59, 40);
			cout << "└─────────┘ ";
			gotoXY(0, 50);
			break;

		case Display::Block::StockMarket:
			SetColor(15);
			gotoXY(18, 15);
			cout << "┌────────────────────────────────────────────────────────────────────┐ ";
			gotoXY(18, 29);
			cout << "└────────────────────────────────────────────────────────────────────┘ ";

			gotoXY(20, 16);
			SetColor(Color::black, Color::white);
			wcout << L"  股票名稱  "; SetColor(15); wcout << L" ";
			SetColor(Color::black, Color::white);
			wcout << L"  成交價  "; SetColor(15); wcout << L" ";
			SetColor(Color::black, Color::white);
			wcout << L"  漲跌  "; SetColor(15); wcout << L" ";
			SetColor(Color::black, Color::white);
			wcout << L"  交易量  "; SetColor(15); wcout << L" ";
			SetColor(Color::black, Color::white);
			wcout << L"  持股數  "; SetColor(15); wcout << L" ";
			SetColor(Color::black, Color::white);
			wcout << L"  購買成本  ";

			SetColor(Color::white, Color::black, true);
			gotoXY(15, 30);
			cout << "├";
			gotoXY(89, 30);
			cout << "┤";
			DrawHorizontalLine({ 17, 30 }, 71, L'─');
			gotoXY(20, 32);
			wcout << L"請選擇要做的動作：";
			gotoXY(70, 31);
			cout << "┌─────────┐ ";
			gotoXY(70, 32);
			cout << "│   取消  │ ";
			gotoXY(70, 33);
			cout << "└─────────┘ ";
			DrawEllipseButton({ 17,35 }, 32, 5);
			gotoXY(30, 37);
			cout << "購買股票";
			DrawEllipseButton({ 54,35 }, 32, 5);
			gotoXY(67, 37);
			cout << "賣出股票";
			gotoXY(0, 50);
			break;

		case Display::Block::BuyStock:
			SetColor(15);
			for (int y = 0; y < 9; y++)
			{
				DrawHorizontalLine({ 17,31 + y }, 72, L' ');
			}
			_setmode(_fileno(stdout), _O_U16TEXT);

			UI::getInstance()->gotoXY(20, 32);
			wcout << L"請問要購買的股票項目？（點選上方選單，再次點擊以重新選擇）";
			UI::getInstance()->gotoXY(19, 34);
			wcout << L"➩  ";
			UI::getInstance()->gotoXY(70, 37);
			wcout << L"┌─────────┐ ";
			UI::getInstance()->gotoXY(70, 38);
			wcout << L"│   取消  │ ";
			UI::getInstance()->gotoXY(70, 39);
			wcout << L"└─────────┘ ";
			UI::getInstance()->gotoXY(20, 36);
			wcout << L"請問要購買的股票張數？（按下Enter以確認）";
			UI::getInstance()->gotoXY(19, 38);
			wcout << L"➩  ";
			_setmode(_fileno(stdout), _O_TEXT);
			break;
		case Display::Block::SellStock:
			SetColor(15);
			for (int y = 0; y < 9; y++)
			{
				DrawHorizontalLine({ 17,31 + y }, 72, L' ');
			}
			_setmode(_fileno(stdout), _O_U16TEXT);


			UI::getInstance()->gotoXY(20, 32);
			wcout << L"請問要賣出的股票項目？（點選上方選單，再次點擊以重新選擇）";
			UI::getInstance()->gotoXY(19, 34);
			wcout << L"➩  ";
			UI::getInstance()->gotoXY(70, 37);
			wcout << L"┌─────────┐ ";
			UI::getInstance()->gotoXY(70, 38);
			wcout << L"│   取消  │ ";
			UI::getInstance()->gotoXY(70, 39);
			wcout << L"└─────────┘ ";
			UI::getInstance()->gotoXY(20, 36);
			wcout << L"請問要賣出的股票張數？（按下Enter以確認）";
			UI::getInstance()->gotoXY(19, 38);
			wcout << L"➩  ";
			_setmode(_fileno(stdout), _O_TEXT);
			break;
		case Display::Block::UseSkill:
			SetColor(Color::white, Color::black, true);
			gotoXY(39, 31);
			cout << "請 點 選 要 使 用 的 技 能 ！";
			gotoXY(74, 30);
			cout << "┌─────────┐ ";
			gotoXY(74, 31);
			cout << "│   取消  │ ";
			gotoXY(74, 32);
			cout << "└─────────┘ ";
			DrawEllipseButton({ 17,33 }, 32, 8);

			DrawEllipseButton({ 54,33 }, 32, 8);

			break;
			//賭場
		case Display::Block::Casino:
			SetColor(Color::white, Color::black, true);
			gotoXY(35, 32);

			cout << " 歡 迎 來 到 賭 場 ， 請 點 選 骰 子";
			Dice({ 46,34 }, 0, 0, true);

			break;
		case Display::Block::PassRound:
			/*_setmode(_fileno(stdout), _O_U16TEXT);
			int OutputY = 40;

			SetColor(15);
			OutputY += 2;
			gotoXY(45, OutputY);
			wcout << L"➤ 玩家： ";
			SetColor(player.getColor());
			wcout << L" " << player.getName() << L" ";


			_setmode(_fileno(stdout), _O_TEXT);*/
			break;
		case Display::Block::GameSave:
			gotoXY(63, 21);
			cout << "請輸入存檔名稱";
			gotoXY(61, 23);
			cout << "請勿超過20個英數字";
			gotoXY(58, 25);
			_setmode(_fileno(stdout), _O_U16TEXT);
			wcout << L"➩ ";
			_setmode(_fileno(stdout), _O_TEXT);
			break;
		}

	}
}

//顯示使用技能
void UI::ShowUseSkill(int playerIndex)
{
	Player& player = Player::Instance[playerIndex];
	auto skill = player.getSkill();
	int length = 0, Y;
	_setmode(_fileno(stdout), _O_U16TEXT);

	//顯示第一個技能
	length = getTextLength(skill.firstSkill.first);
	gotoXY(20 + (31 - length) / 2, 34);
	SetColor(Color::green, Color::black, true);
	wcout << skill.firstSkill.first;


	gotoXY(29, 36);
	SetColor(Color::white, Color::blue, true);
	wcout << L"消耗魔力： " << skill.firstSpend;


	SetColor(15);
	Y = 38;
	length = getTextLength(skill.firstSkill.second);
	if (length < 31)
	{
		gotoXY(20 + (31 - length) / 2, Y);
		wcout << skill.firstSkill.second;
	}
	else
	{
		length = 0;
		auto& content = skill.firstSkill.second;
		gotoXY(19, Y);
		for (int i = 0; i < content.size(); i++)
		{
			wcout << content[i];

			length += ((content[i] <= 122) ? 1 : 2);
			if (length >= 30)
			{
				Y++;
				gotoXY(19, Y);
				length = 0;
			}
		}
	}



	//顯示第二個技能
	length = getTextLength(skill.SecondSkill.first);
	gotoXY(57 + (31 - length) / 2, 34);
	SetColor(Color::green, Color::black, true);
	wcout << skill.SecondSkill.first;


	gotoXY(66, 36);
	SetColor(Color::white, Color::blue, true);
	wcout << L"消耗魔力： " << skill.SecondSpend;


	SetColor(15);
	Y = 38;
	length = getTextLength(skill.SecondSkill.second);
	if (length < 31)
	{
		gotoXY(57 + (31 - length) / 2, Y);
		wcout << skill.SecondSkill.second;
	}
	else
	{
		auto& content2 = skill.SecondSkill.second;
		length = 0;
		gotoXY(56, Y);

		for (int i = 0; i < content2.size(); i++)
		{
			wcout << content2[i];

			length += ((content2[i] <= 122) ? 1 : 2);
			if (length >= 30)
			{
				Y++;
				gotoXY(56, Y);
				length = 0;
			}
		}
	}

	gotoXY(0, 50);
	_setmode(_fileno(stdout), _O_TEXT);
}

//顯示銀行業務(存款機或提款機)
void UI::ShowBankWork(int work, const Player& player)
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	//提款
	for (int y : {29, 40})
	{
		gotoXY(21, y);
		wcout << L"█════════════════════════════════════════════════════════════█";
	}


	for (int y = 1; y < 11; y++)
	{
		gotoXY(21, 29 + y);
		wcout << L"║                                                             ║";
	}

	gotoXY(24, 30);
	wcout << L"┌─────────┐ ";
	gotoXY(24, 31);
	wcout << L"│   返回  │ ";
	gotoXY(24, 32);
	wcout << L"└─────────┘ ";

	gotoXY(42, 30);
	SetColor(player.getColor());
	wcout << L" " << player.getName() << L" ";
	SetColor(15);
	wcout << L" 的銀行帳戶";
	gotoXY(42, 32);
	wcout << L"➤ 現金：";
	OutputMoney(player.getMoney(), false);


	gotoXY(42, 34);
	wcout << L"➤ 銀行存款：";
	OutputMoney(player.getDeposit(), false);

	/*gotoXY(42, 36);
	wcout << L"➤ 借款：";
	OutputMoney(player.getLoan(), false);
	if (player.getLoan() != 0)
	{
		wcout << L"(剩餘還款天數：";
		auto round = player.getLoan_Round();
		while (round >= 10)
		{
			wcout << Number[round / 10];
			round /= 10;
		}
		wcout << Number[round] << L" 天)";
	}*/


	if (work == 0)
	{
		gotoXY(38, 36);
		wcout << L"請問要提取多少金額？(按下Enter以確認)";
	}
	//存款
	else if (work == 1)
	{
		gotoXY(39, 36);
		wcout << L"請問要存進多少金額？(按下Enter以確認)";
	}
	//借款
	else if (work == 2)
	{
		gotoXY(32, 36);
		wcout << L"請問要借多少金額？(按下Enter以確認，上限為一千萬)";
	}
	else if (work == 3)
	{
		gotoXY(42, 36);
		wcout << L"借款金額：";
		OutputMoney(player.getLoan(), false);


		gotoXY(28, 38);
		wcout << L"剩餘還款天數：";
		auto round = player.getLoan_Round();
		while (round >= 10)
		{
			wcout << round / 10;
			round %= 10;
		}
		wcout << round << L" 天，";

		wcout << L"確定立刻還款？(按下Enter確定)";

		_setmode(_fileno(stdout), _O_TEXT);
		gotoXY(0, 50);
		return;
	}
	gotoXY(37, 38);
	wcout << L"➩  ";
	SetColor(Color::yellow, Color::black, false);
	wcout << L"¥ ";

	_setmode(_fileno(stdout), _O_TEXT);
}

void UI::ShowCard(int playerIndex, int page)
{
	static Coordinate pos[6] = {
		{ 19,15 },{ 43,15 },{ 66,15 },{ 19,26 },{ 43,26 },{ 66,26 },
	};
	SetColor(15);
	Player& player = Player::Instance[playerIndex];
	int i = 0;
	auto items = player.getCard();

	for (int i = 0; i < (player.getCardNum() - page * 6); i++)
	{
		auto content = getCardContent(items[page * 6 + i].ID);
		DrawCard(pos[i], content.substr(0, content.find(L'：')), content.substr(content.find(L'：') + 1), items[page * 6 + i].num);
	}

	gotoXY(49, 39);
	wcout << Number[page] << L"　/　" << Number[((items.size() - 1) / 6) + 1];

	gotoXY(0, 50);
}

void UI::DrawCard(const Coordinate& Start, const wstring& title, const wstring& content, int n)
{
	int X = Start.X, Y = Start.Y;
	int length = 19, width = 10;
	DrawHorizontalLine({ X,Y }, length - 1, L'─');
	DrawHorizontalLine({ X,Y + width }, length - 1, L'─');
	DrawVerticalLine({ X,Y + 1 }, width - 1, L'│');
	DrawVerticalLine({ X + length,Y + 1 }, width - 1, L'│');
	gotoXY(X, Y);
	cout << "┌";
	gotoXY(X + length, Y);
	cout << "┐ ";
	gotoXY(X, Y + width);
	cout << "└";
	gotoXY(X + length, Y + width);
	cout << "┘ ";

	DrawHorizontalLine({ X + 1,Y + 2 }, length - 1, L'─');

	gotoXY(X + (10 - title.size()), Y + 1);
	wcout << title << " ";
	SetColor(Color::white, Color::blue, true);
	wcout << "X" << n;
	SetColor(15);
	int x = X + 2;
	int y = Y + 3;
	gotoXY(x, y);
	for (auto c : content)
	{
		wcout << c;
		x++;
		if (x == X + 10)
		{
			x = X + 2;
			y++;
			gotoXY(x, y);
		}
	}
}

void UI::ShowMainNameColor()
{
	gotoXY(18, 28);
	SetColor(159);
	cout << "            ";
	SetColor(47);
	cout << "            ";
	SetColor(79);
	cout << "            ";
	SetColor(207);
	cout << "            ";
	SetColor(95);
	cout << "            ";
	SetColor(223);
	cout << "            ";
	SetColor(96);
	cout << "            ";
	SetColor(224);
	cout << "            ";
	gotoXY(18, 29);
	SetColor(15);
	cout << "請輸入玩家名稱：  ";
}

//在中間的控制板顯示地圖物件資訊
void UI::ShowMapObjInformation(const MapObj& item)
{
	int outputY = 14;
	ShowBuilding(0, 0, 22, 33);
	SetColor(Color::white, Color::black, true);
	DrawEllipseButton({ 72,15 }, 13, 5);
	SetColor(Color::black, Color::white, false, true);
	DrawHorizontalLine({ 74,16 }, 12, L' ');
	DrawHorizontalLine({ 74,17 }, 12, L' ');
	DrawHorizontalLine({ 74,18 }, 12, L' ');
	gotoXY(77, 17);
	cout << "回選單";
	_setmode(_fileno(stdout), _O_U16TEXT);

	outputY += 2;
	gotoXY(19, outputY);

	SetColor(Color::white, Color::black, true);
	wcout << L"➤ 名稱： " << item.getName();
	outputY += 2;
	gotoXY(19, outputY);

	wcout << L"➤ 地價： ";
	//為起點、機會、命運
	if (item.getType() <= 0)
	{
		wcout << L"無法購買";
	}
	else
	{
		OutputMoney(item.getVal(), false);
	}
	outputY += 2;
	gotoXY(19, outputY);
	wcout << L"➤ 障礙物： ";
	//沒有障礙物
	switch (item.getBarrier())
	{
	case 0:
		wcout << L"無";
		break;
	case 1:
		SetColor(Color::white, Color::red, true);
		wcout << L" 路障 ";
		SetColor(15);
		break;
	}

	outputY += 2;
	gotoXY(19, outputY);
	wcout << L"➤ 所有人： ";
	_setmode(_fileno(stdout), _O_TEXT);
	//無所有人
	if (!(~item.getOwner()))
	{
		wcout << L"無";
		gotoXY(0, 50);
		return;
	}
	else
	{
		SetColor(Player::Instance[item.getOwner()].getColor());
		wcout << L" " << Player::Instance[item.getOwner()].getName() << L" ";
		SetColor(Color::white, Color::black, true);
	}


	_setmode(_fileno(stdout), _O_U16TEXT);
	outputY += 2;
	gotoXY(24, outputY);
	wcout << L"✔ 建築種類： " << item.getTypeName();
	if (item.getLv() != 0)
	{
		wcout << L" (" << TypeName[item.getType() - 1] << " LV." << item.getLv() << L")";
	}
	else
	{
		wcout << L"(LV.0)";
	}
	outputY += 2;
	gotoXY(24, outputY);
	wcout << L"✔ 過路費：";
	OutputMoney(item.getToll(item.getLv()), false);
	outputY += 2;
	gotoXY(24, outputY);
	if (item.getLv() == 3)
	{
		wcout << L"✔ 升至下一級所需金錢： 等級MAX!";
	}
	else
	{
		wcout << L"✔ 升至下一級所需金錢(LV." << item.getLv() << L" -> LV." << item.getLv() + 1 << L")：";
		OutputMoney(item.getUpgradeFee(), false);
	}

	outputY += 2;
	gotoXY(24, outputY);
	wcout << L"✔ 總價值：";
	OutputMoney(item.getTotalVal(), false);




	_setmode(_fileno(stdout), _O_TEXT);
	gotoXY(0, 50);
	return;

}


void UI::OutputMoney(int Money, bool ChangeMode)
{
	int tmp = colorCode;
	if (ChangeMode)
	{
		_setmode(_fileno(stdout), _O_U16TEXT);
	}
	SetColor(Color::yellow, Color::black);
	wcout << L" ¥ " << Money;
	if (ChangeMode)
	{
		_setmode(_fileno(stdout), _O_TEXT);
	}
	SetColor(tmp);
}

void UI::drawStockMarket(int playerID, Stock* stock)
{
	for (int i : { 0, 1, 2, 3, 4, 5 })
	{
		UI::getInstance()->gotoXY(20, 18 + 2 * i);

		if (stock[i].getStatus() == 1)UI::getInstance()->SetColor(Color::white, Color::red);
		else if (stock[i].getStatus() == -1)UI::getInstance()->SetColor(Color::white, Color::green);
		else UI::getInstance()->SetColor(Color::white, Color::black);
		int temp = 0;
		for (auto j : stock[i].getName())if (j > 122)temp++;
		wcout << L" " << left << setw(11 - temp) << stock[i].getName() << L"|";
		wcout << right << setw(9) << fixed << setprecision(2) << stock[i].getTradedValue() << L" |";

		if (!stock[i].getStatus())
		{
			if (stock[i].getValueDifference() < 0)UI::getInstance()->SetColor(Color::green, Color::black, 1, 0);
			else if (stock[i].getValueDifference() > 0)UI::getInstance()->SetColor(Color::red, Color::black, 1, 0);
		}
		wcout << right << setw(7) << fixed << setprecision(2) << stock[i].getValueDifference();

		if (stock[i].getStatus() == 1)UI::getInstance()->SetColor(Color::white, Color::red);
		else if (stock[i].getStatus() == -1)UI::getInstance()->SetColor(Color::white, Color::green);
		else UI::getInstance()->SetColor(Color::white, Color::black);
		wcout << L" |" << right << setw(9) << stock[i].getTradedNum() << L" |";
		wcout << right << setw(9) << Player::Instance[playerID].getStockNum(i) << L" |";
		wcout << right << setw(11) << fixed << setprecision(2) << Player::Instance[playerID].getStockCost(i) << L" ";
	}
	gotoXY(0, 50);
}


//更新現在遊戲狀態：輪到誰、剩幾回合
void UI::UpdateNowInformation(int colorCode, wstring name, int round)
{
	SetColor(colorCode);
	gotoXY(29, 8);
	wcout << L" " << name << L" ";
	SetColor(Color::black, Color::white, false, true);
	gotoXY(29, 10);
	wcout << L"           ";
	gotoXY(29, 10);
	if (round >= 10)
	{
		wcout << Number[round / 10] << Number[round % 10];
	}
	else
	{
		wcout << Number[round];
	}
	gotoXY(0, 50);
}

//進行投擲骰子的動畫
void  UI::rollDiceAnimation(int number, int lastNumber, int nowDice, int sumDice, bool outerframe)
{
	SetColor(Color::white, Color::black, true);
	int LX = 46, LY = 34;
	if (sumDice == 2)
	{
		if (nowDice == 1)
			LX = 36;
		else
			LX = 56;
	}
	else if (sumDice == 3)
	{
		if (nowDice == 1)
			LX = 26;
		else if (nowDice == 2)
			LX = 46;

		else if (nowDice == 3)
			LX = 66;
	}
	Dice({ LX,LY }, number, lastNumber, outerframe);
}
void UI::Dice(const Coordinate& start, int number, int lastNumber, bool outerframe)
{
	int LX = start.X, LY = start.Y;
	static vector<vector<Coordinate>> dice = {
		{Coordinate{6,3}},
		{Coordinate{2,1},Coordinate{10,5}},
		{Coordinate{2,1},Coordinate{6,3},Coordinate{10,5}},
		{Coordinate{2,1},Coordinate{2,5},Coordinate{10,1},Coordinate{10,5}},
		{Coordinate{2,1},Coordinate{2,5},Coordinate{6,3},Coordinate{10,1},Coordinate{10,5}},
		{Coordinate{2,1},Coordinate{2,3},Coordinate{2,5},Coordinate{10,1},Coordinate{10,3},Coordinate{10,5}},
	};


	if (outerframe)
	{

		SetColor(Color::black, Color::white, false, true);

		for (int y = 0; y < 7; y++)
		{
			DrawHorizontalLine({ LX , LY + y }, 14, L' ');
		}
	}

	SetColor(Color::black, Color::white, false, true);
	for (auto c : dice[lastNumber])
	{
		gotoXY(LX + c.X, LY + c.Y);
		cout << " ";
	}

	if (number == 0)
	{
		SetColor(Color::red, Color::white, true, true);
	}
	else
	{
		SetColor(Color::black, Color::white, false, true);
	}

	for (auto c : dice[number])
	{
		gotoXY(LX + c.X, LY + c.Y);
		cout << "●";
	}
	gotoXY(0, 50);
}

//顯示大富翁地圖
void UI::ShowMap()
{
	SetColor(Color::white, Color::black, true);
	int UP = 0;

	//畫外框
	DrawFrame({ 0,UP }, 105, 49);
	DrawVerticalLine({ 13,UP + 1 }, 47);
	DrawVerticalLine({ 91,UP + 1 }, 47);
	gotoXY(0, 6);
	cout << "╠═══════════════════════════════════════════════════════════════════════════════════════════════════════╣ ";
	gotoXY(0, 42);
	cout << "╠═══════════════════════════════════════════════════════════════════════════════════════════════════════╣ ";

	for (int x = 13; x < 104; x += 13)
	{
		gotoXY(x, 0);
		cout << "╦";
		DrawVerticalLine({ x,1 }, 5);
		gotoXY(x, 6);
		cout << "╩";
	}
	for (int Y = 12; Y <= 42; Y += 6)
	{
		gotoXY(0, Y);
		cout << "╠";
		DrawHorizontalLine({ 2,Y }, 11);
		cout << "╣ ";
		gotoXY(91, Y);
		cout << "╠";
		DrawHorizontalLine({ 92,Y }, 12);
		cout << "╣ ";
	}

	for (int x = 13; x < 104; x += 13)
	{
		gotoXY(x, 42);
		cout << "╦";
		DrawVerticalLine({ x,43 }, 5);
		gotoXY(x, 48);
		cout << "╩";
	}
	gotoXY(13, UP + 6);
	cout << "╬";
	gotoXY(13, UP + 42);
	cout << "╬";
	gotoXY(91, UP + 6);
	cout << "╬";
	gotoXY(91, UP + 42);
	cout << "╬";
}


//設置游標位置：左上角為(0,0)，向右為+X軸 向下為+Y軸
//參數： (x,y)
void UI::gotoXY(int X, int Y)
{
	if (X >= 0 && Y >= 0)
		SetConsoleCursorPosition(UI::OUTPUT_HANDLE, { static_cast<short>(X),static_cast<short>(Y) });
}
void UI::gotoXY(const Coordinate& pos)
{
	if (pos.X >= 0 && pos.Y >= 0)
		SetConsoleCursorPosition(UI::OUTPUT_HANDLE, { static_cast<short>(pos.X),static_cast<short>(pos.Y) });
}


//設置字的顏色
//輸入： front：字的顏色 back：背景顏色 frontLight：字是否加亮(預設否) backLight：背景是否加亮(預設否)
void UI::SetColor(Color front, Color back, bool frontLight, bool backLight)
{
	colorCode = back * 16 + front;
	if (front != Color::gray && frontLight)
	{
		colorCode += 8;
	}
	if (back != Color::gray && backLight)
	{
		colorCode += 128;
	}
	SetConsoleTextAttribute(UI::OUTPUT_HANDLE, colorCode);
}
void UI::SetColor(int colorCode)
{
	this->colorCode = colorCode;
	SetConsoleTextAttribute(UI::OUTPUT_HANDLE, colorCode);
}

//畫一個橢圓形的按鈕
void UI::DrawEllipseButton(const Coordinate& Start, int length, int width, bool fill)
{
	gotoXY(Start);

	//上面的直線
	cout << "╭ ";
	DrawHorizontalLine({ Start.X + 2, Start.Y }, length - 2, L'─');
	cout << " ╮ ";

	//下面的直線
	gotoXY(Start.X, Start.Y + width - 1);
	cout << "╰ ";
	DrawHorizontalLine({ Start.X + 2, Start.Y + width - 1 }, length - 2, L'─');
	cout << " ╯ ";

	//左邊垂直線
	DrawVerticalLine({ Start.X, Start.Y + 1 }, width - 2, L'│');
	//右邊垂直線
	DrawVerticalLine({ Start.X + length + 1, Start.Y + 1 }, width - 2, L'│');

	//暫存現在的顏色
	int colorCode = this->colorCode;

	if (fill)
	{
		SetColor(Color::black, Color::white, false, true);
	}
	else
	{
		SetColor(Color::white, Color::black);
	}

	for (int y = Start.Y + 1; y < Start.Y + width - 1; y++)
	{
		DrawHorizontalLine({ Start.X + 2, y }, length - 1, L' ');
	}

	//變回原本的顏色
	SetColor(colorCode);
}

//畫一個格子：注意!由於元素為wchar_t，故長(->)每一單位為2bytes，而寬(向下)每一單位為1byte
void UI::DrawFrame(const Coordinate& Start, int length, int width)
{
	gotoXY(Start);

	//上面的直線
	cout << "╔";
	DrawHorizontalLine({ Start.X + 1, Start.Y }, length - 2);
	cout << "╗ ";

	//下面的直線
	gotoXY(Start.X, Start.Y + width - 1);
	cout << "╚";
	DrawHorizontalLine({ Start.X + 1, Start.Y + width - 1 }, length - 2);
	cout << "╝ ";

	//左邊垂直線
	DrawVerticalLine({ Start.X, Start.Y + 1 }, width - 2);
	//右邊垂直線
	DrawVerticalLine({ Start.X + length - 1, Start.Y + 1 }, width - 2);

	//暫存現在的顏色
	int colorCode = this->colorCode;

	SetColor(Color::white, Color::black);
	for (int y = Start.Y + 1; y < Start.Y + width - 1; y++)
	{
		DrawHorizontalLine({ Start.X + 2, y }, length - 3, L' ');
	}

	//變回原本的顏色
	SetColor(colorCode);
}

//畫一條直線：注意!由於元素為wchar_t，故每長度為2byte(全形)
void UI::DrawHorizontalLine(const Coordinate& Start, int length, wchar_t element)
{
	gotoXY(Start);
	for (int i = 0; i < length; i++)
	{
		wcout << element;
	}
}

//畫一條垂直線：注意!由於元素為wchar_t，故每長度為2byte(全形)
void UI::DrawVerticalLine(const Coordinate& Start, int length, wchar_t element)
{
	for (int i = 0, y = Start.Y; i < length; i++, y++)
	{
		gotoXY(Start.X, y);
		wcout << element;
		cout << " ";
	}
}

void UI::UpdateMapObj(const MapObj& item)
{
	int X = list[item.getPos()].X;
	SetColor(Color::white, Color::black, true);
	//計算名字的放置位置
	auto tmp = item.getName();
	int length = 0;
	for (auto c : tmp)
	{
		if (c <= 122)
		{
			length++;
		}
		else
		{
			length += 2;
		}
	}
	gotoXY(X + ((10 - length) / 2), list[item.getPos()].Y);
	wcout << item.getName();
	//有障礙物
	if (item.getBarrier() != 0)
	{
		SetColor(Color::red, Color::black, true);
		wcout << L"！";
		SetColor(15);
	}
	else
	{
		wcout << L"　";
	}


	for (int i = 0; i < 3; i++)
	{
		gotoXY(X + 2 + i * 3, list[item.getPos()].Y + 4);
		cout << "|";
	}

	//若有所有人
	if (item.getOwner() != -1)
	{
		SetColor(15);
		gotoXY(list[item.getPos()].X, list[item.getPos()].Y + 2);
		cout << "           ";
		SetColor(Player::Instance[item.getOwner()].getColor());
		//計算建築種類的放置位置
		auto tmp = item.getTypeName() + L"(" + to_wstring(item.getLv()) + L")";
		int length = 0;
		for (auto c : tmp)
		{
			if (c <= 122)
			{
				length++;
			}
			else
			{
				length += 2;
			}
		}
		gotoXY(list[item.getPos()].X + ((11 - length) / 2), list[item.getPos()].Y + 2);
		wcout << tmp;
	}
	else
	{
		SetColor(15);
		gotoXY(list[item.getPos()].X, list[item.getPos()].Y + 2);
		cout << "           ";
	}
}

void UI::ShowControlPlayer(const Player& player)
{
	SetColor(15);
	DrawHorizontalLine({ 16,30 }, 24, L'─');
	DrawVerticalLine({ 40,15 }, 15, L'│');
	gotoXY(40, 30);
	cout << "┘ ";
	gotoXY(40, 14);
	cout << "┬";
	gotoXY(15, 30);
	cout << "├";
	ShowAvatar(player.getRole(), 17, 15);

	int OutputY = 14;

	_setmode(_fileno(stdout), _O_U16TEXT);

	SetColor(15);
	OutputY += 2;
	gotoXY(45, OutputY);
	wcout << L"➤ 玩家： ";
	SetColor(player.getColor());
	wcout << L" " << player.getName() << L" ";

	SetColor(Color::white, Color::black, true);
	OutputY += 2;
	gotoXY(45, OutputY);
	wcout << L"➤ 現金：";
	OutputMoney(player.getMoney(), false);
	wcout << L"    ";

	OutputY += 2;
	gotoXY(45, OutputY);
	wcout << L"➤ 魔力： ";

	_setmode(_fileno(stdout), _O_TEXT);
	int manaSum = 100, NowMana = player.getMana();
	int blueLong = (int)(32 * ((double)NowMana / manaSum));
	SetColor(Color::white, Color::blue, true);
	DrawHorizontalLine({ 56 , OutputY }, blueLong, L' ');

	int grayLong = 32 - blueLong;
	SetColor(Color::white, Color::gray, true);

	DrawHorizontalLine({ 56 + blueLong, OutputY }, grayLong, L' ');
	string sum = to_string(manaSum), Mana = to_string(NowMana);
	string output = "";

	for (int i = Mana.size(); i < 3; i++)
	{
		output += " ";
	}
	output += Mana;
	output += "  /  ";
	for (int i = sum.size(); i < 3; i++)
	{
		output += " ";
	}
	output += sum;

	SetColor(Color::white, Color::blue, true);
	gotoXY(66, OutputY);
	for (int i = 0; i < output.size(); i++)
	{
		if (66 + i > (56 + blueLong))
		{
			SetColor(Color::white, Color::gray, true);
		}
		cout << output[i];
	}

	_setmode(_fileno(stdout), _O_U16TEXT);
	SetColor(Color::white, Color::black, true);

	OutputY += 2;
	gotoXY(45, OutputY);
	wcout << L"➤ 技能： " << L"1. ";
	auto role_skill = player.getSkill();
	wcout << role_skill.firstSkill.first;

	OutputY += 2;
	gotoXY(55, OutputY);
	wcout << L"2. ";
	wcout << role_skill.SecondSkill.first;


	gotoXY(45, OutputY + 2);
	wcout << L"➤ 狀態： ";

	SetColor(15);
	auto status = player.getStatus();
	//沒有狀態
	if (status.empty())
	{
		wcout << L"無";
	}
	else
	{
		for (auto s : status)
		{
			OutputY += 2;
			gotoXY(55, OutputY);
			switch (s.status)
			{
			case Motion::PlayerStatus::Car:
				SetColor(Color::green, Color::black, true);
				wcout << L"駕駛汽車";
				break;
			case Motion::PlayerStatus::Stay:
				SetColor(Color::red, Color::black, true);
				wcout << L"陷入蕉著";
				break;
			case Motion::PlayerStatus::Bear_skill:
				SetColor(Color::green, Color::black, true);
				wcout << L"向著蘿○前進";
				break;
			case Motion::PlayerStatus::Bear_The_World:
				SetColor(Color::red, Color::black, true);
				wcout << L"薩瓦魯多";
				break;
			case Motion::PlayerStatus::Glasses_sit:
				SetColor(Color::green, Color::black, true);
				wcout << L"  絕食靜坐    ";
				break;
			}
			SetColor(15);
			wcout << L" (剩餘" << Number[s.statusTimes] << L"回合)";
		}
	}
	_setmode(_fileno(stdout), _O_TEXT);
	SetColor(15);
	gotoXY(0, 50);
}

//顯示是否購買升級這塊地，由此函式自動抓取MapObj的LV以顯示升級畫面或購買
//參數：MapObj
void UI::ShowAskSpend(const MapObj& item)
{
	SetColor(15);
	//空地(即購買行為)
	if (item.getOwner() == -1)
	{
		gotoXY(28 + (5 - item.getName().size()), 32);
		wcout << item.getName() << L" 沒有所有人，價格為";
		OutputMoney(item.getVal());
		wcout << L"，是否要購買？";
		DrawEllipseButton({ 21,34 }, 25, 7);
		gotoXY(33, 37);
		cout << "購買";
		DrawEllipseButton({ 56,34 }, 25, 7);
		gotoXY(68, 37);
		cout << "放棄";
	}
	//升級行為
	else
	{
		if (item.getLv() >= 1)
		{
			gotoXY(20 + (5 - item.getName().size()), 32);
			wcout << setw(5) << right << item.getName() << L" 為您的" << TypeName[item.getType() - 1] << L"，由 LV." << item.getLv() << L" -> LV." << item.getLv() + 1 << L" 需花費";
			OutputMoney(item.getToll(item.getLv() + 1));
			wcout << L"，是否要升級？";
			DrawEllipseButton({ 21,34 }, 25, 7);
			gotoXY(33, 37);
			cout << "升級";
			DrawEllipseButton({ 56,34 }, 25, 7);
			gotoXY(68, 37);
			cout << "放棄";
		}
		//可以選擇建築類別
		else
		{
			gotoXY(20 + (5 - item.getName().size()), 32);

			wcout << item.getName() << L" 為您的所有地，您想在上面修建什麼建築 (選定後即無法修改)？";
			DrawEllipseButton({ 18,34 }, 19, 7);
			DrawEllipseButton({ 42,34 }, 19, 7);
			DrawEllipseButton({ 66,34 }, 19, 7);

			gotoXY(27, 37);
			wcout << L"民房";

			gotoXY(51, 37);
			wcout << L"商店";

			gotoXY(75, 37);
			wcout << L"賭場";
		}

	}
	gotoXY(0, 50);
}

//更新地圖上玩家的位置
//參數：Pos：位置、PlayerIndex：玩家編號、bool Clear：是要清除還是顯示(預設false：即不清除)
void UI::UpdatePlayerPos(int pos, int playerIndex, bool clear)
{

	gotoXY(list[pos].X + (playerIndex * 3), list[pos].Y + 4);
	if (clear)
	{
		//白字黑底
		SetColor(15);
		cout << "　";
	}
	else
	{
		SetColor(Player::Instance[playerIndex].getColor());
		wcout << Number[(size_t)(playerIndex + 1)];
	}
}

//在中央顯示玩家的詳細資訊
void UI::ShowPlayerInformation(const Player& player)
{
	int X = 18;
	int OutputY = 14;
	SetColor(Color::white, Color::black, true);
	DrawHorizontalLine({ 64,30 }, 24, L'─');
	DrawVerticalLine({ 64,15 }, 15, L'│');
	gotoXY(64, 30);
	cout << "└ ";
	gotoXY(64, 14);
	cout << "┬";
	gotoXY(89, 30);
	cout << "┤";
	ShowAvatar(player.getRole(), 66, 15);


	_setmode(_fileno(stdout), _O_U16TEXT);
	SetColor(Color::white, Color::black, true);
	OutputY += 2;
	gotoXY(X, OutputY);
	wcout << L"➤ 玩家： ";
	SetColor(player.getColor());
	wcout << L" " << player.getName() << L" ";

	SetColor(Color::white, Color::black, true);
	OutputY += 2;
	gotoXY(X, OutputY);
	wcout << L"➤ 現金：";
	OutputMoney(player.getMoney(), false);


	OutputY += 2;
	gotoXY(X, OutputY);
	wcout << L"➤ 銀行存款：";
	OutputMoney(player.getDeposit(), false);

	OutputY += 2;
	gotoXY(X, OutputY);
	SetColor(Color::white, Color::black);
	wcout << L"➤ 魔力： ";

	_setmode(_fileno(stdout), _O_TEXT);
	int manaSum = 100, NowMana = player.getMana();
	int blueLong = (int)(32 * ((double)NowMana / manaSum));
	SetColor(Color::white, Color::blue, true);
	DrawHorizontalLine({ X + 11 , OutputY }, blueLong, L' ');

	int grayLong = 32 - blueLong;
	SetColor(Color::white, Color::gray, true);

	DrawHorizontalLine({ X + 11 + blueLong, OutputY }, grayLong, L' ');
	string sum = to_string(manaSum), Mana = to_string(NowMana);
	string output = "";

	for (int i = Mana.size(); i < 3; i++)
	{
		output += " ";
	}
	output += Mana;
	output += "  /  ";
	for (int i = sum.size(); i < 3; i++)
	{
		output += " ";
	}
	output += sum;

	SetColor(Color::white, Color::blue, true);
	gotoXY(X + 21, OutputY);
	for (int i = 0; i < output.size(); i++)
	{
		if (X + 21 + i > (X + 11 + blueLong))
		{
			SetColor(Color::white, Color::gray, true);
		}
		cout << output[i];
	}

	_setmode(_fileno(stdout), _O_U16TEXT);
	SetColor(Color::white, Color::black, true);
	OutputY += 2;
	gotoXY(X, OutputY);
	wcout << L"➤ 總土地數： " << player.getHouseNum();
	OutputY += 2;
	gotoXY(X, OutputY);
	wcout << L"➤ 技能： 1. ";
	auto skill_name = player.getSkill();
	wcout << skill_name.firstSkill.first;

	OutputY += 2;
	gotoXY(X + 10, OutputY);
	wcout << L"2. ";
	wcout << skill_name.SecondSkill.first;


	OutputY += 2;
	gotoXY(X, OutputY + 2);
	SetColor(15);
	wcout << L"➤ 借款：";
	OutputMoney(player.getLoan(), false);
	if (player.getLoan() != 0)
	{
		wcout << L" (剩餘還款天數：";
		auto round = player.getLoan_Round();
		while (round >= 10)
		{
			wcout << round / 10;
			round %= 10;
		}
		wcout << round << L" 天)";
	}



	OutputY += 2;
	gotoXY(X, OutputY + 2);
	SetColor(15);
	wcout << L"➤ 狀態： ";

	auto status = player.getStatus();
	//沒有狀態
	if (status.empty())
	{
		wcout << L"無";
	}

	else
	{
		for (auto s : status)
		{
			OutputY += 2;
			gotoXY(X + 10, OutputY);
			switch (s.status)
			{
			case Motion::PlayerStatus::Car:
				SetColor(Color::green, Color::black, true);
				wcout << L"駕駛汽車";
				break;
			case Motion::PlayerStatus::Stay:
				SetColor(Color::red, Color::black, true);
				wcout << L"陷入蕉著";
				break;

			case Motion::PlayerStatus::Bear_skill:
				SetColor(Color::green, Color::black, true);
				wcout << L"向著蘿○前進";
				break;
			case Motion::PlayerStatus::Bear_The_World:
				SetColor(Color::red, Color::black, true);
				wcout << L"薩瓦魯多";
				break;
			case Motion::PlayerStatus::Glasses_sit:
				SetColor(Color::green, Color::black, true);
				wcout << L"  絕食靜坐    ";
				break;
			}
			SetColor(15);
			wcout << L" (剩餘" << Number[s.statusTimes] << L"回合)";
		}
	}

	_setmode(_fileno(stdout), _O_TEXT);
	SetColor(15);
	gotoXY(0, 50);
}

void UI::ClearControl()
{
	SetColor(Color::white, Color::black, true);
	for (int y = 15; y < 41; y++)
	{
		gotoXY(17, y);
		for (int i = 0; i < 72; i++)
		{
			cout << " ";
		}
		//DrawHorizontalLine({ 17,y }, 72, L' ');
	}
	gotoXY(40, 14);
	cout << "─";
	gotoXY(15, 30);
	cout << "│ ";

	gotoXY(64, 14);
	cout << "─";
	gotoXY(89, 30);
	cout << "│ ";


	gotoXY(15, 28);
	cout << "│ ";
	gotoXY(89, 28);
	cout << "│ ";
}

void UI::ClearButton()
{
	SetColor(Color::white, Color::black, true);
	for (int y = 31; y < 41; y++)
	{
		DrawHorizontalLine({ 17,y }, 72, L' ');
	}
	DrawHorizontalLine({ 74,30 }, 11, L' ');
}

void UI::ClearMainBanner()
{
	SetColor(Color::white, Color::black, true);
	for (int y = 1; y < 17; y++)
	{
		DrawHorizontalLine({ 2,y }, 128, L' ');
	}
}

void UI::ClearMainBottom()
{
	SetColor(Color::white, Color::black, true);
	for (int y = 17; y < 47; y++)
	{
		DrawHorizontalLine({ 2,y }, 128, L' ');
	}
}

void UI::ClearMainNameColor()
{
	SetColor(15);
	gotoXY(18, 28);
	cout << "                                                                                                            ";
	gotoXY(18, 29);
	cout << "請輸入玩家名稱：                       ";
}
void UI::WriterAnimation(const Coordinate& start, const wstring& str, int delay)
{
	gotoXY(start);
	for (auto c : str)
	{
		wcout << c;
		Sleep(delay);
	}
}

void UI::ClearPlayerInformation()
{
	SetColor(Color::white, Color::black, true);
	for (int y = 15; y < 31; y++)
	{
		DrawHorizontalLine({ 44, y }, 44, L' ');
	}
}
//顯示右邊的玩家資訊
//參數：exist：這格有沒有玩家、PlayerIndex：第幾個玩家
void UI::UpdatePlayerStatus(int PlayerIndex, bool exist)
{
	SetColor(Color::white, Color::black);
	Player& player = Player::Instance[PlayerIndex];
	int Y = 1 + PlayerIndex * 12;
	//若不存在
	if (!exist || player.isbankrupt())
	{
		for (int y = 0; y < 11; y++)
		{
			DrawHorizontalLine({ 109,Y + y }, 22, L' ');
		}
		for (int x = 109, y = Y; x < 131; y++, x += 2)
		{
			gotoXY(x, y);
			cout << "╲";
		}
	}
	else
	{
		int Y = 1 + PlayerIndex * 12;
		SetColor(player.getColor());
		gotoXY(109, Y);
		cout << " ";
		wcout << setw(21) << left << player.getName();

		SetColor(Color::white, Color::black, true);
		gotoXY(109, Y + 2);

		cout << "角色： ";
		wcout << roleName[player.getRole()];

		gotoXY(113, Y + 4);
		wcout << L"              ";
		gotoXY(109, Y + 4);
		wcout << L"現金：";

		OutputMoney(player.getMoney());

		gotoXY(113, Y + 6);
		wcout << L"              ";
		gotoXY(109, Y + 6);
		wcout << L"銀行存款：";
		OutputMoney(player.getDeposit());
		for (int i = to_string(player.getDeposit()).size(); i < 9; i++)
		{
			wcout << L" ";
		}


		gotoXY(109, Y + 8);
		cout << "狀態： ";
		auto status = player.getStatus();
		//沒有狀態
		if (status.empty())
		{
			wcout << L"無             ";
			gotoXY(114, Y + 9);
			wcout << L"               ";
		}
		else
		{
			switch (status[0].status)
			{
			case Motion::PlayerStatus::Car:
				SetColor(Color::green, Color::black, true);
				wcout << L"  駕駛汽車    ";
				break;
			case Motion::PlayerStatus::Stay:
				SetColor(Color::red, Color::black, true);
				wcout << L"  陷入蕉著    ";
				break;
			case Motion::PlayerStatus::Bear_skill:
				SetColor(Color::green, Color::black, true);
				wcout << L"向著蘿○前進";
				break;
			case Motion::PlayerStatus::Bear_The_World:
				SetColor(Color::red, Color::black, true);
				wcout << L"  薩瓦魯多    ";
				break;
			case Motion::PlayerStatus::Glasses_sit:
				SetColor(Color::green, Color::black, true);
				wcout << L"  絕食靜坐    ";
				break;
			}
			gotoXY(116, Y + 9);
			SetColor(15);
			wcout << L"(剩餘" << Number[status[0].statusTimes] << L"回合)";
		}
	}
	gotoXY(0, 50);
}

void UI::showAvatarIntro(int avatarID)
{
	for (int i = 0; i < 13; i++)
	{
		gotoXY(20, 33 + i);
		cout << "                                                                           ";
	}
	SetColor(15);
	switch (avatarID)
	{
	case 0:
		gotoXY(20, 33);
		wcout << L"角色名稱：  " << roleName[avatarID];
		gotoXY(20, 35);
		wcout << L"技能介紹：";
		gotoXY(24, 37);
		wcout << L"向著蘿○前進：  第一回合骰子點數X2 第二回合X3 第三回合X4";
		gotoXY(24, 39);
		wcout << L"○莉就在前方：  瞬移到前方的角色位置";

		gotoXY(20, 41);
		wcout << L"角色介紹：";
		gotoXY(24, 43);
		wcout << L"又名蘿○熊 會越走越快";
		break;
	case 1:
		gotoXY(20, 33);
		wcout << L"角色名稱：  " << roleName[avatarID];
		gotoXY(20, 35);
		wcout << L"技能介紹：";
		gotoXY(24, 37);
		wcout << L"立足台灣，胸懷大陸：  升級指定建築";
		gotoXY(24, 39);
		wcout << L"放眼世界，征服宇宙：  收購當下位置的房子";

		gotoXY(20, 41);
		wcout << L"角色介紹：";
		gotoXY(24, 43);
		wcout << L"即將參選總統的兔子 跟著他貌似會發財(?";
		break;
	case 2:
		gotoXY(20, 33);
		wcout << L"角色名稱：  " << roleName[avatarID];
		gotoXY(20, 35);
		wcout << L"技能介紹：";
		gotoXY(24, 37);
		wcout << L"絕食靜坐：  原地停留一回合";
		gotoXY(24, 39);
		wcout << L"退休人員優惠存款：  全體玩家獲取銀行利息18%";

		gotoXY(20, 41);
		wcout << L"角色介紹：";
		gotoXY(24, 43);
		wcout << L"退休了閒閒沒事幹 會到公園做晨操";
		break;
	case 3:
		gotoXY(20, 33);
		wcout << L"角色名稱：  " << roleName[avatarID];
		gotoXY(20, 35);
		wcout << L"技能介紹：";
		gotoXY(24, 37);
		wcout << L"薩瓦魯多：  使其他人失去一回合的時間";
		gotoXY(24, 39);
		wcout << L"吃我的壓路機WRRYYYY：  把一塊地夷為平地";

		gotoXY(20, 41);
		wcout << L"角色介紹：";
		gotoXY(24, 43);
		wcout << L"有時候會感覺它背後有個奇怪站姿的影子 可愛的外表下有強大的實力!?";
		break;
	case 4:
		gotoXY(20, 33);
		wcout << L"沒選角色看個毛介紹= =";
		break;
	}
	gotoXY(0, 50);
}