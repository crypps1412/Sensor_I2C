#ifndef __CLASS3D_H__
#define __CLASS3D_H__

template<class T> class VECTOR_3D {
  private:
    T _data[3];
  public:
    /* Constructor */
    VECTOR_3D(T a, T b, T c) {
      _data[0] = a;
      _data[1] = b;
      _data[2] = c;
    }
    VECTOR_3D(T a[]): VECTOR_3D(a[0], a[1], a[2]) {};
    
    /* Type fix */
    template<class P> VECTOR_3D(const VECTOR_3D<P>& a): VECTOR_3D(a[0], a[1], a[2]) {};
    VECTOR_3D(T a = 0): VECTOR_3D(a, a, a) {};
    operator T() const {  // length
      return sqrt(_data[0]*_data[0] + _data[1]*_data[1] + _data[2]*_data[2]);
    }

    /* Property */
    void print(char delim = ' ', char endChar = '\n', uint8_t numF = 2) {
      Serial.print(_data[0], numF);
      Serial.print(delim);
      Serial.print(_data[1], numF);
      Serial.print(delim);
      Serial.print(_data[2], numF);
      Serial.print(endChar);
    }
    // Operator
    T operator [] (const uint8_t& i) const {
      return _data[i];
    }
    VECTOR_3D operator -() const {
      return VECTOR_3D(-_data[0], -_data[1], -_data[2]);
    }
    VECTOR_3D operator +(const VECTOR_3D& a) const {
      return VECTOR_3D(_data[0]+a._data[0], _data[1]+a._data[1], _data[2]+a._data[2]);
    }
    VECTOR_3D operator -(const VECTOR_3D& a) const {
      return *this + -a;
    }
    VECTOR_3D operator *(const T& a) const {  // vector * number
      return VECTOR_3D(_data[0]*a, _data[1]*a, _data[2]*a);
    }
    VECTOR_3D operator *(const VECTOR_3D& a) const {  // this->diag * vector
      return VECTOR_3D(_data[0]*a._data[0], _data[1]*a._data[1], _data[2]*a._data[2]);
    }
    VECTOR_3D operator ^(const VECTOR_3D& a) const {  // vector ^ vector
      return VECTOR_3D( _data[1]*a._data[2] - _data[2]*a._data[1],
                        _data[2]*a._data[0] - _data[0]*a._data[2],
                        _data[0]*a._data[1] - _data[1]*a._data[0] );
    }
    T operator /(const VECTOR_3D& a) const {  // this->transpose vector * vector
      return _data[0]*a._data[0] + _data[1]*a._data[1] + _data[2]*a._data[2];
    }
    VECTOR_3D operator ~() {  // standardized vector
      return *this * (1.0 / *this);
    }

    /* Method */
    void setElement(const int& row, const int& val) {
      _data[row] = val;
    }
};

template<class T> class MATRIX_3D {
  private:
    T _data[3][3];
  public:
    /* Constructor */
    MATRIX_3D(T a1, T a2, T a3, T a4, T a5, T a6, T a7, T a8, T a9) {
      _data[0][0] = a1; _data[0][1] = a2; _data[0][2] = a3;
      _data[1][0] = a4; _data[1][1] = a5; _data[1][2] = a6;
      _data[2][0] = a7; _data[2][1] = a8; _data[2][2] = a9;
    }
    MATRIX_3D(T a[][3]): MATRIX_3D(a[0][0], a[0][1], a[0][2], a[1][0], a[1][1], a[1][2], a[2][0], a[2][1], a[2][2]) {};
    MATRIX_3D(const VECTOR_3D<T>& a, const VECTOR_3D<T>& b, const VECTOR_3D<T>& c):
      MATRIX_3D(a[0], b[0], c[0], a[1], b[1], c[1], a[2], b[2], c[2]) {};

    /* Type fix */
    MATRIX_3D(T a = 0): MATRIX_3D(a, a, a, a, a, a, a, a, a) {};

    /* Property */
    // Operator
    T operator () (const int& i, const int& j) const {  // element
      return _data[i][j];
    }
    VECTOR_3D<T> operator () (const int& i) const {  // row
      return VECTOR_3D<T>(_data[i][0], _data[i][1], _data[i][2]);
    }
    MATRIX_3D operator +(const MATRIX_3D& a) const {
      return MATRIX_3D(   _data[0][0]+a._data[0][0], _data[0][1]+a._data[0][1], _data[0][2]+a._data[0][2],
                          _data[1][0]+a._data[1][0], _data[1][1]+a._data[1][1], _data[1][2]+a._data[1][2],
                          _data[2][0]+a._data[2][0], _data[2][1]+a._data[2][1], _data[2][2]+a._data[2][2] );
    }
    MATRIX_3D operator -(const MATRIX_3D& a) const {
      return MATRIX_3D(   _data[0][0]-a._data[0][0], _data[0][1]-a._data[0][1], _data[0][2]-a._data[0][2],
                          _data[1][0]-a._data[1][0], _data[1][1]-a._data[1][1], _data[1][2]-a._data[1][2],
                          _data[2][0]-a._data[2][0], _data[2][1]-a._data[2][1], _data[2][2]-a._data[2][2] );
    }
    MATRIX_3D operator *(const T& a) const { // matrix * number
      return MATRIX_3D(   _data[0][0]*a, _data[0][1]*a, _data[0][2]*a,
                          _data[1][0]*a, _data[1][1]*a, _data[1][2]*a,
                          _data[2][0]*a, _data[2][1]*a, _data[2][2]*a );
    }
    VECTOR_3D<T> operator *(const VECTOR_3D<T>& a) const {  // matrix * vector
      return VECTOR_3D<T>(  VECTOR_3D<T>(_data[0][0], _data[0][1], _data[0][2]) / a,
                            VECTOR_3D<T>(_data[1][0], _data[1][1], _data[1][2]) / a,
                            VECTOR_3D<T>(_data[2][0], _data[2][1], _data[2][2]) / a );
    }
    MATRIX_3D operator *(const MATRIX_3D& a) const {  // matrix * matrix
      return MATRIX_3D(   _data[0][0]*a._data[0][0] + _data[0][1]*a._data[1][0] + _data[0][2]*a._data[2][0],
                          _data[0][0]*a._data[0][1] + _data[0][1]*a._data[1][1] + _data[0][2]*a._data[2][1],
                          _data[0][0]*a._data[0][2] + _data[0][1]*a._data[1][2] + _data[0][2]*a._data[2][2],
                          _data[1][0]*a._data[0][0] + _data[1][1]*a._data[1][0] + _data[1][2]*a._data[2][0],
                          _data[1][0]*a._data[0][1] + _data[1][1]*a._data[1][1] + _data[1][2]*a._data[2][1],
                          _data[1][0]*a._data[0][2] + _data[1][1]*a._data[1][2] + _data[1][2]*a._data[2][2],
                          _data[2][0]*a._data[0][0] + _data[2][1]*a._data[1][0] + _data[2][2]*a._data[2][0],
                          _data[2][0]*a._data[0][1] + _data[2][1]*a._data[1][1] + _data[2][2]*a._data[2][1],
                          _data[2][0]*a._data[0][2] + _data[2][1]*a._data[1][2] + _data[2][2]*a._data[2][2] );
    }
    MATRIX_3D operator /(const VECTOR_3D<T>& a) const {  // matrix * diag(vector)
      return MATRIX_3D(   _data[0][0]*a[0], _data[0][1]*a[1], _data[0][2]*a[2],
                          _data[1][0]*a[0], _data[1][1]*a[1], _data[1][2]*a[2],
                          _data[2][0]*a[0], _data[2][1]*a[1], _data[2][2]*a[2] );
    }
    MATRIX_3D operator !() const {  // invert matrix
      MATRIX_3D<T> a(_data);
      VECTOR_3D<T> col[3];
      col[0] = a(1)^a(2);
      col[1] = a(2)^a(0);
      col[2] = a(0)^a(1);
      T det = a(0) / col[0];
      return MATRIX_3D(col[0]*(1/det), col[1]*(1/det), col[2]*(1/det));
    }
    MATRIX_3D operator ~() const {  // transpose matrix
      return MATRIX_3D( _data[0][0], _data[1][0], _data[2][0],
                        _data[0][1], _data[1][1], _data[2][1],
                        _data[0][2], _data[1][2], _data[2][2] );
    }
};

class RPY {
  private:
    MATRIX_3D<double> _R;
    VECTOR_3D<double> _angle;
  public:
    RPY(const double& c, const double& b, const double& a): // RPY angle: X-roll-c; Y-pitch_b; Z-yaw-a
      _R(MATRIX_3D<double>( cos(a)*cos(b),  -sin(a)*cos(c)+cos(a)*sin(b)*sin(c),  sin(a)*sin(c)+cos(a)*sin(b)*cos(c),
                            sin(a)*cos(b),  cos(a)*cos(c)+sin(a)*sin(b)*sin(c),   -cos(a)*sin(c)+sin(a)*sin(b)*sin(c),
                            -sin(b),        cos(b)*sin(c),                        cos(b)*cos(c) )),
      _angle(VECTOR_3D<double>(c, b, a)) {}
    RPY(const MATRIX_3D<double>& a):
      _R(a),
      _angle(VECTOR_3D<double> (atan2(a(2,1), a(2,2)), asin(-a(2,0)), atan2(a(1,0), a(0,0)))) {}
    RPY(const VECTOR_3D<double>& a, const VECTOR_3D<double>& b, const VECTOR_3D<double>& c):
      RPY(MATRIX_3D<double>(a[0], b[0], c[0], a[1], b[1], c[1], a[2], b[2], c[2])) {};
    RPY(const VECTOR_3D<double>& angle): RPY(angle[0], angle[1], angle[2]) {};

    /* Type fix */
    RPY(double a = 0): RPY(a, a, a) {}

    /* Property */
    VECTOR_3D<double> radian() const {
      return _angle;
    }
    VECTOR_3D<double> degree() const {
      return _angle * 57.2958;
    }
    // Operator
    RPY operator +(const VECTOR_3D<double>& a) const {  // RPY + vector
      return _angle + a;
    }
    RPY operator *(const RPY& a) const {  // RPY * RPY
      return _R * a._R;
    }
    bool operator !=(const RPY& a) const {  // if 2 RPY have big gap in angles
      return abs(_angle[0] - a._angle[0]) > 0.8 ||
             abs(_angle[1] - a._angle[1]) > 0.8 ||
             abs(_angle[2] - a._angle[2]) > 0.8;
    }
};


#endif
