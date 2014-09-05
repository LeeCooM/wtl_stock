
#include <vector>
#include "gtest/gtest.h"
using namespace std;

class any_struct
{
public:
	static int construct_number, move_number;
	int first_ = 0, second_ = 0;
	any_struct(int _first, int _second) :first_(_first), second_(_second)
	{
		++construct_number;
	}
	any_struct(any_struct &&_other) :first_(_other.first_), second_(_other.second_)
	{
		++move_number;
	}
};
int any_struct::construct_number = 0;
int any_struct::move_number = 0;

TEST(DemoC11, Vector)
{
	vector<int> any_vector;
	any_vector.push_back(1);
	any_vector.push_back(2);
	any_vector.push_back(3);

	//ʵ�ʴ���˼���Ԫ��
	EXPECT_EQ(3, any_vector.size());

	//vector��data()������ָ��T��ָ�룬��������ŵ�
	auto pointer = any_vector.data();
	EXPECT_EQ(1, pointer[0]);
	EXPECT_EQ(2, pointer[1]);
	EXPECT_EQ(3, pointer[2]);

	//assign���÷���
	//Ч���ǣ������vector���ٽ����鿽��������ע��ڶ���ָ����Խ��ĵ�ַ��
	int int_array[] = { 4, 5 };
	any_vector.assign(int_array, int_array + 2);
	EXPECT_EQ(2, any_vector.size());
	EXPECT_EQ(4, any_vector[0]);
	EXPECT_EQ(5, any_vector[1]);
	//EXPECT_EQ(3, any_vector[2]);


	//emplace_back��β������
	//�Ǳ���﷨,������������һ������
	//�����ֱ�ӹ������
	any_struct::construct_number = 0;
	any_struct::move_number = 0;
	vector<any_struct> struct_vector;
	struct_vector.emplace_back(10, 20); //���캯������������

	//emplace_backִֻ��һ�ι��죬û���ƶ�
	EXPECT_EQ(0, any_struct::move_number); //���ƶ�
	EXPECT_EQ(1, any_struct::construct_number); //���캯��ִ��һ��

	any_struct::construct_number = 0;
	any_struct::move_number = 0;
	struct_vector.push_back(any_struct(30, 40)); //ֻ��Ԥ�ȹ��죬Ȼ���ƶ�
	//push_backִ����1�ι��죬������2���ƶ�
	EXPECT_EQ(2, any_struct::move_number); //�ƶ�1��
	EXPECT_EQ(1, any_struct::construct_number); //���캯��ִ��һ��

	//emplace���ض�λ��֮ǰ������һ��
	//������begin�����������ĳ�����beginλ��
	//ʵ�����ǲ������
	struct_vector.emplace(struct_vector.begin(), 50, 60);
	EXPECT_EQ(3, struct_vector.size()); //������һ��
	EXPECT_EQ(50, struct_vector[0].first_);

}
