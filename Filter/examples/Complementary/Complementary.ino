#include <Filter.h>

ComF f(0.01);	// res = a * 0.99 + b * 0.01

double u0 = 100; // real state
double e; // noise
double u; // measurement

void setup()
{
  Serial.begin(9600);
  f.update(u0, u0);	// A way to initialize
}
void loop()
{
  randomSeed(micros() + millis());
  e = random(-100, 100);
  u = u0 + e;
  Serial.print(u);
  Serial.print(",");
  Serial.println(f.update(f.result(), u));	// Use model instead of f.result()
}
