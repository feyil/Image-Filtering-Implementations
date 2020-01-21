// ------------------------------
// Written by Mustafa Ozuysal
// Contact <mustafaozuysal@iyte.edu.tr> for comments and bug reports
// ------------------------------
// Copyright (c) 2018, Mustafa Ozuysal
// All rights reserved.
// ------------------------------
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holders nor the
//       names of his/its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// ------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ------------------------------
#include "image.h"

#include <iostream>
#include <fstream>
#include <cmath>

using std::ofstream;
using std::ifstream;
using std::ios;
using std::cerr;
using std::clog;
using std::endl;
using std::string;
using std::cos;
using std::sin;


namespace ceng391 {

Image::Image(int width, int height, int n_channels, int step)
{
        m_width = width;
        m_height = height;
        m_n_channels = n_channels;

        m_step = m_width*m_n_channels;
        if (m_step < step)
                m_step = step;
        m_data = new uchar[m_step*height];
}

Image::~Image()
{
        delete [] m_data;
}

Image* Image::new_gray(int width, int height)
{
        return new Image(width, height, 1);
}

Image* Image::new_rgb(int width, int height)
{
        return new Image(width, height, 3);
}

void Image::set_rect(int x, int y, int width, int height, uchar red, uchar green, uchar blue)
{
        if (x < 0) {
                width += x;
                x = 0;
        }

        if (y < 0) {
                height += y;
                y = 0;
        }

        if (m_n_channels == 1) {
                int value = 0.3*red + 0.59*green + 0.11*blue;
                if (value > 255)
                        value = 255;
                for (int j = y; j < y+height; ++j) {
                        if (j >= m_height)
                                break;
                        uchar* row_data = data(j);
                        for (int i = x; i < x+width; ++i) {
                                if (i >= m_width)
                                        break;
                                row_data[i] = value;
                        }
                }
        } else if (m_n_channels == 3) {
                for (int j = y; j < y+height; ++j) {
                        if (j >= m_height)
                                break;
                        uchar* row_data = data(j);
                        for (int i = x; i < x+width; ++i) {
                                if (i >= m_width)
                                        break;
                                row_data[i*3]     = red;
                                row_data[i*3 + 1] = green;
                                row_data[i*3 + 2] = blue;
                        }
                }
        }
}

void Image::set_rect(int x, int y, int width, int height, uchar value)
{
        if (x < 0) {
                width += x;
                x = 0;
        }

        if (y < 0) {
                height += y;
                y = 0;
        }

        for (int j = y; j < y+height; ++j) {
                if (j >= m_height)
                        break;
                uchar* row_data = data(j);
                for (int i = x; i < x+width; ++i) {
                        if (i >= m_width)
                                break;
                        for (int c = 0; c < m_n_channels; ++c)
                                row_data[i*m_n_channels + c] = value;
                }
        }
}

void Image::to_grayscale()
{
        if (m_n_channels == 1) {
                return;
        } else if (m_n_channels == 3) {
                int new_step = m_width;
                uchar *new_data = new uchar[new_step * m_height];
                for (int y = 0; y < m_height; ++y) {
                        uchar *row_old = m_data + m_step * y;
                        uchar *row_new = new_data + new_step * y;
                        for (int x = 0; x < m_width; ++x) {
                                uchar red = row_old[3*x];
                                uchar green = row_old[3*x + 1];
                                uchar blue = row_old[3*x + 2];
                                int value = 0.3*red + 0.59*green + 0.11*blue;
                                if (value > 255)
                                        value = 255;
                                row_new[x] = value;
                        }
                }

                delete [] m_data;
                m_data = new_data;
                m_step = new_step;
                m_n_channels = 1;
        }
}

void Image::to_rgb()
{
        if (m_n_channels == 3) {
                return;
        } else if (m_n_channels == 1) {
                int new_step = m_width * 3;
                uchar *new_data = new uchar[new_step * m_height];
                for (int y = 0; y < m_height; ++y) {
                        uchar *row_old = m_data + m_step * y;
                        uchar *row_new = new_data + new_step * y;
                        for (int x = 0; x < m_width; ++x) {
                                uchar value = row_old[x];
                                row_new[3*x]     = value;
                                row_new[3*x + 1] = value;
                                row_new[3*x + 2] = value;
                        }
                }

                delete [] m_data;
                m_data = new_data;
                m_step = new_step;
                m_n_channels = 3;
        }
}

bool Image::write_pnm(const std::string& filename) const
{
        ofstream fout;

        string magic_head = "P5";
        string extended_name = filename + ".pgm";
        if (m_n_channels == 3) {
                magic_head = "P6";
                extended_name = filename + ".ppm";
        }

        fout.open(extended_name.c_str(), ios::out | ios::binary);
        if (!fout.good()) {
                cerr << "Error opening file " << extended_name << " for output!" << endl;
                return false;
        }

        fout << magic_head << "\n";
        fout << m_width << " " << m_height << " 255\n";
        for (int y = 0; y < m_height; ++y) {
                const uchar *row_data = data(y);
                fout.write(reinterpret_cast<const char*>(row_data), m_width*m_n_channels*sizeof(uchar));
        }
        fout.close();

        return true;
}

bool Image::read_pnm(const std::string& filename)
{
        ifstream fin(filename.c_str(), ios::in | ios::binary);
        if (!fin.good()) {
                cerr << "Error opening PNM file " << filename << endl;
                return false;
        }

        int width;
        int height;
        int max_val;
        int n_channels = 1;
        string head = "00";
        head[0] = fin.get();
        head[1] = fin.get();
        if (head == "P5") {
                clog << "Loading PGM Binary" << endl;
                n_channels = 1;
        } else if (head == "P6") {
                clog << "Loading PPM Binary" << endl;
                n_channels = 3;
        } else {
                cerr << "File " << filename << " is not a Binary PGM or PPM!" << endl;
                return false;
        }

        fin >> width;
        fin >> height;
        fin >> max_val;
        if (fin.peek() == '\n')
                fin.get();

        int step = width * n_channels;
        uchar *new_data = new uchar[step*height];
        for (int y = 0; y < height; ++y) {
                fin.read(reinterpret_cast<char*>(new_data + y*step), step*sizeof(uchar));
        }
        fin.close();

        delete [] m_data;
        m_data = new_data;
        m_width = width;
        m_height = height;
        m_step = step;
        m_n_channels = n_channels;

        return true;
}

void Image::rotate(Image *rotated, double theta, double tx, double ty) const
{
        if (m_n_channels != 1) {
                cerr << "Rotate only works on grayscale images!" << endl;
                return;
        }
        rotated->to_grayscale();

        double ct = cos(theta);
        double st = sin(theta);
        double tx_inv = -ct * tx + st * ty;
        double ty_inv = -st * tx - ct * ty;

        int wp = rotated->w();
        int hp = rotated->h();

        for (int yp = 0; yp < hp; ++yp) {
                uchar *row_p = rotated->data(yp);
                for (int xp = 0; xp < wp; ++xp) {
                        double x = ct * xp - st * yp + tx_inv;
                        double y = st * xp + ct * yp + ty_inv;

                        int x0 = static_cast<int>(x);
                        int y0 = static_cast<int>(y);

                        int value = 0;
                        if (x0 < 0 || y0 < 0 || x0 >= m_width || y0 >= m_height) {
                                value = 0;
                        } else {
                                const uchar *row = this->data(y0);
                                value = row[x0];
                        }

                        row_p[xp] = value;
                }
        }
}

void Image::rotate_centered(Image *rotated, double theta) const
{
        double ct = cos(theta);
        double st = sin(theta);
        double hw = m_width / 2.0;
        double hh = m_height / 2.0;
        double hwp = rotated->w() / 2.0;
        double hhp = rotated->h() / 2.0;

        double tx_cap = -ct * hw - st * hh + hwp;
        double ty_cap =  st * hw - ct * hh + hhp;
        this->rotate(rotated, theta, tx_cap, ty_cap);
}

double Image::convolveAt(Buffer* buffer, Filter1D* filter, int convolve_at_index) {

        int filter_size = filter->get_size();
        int filter_center_index = (filter_size - 1) / 2;
        int filter_tail = filter_size - filter_center_index;

        double convolution_result = 0;

        int sign = 1;
        for(int i = 0; i < 2; i++) {

                for(int j = 1; j < filter_tail; j++) {
                        int offset = sign * j;
                     
                        convolution_result += filter->value_at(filter_center_index + offset) 
                                                * buffer->value_at(convolve_at_index + offset);
                       
                }

                sign = -1;
        }
        convolution_result += filter->value_at(filter_center_index) * buffer->value_at(convolve_at_index);
       
        return convolution_result;
}

void Image::fill_buffer_with_row_data(Buffer* buffer, uchar* row_data) {

        uchar* buffer_data = buffer->get_buffer();

        if(m_width == buffer->get_size()) {
                
                for(int x = 0; x < m_width; x++) {
                        buffer_data[x] = row_data[x];
                }
        } 
        else {
                cerr<<"Mismatched Buffer"<<endl;
        }
}

double Image::check_boundary_values(double value) {
        if(value > 255) {
                return 255;
        }
        else if(value < 0) {
                return 0;
        }

        return value;
}

void Image::fill_buffer_with_column_data(Buffer* buffer, int column) {

        uchar* buffer_data = buffer->get_buffer();

        if(m_height == buffer->get_size()) {

             for(int y = 0; y < m_height; y++) {
                        uchar* row_data = data(y);
                   
                        //row_data[x] = column value
                        buffer_data[y] = row_data[column];
                }  
        } 
        else {
                cerr<<"Mismatched Buffer"<<endl;
        }  
}

void Image::filter_x(Filter1D* filter) {
        Buffer* buffer = new Buffer(w());
        
        for(int y = 0; y < m_height; y++) {
                uchar* row_data = data(y);
                fill_buffer_with_row_data(buffer, row_data);
                
                for(int i = 0; i < buffer->get_size(); i++) {
                        double value = convolveAt(buffer, filter, i);
                        value = check_boundary_values(value);

                        row_data[i] = value;
                }
        }

        delete buffer;
}

void Image::filter_y(Filter1D* filter) {
        Buffer* buffer = new Buffer(h());

        for(int x = 0; x < m_width; x++) {

                fill_buffer_with_column_data(buffer, x);

                // buffer->get_size() == m_height
                for(int i = 0; i < buffer->get_size(); i++) {
                        double value = convolveAt(buffer, filter, i);
                        value = check_boundary_values(value);
                        
                        //row_data[x] = column value
                        uchar* row_data = data(i);
                        row_data[x] = value;
                }
        }
        
        delete buffer;
}

void Image::box_filter_x(int n) {

        Filter1D* filter = Filter1D::create_box_filter(n);
        filter_x(filter);
        
        delete filter;
 }

void Image::box_filter_y(int n) {

        Filter1D* filter = Filter1D::create_box_filter(n);
        filter_y(filter);

        delete filter;
}

void Image::box_filter(int n) {
        box_filter_x(n);
        box_filter_y(n);
}

void Image::smooth_x(float sigma) {

        Filter1D* filter = Filter1D::create_gaussian_filter(sigma);
        filter_x(filter);

        delete filter;
}

void Image::smooth_y(float sigma) {

        Filter1D* filter = Filter1D::create_gaussian_filter(sigma);
        filter_y(filter);

        delete filter;
}

void Image::smooth(float sigma) {
        smooth_x(sigma);
        smooth_y(sigma);
}

short* Image::deriv_x() {
        // nearly copy of filter_x, filter_x can support this kind of approach as well.
        // with result destination parameter.
        Filter1D* filter = Filter1D::create_derivative_filter();
        short* deriv_data = new short[m_step * h()];

        Buffer* buffer = new Buffer(w());
        
        for(int y = 0; y < m_height; y++) {
                uchar* row_data = data(y);
                short* row_deriv_data = deriv_data + y * m_step;

                fill_buffer_with_row_data(buffer, row_data);
                
                for(int i = 0; i < buffer->get_size(); i++) {
                        double value = convolveAt(buffer, filter, i);
                
                        row_deriv_data[i] = value;
                }
        }

        delete buffer;
        delete filter;

        return deriv_data;
}

short* Image::deriv_y() {
        // nearly copy of filter_y, filter_y can support this kind of approach as well.
        // with result destination parameter.
        Filter1D* filter = Filter1D::create_derivative_filter();
        short* deriv_data = new short[m_step * h()];
        
        Buffer* buffer = new Buffer(h());

        for(int x = 0; x < m_width; x++) {

                fill_buffer_with_column_data(buffer, x);

                // buffer->get_size() == m_height
                for(int i = 0; i < buffer->get_size(); i++) {
                        double value = convolveAt(buffer, filter, i);
                        
                        //row_data[x] = column value
                        short* row_data = deriv_data + i * m_step;
                        row_data[x] = value;
                }
        }
        
        delete buffer;
        delete filter;

        return deriv_data;
}

}
