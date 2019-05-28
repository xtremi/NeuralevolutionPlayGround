#include "convinient_functions.h"

float randfFromTo(float from, float to) {
	return (float)(rand() % 10000 + 1) / 10000.0;
}

glm::vec3 randVec3FromTo(float from, float to) {
	return glm::vec3(randfFromTo(from, to), randfFromTo(from, to), randfFromTo(from, to));
}

void agkPrintVec2(const std::string& prefix, const glm::vec2& v) {
	std::string str = prefix;
	str += (TOSTR(v.x, 2) + ", " + TOSTR(v.y, 2));
	agk::Print(&str[0]);
}