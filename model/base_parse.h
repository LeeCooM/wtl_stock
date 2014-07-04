#include "parse_iterator.h"
#ifndef base_parse_h
#define base_parse_h
template <typename T, typename source_iterator_type = char *, typename Iter = parse_iterator<T, source_iterator_type>>
class base_parse
{
public:
	using iterator = Iter;
	base_parse(source_iterator_type data, size_t size, T beginValue) :
		block_(data), size_(size), begin_(beginValue)
	{}
	base_parse() = default;


	iterator begin() const
	{
		return iterator(block_, block_ + size_, begin_);
	}

	iterator end() const
	{
		return iterator();
	}
protected:
	source_iterator_type block_ = {};   //�ڲ�ά��һ��char *�������ǴӸö����ȡValueType����
	size_t size_=0;
	T begin_ = {};
};

#endif