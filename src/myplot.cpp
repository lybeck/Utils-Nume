/* 
 * File:   myplot.cpp
 * Author: Lasse Lybeck
 */

#include <cstdarg>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include "myplot.h"

namespace myplot {

    static const std::string default_style = "b-";

    point2d::point2d(double x, double y) : x(x), y(y) {
    }

    point2d::point2d(const point2d& other) : x(other.x), y(other.y) {
    }

    point2d::~point2d() {
    }

    std::ostream& operator<<(std::ostream& stream, point2d const& p) {
        stream << p.x << " " << p.y;
        return stream;
    }

    plot_data::plot_data() : points(), style(default_style), min_x(INFINITY), max_x(-INFINITY), min_y(INFINITY), max_y(-INFINITY) {
    }

    plot_data::plot_data(const std::string& style) : points(), style(style), min_x(INFINITY), max_x(-INFINITY), min_y(INFINITY), max_y(-INFINITY) {
    }

    plot_data::plot_data(const plot_data& other) : points(other.points), style(other.style), min_x(other.min_x), max_x(other.max_x), min_y(other.min_y), max_y(other.max_y) {
    }

    plot_data::~plot_data() {
    }

    void plot_data::add_point(double x, double y) {
        if (x < min_x)
            min_x = x;
        if (x > max_x)
            max_x = x;
        if (y < min_y)
            min_y = y;
        if (y > max_y)
            max_y = y;
        points.push_back(point2d(x, y));
    }
    
    int plot_data::size() const {
        return points.size();
    }
    
    bool plot_data::empty() const {
        return points.empty();
    }

    void plot_data::set_style(const std::string& s) {
        style = s;
    }
    
    char const* plot_data::get_style() const {
        return style.c_str();
    }

    double plot_data::get_min_x() const {
        return min_x;
    }

    double plot_data::get_max_x() const {
        return max_x;
    }

    double plot_data::get_min_y() const {
        return min_y;
    }

    double plot_data::get_max_y() const {
        return max_y;
    }

    plot_data plot_data::operator=(plot_data const& other) {
        return plot_data(other);
    }

    std::ostream& operator<<(std::ostream& stream, plot_data const& data) {
        for (int i = 0; i < data.size(); i++) {
            stream << data.points[i];
            if (i < data.size() - 1)
                stream << std::endl;
        }
        return stream;
    }
    
    static void unpacksty(const char *style, char *ret);

    void plot(data_set data) {
        
        if(data.empty()) {
            std::cerr << "Nothing to plot..." << std::endl;
            return;
        }
        
        double min_x = INFINITY;
        double max_x = -INFINITY;
        double min_y = INFINITY;
        double max_y = -INFINITY;
        
        std::ofstream plotfile("plot.cmd"), mnmxfile("mnmx.dat");
        plotfile << "set timestamp" << std::endl;
        plotfile << "set grid" << std::endl;
        plotfile << "plot 'mnmx.dat' notitle w dots";

        for (int i = 0; i < data.size(); i++) {
            std::stringstream sstream;
            sstream << "data" << i << ".tmp";
            std::string filename = sstream.str();
            if (data[i].get_min_x() < min_x)
                min_x = data[i].get_min_x();
            if (data[i].get_max_x() > max_x)
                max_x = data[i].get_max_x();
            if (data[i].get_min_y() < min_y)
                min_y = data[i].get_min_y();
            if (data[i].get_max_y() > max_y)
                max_y = data[i].get_max_y();
            std::ofstream datafile(filename);
            datafile << data[i];
            datafile.close();
            char ret[20];
            unpacksty(data[i].get_style(), ret);
            plotfile << ", '" << filename << "' notitle w " << ret;
        }
        
        plotfile << std::endl;
        plotfile << "pause -1" << std::endl;
        
        mnmxfile << max_x + std::abs(0.08 * (max_x - min_x)) << " " << max_y + std::abs(0.08 * (max_y - min_y)) << std::endl;
        mnmxfile << min_x - std::abs(0.08 * (max_x - min_x)) << " " << min_y - std::abs(0.08 * (max_y - min_y)) << std::endl;

        plotfile.close();
        mnmxfile.close();
        
        system("gnuplot plot.cmd");
    }

    static void unpacksty(const char *style, char *ret) {
        int i;
        char st = 'l', pt = '1', lt = '1', siz = '1';
        i = 0;
        while (style[i] != '\0') {
            switch (style[i]) {
                case 'y': {lt='7'; break;}
                case 'm': {lt='4'; break;}
                case 'c': {lt='5'; break;}
                case 'r': {lt='1'; break;}
                case 'g': {lt='2'; break;}
                case 'b': {lt='3'; break;}
                case 'w': {lt='0'; break;}
                case 'k': {lt='0'; break;}
                case '0': {siz='0'; break;}
                case '1': {siz='1'; break;}
                case '2': {siz='2'; break;}
                case '3': {siz='3'; break;}
                case '4': {siz='4'; break;}
                case '5': {siz='5'; break;}
                case '6': {siz='6'; break;}
                case '7': {siz='7'; break;}
                case '8': {siz='8'; break;}
                case '9': {siz='9'; break;}
                case '.': {st='p'; break;}
                case 'o': {st='p'; pt='1'; break;}
                case 'x': {st='p'; pt='4'; break;}
                case '+': {st='p'; pt='8'; break;}
                case '*': {st='p'; pt='6'; break;}
                case 's': {st='p'; pt='3'; break;}
                case 'd': {st='p'; pt='1'; break;}
                case '^': {st='p'; pt='5'; break;}
                case '-': {st='l'; break;}
                default : break;
            }
            ++i;
        }
        i = 0;
        ret[0] = st;
        ret[1] = ' ';
        ret[2] = 'l';
        ret[3] = 't';
        ret[4] = ' ';
        ret[5] = lt;
        ret[6] = ' ';
        if (st == 'p') {
            ret[7] = 'p';
            ret[8] = 't';
            ret[9] = ' ';
            ret[10] = pt;
            ret[11] = ' ';
            ret[12] = 'p';
            ret[13] = 's';
            ret[14] = ' ';
            i = 5;
        } else {
            ret[7] = 'l';
            ret[8] = 'w';
            ret[9] = ' ';
        }
        ret[10 + i] = siz;
        ret[11 + i] = '\0';
    }
}

