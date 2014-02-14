/* 
 * File:   eps.cpp
 * Author: Lasse Lybeck
 */

#include <cstdlib>
#include <cmath>
#include "eps.h"

double calculate_machine_epsilon() {
    double epsilon = 1.0;
    while (epsilon / 2.0 + 1.0 != 1.0) {
        epsilon /= 2.0;
    }
    return epsilon;
}

double get_machine_epsilon() {
    static double EPS = -1.0;
    if (EPS < 0.0)
        EPS = calculate_machine_epsilon();
    return EPS;
}

double get_sqrt_machine_epsilon() {
    return sqrt(get_machine_epsilon());
}

