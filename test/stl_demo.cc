
#include <vector>
#include <list>
//#include <algorithm> //stl�㷨
#include "gtest/gtest.h"
#include <iterator>
#include <istream>
using namespace std;

//������stdmymy_Advance��Դ�룬�����������advance��Ϊmy_advance

//������ȡ�����������ͣ������ڸ��ݵ���������ѡ��ͬ�ĺ���
// TEMPLATE FUNCTION my_Iter_cat
template<class _Iter> inline
typename iterator_traits<_Iter>::iterator_category
my_Iter_cat(const _Iter&)
{	// return category from iterator argument
	typename iterator_traits<_Iter>::iterator_category _Cat;
	return (_Cat);
}


// TEMPLATE FUNCTION advance
//output������֮�⣬���ֵ�������֧��
//��������������++
template<class _InIt,
class _Diff> inline
	void my_Advance(_InIt& _Where, _Diff _Off, input_iterator_tag)
{	// increment iterator by offset, input iterators
#if _ITERATOR_DEBUG_LEVEL == 2
		if (_Off < 0)
			_DEBUG_ERROR("negative offset in advance");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		for (; 0 < _Off; --_Off)
			++_Where;
}

//forward���������������������ͬ
template<class _FwdIt,
class _Diff> inline
	void my_Advance(_FwdIt& _Where, _Diff _Off, forward_iterator_tag)
{	// increment iterator by offset, forward iterators
#if _ITERATOR_DEBUG_LEVEL == 2
		if (_Off < 0)
			_DEBUG_ERROR("negative offset in advance");
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */

		for (; 0 < _Off; --_Off)
			++_Where;
}

//˫�������...
template<class _BidIt,
class _Diff> inline
	void my_Advance(_BidIt& _Where, _Diff _Off, bidirectional_iterator_tag)
{	// increment iterator by offset, bidirectional iterators
		for (; 0 < _Off; --_Off)
			++_Where;
		for (; _Off < 0; ++_Off)
			--_Where;
}

//���������
template<class _RanIt,
class _Diff> inline
	void my_Advance(_RanIt& _Where, _Diff _Off, random_access_iterator_tag)
{	// increment iterator by offset, random-access iterators
		_Where += _Off;
}

//�����ڼ䣬�жϵ��������ͣ�����ǰ�治ͬ�ĺ���
template<class _InIt,
class _Diff> inline
	void my_advance(_InIt& _Where, _Diff _Off)
{	// increment iterator by offset, arbitrary iterators
		my_Advance(_Where, _Off, my_Iter_cat(_Where));
}


//ͨ��my_Advance����㷨�͵�����
TEST(StlDemo, lgorithm_and_iterator)
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

	//4.

	
}
