#include <UnitTest++/UnitTest++.h>
#include <../Client/Client.h>
#include <../Client/Client.cpp>

SUITE(get_sha256)
{
	TEST(Test1) {
		CHECK_EQUAL("6788DB5B9A5C5BA379A20B43EFC505B78E26F1208A21274FFE60EF55F18F3A06", CheckClient().get_sha256("1/1AV9+R"));
	}
	TEST(Test2) {
		CHECK_THROW(CheckClient().get_sha256(""), CCError);
	}
	TEST(Test3) {
		CHECK_THROW(CheckClient().get_sha256("1/5ФД"), CCError);
	}
}

struct CheckClient_fixture {
	CheckClient * object;
	CheckClient_fixture() {
		object = new CheckClient();
	}
	~CheckClient_fixture() {
		delete object;
	}
};

SUITE(GenerateTest) {
	TEST_FIXTURE(CheckClient_fixture, Test1){
		object->check_license("1", "123");
		CHECK(true);
	}
	TEST(Test0) {
		CHECK_EQUAL(true, CheckClient().check_license("1", "123"));
	}
	TEST_FIXTURE(CheckClient_fixture, Test2){
		CHECK_THROW(object->check_license("F", "123"), CCError);
	}
	TEST_FIXTURE(CheckClient_fixture, Test3){
		CHECK_THROW(object->check_license("+", "123"), CCError);
	}
	TEST_FIXTURE(CheckClient_fixture, Test4){
		CHECK_THROW(object->check_license("0", "123"), CCError);
	}
	TEST_FIXTURE(CheckClient_fixture, Test5){
		CHECK_THROW(object->check_license("-5", "123"), CCError);
	}
	TEST_FIXTURE(CheckClient_fixture, Test6){
		CHECK_THROW(object->check_license("Ф", "123"), CCError);
	}
	TEST_FIXTURE(CheckClient_fixture, Test7){
		CHECK_THROW(object->check_license("1", ""), CCError);
	}
	TEST_FIXTURE(CheckClient_fixture, Test8){
		CHECK_THROW(object->check_license("1", "ФФФ"), CCError);
	}
	TEST_FIXTURE(CheckClient_fixture, Test9){
		CHECK_THROW(object->check_license("1", "1"), CCError);
	}
	
}

int main(int argc, char **argv) {
	return UnitTest::RunAllTests();
}