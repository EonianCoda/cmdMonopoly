#pragma once
#include"UI.h"
#include<ctime>
#include<fstream>
#include<conio.h>
#include<cctype>
namespace System {
	enum Status {
		GameTop, //����Y�쪱�a�A�B�L�٨S�i�����ʧ@
		SelectDice, //��ܦ�l
		RollDice, //���Y��l
		UseCard, //�ϥΥd��
		UseSkill, //�ϥΧޯ�

		askBuy, //�R�a�ƥ�
		askSelectBuilding, //��ܫؿv�����ƥ�
		askLVup, //�ɯũФl�ƥ�
		Casino, //���

		MapObj, //�ӬݬY�Ӯ�l����T
		IKnow, //�ڪ��D�F

		Bank, //�i�J�Ȧ�
		WithDraw, //����
		SaveMoney, //�s��
		Loan, //�ɴ�
		Repay,//�ٴ�

		StockMarket,//�i�ѥ�
		BuyStock,
		SellStock,

		Card_Dice, //������l
		Card_banana, //���J����
		Card_barrier, //��m����
		Card_wind, //�ۭ��d

		Skill_roller, //�Y�ڪ�������
		Skill_upgrade, //�ߨ��x�W�A���h�j���G�ɯū��w�ؿv

		Game_End,//�C������
		Menu, //���

		GameSave,

		MainBeginning,//�}�l�C�� ���� ���}�C��
		MainSelectMode, //�s�C�� �}������
		MainSelectFile, //�������
		MainNewMap, //��ܷs�a��
		MainNewCharater, //��ܸ}�� �C��
		MainSetNameColor,
		MainInputTotalMoney,
		MainInputDays,
		MainSetSound,

		Win, //Ĺ�o�C��
	};

}


class GameSystem {
	static GameSystem* Instance;
	static HANDLE hInput;

	Display::Screen screen;
	System::Status nowStatus;
	System::Status LastStatus;
	int Round; //�Ѿl�^�X��
	wstring mapName; //�a�ϦW��
	int playerNum; //�`���a�H��
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
