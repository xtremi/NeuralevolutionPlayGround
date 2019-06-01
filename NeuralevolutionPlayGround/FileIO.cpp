#include "FileIO.h"

bool FileIO::openWrite(const std::string& filepath) {
	file_o.open(filepath);
	if (!file_o.is_open()) {
		errmsg = "Could not open " + filepath + " for writing.";
		return false;
	}
	return true;
}
bool FileIO::openRead(const std::string& filepath) {
	file_i.open(filepath);
	if (!file_i.is_open()) {
		errmsg = "Could not open " + filepath + " for reading.";
		return false;
	}
	return true;
}
