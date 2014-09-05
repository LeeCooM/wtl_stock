
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

TEST(DemoC11,vector)
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

	//vector����һ���ڴ棬β��׷�ӵ�ʱ�򣬲������£�
	//1.�����ڴ����·���
	//2.ԭ�������ݿ������µ��ڴ��(memecpy)
	//3.���ڴ��β������
	//4.�ͷ�ԭ�����ڴ��
	//5.vector��dataָ���µĵ�ַ
	//��Ϊ�˱�֤���ܣ�һ���·�����ڴ��Ҫ��һЩ�������
	//��Ȼ������Ԥ֪��С��Ԥ��resize���򲻻���ֶ���ڴ���䣬���ܴ�����
	
	auto real_size = any_vector.capacity();
	auto object_size = sizeof(any_vector);

	//ʹ��<<:����ʱ���ṩ������Ҫ����Ϣ����������ʾ
	EXPECT_LE(2u, any_vector.capacity()) << "real_size=" << real_size << "object_size=" << object_size;
	
	//��ˣ������������test.exe���Կ�����ʾ��������Դ����������ʱ��������
	std::cout << "capacity=" << real_size << " , object_size=" << object_size;

	//ʹ��reverse���·���ռ�,ʵ�ʵ�size()���䣬�����ı�
	any_vector.reserve(10);
	EXPECT_EQ(2, any_vector.size()); //��С����
	EXPECT_EQ(10, any_vector.capacity());  //�洢�����ı�

	//ʹ��resize�ı��С,size��capacityͬʱ�ı䡣
	any_vector.resize(20);
	EXPECT_EQ(20, any_vector.size()); //��С����
	EXPECT_EQ(20, any_vector.capacity());  //�洢�����ı�

	//vector��Сһֱ��16������װ�������ݣ�˵���ڴ���heap�Ϸ���ģ�����ֻ����ָ�롢��С�ȡ�
	//EXPECT_EQ(16, sizeof(any_vector)) << "object_size=" << object_size;

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
