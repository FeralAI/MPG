#include <string.h>
#include <GamepadStorage.h>
#include <EEPROM.h>

void GamepadStorage::get(int index, void *data, uint16_t size)
{
	uint8_t buffer[size] = { };
	for (int i = 0; i < size; i++)
		EEPROM.get(index + i, buffer[i]);

	memcpy(data, buffer, size);
}

void GamepadStorage::set(int index, void *data, uint16_t size)
{
	uint8_t buffer[size] = { };
	memcpy(buffer, data, size);
	for (int i = 0; i < size; i++)
		EEPROM.put(index + i, buffer[i]);
}

void GamepadStorage::start() { }
void GamepadStorage::save() { }
