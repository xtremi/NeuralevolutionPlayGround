#include "ProjectIO.h"


bool ProjectIO::read(const std::string& filepath, population_input& popInput, 
	std::string& mapfile, std::string& nnfile) 
{
	std::string type, arg1, arg2, arg3;

	if (!openRead(filepath))
		return false;

	while (!file_i.eof()) {
		file_i >> type >> arg1;
		if (type == "PSIZE") {
			popInput.size = std::stoi(arg1);
		}
		else if (type == "PSTART" || type == "PDIR") {
			file_i >> arg2;
			glm::vec2 tmpvec(std::stof(arg1), std::stof(arg2));
			if (type == "PSTART"){
				popInput.startpos = tmpvec;
			}
			else{
				popInput.startdir = tmpvec;
			}
		}
		else if (type == "MAP") {
			mapfile = arg1;
		}
		else if (type == "NN") {
			nnfile = arg1;
		}
	}

	file_i.close();
	return true;
}


bool ProjectIO::write(const std::string& filepath, const population_input& popinput, 
	const std::string& mapfile, const std::string& nnfile) 
{
	if (!openWrite(filepath))
		return false;

	file_o << "VERSION " << "1.0.0" << std::endl << std::endl;
	file_o << "PSIZE  " << popinput.size << std::endl;
	file_o << "PSTART " << TOSTR(popinput.startpos.x, 3) << " " << TOSTR(popinput.startpos.y, 3) << std::endl;
	file_o << "PDIR   " << TOSTR(popinput.startdir.x, 3) << " " << TOSTR(popinput.startdir.y, 3) << std::endl;
	file_o << "MAP    " << mapfile << std::endl;
	file_o << "NN     " << nnfile << std::endl;

	file_o.close();
	return true;
}


