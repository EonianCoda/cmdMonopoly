#pragma once
#pragma comment(lib, "winmm.lib")
#include<Windows.h>
#include<mmsystem.h>
#include<string>
#include<map>
using namespace std;

namespace Music {
	enum MusicMode {
		quiet, //�R��
		normal, //���`
	};

	enum music {
		Error,
		Bankrupt,
		SpendMoney,
		RollDice,
		PeaceBread, //�M���B�g�a�B�ѥ]
	};
}
int getMusicMode();
void SetMusicMode(Music::MusicMode mode);
void playChanceMusic(int ID);
void playMusic(Music::music);
