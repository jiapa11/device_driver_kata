#include "gmock/gmock.h"
#include "device_driver.h"
#include "mock_flash_memory_device.cpp"

using namespace testing;

class DeviceDriverFixture : public testing::Test {
protected:
	void SetUp() override {
		
	}

public: 
	NiceMock<MockFlashMemoryDevice> hardware;
	DeviceDriver driver{&hardware};

	const int TARGET_READ_ADDRESS = 0xFF;
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

TEST_F(DeviceDriverFixture, Read_Called5times) {
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

TEST_F(DeviceDriverFixture, Write_General_ReadCalledAtLeastOnce) {
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
		driver.write(TARGET_READ_ADDRESS, 0xDEAD);
	}
	catch (const std::exception& e) {
		FAIL();
	}
}

TEST_F(DeviceDriverFixture, Write_Fail_TargetAddressNotDeleted) {
	EXPECT_CALL(hardware, read)
		.WillOnce(Return(0x12));

	EXPECT_THROW(driver.write(TARGET_READ_ADDRESS, 0xDEAD), WriteException);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
