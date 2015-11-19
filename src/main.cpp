/*******************************************************************************
 * @file: main.cpp
 *      @author: soultaker
 *
 * Created on: 20 Apr 2014
*******************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <memory>

#include "average.hpp"


typedef unsigned int uint;
typedef float	DataType;
typedef Average_<DataType> Average;
typedef ExponentialMovingAverage_<DataType> ExponentialMovingAverage;


std::ostream& operator << (std::ostream& stream, const Average::Point& p) {
	for (Average::Point::Index i = 0; i < p.size(); ++i) {
		stream << p(i);

		if (i + 1 < p.size()) {
			stream << ", ";
		}
	}

	return stream;
}

std::ostream& operator << (std::ostream& stream, const Average& a) {
	stream << a.getDataSize() << ": ";

	stream << "{ ";
	stream << "v: " << sqrt(a.getVariance()) << ", ";

	const Average::PointComponents c = a.getComponents();
	for (Average::PointComponents::const_iterator i = c.begin(), end = c.end(); i != end; ++i) {
		stream << i->first << ": [ " << i->second << " ], ";
	}

	stream << "}" << std::endl;

	return stream;
}

int main(int argc, const char* argv[]) {

	// Command line options
	size_t dimentions = 1;
	DataType expLambda = 0;
	bool expGiven = false;
	bool summaryOnly = false;

	for (int arg = 1; arg < argc; ++arg) {

		if (strcmp(argv[arg], "-d") == 0 ||
			strcmp(argv[arg], "--dim") == 0) {
			if (arg + 1 >= argc) {
				std::cerr << "Expecting an argument after '" << argv[arg] << "', abort" << std::endl;

				exit(EXIT_FAILURE);
			} else {
				dimentions = atoi(argv[arg + 1]);
			}

			arg++;
		} else if (strcmp(argv[arg], "-e") == 0 ||
				   strcmp(argv[arg], "--exp") == 0) {
			if (arg + 1 >= argc) {
				std::cerr << "Expecting an argument after '" << argv[arg] << "', abort" << std::endl;

				exit(EXIT_FAILURE);
			} else {
				expLambda = atof(argv[arg + 1]);
				expGiven = true;
			}

			arg++;
		} else if (strcmp(argv[arg], "-s") == 0 ||
				   strcmp(argv[arg], "--summary") == 0) {
			summaryOnly = true;
		 } else {
			std::cerr << "Unexpected argument '" << argv[arg] << "', abort" << std::endl;

			exit(EXIT_FAILURE);
		}
	}

	Average::Point datum_point(dimentions);
	std::auto_ptr<Average> average(expGiven 
									? new ExponentialMovingAverage(dimentions, expLambda) 
									: new Average(dimentions)
								);
	while (std::cin) {

		for (Average::Point::Index i = 0; i < datum_point.size() && std::cin; ++i) {
			std::cin >> datum_point(i);
		}

		if (std::cin) {
			average->addPoint(datum_point);

			if (!summaryOnly) {
				std::cout << *average;
			}
		}
	}

	if (summaryOnly) {
		std::cout << *average;
	}

	return EXIT_SUCCESS;
}

