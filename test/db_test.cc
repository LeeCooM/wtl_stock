
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
	}

	virtual void TearDown()
	{
		
	}
};

////db��ľ�̬����set_default
TEST_F(dbTest, set_default_test)
{
	auto rc = DbConnection::set_default("ctest.db",create_demo_database);
	EXPECT_STREQ("ctest.db", DbConnection::get_default());
}

//db��ľ�̬����set_defaultʹ��wstring
TEST_F(dbTest, set_default_wstring_test)
{
	auto rc = DbConnection::set_default(L"wtest.db",create_demo_database);
	EXPECT_STREQ("wtest.db", DbConnection::get_default());
}

//db�๹�캯��
TEST_F(dbTest, db_construct_test)
{
	//�ļ������ڣ�Ӧ�׳��쳣
	EXPECT_ANY_THROW(DbConnection db_not_exist("file"));
		 //file���ݿⲻ����
	//EXPECT_FALSE(db_not_exist());
	//�ٴ�ȷ�ϴ�����ctest.db
	DbConnection::set_default("ctest.db", global::create_default_database);
	DbConnection db_exist("ctest.db");
	EXPECT_TRUE(db_exist());

	DbConnection db_empty;
	EXPECT_TRUE(db_empty());
}

//db�๹�캯��
TEST_F(dbTest, db_wchar_construct_test)
{
	//�ļ�������Ӧ�׳��쳣
	EXPECT_ANY_THROW(DbConnection db_not_exist(L"file"));

	//�ٴ�ȷ�ϴ�����ctest.db
	DbConnection::set_default(L"wtest.db", global::create_default_database);
	DbConnection db_exist(L"wtest.db");
	EXPECT_TRUE(db_exist());

	DbConnection db_empty;
	EXPECT_TRUE(db_empty());
}

//char������һλ"/0",wchar_t��������λ
TEST_F(dbTest, wchar_array_test)
{
	wchar_t first[] = L"���";
	EXPECT_EQ(6, sizeof(first)); //wchar����ĳ���Ϊ6��������Ϊ����0
}

//db�๹�캯��
TEST_F(dbTest, db_insert_test)
{
	//�ٴ�ȷ�ϴ�����ctest.db
	DbConnection::set_default("ctest.db", create_demo_database);

	DbConnection conn;

	//����6���ֶΣ����ֻ��д5���ֶλᴥ���쳣
	DbCommand cmd=conn.get_command( "INSERT INTO PRODUCT VALUES (?,?,?,?,?,?,?)");

	//��2��ʼbind����Ϊ��һ�����������ֶ�
	//ֻbind 4�����������һ��blob�ֶ�û��bind
	cmd.bind(2,"first", "��һ��",(unsigned long long)1402876800,(double)100.10,(int)10);
	cmd.ExecuteNonQuery();

	cmd.bind(2, "second",L"�ڶ���", (int)1402876800, (float)200.20, (int)20);
	cmd.ExecuteNonQuery();

	std::string first_string="third";
	std::wstring first_wstring = L"������";
	cmd.bind(2, first_string, first_wstring,(int)1402876800, (float)300.20, (int)30);
	cmd.ExecuteNonQuery();

	DbCommand query = conn.get_command("SELECT COUNT(*) FROM PRODUCT");
	int count = 0;
	query.Execute(count);
	EXPECT_EQ(3,count);
}