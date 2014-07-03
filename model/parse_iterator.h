#include <iterator>
#include <functional>
#include <istream>

//ͷ�ļ������ں�֮���壿
#ifndef parse_iterator_h
#define parse_iterator_h
//���ڵ������ĵ�����
//����Source��ָ��Ĭ��ֵ��������Ҫ���ں���
//T��Ҫ��������������
template <typename T, typename source_iterator_type>
class parse_iterator :std::iterator<std::input_iterator_tag, T>
{
public:
	//������ģ���࣬self_typeӦ���¶���
	using self_type = parse_iterator<T, source_iterator_type>;

	//�����Ĺ��캯��
	parse_iterator(source_iterator_type _begin, source_iterator_type _end, value_type &&_value):
		begin_(_begin), end_(_end), value_(_value), ptr_(&value_)
	{

	}
	//iterator
	parse_iterator() = default;//ָʾ����������Ĭ�ϵĹ��캯��
	parse_iterator(const self_type&); //ʹ����һ������������
	~parse_iterator(){};
	self_type& operator=(const self_type&);


	self_type& operator++(); //prefix increment
	reference operator*() const;
	friend void swap(self_type& lhs, self_type& rhs); //C++11 I think

	//input_iterator
	self_type operator++(int); //postfix increment����++
	//value_type operator*() const;
	pointer operator->() const;
	friend bool operator==(const self_type& _first, const self_type& _next)
	{
		//nullptr����ִ��*������������Ҫ�ж�
		//����Ƚϵ�������������һ��Ϊnullptr
		if (!_first.ptr_ || !_next.ptr_) return (_first.ptr_ == _next.ptr_);

		//���������
		return (*_first.ptr_ == *_next.ptr_);
		
	};
	friend bool operator!=(const self_type&, const self_type&);
protected:
	pointer ptr_ = nullptr;//��ʼ��Ϊnullptr����ʾend
	value_type value_;//���ڹ��캯����ʹ��
	source_iterator_type begin_; //����������Ŀ�ʼֵ
	source_iterator_type end_;//����������Ľ���ֵ
	std::function<void(reference)> advance_; //ǰ��++����
	std::function<bool(value_type, value_type)> equal_to_; //��Ȳ���
};
#endif