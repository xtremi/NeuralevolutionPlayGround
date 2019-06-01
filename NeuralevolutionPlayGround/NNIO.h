#pragma once
#include "FileIO.h"
#include <vector>
#include <map>
#include "NeuralNetwork.h"

class NNIO : public FileIO {
public:
	NNIO() : FileIO(){}

	struct NN_header {
		int nNNs = 0, nlayers = 0, nIneurons = 0, nOneurons = 0, nHneurons = 0;
		int generation;
		std::string version;
	};

	bool read(const std::string& filepath, std::vector<NN::NNdata>* nndatas, int& genN);
	bool write(const std::string& filepath, std::vector<NN::NNdata*>* nndatas, int genN);

private:
	bool readNNFileHeader(std::ifstream& file, NN_header& header);
	bool readNNmatrices(std::ifstream& file, const NN_header& header, NN::NNdata* nndata);
	bool readNNmatrix(std::ifstream& file, int height, int width, NN::Matrix* mat);
};