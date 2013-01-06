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
"__kernel void bench( __global double* vectorA,"
"                     __global double* vectorB,"
"                     __global double* result_vector,"
"                     uint size )"
"{"
"    int i = get_global_id(0);"
"    double a = vectorA[i];"
"    double b = vectorB[i];"
"    double c = a + b;"

"    for( int i = 0; i < 0x1fff; ++i )"
"    {"
"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 1
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 2
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 3
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 4
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 5
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 6
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 7
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 8
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 9
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 10
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 11
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 12
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 13
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 14
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 15
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 16
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 17
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 18
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 19
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 20
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 21
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 22
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 23
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 24
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 25
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 26
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 27
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 28
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 29
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 30
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 31
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 32
"    }"
"    result_vector[i] = c;"
"}"
);
std::string source_generator< cl_float >::source_(
"__kernel void bench( __global float* vectorA,"
"                     __global float* vectorB,"
"                     __global float* result_vector,"
"                     uint size )"
"{"
"    int i = get_global_id(0);"
"    float a = vectorA[i];"
"    float b = vectorB[i];"
"    float c = a + b;"

"    for( int i = 0; i < 0x1ff; ++i )"
"    {"
"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 1
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 2
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 3
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 4
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 5
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 6
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 7
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 8
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 9
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 10
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 11
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 12
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 13
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 14
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 15
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 16
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 17
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 18
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 19
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 20
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 21
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 22
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 23
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 24
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 25
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 26
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 27
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 28
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 29
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 30
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 31
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 32
"    }"
"    result_vector[i] = c;"
"}"
);
std::string source_generator< cl_int >::source_(
"__kernel void bench( __global int* vectorA,"
"                     __global int* vectorB,"
"                     __global int* result_vector,"
"                     uint size )"
"{"
"    int i = get_global_id(0);"
"    int a = vectorA[i];"
"    int b = vectorB[i];"
"    int c = a + b;"

"    for( int i = 0; i < 0x1fff; ++i )"
"    {"
"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 1
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 2
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 3
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 4
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 5
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 6
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 7
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 8
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 9
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 10
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 11
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 12
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 13
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 14
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 15
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 16
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 17
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 18
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 19
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 20
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 21
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 22
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 23
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 24
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 25
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 26
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 27
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 28
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 29
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 30
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 31
//"        c *= b; b *= a; c /= a+b; c*= a-b;c += a/b;c -= a*b;" // 32
"    }"
"    result_vector[i] = c;"
"}"
);
//-----------------------------------------------------------------------------
}} // namespace dcl::benchmark
//-----------------------------------------------------------------------------
