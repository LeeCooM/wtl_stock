#include "gtest/gtest.h"
#include <vector>
#include <iterator>

std::vector<int> vector_int{ 1, 2, 3, 4, 5 };



//Ҫ�󣬵�������value_type������int�����Ҫ����ȫ��ͬ����is_same
//int *Ҳ�������
//����ֻ�ڷ���ֵ�����жϼ��ɣ������ڲ��������жϡ�
template <typename T>
typename std::enable_if<std::is_convertible<typename std::iterator_traits<T>::value_type, int>::value, typename std::iterator_traits<T>::distance_type>::type count_vector(T _begin,T _end)
{
	
		return std::distance(_begin, _end);
}
TEST(Faq, iterator_as_point)
{
	bool result = typeid(std::iterator_traits<std::vector<int>::iterator>::distance_type) == typeid(int);
	EXPECT_EQ(true, result);
	result = std::is_same<typename std::iterator_traits<std::vector<int>::iterator>::value_type, int>::value;
	EXPECT_EQ(true, result);	
	result = typeid(std::enable_if<std::is_same<typename std::iterator_traits<std::vector<int>::iterator>::value_type, int>::value, std::vector<int>::iterator>::type) == typeid(std::vector<int>::iterator);
	EXPECT_TRUE(result);
	auto i = count_vector(vector_int.begin(), vector_int.end());
	EXPECT_EQ(5, i);
	//auto i = count_vector(vector_int.begin(), vector_int.end());
	//EXPECT_EQ(5, i);
}