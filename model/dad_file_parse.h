#include <string>
#include <cerrno>
#include <fstream>
#include <memory>
#include "file_parse.h"
#include "dad_parse_iterator.h"

#ifndef dad_file_parse_h
#define dad_file_parse_h
using namespace std;

class dad_file_parse :public file_parse<parse_of_dad, char *, dad_parse_iterator>
{
public:
	
	//����ļ��Ƿ�Ϸ����һ�ȡ��Ʊ��������������
	bool check(const wchar_t *filename)
	{
		bool isValidFile = false;
		const long DadFlag = 0x33FC198C;//ע�⣬�ļ��е�4���ֽڵ�˳��Ϊ8C19FC33������0x33fc198c��˳�������෴��

		long flag = 0;

		std::ifstream reader(filename, std::ios::in | std::ios::binary);
		if (!reader)
		{
			reader.close();
			return false;
		}

		reader.seekg(0, ios::end);//���ļ�β��λ,beg���ļ����ֽ�Ϊ0��ʼ��λ��cur�Ե�ǰλ�ÿ�ʼ��λ
		this->size_ = reader.tellg();//�õ��ļ���С
		reader.seekg(0, ios::beg); //�ص��ļ���һ���ֽ�
		std::shared_ptr<char> ptr(new char[16], [](char* ptr){delete[] ptr; });
		char *buffer = ptr.get();
		reader.read(buffer, 16); //��ȫ���ļ���ȡ���ڴ�
		reader.close();

		flag = *(long *)buffer;
		this->m_stock_count = *(unsigned long *)(buffer + 8);
		this->m_quote_count = (unsigned long)(size_ - 16 - 32 - m_stock_count * 32) / 32;
		return (flag == DadFlag && (((size_ - 16) % 32) == 0));
	}
	bool open(const wchar_t *filename)
	{
		//���û����open������Ψһ��Ŀ�ģ��ǵõ���������
		if (file_parse<parse_of_dad, char *, dad_parse_iterator>::open(filename))
		{
			m_stock_count = *(size_t *)(block_ + 8);
			m_quote_count = (size_t)(this->size_ - 16 - 32 - m_stock_count * 32) / 32;
			return true;
		}
		return false;
	}
	iterator begin() const
	{
		if (block_)
			return dad_parse_iterator(block_, block_ + size_, {
			(id_of_dad *)(block_ + 20),
			(quote_of_dad *)(block_ + 16 + 32)
		});
		else
			return dad_parse_iterator();
	}

	iterator end() const
	{
		return dad_parse_iterator();
	}

	size_t m_stock_count=0, m_quote_count=0;

	
};

#endif