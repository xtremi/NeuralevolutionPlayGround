#include "NNIO.h"


bool NNIO::write(const std::string& filepath, std::vector<NN::NNdata*>* nndatas, int genN) {

	if (!openWrite(filepath))
		return false;

	file_o << "VERSION    " << "1.0" << std::endl;
	file_o << "GENERATION " << genN << std::endl;
	file_o << "SIZE       " << nndatas->size() << std::endl;
	file_o << "LAYERS     " << 2 << std::endl;
	file_o << "INEURONS   " << (*nndatas)[0]->W1.getHeight() << std::endl;
	file_o << "ONEURONS   " << (*nndatas)[0]->W3.getWidth() << std::endl;
	file_o << "HNEURONS   " << (*nndatas)[0]->W2.getWidth() << std::endl;

	int c = 0;
	for (NN::NNdata* nndata : (*nndatas)) {
		file_o << "NNDATA    " << c++ << std::endl;
		file_o << "W1\n";	nndata->W1.print(file_o);
		file_o << "W2\n";	nndata->W2.print(file_o);
		file_o << "W3\n";	nndata->W3.print(file_o);
		file_o << "B1\n";	nndata->B1.print(file_o);
		file_o << "B2\n";	nndata->B2.print(file_o);
		file_o << "B3\n";	nndata->B3.print(file_o);
	}

	file_o.close();
	return true;
}

bool NNIO::read(const std::string& filepath, std::vector<NN::NNdata>* nndatas, int& genN) {
	
	if (!openRead(filepath))
		return false;

	NN_header header;
	if (!readNNFileHeader(file_i, header))
		return false;

	genN = header.generation;

	for (int i = 0; i < header.nNNs; i++) {
		NN::NNdata nndata;
		if (!readNNmatrices(file_i, header, &nndata))
			return false;
		nndatas->push_back(nndata);
	}

	file_i.close();
	return true;
}

bool NNIO::readNNFileHeader(std::ifstream& file, NN_header& header) {

	std::string key;
	while (!file.eof()) {

		file >> key;
		std::string argstr;
		file >> argstr;
		int val;

		if (key != "VERSION")
			val = std::stoi(argstr);
		else
			header.version = key;

		if (key == "SIZE")
			header.nNNs = val;
		else if (key == "SIZE")
			header.nNNs = val;
		else if (key == "LAYERS")
			header.nlayers = val;
		else if (key == "INEURONS")
			header.nIneurons = val;
		else if (key == "ONEURONS")
			header.nOneurons = val;
		else if (key == "HNEURONS")
			header.nHneurons = val;

		if (header.nNNs && header.nlayers && header.nIneurons && header.nHneurons && header.nOneurons)
			return true;
	}
	return false;
}

bool NNIO::readNNmatrices(std::ifstream& file, const NN_header& header, NN::NNdata* nndata) {
	static std::map<std::string, int> keyMap = {
		{"W1", 0},{"W2", 1},{"W3", 2},{"B1", 3},{"B2", 4},{"B3", 5}
	};

	int matHeights[6] = { header.nIneurons, header.nHneurons, header.nHneurons, 1,1,1 };
	int matWidth[6] = { header.nHneurons, header.nHneurons, header.nOneurons, header.nHneurons,header.nHneurons,header.nOneurons };
	bool matsFound[6] = { false, false, false, false, false, false };
	NN::Matrix* matrices[6] = { &nndata->W1, &nndata->W2, &nndata->W3, &nndata->B1, &nndata->B2, &nndata->B3 };

	std::string key;
	while (!file.eof()) {
		file >> key;
		if (keyMap.find(key) != keyMap.end()) {
			int index = keyMap[key];

			int h = matHeights[index];
			int w = matWidth[index];
			matsFound[index] = true;

			if (!readNNmatrix(file, h, w, matrices[index]))
				return false;
		}

		bool allFound = true;
		for (int i = 0; i < 6; i++) {
			if (!matsFound[i]) {
				allFound = false;
				break;
			}
		}
		if (allFound) return true;

	}
	return false;
}
bool NNIO::readNNmatrix(std::ifstream& file, int height, int width, NN::Matrix* mat) {
	std::vector<std::vector<double>> data;
	std::string vstr;
	double v;
	for (int i = 0; i < height; i++) {
		std::vector<double> row;
		for (int j = 0; j < width; j++) {
			if (file.eof()) {
				file.close();
				return false;
			}
			file >> vstr;
			v = std::stod(vstr);
			row.push_back(v);
		}
		data.push_back(row);
	}

	(*mat) = NN::Matrix(data);
	return true;
}

