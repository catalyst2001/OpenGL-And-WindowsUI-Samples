//-----------------------------------------------------------------------------
// Copyright (c) 2008 dhpoware. All Rights Reserved.
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

#include <cassert>
#include "GL_ARB_multitexture.h"

namespace
{
    template <typename T>
    inline void LoadEntryPoint(const char *pszName, T &pfnEntryPoint)
    {
        pfnEntryPoint = reinterpret_cast<T>(wglGetProcAddress(pszName));
        assert(pfnEntryPoint != 0);
    }
}

void glActiveTextureARB(GLenum texture)
{
    typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC)(GLenum texture);
    static PFNGLACTIVETEXTUREARBPROC pfnActiveTextureARB = 0;
    LoadEntryPoint("glActiveTextureARB", pfnActiveTextureARB);
    pfnActiveTextureARB(texture);
}

void glClientActiveTextureARB(GLenum texture)
{
    typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC)(GLenum texture);
    static PFNGLCLIENTACTIVETEXTUREARBPROC pfnClientActiveTextureARB = 0;
    LoadEntryPoint("glClientActiveTextureARB", pfnClientActiveTextureARB);
    pfnClientActiveTextureARB(texture);
}

void glMultiTexCoord1dARB(GLenum target, GLdouble s)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD1DARBPROC)(GLenum target, GLdouble s);
    static PFNGLMULTITEXCOORD1DARBPROC pfnMultiTexCoord1dARB = 0;
    LoadEntryPoint("glMultiTexCoord1dARB", pfnMultiTexCoord1dARB);
    pfnMultiTexCoord1dARB(target, s);
}

void glMultiTexCoord1dvARB(GLenum target, const GLdouble *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD1DVARBPROC)(GLenum target, const GLdouble *v);
    static PFNGLMULTITEXCOORD1DVARBPROC pfnMultiTexCoord1dvARB = 0;
    LoadEntryPoint("glMultiTexCoord1dvARB", pfnMultiTexCoord1dvARB);
    pfnMultiTexCoord1dvARB(target, v);
}

void glMultiTexCoord1fARB(GLenum target, GLfloat s)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD1FARBPROC)(GLenum target, GLfloat s);
    static PFNGLMULTITEXCOORD1FARBPROC pfnMultiTexCoord1fARB = 0;
    LoadEntryPoint("glMultiTexCoord1fARB", pfnMultiTexCoord1fARB);
    pfnMultiTexCoord1fARB(target, s);
}

void glMultiTexCoord1fvARB(GLenum target, const GLfloat *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD1FVARBPROC)(GLenum target, const GLfloat *v);
    static PFNGLMULTITEXCOORD1FVARBPROC pfnMultiTexCoord1fvARB = 0;
    LoadEntryPoint("glMultiTexCoord1fvARB", pfnMultiTexCoord1fvARB);
    pfnMultiTexCoord1fvARB(target, v);
}

void glMultiTexCoord1iARB(GLenum target, GLint s)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD1IARBPROC)(GLenum target, GLint s);
    static PFNGLMULTITEXCOORD1IARBPROC pfnMultiTexCoord1iARB = 0;
    LoadEntryPoint("glMultiTexCoord1iARB", pfnMultiTexCoord1iARB);
    pfnMultiTexCoord1iARB(target, s);
}

void glMultiTexCoord1ivARB(GLenum target, const GLint *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD1IVARBPROC)(GLenum target, const GLint *v);
    static PFNGLMULTITEXCOORD1IVARBPROC pfnMultiTexCoord1ivARB = 0;
    LoadEntryPoint("glMultiTexCoord1ivARB", pfnMultiTexCoord1ivARB);
    pfnMultiTexCoord1ivARB(target, v);
}

void glMultiTexCoord1sARB(GLenum target, GLshort s)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD1SARBPROC)(GLenum target, GLshort s);
    static PFNGLMULTITEXCOORD1SARBPROC pfnMultiTexCoord1sARB = 0;
    LoadEntryPoint("glMultiTexCoord1sARB", pfnMultiTexCoord1sARB);
    pfnMultiTexCoord1sARB(target, s);
}

void glMultiTexCoord1svARB(GLenum target, const GLshort *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD1SVARBPROC)(GLenum target, const GLshort *v);
    static PFNGLMULTITEXCOORD1SVARBPROC pfnMultiTexCoord1svARB = 0;
    LoadEntryPoint("glMultiTexCoord1svARB", pfnMultiTexCoord1svARB);
    pfnMultiTexCoord1svARB(target, v);
}

void glMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD2DARBPROC)(GLenum target, GLdouble s, GLdouble t);
    static PFNGLMULTITEXCOORD2DARBPROC pfnMultiTexCoord2dARB = 0;
    LoadEntryPoint("glMultiTexCoord2dARB", pfnMultiTexCoord2dARB);
    pfnMultiTexCoord2dARB(target, s, t);
}

void glMultiTexCoord2dvARB(GLenum target, const GLdouble *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD2DVARBPROC)(GLenum target, const GLdouble *v);
    static PFNGLMULTITEXCOORD2DVARBPROC pfnMultiTexCoord2dvARB = 0;
    LoadEntryPoint("glMultiTexCoord2dvARB", pfnMultiTexCoord2dvARB);
    pfnMultiTexCoord2dvARB(target, v);
}

void glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC)(GLenum target, GLfloat s, GLfloat t);
    static PFNGLMULTITEXCOORD2FARBPROC pfnMultiTexCoord2fARB = 0;
    LoadEntryPoint("glMultiTexCoord2fARB", pfnMultiTexCoord2fARB);
    pfnMultiTexCoord2fARB(target, s, t);
}

void glMultiTexCoord2fvARB(GLenum target, const GLfloat *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD2FVARBPROC)(GLenum target, const GLfloat *v);
    static PFNGLMULTITEXCOORD2FVARBPROC pfnMultiTexCoord2fvARB = 0;
    LoadEntryPoint("glMultiTexCoord2fvARB", pfnMultiTexCoord2fvARB);
    pfnMultiTexCoord2fvARB(target, v);
}

void glMultiTexCoord2iARB(GLenum target, GLint s, GLint t)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD2IARBPROC)(GLenum target, GLint s, GLint t);
    static PFNGLMULTITEXCOORD2IARBPROC pfnMultiTexCoord2iARB = 0;
    LoadEntryPoint("glMultiTexCoord2iARB", pfnMultiTexCoord2iARB);
    pfnMultiTexCoord2iARB(target, s, t);
}

void glMultiTexCoord2ivARB(GLenum target, const GLint *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD2IVARBPROC)(GLenum target, const GLint *v);
    static PFNGLMULTITEXCOORD2IVARBPROC pfnMultiTexCoord2ivARB = 0;
    LoadEntryPoint("glMultiTexCoord2ivARB", pfnMultiTexCoord2ivARB);
    pfnMultiTexCoord2ivARB(target, v);
}

void glMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD2SARBPROC)(GLenum target, GLshort s, GLshort t);
    static PFNGLMULTITEXCOORD2SARBPROC pfnMultiTexCoord2sARB = 0;
    LoadEntryPoint("glMultiTexCoord2sARB", pfnMultiTexCoord2sARB);
    pfnMultiTexCoord2sARB(target, s, t);
}

void glMultiTexCoord2svARB(GLenum target, const GLshort *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD2SVARBPROC)(GLenum target, const GLshort *v);
    static PFNGLMULTITEXCOORD2SVARBPROC pfnMultiTexCoord2svARB = 0;
    LoadEntryPoint("glMultiTexCoord2svARB", pfnMultiTexCoord2svARB);
    pfnMultiTexCoord2svARB(target, v);
}

void glMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD3DARBPROC)(GLenum target, GLdouble s, GLdouble t, GLdouble r);
    static PFNGLMULTITEXCOORD3DARBPROC pfnMultiTexCoord3dARB = 0;
    LoadEntryPoint("glMultiTexCoord3dARB", pfnMultiTexCoord3dARB);
    pfnMultiTexCoord3dARB(target, s, t, r);
}

void glMultiTexCoord3dvARB(GLenum target, const GLdouble *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD3DVARBPROC)(GLenum target, const GLdouble *v);
    static PFNGLMULTITEXCOORD3DVARBPROC pfnMultiTexCoord3dvARB = 0;
    LoadEntryPoint("glMultiTexCoord3dvARB", pfnMultiTexCoord3dvARB);
    pfnMultiTexCoord3dvARB(target, v);
}

void glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD3FARBPROC)(GLenum target, GLfloat s, GLfloat t, GLfloat r);
    static PFNGLMULTITEXCOORD3FARBPROC pfnMultiTexCoord3fARB = 0;
    LoadEntryPoint("glMultiTexCoord3fARB", pfnMultiTexCoord3fARB);
    pfnMultiTexCoord3fARB(target, s, t, r);
}

void glMultiTexCoord3fvARB(GLenum target, const GLfloat *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD3FVARBPROC)(GLenum target, const GLfloat *v);
    static PFNGLMULTITEXCOORD3FVARBPROC pfnMultiTexCoord3fvARB = 0;
    LoadEntryPoint("glMultiTexCoord3fvARB", pfnMultiTexCoord3fvARB);
    pfnMultiTexCoord3fvARB(target, v);
}

void glMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD3IARBPROC)(GLenum target, GLint s, GLint t, GLint r);
    static PFNGLMULTITEXCOORD3IARBPROC pfnMultiTexCoord3iARB = 0;
    LoadEntryPoint("glMultiTexCoord3iARB", pfnMultiTexCoord3iARB);
    pfnMultiTexCoord3iARB(target, s, t, r);
}

void glMultiTexCoord3ivARB(GLenum target, const GLint *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD3IVARBPROC)(GLenum target, const GLint *v);
    static PFNGLMULTITEXCOORD3IVARBPROC pfnMultiTexCoord3ivARB = 0;
    LoadEntryPoint("glMultiTexCoord3ivARB", pfnMultiTexCoord3ivARB);
    pfnMultiTexCoord3ivARB(target, v);
}

void glMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD3SARBPROC)(GLenum target, GLshort s, GLshort t, GLshort r);
    static PFNGLMULTITEXCOORD3SARBPROC pfnMultiTexCoord3sARB = 0;
    LoadEntryPoint("glMultiTexCoord3sARB", pfnMultiTexCoord3sARB);
    pfnMultiTexCoord3sARB(target, s, t, r);
}

void glMultiTexCoord3svARB(GLenum target, const GLshort *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD3SVARBPROC)(GLenum target, const GLshort *v);
    static PFNGLMULTITEXCOORD3SVARBPROC pfnMultiTexCoord3svARB = 0;
    LoadEntryPoint("glMultiTexCoord3svARB", pfnMultiTexCoord3svARB);
    pfnMultiTexCoord3svARB(target, v);
}

void glMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD4DARBPROC)(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
    static PFNGLMULTITEXCOORD4DARBPROC pfnMultiTexCoord4dARB = 0;
    LoadEntryPoint("glMultiTexCoord4dARB", pfnMultiTexCoord4dARB);
    pfnMultiTexCoord4dARB(target, s, t, r, q);
}

void glMultiTexCoord4dvARB(GLenum target, const GLdouble *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD4DVARBPROC)(GLenum target, const GLdouble *v);
    static PFNGLMULTITEXCOORD4DVARBPROC pfnMultiTexCoord4dvARB = 0;
    LoadEntryPoint("glMultiTexCoord4dvARB", pfnMultiTexCoord4dvARB);
    pfnMultiTexCoord4dvARB(target, v);
}

void glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD4FARBPROC)(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
    static PFNGLMULTITEXCOORD4FARBPROC pfnMultiTexCoord4fARB = 0;
    LoadEntryPoint("glMultiTexCoord4fARB", pfnMultiTexCoord4fARB);
    pfnMultiTexCoord4fARB(target, s, t, r, q);
}

void glMultiTexCoord4fvARB(GLenum target, const GLfloat *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD4FVARBPROC)(GLenum target, const GLfloat *v);
    static PFNGLMULTITEXCOORD4FVARBPROC pfnMultiTexCoord4fvARB = 0;
    LoadEntryPoint("glMultiTexCoord4fvARB", pfnMultiTexCoord4fvARB);
    pfnMultiTexCoord4fvARB(target, v);
}

void glMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD4IARBPROC)(GLenum target, GLint s, GLint t, GLint r, GLint q);
    static PFNGLMULTITEXCOORD4IARBPROC pfnMultiTexCoord4iARB = 0;
    LoadEntryPoint("glMultiTexCoord4iARB", pfnMultiTexCoord4iARB);
    pfnMultiTexCoord4iARB(target, s, t, r, q);
}

void glMultiTexCoord4ivARB(GLenum target, const GLint *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD4IVARBPROC)(GLenum target, const GLint *v);
    static PFNGLMULTITEXCOORD4IVARBPROC pfnMultiTexCoord4ivARB = 0;
    LoadEntryPoint("glMultiTexCoord4ivARB", pfnMultiTexCoord4ivARB);
    pfnMultiTexCoord4ivARB(target, v);
}

void glMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD4SARBPROC)(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
    static PFNGLMULTITEXCOORD4SARBPROC pfnMultiTexCoord4sARB = 0;
    LoadEntryPoint("glMultiTexCoord4sARB", pfnMultiTexCoord4sARB);
    pfnMultiTexCoord4sARB(target, s, t, r, q);
}

void glMultiTexCoord4svARB(GLenum target, const GLshort *v)
{
    typedef void (APIENTRY * PFNGLMULTITEXCOORD4SVARBPROC)(GLenum target, const GLshort *v);
    static PFNGLMULTITEXCOORD4SVARBPROC pfnMultiTexCoord4svARB = 0;
    LoadEntryPoint("glMultiTexCoord4svARB", pfnMultiTexCoord4svARB);
    pfnMultiTexCoord4svARB(target, v);
}