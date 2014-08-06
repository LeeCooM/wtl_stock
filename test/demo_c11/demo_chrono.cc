#include <chrono>
#include "gtest/gtest.h"
using namespace std;

//ʱ��㣺����ʱ���֮��ļ��
TEST(demo, chrono_time_point)
{
	//system_clock��ϵͳʱ��,���û��ڼ�ʱ�׶��޸���ʱ�䣬�����������Ӱ��
	//steady_clock�����ܱ��û��޸ĵ�ʱ�ӣ�����֮����ʱ�ڼ伴ʹ�û��޸���ʱ�ӣ�����Ҳ����Ӱ�졣�������ʹ��ϵͳʱ�ӣ�
	//high_resolution_clock���߾���ʱ�ӣ�ʵ������system_clock��steady_clock��

	std::chrono::steady_clock::time_point t1 = std::chrono::system_clock::now();
	std::chrono::steady_clock::time_point t2 = std::chrono::system_clock::now();
	auto countByClock = (t2 - t1).count(); //���ֲ��Ϊticks����ʱ�����ڣ���΢��Ҫ��
	//EXPECT_GT(i, 0);//����0
	EXPECT_GE(countByClock, 0);//���ڵ���0�����̫�̴˴�Ϊ0

	//duration_cast���ڽ�ʱ��ļ����ת��Ϊ��ĳ�����ڱ�����ʽ��chrono::seconds��Ԥ�������(1��)
	auto countBySecond = chrono::duration_cast<std::chrono::seconds>(t2 - t1).count(); //�����תΪ��
	EXPECT_GE(countBySecond, 0);
}

//��ctime����໥ת��
TEST(demo, chrono_and_ctime)
{
	std::time_t firstTime = (std::time_t)1402876800;
	
	//��time_tת��Ϊʱ���
	std::chrono::steady_clock::time_point aTime_point=chrono::system_clock::from_time_t(firstTime);

	//time_since_eproch�������1970��1��1�յ�time_point��������duration��
	auto real_duration = aTime_point.time_since_epoch().count();
	EXPECT_EQ((long long)1402876800 * 10000000, real_duration); //���仰˵��1���ǧ���֮һ
	auto real_seconds = chrono::duration_cast<std::chrono::seconds>(aTime_point.time_since_epoch()).count();
	EXPECT_EQ((long long)1402876800 , real_seconds); //����������������㣬ʱ�������time_t����Դһ��
	//Ҳ��ʹ��chrono::time_point_cast��ʱ���ת��Ϊ�������

	//Ҳ��ת����ȥ�����±��time_t
	std::time_t secondTime = std::chrono::system_clock::to_time_t(aTime_point);
	EXPECT_EQ(firstTime, secondTime);
}

//ת��Ϊ"2014-1-1"������ʽ��������ʾ
TEST(demo, chrono_to_string)
{
	std::time_t firstTime = (std::time_t)1402876800;


	//ʱ��ת�ַ�����size_t strftime( char *strDest, size_t maxsize, const char *format, co
	//ת����time_point
	//std::chrono::steady_clock::time_point aTime_point = chrono::system_clock::from_time_t(firstTime);
	//auto first_string=std::put_time(std::localtime(&firstTime), "%Y-%m-%d %X");
	//EXPECT_STREQ("2014-01-01 09:00:00",first_string);
}
