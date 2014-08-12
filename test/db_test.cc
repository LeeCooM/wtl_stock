
#include "gtest/gtest.h"
#include "db.h"
#include "global.h"
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

////db类的静态函数set_default
TEST_F(dbTest, set_default_test)
{
	auto rc = DbConnection::set_default("ctest.db",global::create_default_database);
	EXPECT_STREQ("ctest.db", DbConnection::get_default());
}

//db类的静态函数set_default使用wstring
TEST_F(dbTest, set_default_wstring_test)
{
	auto rc = DbConnection::set_default(L"wtest.db",global::create_default_database);
	EXPECT_STREQ("wtest.db", DbConnection::get_default());
}

//db类构造函数
TEST_F(dbTest, db_construct_test)
{
	DbConnection db_not_exist("file"); //file数据库不存在
	EXPECT_FALSE(db_not_exist());

	//再次确认创建了ctest.db
	DbConnection::set_default("ctest.db", global::create_default_database);
	DbConnection db_exist("ctest.db");
	EXPECT_TRUE(db_exist());

	DbConnection db_empty;
	EXPECT_TRUE(db_empty());
}

//db类构造函数
TEST_F(dbTest, db_wchar_construct_test)
{
	DbConnection db_not_exist(L"file"); //file数据库不存在
	EXPECT_FALSE(db_not_exist());

	//再次确认创建了ctest.db
	DbConnection::set_default(L"wtest.db", global::create_default_database);
	DbConnection db_exist(L"wtest.db");
	EXPECT_TRUE(db_exist());

	DbConnection db_empty;
	EXPECT_TRUE(db_empty());
}