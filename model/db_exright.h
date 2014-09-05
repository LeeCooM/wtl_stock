#include <fstream>
#include "sqlite/sqlite3.h"

#include "global.h"
#include "db.h"


class DbExright
{
public:
	
	//��ȡ...
	//�����ص�vector����Ϊ0,�򲻺Ϸ�

	//insertʹ��vector��begin end������

	

	size_t bulk_insert(const wchar_t *_file, int totals = -1, int period = 2000, std::function<void(const wchar_t *, int)> func = nullptr)
	{
		std::vector<Stock> stock_list;

		//��ȡ�ļ�
		std::ifstream reader;  //ע�⣬����<fstream>����<iostream>
		reader.open(_file,std::ios::binary |std::ios::in);  //�Զ�����ֻ����ʽ���ļ�
		reader.seekg(0, std::ios::end);//���ļ�β��λ,beg���ļ����ֽ�Ϊ0��ʼ��λ��cur�Ե�ǰλ�ÿ�ʼ��λ
		long fileSize = (long)reader.tellg();//�õ��ļ���С
		reader.seekg(0, std::ios::beg); //�ص��ļ���һ���ֽ�
		std::shared_ptr<char> ptr(new char[fileSize], [](char* ptr){delete[] ptr; });
		reader.read(ptr.get(), fileSize); //��ȫ���ļ���ȡ���ڴ�
		reader.close(); //�ر��ļ�

		//��Exrights��ȡ��vector
		stock_list.clear();
		Stock stock;
		ExRight exRight;
		char *buffer = ptr.get();		
		//�ӵ�12���ֽڿ�ʼ
		int position = 12;
		int flag = -1;
		string id = "";
		while (position<fileSize)
		{
			//��ȡ����
			memcpy(stock.Id,(char *)buffer + position, 8); //��8λ����id
			stock.Id[8] = 0;

			stock.ExRightSet.clear();
			position += 16;
			flag = *((long *)(buffer + position));
			while (flag>0)
			{
				exRight.QuoteTime = flag; //����
				exRight.AddStock = *((float *)(buffer + position + 4));  //�͹�
				exRight.BuyStock = *((float *)(buffer + position + 8));  //���
				exRight.Price = *((float *)(buffer + position + 12));    //��ɼ۸�
				exRight.Bonus = *((float *)(buffer + position + 16));    //�ֺ�
				stock.ExRightSet.push_back(exRight);
				position += 20;
				if ((position<fileSize))
				{
					flag = *((long *)(buffer + position));
					if (flag <= 0)
						position += 4;
				}
				else
					flag = -1;
			}

			stock_list.push_back(stock);
		}

		//ɾ���������ݿ��е�ȨϢ����

		//д������ȨϢ����
	}
};