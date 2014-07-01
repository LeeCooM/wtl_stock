//gtest��gmock���Ե�ʾ��
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using testing::Return;

TEST(DemoTest, JustDemo)
{
	EXPECT_EQ(2, 1+1);
}

//������gmockʹ�õ�����
//�ӿ��ࣺΪ������������ǲ�ʵ��Vistor�����ṩһ���ӿ�count,��ʾĿǰ���ʵ�����
class Vistor {
public:
	virtual int count() = 0;
};

//mockһ��Vistor
class MockVistor : public Vistor {
public:
	//��˼�������һ��mock�ĺ���
	MOCK_METHOD0(count, int());
};

//����
TEST(MockDemoTest, mockDemo) {
//ʵ����һ��mock�Ķ���
MockVistor vistor;
//������2�Σ�ÿ�η���ʲôֵ��ע�⣬���������û�е������Σ����Ի᲻��ͨ��
EXPECT_CALL(vistor,count()).Times(2).WillOnce(Return((int)100)).WillRepeatedly(Return((int)200));
//��һ�ε��ã�Ӧ����100
int count = vistor.count();
EXPECT_EQ(count,(int)100);
//�ڶ��ε��ã�Ӧ����200
count = vistor.count();
EXPECT_EQ(count, (int)200);
}
