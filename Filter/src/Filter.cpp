#include "Filter.h"

/**********Kalman filter**********/
KalF::KalF(double const &VX, double const &VU, double const &VZ): _initP(VX), _p(VX), _q(VU), _r(VZ) {};

double KalF::init(const double &x) {
	return _x = x;
}

double KalF::setVarX(const double &p) {
	_initP = p;
	return _p = p;
}

double KalF::setVarU(const double &q) {
	return _q = q;
}

double KalF::setVarZ(const double &r) {
	return _r = r;
}

double KalF::restart() {
	return _p = _initP;
}

double KalF::update(const double &z, double u = 0) {
	// Predict by model
	_x = _x + u;
	_p = _p + _q;

	// Correct by measurement
	_k = (double)_p / (_p + _r);
	_p = (1 - _k) * _p;
	return _x = _x + _k * (z - _x);
}

double KalF::result() {
	return _x;
}


/**********Complementary filter**********/
ComF::ComF(const double &k): _k(k) {};

double ComF::setRate(const double &k) {
	return _k = k;
}

double ComF::update(const double &a, const double &b) {
	return _x = (1 - _k) * a + _k * b;
}

double ComF::result() {
	return _x;
}


/**********Average filter**********/
AverF::AverF(const int &N): _num(N) {
	_list = new double[N];
}

AverF::~AverF() {
	delete[] _list;
}

bool AverF::init(const double &val) {
	if (_full) return 0;
	_list[_id++] = val;
	_sum += val;
	if (_id == _num) {
		_id = 0;
		_full = 1;
		return 0;
	}
	return 1;
}

void AverF::setSize(const int &N) {
	double *arr = new double[N];
	if (_num < N) {
		for (int i = 1; i <= _num; i++) {
			if (_id - i < 0) _id += _num;
			arr[_num - i] = _list[_id - i];
		}
		_id = _num;
		_full = 0;
	} else {
		for (int i = 1; i <= N; i++) {
			if (_id - i < 0) _id += _num;
			arr[N - i] = _list[_id - i];
		}
		_id = 0;
	}
	delete[] _list;
	_list = arr;
	_num = N;
}

void AverF::restart(int N = 0) {
	_sum = 0;
	_id = 0;
	_full = 0;
	if (N != 0) {
		_num = N;
		delete[] _list;
		_list = new double[N];
	}
}

double AverF::update(const double &val) {
	_sum -= _list[_id];
	_list[_id++] = val;
	_sum += val;
	if (_id == _num) _id = 0;
	return _res = _sum / _num;
}

double AverF::result() {
	return _res;
}
