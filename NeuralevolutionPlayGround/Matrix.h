#ifndef DEF_MATRIX
#define DEF_MATRIX

#include <vector>
#include <iostream>

namespace NN {

	class Matrix
	{
	public:
		Matrix();
		Matrix(int height, int width);
		Matrix(std::vector<std::vector<double> > const &array);
		Matrix(std::vector<std::vector<float> > const &array);
		Matrix(std::vector<std::vector<unsigned char>> const &array);

		double at(int i, int j) { return array[i][j]; }
		void set(double val, int i, int j);

		Matrix multiply(double const &value); // scalar multiplication

		Matrix add(Matrix const &m) const; // addition
		Matrix subtract(Matrix const &m) const; // subtraction
		Matrix multiply(Matrix const &m) const; // hadamard product

		Matrix dot(Matrix const &m) const; // dot product
		Matrix transpose() const; // transposed matrix

		Matrix applyFunction(double(*function)(double)) const; // to apply a function to every element of the matrix

		void print(std::ostream &flux) const; // pretty print of the matrix

		int getHeight() { return height; }
		int getWidth() { return width; }

	private:
		std::vector<std::vector<double> > array;
		int height;
		int width;
	};

}

std::ostream& operator<<(std::ostream &flux, NN::Matrix const &m); // overloading << operator to print easily



#endif

