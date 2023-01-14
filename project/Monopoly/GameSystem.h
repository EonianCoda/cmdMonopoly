#pragma once
#include"UI.h"
#include<ctime>
#include<fstream>
#include<conio.h>
#include<cctype>
namespace System {
	enum Status {
		GameTop, //輪到某位玩家，且他還沒進行任何動作
		SelectDice, //選擇色子
		RollDice, //投擲骰子
		UseCard, //使用卡片
		UseSkill, //使用技能

		askBuy, //買地事件
		askSelectBuilding, //選擇建築類型事件
		askLVup, //升級房子事件
		Casino, //賭場

		MapObj, //細看某個格子的資訊
		IKnow, //我知道了

		Bank, //進入銀行
		WithDraw, //提款
		SaveMoney, //存款
		Loan, //借款
		Repay,//還款

		StockMarket,//進股市
		BuyStock,
		SellStock,

		Card_Dice, //遙控骰子
		Card_banana, //陷入蕉著
		Card_barrier, //放置路障
		Card_wind, //旋風卡

		Skill_roller, //吃我的壓路機
		Skill_upgrade, //立足台灣，胸懷大陸：升級指定建築

		Game_End,//遊戲結束
		Menu, //選單

		GameSave,

		MainBeginning,//開始遊戲 音效 離開遊戲
		MainSelectMode, //新遊戲 開啟舊檔
		MainSelectFile, //選擇舊檔
		MainNewMap, //選擇新地圖
		MainNewCharater, //選擇腳色 顏色
		MainSetNameColor,
		MainInputTotalMoney,
		MainInputDays,
		MainSetSound,

		Win, //贏得遊戲
	};

}


class GameSystem {
	static GameSystem* Instance;
	static HANDLE hInput;

	Display::Screen screen;
	System::Status nowStatus;
	System::Status LastStatus;
	int Round; //剩餘回合數
	wstring mapName; //地圖名稱
	int playerNum; //總玩家人數
	int nowPlayer;
	MapObj* Map[28];
	string input;
	string fileName;
	Stock stock[6];
	int nowPage;
	int SumPage;
	int selectedStockID;
	Item selectedCard;
	bool WinCasino;
	int selectedItem;

	int mapID;
	vector<string> mapFiles, savedFiles;
	void scanMapFiles();
	void scanSavedFiles();
	int setPlayerNameColor;
	wstring stringToWstring(string input);
public:
	static GameSystem* getInstance();

	GameSystem();
	~GameSystem();

	void initGame();

	int DetectInformationClick(int X, int Y, bool achieve = true);

	void ContinueGame();
	void MouseInput();
	void PlayerMove(int step, bool sleepable = true);
	void ChangePlayer();
	int rollDice(int diceNum = 1);

	bool checkMainColorRepeat();
	bool LoadMap();
	bool loadNewMap();

	void UseCard(int);
	void UseSkill(int skillID);
	void Win(int index);
	int determinWinner();
	bool Bankrupt();
	void gameSave(string input);

};
