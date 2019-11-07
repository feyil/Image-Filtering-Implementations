#include "buffer.h"

#include <iostream>

namespace ceng391 {

    Buffer::Buffer(int size) {
        m_size = size;
        m_buffer = new uchar[size];
    }

    Buffer::~Buffer() {
        delete [] m_buffer;
    }

    int Buffer::value_at(int index) {
        if(index < 0) {
            return m_buffer[0];
        }
        else if(index > m_size) {
            return m_buffer[m_size - 1];
        }
        
        return m_buffer[index];
    }

    int Buffer::get_size() {
        return m_size;
    }

    uchar* Buffer::get_buffer() {
        return m_buffer;
    }

}