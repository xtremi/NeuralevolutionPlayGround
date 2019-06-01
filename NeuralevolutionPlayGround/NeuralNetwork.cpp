#include "NeuralNetwork.h"


namespace NN{

void NNdata::init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate, int layers) {
	/*W1 = NN::Matrix(inputNeuron, hiddenNeuron);
	W2 = NN::Matrix(hiddenNeuron, outputNeuron);
	B1 = NN::Matrix(1, hiddenNeuron);
	B2 = NN::Matrix(1, outputNeuron);

	W1 = W1.applyFunction(NN::random);
	W2 = W2.applyFunction(NN::random);
	B1 = B1.applyFunction(NN::random);
	B2 = B2.applyFunction(NN::random);*/

	W1 = NN::Matrix(inputNeuron, hiddenNeuron);
	W2 = NN::Matrix(hiddenNeuron, hiddenNeuron);
	W3 = NN::Matrix(hiddenNeuron, outputNeuron);
	B1 = NN::Matrix(1, hiddenNeuron);
	B2 = NN::Matrix(1, hiddenNeuron);
	B3 = NN::Matrix(1, outputNeuron);

	W1 = W1.applyFunction(NN::random);
	W2 = W2.applyFunction(NN::random);
	W3 = W3.applyFunction(NN::random);
	B1 = B1.applyFunction(NN::random);
	B2 = B2.applyFunction(NN::random);
	B3 = B3.applyFunction(NN::random);

	learningRate = rate;

	std::stringstream ss;
	W1.print(ss);
	std::string str;
	ss >> str;
}

void NNdata::computeOutput(const std::vector<float>& input) {
	X = NN::Matrix({ input });
	/*H1 = X.dot(W1).add(B1).applyFunction(sigmoid);
	Y = H1.dot(W2).add(B2).applyFunction(sigmoid);*/
	
	H1 = X.dot(W1).add(B1).applyFunction(sigmoid);
	H2 = H1.dot(W2).add(B2).applyFunction(sigmoid);
	Y = H2.dot(W3).add(B3).applyFunction(sigmoid);
}


void NNdata::crossover1(NNdata* parent1, NNdata* parent2, float coef) {
	W1 = parent1->W1.multiply(coef).add(parent2->W1.multiply(1.0f - coef));
	W2 = parent1->W2.multiply(coef).add(parent2->W2.multiply(1.0f - coef));
	W3 = parent1->W3.multiply(coef).add(parent2->W3.multiply(1.0f - coef));
	B1 = parent1->B1.multiply(coef).add(parent2->B1.multiply(1.0f - coef));
	B2 = parent1->B2.multiply(coef).add(parent2->B2.multiply(1.0f - coef));
	B3 = parent1->B3.multiply(coef).add(parent2->B3.multiply(1.0f - coef));
}

void NNdata::crossover2(NNdata* parent1, NNdata* parent2, float coef) {
	
	/*Matrix* p1mats[4] = { &parent1->W1, &parent1->W2, &parent1->B1, &parent1->B2 };
	Matrix* p2mats[4] = { &parent2->W1, &parent2->W2, &parent2->B1, &parent2->B2 };
	Matrix* cmats[4] = { &W1, &W2, &B1, &B2 };*/

	Matrix* p1mats[6] = { &parent1->W1, &parent1->W2,&parent1->W3, &parent1->B1, &parent1->B2, &parent1->B3};
	Matrix* p2mats[6] = { &parent2->W1, &parent2->W2, &parent2->W3, &parent2->B1, &parent2->B2, &parent2->B3};
	Matrix* cmats[6]  = { &W1, &W2, &W3, &B1, &B2, &B3 }; 

	for (int i = 0; i < 6; i++) {

		for(int r = 0; r < cmats[i]->getHeight(); r++){
			for (int c = 0; c < cmats[i]->getWidth(); c++){

				float rv = NN::random2();

				if (rv > 0.667f) {
					cmats[i]->set(0.5f*p1mats[i]->at(r, c) + 0.5f*p2mats[i]->at(r, c), r, c);
				}
				else if (rv > 0.33f) {
					cmats[i]->set(p1mats[i]->at(r, c), r, c);
				}
				else {
					cmats[i]->set(p2mats[i]->at(r, c), r, c);
				}
			}

		}
	}
}

void NNdata::mutate(float threshold) {

	Matrix* cmats[6] = { &W1, &W2, &W3, &B1, &B2, &B3 }; 

	float randomVal = NN::random2();
	//randomVal = 0.0f;
	if (randomVal < threshold) {

		int randomIndex = NN::randomFromTo(0, 5);
		
		//float randFactor = 1.0f + 0.3f*NN::random();
		float randIncr = random();
		int i, j;

		getRandomMatrixIndices(cmats[randomIndex], i, j);
		//cmats[randomIndex]->set(cmats[randomIndex]->at(i, j) * randFactor, i, j);
		cmats[randomIndex]->set(cmats[randomIndex]->at(i, j) + randIncr, i, j);	
	}

}




void getRandomMatrixIndices(Matrix* m, int& i, int& j) {
	i = randomFromTo(0, m->getHeight() - 1);
	j = randomFromTo(0, m->getWidth() - 1);
}

double random(double x){
	return random2() - 0.5;
}
double random2(double x) {
	return (double)(rand() % 10000 + 1) / 10000;
}

int randomFromTo(int from, int to) {
	return rand() % (to - from + 1) + from;
}

double sigmoid(double x)
{
	return 1 / (1 + std::exp(-x));
}

double sigmoidePrime(double x)
{
	return std::exp(-x) / (std::pow(1 + std::exp(-x), 2));
}

double stepFunction(double x)
{
	if (x > 0.9) {
		return 1.0;
	}
	if (x < 0.1) {
		return 0.0;
	}
	return x;
}

}