#include "gtest/gtest.h"
using namespace std;


//wtl�Ķ�̬���ƣ�ʹ��ģ������麯����ʡ����vtable�Ŀ���

//�������
template <class T>
class base_class
{
public:
	//��RunWhoAmI��WhoAmI����������ʵ��WhoAmI�Ķ�̬�����า�Ǻ��ߣ�ǰ�ߵ��������WhoAmI
	std::string RunWhoAmI()
	{
		T *ptr = static_cast<T*>(this); //������ǿ��ת����T�࣬Ҳ�������������
		//Ҳ��д�ɣ�T* ptr = (T *)this;
		return ptr->WhoAmI();//ִ�������WhoAmI����
	}

	//ֱ�ӷ���string�ǿ��еģ�ʵ�����ǿ���һ�ݸ��ⲿ����
	std::string WhoAmI()
	{
		return "base_class";
	}

};

//�������࣬����WhoAmI����
//�̳���base_class��ģ��������Լ����������ݽ�ȥ����ν�ĵݹ�ģ��
class child_class :public base_class<child_class>
{
public:
	//���ǻ����WhoAmI����
	std::string WhoAmI()
	{
		return "child_class";
	}
};

//�������࣬������WhoAmI����
class child :public base_class<child>
{

};


//wtl�ĵݹ�ģ��Ķ�̬����
TEST(demo,wtl_polymorphism)
{
	child_class A;
	child B;

	//ִ�и����RuncWhoAmI��������ʵ�ϵ����������WhoAmI����
	EXPECT_TRUE("child_class" == A.RunWhoAmI());

	//δ���Ǹ����WhoAmI��������ˣ�RunWhoAmI���ø����WhoAmI����
	EXPECT_TRUE("base_class" == B.RunWhoAmI());
	//���ֻ��ƽ�ʵ���˶�̬�����������������������ͳһ�Ļ���ָ��ָ��������ʵ��
	//�������������ܵļ���
}