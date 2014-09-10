#include <fstream>
#include "sqlite/sqlite3.h"

#include "global.h"
#include "db.h"

//ע�⣺ȨϢ���ݴ��µ���˳������
class DbExright
{
public:
	
	//��ȡ...
	//�����ص�vector����Ϊ0,�򲻺Ϸ�

	//insertʹ��vector��begin end������

	bool check(const wchar_t *filename)
	{
		bool isValidFile = false;
		const long ExrightFlag =0xFF43C832;//ע�⣬�ļ��е�4���ֽڵ�˳��Ϊ0x32C843FF��˳�������෴��
		size_t size=0;
		long flag = 0;

		std::ifstream reader(filename, std::ios::in | std::ios::binary);
		if (!reader)
		{
			reader.close();
			return false;
		}

		reader.seekg(0, ios::end);//���ļ�β��λ,beg���ļ����ֽ�Ϊ0��ʼ��λ��cur�Ե�ǰλ�ÿ�ʼ��λ
		size = (size_t)reader.tellg();//�õ��ļ���С
		reader.seekg(0, ios::beg); //�ص��ļ���һ���ֽ�
		std::shared_ptr<char> ptr(new char[8], [](char* ptr){delete[] ptr; });
		char *buffer = ptr.get();
		reader.read(buffer, 8); //�����8���ֽڶ���

		flag = *(long *)buffer;

		return (flag == ExrightFlag);

	}

	size_t bulk_insert(const wchar_t *_file, int totals = -1, int period = 50, std::function<void(const wchar_t *, int)> func = nullptr)
	{

		if (g_stock.Data.size() <= 0)
		{
			func(L"�����û�а�װ���˴ΰ�װ��Ȩ��ʧ��!",100);
			return 0;
		}
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
		//stock_list.clear();
		ExRight exRight;
		char *buffer = ptr.get();		
		//�ӵ�12���ֽڿ�ʼ
		int position = 12; //��8λ��ʼ��FFFFFFFF��ǣ�Ȼ���ǵ�һֻ��Ʊ��id
		int flag = -1;
		int order = -1; //������ȫ�ִ�����е�˳��
		char id[9] = { 0 }; //�����ļ��е�id

		int id_number = 0;

		Db conn;
		conn.Begin();
		auto cmd = conn.CreateQuery("UPDATE STOCK SET EXRIGHTS=? WHERE ID=?");
		while (position<fileSize)
		{
			++id_number;
			//��ȡ����
			memcpy(id, (char *)buffer + position, 8); //��8λ����id
			//stock.Id[8] = 0;
			order = g_stock.FindStock(id);
			if (order>0)
			{
				g_stock.Data[order].ExRightSet.clear();
			}
			position += 16;
			flag = *((long *)(buffer + position));
			while (flag > 0)
			{
				if (order > 0)
				{
					exRight.QuoteTime = flag; //����
					exRight.AddStock = *((float *)(buffer + position + 4));  //�͹�
					exRight.BuyStock = *((float *)(buffer + position + 8));  //���
					exRight.Price = *((float *)(buffer + position + 12));    //��ɼ۸�
					exRight.Bonus = *((float *)(buffer + position + 16));    //�ֺ�
					g_stock.Data[order].ExRightSet.push_back(exRight);
				}
				position += 20;
				if ((position < fileSize))
				{
					flag = *((long *)(buffer + position));
					if (flag <= 0)
						position += 4;
				}
				else
					flag = -1;
			}
			//��ʱ�������ݿ�
			if (order>0)
			{
				cmd.Bind(1,g_stock.Data[order].ExRightSet, id);
				cmd.ExcuteNonQuery();
			}
			if ((id_number % period) == 0)
			{
				func(nullptr, id_number * 80 / g_stock.Data.size());
			}
		}
		
		conn.Commit();
		//func(nullptr, 100);
		return id_number;
		//д������ȨϢ����
	}
	
};