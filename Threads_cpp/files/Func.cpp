//
// Created by Dashbah on 14.12.2022.

//
#include <cmath>
#include <algorithm>
#include <thread>
#include <vector>

extern double left, right;

double EPS = 0.00001;

double func(double x) {
    return sin(5 * x) + 1;
}


double qIntegral(double left_, double right_, double f_left, double f_right, double intgrl_now) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    double mid = (left_ + right_) / 2;
    double f_mid = func(mid);

    //Аппроксимация по левому отрезку
    double l_integral = (f_left + f_mid) * (mid - left_) / 2;
    //Аппроксимация по правому отрезку
    double r_integral = (f_mid + f_right) * (right_ - mid) / 2;

    if (abs((l_integral + r_integral) - intgrl_now) > EPS) {
        //Рекурсия для интегрирования обоих значений
        l_integral = qIntegral(left_, mid, f_left, f_mid, l_integral);
        r_integral = qIntegral(mid, right_, f_mid, f_right, r_integral);
    }

    return (l_integral + r_integral);
}

void qIntegralVoid(double left_, double right_, double f_left, double f_right, double intgrl_now, double *res) {
    *res = qIntegral(left_, right_, f_left, f_right, intgrl_now);
}

double getResult() {
    auto n = std::thread::hardware_concurrency();

    auto len = (right - left) / n;

    std::vector<std::thread> threads;
    std::vector<double> part_res;
    part_res.resize(n);

    double result = 0;
    for (auto i = 0; i < n; ++i) {
        threads.emplace_back(qIntegralVoid, left + len * i, left + len * (i + 1), func(left + len * i),
                             func(left + len * (i + 1)), (func(left + len * i) + func(left + len * (i + 1))) * len / 2,
                             &part_res[i]);
    }

    for (std::thread& th : threads) {
        th.join();
    }

    for (auto res : part_res) {
        result += res;
    }
    return result;
}

double checker() {
    return (right - cos(5 * right) / 5) - (left - cos(5 * left) / 5);
}