#pragma once
#include "flash_memory_device.h"
#include <exception>

class ReadException : public std::exception {
public:
    char const* what() const override {
        return READ_EXCEPTION_MSG;
    }
    char const* READ_EXCEPTION_MSG = "Read verification failed; must show consistent values for five attempts";
};

class WriteException : public std::exception {
public:
    char const* what() const override {
        return WRITE_EXCEPTION_MSG;
    }
    char const* WRITE_EXCEPTION_MSG = "Attempting to write to address with existing values; failing write";
};

class DeviceDriver
{
public:
    DeviceDriver() = default;
    DeviceDriver(FlashMemoryDevice *hardware);
    void setDevice(FlashMemoryDevice& hardware);
    int read(long address);
    bool verifyRead(int readValue, long address);
    void write(long address, int data);

    bool isTargetAddressDeleted(long address);
    
    static const int DELETED = FlashMemoryDevice::DELETED;
protected:
    FlashMemoryDevice *m_hardware;

    int read_(long address);
    void write_(long address, int data);
};
