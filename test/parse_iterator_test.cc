#include "gtest/gtest.h"
#include <functional>
#include "parse_iterator.h" 

//���ҪΪһ�����(testSuitΪblockTest)�ṩ��ʼ��������������Ҫ�����testSuit
class ParseIterator : public testing::Test {
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

int intArray[5]{1, 2, 3, 4, 5};
char *int_ptr = reinterpret_cast<char*>(intArray);

/*
stl�淶�У�input����������߱���������
1.Is copy-constructible, copy-assignable and destructible
x b(a);b=a
����ʵ�ֿ������캯����������������
2.Can be compared for equivalence using the equality/inequality operators
(meaningful if both iterators are be in domain).
a == b;a != b
��������==��!=������
3.Can be dereferenced as an rvalue (if in a dereferenceable state).
*a;a->m
����*��->������
4.Can be incremented (if in a dereferenceable state).
The result is either also dereferenceable or a past-the-end iterator.
The previous iterator value is not required to be dereferenceable after the increase.
++a
(void)a++
*a++
Ҫ����ǰ�úͺ��õ�++������
5.Its value type does not need to be assignable
t = u not required
����value type����Ҫassignable������Ҫ��������ɡ�
6.c++ 11 only
Lvalues are swappable.
swap(a,b)
�ṩswap����
*/

//Ĭ�Ϲ��캯�����ɱ��������ɣ�����ʵ�ִ���
TEST_F(ParseIterator, construct_default)
{
	parse_iterator<int *, char *> iter;
}

//��Ҫ���캯��������������������һ��T���͵�move
TEST_F(ParseIterator, construct_regular)
{
	//Ĭ�Ϲ��캯�����ɱ��������ɣ�����ʵ�ִ���
	parse_iterator<int *, char *> iter(int_ptr,int_ptr+sizeof(int)*5,(int *)int_ptr );
}
//�������캯��
TEST_F(ParseIterator, construct_copy)
{
	parse_iterator<int *, char *> iter(int_ptr, int_ptr + sizeof(int)* 5, (int *)int_ptr);
	parse_iterator<int *, char *> iter_copy(iter);
	parse_iterator<int *, char *> iter_assign=iter;
	EXPECT_EQ(iter,iter_copy);
	EXPECT_EQ(iter, iter_assign);
}


//operator==
TEST_F(ParseIterator, operator_equal)
{
	//����nullptr�Ƚϣ�Ӧ���
	parse_iterator<int *, char *> iter;
	parse_iterator<int *, char *> empty_iter;
	EXPECT_EQ(iter, empty_iter);

	//������ȵ�iterator
	parse_iterator<int *, char *> iter1(int_ptr, int_ptr + sizeof(int)* 5, (int *)int_ptr);
	parse_iterator<int *, char *> iter2(int_ptr, int_ptr + sizeof(int)* 5, (int *)int_ptr);
	EXPECT_EQ(true, iter1 == iter2);

	//����ȵ�����������
	parse_iterator<int *, char *> iter3(int_ptr, int_ptr + sizeof(int)* 5, (int *)(int_ptr + sizeof(int)));
	EXPECT_EQ(false, iter1 == iter3);

	//��Ĭ�Ϲ���ĵ������Ƚ�
	EXPECT_EQ(false, iter == iter3);
}

//operator!=
TEST_F(ParseIterator, operator_equal_not)
{
	//����nullptr�Ƚϣ�Ӧ���
	parse_iterator<int *, char *> iter;
	parse_iterator<int *, char *> empty_iter;
	EXPECT_EQ(iter, empty_iter);

	//������ȵ�iterator
	parse_iterator<int *, char *> iter1(int_ptr, int_ptr + sizeof(int)* 5, (int *)int_ptr);
	parse_iterator<int *, char *> iter2(int_ptr, int_ptr + sizeof(int)* 5, (int *)int_ptr);
	EXPECT_EQ(false, iter1 != iter2);

	//����ȵ�����������
	parse_iterator<int *, char *> iter3(int_ptr, int_ptr + sizeof(int)* 5, (int *)(int_ptr + sizeof(int)));
	EXPECT_EQ(true, iter1 != iter3);

	//��Ĭ�Ϲ���ĵ������Ƚ�
	EXPECT_EQ(true, iter != iter3);
}

TEST_F(ParseIterator, operator_point)
{
	parse_iterator<int *, char *> iter(int_ptr, int_ptr + sizeof(int)* 5, (int *)int_ptr);
	EXPECT_EQ(1, **iter);
	parse_iterator<int *, char *> iter1(int_ptr, int_ptr + sizeof(int)* 5, (int *)(int_ptr+sizeof(int)*2));
	EXPECT_EQ(3, **iter1);
	EXPECT_EQ((int *)(int_ptr),*iter);
}

TEST_F(ParseIterator, operator_increment_prefix)
{
	parse_iterator<int *, char *> iter(int_ptr, int_ptr + sizeof(int)* 5, (int *)int_ptr);
	EXPECT_EQ(1, **iter);
	++iter;
	EXPECT_EQ(2, **iter);
	++iter;
	EXPECT_EQ(3, **iter);
	++iter;
	EXPECT_EQ(4, **iter);
	++iter;
	EXPECT_EQ(5, **iter);
}

TEST_F(ParseIterator, operator_increment_postfix)
{
	parse_iterator<int *, char *> iter(int_ptr, int_ptr + sizeof(int)* 5, (int *)int_ptr);
	EXPECT_EQ(1, **iter);

	iter++;
	EXPECT_EQ(2, **iter);

	auto iter1=(iter++); //iter1���µĶ�������
	EXPECT_EQ(3, **iter); //iter����1
	EXPECT_EQ(3, **iter1);// iter1û�м�..��Ҫ�޸�

	++iter1;
	EXPECT_EQ(4, **iter1);
	EXPECT_EQ(3, **iter);
}