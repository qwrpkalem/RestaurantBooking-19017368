#include "gmock/gmock.h"
#include "booking_scheduler.cpp"

using namespace testing;

class BookingItem : public Test {
public:
	Customer customer{ "Fake me", "010-1234-5678" };
	tm notOntheHour;
	tm onTheHour;
	void SetUp() override {
		notOntheHour = getTime(2021, 3, 26, 9, 5);
		onTheHour = getTime(2021, 3, 26, 9, 0);
	}
	

	tm getTime(int year, int mon, int day, int hour, int min) {
		tm result = { 0,min, hour, day, mon - 1, year - 1900, 0, 0, -1 };
		mktime(&result);
		return result;
	}
};

TEST_F(BookingItem, ���������ÿ��������ϴ����ð��ƴѰ�쿹��Ұ�) {
	Schedule* schedule = new Schedule{ notOntheHour , 1, customer };
	BookingScheduler bookingScheduler{ 3 };

	//act
	EXPECT_THROW({
		bookingScheduler.addSchedule(schedule);
		}, std::runtime_error);
}

TEST_F(BookingItem, ���������ÿ��������ϴ������ΰ�쿹�డ��) {
	Schedule* schedule = new Schedule{ onTheHour , 1, customer };
	BookingScheduler bookingScheduler{ 3 };

	//act
	bookingScheduler.addSchedule(schedule);

	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
}

TEST(BookingSchedulerTest, �ð��뺰�ο��������ִٰ����ð��뿡Capacity�ʰ��Ұ�쿹�ܹ߻�) {

}

TEST(BookingSchedulerTest, �ð��뺰�ο��������ִٰ����ð��밡�ٸ���Capacity���־�������߰�����) {

}

TEST(BookingSchedulerTest, ����Ϸ��SMS�¹����ǹ߼�) {

}

TEST(BookingSchedulerTest, �̸����̾��°�쿡���̸��Ϲ̹߼�) {

}

TEST(BookingSchedulerTest, �̸������ִ°�쿡���̸��Ϲ߼�) {

}

TEST(BookingSchedulerTest, ���糯¥���Ͽ����ΰ�쿹��Ұ�����ó��) {

}

TEST(BookingSchedulerTest, ���糯¥���Ͽ����̾ƴѰ�쿹�డ��) {

}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}