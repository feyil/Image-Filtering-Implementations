
#include "filter.h"

#include <iostream>
#include <cmath>

namespace ceng391 {

    Filter1D::Filter1D(int size) {
        m_size = size;
        m_filter = new double[size];
    }

    Filter1D::~Filter1D() {
        delete [] m_filter;
    }

    Filter1D* Filter1D::create_box_filter(int n) {
        Filter1D* filter = new Filter1D(n);

        // Initialize the filter
        double* filter_data = filter->get_filter();
        for(int i = 0; i < filter->get_size(); i++) {
                filter_data[i] = 1.0f / n;
   
        }
        
        return filter;
    }

    Filter1D* Filter1D::create_gaussian_filter(float sigma) {
        int l = ceil(sigma * 2);
        int filter_size = 2 * l + 1;
     
        Filter1D* filter = new Filter1D(filter_size);
        double* filter_data = filter->get_filter();

        double sum = 0;
        int counter = 0;
        int x_start = -1 * (filter_size - 1) / 2.0f; 
        for(int x = x_start; x < -1 * x_start + 1; x++) {
                double gaussian = exp(-0.5 * (pow(x, 2) / pow(sigma, 2)));
                sum += gaussian;
            
                filter_data[counter] = gaussian;
                counter += 1;
        }

        // Normalization
        for(int i = 0; i < filter_size; i++) {
                filter_data[i] = filter_data[i] / sum;
        }

        return filter;
    }

    Filter1D* Filter1D::create_derivative_filter() {

        Filter1D* filter = new Filter1D(3);
        double* filter_data = filter->get_filter();

        filter_data[0] = -1;
        filter_data[1] = 0;
        filter_data[2] = 1;

        return filter;
    }

    double Filter1D::value_at(int index) {
        return m_filter[index];
    }

    int Filter1D::get_size() {
        return m_size;
    }

    double* Filter1D::get_filter() {
        return m_filter;
    }

}