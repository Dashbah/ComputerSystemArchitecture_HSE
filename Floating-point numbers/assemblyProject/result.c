//
// Created by Dashbah on 24.11.2022.
//
// function: a + b * x ^ (-4)
// task: calculate integral from left to right
// using Trapezoidal rule

extern double left, right, a, b;

double func(double x) {
    return a + b / (x * x * x * x);
}

double getResult() {
    double result = 0;
    for (double x = left; x <= right - 0.0001; x += 0.0001) {
        if (x!=0) {
            // 0.00005 = 0.0001 / 2
            // 0.0001 is the height
            result += 0.00005 * (func(x) + func(x + 0.0001));
        }
    }
    return result;
}

double checker() {
    return a * right - a * left + b / (3 * left * left * left) - b / (3 * right * right * right);
}