
#include <thread>
#include <vector>
#include <algorithm>
#include "singleton_v1.h"
#include "gtest/gtest.h"


TEST(singleton,v1)
{
	auto *first = singleton_v1::GetInstance(1);
	EXPECT_EQ(1, first->id);

	//�ڶ��Σ���ʹʹ�ò�ͬ���������ڴ���û��ִ�У����ص���Ȼ��1
	//����ζ��ֻ���״�ʹ�ø�ֵ
	auto *second = singleton_v1::GetInstance(2);
	EXPECT_EQ(1, first->id);
}

//���������̰߳�ȫ��
TEST(singleton, v1_multi_thread)
{
	//�״�ִ��
	auto p=singleton_v1::GetInstance(1);

	std::vector<std::thread> thread_vector;
	int num = 20;
	for (int n = 0; n < num; ++n) {
		thread_vector.push_back(std::thread([n](int id)
		{
			EXPECT_EQ(n, id); //id��Ϊѭ��������n
			EXPECT_EQ(1,singleton_v1::GetInstance(id)->id);
		}
		, n));
	}

	//�ȴ�ÿ���߳̽���
	std::for_each(thread_vector.begin(), thread_vector.end(), std::mem_fn(&std::thread::join));
}