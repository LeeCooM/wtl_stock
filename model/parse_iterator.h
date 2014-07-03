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
		begin_(_begin), end_(_end), value_(_value)
	{}

	//Ĭ�Ϲ��캯��
	parse_iterator() = default;//ָʾ����������Ĭ�ϵĹ��캯��
	
	//�������캯��
	//ʵ�ֿ��������assign����������������������=
	//self_type& operator=(const self_type&);
	parse_iterator(const self_type& _another):
		begin_(_another.begin_), end_(_another.end_), value_(_another.value_)
	{}

	//Ĭ����������
	~parse_iterator() = default;

	//==������
	friend bool operator==(const self_type& _first, const self_type& _next)
	{
		return (_first.value_ == _next.value_);
	};

	//!=������
	friend bool operator!=(const self_type& _first, const self_type& _next)
	{
		return (_first.value_ != _next.value_);
	}


	//*������
	value_type  operator*() const
	{
		return value_;
	};

	self_type& operator++(); //prefix increment
	self_type operator++(int); //postfix increment����++

	
	friend void swap(self_type& lhs, self_type& rhs); //C++11 I think

	//value_type operator*() const;
	pointer operator->() const;

protected:
	value_type value_;//���ڹ��캯����ʹ��

	//����ʹ��Ĭ�ϵ�{}����ʼ������ζ�Ŵ��������Ӧ�߱����캯��
	source_iterator_type begin_ = {}; //����������Ŀ�ʼֵ
	source_iterator_type end_ = {};//����������Ľ���ֵ
};
#endif