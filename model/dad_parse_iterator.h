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
	using self_type = dad_parse_iterator;
	dad_parse_iterator() = default;
	dad_parse_iterator(char * _begin, char * _end, parse_of_dad _value):
		parse_iterator<parse_of_dad>(_begin, _end, _value)
	{

	}

	self_type& operator++()
	{
		//����Ѿ���end����++����ʲô������
		if (value_.quoteOfDad == nullptr)
			return *this;
		value_.quoteOfDad=(quote_of_dad *)((char *)value_.quoteOfDad + 32);
		if (*(long *)(value_.quoteOfDad) == 0xffffffff) //���ʱ��Ϊ0xffffffff����ʾ��ʱ������һֻ��Ʊ
		{
			//�ļ������0xffffffff��ʼ��Ȼ��ȫ0.����֮����һֻ��Ʊ����idΪ�գ���ʾ�ļ�������
			//���  FF FF FF FF 00 00 00 00  00 00 00 00 00 00 00 00   ����
			//      00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00
			if ((*((char *)value_.quoteOfDad + 4) == 0x00))  //β����0xffffffff�ĸ��ֽں�һ��0.
			{
				value_.idOfDad = nullptr; //�����Ƿ����ڴ�й©��
				value_.quoteOfDad = nullptr;
			}
			else  //��ʱȷ��Ϊ��һֻ��Ʊ
			{
				value_.idOfDad = (id_of_dad *)((char *)value_.quoteOfDad + 4);  //�޸�id
				value_.quoteOfDad = (quote_of_dad *)((char *)value_.quoteOfDad + 32);//�ƶ�32λ
			}
		}
		return *this;
	};

	//���ǣ����಻����virtual
	bool  operator==(const self_type& _next) 
	{
		return (value_.quoteOfDad == _next.value_.quoteOfDad);
	};

	bool  operator!=(const self_type& _next)
	{
		return (value_.quoteOfDad != _next.value_.quoteOfDad);
	};
};


#endif