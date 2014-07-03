#include "parse_iterator.h"
#ifndef dad_parse_iterator_h
#define dad_parse_iterator_h

//Dad�ļ���֤����롢���ƶ��ձ�ṹ
struct id_of_dad
{
	char id[16];
	char title[16];
};

//Dad�ļ��������¼�ṹ
struct quote_of_dad
{
	long quoteTime; //ʹ��_mktime32תΪ__time32t�洢��ʹ��_localtime32��ȡ
	float open, high, low, close;
	float volume, amount;
};

//Dad�ļ���ʶ��ṹ
struct parse_of_dad
{
	id_of_dad *idOfDad;
	quote_of_dad *quoteOfDad;
};

//����ʶ��dad,�ɴ��ṩparse_of_dad�ṹ
class dad_parse_iterator :public parse_iterator<parse_of_dad>
{
public:
	dad_parse_iterator() = default;
	dad_parse_iterator(char * _begin, char * _end, parse_of_dad _value):
		parse_iterator<parse_of_dad>(_begin, _end, _value)
	{

	}
};


#endif