#include "gmock/gmock.h"
#include "booking_scheduler.cpp"
#include "testable_sms_sender.cpp"
#include "testable_mail_sender.cpp"

using namespace testing;

class BookingItem : public Test {
public:
	Customer CUSTOMER{ "Fake me", "010-1234-5678" };
	tm NOT_ON_THE_HOUR;
	tm ON_THE_HOUR;
	const int UNDER_CAPACITY = 1;
	const int CAPACITY_PER_HOUR = 3;
	BookingScheduler bookingScheduler{ CAPACITY_PER_HOUR };
	TestableSmsSender testableSmsSender;

	void SetUp() override {
		NOT_ON_THE_HOUR = getTime(2021, 3, 26, 9, 5);
		ON_THE_HOUR = getTime(2021, 3, 26, 9, 0);

		bookingScheduler.setSmsSender(&testableSmsSender);
	}
	

	tm getTime(int year, int mon, int day, int hour, int min) {
		tm result = { 0,min, hour, day, mon - 1, year - 1900, 0, 0, -1 };
		mktime(&result);
		return result;
	}

	tm plusHour(tm base, int hour) {
		base.tm_hour += hour;
		mktime(&base);
		return base;
	}
};

TEST_F(BookingItem, 예약은정시에만가능하다정시가아닌경우예약불가) {
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR , UNDER_CAPACITY, CUSTOMER };

	//act
	EXPECT_THROW({
		bookingScheduler.addSchedule(schedule);
		}, std::runtime_error);
}

TEST_F(BookingItem, 예약은정시에만가능하다정시인경우예약가능) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR , UNDER_CAPACITY, CUSTOMER };

	//act
	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
}

TEST_F(BookingItem, 시간대별인원제한이있다같은시간대에Capacity초과할경우예외발생) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR , CAPACITY_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);

	// act
	try {
		Schedule* newSchedule = new Schedule{ ON_THE_HOUR , UNDER_CAPACITY, CUSTOMER };
		bookingScheduler.addSchedule(newSchedule);
		FAIL();
	}
	catch (std::runtime_error& e) {
		// assert
		EXPECT_EQ(string{ e.what() }, string{ "Number of people is over restaurant capacity per hour" });

	}
}

TEST_F(BookingItem, 시간대별인원제한이있다같은시간대가다르면Capacity차있어도스케쥴추가성공) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR , CAPACITY_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);

	tm differentHour = plusHour(ON_THE_HOUR, 1);

	Schedule* newSchedule = new Schedule{ differentHour, UNDER_CAPACITY, CUSTOMER };
	bookingScheduler.addSchedule(newSchedule);

	//assert
	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));


}

TEST_F(BookingItem, 예약완료시SMS는무조건발송) {
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(true, testableSmsSender.isSendMethodIsCalled());
}

TEST_F(BookingItem, 이메일이없는경우에는이메일미발송) {
	TestableMailSender testableMailSender;
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
	bookingScheduler.setMailSender(&testableMailSender);

	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(0, testableMailSender.getCountSendMailMethodIsCalled());
}

TEST_F(BookingItem, 이메일이있는경우에는이메일발송) {
	Customer customerWithMail{ "Fake name", "010-1234-5678", "test@test.com" };
	TestableMailSender testableMailSender;
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, customerWithMail };
	bookingScheduler.setMailSender(&testableMailSender);

	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(1, testableMailSender.getCountSendMailMethodIsCalled());
}

TEST(BookingSchedulerTest, 현재날짜가일요일인경우예약불가예외처리) {

}

TEST(BookingSchedulerTest, 현재날짜가일요일이아닌경우예약가능) {

}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}