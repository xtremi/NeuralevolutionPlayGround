#pragma once
#include <stdlib.h>
#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include "agk.h"

#define TOSTR(f,p) std::string(agk::Str(f,p))

void agkPrintVec2(const std::string& prefix, const glm::vec2& v);
template<typename T>
void agkPrint(const std::string& prefix, T f) {
	std::string str = prefix + TOSTR(f, 2);
	agk::Print(&str[0]);
}

float randfFromTo(float from, float to);
glm::vec3 randVec3FromTo(float from, float to);