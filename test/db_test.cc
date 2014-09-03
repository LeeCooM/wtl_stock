
#include "gtest/gtest.h"
#include "db.h"
#include "global.h"

bool create_demo_database(const char * path)
{
	sqlite3 *db = NULL;
	int rc = sqlite3_open_v2(path, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	char * pErrMsg = 0;

	rc = sqlite3_exec(db,
		"PRAGMA page_size=4096;"
		"CREATE TABLE if not exists [Product] ("
		"[ID] INTEGER PRIMARY KEY AUTOINCREMENT,"
		"[TITLE] nchar[20] NOT NULL,"
		"[WTITLE] nchar[20] NOT NULL,"
		"[DATE] INTEGER,"		
		"[PRICE] FLOAT,"
		"[NUMBER] INTEGER,"
		"[IMAGE] Blob); ",
		0, 0, &pErrMsg);
	if (rc != SQLITE_OK)
	{
		printf("Error message: %s\n", pErrMsg);
		sqlite3_free(pErrMsg);//����д�������Ҫfree���صĴ�����Ϣ
		sqlite3_close_v2(db);
		return false;
	}
	else
	{
		sqlite3_close_v2(db);
		return true;
	}
}
class dbTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		_unlink("ctest.db");
		_unlink("wtest.db");
		_unlink("test.db");
		Db::set_default_path("ctest.db", create_demo_database);
		Db conn;

		//����6���ֶΣ����ֻ��д5���ֶλᴥ���쳣
		Query cmd = conn.create_query("INSERT INTO PRODUCT VALUES (?,?,?,?,?,?,?)");

		//��2��ʼbind����Ϊ��һ�����������ֶ�
		//ֻbind 4�����������һ��blob�ֶ�û��bind
		cmd.bind(2, "first", "��һ��", (unsigned long long)1402876800, (double)100.10, (int)10);
		cmd.excute_non_query();

		cmd.bind(2, "second", L"�ڶ���", (int)1402876800, (float)200.20, (int)20);
		cmd.excute_non_query();

		std::string first_string = "third";
		std::wstring first_wstring = L"������";
		cmd.bind(2, first_string, first_wstring, (int)1402876800, (float)300.20, (int)30);
		cmd.excute_non_query();
	}

	virtual void TearDown()
	{
		
	}
};

////db��ľ�̬����set_default
TEST_F(dbTest, set_default_test)
{
	auto rc = Db::set_default_path("ctest.db",create_demo_database);
	EXPECT_STREQ("ctest.db", Db::default_path()->c_str());
}

//db��ľ�̬����set_defaultʹ��wstring
TEST_F(dbTest, set_default_wstring_test)
{
	auto rc = Db::set_default_path(L"wtest.db",create_demo_database);
	EXPECT_STREQ("wtest.db", Db::default_path()->c_str());
}

//db�๹�캯��
TEST_F(dbTest, db_construct_test)
{
	//�ļ������ڣ�Ӧ�׳��쳣
	EXPECT_ANY_THROW(Db db_not_exist("file"));
		 //file���ݿⲻ����
	//EXPECT_FALSE(db_not_exist());
	//�ٴ�ȷ�ϴ�����ctest.db
	Db::set_default_path("ctest.db", global::create_default_database);
	Db db_exist("ctest.db");
	EXPECT_TRUE(db_exist());

	Db db_empty;
	EXPECT_TRUE(db_empty());
}

//db�๹�캯��
TEST_F(dbTest, db_wchar_construct_test)
{
	//�ļ�������Ӧ�׳��쳣
	EXPECT_ANY_THROW(Db db_not_exist(L"file"));

	//�ٴ�ȷ�ϴ�����ctest.db
	Db::set_default_path(L"wtest.db", global::create_default_database);
	Db db_exist(L"wtest.db");
	EXPECT_TRUE(db_exist());

	Db db_empty;
	EXPECT_TRUE(db_empty());
}

//char������һλ"/0",wchar_t��������λ
TEST_F(dbTest, wchar_array_test)
{
	wchar_t first[] = L"���";
	EXPECT_EQ(6, sizeof(first)); //wchar����ĳ���Ϊ6��������Ϊ����0
}

struct Product
{
	int id;
	char title[21];
	wchar_t wtitle[11];
	int date;
	float price;
	int number;
	//image ��ʱ������
};
//db�๹�캯��
TEST_F(dbTest, db_insert_test)
{
	//�ٴ�ȷ�ϴ�����ctest.db
	Db::set_default_path("ctest.db", create_demo_database);
	Db conn;
	Query query = conn.create_query("SELECT COUNT(*) FROM PRODUCT");
	int count = 0;
	query.excute(count);
	EXPECT_EQ(3,count);
}

//��ȡһ�е���������
//ע�⸲����int��float��string�ȣ�����������һ�н���ĸ�������
TEST_F(dbTest, db_query_test)
{
	Db::set_default_path("ctest.db", create_demo_database);
	Db conn;
	Product product;
	Query row_cmd = conn.create_query("SELECT * FROM PRODUCT");
	//while (row_cmd.excute(product.id, product.title, product.wtitle, product.date, product.price, product.number))
	//{
	//	int i=product.id;
	//}
	char wtitle_ptr[11] = { 0 };
	row_cmd.excute(product.id, product.title, wtitle_ptr, product.date, product.price, product.number);

	//ע�⣬char[]���ĺ��֣�д�����ݿ⣬����char[]��ȷ�����������ݿ���ʹ����ͨ�����ߣ�Ϊ���룬�޹ؽ�Ҫ��
	EXPECT_STREQ("��һ��", wtitle_ptr);
}

//Db��Ӧ�ṩһ��ʼ�մ򿪵�����
TEST_F(dbTest, db_default_connection_)
{
	//auto conn = Db::GetDb(); //����Ĭ�ϵ�����
	//EXPECT_TRUE(conn());
}




