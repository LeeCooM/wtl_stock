#include <unordered_map>
#include <concurrent_unordered_map.h>
#include <unordered_set>
#include "gtest/gtest.h"
using namespace std;

//ע�⣬unordered_mapϵ����c++ 11��׼��ģ�֮ǰstl��hash_map֮��Ľṹ����c++ ��׼�ġ�
TEST(DemoC11, unordered_map)
{
	//����ʱ����ָ����ֵ����
	//������Ĭ�ϵĲ������鿴Դ��ɿ�����
	//class _Hasher = hash<_Kty>,
	//class _Keyeq = equal_to<_Kty>,
	unordered_map<std::string,int> int_map;
	int_map.emplace("SH000002", 1);
	EXPECT_EQ(1, int_map.size());

	//EXPECT_EQ(8, int_map.bucket_count()); //���ٸ�"Ͱ"����ʼ�ͷ�����8������������ֲ�Ӧ�ǹ̶���
	int_map["SH000003"] = 3; //���±귽ʽ���ʣ��������ڣ����Զ�����
	int_map["SH000001"] = 2;
	EXPECT_EQ(3, int_map.size());

	//EXPECT_EQ(8, int_map.bucket_count());
	
	//��װhashֵ�����򼸸������ƣ���۱���Ϊ�������˳������
	//ע�⣺���ǰ�����˳������
	//��ˣ�����ֱ�Ӱ�˳���������ֻ����������
	EXPECT_EQ("SH000002", (*(int_map.begin())).first);
	EXPECT_EQ("SH000003", (*(++int_map.begin())).first);
	EXPECT_EQ("SH000001", (*(++++int_map.begin())).first);

	//��ˣ���Թ�Ʊ�����
	//1.��������ҵ�ĳ������
	//2.�ζ����󣺰�id˳����ʾ��ά��һ��vector��������id�����ݱ仯������
	//3.���ַ�ʽ��ʹ��map����ѯ����Ҳ����(�۰�)��ͬʱ��˳��ά��(��������)

	//���е���Щ����������Ҫ�����������ڴ������
	//��ˣ�Ԥ��reverse��С��Ȼ��������push������Ԥ���趨�Ĵ�С����������õ�������

	//unordered_set��map�Ĳ�ͬ����ֻ����������Ǽ�ֵ�ԣ�Ҳ���Ǽ��ϵĸ���
	//��Ȼ�����Ƕ����˼��ϵ��롢�򡢷ǲ����������Ǻ�unordered_map���ƣ���û��"ֵ"��������
	unordered_set<string> str_set;
	str_set.emplace("second"); 
	str_set.emplace("first");
	str_set.emplace("third");
	//unordered_set<string> second_set;

}