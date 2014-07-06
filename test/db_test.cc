
#include "gtest/gtest.h"
#include "db.h"

class dbTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		db::set_default("test.db");
		EXPECT_STREQ("test.db",db::default_.c_str());

		db::set_default(L"test.db");
		EXPECT_STREQ("test.db", db::default_.c_str());
		//���ڴ�ɾ��unlink()
	}

	virtual void TearDown()
	{
	}

	//db* db_;
};

//db��ľ�̬����set_default
TEST_F(dbTest, set_default_test)
{
	db::set_default("test.db");
	EXPECT_STREQ("test.db", db::default_.c_str());
}

//db��ľ�̬����set_defaultʹ��wstring
TEST_F(dbTest, set_default_wstring_test)
{
	db::set_default(L"test.db");
	EXPECT_STREQ("test.db", db::default_.c_str());
}
