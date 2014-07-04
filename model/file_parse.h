#include <string>
#include <cerrno>
#include <fstream>
#include "base_parse.h"

#ifndef file_parse_h
#define file_parse_h


using namespace std;

template <typename T, typename source_iterator_type = char *, typename Iter = parse_iterator<T, source_iterator_type>>
class file_parse :public base_parse<T,source_iterator_type,Iter>
{
public:
	file_parse() = default;
	file_parse(T beginValue) :
		base_parse({}, {}, beginValue)
	{}

	//��ʱ�Ŷ�ȡ����Ϊ�жϸ�ʽ�Ϸ�����������ȫ���ļ�
	bool open(const char *filename)
	{
	    std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in)
		{			
			in.seekg(0, std::ios::end);
			this->size_=in.tellg();
			if (this->block_)
				free(block_);

			//�����block_Ϊ�������͵��������Ƿ�����???
			block_ =(source_iterator_type) malloc(this->size_);
			in.seekg(0, std::ios::beg);
			in.read(&block_[0],size_);//��char���Ͷ�ȡ
			in.close();
			return true;
		}
		else
			return false;

		//��Ҫ�׳��쳣
		//throw(errno);
	}
protected:
	string path_ = {};
};
#endif