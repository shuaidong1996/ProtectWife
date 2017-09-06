#include "Music.h"
#include "SimpleAudioEngine.h"


bool Music::init()
{
	CCLayer::init();

	// Ԥ����
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_background);//���ص��ڴ���ȥ��������ִ��
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(MUSIC_startMusic);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_shoot);//���ص��ڴ���ȥ��������ִ��
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_eat1);//���ص��ڴ���ȥ��������ִ��
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_eat2);//���ص��ڴ���ȥ��������ִ��
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(MUSIC_gameover);//���ص��ڴ���ȥ��������ִ��

	return true;
}

void Music::StartLayerMusic()
{
	//����������С
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_startMusic,true);
}
//void Music::resumeStartMusic()
//{
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
//}
//void Music::pauseStartMusic()
//{
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();//backgroundֻ����һ�������Բ�Ҫ����
//}
void Music::StartGameMusic()
{
	//����������С
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_background, true);
}

void Music::stopBackgroundMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void Music::shootedMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_shoot);
}

void Music::eat1()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_eat1);
}
void Music::eat2()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_eat2);
}

void Music::gameOver()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_gameover);
}