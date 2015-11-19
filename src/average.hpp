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
#include <string>
#include <map>

#include <Eigen/Dense>


/**
 * Running average
 */
template <typename dt>
class Average_ {
public:
	typedef dt data_type;
	typedef Average_<dt> This;
	typedef Eigen::Matrix<data_type, Eigen::Dynamic, 1> Point;
	typedef std::map<std::string, Point> PointComponents;

public:

	Average_(size_t dimentions):
		_n(0),
		_M2(0),
		_mean(dimentions), _min(dimentions), _max(dimentions)
	{
	}

	virtual ~Average_() {
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

	virtual PointComponents getComponents() const {
		PointComponents c;
		c["min"] = getMin();
		c["max"] = getMax();
		c["mean"] = getMean();

		return c;
	}

	/**
	 * Add new datum point
	 * New datum might update distribution properties
	 */
	virtual This& addPoint(const Point& x) {
	    const Point delta = x - _mean;

	    ++_n;
	    _mean += (delta / _n);
	    _M2 += delta.dot(x - _mean);

	    // update min/max
	    for (size_t i = 0; i < getDimentions(); ++i) {
	    	_min[i] = std::min(_min[i], x[i]);
	    	_max[i] = std::max(_max[i], x[i]);
	    }

	    return *this;
	}

	/**
	 * Remove datum point from the distribution
	 * Removal of datum point might update distribution properties
	 */
	virtual This& removePoint(const Point& x) {
	    const Point delta = x - _mean;

	    --_n;
	    _mean -= delta/_n;
	    _M2 -= delta.dot(x - _mean);

	    return *this;
	}

	/**
	 * Change datum point
	 * Datum point change might update distribution properties
	 */
	virtual This& updateVariable(const Point& oldX, const Point& newX) {
	    const Point delta = newX - oldX;
	    const Point dold = oldX - _mean;

	    _mean = _mean + delta/_n;

	    const Point dnew = newX - _mean;
	    _M2 += delta.dot(dold + dnew);

	    return *this;
	}

protected:
	size_t 		_n;
	data_type	_M2;
	Point 		_mean;

	Point 		_min;
	Point 		_max;

};


template <typename dt>
class ExponentialMovingAverage_ : public Average_<dt> {
public:
	typedef Average_<dt> Average;
	typedef typename Average::data_type data_type;
	typedef typename Average::Point Point;
	typedef typename Average::PointComponents PointComponents;

public: 

	ExponentialMovingAverage_(size_t dimentions, data_type v): Average(dimentions), 
					_lambda(v),
					_EMA(dimentions) 
	{}

	virtual ~ExponentialMovingAverage_() {}

	virtual Average& addPoint(const Point& x) {
	    const Point newEma = x * _lambda + _EMA * (1 - _lambda);
	    _EMA = this->getDataSize() ? newEma : x;

	    return Average::addPoint(x);
	}


	data_type getLamda() const { return _lambda; }

	virtual PointComponents getComponents() const {
		PointComponents c = Average::getComponents();

		c["emp"] = _EMA;

		return c;
	}

private:
	// Exponential moving average stats:
	data_type 	_lambda;
	Point 		_EMA;

};


#endif/*AVERAGE_HPP_*/
