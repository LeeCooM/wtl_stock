#include "gtest/gtest.h"
using namespace std;

//��ֵ����
//��֧�ֳ����ַ�������Ϊ�����ͱ�����Ϊ��ͬ���ȵ�����
//�����Ҫ֧�֣�������T *���ػ�T[]���أ��Ҳ���ʹ�����ã�ԭ�������鱾������ʹ��ֵ����
template <typename T>
bool max(T &first, T &second)
{
	return first > second;
}

//�������������ͨ������
bool max(char *first, char *second)
{

	return strcmp(first,second)>0;
}

template <typename T>
bool max_r(T &&first, T &&second)
{
	return first > second;  
}

template <typename T>
bool max_r(T first[], T  second[])
{
	return strcmp(first, second)>0;
}
//����ת��
TEST(DemoC11, lvalue_refrences)
{
	EXPECT_TRUE(max("2", "1"));

	//���ﲻ��ͨ�����룬��Ϊ"2"����Ϊchar[1],"10"����Ϊchar[2]�����ò����У����鲻���ɻ�Ϊָ�롣
	//����������ӵڶ���max������Ӧ��char *����
	EXPECT_TRUE(max("2", "10"));

	//��ֵ���ò���������ͬ������ͨ������
	EXPECT_TRUE(max_r("2", "10"));
	EXPECT_TRUE(max_r(3, 2));

}