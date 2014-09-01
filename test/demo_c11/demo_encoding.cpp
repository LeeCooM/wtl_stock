#include <string>
//#include <locale>
#include <codecvt>
#include "gtest/gtest.h"
using namespace std;

//���ۣ�
//���ǻ�ʹ��std::string��ʽ��gb2312������Ϊ����Դ�������ʾ����
//������ȫ��ʹ��unicode
//sqlite�У�unicode�Ĳ�ѯ�������⣬��ֻ��ʹ��utf8����һ��Ҫ����һ�¡�
//����֮�������ⲿ����Դ��Ҫ������������Ӧ����ͳһʹ��unicode��utf8


//��wstringת����utf8��ʹ��c++��׼��ʽ����ʹ��windows api
std::string to_utf8(const std::wstring& str)
{
	//����һ��ת����
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	//��wstringת����wtf8
	return conv.to_bytes(str);
	
	//���������ת����
	// std::wstring wideStr = conv.from_bytes(narrowStr);
}

//gb2312תunicode
std::wstring gb2312_to_unicode(const std::string& str)
{
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>conv(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));//GBK<->Unicodeת����
	//����һ��ת����
	//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	//��wstringת����wtf8
	return conv.from_bytes(str);

	//���������ת����
	// std::wstring wideStr = conv.from_bytes(narrowStr);
}

//����ת��
TEST(demo, encoding_convert)
{
	auto result = to_utf8(L"���");
	EXPECT_EQ(6, result.length());
}

//c++ 11���ַ����ͣ�char16_t,char_32t,�ֱ�ʹ��u��U��Vs2013��֧��
//utf_16�ֳ�utf16_LE��utf16_BE���֣�ǰ����Сͷ���Ƚϳ��ã�Ϊ0x61,0x00��cpu�ǵ��ųԵģ������ܹ�����ٶ�
//L"a"��ʵ������Сͷ������vs2013Ĭ�ϵ���0x61,0x00���ּ�utf_16BE...��סBig
//utf16_BE,big endian�����Ϊ0x00,0x61
TEST(demo, encoding)
{
	const wchar_t wcharString[] = L"a"; //����0x61,0x00,0x00��0x00��
	const wchar_t nextString[] = L"��";
	const char thirdString[] = "��";

	int size = sizeof(L"ab");
	//����wchar����
	EXPECT_EQ((size_t)1, strlen(((char *)wcharString))); //�ַ����Ƕȣ�ֻ��һ���ַ�
	EXPECT_EQ((size_t)4, sizeof(wcharString)); //����Ϊ4���ֽ�

	//ע��unsigned char *���ܺ�������ȷ�Ƚ�
	EXPECT_EQ(0x61, ((unsigned char *)wcharString)[0]); //��һ���ֽ�Ϊ0x61
	EXPECT_EQ(0x00, ((unsigned char *)wcharString)[1]); //�ڶ���Ϊ0�����ڸ��ַ��ڶ����ֽ�Ϊ0������char *��lenΪ1
	EXPECT_EQ(0x00, ((unsigned char *)wcharString)[2]); //��β����Ϊ����0
	EXPECT_EQ(0x00, ((unsigned char *)wcharString)[3]);

	//���֣�
	EXPECT_EQ((size_t)2, strlen(((char *)nextString))); //��Ϊ�ַ���������λ
	EXPECT_EQ((size_t)1, wcslen(nextString));//��Ϊ���ַ���1λ
	EXPECT_EQ((size_t)4, sizeof(nextString)); //ʵ�ʳ���Ϊ4

	EXPECT_EQ(0x4A, ((unsigned char *)nextString)[0]); //'��'��unicode�����һλ
	EXPECT_EQ(0x55, ((unsigned char *)nextString)[1]); //'��'��unicode����ڶ�λ
	EXPECT_EQ(0x00, ((unsigned char *)nextString)[2]); //ʣ����λ0Ϊ��������
	EXPECT_EQ(0x00, ((unsigned char *)nextString)[3]);
    
	//char[]���ĺ���
	EXPECT_EQ((size_t)3, sizeof(thirdString)); //����Ӧ��3λ��һ��˫�ֽ��ַ���0
	EXPECT_EQ(0xB0, ((unsigned char *)thirdString)[0]); //'��'��unicode�����һλ
	EXPECT_EQ(0xA1, ((unsigned char *)thirdString)[1]); //'��'��unicode����ڶ�λ
	EXPECT_EQ(0x00, ((unsigned char *)thirdString)[2]); //ʣ����λ0Ϊ��������

	std::wstring result = gb2312_to_unicode(thirdString);
	auto p = result.c_str();
	EXPECT_EQ(0x4A, ((unsigned char *)p)[0]); //'��'��unicode�����һλ
	EXPECT_EQ(0x55, ((unsigned char *)p)[1]); //'��'��unicode����ڶ�λ
	EXPECT_EQ(0x00, ((unsigned char *)p)[2]); //ʣ����λ0Ϊ��������
	EXPECT_EQ(0x00, ((unsigned char *)p)[3]);

	//���ۣ�char����ĺ��֣���wchar_t����ĺ��֣����ֽ��ǲ�ͬ��
	//1.ͬһ�����֣�char����Ϊ3λ��ǰ��λ��ʾһ�����֣������0��wchar_t������λ������λΪ0
	//2.ͬ����"��",wchar_t��unicode��Ϊ4a 55,charΪgb2312��Ϊb0,a1
	//3.���char[]��wchar_t��Ҫת����unicode

	//
	//EXPECT_STREQ("1\\", R"(1\)");//Vs2013��R��ʾԭ���ַ���
	//���ǲ��ܱ���ģ�Assert::AreEqual("1\\", R"1\");
}