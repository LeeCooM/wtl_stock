//ͷ�ļ����ݿ������
#include "global.h"
#include "sqlite/sqlite3.h"
#include "db.h"
#include "dad_parse_iterator.h"
#include <vector>
#include <algorithm>

#ifndef db_code_h
#define db_code_h

class DbCode
{
public:
	DbCode() = default;
	~DbCode() = default;

	//��ȡȫ����Ʊ������Ϣ��������global::StockSet��
	//ʱ������Ӧ������ʱ����ÿֻ��Ʊ��ص����ߡ�ȨϢ���ݣ��������ϴ󣬸������ΰ�������
	//Ӧ�ó�������ʱ��Global::init����get_date_range����Ѱ�װ���ݵ���ֹ���ڣ������ô˷�����ô����
	//��װ��������ʱ���ɿ��Ƿ��´���StockSet����������ߣ���˳�����¶���
	//��ȡȫ�����ߣ���Ҫ2000-3000�ε����������ÿ��Լ2000�����ݣ�����Ϊ600��β�����
	static void GetAllCode(bool refresh=false)
	{
		if (DbCode::get_stock_list().size()>0)
		{
			if (!refresh)	return;
			else
				DbCode::get_stock_list().clear();
		}
		Db connection_;
		auto query = connection_.create_query(L"select * from Stock order by Id");
		Stock stock;
		int market, catalog;
		while (query.Execute(stock.Id, market,catalog, stock.Title, stock.MiniCode))
		{
			stock.Market = (MarketEnum)market;
			stock.Catalog = (CatalogEnum)catalog;
			DbCode::get_stock_list().push_back(stock);
		}
		
	}

	static vector<Stock> &get_stock_list()
	{
		static vector<Stock> stock_list;
			return stock_list;
	}

	//�緵��ֵresultС��0����ʾû���ҵ���-1*result-1Ϊ����λ��
	static int FindStock(char id[])
	{
		Stock stock;
		strcpy_s(stock.Id, id); //����
		pair<vector<Stock>::iterator, vector<Stock>::iterator> bounds;
		bounds = std::equal_range(DbCode::get_stock_list().begin(), DbCode::get_stock_list().end(), stock);//����
		if (bounds.first == bounds.second)
			return -1 - std::distance(DbCode::get_stock_list().begin(), bounds.first); //û���ҵ�,���ظ�ֵ����ʾ�����λ�ã�ע�⣬��beginΪ0���򷵻�-1
		return (std::distance(DbCode::get_stock_list().begin(), bounds.first)); //�����ҵ��ĵ�һ�����һ���ľ��룬Ҳ�������
	}

	//���ݴ����ȡ������
	static MarketEnum GetMarket(const char id[])
	{
		return (id[1] == 'H') ? ::ShangHai : (id[1] == 'Z') ? ::ShenZhen : (::QiHuo);
	}

	//���ݴ��룬��ȡ��Ʊ���ͣ�"����"���ͽ�������
	//#include <regex> ������ʽ��Ч�ʿ����Ե�
	static CatalogEnum GetCatalog(const char id[])
	{
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

		//ֻҪ���ĸ�ָ����
		if (id == "SH000001" || id == "SZ399001" || id == "SZ399005" || id == "SZ399006")
			return ::Index;

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

		//�ж���֤
		if (memcmp("SH6", id, 3) == 0)
			return ::Ashares;
		
		//���ж�������С��
		if (memcmp("SZ002", id, 5) == 0)
			return ::SmallAndMedium;

		//���ж����ڴ�ҵ��
		if (memcmp("SZ300", id, 5) == 0)
			return ::Growth;

		//Ȼ������A��
		if (memcmp("SZ00", id, 4) == 0)
			return ::Ashares;

		//�ĸ�ָ��
		if (id == "SH000001" || id == "SZ399001" || id == "SZ399005" || id == "SZ399006")
			return ::Index;
		return ::Others;
	}

	//��ȡƴ������ĸ
	//֧��gb2312,Ҳ֧�ִ��ַ�����GBK����
	//���ԣ�http://blog.csdn.net/blackoto/article/details/4430983
	static void GetMiniCode(char strName[],char result[])
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

#endif