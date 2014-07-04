#include <string>
//#include <locale>
#include <codecvt>
#include "gtest/gtest.h"
using namespace std;



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
	const wchar_t wcharString[] = L"a"; //����0x61,0x00,0x00��
	const wchar_t nextString[] = L"��";
	EXPECT_EQ((size_t)1, strlen(((char *)wcharString)));
	EXPECT_EQ(0x61, ((char *)wcharString)[0]);
	EXPECT_EQ(0x00, ((char *)wcharString)[1]);
	EXPECT_EQ(0x00, ((char *)wcharString)[2]);
	EXPECT_EQ((size_t)2, strlen(((char *)nextString)));
	EXPECT_EQ((size_t)1, wcslen(nextString));//��Ϊ���ַ���1λ

	EXPECT_STREQ("1\\", R"(1\)");//Vs2013��R��ʾԭ���ַ���
	//���ǲ��ܱ���ģ�Assert::AreEqual("1\\", R"1\");
}