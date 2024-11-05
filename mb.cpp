#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <chrono>

#include "FractalFormulas.h"
#include "Gradient.h"

using std::cout;
using std::endl;
using std::ostream;

const double pi = 3.14159265359;

// get complex coordinate for the (sub)pixel coordinates using
// center, zoom and image dimensions
complex getComplexCoordinate(const double x, const double y, const complex center, const double magn,
	const complex rotation, const std::vector<std::vector<double>> &skew, const double span, const int imgWidth, const int imgHeight)
{
	const double aspect = (double)imgWidth / imgHeight;
	const double xRange = 2 * span * aspect;
	const double yRange = 2 * span;
	complex z(((x/imgWidth) - 0.5)*xRange, ((y/imgHeight) - 0.5)*yRange);
	z = rotation * z * (1./ magn);
	z = complex(skew[0][0] * z.x + skew[0][1] * z.y, skew[1][0] * z.x + skew[1][1] * z.y);
	return z + center;
}



// AA stuff
// int to double
double uintToDouble(const uint32_t n)
{
	constexpr double scale = 1. / (1ull << 32);
	return n * scale;
}

// integer hashing function
//https://burtleburtle.net/bob/hash/integer.html
uint32_t hash(uint32_t a)
{
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
}

inline double wrap1d(const double x, const double y) 
{ 
	return (x + y > 1) ? x + y - 1 : x + y;
}

inline double sign(const double x) {return (x==0) ? 0 : x/std::abs(x); }

// tent filter ignoring zeros
double triDist(const double x)
{
	const double s = 2*x - 1;
	return sign(s)*(1-std::sqrt(std::abs(s)));
}

// Halton sequence
template <const int b>
double halton(int i)
{
	double f = 1;
	double r = 0;
	double inv_b = 1./b;
	while (i > 0)
	{
		f = f * inv_b;
		r = r + f * (i%b);
		i = i/b;
	}
	return r;
}

// write to PPM
void writeImage(const std::vector<float> &imgData, const int &imgWidth, const int &imgHeight, const int &maxPasses)
{
	std::ofstream ofs("test4.ppm");
	ofs << "P6" << endl << imgWidth << " " << imgHeight << endl << "255" << endl;
	for (int jj = 0; jj < imgHeight; jj++)
	{
		for (int ii = 0; ii < imgWidth; ii++)
		{
			int pixelIndex = jj*imgWidth + ii;
			// int colR = (int)(linearToSRGB(imgData[pixelIndex*3  ]/maxPasses)*255);
			// int colG = (int)(linearToSRGB(imgData[pixelIndex*3+1]/maxPasses)*255);
			// int colB = (int)(linearToSRGB(imgData[pixelIndex*3+2]/maxPasses)*255);
			int colR = (int)(imgData[pixelIndex*3  ]/maxPasses*255);
			int colG = (int)(imgData[pixelIndex*3+1]/maxPasses*255);
			int colB = (int)(imgData[pixelIndex*3+2]/maxPasses*255);
			ofs << (char)colR << (char)colG << (char)colB;
		}
	}
	ofs.close();
}

void test_operators()
{
	cout << "Testing complex functions and operators\n\n";
	const complex a(1,2);
	const complex b(3,4);
	// bunch of constants
	// const int k = 2;
	// const int l = 3;
	// const double x = 2.5;
	// const double y = std::sqrt(2.0);
	cout << "conj(" << a << ")=" << a.conj() << "\n";
	cout << "flip(" << a << ")=" << a.flip() << "\n";
	cout << "abs(" << a << ")=" << a.abs() << "\n";
	cout << "cabs(" << a << ")=" << a.cabs() << "\n";
	cout << "cabs_squared(" << a << ")=" << a.cabs_squared() << "\n";
	cout << a << "+" << b << "=" << a+b << "\n";
	cout << a << "+2=" << a+2 << "\n";
	cout << "2+" << a << "=" << 2+a << "\n";
	cout << a << "-" << b << "=" << a-b << "\n";
	cout << a << "-2=" << a-2 << "\n";
	cout << "2-" << a << "=" << 2-a << "\n";
	cout << a << "*" << b << "=" << a*b << "\n";
	cout << a << "*2=" << a*2 << "\n";
	cout << "2*" << a << "=" << 2*a << "\n";
	cout << a << "/" << b << "=" << a/b << "\n";
	cout << a << "/2=" << a/2 << "\n";
	cout << "2/" << a << "=" << 2/a << "\n";
	cout << "exp(" << a << ")=" << a.exp() << "\n";
	cout << "log(" << a << ")=" << a.log() << "\n";
	cout << "pow(" << a << ", 9)=" << pow(a,9) << "\n";
	cout << "pow(" << a << ", 15)=" << pow(a,15) << "\n";
	cout << "pow(" << a << ", -9)=" << pow(a,-9) << "\n";
	cout << "pow(" << a << ", -10)=" << pow(a,-10) << "\n";	
	cout << "pow(" << a << ", -3)=" << pow(a,-3) << "\n";
	cout << "pow(" << a << ", 1.5)=" << pow(a,1.5) << "\n";
	cout << "pow(" << a << ", -1.5)=" << pow(a,-1.5) << "\n";
	cout << a << ".sqr()=" << a.sqr() << "\n";
	cout << a << ".cube()=" << a.cube() << "\n";
	color col1;
	color col2(0.5f);
	color col3(0.1f, 0.2f, 0.9f);
	cout << col1 << "+" << col2 << "=" << col1+col2 << "\n";
	cout << "0.3*" << col3 << "=" << 0.3f*col3 << "\n";
	cout << col3 << "*0.3=" << col3*0.3 << "\n";
	cout << col2 << "+" << col3 << "=" <<col2+col3 << "\n";
	cout << "sRGBtoLinear(col3)=" << sRGBtoLinear(col3) << "\n";
	cout << "linearToSRGB(col3)=" << linearToSRGB(col3) << "\n";
	cout << "linearToSRGB(sRGBtoLinear(col3))=" << linearToSRGB(sRGBtoLinear(col3)) << "\n";
	cout << "sRGBtoLinear(linearToSRGB(col3))=" << sRGBtoLinear(linearToSRGB(col3)) << "\n";

}


int main()
{
      //omp_set_num_threads(4);
	// test_operators();
	// return 0;
	// image dimensions
	const int imgMult = 240 	; // 1280x720
	const int imgWidth = 16*imgMult;
	const int imgHeight =9*imgMult;
	// location in the complex plane
	const complex center(-0.5, 0);
	const double angle = 0. / 180. * pi;
	const complex rotation = complex(std::cos(angle), std::sin(angle));
	const std::vector<std::vector<double>> skew{ {1, 0}, {0, 1 }};
	const double magn = 1;

	// other parameters:
	// const char* fractalName = "morphingMB";
	const double span = 1.5; // base size of region shown
	// const complex seed(-0.4, 0.6); // Julia seed
	// const int maxIter = 2550;
	const int maxPasses = 1024;
	const double invMaxPasses = 1./maxPasses;

	std::vector<float> image(imgWidth*imgHeight*3, 0);
	//abstractBaseFractal* fractal_ = getFractal(fractalName);
	//fractalParameters params = fractal_->params;
	//params.complexParameters["seed"] = complex(0.0987, 0.2412);
	//params.complexParameters["seed"] = complex(0.27, 0);
	//cout << "Calling factory... ";
	// abstractBaseFractal* fractal = getFractal(fractalName); //, params); 

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	for (int pass = 0; pass < maxPasses; pass++)
	{
		cout << "Pass " << pass << "... ";
     	        #pragma omp parallel for schedule(dynamic,1)
		for (int ii = 0; ii < imgHeight; ii++)
		{
                  //cout << "Hello from thread ???"; //<< omp_get_thread_num() << "\n";
			for (int jj = 0; jj < imgWidth; jj++)
			{
				const int pixelIndex = ii*imgWidth+jj;
				// abstractBaseFractal *fractal;
				// JuliaSet jul; // initiate fractal formula with seed, bailout, maxIter
				// fractal = &jul;
				abstractBaseFractal* fractal; //getFractal(fractalName); //, params);
				JuliaSet fr;
				fractal = &fr;
				const double hashValue = uintToDouble(hash(pixelIndex));
				const double xOffset = std::min(maxPasses - 1, 1) * triDist(wrap1d((double)pass*invMaxPasses, hashValue)); // Hammersley
				const double yOffset = std::min(maxPasses - 1, 1) * triDist(wrap1d(halton<2>(pass), hashValue));
				const double xShifted = jj + xOffset;
				const double yShifted = ii + yOffset;
				const complex z0 = getComplexCoordinate(xShifted, yShifted, center, 
					magn, rotation, skew, span, imgWidth, imgHeight);
				complex z;
				int iter = 0;
				bool bailedOut = false;
				z = fractal->start(z, z0);
				while (!bailedOut && iter < fr.maxIter)
				{
					z = fractal->iterate(z, z0);
					//cout << z << " ";
					iter++;
					bailedOut = fractal->bailoutCheck(z, iter);
				}
				//float grayVal = (iter < fr.maxIter) ? 1.f : 0.f; //0.5f*std::cos(0.1f*iter) : 0.f;
				//color pixelColor = (bailedOut) ? sRGBtoLinear(standard_muted.get_color(0.1*sqrt((float)iter))) : color(0);
				color pixelColor = (bailedOut) ? volcano_under_a_glacier.get_color(0.1*sqrt((float)iter)) : color(0);
				//color pixelColor = (bailedOut) ? standard_muted.get_color(0.1*sqrt((float)iter)) : color(0);
				//color pixelColor = (bailedOut) ? standard_muted.get_color(0.05*(float)iter) : color(0);
				//color pixelColor = (bailedOut) ? ((iter/5)%2 == 1) ? color(1) : color(0) : color(0);
				//cout << iter << "," << (float)iter/100. << ":" << pixelColor << "\n";
				image[3*pixelIndex    ] += pixelColor.r; //grayVal;
				image[3*pixelIndex + 1] += pixelColor.g; //grayVal;
				image[3*pixelIndex + 2] += pixelColor.b; //grayVal;
			}
		}
		cout << "Done.\n";
	}
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
	std::cout << "Calculation took " << time_span.count() << " seconds.\n";
	writeImage(image, imgWidth, imgHeight, maxPasses);
	return 0;
}
