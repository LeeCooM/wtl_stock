#ifndef demo_stl_my_advance_h
#define demo_stl_my_advance_h
#include <iterator>
using namespace std;

//��ʾ�㷨����ڱ����ڣ�ʹ��traits�õ����������ͣ��Ӷ���������ʱ�жϵĿ���

//������ȡ�����������ͣ������ڸ��ݵ���������ѡ��ͬ�ĺ���
// TEMPLATE FUNCTION my_Iter_cat
template<class _Iter> inline
typename iterator_traits<_Iter>::iterator_category my_Iter_cat(const _Iter&)
{	// return category from iterator argument

	//������ִ�У���������
	typename iterator_traits<_Iter>::iterator_category _Cat;
	
	//��Ϊinline���൱��ֱ�ӷ��أ�û�к������п���
	return (_Cat);
};

// TEMPLATE FUNCTION advance
//output������֮�⣬���ֵ�������֧��
//��������������++
template<class _InIt,
class _Diff> inline
	void my_Advance(_InIt& _Where, _Diff _Off, input_iterator_tag)
{	// increment iterator by offset, input iterators
		for (; 0 < _Off; --_Off)
			++_Where;
}

//forward���������������������ͬ
template<class _FwdIt,
class _Diff> inline
	void my_Advance(_FwdIt& _Where, _Diff _Off, forward_iterator_tag)
{	// increment iterator by offset, forward iterators
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
//��������Ĳ������Ե�����traits��صĶ��������仰˵�����յ�ʹ��Ҫ��Ȼ�򵥡�
template<class _InIt,
class _Diff> inline
	void my_advance(_InIt& _Where, _Diff _Off)
{	// increment iterator by offset, arbitrary iterators
		my_Advance(_Where, _Off, my_Iter_cat(_Where));
}
#endif //