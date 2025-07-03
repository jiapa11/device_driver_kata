#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int readValue = read_(address);
    if (false == verifyRead(readValue, address)) {
        throw ReadException();
    }
    return readValue;
}

int DeviceDriver::read_(long address)
{
    return (int)(m_hardware->read(address));
}

bool DeviceDriver::verifyRead(int readValue, long address)
{
    for (int i = 0; i < 4; i++) {
        if (readValue != read_(address)) {
            return false;
        }
    }
    return true;
}

void DeviceDriver::write(long address, int data)
{
    int readValue = read_(address);
    if (FlashMemoryDevice::DELETED == readValue)
    {
        write_(address, data);
    }
    else {
        //throw std::exception("writing to not-deleted area");
        throw WriteException();
    }
}

void DeviceDriver::write_(long address, int data)
{
    m_hardware->write(address, (unsigned char)data);
}

void DeviceDriver::setDevice(FlashMemoryDevice& hardware) 
{
    m_hardware = &hardware;
}