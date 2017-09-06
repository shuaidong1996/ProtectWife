#ifndef __Util_H__
#define __Util_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define winSize CCDirector::sharedDirector()->getWinSize()
#define changeScene(__layer_name__,...) \
	CCDirector::sharedDirector()->replaceScene(Util::createScene(__layer_name__::create(__VA_ARGS__)))

class Util
{
public:
	//����һ���������Ѵ������Ĳ㣬���뵽�����������
	static CCScene* createScene(CCLayer* layer)
	{
		CCScene* s = CCScene::create();
		s->addChild(layer);
		return s;
	}
	//��������ͼƬ
	static CCSprite* addBackageground(const char* picFile, CCNode* parent)
	{
		CCSprite* s = CCSprite::create(picFile);
		parent->addChild(s);
		s->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		return s;
	}
	//����ƶ��ڵ�
	static void MoveNode(CCNode* node, CCPoint& delta)
	{
		node->setPosition(node->getPosition() + delta);
	}
	//ƴ�����ֺ��ַ���
	static char* format(int value, const char* str1 = "", const char* str2 = "")
	{
		static char buf[1024];
		sprintf(buf, "%s%d%s", str1, value, str2);
		return buf;
	}
	//�ж�һ�� �� �Ƿ��� ��� map��
	static bool isContent(const CCPoint& pt, CCNode* node)
	{
		CCRect rt = CCRect(0, 0, node->getContentSize().width, node->getContentSize().height);//��ȡ���Node �� Rect
		return rt.containsPoint(pt);//���Ƿ���������
	}
	//��openGL����  ת��Ϊ  map����
	static CCPoint GL2map(const CCPoint& pt, CCTMXTiledMap* map)
	{
		int mapX = pt.x / 48;
		int mapY = map->getMapSize().height - (int)pt.y / 48 - 1;//ע��ǿ������ת��

		return ccp(mapX, mapY);
	}
};



#endif

