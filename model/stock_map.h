//ע�⣺Db.h��Ĭ�����ݿ�·�����Ǿֲ���̬��������model�з���ʱ����Ҫ�����ֵ�һͷ�ļ���ʽ

#include <vector>
#include <algorithm>

#include "db.h"

#ifndef STOCK_MODEL_STOCKAPI_H
#define STOCK_MODEL_STOCKAPI_H
#pragma once
//
//#ifdef MODEL_EXPORTS
//#define MODEL_API __declspec(dllexport)
//#else
//#define MODEL_API __declspec(dllimport)
//#endif

//���߽ṹ
struct Quote
{
	unsigned long QuoteTime; //ʹ��_mktime32תΪ__time32t�洢��ʹ��_localtime32��ȡ
	float Open, High, Low, Close, Volume, Amount;
};
struct ExRight
{
	int QuoteTime;
	float AddStock, BuyStock, Price, Bonus;
};

struct StockInfo
{
	//������벿��
	char Id[9];
	short Market; //��Ʊ������г�
	short Catalog;  //��Ʊ����
	char Title[9]; //��Ʊ���ƣ�4������+0
	char MiniCode[5]; //ƴ������	

	//������벿��
	size_t TotalCirculation; //�ܹɱ�
	size_t Circulation; //��ͨ��
	float EarningsPerShare; //ÿ������earnings per share

	//ȨϢ���벿��
	std::vector<ExRight> ExRightSet;

	//����
	std::vector<Quote> QuoteSet;

	//����С�ں͵��ڲ������������㷨��Ҫ
	bool operator < (const StockInfo& a) const
	{
		return (strcmp(Id, a.Id)<0);
	}
	bool operator == (const StockInfo& a)const
	{
		return (strcmp(Id, a.Id) == 0);
	}
};
class StockMap
{
public:
	std::vector<StockInfo> Data;
	unsigned long BeginDate = 0, EndDate = 0;

	void SetConnection(); //�������Ӻ�cmd��������������

	//��������
	void LoadQuote(const char *id);

	//����������޸ģ�����������룬���浽���ݿ����д���
	void UpdateStock(const char *id,const char *title); //��������id�����ڣ����롣������������Ʋ�ͬ���޸����ơ��������������δ�䣬������
	void UpdateBase(const char *id, size_t toatal_circulation,size_t circulation, float earnings_per_share);//�����3�������������������¡����벻���ڲ�������
	void UpdateExright();//ȨϢ��������������ա����һ�յ�������Ȳ�������������¡�
	
	StockMap()
	{
	}


	~StockMap()
	{
	}

	//�緵��ֵresultС��0����ʾû���ҵ���-1*result-1Ϊ����λ��
	int FindStock(const char id[9])
	{
		StockInfo stock;
		strcpy_s(stock.Id, id); //����
		std::pair<std::vector<StockInfo>::iterator, std::vector<StockInfo>::iterator> bounds;
		bounds = std::equal_range(Data.begin(), Data.end(), stock);//����
		if (bounds.first == bounds.second)
			return -1 - std::distance(Data.begin(), bounds.first); //û���ҵ�,���ظ�ֵ����ʾ�����λ�ã�ע�⣬��beginΪ0���򷵻�-1
		return (std::distance(Data.begin(), bounds.first)); //�����ҵ��ĵ�һ�����һ���ľ��룬Ҳ�������
	}

	void Load(bool refresh=false)
	{
		if (Data.size()>0)
		{
			if (!refresh)	return;
			else
				Data.clear();
		}
		Db connection;
		auto query = connection.CreateQuery(L"select * from Stock order by Id");
		StockInfo stock;
		while (query.Excute(stock.Id, stock.Market, stock.Catalog, stock.Title, stock.MiniCode, stock.TotalCirculation, stock.Circulation, stock.EarningsPerShare))
		{
			Data.push_back(stock);
		}
	}

	//���ݴ����ȡ������
	short int GetMarket(const char id[])
	{
		return (id[1] == 'H') ? 0 : (id[1] == 'Z') ? 1 : 2;
	}

	//���ݴ��룬��ȡ��Ʊ���ͣ�"����"���ͽ�������
	//#include <regex> ������ʽ��Ч�ʿ����Ե�
	short int GetCatalog(const char id[])
	{
		//�ж���֤
		if (memcmp("SH6", id, 3) == 0)
			return 1;

		//���ж�������С��
		if (memcmp("SZ002", id, 5) == 0)
			return 2;

		//���ж����ڴ�ҵ��
		if (memcmp("SZ300", id, 5) == 0)
			return 3;

		//Ȼ������A��
		if (memcmp("SZ00", id, 4) == 0)
			return 1;

		//�ĸ�ָ��
		if (memcmp(id,"SH000001",8)==0 || memcmp(id,"SZ399001",8)==0 || memcmp(id,"SZ399005",8)==0 || memcmp(id,"SZ399006",8)==0)
			return 4;
		return -1;
	}

	//��ȡƴ������ĸ
	//֧��gb2312,Ҳ֧�ִ��ַ�����GBK����
	//���ԣ�http://blog.csdn.net/blackoto/article/details/4430983
	void GetMiniCode(char strName[], char result[])
	{
		static int li_SecPosValue[] = {
			1601, 1637, 1833, 2078, 2274, 2302, 2433, 2594, 2787, 3106, 3212,
			3472, 3635, 3722, 3730, 3858, 4027, 4086, 4390, 4558, 4684, 4925, 5249
		};

		static char* lc_FirstLetter[] = {
			"A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "O",
			"P", "Q", "R", "S", "T", "W", "X", "Y", "Z"
		};

		static char* ls_SecondSecTable =
			"CJWGNSPGCGNE[Y[BTYYZDXYKYGT[JNNJQMBSGZSCYJSYY[PGKBZGY[YWJKGKLJYWKPJQHY[W[DZLSGMRYPYWWCCKZNKYYGTTNJJNYKKZYTCJNMCYLQLYPYQFQRPZSLWBTGKJFYXJWZLTBNCXJJJJTXDTTSQZYCDXXHGCK[PHFFSS[YBGXLPPBYLL[HLXS[ZM[JHSOJNGHDZQYKLGJHSGQZHXQGKEZZWYSCSCJXYEYXADZPMDSSMZJZQJYZC[J[WQJBYZPXGZNZCPWHKXHQKMWFBPBYDTJZZKQHY"
			"LYGXFPTYJYYZPSZLFCHMQSHGMXXSXJ[[DCSBBQBEFSJYHXWGZKPYLQBGLDLCCTNMAYDDKSSNGYCSGXLYZAYBNPTSDKDYLHGYMYLCXPY[JNDQJWXQXFYYFJLEJPZRXCCQWQQSBNKYMGPLBMJRQCFLNYMYQMSQYRBCJTHZTQFRXQHXMJJCJLXQGJMSHZKBSWYEMYLTXFSYDSWLYCJQXSJNQBSCTYHBFTDCYZDJWYGHQFRXWCKQKXEBPTLPXJZSRMEBWHJLBJSLYYSMDXLCLQKXLHXJRZJMFQHXHWY"
			"WSBHTRXXGLHQHFNM[YKLDYXZPYLGG[MTCFPAJJZYLJTYANJGBJPLQGDZYQYAXBKYSECJSZNSLYZHSXLZCGHPXZHZNYTDSBCJKDLZAYFMYDLEBBGQYZKXGLDNDNYSKJSHDLYXBCGHXYPKDJMMZNGMMCLGWZSZXZJFZNMLZZTHCSYDBDLLSCDDNLKJYKJSYCJLKWHQASDKNHCSGANHDAASHTCPLCPQYBSDMPJLPZJOQLCDHJJYSPRCHN[NNLHLYYQYHWZPTCZGWWMZFFJQQQQYXACLBHKDJXDGMMY"
			"DJXZLLSYGXGKJRYWZWYCLZMSSJZLDBYD[FCXYHLXCHYZJQ[[QAGMNYXPFRKSSBJLYXYSYGLNSCMHZWWMNZJJLXXHCHSY[[TTXRYCYXBYHCSMXJSZNPWGPXXTAYBGAJCXLY[DCCWZOCWKCCSBNHCPDYZNFCYYTYCKXKYBSQKKYTQQXFCWCHCYKELZQBSQYJQCCLMTHSYWHMKTLKJLYCXWHEQQHTQH[PQ[QSCFYMNDMGBWHWLGSLLYSDLMLXPTHMJHWLJZYHZJXHTXJLHXRSWLWZJCBXMHZQXSDZP"
			"MGFCSGLSXYMJSHXPJXWMYQKSMYPLRTHBXFTPMHYXLCHLHLZYLXGSSSSTCLSLDCLRPBHZHXYYFHB[GDMYCNQQWLQHJJ[YWJZYEJJDHPBLQXTQKWHLCHQXAGTLXLJXMSL[HTZKZJECXJCJNMFBY[SFYWYBJZGNYSDZSQYRSLJPCLPWXSDWEJBJCBCNAYTWGMPAPCLYQPCLZXSBNMSGGFNZJJBZSFZYNDXHPLQKZCZWALSBCCJX[YZGWKYPSGXFZFCDKHJGXDLQFSGDSLQWZKXTMHSBGZMJZRGLYJB"
			"PMLMSXLZJQQHZYJCZYDJWBMYKLDDPMJEGXYHYLXHLQYQHKYCWCJMYYXNATJHYCCXZPCQLBZWWYTWBQCMLPMYRJCCCXFPZNZZLJPLXXYZTZLGDLDCKLYRZZGQTGJHHGJLJAXFGFJZSLCFDQZLCLGJDJCSNZLLJPJQDCCLCJXMYZFTSXGCGSBRZXJQQCTZHGYQTJQQLZXJYLYLBCYAMCSTYLPDJBYREGKLZYZHLYSZQLZNWCZCLLWJQJJJKDGJZOLBBZPPGLGHTGZXYGHZMYCNQSYCYHBHGXKAMTX"
			"YXNBSKYZZGJZLQJDFCJXDYGJQJJPMGWGJJJPKQSBGBMMCJSSCLPQPDXCDYYKY[CJDDYYGYWRHJRTGZNYQLDKLJSZZGZQZJGDYKSHPZMTLCPWNJAFYZDJCNMWESCYGLBTZCGMSSLLYXQSXSBSJSBBSGGHFJLYPMZJNLYYWDQSHZXTYYWHMZYHYWDBXBTLMSYYYFSXJC[DXXLHJHF[SXZQHFZMZCZTQCXZXRTTDJHNNYZQQMNQDMMG[YDXMJGDHCDYZBFFALLZTDLTFXMXQZDNGWQDBDCZJDXBZGS"
			"QQDDJCMBKZFFXMKDMDSYYSZCMLJDSYNSBRSKMKMPCKLGDBQTFZSWTFGGLYPLLJZHGJ[GYPZLTCSMCNBTJBQFKTHBYZGKPBBYMTDSSXTBNPDKLEYCJNYDDYKZDDHQHSDZSCTARLLTKZLGECLLKJLQJAQNBDKKGHPJTZQKSECSHALQFMMGJNLYJBBTMLYZXDCJPLDLPCQDHZYCBZSCZBZMSLJFLKRZJSNFRGJHXPDHYJYBZGDLQCSEZGXLBLGYXTWMABCHECMWYJYZLLJJYHLG[DJLSLYGKDZPZXJ"
			"YYZLWCXSZFGWYYDLYHCLJSCMBJHBLYZLYCBLYDPDQYSXQZBYTDKYXJY[CNRJMPDJGKLCLJBCTBJDDBBLBLCZQRPPXJCJLZCSHLTOLJNMDDDLNGKAQHQHJGYKHEZNMSHRP[QQJCHGMFPRXHJGDYCHGHLYRZQLCYQJNZSQTKQJYMSZSWLCFQQQXYFGGYPTQWLMCRNFKKFSYYLQBMQAMMMYXCTPSHCPTXXZZSMPHPSHMCLMLDQFYQXSZYYDYJZZHQPDSZGLSTJBCKBXYQZJSGPSXQZQZRQTBDKYXZK"
			"HHGFLBCSMDLDGDZDBLZYYCXNNCSYBZBFGLZZXSWMSCCMQNJQSBDQSJTXXMBLTXZCLZSHZCXRQJGJYLXZFJPHYMZQQYDFQJJLZZNZJCDGZYGCTXMZYSCTLKPHTXHTLBJXJLXSCDQXCBBTJFQZFSLTJBTKQBXXJJLJCHCZDBZJDCZJDCPRNPQCJPFCZLCLZXZDMXMPHJSGZGSZZQLYLWTJPFSYASMCJBTZKYCWMYTCSJJLJCQLWZMALBXYFBPNLSFHTGJWEJJXXGLLJSTGSHJQLZFKCGNNNSZFDEQ"
			"FHBSAQTGYLBXMMYGSZLDYDQMJJRGBJTKGDHGKBLQKBDMBYLXWCXYTTYBKMRTJZXQJBHLMHMJJZMQASLDCYXYQDLQCAFYWYXQHZ";

		int H, L, W;
		int result_no = 0;
		unsigned int i, stringlen = strlen(strName);
		int j;
		for (i = 0; i < stringlen; i++) {
			H = (unsigned char)(strName[i + 0]);
			L = (unsigned char)(strName[i + 1]);

			if (H < 0xA1 || L < 0xA1) {

				//���Ǻ��ֵĺ��Ե�
				//result[result_no] = strName[i];
				//result_no++;
				continue;
			}
			else {
				W = (H - 160) * 100 + L - 160;
			}

			if (W > 1600 && W < 5590) {
				for (j = 22; j >= 0; j--) {
					if (W >= li_SecPosValue[j]) {
						result[result_no] = lc_FirstLetter[j][0];
						result_no++;
						i++;
						break;
					}
				}

				continue;
			}
			else {
				i++;
				W = (H - 160 - 56) * 94 + L - 161;
				if (W >= 0 && W <= 3007)
				{
					result[result_no] = ls_SecondSecTable[W];
					result_no++;
				}
				else {
					//ע�͵���ȥ��ȫ�ǵ�A��B�����ֵ�
					//result[result_no]= (char)H;
					//++result_no;
					//result[result_no]= (char)L;
					//++result_no;
				}
			}
		}
		return;
	}


};


//�����й����е����ͣ�
//���Դ�Сд 
//std::regex_constants::syntax_option_type fl = std::regex_constants::icase;
//// ������ҵĽ��  
//std::smatch ms;
////A�ɱ��ʽ 
//std::regex Ashares("SH60[0-8]\\d{3}|SZ00[01256789]\\d{3}");        //A��
//std::regex SmallAndMedium("(SZ002\\d{3})");						//������С�� 
//std::regex Growth("(SZ300\\d{3})");								//���ڴ�ҵ�� 
//std::wregex Index(L"SH000\\d{3}|SH8[013]\\d{4}|SZ399\\d{3}");        //ָ��
//std::wregex Bshares(L"SH90\\d{4}|SZ20\\d{4}");					    //B��
//std::wregex Fund(L"SH5[01]\\d{4}|SZ184\\d{3}|SZ1[56]\\d{4}");        //����
//std::wregex Bond(L"SH[012]\\d{5}|SZ1[0123]\\d{4}");                  //ծȯ
//std::wregex Futures(L"SF\\w{6}");                                    //�ڻ�
//std::wregex Warrant(L"SH58\\d{4}|SZ03\\d{4}");					    //Ȩ֤ 
//if (std::regex_match(code, ms, Ashares))
//{
//	return ::Ashares;
//}
//if (std::regex_match(code, ms, SmallAndMedium))
//{
//	return ::SmallAndMedium;
//}
//if (std::regex_match(code, ms, Growth))
//{
//	return ::Growth;
//}

// if(std::regex_match(code,ms,Index))
//{
//	return ::Index;
//}
//if(std::regex_match(code,ms,Bshares))
//{
//	return ::Bshares;
//}
//	if(std::regex_match(code,ms,Fund))
//{
//	return ::Fund;
//}
//if(std::regex_match(code,ms,Bond))
//{
//	return ::Bond;
//}
//if(std::regex_match(code,ms,Warrant))
//{
//	return ::Warrant;
//}
//if(std::regex_match(code,ms,Futures))
//{
//	return ::Futures;
//}
#endif