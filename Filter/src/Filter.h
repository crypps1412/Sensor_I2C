/*
 * My nickname is crypps1412
 * and these are Kalman, Complementary and Average filter.
 * Kalman filter is Complementary filter in a more mathematical way,
 * using when there're 2 option for 1 answer.
 * Average filter is used in high-frequency noise model.
 * You can also use Gaussian, Median or other low-pass filter instead.
 * You can find this in: https://github.com/crypps1412/MPU6050
 * Enjoy coding.
 */

#ifndef __FILTER_H__
#define __FILTER_H__

/**********Kalman filter**********/
class KalF {
		double _x = 0;	// State variable
		double _initP, _p, _q, _r;	// Variances and their default
		double _k;	// Ratio
	public:
		KalF(const double &VX, const double &VU, const double &VZ);
		virtual double init(const double &x);	// Initial state value
		virtual double update(const double &z, double u = 0);	// Predict and correct the state value
		virtual double setVarX(const double &p);	// Set variance for state value
		virtual double setVarU(const double &q);	// Set variance for control value
		virtual double setVarZ(const double &r);	// Set variance for measurement value
		virtual double restart();	// State variance return to initial value
		virtual double result();	// Get state value
};

/**********Complementary filter**********/
class ComF {
		double _x = 0;	// State variable
		double _k;	// Ratio
	public:
		ComF(const double &k);
		virtual double update(const double &a, const double &b);	// Calculate the state value
		virtual double setRate(const double &k);	// Set ratio between 2 parameter
		virtual double result();	// Get state value
};

/**********Average filter**********/
class AverF {
		double _sum = 0, _res, *_list;	// Sum, result and list of state value
		int _id = 0, _num;	// Index of the oldest state value from list, number of stages
		bool _full;	// Check if initializing completely
	public:
		AverF(const int &N);
		~AverF();
		bool init(const double &val);	// Set list and sum of all value
		void setSize(const int &N);	// Change number of stages, if more than before then continue to initialize values left
		void restart(int N = 0);	// Throw away every old states and get new ones, if N != 0 then also resize
		double update(const double &val);		// Calculate the state value
		double result();	// Get state value
};
#endif
