#include "Panel.h"

bool Panel::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	//�ж��Ƿ񱻰���
	bool bTouch = CCMenu::ccTouchBegan(touch, event); //ֱ�ӵ���  �����ccTouchBegan ��������ֻ��Ҫ�ӹ��ܾͿ����ˡ������Ͳ���Ӱ��CCMenu�Ĺ�����
	if (!bTouch) return false;

	//�ж��Ƿ��Ƿ����ӵ�
	if (_isBullet)
	{
		m_pSelectedItem->activate();
	}
	else
		scheduleUpdate();//������ʱ��

	return true;
}
void Panel::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if (_isBullet)//���ӵ� �Ѱ�ťѡ�е�Ч����ȡ������ɡ�û��ѡ�С�  ״̬ҲҪ��ɡ��ȴ���
	{
		if (m_pSelectedItem!=NULL)
			m_pSelectedItem->unselected();
		m_eState = kCCMenuStateWaiting;
		return;
	}

	unscheduleUpdate();
	CCMenu::ccTouchEnded(touch, event);
}
void Panel::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
	unscheduleUpdate();
	CCMenu::ccTouchCancelled(touch, event);
}

void Panel::update(float)
{
	if (!m_pSelectedItem)//��������ж�Ҳ��һ��bug   ���ɰ�ť�ƶ����յ�ʱ����û���κΰ�ť��ѡ�У���ʱ���Ͳ�����m_pSelectedItem->activate();
	{																		//��ʱ m_pSelectedItemΪNULL
		unscheduleUpdate();
		return;
	}

	//CCMenu ִ�д�����������ֻ��Ҫ m_pSelectedItem->activate(); ����
	m_pSelectedItem->activate();//m_pSelectedItem�Ǳ�����Ա��������Է���
}