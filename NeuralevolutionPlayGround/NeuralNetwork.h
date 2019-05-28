#pragma once
#include <cmath>
#include "Matrix.h"
#include <sstream>

namespace NN{

class NNdata {

public:
	NNdata(){};
	void init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate, int layers = 1);	
	void computeOutput(const std::vector<float>& input);
	void crossover1(NNdata* parent1, NNdata* parent2, float coef);
	void crossover2(NNdata* parent1, NNdata* parent2, float coef);
	void mutate(float threshold);

	NN::Matrix X, W1, H1, W2, Y, B1, B2, Y2, dJdB1, dJdB2, dJdW1, dJdW2, W3, H2, B3;
	double learningRate;
};


void getRandomMatrixIndices(Matrix* m, int& i, int& j);

int randomFromTo(int from, int to);
double random2(double x = 0.0);
double random(double x = 0.0);
double sigmoid(double x);
double sigmoidePrime(double x);
double stepFunction(double x);

}