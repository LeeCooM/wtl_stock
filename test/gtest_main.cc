#include <stdio.h>
#include "gtest/gtest.h"

GTEST_API_ int main(int argc, char **argv) {
	printf("Running main() from gtest_main.cc\n");
	testing::InitGoogleTest(&argc, argv);
	int result=RUN_ALL_TESTS();
	
	//��������з�ʽ���У����ڴ���ͣ���Ա�۲������
	//����ڲ�����Դ���������У���Ҫע�͵���������Ի�һֱ���в��ȴ���
	//system("PAUSE");
	
	return result;
}