//-----------------------------------------------------------------------------
// Copyright (c) 2005-2006 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------
//
// This is a stripped down version of the dhpoware 3D Math Library. To download
// the full version visit: http://www.dhpoware.com/source/mathlib.html
//
//-----------------------------------------------------------------------------

#include "mathlib.h"

const float Math::PI = 3.1415926f;
const float Math::EPSILON = 1e-6f;

const Matrix4 Matrix4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f);

void Matrix4::rotate(const Vector3 &axis, float degrees)
{
    // Creates a rotation matrix about the specified axis.
    // The axis must be a unit vector. The angle must be in degrees.
    //
    // Let u = axis of rotation = (x, y, z)
    //
    //             | x^2(1 - c) + c  xy(1 - c) + zs  xz(1 - c) - ys   0 |
    // Ru(angle) = | yx(1 - c) - zs  y^2(1 - c) + c  yz(1 - c) + xs   0 |
    //             | zx(1 - c) - ys  zy(1 - c) - xs  z^2(1 - c) + c   0 |
    //             |      0              0                0           1 |
    //
    // where,
    //	c = cos(angle)
    //  s = sin(angle)

    degrees = Math::degreesToRadians(degrees);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float c = cosf(degrees);
    float s = sinf(degrees);

    mtx[0][0] = (x * x) * (1.0f - c) + c;
    mtx[0][1] = (x * y) * (1.0f - c) + (z * s);
    mtx[0][2] = (x * z) * (1.0f - c) - (y * s);
    mtx[0][3] = 0.0f;

    mtx[1][0] = (y * x) * (1.0f - c) - (z * s);
    mtx[1][1] = (y * y) * (1.0f - c) + c;
    mtx[1][2] = (y * z) * (1.0f - c) + (x * s);
    mtx[1][3] = 0.0f;

    mtx[2][0] = (z * x) * (1.0f - c) + (y * s);
    mtx[2][1] = (z * y) * (1.0f - c) - (x * s);
    mtx[2][2] = (z * z) * (1.0f - c) + c;
    mtx[2][3] = 0.0f;

    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = 0.0f;
    mtx[3][3] = 1.0f;
}