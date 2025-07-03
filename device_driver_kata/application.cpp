#include <iostream>
#include <exception>
#include <vector>
#include "device_driver.h"

class Application {
public:
    Application(DeviceDriver* driver) : m_driver(driver) {};

    void readAndPrint(long startAddr, long endAddr) {
        if (startAddr < 0 || endAddr < 0) throw std::exception("[ERROR] invalid address; must be bigger than 0");
        if (endAddr < startAddr) throw std::exception("[ERROR] end addr is lower than start addr");

        std::vector<std::pair<long, int>> readResult{};
        for (long addr = startAddr; addr <= endAddr; addr++) {
            try {
                int readVal = m_driver->read(addr);
                readResult.push_back(std::make_pair(addr, readVal));
            }
            catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
            
        }
        
        for (auto result : readResult) {
            std::cout << "[Addr 0x" << std::hex << result.first << "] ";
            std::cout << "0x" << std::hex << result.second << ", ";
        }
        std::cout << std::endl;
    }

    void writeAll(int value) {
        for (long addr = 0x0; addr <= 0x4; addr++) {
            m_driver->write(addr, value);
        }
    }
private:
    DeviceDriver* m_driver;
};