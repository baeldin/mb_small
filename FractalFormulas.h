#pragma once
#include <stdio.h>
#include "FractalParameters.h"

/* abstract base class for a fractal formula
   this needs the following ingredients:
   1. parameters of the fractal
   2. initialization (init part in UF) to set initial values
   3. all calculations to iterate
   4. a bailout check (could be independent, but doesn't have to be)*/
class abstractBaseFractal
{
public:
	fractalParameters params; // holds parameters
	abstractBaseFractal() {}
	~abstractBaseFractal() {}
	virtual fractalParameters getParams() const = 0;
	virtual complex start(complex z, const complex z0) = 0; // init ini UF
	virtual complex iterate(complex z, const complex z0) = 0; // loop in UF
	virtual bool bailoutCheck(const complex z, const int iter) const = 0;
};


// FRACTAL FORMULAS AS DERIVED CLASSES 

// Mandelbrot Set
class MandelbrotSet : public abstractBaseFractal
{
public:
	fractalParameters params;
	int maxIter = 250;
	int exponent = 2;
	double bailout = 100000000000000000000.;
	// default constructor
	MandelbrotSet() {
		this->bailout = 128.;
		this->maxIter = 250;
		this->exponent = 2;
	}
	MandelbrotSet(fractalParameters params_)
	{
		this->maxIter = params_.integerParameters["maxIter"];
		this->exponent = params_.integerParameters["exponent"];
		this->bailout = params_.doubleParameters["bailout"];
	}
	fractalParameters getParams() const override {
		fractalParameters params;
		params.integerParameters["maxIter"] = this->maxIter;
		params.integerParameters["exponent"] = this->exponent;
		params.doubleParameters["bailout"] = this->bailout;
		return params;
	}
	complex start(complex z, const complex z0) override
	{
		z = z0;
		return z;
	}
	complex iterate(complex z, const complex z0) override
	{
		z = z * z + z0;
		return z;
	}
	bool bailoutCheck(const complex z, const int iter) const override
	{
		return (z.cabs_squared() < this->bailout) ? false : true;
	}
};

// Mandelbrot Set
class JuliaSet : public abstractBaseFractal
{
public:
	fractalParameters params;
	int maxIter = 2500;
	int exponent = 2;
	double bailout = 100000000000000000000.;
	complex seed = complex(-0.4, 0.6);
	// default constructor
	JuliaSet() {
		this->bailout = 128.;
		this->maxIter = 2500;
		this->exponent = 2;
		this->seed = complex(-0.4, 0.6);
	}
	JuliaSet(fractalParameters params_)
	{
		this->maxIter = params_.integerParameters["maxIter"];
		this->exponent = params_.integerParameters["exponent"];
		this->bailout = params_.doubleParameters["bailout"];
		this->seed = params_.complexParameters["seed"];
	}
	fractalParameters getParams() const override {
		fractalParameters params;
		params.integerParameters["maxIter"] = this->maxIter;
		params.integerParameters["exponent"] = this->exponent;
		params.doubleParameters["bailout"] = this->bailout;
		params.complexParameters["seed"] = this->seed;
		return params;
	}
	complex start(complex z, const complex z0) override
	{
		z = z0;
		return z;
	}
	complex iterate(complex z, const complex z0) override
	{
		z = z * z + this->seed;
		return z;
	}
	bool bailoutCheck(const complex z, const int iter) const override
	{
		return (z.cabs_squared() < this->bailout) ? false : true;
	}
};

// // Burning Ship
// class BurningShip : public abstractBaseFractal
// {
// public:
// 	fractalParameters params;
// 	// default constructor
// 	BurningShip() {
// 		this->params.doubleParameters["bailout"] = 128.;
// 		this->params.integerParameters["maxIter"] = 250;
// 		this->params.integerParameters["exponent"] = 2;
// 	}
// 	BurningShip(fractalParameters params_)
// 	{
// 		this->params = params_;	
// 	}
// 	complex start(complex z, const complex z0) override
// 	{
// 		z = z0;
// 	}
// 	complex iterate(complex z, const complex z0) override
// 	{
// 		z = z.abs();
// 		z = z * z + z0;
// 	}
// 	bool bailoutCheck(const complex z, const int iter) const override
// 	{
// 		bailedOut = (z.cabs_squared() < 4. && iter < maxIter) ? false : true;
// 	}
// };

// class BurningShipJulia : public abstractBaseFractal
// {
// public:
// 	fractalParameters params;
// 	// default constructor
// 	BurningShipJulia() {
// 		this->params.doubleParameters["bailout"] = 128.;
// 		this->params.integerParameters["maxIter"] = 250;
// 		this->params.integerParameters["exponent"] = 2;
// 		this->params.complexParameters["seed"] = complex(-0.4, 0.6); // nice location for default power 2 version
// 	}
// 	BurningShipJulia(fractalParameters params_)
// 	{
// 		this->params = params_;	
// 	}
// 	complex start(complex z, const complex z0)
// 	{
// 		z = z0;
// 	}
// 	complex iterate(complex z, const complex z0)
// 	{
// 		z = z.abs();
// 		z = z * z + this->params.complexParameters["seed"];
// 	}
// 	bool bailoutCheck(const complex z, const int iter)
// const 	{
// 		bailedOut = (z.cabs_squared() < 4. && iter < maxIter) ? false : true;
// 	}
// };

// // Mandelbrot Set
// class Manowar : public abstractBaseFractal
// {
// public:
// 	struct variables
// 	{
// 		complex zold;
// 		complex z1;
// 	};
// 	fractalParameters params;
// 	variables vars;
// 	// integer parameters
// 	int maxIter = 250;
// 	int exponent = 2;
// 	// double parameters
// 	double bailout = 120.;
// 	// complex parameters
// 	// default constructor
// 	Manowar() {
// 		this->params.doubleParameters["bailout"] = 128.;
// 		this->params.integerParameters["maxIter"] = 250;
// 		this->params.integerParameters["exponent"] = 2;
// 	}
// 	Manowar(fractalParameters params_)
// 	{
// 		this->params = params_;	
// 	}
// 	fractalParameters getParams() const {
// 		return this->params;"] 
// "] 
// "] 
// "] 
// 	}
// 	complex start(complex z, const complex z0) override
// 	{
// 		z = z0;
// 	}
// 	complex iterate(complex z, const complex z0) override
// 	{
// 		this->vars.zold = z;
// 		z = z * z + this->vars.z1 + z0;
// 		this->vars.z1 = this->vars.zold;
// 	}
// 	bool bailoutCheck(const complex z, const int iter) const override
// 	{
// 		bailedOut = (z.cabs_squared() < 4. && iter < maxIter) ? false : true;
// 	}
// };

// Draw a grid with selected spacings and width
class Grid : public abstractBaseFractal
{
public:
	fractalParameters params;
	// integer parameters
	int maxIter = 250;
	// double parameters
	double GridWidth = 0.01;
	double GridX = 0.2;
	double GridY = 0.2;
	// complex parameters
	// default constructor
	Grid() {
		this->GridWidth = 0.02;
		this->maxIter = 2; // not strictly necessary but allows to color "inside" points
		this->GridX = 0.2;
		this->GridY = 0.2;
	}
	Grid(fractalParameters params_)
	{
		this->GridWidth = params_.doubleParameters["GridWidth"];
		this->maxIter = params_.integerParameters["maxIter"]; // not strictly necessary but allows to color "inside" points
		this->GridX = params_.doubleParameters["GridX"];
		this->GridY = params_.doubleParameters["GridY"];
	}
	~Grid() {}
	fractalParameters getParams() const override {
		fractalParameters params;
		params.integerParameters["maxIter"] = this->maxIter;
		params.doubleParameters["GridWith"] = this->GridWidth;
		params.doubleParameters["GridX"] = this->GridX;
		params.doubleParameters["GridY"] = this->GridY;
		return params;
	}
	complex start(complex z, const complex z0) override
	{
		z = z0;
		return z;
	}
	complex iterate(complex z, const complex z0) override
	{
		return z; // yeah... I know D: 
	}
	double wrapToRange1d(const double x, const double y) const { return x - y * floor(x / y); }
	bool bailoutCheck(const complex z, const int iter) const override
	{
		return (std::min(wrapToRange1d(z.x, this->GridX), wrapToRange1d(z.y, this->GridY)) < this->GridWidth && iter < this->maxIter) ? false : true;
	}
};

abstractBaseFractal *getFractal(std::string FractalName)
{
	if (FractalName == "MandelbrotSet")
		return new MandelbrotSet();
	// else if (FractalName == "JuliaSet")
	// 	return new JuliaSet();
	// else if (FractalName == "BurningShip")
	// 	return new BurningShip();
	// else if (FractalName == "BurningShipJulia")
	// 	return new BurningShipJulia();
	// else if (FractalName == "Manowar")
	// 	return new Manowar();
	else if (FractalName == "Grid")
	    return new Grid();
	else
		return nullptr;
}

abstractBaseFractal *getFractal(std::string FractalName, fractalParameters params)
{
	std::cout << "Called two parameter version of the factory\n";
	if (FractalName == "MandelbrotSet")
		return new MandelbrotSet(params);
	// else if (FractalName == "JuliaSet")
	// 	return new JuliaSet(params);
	// else if (FractalName == "BurningShip")
	// 	return new BurningShip(params);
	// else if (FractalName == "BurningShipJulia")
	// 	return new BurningShipJulia(params);
	// else if (FractalName == "Manowar")
	// 	return new Manowar();
	else if (FractalName == "Grid")
	    return new Grid();
	else
		return nullptr;
}
