#pragma once
#include"MapObj.h"
#include"StockMarket.h"
#include <fcntl.h>
#include <io.h>
#include<locale.h>
#include<vector>
#include<Windows.h>
#include<iostream>
#include<iomanip>
#include<string>

using namespace std;


typedef struct { int X; int Y; } Coordinate;
namespace Display {
	enum Screen {
		MainMenu,
		Game,
	};
	enum Block {
		PlayerStatus,
		GameStatus,
		Control, //控制板
		SelectDice, //選擇骰子
		RollDice, //投擲骰子
		Button, //四個功能按鈕(投擲骰子、股票市場、使用卡片、使用技能)
		Iknow, //我知道了
		Bank, //銀行
		UseCard, //使用卡片
		StockMarket,//股市
		BuyStock,
		SellStock,
		UseSkill, //使用技能
		Casino, //賭場

		Menu, //選單
		PassRound, //每一回合的結算畫面

		MainBanner,//MONOPOLY
		MainBeginning,//開始遊戲 音效 離開遊戲
		MainSelectMode, //新遊戲 開啟舊檔
		MainSelectFile, //選擇舊檔
		MainNewMap, //選擇新地圖
		MainNewCharater, //選擇腳色 顏色
		MainSetSound,

		GameSave,
	};
}

enum Color {
	black = 0,
	blue = 1,
	green = 2,
	cyan = 3,
	red = 4,
	purple = 5,
	yellow = 6,
	white = 7,
	gray = 8,
};


class UI {
	static HANDLE OUTPUT_HANDLE;
	static UI* Instance;
	int colorCode;

public:
	UI();
	static UI* getInstance();

	void gotoXY(int X, int  Y);
	void gotoXY(const Coordinate& pos);
	void SetColor(Color front, Color back, bool frontLight = false, bool backLight = false);
	void SetColor(int colorCode);

	void ShowScreen(Display::Screen);
	void ShowBlock(Display::Screen, Display::Block);
	void ShowMap();
	void ShowAvatar(const int avatarID, const int X, const int Y);
	void ShowBuilding(int type, int ID, const int X, const int Y);
	void ShowAskSpend(const MapObj&);
	void ShowCard(int playerIndex, int page);
	void ShowControlPlayer(const Player&);
	void ShowMapObjInformation(const MapObj&);
	void ShowPlayerInformation(const Player&);
	void ShowBankWork(int work, const Player&);
	void ShowUseSkill(int playerIndex);
	void ShowFail(const Coordinate& start);
	void ShowMainNameColor();
	void showUnionFlag();

	void ClearControl();
	void ClearPlayerInformation();
	void ClearButton();
	void ClearMainBanner();
	void ClearMainBottom();
	void ClearMainNameColor();



	void UpdateMapObj(const MapObj&);

	void UpdatePlayerStatus(int PlayerIndex, bool exist = true);
	void UpdateNowInformation(int colorCode, wstring name, int round);
	void UpdatePlayerPos(int pos, int playerIndex, bool clear = false);

	void Dice(const Coordinate& start, int number, int lastNumber, bool outerframe);
	void rollDiceAnimation(int number, int lastNumber, int nowDice = 1, int sumDice = 1, bool outerframe = false);
	void WriterAnimation(const Coordinate& start, const wstring& str, int delay = 150);

	void DrawFrame(const Coordinate& Start, int length, int width);
	void DrawEllipseButton(const Coordinate& Start, int length, int width, bool fill = false);
	void DrawHorizontalLine(const Coordinate& Start, int length, wchar_t element = L'═');
	void DrawVerticalLine(const Coordinate& Start, int length, wchar_t element = L'║');
	void DrawCard(const Coordinate& Start, const wstring& title, const wstring& content, int n = 1);
	void OutputMoney(int Money, bool ChangeMode = true);
	void drawStockMarket(int playerID, Stock* stock);

	void showAvatarIntro(int avatarID);
};

