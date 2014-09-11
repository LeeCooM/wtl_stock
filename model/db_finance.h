#include <fstream>
#include "sqlite/sqlite3.h"

#include "global.h"
#include "db.h"


// ��ȡ���ǻ۵����Ĳ�������
//�ļ��л�������ۼ��������е�����
// ���ǻ۲��������ļ��ṹ�������ģ�
// �ļ�ǰ8���ֽ����ļ�ͷ���������ݼ�¼�ӵ�9���ֽڿ�ʼ��
// 9��16���ֽ��ǹ�Ʊ�����磺sh000001��Ȼ���4���ֽڣ�Ȼ���Ƿ������ڣ�4���ֽڣ�
// �����ڣ�4���ֽڣ� �������ڣ�4���ֽڣ�������Ķ���4���ֽ�  ������  ÿ������
// ÿ�ɾ��ʲ�   ���ʲ�������   ÿ�ɾ�Ӫ�ֽ�  ÿ�ɹ�����  ÿ��δ����  �ɶ�Ȩ���
// ������ͬ��  ��Ӫ����ͬ��  ����ë����  ����ÿ�ɾ��ʲ�  ���ʲ�  �����ʲ� �̶��ʲ�
// �����ʲ�  ������ծ  ���ڸ�ծ  �ܸ�ծ   �ɶ�Ȩ��  �ʱ�������  ��Ӫ�ֽ�����
// Ͷ���ֽ�����   �����ֽ�����  �ֽ����Ӷ�   ��Ӫ���� ��Ӫ����  Ӫҵ����
// Ͷ������   Ӫҵ����֧  �����ܶ�    ������  δ��������  �ܹɱ�  �����۹ɺϼ�
// A��  B��  �������й�  ������ͨ��   ���۹ɺϼ�   ���ҳֹ�  ���з��˹�   ���ڷ��˹�
// ������Ȼ�˹�  ���������˹�   ļ�����˹�    ���ⷨ�˹�  ������Ȼ�˹�   ���ȹɻ����� 
// Ȼ��ʼ��һ����¼
// ÿ����¼��148���ֽڡ�
//����ӳ��ÿֻ��Ʊ�ڲ����ļ��е�����

//51X4+12=216�ֽ�
struct FinanceFull
{
	char Id[9]; //���룬8λ+0;
	char NoUse[3]; //ʵ���ϴ���ռ��12λ������3��0��ȥ
	int32_t PublishDate; //��������...ʵ���������ɲ����ļ�������
	int32_t ReportDate; //��������...ÿ�����ȵļ�β��
	int32_t StartDate; //�ù�Ʊ��������
	float EarningsPerShare; //ÿ������
	float NetAssetPerShare; //ÿ�ɾ��ʲ�
	float ROE; //���ʲ�������
	float OperatingCashPerShare; //ÿ�ɾ�Ӫ�ֽ�
	float ShareFund; //ÿ�ɹ�����
	float UnallocatedShare; //ÿ��δ����
	float EquityRatio; //�ɶ�Ȩ���
	float TheSamePeriodNetProfitRatio; //������ͬ��
	float TheSamePeriodMainIncomeRaio; //��Ӫ����ͬ��
	float SalesGrossProfitRate; //����ë����
	float AdjustNetAssetPerShare; //����ÿ�ɾ��ʲ�
	float TotalAssets; //���ʲ�
	float CurrentAssets; //�����ʲ�
	float FixedAssets; //�̶��ʲ�
	float IntangibleAssets; //�����ʲ�
	float CurrentLiabilities; //������ծ
	float LongTermLiabilities; //���ڸ�ծ
	float TotalLiabilities; //�ܸ�ծ
	float Equity; //�ɶ�Ȩ��
	float CapitalFund; //�ʱ�������
	float OperatingCashFlow; //��Ӫ�ֽ�����
	float CashInvestmentFlow;//Ͷ���ֽ�����

	float FinancingCashFlow;//�����ֽ�����
	float IncreaseInCash; //�ֽ����Ӷ�
	float MainIncome;//��Ӫ����
	float CoreProfit; //��Ӫ����
	float OperatingProfit; //Ӫҵ����
	float InvestmentIncome; //Ͷ������
	float UnrelatedBusinessIncome; //Ӫҵ����֧
	float TotalProfit; //�����ܶ�
	float NetProfit; //������
	float UndistributedProfit; //δ��������
	float TotalEquity; //�ܹɱ�
	
	//���ǽ���������뵽���������ͨ�ɷݡ�
	//ע�⣬��λ��"���"������������10000����ʵ�ʵ���ͨ�ɡ�
	float TotalUnlimitedShares; //�����۹ɺϼ�

	float AShares; //A��
	float BShares; //B��
	float OverseasShares; //�������й�
	float OtherShares; //������ͨ��
	float TotalRestrictedShares; //���۹ɺϼ�
	float StateOwnership; //���ҳֹ�
	float StateLegalPersonShares; //���з��˹�
	float DomesticLegalPersonShares; //���ڷ��˹�	
	float DomesticNaturalPersonShares; // ������Ȼ�˹�
	float OtherPromotersShares; //���������˹�
	float RaiseCorporateShares; //ļ�����˹�
	float ForeignLegalPersonShares; //���ⷨ�˹�
	float ForeignNaturalPersonShares; //������Ȼ�˹�
	float PreferenceSharesOrOther; //���ȹɻ�����
};
//ע�⣺ȨϢ���ݴ��µ���˳������
class DbFinance
{
public:

	//��ȡ...
	//�����ص�vector����Ϊ0,�򲻺Ϸ�

	//insertʹ��vector��begin end������

	bool check(const wchar_t *filename)
	{
		bool isValidFile = false;
		const long ExrightFlag = 0x223FD90D;
		size_t size = 0;
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
			func(L"�����û�а�װ���˴ΰ�װ��������ʧ��!", 100);
			return 0;
		}
		//��ȡ�ļ�
		std::ifstream reader;  //ע�⣬����<fstream>����<iostream>
		reader.open(_file, std::ios::binary | std::ios::in);  //�Զ�����ֻ����ʽ���ļ�
		reader.seekg(0, std::ios::end);//���ļ�β��λ,beg���ļ����ֽ�Ϊ0��ʼ��λ��cur�Ե�ǰλ�ÿ�ʼ��λ
		long fileSize = (long)reader.tellg();//�õ��ļ���С
		reader.seekg(0, std::ios::beg); //�ص��ļ���һ���ֽ�
		std::shared_ptr<char> ptr(new char[fileSize], [](char* ptr){delete[] ptr; });
		reader.read(ptr.get(), fileSize); //��ȫ���ļ���ȡ���ڴ�
		reader.close(); //�ر��ļ�


		FinanceFull *position = (FinanceFull *)(ptr.get() + 8);

		int total_number = (fileSize - 8) / sizeof(FinanceFull); //һ����������¼
		int id_number = 0;

		Db conn;
		conn.Begin();
		auto cmd = conn.CreateQuery("UPDATE STOCK SET Circulation=? WHERE ID=?");
		int order = -1;
		for (int i = 0; i < total_number;i++)
		{
			order = g_stock.FindStock(position->Id);
			
			if (order>0 && g_stock.Data[order].Catalog!=4) //����Ҫ����ָ��
			{
				g_stock.Data[order].Circulation = position->TotalUnlimitedShares;
				cmd.Bind(1, g_stock.Data[order].Circulation,position->Id);
				cmd.ExcuteNonQuery();
			}
			if ((i % period) == 0)
			{
				func(nullptr, i * 80 / total_number);
			}
			position++;
		}

		conn.Commit();
		//func(nullptr, 100);
		return total_number;
		//д������ȨϢ����
	}

};