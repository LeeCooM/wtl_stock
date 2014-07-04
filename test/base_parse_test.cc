#include "gtest/gtest.h"
#include <functional>
#include "base_parse.h"

//���ҪΪһ�����(testSuitΪblockTest)�ṩ��ʼ��������������Ҫ�����testSuit
class BaseParse : public testing::Test {
protected:
	//testSuit�ĳ�ʼ������
	static void SetUpTestCase() {};

	//testSuit��������
	static void TearDownTestCase() {};

	//SetUpTestCase() �����ڵ�һ��TestCase֮ǰִ��
	//TearDownTestCase() ���������һ��TestCase֮��ִ��
	//SetUp()������ÿ��TestCase֮ǰִ��
	//TearDown()������ÿ��TestCase֮��ִ��
	//��TestSuit�ĸ����԰����й����һЩ��Դ������Ϊstatic
};

int demoArray[5]{1, 2, 3, 4, 5};
char *demo_ptr = reinterpret_cast<char*>(demoArray);

//Ĭ�Ϲ��캯�����ɱ��������ɣ�����ʵ�ִ���
TEST_F(BaseParse, construct_default)
{
	base_parse<int *, char *> container;
}

//��Ҫ���캯��������������������һ��T���͵�move
TEST_F(BaseParse, construct_regular)
{
	//Ĭ�Ϲ��캯�����ɱ��������ɣ�����ʵ�ִ���
	base_parse<int *, char *> container(demo_ptr, 5, (int *)demo_ptr);
}

//����begin
TEST_F(BaseParse, begin)
{
	base_parse<int *, char *> container(demo_ptr, 5*sizeof(int), (int *)demo_ptr);
	auto iter = container.begin();
	EXPECT_EQ(1, **iter);
	++iter;
	EXPECT_EQ(2, **iter);
}

//����end
TEST_F(BaseParse, end)
{
	base_parse<int *, char *> container;
	auto iter = container.end();
	EXPECT_EQ(nullptr, *iter);
}

//Ӧ֧��range_based loop
TEST_F(BaseParse, should_support_range_based_loop)
{
	base_parse<int *, char *> container(demo_ptr, 5*sizeof(int), (int *)demo_ptr);
	int j = 0;
	for (auto i : container)
	{
		j++;
	}
	EXPECT_EQ(5, j);
}

//Ӧ֧��range_based loop
TEST_F(BaseParse, should_support_loop_for_default)
{
	base_parse<int *, char *> container;
	int j = 0;
	for (auto i : container)
	{
		j++;
	}
	EXPECT_EQ(0, j);
}