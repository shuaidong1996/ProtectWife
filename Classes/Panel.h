#ifndef __PANEL_H__
#define __PANEL_H__

#include "Common.h"

//ΪʲôҪ������أ�
//��ΪcocosĬ�� MenuItem ֻ�е�TouchEnd ֮��Ŵ����˵��� ����
//Ŀ�ģ��ı�CCMenu ʹ������ʱ��ִ�д���������ֱ���ɿ�

//��������ھ���  �Լ���CCMenu�ˣ����Զ����˹��ܣ����Ժ�Ҫ������ܾͲ�Ҫ��CCMenu��   ֱ��Panel* menu
//���൱�ڸ�CCMenu �ı���һ������
class Panel:public CCMenu
{
	bool _isBullet;//�ж��ǲ����ӵ���ť  ���µ�
public:
	static Panel* create(bool isBullet = false)
	{
		Panel* p = new Panel;
		p->_isBullet = isBullet;
		p->init();				//ע�����Ƕ����õĸ���Ķ�������仰ǧ�����١�ƽʱ��û�ӣ���β��Ӿͻ����MenuItem����ʧ
		p->autorelease();
		return p;
	}
	//CREATE_FUNC(Panel);

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);

	void update(float);
};

#endif