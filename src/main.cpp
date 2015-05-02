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

#include "average.hpp"

typedef unsigned int uint;
typedef Average_<float> Average;

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
	stream << "{ ";

	stream << "i: " << a.getDataSize() << ", ";
	stream << "v: " << sqrt(a.getVariance()) << ", ";

	stream << "mean: [ " << a.getMean() << " ], ";
	stream << "min: [ " << a.getMin() << " ], ";
	stream << "max: [ " << a.getMax() << " ] ";

	stream << "} " << std::endl;

	return stream;
}

int main(int argc, const char* argv[]) {

	size_t dimentions = 1;

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
		} else {
			std::cerr << "Unexpected argument '" << argv[arg] << "', abort" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	Average average(dimentions);
	Average::Point datum_point(dimentions);

	while (std::cin) {

		for (Average::Point::Index i = 0; i < datum_point.size() && std::cin; ++i) {
			std::cin >> datum_point(i);
		}

		if (std::cin) {
			average.addPoint(datum_point);
			std::cout << average;
		}
	}

	return EXIT_SUCCESS;
}

