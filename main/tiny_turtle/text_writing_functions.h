#pragma once

#include <cstdint>
#include <string>

void plotText(const std::string& str, int scale);
void plotChar(uint8_t character, float scale);
int ASCIItoIndex(uint8_t c);
