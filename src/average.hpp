/*******************************************************************************
 * @file: average.hpp
 *      @author: soultaker
 *
 * Created on: 20 Apr 2014
*******************************************************************************/
#pragma once
#ifndef AVERAGE_HPP_
#define AVERAGE_HPP_

#include <limits>       // std::numeric_limits

#include <Eigen/Dense>


/**
 * Running average
 */
template <typename dt>
class Average_ {
public:
	typedef dt data_type;
//	typedef Eigen::VectorXd Point;
	typedef Eigen::Matrix<data_type, Eigen::Dynamic, 1> Point;

public:

	Average_(size_t dimentions):
		_n(0),
		_M2(0),
		_mean(dimentions), _min(dimentions), _max(dimentions)
	{
	}

	/**
	 * Get data-set dimensionality
	 */
	size_t getDimentions() const {
		return _mean.size();
	}

	/**
	 * Get data sample size seen so far
	 */
	size_t getDataSize() const { return _n; }

	/**
	 * Get mean point in a data set processed so far
	 */
	const Point& getMean() const { return _mean; }

	/**
	 * Get variance of the data processed so far
	 */
	data_type getVariance() const {
	    return (_n == 0) ? std::numeric_limits<data_type>::infinity()
	    		: ((_n == 1) ? 0 : _M2 / (_n - 1));
	}

	/** Get coordinate-wise minimum */
	const Point& getMin() const { return _min; }

	/** Get coordinate-wise maximum */
	const Point& getMax() const { return _max; }

	/**
	 * Add new datum point
	 * New datum might update distribution properties
	 */
	void addPoint(const Point& x) {
	    const Point delta = x - _mean;

	    ++_n;
	    _mean += (delta / _n);
	    _M2 += delta.dot(x - _mean);

	    // update min/max
	    for (size_t i = 0; i < getDimentions(); ++i) {
	    	_min[i] = std::min(_min[i], x[i]);
	    	_max[i] = std::max(_max[i], x[i]);
	    }
	}

	/**
	 * Remove datum point from the distribution
	 * Removal of datum point might update distribution properties
	 */
	void removePoint(const Point& x) {
	    const Point delta = x - _mean;

	    --_n;
	    _mean -= delta/_n;
	    _M2 -= delta.dot(x - _mean);
	}

	/**
	 * Change datum point
	 * Datum point change might update distribution properties
	 */
	void updateVariable(const Point& oldX, const Point& newX) {
	    const Point delta = newX - oldX;
	    const Point dold = oldX - _mean;

	    _mean = _mean + delta/_n;

	    const Point dnew = newX - _mean;
	    _M2 += delta.dot(dold + dnew);
	}

private:
	size_t 		_n;
	data_type	_M2;
	Point 		_mean;

	Point 		_min;
	Point 		_max;
};


#endif/*AVERAGE_HPP_*/
