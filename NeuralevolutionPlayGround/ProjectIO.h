#pragma once
#include "FileIO.h"
#include "Population.h"

class ProjectIO : public FileIO {
public:
	ProjectIO() : FileIO() {}

	bool read(const std::string& filepath, population_input& popinput, 
		std::string& mapfile, std::string& nnfile);
	bool write(const std::string& filepath, const population_input& popinput, 
		const std::string& mapfile, const std::string& nnfile);
};



