#include <vector>
#include <string>
#include <sstream>
#include "utils.h"

#define PI 3.14159265

float utils::toRads(float angle)
{
	return angle*PI/180;
}

float utils::toDegrees(float angle)
{
	return angle/PI*180;
}

std::vector<std::string> utils::splitString(std::string string, char delimeter)
{
	std::vector<std::string> items;
	std::string item;
	std::stringstream stream(string);

	while (std::getline(stream, item, delimeter))
	{
		items.push_back(item);
	}

	return items;
}

unsigned int utils::countCharacters(std::string s, char c)
{
	int count = 0;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] == c)
		{
			count++;
		}
	}
	return count;
}

int utils::toInt(std::string text) {
	std::istringstream stream(text);
	int i;
	stream >> i;
	return i;
}

std::string utils::toString(int i)
{
	std::ostringstream stream;
	stream << i;
	return stream.str();
}