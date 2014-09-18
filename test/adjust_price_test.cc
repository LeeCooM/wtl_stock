#include <vector>
#include "gtest/gtest.h"
#include "global.h"
#include "db_quote.h"

//������
//1.��Ԫ���ԣ�����Ҫʵ�����ݣ���������ֵ����
//2.����wind������ǵ���ǰ��Ȩ������֮����֤�Ǽ��պ͸�Ȩǰһ����ǵ�����ȷ����֤���ռ۸�����̼۵�ƫ����ȷ��
//3.�Ȳ���3�������µ�ǰ��Ȩ����


//�������ݣ�����10������������
std::vector<Quote>   quotes =
{
	{1409011200,21.58f,21.79f,21.90f,21.21f,87393.0f,187047984.0f}, //20120518����Ȩǰһ��
	{1409097600,12.88f,12.90f,12.57f,12.68f,50629.0f,64518388.0f},  //20120512,��Ȩ��
	{ 1409184000, 7.81f, 7.94f, 7.72f, 7.93f, 111151.0f, 87543768.000f},//20130517,��Ȩǰһ��
	{ 1409270400, 7.88f, 8.13f, 7.72f, 8.11f, 136405.0f, 108198920.000f},//20130520,��Ȩ��
	{ 1409529600, 6.4f, 6.52f, 6.32f, 6.33f, 178206, 113996256.000f},//20131213��Ȩǰһ�գ�֮��ͣ��10��
	{ 1409616000, 5.44f, 5.61f, 5.34f, 5.48f, 126405.0f, 69211328.000f},//20131224��Ȩ��
	{1409702400,5.13f,5.17f,5.10f,5.16f,121149.0f,62278860.000f},//20140616��Ȩǰ2��
	{1409788800,5.13f,5.16f,5.10f,5.16f,137956.0f,70828760.0f}, //20140617 ��Ȩǰһ��
	{1409875200,4.97f,4.98f,4.80f,4.83f,156783.0f,76399296.000f}, //20140618 ��Ȩ��
	{1410220800,4.82f,4.92f,4.67f,4.69f,135345.0f,64897560.0f},//20140619 ��Ȩ��һ��
};
//�޻��ɷ�ǰ����ȨϢ����
//ע��ʱ�䵹�����У�������ǰ
std::vector<ExRight> exrights = {
	{ 1409875200, 0.0f, 0.0f, 0.0f, 0.2f },
	{ 1409616000, 0.0f,0.3f, 4.23f, 0.0f },
	{ 1409270400, 0.0f, 0.0f, 0.0f, 0.15f},
	{ 1409097600,0.6f,0.0f,0.0f,0.5f},
};
//������ǰ��Ȩ�㷨
//ģ��10������,3��ȨϢ����
//ΪȨϢ���ݣ����㸴Ȩ���Ӻ�ʱ���
//��Ȩ����ʹ��ʵ�ʵ�����..������������ʵ�������еļ����ؼ��㼴ǰ��һ����
//�޻��ɷݣ�������1���ֺ죻2���͹ɣ�3����ɣ�4���͹ɼӷֺ�
//2006


//��ǰ��Ȩ�������Ȩ�Ǽ���
//�������ߣ����ҵ����ڣ�������Ϊ��Ȩ�Ǽ���
//��δ�ҵ��������λ��(��һ�����ڸ����ڵļ�¼)+1��Ҳ���ǵ�һ�����ڸ����ڵģ�Ϊ�Ǽ���
//�Ǽ������-1Ϊǰ��ȨӦ�����λ�á�
TEST(AdjustPrice, GetExOrder)
{
	DbQuote dbQuote;

	//���ҵڶ�����
	int rc = dbQuote.GetExOrder(quotes, 1409097600);
	EXPECT_EQ(1, rc);

	//���Ҳ����ڣ����ȵڶ���С��
	rc = dbQuote.GetExOrder(quotes, 1409097599);
	EXPECT_EQ(1, rc);

	//Խ�磺���ұȵ�һ��С��
	rc = dbQuote.GetExOrder(quotes, 1409011199);
	EXPECT_EQ(0, rc);

	//Խ�磺���ұ����һ�����
	rc = dbQuote.GetExOrder(quotes, 1410220801);
	EXPECT_EQ(10, rc); //�������һ�����+1���µĲ���λ��

	//�����е�����ҳ���
	dbQuote.GetExOrder(quotes, exrights);
	EXPECT_EQ(8, exrights[0].Start);
	EXPECT_EQ(5, exrights[1].Start);
	EXPECT_EQ(3, exrights[2].Start);
	EXPECT_EQ(1, exrights[3].Start);


}
//���춯�� 600343
//20140423Ϊ��Ȩ�Ǽ��գ�ǰһ�쿪ʼ��Ȩ...��С�ڹ�Ȩ�Ǽ��գ��Ҳ����ڡ�
//ǰ��Ȩ����Ȩ��۸�[(��Ȩǰ�۸� - �ֽ����)����(��)�ɼ۸����ͨ�ɷݱ䶯����]��(1����ͨ�ɷݱ䶯����)
//��Ȩ����Ȩ��۸񣽸�Ȩǰ�۸��(1����ͨ�ɷݱ䶯����) - ��(��)�ɼ۸����ͨ�ɷݱ䶯�������ֽ����
TEST(AdjustPrice, CaculateFactor) //���㸴Ȩ����
{
	DbQuote dbQuote;

	//�ȼ���˳��
	dbQuote.GetExOrder(quotes, exrights);

	//�����һ����Ȩ����
	float factor=dbQuote.CaculateFactor(quotes, exrights[0]);
	//EXPECT_FLOAT_EQ(0.96124035f,factor);
	EXPECT_FLOAT_EQ(4.96f, quotes[exrights[0].Start - 1].Close/factor);
	//6��17�գ����ǻ�ʵ��5.16��ǰ��Ȩ����ļ۸�Ϊ4.96,ͨ������ͬ��

	factor = factor*dbQuote.CaculateFactor(quotes, exrights[1]); //���㵹���ڶ��θ�Ȩ����
	EXPECT_FLOAT_EQ(5.6188192f, quotes[exrights[1].Start - 1].Close/factor);
	//ͨ����ǰ��ȨֵΪ5.65,��0.03

	//�����0.03���ƺ����������м�������ɵģ�
	//��ͨ����ʹ�õݹ�ǰ��Ȩ��
	dbQuote.CaculateFactor(quotes, exrights);
	EXPECT_FLOAT_EQ(4.96f, quotes[exrights[0].Start - 1].Close/exrights[0].Factor); //4.96ͨ���źʹ��ǻ�4.96
	EXPECT_FLOAT_EQ(5.6188192f, quotes[exrights[1].Start - 1].Close/exrights[1].Factor); //5.62ͨ���źʹ��ǻ�5.65

	//������ͨ���š����ǻ۾�Ϊ6.76
	EXPECT_FLOAT_EQ(6.9059105f, quotes[exrights[2].Start - 1].Close/exrights[2].Factor);//6.91ͨ���źʹ��ǻ�6.76



}

TEST(AdjustPrice, Adjust) //����ǰ��Ȩ����������ĸ��۸񡢳ɽ���
{
	DbQuote dbQuote;
	std::vector<Quote> backup_quote;
	backup_quote.assign(quotes.begin(), quotes.end());
	dbQuote.AdjustPrice(backup_quote,exrights);

	float rate_for_old = (quotes[7].Close - quotes[6].Close) / quotes[6].Close;
	float rate_for_adjust = (backup_quote[7].Close - backup_quote[6].Close) / backup_quote[6].Close;
	EXPECT_FLOAT_EQ(rate_for_adjust, rate_for_old);

}

//������ͨ�ɱ�(����������ͨ�ɺ����������)
//��ͨ�ɱ�������ȨϢ������
TEST(AdjustPrice,TotalShares) 
{

}