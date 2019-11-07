
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