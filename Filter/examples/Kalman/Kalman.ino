#include <Filter.h>

KalF f(2, 0.0001, 2);	// P=2, Q=0.0001, R=2

double u0 = 100; // real state
double e; // noise
double u; // measurement

void setup()
{
	Serial.begin(9600);
	f.init(u0);
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