#include "gmock/gmock.h"
#include "device_driver.h"
#include "mock_flash_memory_device.cpp"
#include "application.cpp"

using namespace testing;

class DeviceDriverFixture : public testing::Test {
protected:
	void SetUp() override {
		
	}

public: 
	NiceMock<MockFlashMemoryDevice> hardware;
	DeviceDriver driver{&hardware};

	const int TARGET_READ_ADDRESS = 0xFF;
	const int TARGET_WRITE_ADDRESS = TARGET_READ_ADDRESS;
	const int WRITE_VALUE = 0xDE;
};

TEST_F(DeviceDriverFixture, Read_Success_ReturnCorrectValue) {
	EXPECT_CALL(hardware, read)
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA));
	
	int data = driver.read(TARGET_READ_ADDRESS);

	EXPECT_EQ(0xAA, data);
}

TEST_F(DeviceDriverFixture, Read_General_Called5times) {
	EXPECT_CALL(hardware, read)
		.Times(5);

	int data = driver.read(TARGET_READ_ADDRESS);
}

TEST_F(DeviceDriverFixture, Read_Fail_RaiseException) {
	EXPECT_CALL(hardware, read)
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAA))
		.WillOnce(Return(0xAD));

	EXPECT_THROW(driver.read(TARGET_READ_ADDRESS), ReadException);
}

TEST_F(DeviceDriverFixture, Write_General_InternalReadCalledAtLeastOnce) {
	EXPECT_CALL(hardware, read)
		.Times(1);
	
	try {
		driver.write(TARGET_READ_ADDRESS, 0xDEAD);
	}
	catch (const std::exception& e) {

	}
}

TEST_F(DeviceDriverFixture, Write_Success) {
	EXPECT_CALL(hardware, read)
		.WillOnce(Return(DeviceDriver::DELETED));

	try {
		driver.write(TARGET_WRITE_ADDRESS, WRITE_VALUE);
	}
	catch (const std::exception& e) {
		FAIL();
	}
}

TEST_F(DeviceDriverFixture, Write_Fail_TargetAddressNotDeleted) {
	EXPECT_CALL(hardware, read)
		.WillOnce(Return(0x12));

	EXPECT_THROW(driver.write(TARGET_WRITE_ADDRESS, WRITE_VALUE), WriteException);
}

class ApplicationFixture : public DeviceDriverFixture {
public:
	Application application{&driver};
};

TEST_F(ApplicationFixture, application_read_success) {
	EXPECT_CALL(hardware, read)
		.Times(9 * 5)
		.WillRepeatedly(Return(0x28));
		
	application.readAndPrint(0, 8);
}

TEST_F(ApplicationFixture, application_write_success) {
	EXPECT_CALL(hardware, read)
		.WillRepeatedly(Return(DeviceDriver::DELETED));
	application.writeAll(0xAB);
}

TEST_F(ApplicationFixture, application_write_fail_inbetween) {
	EXPECT_CALL(hardware, read)
		.WillOnce(Return(DeviceDriver::DELETED))
		.WillOnce(Return(DeviceDriver::DELETED))
		.WillOnce(Return(0x65))
		.WillRepeatedly(Return(DeviceDriver::DELETED));

	EXPECT_THROW(application.writeAll(0xAB), WriteException);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
