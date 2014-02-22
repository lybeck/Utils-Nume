/* 
 * File:   mypoly.h
 * Author: Lasse Lybeck
 */

#ifndef MYPOLY_H
#define	MYPOLY_H

#include <iostream>
#include <vector>

namespace mypoly {

    typedef std::vector<double> poly_coeffs;
    
    class poly {
    public:
        poly(poly const&);
        poly(poly_coeffs const&);
        ~poly();
        double operator() (double) const;
        unsigned int deg() const;
        poly_coeffs& get_coefficients();
        poly_coeffs const& get_coefficients() const;
        friend std::ostream& operator<<(std::ostream&, poly const&);
    private:
        poly_coeffs coefficients;
    };
}

#endif	/* MYPOLY_H */

