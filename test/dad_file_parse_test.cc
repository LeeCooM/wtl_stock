#include "gtest/gtest.h"
#include <functional>
#include "dad_file_parse.h"

//���ҪΪһ�����(testSuitΪblockTest)�ṩ��ʼ��������������Ҫ�����testSuit
class DadFileParse : public testing::Test {
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
TEST_F(DadFileParse, construct_default)
{
	dad_file_parse container;
}

//Ĭ�Ϲ����ʵ����ͬ���ܹ�֧�ֱ���
//TEST_F(DadFileParse, shuld_support_loop_for_default)
//{
//	dad_file_parse container;
//	int j = 0;
//	for (auto i : container)
//	{
//		j++;
//	}
//	EXPECT_EQ(0, j);
//}

//Ĭ�Ϲ��캯�����ɱ��������ɣ�����ʵ�ִ���
TEST_F(DadFileParse, open)
{
	dad_file_parse container;
	auto result = container.open("test.dad");
	EXPECT_TRUE(result);
	int j = 0;
	//for (auto i : container)
	//{
	//	j++;
	//}
	//EXPECT_EQ(0, j);

}