#include <cmath>
#include <math.h>
#include <cfenv>
#include "gtest/gtest.h"
using namespace std;


double round(double& x, int n) //��x�������뱣��nΪС��
{
	double int_of_x;
	double t2 = modf(x, &int_of_x); //cmath�������������ַ���t1������С�����֡�
	double t3 = pow(10, n); //10��N�η�
	long t4 = long(t2*t3 + 0.5); //����õ�С������
	x = int_of_x + t4 / t3; //С�����ָ�ԭ
	return x;
}
//����������������
TEST(DemoCpp, float)
{
	//c99֮�󣬸���������������
	float float_one = 10.2478f;
	double double_first = 10.24;
	long double long_double_first = 10.24;
	//c99֮��

	//rc��ʱ0.19999999....
	float rc=roundf(float_one*10.0f) / 10.0f;
	EXPECT_DOUBLE_EQ(10.2f, rc);//

	rc = (int)(float_one * 10.0f + 0.5f) / 10.0f;
	EXPECT_DOUBLE_EQ(10.2f, rc);//

	rc = float_one;
	rc = rc*10.0f;
	rc = (rc>(floor(rc) + 0.5)) ? ceil(rc) : floor(rc);
	rc = rc / 10.0f;
	EXPECT_DOUBLE_EQ(10.2f, rc);//

	

	double x = 1.53546;
	auto result = round(x, 3);
	EXPECT_DOUBLE_EQ(1.535, result);//
}