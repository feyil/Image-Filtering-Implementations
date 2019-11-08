
#include "filter.h"

#include <iostream>

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