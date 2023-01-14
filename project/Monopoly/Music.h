#pragma once
#pragma comment(lib, "winmm.lib")
#include<Windows.h>
#include<mmsystem.h>
#include<string>
#include<map>
using namespace std;

namespace Music {
	enum MusicMode {
		quiet, //靜音
		normal, //正常
	};

	enum music {
		Error,
		Bankrupt,
		SpendMoney,
		RollDice,
		PeaceBread, //和平、土地、麵包
	};
}
int getMusicMode();
void SetMusicMode(Music::MusicMode mode);
void playChanceMusic(int ID);
void playMusic(Music::music);
