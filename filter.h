#ifndef FILTER_H
#define FILTER_H

namespace ceng391 {

class Filter1D {
    public:
        Filter1D(int size);
        ~Filter1D();

        double value_at(int index);
        int get_size();
        double* get_filter();

    private:
        int m_size;
        double* m_filter;

};

}

#endif
