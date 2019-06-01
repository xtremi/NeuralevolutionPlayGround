#pragma once
#include <iostream>
#include <fstream>
#include <string>


class FileIO {
public:
	FileIO(){}

protected:
	std::ifstream file_i;
	std::ofstream file_o;
	std::string errmsg = "";

	bool openWrite(const std::string& filepath);
	bool openRead(const std::string& filepath);

};