#include "mail_sender.cpp"

class TestableMailSender : public MailSender {
public:
	void sendMail(Schedule* schedule) override {
		countSendMailNethodIsCalled++;
	}

	int getCountSendMailMethodIsCalled() {
		return countSendMailNethodIsCalled;
	}

private:
	int countSendMailNethodIsCalled = 0;
};