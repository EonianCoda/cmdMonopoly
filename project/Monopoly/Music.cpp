#include"Music.h"

namespace {
	Music::MusicMode musicMode = Music::MusicMode::normal;
}
int getMusicMode()
{
	return musicMode;
}

void SetMusicMode(Music::MusicMode mode)
{
	musicMode = mode;
}
void playChanceMusic(int ID)
{
	string tmp = "Sound\\Chance_" + to_string(ID) + ".wav";
	if (musicMode == Music::MusicMode::quiet) {
		return;
	}
	else if (musicMode == Music::MusicMode::normal) {
		PlaySound(tmp.c_str(), NULL, SND_FILENAME | SND_ASYNC);
	}

}


void playMusic(Music::music file)
{
	if (musicMode == Music::MusicMode::quiet) {
		return;
	}
	switch (file) {
	case Music::music::Error:
		PlaySound("Sound\\Error.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case Music::music::Bankrupt:
		PlaySound("Sound\\Bankrupt.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case Music::music::SpendMoney:
		PlaySound("Sound\\SpendMoney.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case Music::music::RollDice:
		PlaySound("Sound\\RollDice.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	case Music::music::PeaceBread:
		PlaySound("Sound\\PeaceBread.wav", NULL, SND_FILENAME | SND_ASYNC);
		break;
	}
}