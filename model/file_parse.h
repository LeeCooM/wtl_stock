#ifndef file_parse_h
#define file_parse_h
#include <string>
#include "base_parse.h"
using namespace std;

template <typename T, typename source_iterator_type = char *>
class file_parse :public base_parse<T,source_iterator_type>
{
public:
	file_parse() = default;
	file_parse(T beginValue) :
		base_parse({}, {}, beginValue)
	{}

	//��ʱ�Ŷ�ȡ����Ϊ�жϸ�ʽ�Ϸ�����������ȫ���ļ�
	bool open(string _path)
	{
		return false;
	}
protected:
	string path_ = {};
};
#endif