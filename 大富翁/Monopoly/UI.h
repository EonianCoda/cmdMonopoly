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
		Control, //����O
		SelectDice, //��ܻ�l
		RollDice, //���Y��l
		Button, //�|�ӥ\����s(���Y��l�B�Ѳ������B�ϥΥd���B�ϥΧޯ�)
		Iknow, //�ڪ��D�F
		Bank, //�Ȧ�
		UseCard, //�ϥΥd��
		StockMarket,//�ѥ�
		BuyStock,
		SellStock,
		UseSkill, //�ϥΧޯ�
		Casino, //���

		Menu, //���
		PassRound, //�C�@�^�X������e��

		MainBanner,//MONOPOLY
		MainBeginning,//�}�l�C�� ���� ���}�C��
		MainSelectMode, //�s�C�� �}������
		MainSelectFile, //�������
		MainNewMap, //��ܷs�a��
		MainNewCharater, //��ܸ}�� �C��
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
	void DrawHorizontalLine(const Coordinate& Start, int length, wchar_t element = L'��');
	void DrawVerticalLine(const Coordinate& Start, int length, wchar_t element = L'��');
	void DrawCard(const Coordinate& Start, const wstring& title, const wstring& content, int n = 1);
	void OutputMoney(int Money, bool ChangeMode = true);
	void drawStockMarket(int playerID, Stock* stock);

	void showAvatarIntro(int avatarID);
};

