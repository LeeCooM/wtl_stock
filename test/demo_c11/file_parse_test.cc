#include "gtest/gtest.h"
#include <functional>
#include "file_parse.h"

//���ҪΪһ�����(testSuitΪblockTest)�ṩ��ʼ��������������Ҫ�����testSuit
class FileParse : public testing::Test {
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

//Ĭ�Ϲ��캯�����ɱ��������ɣ�����ʵ�ִ���
TEST_F(FileParse, construct_default)
{
	file_parse<int *, char *> container;
}

//Ĭ�Ϲ����ʵ����ͬ���ܹ�֧�ֱ���
TEST_F(FileParse, shuld_support_loop_for_default)
{
	file_parse<int *, char *> container;
	int j = 0;
	for (auto i : container)
	{
		j++;
	}
	EXPECT_EQ(0, j);
}