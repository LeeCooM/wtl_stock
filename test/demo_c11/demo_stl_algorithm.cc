
#include <vector>
#include <list>
#include "my_advance.h"
#include "gtest/gtest.h"
using namespace std;

//ͨ��my_Advance����㷨�͵�����
TEST(demo, using_traits_in_algorithm)
{
	//1.ʹ��ָ�룺����intָ���ľ���
	int intPtr[]={ 1, 2, 3, 4, 5 };
	int *next_ptr = &intPtr[0];
	my_advance(next_ptr,1);
	EXPECT_EQ(2, *next_ptr);

	//2.���������
	vector<int> vec = { 1, 2, 3, 4, 5 };
	auto next_vec =  vec.begin();
	my_advance(next_vec, 1);
	EXPECT_EQ(2, *next_vec); //��һ��������������begin+3������0������ƶ�3�Σ���Ӧ��Ϊ��3����ֵΪ4

	//3.list��˫�������
	list<int> list_int = { 1, 2, 3, 4, 5 };
	auto next_list = list_int.begin();
	next_list++;
	next_list++; //�õ�����ǰ�����Σ�ע�⣬���ڲ������������������һ���ƶ����Ρ�
	EXPECT_EQ(3,*next_list);
	my_advance(next_list, 1);
	EXPECT_EQ(4,*next_list);
}
