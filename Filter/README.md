# Filter
Release on Sat July 10 09:23:38 CDT 2021 Version 1.0

The library includes three filters: Kalman filter, Complementary filter and Average filter. result() function can be used in all class to extract state value. All set function can be used again.

- Kalman filter needs to be initialized with 3 variances (P, Q, R) in constructor and a start value (default is 0) in init() function. restart() function is for reset P value due to its change in loop.

- Complementary filter only requires ratio before any update.

- Much more setup need to be done in Average filter. There's an easy way for initial code: "while (AverageObject.init(getData()));", while getData() is user's function to pass data from sensor.