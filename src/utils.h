#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <map>

namespace utils
{
	std::vector<std::string> splitString(std::string string, char delimeter);
	unsigned int countCharacters(std::string s, char c);
	int toInt(std::string text);
	std::string toString(int i);
	float toRads(float angle);
	float toDegrees(float angle);

	template <class T>
	unsigned short firstUnusedKey(std::map<unsigned short, T> map);

	std::string readFile(std::string filename);

	std::wstring toWstring(std::string text);
}
template <class T>
unsigned short utils::firstUnusedKey(std::map<unsigned short, T> map) {
	for (unsigned short i = 0; i < 65535; ++i) {
		if (map.count(i) == 0) {
			return i;
		}
	}

	return 65535;
}

#endif // UTILS_H
