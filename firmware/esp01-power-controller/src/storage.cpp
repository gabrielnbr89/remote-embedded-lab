#include <Arduino.h>
#include <EEPROM.h>

#include "storage.h"

namespace
{
    constexpr uint16_t EEPROM_SIZE = 16;
    constexpr uint16_t BOOT_COUNT_ADDRESS = 0;
}

void storage_init()
{
    EEPROM.begin(EEPROM_SIZE);
}

uint32_t storage_get_boot_count()
{
    uint32_t bootCount = 0;

    EEPROM.get(BOOT_COUNT_ADDRESS, bootCount);

    ++bootCount;

    EEPROM.put(BOOT_COUNT_ADDRESS, bootCount);
    EEPROM.commit();

    return bootCount;
}