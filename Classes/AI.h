#ifndef __AI_H__
#define __AI_H__

#include "Common.h"

// ����о��͵о����ӵ�
class AI:public CCLayer
{
public:
	CREATE_FUNC(AI);

	int _countTank;
	CCArray* _tanks;
	CCArray* _bullets;

	void onEnter();
	void onExit();

	//���ڼ���ӵ�
	void update(float);
	//���ڼ��tank
	void addTanks(float);
	//ת������
	void changeDir(float);

	CCTMXTiledMap* getMap()
	{
		return (CCTMXTiledMap*)getParent();
	}
};

#endif