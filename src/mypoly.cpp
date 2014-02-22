
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "mypoly.h"

namespace mypoly {

    poly::poly(const poly& o) : coefficients(o.coefficients) { }

    poly::poly(const poly_coeffs& c) : coefficients(c) {
        if (coefficients.empty())
            throw std::runtime_error("Polynomial coefficients cannot be empty!");
    }

    poly::~poly() { }

    inline
    unsigned int poly::deg() const {
        return coefficients.size() - 1;
    }

    poly_coeffs& poly::get_coefficients() {
        return coefficients;
    }

    poly_coeffs const& poly::get_coefficients() const {
        return coefficients;
    }

    double poly::operator ()(double x) const {
        double sum = 0;
        int n = deg();
        for (int i = 0; i < coefficients.size(); i++) {
            sum += coefficients[i] * pow(x, n - i);
        }
        return sum;
    }

    std::ostream& operator <<(std::ostream& s, const poly& p) {
        int d = p.deg();
        int n;
        for (int i = 0; i < p.coefficients.size(); i++) {
            n = d - i;
            if (i > 0)
                s << (std::signbit(p.coefficients[i]) ? " - " : " + ") << std::abs(p.coefficients[i]);
            else
                s << p.coefficients[i];
            if (n > 0)
                s << "*x";
            if (n > 1)
                s << "^" << n;
        }
        s << std::endl;
        return s;
    }
}
