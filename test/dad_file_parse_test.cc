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
TEST_F(DadFileParse, shuld_support_loop_for_default)
{
	dad_file_parse container;
	int j = 0;
	//for (auto i : container)
	//{
	//	j++;
	//}
	EXPECT_EQ(0, j);
}

//Ĭ�Ϲ��캯�����ɱ��������ɣ�����ʵ�ִ���
TEST_F(DadFileParse, open)
{
	dad_file_parse container;
	auto result = container.open("test.dad");
	EXPECT_TRUE(result);
	int j = 0;
	auto p = container.begin();
	EXPECT_STREQ("SH000001", p->idOfDad->id);
	EXPECT_STREQ("��ָ֤��", p->idOfDad->title);
	EXPECT_EQ((long)1402876800, (long)p->quoteOfDad->quoteTime);
	//ʹ�þ��ȱȽϸ�����
	EXPECT_TRUE(fabs(2070.70f - (*p).quoteOfDad->open) < std::numeric_limits<float>::epsilon());
	EXPECT_FLOAT_EQ(2070.700F, p->quoteOfDad->open);
	EXPECT_FLOAT_EQ(2087.320F, p->quoteOfDad->high);
	EXPECT_FLOAT_EQ(2069.440F, p->quoteOfDad->low);
	EXPECT_FLOAT_EQ(2085.980F, p->quoteOfDad->close);
	EXPECT_FLOAT_EQ(95939768.00F, p->quoteOfDad->volume);
	EXPECT_FLOAT_EQ(79953076224.00F, p->quoteOfDad->amount);

	p++;
	EXPECT_STREQ("SH000001", p->idOfDad->id);
	EXPECT_STREQ("��ָ֤��", p->idOfDad->title);
	EXPECT_EQ((long)1402963200, (long)p->quoteOfDad->quoteTime);

	for (auto i : container)
	{
		j++;
	}
	EXPECT_EQ(9, j);

}