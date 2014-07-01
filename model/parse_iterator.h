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
	using self_type = parse_iterator;

	//����advance�ĺ�������
	using advance_function = std::function<void(reference)>;
	using equal_to_function = std::function<bool(value_type)>;

	parse_iterator(source_iterator_type _being, source_iterator_type _end, value_type _value)
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
	friend bool operator==(const self_type&, const self_type&);
	friend bool operator!=(const self_type&, const self_type&);
private:
	pointer ptr_ = nullptr;//��ʼ��Ϊnullptr����ʾend
	value_type value_;//���ڹ��캯����ʹ��
	source_iterator_type begin_; //����������Ŀ�ʼֵ
	source_iterator_type end_;//����������Ľ���ֵ
	std::function<void(reference)> advance_; //ǰ��++����
	std::function<bool(value_type, value_type)> equal_to_; //��Ȳ���
};
#endif