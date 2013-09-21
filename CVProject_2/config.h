/*
 *  CS 6643   Fall  2012
 *  Project 2:  Image Segmentation
 *  Programmer: Ching-Che Chang
 *  Description:
 *      This program is designed for image segmentation in the color space with iterating K-mean algorithm
 *      and built on the environment of Mac OS X Mountain Lion with the tool of Xcode. Since Mac OS X is an
 *      Unix-like OS, the program is compatible with the OS like Unix and Linux if using the same compilers.
 *      This program is compiled by GNU compiler collection (GCC) of the latest version 4.7.2, which is suitable
 *      for operating systems, such as Unix, Linux, and Mac OS X. Once you install the compiler, you will be
 *      able to compile and run this program on your computer with those operating systems listed above.
 *
 */

#ifndef CVProject_2_config_h
#define CVProject_2_config_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;

struct coordinate {
    int r;
    int c;
};

struct color {
    int R;
    int G;
    int B;
};

struct pixel {
    coordinate position;
    color color_space;
    int group;
};

struct mean {
    int r;
    int g;
    int b;
};

struct tcolor {
    int tr;
    int tg;
    int tb;
};

struct tdist {
    int tdr;
    int tdg;
    int tdb;
    int original;
    int normal;
    int n_members;
};

#endif
