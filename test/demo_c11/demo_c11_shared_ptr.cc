#include <memory>
#include "gtest/gtest.h"
using namespace std;

void func(void){
	int *p = new int[5];
	std::shared_ptr<int> p1(p);
	std::shared_ptr<int> p2(p);
	//Ҫʹ��...��ע����������м���/EHa�������Ż���try catchģ�顣
	//�˴�����Test��Ŀ|����|C++|�����У���������������
	//�뿪�������ᵼ����������ͬһ���ڴ�
};
TEST(DemoC11,shared_ptr)
{
	//��ʾ���ü��������ٵĺ���
	//����ʹ��{}������������
	{
		std::shared_ptr<int> pint;
		EXPECT_EQ(0l, pint.use_count());
		{
			std::shared_ptr<int> pint2(new int[3]);
			EXPECT_EQ(1l, pint2.use_count()); //new int[3]���ָ�뱻����1��

			pint = pint2;//��ʱnew int[3]���ָ�뱻��������
			EXPECT_EQ(2l, pint2.use_count());
			EXPECT_EQ(2l, pint.use_count());
		}//pint2��ʧ����ʱ��new int[3]�����ü�����һ
		EXPECT_EQ(1l, pint.use_count());
	}//��ʱ�뿪pint������pint��ʧ��new int[3]�����ü�����һ����Ϊ0����ʱ������

	//Ҫע�⣬��Ҫ����������ָ��ָ��ͬһ���ڴ�
	//�������뿪�������ʱ����������٣������쳣
	//��������������һ�л���shared_ptrָ��

	//�˲���δͨ����
	//EXPECT_ANY_THROW(func);
	//Assert::ExpectException

	//���߳������£���������ֵ����ʱ����������ͬʱ�������ü��������¼�����Ч
	//���������Ӧ��ÿ���̴߳��ݹ�����weak_ptr��ʹ��ʱתΪshared_ptr

}
