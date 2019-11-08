#ifndef BUFFER_H
#define BUFFER_H

#include "util.h"

namespace ceng391 {

    class Buffer {
        
        public:
            Buffer(int size);
            ~Buffer();

            uchar value_at(int index);
            int get_size();
            uchar* get_buffer();
        
        private:
            int m_size;
            uchar* m_buffer;
    };
}

#endif
