#pragma once

#include <vector>

int randomInt(int exclusiveMax);
int randomInt(int min, int inclusiveMax);

std::size_t pickOne(std::vector<std::size_t>& vector);
void shuffle(std::vector<std::size_t>& vector);

int sign(int value);
