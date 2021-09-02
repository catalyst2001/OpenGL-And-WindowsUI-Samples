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

#if !defined(GL_ARB_MULTITEXTURE_H)
#define GL_ARB_MULTITEXTURE_H

#include <windows.h>
#include <GL/gl.h>

extern "C"
{

// GL_ARB_multitexture

#define GL_TEXTURE0_ARB                   0x84C0
#define GL_TEXTURE1_ARB                   0x84C1
#define GL_TEXTURE2_ARB                   0x84C2
#define GL_TEXTURE3_ARB                   0x84C3
#define GL_TEXTURE4_ARB                   0x84C4
#define GL_TEXTURE5_ARB                   0x84C5
#define GL_TEXTURE6_ARB                   0x84C6
#define GL_TEXTURE7_ARB                   0x84C7
#define GL_TEXTURE8_ARB                   0x84C8
#define GL_TEXTURE9_ARB                   0x84C9
#define GL_TEXTURE10_ARB                  0x84CA
#define GL_TEXTURE11_ARB                  0x84CB
#define GL_TEXTURE12_ARB                  0x84CC
#define GL_TEXTURE13_ARB                  0x84CD
#define GL_TEXTURE14_ARB                  0x84CE
#define GL_TEXTURE15_ARB                  0x84CF
#define GL_TEXTURE16_ARB                  0x84D0
#define GL_TEXTURE17_ARB                  0x84D1
#define GL_TEXTURE18_ARB                  0x84D2
#define GL_TEXTURE19_ARB                  0x84D3
#define GL_TEXTURE20_ARB                  0x84D4
#define GL_TEXTURE21_ARB                  0x84D5
#define GL_TEXTURE22_ARB                  0x84D6
#define GL_TEXTURE23_ARB                  0x84D7
#define GL_TEXTURE24_ARB                  0x84D8
#define GL_TEXTURE25_ARB                  0x84D9
#define GL_TEXTURE26_ARB                  0x84DA
#define GL_TEXTURE27_ARB                  0x84DB
#define GL_TEXTURE28_ARB                  0x84DC
#define GL_TEXTURE29_ARB                  0x84DD
#define GL_TEXTURE30_ARB                  0x84DE
#define GL_TEXTURE31_ARB                  0x84DF
#define GL_ACTIVE_TEXTURE_ARB             0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB      0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB          0x84E2

extern void glActiveTextureARB(GLenum texture);
extern void glClientActiveTextureARB(GLenum texture);
extern void glMultiTexCoord1dARB(GLenum target, GLdouble s);
extern void glMultiTexCoord1dvARB(GLenum target, const GLdouble *v);
extern void glMultiTexCoord1fARB(GLenum target, GLfloat s);
extern void glMultiTexCoord1fvARB(GLenum target, const GLfloat *v);
extern void glMultiTexCoord1iARB(GLenum target, GLint s);
extern void glMultiTexCoord1ivARB(GLenum target, const GLint *v);
extern void glMultiTexCoord1sARB(GLenum target, GLshort s);
extern void glMultiTexCoord1svARB(GLenum target, const GLshort *v);
extern void glMultiTexCoord2dARB(GLenum target, GLdouble s, GLdouble t);
extern void glMultiTexCoord2dvARB(GLenum target, const GLdouble *v);
extern void glMultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t);
extern void glMultiTexCoord2fvARB(GLenum target, const GLfloat *v);
extern void glMultiTexCoord2iARB(GLenum target, GLint s, GLint t);
extern void glMultiTexCoord2ivARB(GLenum target, const GLint *v);
extern void glMultiTexCoord2sARB(GLenum target, GLshort s, GLshort t);
extern void glMultiTexCoord2svARB(GLenum target, const GLshort *v);
extern void glMultiTexCoord3dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r);
extern void glMultiTexCoord3dvARB(GLenum target, const GLdouble *v);
extern void glMultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r);
extern void glMultiTexCoord3fvARB(GLenum target, const GLfloat *v);
extern void glMultiTexCoord3iARB(GLenum target, GLint s, GLint t, GLint r);
extern void glMultiTexCoord3ivARB(GLenum target, const GLint *v);
extern void glMultiTexCoord3sARB(GLenum target, GLshort s, GLshort t, GLshort r);
extern void glMultiTexCoord3svARB(GLenum target, const GLshort *v);
extern void glMultiTexCoord4dARB(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void glMultiTexCoord4dvARB(GLenum target, const GLdouble *v);
extern void glMultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void glMultiTexCoord4fvARB(GLenum target, const GLfloat *v);
extern void glMultiTexCoord4iARB(GLenum target, GLint s, GLint t, GLint r, GLint q);
extern void glMultiTexCoord4ivARB(GLenum target, const GLint *v);
extern void glMultiTexCoord4sARB(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
extern void glMultiTexCoord4svARB(GLenum target, const GLshort *v);

} // extern "C"

#endif