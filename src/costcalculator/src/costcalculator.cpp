#include "../header/costcalculator.h"
#include <stdexcept>

using namespace Coruh::Costcalculator;

double Costcalculator::add(double a, double b) {
    return a + b;
}

double Costcalculator::subtract(double a, double b) {
    return a - b;
}

double Costcalculator::multiply(double a, double b) {
    return a * b;
}

double Costcalculator::divide(double a, double b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero is not allowed.");
    }
    return a / b;
}