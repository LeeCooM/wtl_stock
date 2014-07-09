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

	/*//ʹ��c�����ļ���д�������ķ���
	size_t open(wstring filename)
	{
		FILE* file;
		errno_t err = _wfopen_s(&file, filename.c_str(), L"rb");
		if (err != 0)
			return 0;
		fseek(file, 0, SEEK_END);
		size_t len = ftell(file);
		rewind(file);

		if (this->block_) free(block_);
		//�����block_Ϊ�������͵��������Ƿ�����???
		block_ = (source_iterator_type)malloc(this->size_);
		//���������ָ��
		// if (block_) block_reset()//����Ѿ�����ռ䣬��ɾ�������Ѿ����Ƹ�����ı�������ֻ�Ǽ������ü���
		//����ռ䣬ʹ��free��Ϊdelete����
		//block_ = shared_ptr<char>((char *)malloc(len), free);
		fread_s((void *)block_.get(), len, len, 1, file);
		fclose(file);//�ر�


		//���ù�Ʊ�����ͼ�¼����
		//TagNumber = *(unsigned long *)(all.get() + 8);
		//RecordNumber = (unsigned long)(len - 16 - 32 - TagNumber * 32) / 32;

		return len;

		//��ν�����ã�ֻ���ã������������仰˵�������shared_ptr�������������reset�˲�ɾ���ռ䡣
		//weak_ptr<char> head(block_);
		//char *current = block_.get();
	}
	*/

	//��ʱ�Ŷ�ȡ����Ϊ�жϸ�ʽ�Ϸ�����������ȫ���ļ�
	bool open(const char *filename)
	{
	    std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in)
		{			
			in.seekg(0, std::ios::end);
			this->size_=in.tellg();
			if (this->block_) free(block_);
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