#include <Windows.h>
#include "gtest/gtest.h"

class uiTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{

	}
};

//
//rcArea����������
//left,top�����Ͻ����꣬����0Ϊ�̶���ֵ��-1��-100��ʾ��rcArea��Ⱥ͸߶ȵİٷֱȡ�
//width,height����Ⱥ͸߶ȣ�����0Ϊ�̶���ֵ������0��ά�ֲ���
//�����ȼ����С����ȷ�����Ͻǵ�λ��
void CalcRect(RECT rcArea,RECT rcRate,RECT &rcChild, bool mode = FALSE)
{
	//��ʼ������Ⱥ͸߶�
	int widthArea = rcArea.right - rcArea.left + 1;
	int heightArea = rcArea.bottom - rcArea.top + 1;
	int width = rcChild.right - rcChild.left + 1;
	int height = rcChild.bottom - rcChild.top + 1;

	//���ȼ����С
	//mode���������Ͻ�����������
	//rcRate.right=0,����ά��ԭ״
	//rcRate.right>0,������ΪrcRate��rightֵ
	//rcRate.right<0�������ģʽ���趨ΪrcArea���Ϊ���������Ȳ�Ϊ���������ֵ��-1����1%��-100Ϊ100%
	if (rcRate.right > 0) width = rcRate.right;
	if (rcRate.right < 0) width = -widthArea*rcRate.right / 100;

	//rcRate.right=0,��߶�ά��ԭ״
	//rcRate.bottom>0,��߶���ΪrcRate��bottomֵ
	//rcRate.bottom<0�������ģʽ���趨ΪrcAre�߶�ΪrcArea�߶ȵ����ֵ��-1����1%��-100Ϊ100%
	if (rcRate.bottom > 0) height = rcRate.bottom;
	if (rcRate.bottom < 0) height = -heightArea*rcRate.bottom/ 100;
	
	
	int widthBase = mode?(widthArea - width):widthArea;
	int heightBase = mode ? (heightArea - height) : heightArea;

	//Ȼ��ȷ��λ��
	if (rcRate.left >= 0)
		rcChild.left = rcArea.left+rcRate.left; //rcRate����Ը�RECT��ֵ��������Ҫ�õ���������
	else
		rcChild.left =rcArea.left-widthBase*rcRate.left/100;//���������꣬��Ӧ��ȥ1
	
	//������rcAreaΪ������ʼ�ջ����������˲�����С��0
	//if (rcChild.left < 0) rcChild.left = 0;

	if (rcRate.top >= 0)
		rcChild.top =rcArea.top+rcRate.top;
	else
		rcChild.top =rcArea.top-heightBase*rcRate.top / 100;

	//���ˣ�λ���Ѿ���ȷ����ʱ�޸���������
	rcChild.right = rcChild.left + width - 1;
	rcChild.bottom = rcChild.top + height - 1;
}

////db��ľ�̬����set_default
TEST_F(uiTest, CalcRect_Test)
{
	//���յõ������꣬�����rcArea�ĸ�����
	RECT rcParent = { 23,33, 424, 536};//������Ը����ڵ�����
	RECT rcRate = {0,0,0,0};//���Ǳ���
	RECT rcNow = { 0, 0, 254, 156 }; //һ���ȡ�ͻ��˴�С����(�б߿���ʹ��WindowRect,����ν����С������Ҫ��)

	//rcRate��Ϊȫ0��λ��ΪrcParent�����Ͻ����꣬��С���ֲ���
	RECT rcTarget = rcNow;//���ڲ���
	CalcRect(rcParent, rcRate, rcTarget);
	EXPECT_EQ(23, rcTarget.left);
	EXPECT_EQ(33, rcTarget.top);
	EXPECT_EQ(23+255-1,rcTarget.right); //�Ӵ��ڿ��255
	EXPECT_EQ(33+157-1, rcTarget.bottom); //�Ӵ��ڸ߶�157


	//��С��Ϊ�̶�ֵ
	rcRate = {0,0,30,100};
	rcTarget = rcNow;
	CalcRect(rcParent, rcRate, rcTarget);
	EXPECT_EQ(23+30-1, rcTarget.right);
	EXPECT_EQ(33+100-1, rcTarget.bottom);

	//��С��Ϊ���ֵ
	rcRate = { 0, 0, -10, -20 };
	rcTarget = rcNow;
	CalcRect(rcParent, rcRate, rcTarget);
	EXPECT_EQ(23 + 40-1, rcTarget.right); //���402��10%Ϊ40.2
	EXPECT_EQ(33 + 100-1, rcTarget.bottom); //�߶�504��20%Ϊ100.8

	//λ����Ϊ���ֵ��
	rcRate = { -10, -10, 30, 100 };
	rcTarget = rcNow;
	CalcRect(rcParent, rcRate, rcTarget);
	EXPECT_EQ(23 + 40 , rcTarget.left);//���402��10%Ϊ40
	EXPECT_EQ(33 + 50 , rcTarget.top); //�߶�504,10%Ϊ50

	//λ�úʹ�С����Ϊ���ֵ��
	rcRate = { -10, -10, -10, -20 };
	rcTarget = rcNow;
	CalcRect(rcParent, rcRate, rcTarget);
	EXPECT_EQ(23 + 40, rcTarget.left);//���402��10%Ϊ40
	EXPECT_EQ(33 + 50, rcTarget.top); //�߶�504,10%Ϊ50
	EXPECT_EQ(23 + 40+40-1, rcTarget.right);//���402��10%Ϊ40
	EXPECT_EQ(33 + 50+100-1, rcTarget.bottom); //�߶�504,20%Ϊ100.8



}
