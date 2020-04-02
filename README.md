# Image Filtering Implementations

### Introduction

* This repo includes my solution of the given homework(5/2) in the scope of the Introduction to Image Understanding(CENG391) course which is given as a technical elective in 2019-2020 Fall semester by Computer Engineering Department at Izmir Institute of Technology.

* (*)README.md file uses some parts of the official Homework Doc to better express the purpose of the Homework.

* All solutions implemented on top of base code.

### Problem*

#### Exercise 1 

* Write a new member function Image::box filter x.
* Write a new member function Image::box_filter_y.
* Write a new member function Image::box_filter.

#### Exercise 2

* Write a new member function Image::smooth_x which applies a gaussian filter. Assume that the filter coefficients are zero after two standard deviations.
* Write a new member function Image::smooth_y that performs the same operation in the y direction.
* Write a new member function Image::smooth that performs Gaussian Smoothing both in the x and y directions with standard deviations sigma_x and sigma_y.


#### Exercise 3

* Write a new member function Image::deriv_x that takes computes the image derivative in the x direction using a filter of the form [-1, 0, 1].
* Write a new member function Image::deriv_y that takes computes the image derivative in the y direction using a filter of the form [-1, 0, 1]^T.
* Both results should be returned in a newly allocated array of type short which can store negative values.

### Implementation and Result Showcase

* I have prepared a sh script to add small automation to my compile process. Therefore you can compile given files with this script. Compilation process uses valgrind to check and find any memory leak my occur and this check takes time to complete.If you want you can disable it inside of the compile.sh file.

* I have implemented methods in the problem statement and some others to help me to solve the problems more effectively.

#### Setup

* When you compile with compile.sh it runs the program once with valgrind. After that you can also run it yourself.

```bash
$ sh compile
$ ./build/image-test
```

* You can play with image_test.cc to get different results. However don't forget to compile whenever you made a change ;)

```C++
int main(int argc, char** argv)
{
     Image* img = new Image(4, 4, 1);
     img->read_pnm("small_watch.pgm");
     // img.to_rgb();
     img->smooth(2.0f);
     img->write_pnm("test_image_gray2rgb");

     delete img;

     return EXIT_SUCCESS;
}

```

![alt text](https://github.com/feyil/Image-Filtering-Implementations/blob/master/screenshots/smooth.png "smooth")
