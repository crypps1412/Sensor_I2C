#include <Filter.h>

AverF f(100);	// average of 100 values

double u0 = 100; // real state
double e; // noise
double u; // measurement

void setup()
{
  Serial.begin(9600);
  do {
    randomSeed(micros() + millis());
    e = random(-100, 100);
  }
  while (f.init(u = u0 + e));
}
void loop()
{
  randomSeed(micros() + millis());
  e = random(-100, 100);
  u = u0 + e;
  Serial.print(u);
  Serial.print(",");
  Serial.println(f.update(u));
}