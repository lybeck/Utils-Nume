#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "nr.h"
#include "solve.h"
#include "myplot.h"
#include "mutils.h"
#include "matutl02.h"

using namespace std;

int main() {
    double start = -1;
    double end = 1;

    myplot::surf_data_set container;
    myplot::surf_data surface1("-");
    myplot::surf_data surface2("-");
    myplot::surf_data dots("+");
    
    myplot::generate_data([] (double x, double y) {return abs(y/(abs(x) + 1e-4));}, container, surface1, 
            start, end, start, end);
    myplot::generate_data([] (double x, double y) {return abs(x/(abs(y) + 1e-4));}, container, surface2, 
            start, end, start, end);
    for (int i = 0; i < 20; i++) {
        dots.add_point(0, 0, i*2);
    }
    
    container.push_back(dots);
    myplot::surf(container, "Testing mysurf");

}
