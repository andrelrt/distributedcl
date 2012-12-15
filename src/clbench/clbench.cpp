/*
 * Copyright (c) 2009-2012 André Tupinambá (andrelrt@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
//-----------------------------------------------------------------------------
#include "clbench.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace benchmark {
//-----------------------------------------------------------------------------
std::string source_generator< cl_double >::source_(
"__kernel void bench( __global double* matrixA,"
"                     __global double* matrixB,"
"                     __global double* resultMatrix,"
"                     uint size )"
"{"
"    int x = get_global_id(0);"
"    int y = get_global_id(1);"
"    double sum = 0;"
"    for(int i = 0; i < size; ++i)"
"    {"
"        sum = matrixA[size*y+i] * matrixB[size*i+x];"
"    }"
"    resultMatrix[size*y+x] = sum;"
"}"
);
std::string source_generator< cl_float >::source_(
"__kernel void bench( __global float* matrixA,"
"                     __global float* matrixB,"
"                     __global float* resultMatrix,"
"                     uint size )"
"{"
"    int x = get_global_id(0);"
"    int y = get_global_id(1);"
"    float sum = 0;"
"    for(int i = 0; i < size; ++i)"
"    {"
"        sum = matrixA[size*y+i] * matrixB[size*i+x];"
"    }"
"    resultMatrix[size*y+x] = sum;"
"}"
);
std::string source_generator< cl_int >::source_(
"__kernel void bench( __global int* matrixA,"
"                     __global int* matrixB,"
"                     __global int* resultMatrix,"
"                     uint size )"
"{"
"    int x = get_global_id(0);"
"    int y = get_global_id(1);"
"    int sum = 0;"
"    for(int i = 0; i < size; ++i)"
"    {"
"        sum = matrixA[size*y+i] * matrixB[size*i+x];"
"    }"
"    resultMatrix[size*y+x] = sum;"
"}"
);
//-----------------------------------------------------------------------------
}} // namespace dcl::benchmark
//-----------------------------------------------------------------------------
