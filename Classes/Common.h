#ifndef __Common_H__
#define __Common_H__

#include "Macro.h"
#include "Util.h"

enum TILE_TYPE {//tile ��Ƭ
	tileNone,
	tileGrass,
	tileSteel,
	tileWall,
	tileKing,
	tileRiver
};

static TILE_TYPE tileGidType[] = {
	tileNone,
	tileNone, tileNone, tileGrass, tileGrass, tileSteel, tileSteel,
	tileNone, tileNone, tileGrass, tileGrass, tileSteel, tileSteel,
	tileWall, tileWall, tileRiver, tileRiver, tileKing, tileKing,
	tileWall, tileWall, tileRiver, tileRiver, tileKing, tileKing
};

#if 0
//���������ȿ���˯�ˣ��������������⣬����ֲ��android��  fread fwrite����������������
//���˹��ˣ��������һ���һ�׽�������������Ǵ�����5�㣬����˯��
static int getLevel()
{
	FILE* fp = NULL;
	fp = fopen(TXT_level, "rb");
	if (!fp)
		return 1;

	char buf;
	fread(&buf, 1, 1, fp);//������������������Ϊ char buf[0] ֻռһ���ֽڣ���ô�����ַ���Ӧ��ascii�룬�����д�����char ��ô ��Ӧ�÷��� buf[0]-'0'
	
	fclose(fp);
	fp = NULL;

	return buf;
}


static void setLevel(char newLevel)
{
	FILE* fp = NULL;
	fp = fopen(TXT_level, "wb+");
	if (!fp)
		return;

	char buf = newLevel;
	fwrite(&buf, 1, 1, fp);

	fclose(fp);
	fp = NULL;
}

#else

static int getLevel()
{
	//��ȡ��Ϸ����û�ж�ȡ��ʱ����1
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("level", 1);
}
static int getFire1()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("fire1", 2);
}
static int getFire2()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("fire2", 2);
}
static int getFire3()
{
	return CCUserDefault::sharedUserDefault()->getIntegerForKey("fire3", 2);
}

static void setLevel(int newLevel)
{
	/*
	1������һ��xml�ļ�         Ҳ��tiny databases
	2������߷�д��xml�ļ���
	3����ȡxml�ļ��е���߷�
	*/
	//���浽�ڴ���
	CCUserDefault::sharedUserDefault()->setIntegerForKey("level", newLevel);
	
	//д��Ӳ����
	CCUserDefault::sharedUserDefault()->flush();
}
static void setFire1(int num)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("fire1", num);
	CCUserDefault::sharedUserDefault()->flush();
}
static void setFire2(int num)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("fire2", num);
	CCUserDefault::sharedUserDefault()->flush();
}
static void setFire3(int num)
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey("fire3", num);
	CCUserDefault::sharedUserDefault()->flush();
}
#endif

#endif