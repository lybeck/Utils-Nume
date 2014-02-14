/* 
 * File:   myplot.h
 * Author: Lasse Lybeck
 */

#ifndef MYPLOT_H
#define	MYPLOT_H

#include <iostream>
#include <vector>

namespace myplot {

    class point2d {
    public:
        point2d(double, double);
        point2d(point2d const&);
        ~point2d();
        friend std::ostream& operator<<(std::ostream&, point2d const&);
    private:
        double x, y;
    };

    class plot_data {
    public:
        plot_data();
        plot_data(std::string const&);
        plot_data(plot_data const&);
        ~plot_data();
        void add_point(double, double);
        int size() const;
        bool empty() const;
        void set_style(std::string const&);
        char const* get_style() const;
        double get_min_x() const;
        double get_max_x() const;
        double get_min_y() const;
        double get_max_y() const;
        plot_data operator=(plot_data const&);
        friend std::ostream& operator<<(std::ostream&, plot_data const&);
    private:
        std::vector<point2d> points;
        double min_x, max_x, min_y, max_y;
        std::string style;
    };
    
    typedef std::vector<plot_data> data_set;

    void plot(data_set data);

}

#endif	/* MYPLOT_H */

