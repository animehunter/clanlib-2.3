/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

/// \addtogroup clanGL_System clanGL System
/// \{

#pragma once

#include "opengl.h"
#include "opengl_defines.h"

#ifdef WIN32
#define CL_GLFUNC WINAPI
#else
#define CL_GLFUNC
#endif

/// \name OpenGL 2.0 binds
/// \{

/// \brief CL_GLFunctions
///
/// \xmlonly !group=GL/System! !header=gl.h! \endxmlonly
class CL_GLFunctions
{

/// \name Typedefs
/// \{

public:
	typedef void (CL_GLFUNC *ptr_glCullFace)(GLenum mode);
	typedef void (CL_GLFUNC *ptr_glFrontFace)(GLenum mode);
	typedef void (CL_GLFUNC *ptr_glHint)(GLenum target, GLenum mode);
	typedef void (CL_GLFUNC *ptr_glLineWidth)(GLfloat width);
	typedef void (CL_GLFUNC *ptr_glPointSize)(GLfloat size);
	typedef void (CL_GLFUNC *ptr_glPolygonMode)(GLenum face, GLenum mode);
	typedef void (CL_GLFUNC *ptr_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (CL_GLFUNC *ptr_glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
	typedef void (CL_GLFUNC *ptr_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glTexParameteri)(GLenum target, GLenum pname, GLint param);
	typedef void (CL_GLFUNC *ptr_glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
	typedef void (CL_GLFUNC *ptr_glTexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glDrawBuffer)(GLenum mode);
	typedef void (CL_GLFUNC *ptr_glClear)(GLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	typedef void (CL_GLFUNC *ptr_glClearStencil)(GLint s);
	typedef void (CL_GLFUNC *ptr_glClearDepth)(GLclampd depth);
	typedef void (CL_GLFUNC *ptr_glStencilMask)(GLuint mask);
	typedef void (CL_GLFUNC *ptr_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	typedef void (CL_GLFUNC *ptr_glDepthMask)(GLboolean flag);
	typedef void (CL_GLFUNC *ptr_glDisable)(GLenum cap);
	typedef void (CL_GLFUNC *ptr_glEnable)(GLenum cap);
	typedef void (CL_GLFUNC *ptr_glFinish)(void);
	typedef void (CL_GLFUNC *ptr_glFlush)(void);
	typedef void (CL_GLFUNC *ptr_glBlendFunc)(GLenum sfactor, GLenum dfactor);
	typedef void (CL_GLFUNC *ptr_glLogicOp)(GLenum opcode);
	typedef void (CL_GLFUNC *ptr_glStencilFunc)(GLenum func, GLint ref, GLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
	typedef void (CL_GLFUNC *ptr_glDepthFunc)(GLenum func);
	typedef void (CL_GLFUNC *ptr_glPixelStoref)(GLenum pname, GLfloat param);
	typedef void (CL_GLFUNC *ptr_glPixelStorei)(GLenum pname, GLint param);
	typedef void (CL_GLFUNC *ptr_glReadBuffer)(GLenum mode);
	typedef void (CL_GLFUNC *ptr_glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glGetBooleanv)(GLenum pname, GLboolean *params);
	typedef void (CL_GLFUNC *ptr_glGetDoublev)(GLenum pname, GLdouble *params);
	typedef GLenum (CL_GLFUNC *ptr_glGetError)(void);
	typedef void (CL_GLFUNC *ptr_glGetFloatv)(GLenum pname, GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetIntegerv)(GLenum pname, GLint *params);
	typedef const GLubyte * (CL_GLFUNC *ptr_glGetString)(GLenum name);
	typedef void (CL_GLFUNC *ptr_glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
	typedef GLboolean (CL_GLFUNC *ptr_glIsEnabled)(GLenum cap);
	typedef void (CL_GLFUNC *ptr_glDepthRange)(GLclampd near, GLclampd far);
	typedef void (CL_GLFUNC *ptr_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (CL_GLFUNC *ptr_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
	typedef void (CL_GLFUNC *ptr_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	typedef void (CL_GLFUNC *ptr_glGetPointerv)(GLenum pname, GLvoid* *params);
	typedef void (CL_GLFUNC *ptr_glPolygonOffset)(GLfloat factor, GLfloat units);
	typedef void (CL_GLFUNC *ptr_glCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
	typedef void (CL_GLFUNC *ptr_glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (CL_GLFUNC *ptr_glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glBindTexture)(GLenum target, GLuint texture);
	typedef void (CL_GLFUNC *ptr_glDeleteTextures)(GLsizei n, const GLuint *textures);
	typedef void (CL_GLFUNC *ptr_glGenTextures)(GLsizei n, GLuint *textures);
	typedef GLboolean (CL_GLFUNC *ptr_glIsTexture)(GLuint texture);
	typedef void (CL_GLFUNC *ptr_glBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	typedef void (CL_GLFUNC *ptr_glBlendEquation)(GLenum mode);
	typedef void (CL_GLFUNC *ptr_glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
	typedef void (CL_GLFUNC *ptr_glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
	typedef void (CL_GLFUNC *ptr_glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (CL_GLFUNC *ptr_glActiveTexture)(GLenum texture);
	typedef void (CL_GLFUNC *ptr_glSampleCoverage)(GLclampf value, GLboolean invert);
	typedef void (CL_GLFUNC *ptr_glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
	typedef void (CL_GLFUNC *ptr_glCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
	typedef void (CL_GLFUNC *ptr_glGetCompressedTexImage)(GLenum target, GLint level, GLvoid *img);
	typedef void (CL_GLFUNC *ptr_glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
	typedef void (CL_GLFUNC *ptr_glMultiDrawArrays)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
	typedef void (CL_GLFUNC *ptr_glMultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount);
	typedef void (CL_GLFUNC *ptr_glPointParameterf)(GLenum pname, GLfloat param);
	typedef void (CL_GLFUNC *ptr_glPointParameterfv)(GLenum pname, const GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glPointParameteri)(GLenum pname, GLint param);
	typedef void (CL_GLFUNC *ptr_glPointParameteriv)(GLenum pname, const GLint *params);
	typedef void (CL_GLFUNC *ptr_glGenQueries)(GLsizei n, GLuint *ids);
	typedef void (CL_GLFUNC *ptr_glDeleteQueries)(GLsizei n, const GLuint *ids);
	typedef GLboolean (CL_GLFUNC *ptr_glIsQuery)(GLuint id);
	typedef void (CL_GLFUNC *ptr_glBeginQuery)(GLenum target, GLuint id);
	typedef void (CL_GLFUNC *ptr_glEndQuery)(GLenum target);
	typedef void (CL_GLFUNC *ptr_glGetQueryiv)(GLenum target, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
	typedef void (CL_GLFUNC *ptr_glBindBuffer)(GLenum target, GLuint buffer);
	typedef void (CL_GLFUNC *ptr_glDeleteBuffers)(GLsizei n, const GLuint *buffers);
	typedef void (CL_GLFUNC *ptr_glGenBuffers)(GLsizei n, GLuint *buffers);
	typedef GLboolean (CL_GLFUNC *ptr_glIsBuffer)(GLuint buffer);
	typedef void (CL_GLFUNC *ptr_glBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
	typedef void (CL_GLFUNC *ptr_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
	typedef void (CL_GLFUNC *ptr_glGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
	typedef GLvoid* (CL_GLFUNC *ptr_glMapBuffer)(GLenum target, GLenum access);
	typedef GLboolean (CL_GLFUNC *ptr_glUnmapBuffer)(GLenum target);
	typedef void (CL_GLFUNC *ptr_glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetBufferPointerv)(GLenum target, GLenum pname, GLvoid* *params);
	typedef void (CL_GLFUNC *ptr_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
	typedef void (CL_GLFUNC *ptr_glDrawBuffers)(GLsizei n, const GLenum *bufs);
	typedef void (CL_GLFUNC *ptr_glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
	typedef void (CL_GLFUNC *ptr_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
	typedef void (CL_GLFUNC *ptr_glStencilMaskSeparate)(GLenum face, GLuint mask);
	typedef void (CL_GLFUNC *ptr_glAttachShader)(GLuint program, GLuint shader);
	typedef void (CL_GLFUNC *ptr_glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
	typedef void (CL_GLFUNC *ptr_glCompileShader)(GLuint shader);
	typedef GLuint (CL_GLFUNC *ptr_glCreateProgram)(void);
	typedef GLuint (CL_GLFUNC *ptr_glCreateShader)(GLenum type);
	typedef void (CL_GLFUNC *ptr_glDeleteProgram)(GLuint program);
	typedef void (CL_GLFUNC *ptr_glDeleteShader)(GLuint shader);
	typedef void (CL_GLFUNC *ptr_glDetachShader)(GLuint program, GLuint shader);
	typedef void (CL_GLFUNC *ptr_glDisableVertexAttribArray)(GLuint index);
	typedef void (CL_GLFUNC *ptr_glEnableVertexAttribArray)(GLuint index);
	typedef void (CL_GLFUNC *ptr_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
	typedef GLint (CL_GLFUNC *ptr_glGetAttribLocation)(GLuint program, const GLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	typedef void (CL_GLFUNC *ptr_glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	typedef void (CL_GLFUNC *ptr_glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
	typedef GLint (CL_GLFUNC *ptr_glGetUniformLocation)(GLuint program, const GLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetUniformfv)(GLuint program, GLint location, GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetUniformiv)(GLuint program, GLint location, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid* *pointer);
	typedef GLboolean (CL_GLFUNC *ptr_glIsProgram)(GLuint program);
	typedef GLboolean (CL_GLFUNC *ptr_glIsShader)(GLuint shader);
	typedef void (CL_GLFUNC *ptr_glLinkProgram)(GLuint program);
	typedef void (CL_GLFUNC *ptr_glShaderSource)(GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
	typedef void (CL_GLFUNC *ptr_glUseProgram)(GLuint program);
	typedef void (CL_GLFUNC *ptr_glUniform1f)(GLint location, GLfloat v0);
	typedef void (CL_GLFUNC *ptr_glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
	typedef void (CL_GLFUNC *ptr_glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void (CL_GLFUNC *ptr_glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void (CL_GLFUNC *ptr_glUniform1i)(GLint location, GLint v0);
	typedef void (CL_GLFUNC *ptr_glUniform2i)(GLint location, GLint v0, GLint v1);
	typedef void (CL_GLFUNC *ptr_glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
	typedef void (CL_GLFUNC *ptr_glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void (CL_GLFUNC *ptr_glUniform1fv)(GLint location, GLsizei count, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform2fv)(GLint location, GLsizei count, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform4fv)(GLint location, GLsizei count, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniform1iv)(GLint location, GLsizei count, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform2iv)(GLint location, GLsizei count, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform3iv)(GLint location, GLsizei count, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glUniform4iv)(GLint location, GLsizei count, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glValidateProgram)(GLuint program);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1d)(GLuint index, GLdouble x);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1dv)(GLuint index, const GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1f)(GLuint index, GLfloat x);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1fv)(GLuint index, const GLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1s)(GLuint index, GLshort x);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib1sv)(GLuint index, const GLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2dv)(GLuint index, const GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2fv)(GLuint index, const GLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2s)(GLuint index, GLshort x, GLshort y);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib2sv)(GLuint index, const GLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3dv)(GLuint index, const GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3fv)(GLuint index, const GLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib3sv)(GLuint index, const GLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nbv)(GLuint index, const GLbyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Niv)(GLuint index, const GLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nsv)(GLuint index, const GLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nubv)(GLuint index, const GLubyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nuiv)(GLuint index, const GLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4Nusv)(GLuint index, const GLushort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4bv)(GLuint index, const GLbyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4dv)(GLuint index, const GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4fv)(GLuint index, const GLfloat *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4iv)(GLuint index, const GLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4sv)(GLuint index, const GLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4ubv)(GLuint index, const GLubyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4uiv)(GLuint index, const GLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttrib4usv)(GLuint index, const GLushort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
	typedef void (CL_GLFUNC *ptr_glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);
	typedef void (CL_GLFUNC *ptr_glGetIntegeri_v)(GLenum target, GLuint index, GLint *data);
	typedef void (CL_GLFUNC *ptr_glEnablei)(GLenum target, GLuint index);
	typedef void (CL_GLFUNC *ptr_glDisablei)(GLenum target, GLuint index);
	typedef GLboolean (CL_GLFUNC *ptr_glIsEnabledi)(GLenum target, GLuint index);
	typedef void (CL_GLFUNC *ptr_glBeginTransformFeedback)(GLenum primitiveMode);
	typedef void (CL_GLFUNC *ptr_glEndTransformFeedback)(void);
	typedef void (CL_GLFUNC *ptr_glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
	typedef void (CL_GLFUNC *ptr_glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
	typedef void (CL_GLFUNC *ptr_glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar* *varyings, GLenum bufferMode);
	typedef void (CL_GLFUNC *ptr_glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
	typedef void (CL_GLFUNC *ptr_glClampColor)(GLenum target, GLenum clamp);
	typedef void (CL_GLFUNC *ptr_glBeginConditionalRender)(GLuint id, GLenum mode);
	typedef void (CL_GLFUNC *ptr_glEndConditionalRender)(void);
	typedef void (CL_GLFUNC *ptr_glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI1i)(GLuint index, GLint x);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI2i)(GLuint index, GLint x, GLint y);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI1ui)(GLuint index, GLuint x);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI1iv)(GLuint index, const GLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI2iv)(GLuint index, const GLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI3iv)(GLuint index, const GLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4iv)(GLuint index, const GLint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI1uiv)(GLuint index, const GLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI2uiv)(GLuint index, const GLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI3uiv)(GLuint index, const GLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4uiv)(GLuint index, const GLuint *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4bv)(GLuint index, const GLbyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4sv)(GLuint index, const GLshort *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4ubv)(GLuint index, const GLubyte *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribI4usv)(GLuint index, const GLushort *v);
	typedef void (CL_GLFUNC *ptr_glGetUniformuiv)(GLuint program, GLint location, GLuint *params);
	typedef void (CL_GLFUNC *ptr_glBindFragDataLocation)(GLuint program, GLuint color, const GLchar *name);
	typedef GLint (CL_GLFUNC *ptr_glGetFragDataLocation)(GLuint program, const GLchar *name);
	typedef void (CL_GLFUNC *ptr_glUniform1ui)(GLint location, GLuint v0);
	typedef void (CL_GLFUNC *ptr_glUniform2ui)(GLint location, GLuint v0, GLuint v1);
	typedef void (CL_GLFUNC *ptr_glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
	typedef void (CL_GLFUNC *ptr_glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
	typedef void (CL_GLFUNC *ptr_glUniform1uiv)(GLint location, GLsizei count, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glUniform2uiv)(GLint location, GLsizei count, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glUniform3uiv)(GLint location, GLsizei count, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glUniform4uiv)(GLint location, GLsizei count, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glTexParameterIiv)(GLenum target, GLenum pname, const GLint *params);
	typedef void (CL_GLFUNC *ptr_glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
	typedef void (CL_GLFUNC *ptr_glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
	typedef const GLubyte * (CL_GLFUNC *ptr_glGetStringi)(GLenum name, GLuint index);
	typedef void (CL_GLFUNC *ptr_glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
	typedef void (CL_GLFUNC *ptr_glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
	typedef void (CL_GLFUNC *ptr_glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
	typedef void (CL_GLFUNC *ptr_glPrimitiveRestartIndex)(GLuint index);
	typedef void (CL_GLFUNC *ptr_glGetInteger64i_v)(GLenum target, GLuint index, CLint64 *data);
	typedef void (CL_GLFUNC *ptr_glGetBufferParameteri64v)(GLenum target, GLenum pname, CLint64 *params);
	typedef void (CL_GLFUNC *ptr_glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
	typedef void (CL_GLFUNC *ptr_glVertexAttribDivisor)(GLuint index, GLuint divisor);
	typedef void (CL_GLFUNC *ptr_glMinSampleShading)(GLclampf value);
	typedef void (CL_GLFUNC *ptr_glBlendEquationi)(GLuint buf, GLenum mode);
	typedef void (CL_GLFUNC *ptr_glBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
	typedef void (CL_GLFUNC *ptr_glBlendFunci)(GLuint buf, GLenum src, GLenum dst);
	typedef void (CL_GLFUNC *ptr_glBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	typedef GLboolean (CL_GLFUNC *ptr_glIsRenderbuffer)(GLuint renderbuffer);
	typedef void (CL_GLFUNC *ptr_glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
	typedef void (CL_GLFUNC *ptr_glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
	typedef void (CL_GLFUNC *ptr_glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
	typedef void (CL_GLFUNC *ptr_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (CL_GLFUNC *ptr_glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
	typedef GLboolean (CL_GLFUNC *ptr_glIsFramebuffer)(GLuint framebuffer);
	typedef void (CL_GLFUNC *ptr_glBindFramebuffer)(GLenum target, GLuint framebuffer);
	typedef void (CL_GLFUNC *ptr_glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
	typedef void (CL_GLFUNC *ptr_glGenFramebuffers)(GLsizei n, GLuint *framebuffers);
	typedef GLenum (CL_GLFUNC *ptr_glCheckFramebufferStatus)(GLenum target);
	typedef void (CL_GLFUNC *ptr_glFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (CL_GLFUNC *ptr_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (CL_GLFUNC *ptr_glFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
	typedef void (CL_GLFUNC *ptr_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	typedef void (CL_GLFUNC *ptr_glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGenerateMipmap)(GLenum target);
	typedef void (CL_GLFUNC *ptr_glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
	typedef void (CL_GLFUNC *ptr_glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (CL_GLFUNC *ptr_glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
	typedef GLvoid* (CL_GLFUNC *ptr_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
	typedef void (CL_GLFUNC *ptr_glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
	typedef void (CL_GLFUNC *ptr_glBindVertexArray)(GLuint array);
	typedef void (CL_GLFUNC *ptr_glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
	typedef void (CL_GLFUNC *ptr_glGenVertexArrays)(GLsizei n, GLuint *arrays);
	typedef GLboolean (CL_GLFUNC *ptr_glIsVertexArray)(GLuint array);
	typedef void (CL_GLFUNC *ptr_glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar* *uniformNames, GLuint *uniformIndices);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
	typedef GLuint (CL_GLFUNC *ptr_glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
	typedef void (CL_GLFUNC *ptr_glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
	typedef void (CL_GLFUNC *ptr_glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
	typedef void (CL_GLFUNC *ptr_glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
	typedef void (CL_GLFUNC *ptr_glDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
	typedef void (CL_GLFUNC *ptr_glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex);
	typedef void (CL_GLFUNC *ptr_glMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount, const GLint *basevertex);
	typedef void (CL_GLFUNC *ptr_glProvokingVertex)(GLenum mode);
	typedef CLsync (CL_GLFUNC *ptr_glFenceSync)(GLenum condition, GLbitfield flags);
	typedef GLboolean (CL_GLFUNC *ptr_glIsSync)(CLsync sync);
	typedef void (CL_GLFUNC *ptr_glDeleteSync)(CLsync sync);
	typedef GLenum (CL_GLFUNC *ptr_glClientWaitSync)(CLsync sync, GLbitfield flags, CLuint64 timeout);
	typedef void (CL_GLFUNC *ptr_glWaitSync)(CLsync sync, GLbitfield flags, CLuint64 timeout);
	typedef void (CL_GLFUNC *ptr_glGetInteger64v)(GLenum pname, CLint64 *params);
	typedef void (CL_GLFUNC *ptr_glGetSynciv)(CLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
	typedef void (CL_GLFUNC *ptr_glTexImage2DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
	typedef void (CL_GLFUNC *ptr_glTexImage3DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
	typedef void (CL_GLFUNC *ptr_glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);
	typedef void (CL_GLFUNC *ptr_glSampleMaski)(GLuint index, GLbitfield mask);
	typedef void (CL_GLFUNC *ptr_glBlendEquationiARB)(GLuint buf, GLenum mode);
	typedef void (CL_GLFUNC *ptr_glBlendEquationSeparateiARB)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
	typedef void (CL_GLFUNC *ptr_glBlendFunciARB)(GLuint buf, GLenum src, GLenum dst);
	typedef void (CL_GLFUNC *ptr_glBlendFuncSeparateiARB)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	typedef void (CL_GLFUNC *ptr_glMinSampleShadingARB)(GLclampf value);
	typedef void (CL_GLFUNC *ptr_glNamedStringARB)(GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string);
	typedef void (CL_GLFUNC *ptr_glDeleteNamedStringARB)(GLint namelen, const GLchar *name);
	typedef void (CL_GLFUNC *ptr_glCompileShaderIncludeARB)(GLuint shader, GLsizei count, const GLchar* *path, const GLint *length);
	typedef GLboolean (CL_GLFUNC *ptr_glIsNamedStringARB)(GLint namelen, const GLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetNamedStringARB)(GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string);
	typedef void (CL_GLFUNC *ptr_glGetNamedStringivARB)(GLint namelen, const GLchar *name, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
	typedef GLint (CL_GLFUNC *ptr_glGetFragDataIndex)(GLuint program, const GLchar *name);
	typedef void (CL_GLFUNC *ptr_glGenSamplers)(GLsizei count, GLuint *samplers);
	typedef void (CL_GLFUNC *ptr_glDeleteSamplers)(GLsizei count, const GLuint *samplers);
	typedef GLboolean (CL_GLFUNC *ptr_glIsSampler)(GLuint sampler);
	typedef void (CL_GLFUNC *ptr_glBindSampler)(GLuint unit, GLuint sampler);
	typedef void (CL_GLFUNC *ptr_glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param);
	typedef void (CL_GLFUNC *ptr_glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param);
	typedef void (CL_GLFUNC *ptr_glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params);
	typedef void (CL_GLFUNC *ptr_glQueryCounter)(GLuint id, GLenum target);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjecti64v)(GLuint id, GLenum pname, CLint64 *params);
	typedef void (CL_GLFUNC *ptr_glGetQueryObjectui64v)(GLuint id, GLenum pname, CLuint64 *params);
	typedef void (CL_GLFUNC *ptr_glVertexP2ui)(GLenum type, GLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexP2uiv)(GLenum type, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexP3ui)(GLenum type, GLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexP3uiv)(GLenum type, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexP4ui)(GLenum type, GLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexP4uiv)(GLenum type, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glTexCoordP1ui)(GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP1uiv)(GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP2ui)(GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP2uiv)(GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP3ui)(GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP3uiv)(GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP4ui)(GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glTexCoordP4uiv)(GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP1ui)(GLenum texture, GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP1uiv)(GLenum texture, GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP2ui)(GLenum texture, GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP2uiv)(GLenum texture, GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP3ui)(GLenum texture, GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP3uiv)(GLenum texture, GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP4ui)(GLenum texture, GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glMultiTexCoordP4uiv)(GLenum texture, GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glNormalP3ui)(GLenum type, GLuint coords);
	typedef void (CL_GLFUNC *ptr_glNormalP3uiv)(GLenum type, const GLuint *coords);
	typedef void (CL_GLFUNC *ptr_glColorP3ui)(GLenum type, GLuint color);
	typedef void (CL_GLFUNC *ptr_glColorP3uiv)(GLenum type, const GLuint *color);
	typedef void (CL_GLFUNC *ptr_glColorP4ui)(GLenum type, GLuint color);
	typedef void (CL_GLFUNC *ptr_glColorP4uiv)(GLenum type, const GLuint *color);
	typedef void (CL_GLFUNC *ptr_glSecondaryColorP3ui)(GLenum type, GLuint color);
	typedef void (CL_GLFUNC *ptr_glSecondaryColorP3uiv)(GLenum type, const GLuint *color);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
	typedef void (CL_GLFUNC *ptr_glVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glDrawArraysIndirect)(GLenum mode, const GLvoid *indirect);
	typedef void (CL_GLFUNC *ptr_glDrawElementsIndirect)(GLenum mode, GLenum type, const GLvoid *indirect);
	typedef void (CL_GLFUNC *ptr_glUniform1d)(GLint location, GLdouble x);
	typedef void (CL_GLFUNC *ptr_glUniform2d)(GLint location, GLdouble x, GLdouble y);
	typedef void (CL_GLFUNC *ptr_glUniform3d)(GLint location, GLdouble x, GLdouble y, GLdouble z);
	typedef void (CL_GLFUNC *ptr_glUniform4d)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	typedef void (CL_GLFUNC *ptr_glUniform1dv)(GLint location, GLsizei count, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniform2dv)(GLint location, GLsizei count, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniform3dv)(GLint location, GLsizei count, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniform4dv)(GLint location, GLsizei count, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix2x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix3x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glUniformMatrix4x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glGetUniformdv)(GLuint program, GLint location, GLdouble *params);
	typedef GLint (CL_GLFUNC *ptr_glGetSubroutineUniformLocation)(GLuint program, GLenum shadertype, const GLchar *name);
	typedef GLuint (CL_GLFUNC *ptr_glGetSubroutineIndex)(GLuint program, GLenum shadertype, const GLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetActiveSubroutineUniformiv)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
	typedef void (CL_GLFUNC *ptr_glGetActiveSubroutineUniformName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
	typedef void (CL_GLFUNC *ptr_glGetActiveSubroutineName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
	typedef void (CL_GLFUNC *ptr_glUniformSubroutinesuiv)(GLenum shadertype, GLsizei count, const GLuint *indices);
	typedef void (CL_GLFUNC *ptr_glGetUniformSubroutineuiv)(GLenum shadertype, GLint location, GLuint *params);
	typedef void (CL_GLFUNC *ptr_glGetProgramStageiv)(GLuint program, GLenum shadertype, GLenum pname, GLint *values);
	typedef void (CL_GLFUNC *ptr_glPatchParameteri)(GLenum pname, GLint value);
	typedef void (CL_GLFUNC *ptr_glPatchParameterfv)(GLenum pname, const GLfloat *values);
	typedef void (CL_GLFUNC *ptr_glBindTransformFeedback)(GLenum target, GLuint id);
	typedef void (CL_GLFUNC *ptr_glDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);
	typedef void (CL_GLFUNC *ptr_glGenTransformFeedbacks)(GLsizei n, GLuint *ids);
	typedef GLboolean (CL_GLFUNC *ptr_glIsTransformFeedback)(GLuint id);
	typedef void (CL_GLFUNC *ptr_glPauseTransformFeedback)(void);
	typedef void (CL_GLFUNC *ptr_glResumeTransformFeedback)(void);
	typedef void (CL_GLFUNC *ptr_glDrawTransformFeedback)(GLenum mode, GLuint id);
	typedef void (CL_GLFUNC *ptr_glDrawTransformFeedbackStream)(GLenum mode, GLuint id, GLuint stream);
	typedef void (CL_GLFUNC *ptr_glBeginQueryIndexed)(GLenum target, GLuint index, GLuint id);
	typedef void (CL_GLFUNC *ptr_glEndQueryIndexed)(GLenum target, GLuint index);
	typedef void (CL_GLFUNC *ptr_glGetQueryIndexediv)(GLenum target, GLuint index, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glReleaseShaderCompiler)(void);
	typedef void (CL_GLFUNC *ptr_glShaderBinary)(GLsizei count, const GLuint *shaders, GLenum binaryformat, const GLvoid *binary, GLsizei length);
	typedef void (CL_GLFUNC *ptr_glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
	typedef void (CL_GLFUNC *ptr_glDepthRangef)(GLclampf n, GLclampf f);
	typedef void (CL_GLFUNC *ptr_glClearDepthf)(GLclampf d);
	typedef void (CL_GLFUNC *ptr_glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
	typedef void (CL_GLFUNC *ptr_glProgramBinary)(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length);
	typedef void (CL_GLFUNC *ptr_glProgramParameteri)(GLuint program, GLenum pname, GLint value);
	typedef void (CL_GLFUNC *ptr_glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
	typedef void (CL_GLFUNC *ptr_glActiveShaderProgram)(GLuint pipeline, GLuint program);
	typedef GLuint (CL_GLFUNC *ptr_glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar* *strings);
	typedef void (CL_GLFUNC *ptr_glBindProgramPipeline)(GLuint pipeline);
	typedef void (CL_GLFUNC *ptr_glDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);
	typedef void (CL_GLFUNC *ptr_glGenProgramPipelines)(GLsizei n, GLuint *pipelines);
	typedef GLboolean (CL_GLFUNC *ptr_glIsProgramPipeline)(GLuint pipeline);
	typedef void (CL_GLFUNC *ptr_glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1i)(GLuint program, GLint location, GLint v0);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1d)(GLuint program, GLint location, GLdouble v0);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
	typedef void (CL_GLFUNC *ptr_glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2d)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
	typedef void (CL_GLFUNC *ptr_glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
	typedef void (CL_GLFUNC *ptr_glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
	typedef void (CL_GLFUNC *ptr_glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix2x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix3x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glProgramUniformMatrix4x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
	typedef void (CL_GLFUNC *ptr_glValidateProgramPipeline)(GLuint pipeline);
	typedef void (CL_GLFUNC *ptr_glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	typedef void (CL_GLFUNC *ptr_glVertexAttribL1d)(GLuint index, GLdouble x);
	typedef void (CL_GLFUNC *ptr_glVertexAttribL2d)(GLuint index, GLdouble x, GLdouble y);
	typedef void (CL_GLFUNC *ptr_glVertexAttribL3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
	typedef void (CL_GLFUNC *ptr_glVertexAttribL4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
	typedef void (CL_GLFUNC *ptr_glVertexAttribL1dv)(GLuint index, const GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribL2dv)(GLuint index, const GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribL3dv)(GLuint index, const GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribL4dv)(GLuint index, const GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glVertexAttribLPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	typedef void (CL_GLFUNC *ptr_glGetVertexAttribLdv)(GLuint index, GLenum pname, GLdouble *params);
	typedef void (CL_GLFUNC *ptr_glViewportArrayv)(GLuint first, GLsizei count, const GLfloat *v);
	typedef void (CL_GLFUNC *ptr_glViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	typedef void (CL_GLFUNC *ptr_glViewportIndexedfv)(GLuint index, const GLfloat *v);
	typedef void (CL_GLFUNC *ptr_glScissorArrayv)(GLuint first, GLsizei count, const GLint *v);
	typedef void (CL_GLFUNC *ptr_glScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
	typedef void (CL_GLFUNC *ptr_glScissorIndexedv)(GLuint index, const GLint *v);
	typedef void (CL_GLFUNC *ptr_glDepthRangeArrayv)(GLuint first, GLsizei count, const GLclampd *v);
	typedef void (CL_GLFUNC *ptr_glDepthRangeIndexed)(GLuint index, GLclampd n, GLclampd f);
	typedef void (CL_GLFUNC *ptr_glGetFloati_v)(GLenum target, GLuint index, GLfloat *data);
	typedef void (CL_GLFUNC *ptr_glGetDoublei_v)(GLenum target, GLuint index, GLdouble *data);
	typedef CLsync (CL_GLFUNC *ptr_glCreateSyncFromCLeventARB)(struct _cl_context * context, struct _cl_event * event, GLbitfield flags);
	typedef void (CL_GLFUNC *ptr_glDebugMessageControlARB)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
	typedef void (CL_GLFUNC *ptr_glDebugMessageInsertARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
	typedef void (CL_GLFUNC *ptr_glDebugMessageCallbackARB)(CLDEBUGPROCARB callback, const GLvoid *userParam);
	typedef GLuint (CL_GLFUNC *ptr_glGetDebugMessageLogARB)(GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
	typedef GLenum (CL_GLFUNC *ptr_glGetGraphicsResetStatusARB)(void);
	typedef void (CL_GLFUNC *ptr_glGetnMapdvARB)(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v);
	typedef void (CL_GLFUNC *ptr_glGetnMapfvARB)(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v);
	typedef void (CL_GLFUNC *ptr_glGetnMapivARB)(GLenum target, GLenum query, GLsizei bufSize, GLint *v);
	typedef void (CL_GLFUNC *ptr_glGetnPixelMapfvARB)(GLenum map, GLsizei bufSize, GLfloat *values);
	typedef void (CL_GLFUNC *ptr_glGetnPixelMapuivARB)(GLenum map, GLsizei bufSize, GLuint *values);
	typedef void (CL_GLFUNC *ptr_glGetnPixelMapusvARB)(GLenum map, GLsizei bufSize, GLushort *values);
	typedef void (CL_GLFUNC *ptr_glGetnPolygonStippleARB)(GLsizei bufSize, GLubyte *pattern);
	typedef void (CL_GLFUNC *ptr_glGetnColorTableARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *table);
	typedef void (CL_GLFUNC *ptr_glGetnConvolutionFilterARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *image);
	typedef void (CL_GLFUNC *ptr_glGetnSeparableFilterARB)(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, GLvoid *row, GLsizei columnBufSize, GLvoid *column, GLvoid *span);
	typedef void (CL_GLFUNC *ptr_glGetnHistogramARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
	typedef void (CL_GLFUNC *ptr_glGetnMinmaxARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
	typedef void (CL_GLFUNC *ptr_glGetnTexImageARB)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid *img);
	typedef void (CL_GLFUNC *ptr_glReadnPixelsARB)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid *data);
	typedef void (CL_GLFUNC *ptr_glGetnCompressedTexImageARB)(GLenum target, GLint lod, GLsizei bufSize, GLvoid *img);
	typedef void (CL_GLFUNC *ptr_glGetnUniformfvARB)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
	typedef void (CL_GLFUNC *ptr_glGetnUniformivARB)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
	typedef void (CL_GLFUNC *ptr_glGetnUniformuivARB)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
	typedef void (CL_GLFUNC *ptr_glGetnUniformdvARB)(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);

/// \}

/// \name Functions
/// \{

public:
	ptr_glCullFace cullFace;
	ptr_glFrontFace frontFace;
	ptr_glHint hint;
	ptr_glLineWidth lineWidth;
	ptr_glPointSize pointSize;
	ptr_glPolygonMode polygonMode;
	ptr_glScissor scissor;
	ptr_glTexParameterf texParameterf;
	ptr_glTexParameterfv texParameterfv;
	ptr_glTexParameteri texParameteri;
	ptr_glTexParameteriv texParameteriv;
	ptr_glTexImage1D texImage1D;
	ptr_glTexImage2D texImage2D;
	ptr_glDrawBuffer drawBuffer;
	ptr_glClear clear;
	ptr_glClearColor clearColor;
	ptr_glClearStencil clearStencil;
	ptr_glClearDepth clearDepth;
	ptr_glStencilMask stencilMask;
	ptr_glColorMask colorMask;
	ptr_glDepthMask depthMask;
	ptr_glDisable disable;
	ptr_glEnable enable;
	ptr_glFinish finish;
	ptr_glFlush flush;
	ptr_glBlendFunc blendFunc;
	ptr_glLogicOp logicOp;
	ptr_glStencilFunc stencilFunc;
	ptr_glStencilOp stencilOp;
	ptr_glDepthFunc depthFunc;
	ptr_glPixelStoref pixelStoref;
	ptr_glPixelStorei pixelStorei;
	ptr_glReadBuffer readBuffer;
	ptr_glReadPixels readPixels;
	ptr_glGetBooleanv getBooleanv;
	ptr_glGetDoublev getDoublev;
	ptr_glGetError getError;
	ptr_glGetFloatv getFloatv;
	ptr_glGetIntegerv getIntegerv;
	ptr_glGetString getString;
	ptr_glGetTexImage getTexImage;
	ptr_glGetTexParameterfv getTexParameterfv;
	ptr_glGetTexParameteriv getTexParameteriv;
	ptr_glGetTexLevelParameterfv getTexLevelParameterfv;
	ptr_glGetTexLevelParameteriv getTexLevelParameteriv;
	ptr_glIsEnabled isEnabled;
	ptr_glDepthRange depthRange;
	ptr_glViewport viewport;
	ptr_glDrawArrays drawArrays;
	ptr_glDrawElements drawElements;
	ptr_glGetPointerv getPointerv;
	ptr_glPolygonOffset polygonOffset;
	ptr_glCopyTexImage1D copyTexImage1D;
	ptr_glCopyTexImage2D copyTexImage2D;
	ptr_glCopyTexSubImage1D copyTexSubImage1D;
	ptr_glCopyTexSubImage2D copyTexSubImage2D;
	ptr_glTexSubImage1D texSubImage1D;
	ptr_glTexSubImage2D texSubImage2D;
	ptr_glBindTexture bindTexture;
	ptr_glDeleteTextures deleteTextures;
	ptr_glGenTextures genTextures;
	ptr_glIsTexture isTexture;
	ptr_glBlendColor blendColor;
	ptr_glBlendEquation blendEquation;
	ptr_glDrawRangeElements drawRangeElements;
	ptr_glTexImage3D texImage3D;
	ptr_glTexSubImage3D texSubImage3D;
	ptr_glCopyTexSubImage3D copyTexSubImage3D;
	ptr_glActiveTexture activeTexture;
	ptr_glSampleCoverage sampleCoverage;
	ptr_glCompressedTexImage3D compressedTexImage3D;
	ptr_glCompressedTexImage2D compressedTexImage2D;
	ptr_glCompressedTexImage1D compressedTexImage1D;
	ptr_glCompressedTexSubImage3D compressedTexSubImage3D;
	ptr_glCompressedTexSubImage2D compressedTexSubImage2D;
	ptr_glCompressedTexSubImage1D compressedTexSubImage1D;
	ptr_glGetCompressedTexImage getCompressedTexImage;
	ptr_glBlendFuncSeparate blendFuncSeparate;
	ptr_glMultiDrawArrays multiDrawArrays;
	ptr_glMultiDrawElements multiDrawElements;
	ptr_glPointParameterf pointParameterf;
	ptr_glPointParameterfv pointParameterfv;
	ptr_glPointParameteri pointParameteri;
	ptr_glPointParameteriv pointParameteriv;
	ptr_glGenQueries genQueries;
	ptr_glDeleteQueries deleteQueries;
	ptr_glIsQuery isQuery;
	ptr_glBeginQuery beginQuery;
	ptr_glEndQuery endQuery;
	ptr_glGetQueryiv getQueryiv;
	ptr_glGetQueryObjectiv getQueryObjectiv;
	ptr_glGetQueryObjectuiv getQueryObjectuiv;
	ptr_glBindBuffer bindBuffer;
	ptr_glDeleteBuffers deleteBuffers;
	ptr_glGenBuffers genBuffers;
	ptr_glIsBuffer isBuffer;
	ptr_glBufferData bufferData;
	ptr_glBufferSubData bufferSubData;
	ptr_glGetBufferSubData getBufferSubData;
	ptr_glMapBuffer mapBuffer;
	ptr_glUnmapBuffer unmapBuffer;
	ptr_glGetBufferParameteriv getBufferParameteriv;
	ptr_glGetBufferPointerv getBufferPointerv;
	ptr_glBlendEquationSeparate blendEquationSeparate;
	ptr_glDrawBuffers drawBuffers;
	ptr_glStencilOpSeparate stencilOpSeparate;
	ptr_glStencilFuncSeparate stencilFuncSeparate;
	ptr_glStencilMaskSeparate stencilMaskSeparate;
	ptr_glAttachShader attachShader;
	ptr_glBindAttribLocation bindAttribLocation;
	ptr_glCompileShader compileShader;
	ptr_glCreateProgram createProgram;
	ptr_glCreateShader createShader;
	ptr_glDeleteProgram deleteProgram;
	ptr_glDeleteShader deleteShader;
	ptr_glDetachShader detachShader;
	ptr_glDisableVertexAttribArray disableVertexAttribArray;
	ptr_glEnableVertexAttribArray enableVertexAttribArray;
	ptr_glGetActiveAttrib getActiveAttrib;
	ptr_glGetActiveUniform getActiveUniform;
	ptr_glGetAttachedShaders getAttachedShaders;
	ptr_glGetAttribLocation getAttribLocation;
	ptr_glGetProgramiv getProgramiv;
	ptr_glGetProgramInfoLog getProgramInfoLog;
	ptr_glGetShaderiv getShaderiv;
	ptr_glGetShaderInfoLog getShaderInfoLog;
	ptr_glGetShaderSource getShaderSource;
	ptr_glGetUniformLocation getUniformLocation;
	ptr_glGetUniformfv getUniformfv;
	ptr_glGetUniformiv getUniformiv;
	ptr_glGetVertexAttribdv getVertexAttribdv;
	ptr_glGetVertexAttribfv getVertexAttribfv;
	ptr_glGetVertexAttribiv getVertexAttribiv;
	ptr_glGetVertexAttribPointerv getVertexAttribPointerv;
	ptr_glIsProgram isProgram;
	ptr_glIsShader isShader;
	ptr_glLinkProgram linkProgram;
	ptr_glShaderSource shaderSource;
	ptr_glUseProgram useProgram;
	ptr_glUniform1f uniform1f;
	ptr_glUniform2f uniform2f;
	ptr_glUniform3f uniform3f;
	ptr_glUniform4f uniform4f;
	ptr_glUniform1i uniform1i;
	ptr_glUniform2i uniform2i;
	ptr_glUniform3i uniform3i;
	ptr_glUniform4i uniform4i;
	ptr_glUniform1fv uniform1fv;
	ptr_glUniform2fv uniform2fv;
	ptr_glUniform3fv uniform3fv;
	ptr_glUniform4fv uniform4fv;
	ptr_glUniform1iv uniform1iv;
	ptr_glUniform2iv uniform2iv;
	ptr_glUniform3iv uniform3iv;
	ptr_glUniform4iv uniform4iv;
	ptr_glUniformMatrix2fv uniformMatrix2fv;
	ptr_glUniformMatrix3fv uniformMatrix3fv;
	ptr_glUniformMatrix4fv uniformMatrix4fv;
	ptr_glValidateProgram validateProgram;
	ptr_glVertexAttrib1d vertexAttrib1d;
	ptr_glVertexAttrib1dv vertexAttrib1dv;
	ptr_glVertexAttrib1f vertexAttrib1f;
	ptr_glVertexAttrib1fv vertexAttrib1fv;
	ptr_glVertexAttrib1s vertexAttrib1s;
	ptr_glVertexAttrib1sv vertexAttrib1sv;
	ptr_glVertexAttrib2d vertexAttrib2d;
	ptr_glVertexAttrib2dv vertexAttrib2dv;
	ptr_glVertexAttrib2f vertexAttrib2f;
	ptr_glVertexAttrib2fv vertexAttrib2fv;
	ptr_glVertexAttrib2s vertexAttrib2s;
	ptr_glVertexAttrib2sv vertexAttrib2sv;
	ptr_glVertexAttrib3d vertexAttrib3d;
	ptr_glVertexAttrib3dv vertexAttrib3dv;
	ptr_glVertexAttrib3f vertexAttrib3f;
	ptr_glVertexAttrib3fv vertexAttrib3fv;
	ptr_glVertexAttrib3s vertexAttrib3s;
	ptr_glVertexAttrib3sv vertexAttrib3sv;
	ptr_glVertexAttrib4Nbv vertexAttrib4Nbv;
	ptr_glVertexAttrib4Niv vertexAttrib4Niv;
	ptr_glVertexAttrib4Nsv vertexAttrib4Nsv;
	ptr_glVertexAttrib4Nub vertexAttrib4Nub;
	ptr_glVertexAttrib4Nubv vertexAttrib4Nubv;
	ptr_glVertexAttrib4Nuiv vertexAttrib4Nuiv;
	ptr_glVertexAttrib4Nusv vertexAttrib4Nusv;
	ptr_glVertexAttrib4bv vertexAttrib4bv;
	ptr_glVertexAttrib4d vertexAttrib4d;
	ptr_glVertexAttrib4dv vertexAttrib4dv;
	ptr_glVertexAttrib4f vertexAttrib4f;
	ptr_glVertexAttrib4fv vertexAttrib4fv;
	ptr_glVertexAttrib4iv vertexAttrib4iv;
	ptr_glVertexAttrib4s vertexAttrib4s;
	ptr_glVertexAttrib4sv vertexAttrib4sv;
	ptr_glVertexAttrib4ubv vertexAttrib4ubv;
	ptr_glVertexAttrib4uiv vertexAttrib4uiv;
	ptr_glVertexAttrib4usv vertexAttrib4usv;
	ptr_glVertexAttribPointer vertexAttribPointer;
	ptr_glUniformMatrix2x3fv uniformMatrix2x3fv;
	ptr_glUniformMatrix3x2fv uniformMatrix3x2fv;
	ptr_glUniformMatrix2x4fv uniformMatrix2x4fv;
	ptr_glUniformMatrix4x2fv uniformMatrix4x2fv;
	ptr_glUniformMatrix3x4fv uniformMatrix3x4fv;
	ptr_glUniformMatrix4x3fv uniformMatrix4x3fv;
	ptr_glColorMaski colorMaski;
	ptr_glGetBooleani_v getBooleani_v;
	ptr_glGetIntegeri_v getIntegeri_v;
	ptr_glEnablei enablei;
	ptr_glDisablei disablei;
	ptr_glIsEnabledi isEnabledi;
	ptr_glBeginTransformFeedback beginTransformFeedback;
	ptr_glEndTransformFeedback endTransformFeedback;
	ptr_glBindBufferRange bindBufferRange;
	ptr_glBindBufferBase bindBufferBase;
	ptr_glTransformFeedbackVaryings transformFeedbackVaryings;
	ptr_glGetTransformFeedbackVarying getTransformFeedbackVarying;
	ptr_glClampColor clampColor;
	ptr_glBeginConditionalRender beginConditionalRender;
	ptr_glEndConditionalRender endConditionalRender;
	ptr_glVertexAttribIPointer vertexAttribIPointer;
	ptr_glGetVertexAttribIiv getVertexAttribIiv;
	ptr_glGetVertexAttribIuiv getVertexAttribIuiv;
	ptr_glVertexAttribI1i vertexAttribI1i;
	ptr_glVertexAttribI2i vertexAttribI2i;
	ptr_glVertexAttribI3i vertexAttribI3i;
	ptr_glVertexAttribI4i vertexAttribI4i;
	ptr_glVertexAttribI1ui vertexAttribI1ui;
	ptr_glVertexAttribI2ui vertexAttribI2ui;
	ptr_glVertexAttribI3ui vertexAttribI3ui;
	ptr_glVertexAttribI4ui vertexAttribI4ui;
	ptr_glVertexAttribI1iv vertexAttribI1iv;
	ptr_glVertexAttribI2iv vertexAttribI2iv;
	ptr_glVertexAttribI3iv vertexAttribI3iv;
	ptr_glVertexAttribI4iv vertexAttribI4iv;
	ptr_glVertexAttribI1uiv vertexAttribI1uiv;
	ptr_glVertexAttribI2uiv vertexAttribI2uiv;
	ptr_glVertexAttribI3uiv vertexAttribI3uiv;
	ptr_glVertexAttribI4uiv vertexAttribI4uiv;
	ptr_glVertexAttribI4bv vertexAttribI4bv;
	ptr_glVertexAttribI4sv vertexAttribI4sv;
	ptr_glVertexAttribI4ubv vertexAttribI4ubv;
	ptr_glVertexAttribI4usv vertexAttribI4usv;
	ptr_glGetUniformuiv getUniformuiv;
	ptr_glBindFragDataLocation bindFragDataLocation;
	ptr_glGetFragDataLocation getFragDataLocation;
	ptr_glUniform1ui uniform1ui;
	ptr_glUniform2ui uniform2ui;
	ptr_glUniform3ui uniform3ui;
	ptr_glUniform4ui uniform4ui;
	ptr_glUniform1uiv uniform1uiv;
	ptr_glUniform2uiv uniform2uiv;
	ptr_glUniform3uiv uniform3uiv;
	ptr_glUniform4uiv uniform4uiv;
	ptr_glTexParameterIiv texParameterIiv;
	ptr_glTexParameterIuiv texParameterIuiv;
	ptr_glGetTexParameterIiv getTexParameterIiv;
	ptr_glGetTexParameterIuiv getTexParameterIuiv;
	ptr_glClearBufferiv clearBufferiv;
	ptr_glClearBufferuiv clearBufferuiv;
	ptr_glClearBufferfv clearBufferfv;
	ptr_glClearBufferfi clearBufferfi;
	ptr_glGetStringi getStringi;
	ptr_glDrawArraysInstanced drawArraysInstanced;
	ptr_glDrawElementsInstanced drawElementsInstanced;
	ptr_glTexBuffer texBuffer;
	ptr_glPrimitiveRestartIndex primitiveRestartIndex;
	ptr_glGetInteger64i_v getInteger64i_v;
	ptr_glGetBufferParameteri64v getBufferParameteri64v;
	ptr_glFramebufferTexture framebufferTexture;
	ptr_glVertexAttribDivisor vertexAttribDivisor;
	ptr_glMinSampleShading minSampleShading;
	ptr_glBlendEquationi blendEquationi;
	ptr_glBlendEquationSeparatei blendEquationSeparatei;
	ptr_glBlendFunci blendFunci;
	ptr_glBlendFuncSeparatei blendFuncSeparatei;
	ptr_glIsRenderbuffer isRenderbuffer;
	ptr_glBindRenderbuffer bindRenderbuffer;
	ptr_glDeleteRenderbuffers deleteRenderbuffers;
	ptr_glGenRenderbuffers genRenderbuffers;
	ptr_glRenderbufferStorage renderbufferStorage;
	ptr_glGetRenderbufferParameteriv getRenderbufferParameteriv;
	ptr_glIsFramebuffer isFramebuffer;
	ptr_glBindFramebuffer bindFramebuffer;
	ptr_glDeleteFramebuffers deleteFramebuffers;
	ptr_glGenFramebuffers genFramebuffers;
	ptr_glCheckFramebufferStatus checkFramebufferStatus;
	ptr_glFramebufferTexture1D framebufferTexture1D;
	ptr_glFramebufferTexture2D framebufferTexture2D;
	ptr_glFramebufferTexture3D framebufferTexture3D;
	ptr_glFramebufferRenderbuffer framebufferRenderbuffer;
	ptr_glGetFramebufferAttachmentParameteriv getFramebufferAttachmentParameteriv;
	ptr_glGenerateMipmap generateMipmap;
	ptr_glBlitFramebuffer blitFramebuffer;
	ptr_glRenderbufferStorageMultisample renderbufferStorageMultisample;
	ptr_glFramebufferTextureLayer framebufferTextureLayer;
	ptr_glMapBufferRange mapBufferRange;
	ptr_glFlushMappedBufferRange flushMappedBufferRange;
	ptr_glBindVertexArray bindVertexArray;
	ptr_glDeleteVertexArrays deleteVertexArrays;
	ptr_glGenVertexArrays genVertexArrays;
	ptr_glIsVertexArray isVertexArray;
	ptr_glGetUniformIndices getUniformIndices;
	ptr_glGetActiveUniformsiv getActiveUniformsiv;
	ptr_glGetActiveUniformName getActiveUniformName;
	ptr_glGetUniformBlockIndex getUniformBlockIndex;
	ptr_glGetActiveUniformBlockiv getActiveUniformBlockiv;
	ptr_glGetActiveUniformBlockName getActiveUniformBlockName;
	ptr_glUniformBlockBinding uniformBlockBinding;
	ptr_glCopyBufferSubData copyBufferSubData;
	ptr_glDrawElementsBaseVertex drawElementsBaseVertex;
	ptr_glDrawRangeElementsBaseVertex drawRangeElementsBaseVertex;
	ptr_glDrawElementsInstancedBaseVertex drawElementsInstancedBaseVertex;
	ptr_glMultiDrawElementsBaseVertex multiDrawElementsBaseVertex;
	ptr_glProvokingVertex provokingVertex;
	ptr_glFenceSync fenceSync;
	ptr_glIsSync isSync;
	ptr_glDeleteSync deleteSync;
	ptr_glClientWaitSync clientWaitSync;
	ptr_glWaitSync waitSync;
	ptr_glGetInteger64v getInteger64v;
	ptr_glGetSynciv getSynciv;
	ptr_glTexImage2DMultisample texImage2DMultisample;
	ptr_glTexImage3DMultisample texImage3DMultisample;
	ptr_glGetMultisamplefv getMultisamplefv;
	ptr_glSampleMaski sampleMaski;
	ptr_glBlendEquationiARB blendEquationiARB;
	ptr_glBlendEquationSeparateiARB blendEquationSeparateiARB;
	ptr_glBlendFunciARB blendFunciARB;
	ptr_glBlendFuncSeparateiARB blendFuncSeparateiARB;
	ptr_glMinSampleShadingARB minSampleShadingARB;
	ptr_glNamedStringARB namedStringARB;
	ptr_glDeleteNamedStringARB deleteNamedStringARB;
	ptr_glCompileShaderIncludeARB compileShaderIncludeARB;
	ptr_glIsNamedStringARB isNamedStringARB;
	ptr_glGetNamedStringARB getNamedStringARB;
	ptr_glGetNamedStringivARB getNamedStringivARB;
	ptr_glBindFragDataLocationIndexed bindFragDataLocationIndexed;
	ptr_glGetFragDataIndex getFragDataIndex;
	ptr_glGenSamplers genSamplers;
	ptr_glDeleteSamplers deleteSamplers;
	ptr_glIsSampler isSampler;
	ptr_glBindSampler bindSampler;
	ptr_glSamplerParameteri samplerParameteri;
	ptr_glSamplerParameteriv samplerParameteriv;
	ptr_glSamplerParameterf samplerParameterf;
	ptr_glSamplerParameterfv samplerParameterfv;
	ptr_glSamplerParameterIiv samplerParameterIiv;
	ptr_glSamplerParameterIuiv samplerParameterIuiv;
	ptr_glGetSamplerParameteriv getSamplerParameteriv;
	ptr_glGetSamplerParameterIiv getSamplerParameterIiv;
	ptr_glGetSamplerParameterfv getSamplerParameterfv;
	ptr_glGetSamplerParameterIuiv getSamplerParameterIuiv;
	ptr_glQueryCounter queryCounter;
	ptr_glGetQueryObjecti64v getQueryObjecti64v;
	ptr_glGetQueryObjectui64v getQueryObjectui64v;
	ptr_glVertexP2ui vertexP2ui;
	ptr_glVertexP2uiv vertexP2uiv;
	ptr_glVertexP3ui vertexP3ui;
	ptr_glVertexP3uiv vertexP3uiv;
	ptr_glVertexP4ui vertexP4ui;
	ptr_glVertexP4uiv vertexP4uiv;
	ptr_glTexCoordP1ui texCoordP1ui;
	ptr_glTexCoordP1uiv texCoordP1uiv;
	ptr_glTexCoordP2ui texCoordP2ui;
	ptr_glTexCoordP2uiv texCoordP2uiv;
	ptr_glTexCoordP3ui texCoordP3ui;
	ptr_glTexCoordP3uiv texCoordP3uiv;
	ptr_glTexCoordP4ui texCoordP4ui;
	ptr_glTexCoordP4uiv texCoordP4uiv;
	ptr_glMultiTexCoordP1ui multiTexCoordP1ui;
	ptr_glMultiTexCoordP1uiv multiTexCoordP1uiv;
	ptr_glMultiTexCoordP2ui multiTexCoordP2ui;
	ptr_glMultiTexCoordP2uiv multiTexCoordP2uiv;
	ptr_glMultiTexCoordP3ui multiTexCoordP3ui;
	ptr_glMultiTexCoordP3uiv multiTexCoordP3uiv;
	ptr_glMultiTexCoordP4ui multiTexCoordP4ui;
	ptr_glMultiTexCoordP4uiv multiTexCoordP4uiv;
	ptr_glNormalP3ui normalP3ui;
	ptr_glNormalP3uiv normalP3uiv;
	ptr_glColorP3ui colorP3ui;
	ptr_glColorP3uiv colorP3uiv;
	ptr_glColorP4ui colorP4ui;
	ptr_glColorP4uiv colorP4uiv;
	ptr_glSecondaryColorP3ui secondaryColorP3ui;
	ptr_glSecondaryColorP3uiv secondaryColorP3uiv;
	ptr_glVertexAttribP1ui vertexAttribP1ui;
	ptr_glVertexAttribP1uiv vertexAttribP1uiv;
	ptr_glVertexAttribP2ui vertexAttribP2ui;
	ptr_glVertexAttribP2uiv vertexAttribP2uiv;
	ptr_glVertexAttribP3ui vertexAttribP3ui;
	ptr_glVertexAttribP3uiv vertexAttribP3uiv;
	ptr_glVertexAttribP4ui vertexAttribP4ui;
	ptr_glVertexAttribP4uiv vertexAttribP4uiv;
	ptr_glDrawArraysIndirect drawArraysIndirect;
	ptr_glDrawElementsIndirect drawElementsIndirect;
	ptr_glUniform1d uniform1d;
	ptr_glUniform2d uniform2d;
	ptr_glUniform3d uniform3d;
	ptr_glUniform4d uniform4d;
	ptr_glUniform1dv uniform1dv;
	ptr_glUniform2dv uniform2dv;
	ptr_glUniform3dv uniform3dv;
	ptr_glUniform4dv uniform4dv;
	ptr_glUniformMatrix2dv uniformMatrix2dv;
	ptr_glUniformMatrix3dv uniformMatrix3dv;
	ptr_glUniformMatrix4dv uniformMatrix4dv;
	ptr_glUniformMatrix2x3dv uniformMatrix2x3dv;
	ptr_glUniformMatrix2x4dv uniformMatrix2x4dv;
	ptr_glUniformMatrix3x2dv uniformMatrix3x2dv;
	ptr_glUniformMatrix3x4dv uniformMatrix3x4dv;
	ptr_glUniformMatrix4x2dv uniformMatrix4x2dv;
	ptr_glUniformMatrix4x3dv uniformMatrix4x3dv;
	ptr_glGetUniformdv getUniformdv;
	ptr_glGetSubroutineUniformLocation getSubroutineUniformLocation;
	ptr_glGetSubroutineIndex getSubroutineIndex;
	ptr_glGetActiveSubroutineUniformiv getActiveSubroutineUniformiv;
	ptr_glGetActiveSubroutineUniformName getActiveSubroutineUniformName;
	ptr_glGetActiveSubroutineName getActiveSubroutineName;
	ptr_glUniformSubroutinesuiv uniformSubroutinesuiv;
	ptr_glGetUniformSubroutineuiv getUniformSubroutineuiv;
	ptr_glGetProgramStageiv getProgramStageiv;
	ptr_glPatchParameteri patchParameteri;
	ptr_glPatchParameterfv patchParameterfv;
	ptr_glBindTransformFeedback bindTransformFeedback;
	ptr_glDeleteTransformFeedbacks deleteTransformFeedbacks;
	ptr_glGenTransformFeedbacks genTransformFeedbacks;
	ptr_glIsTransformFeedback isTransformFeedback;
	ptr_glPauseTransformFeedback pauseTransformFeedback;
	ptr_glResumeTransformFeedback resumeTransformFeedback;
	ptr_glDrawTransformFeedback drawTransformFeedback;
	ptr_glDrawTransformFeedbackStream drawTransformFeedbackStream;
	ptr_glBeginQueryIndexed beginQueryIndexed;
	ptr_glEndQueryIndexed endQueryIndexed;
	ptr_glGetQueryIndexediv getQueryIndexediv;
	ptr_glReleaseShaderCompiler releaseShaderCompiler;
	ptr_glShaderBinary shaderBinary;
	ptr_glGetShaderPrecisionFormat getShaderPrecisionFormat;
	ptr_glDepthRangef depthRangef;
	ptr_glClearDepthf clearDepthf;
	ptr_glGetProgramBinary getProgramBinary;
	ptr_glProgramBinary programBinary;
	ptr_glProgramParameteri programParameteri;
	ptr_glUseProgramStages useProgramStages;
	ptr_glActiveShaderProgram activeShaderProgram;
	ptr_glCreateShaderProgramv createShaderProgramv;
	ptr_glBindProgramPipeline bindProgramPipeline;
	ptr_glDeleteProgramPipelines deleteProgramPipelines;
	ptr_glGenProgramPipelines genProgramPipelines;
	ptr_glIsProgramPipeline isProgramPipeline;
	ptr_glGetProgramPipelineiv getProgramPipelineiv;
	ptr_glProgramUniform1i programUniform1i;
	ptr_glProgramUniform1iv programUniform1iv;
	ptr_glProgramUniform1f programUniform1f;
	ptr_glProgramUniform1fv programUniform1fv;
	ptr_glProgramUniform1d programUniform1d;
	ptr_glProgramUniform1dv programUniform1dv;
	ptr_glProgramUniform1ui programUniform1ui;
	ptr_glProgramUniform1uiv programUniform1uiv;
	ptr_glProgramUniform2i programUniform2i;
	ptr_glProgramUniform2iv programUniform2iv;
	ptr_glProgramUniform2f programUniform2f;
	ptr_glProgramUniform2fv programUniform2fv;
	ptr_glProgramUniform2d programUniform2d;
	ptr_glProgramUniform2dv programUniform2dv;
	ptr_glProgramUniform2ui programUniform2ui;
	ptr_glProgramUniform2uiv programUniform2uiv;
	ptr_glProgramUniform3i programUniform3i;
	ptr_glProgramUniform3iv programUniform3iv;
	ptr_glProgramUniform3f programUniform3f;
	ptr_glProgramUniform3fv programUniform3fv;
	ptr_glProgramUniform3d programUniform3d;
	ptr_glProgramUniform3dv programUniform3dv;
	ptr_glProgramUniform3ui programUniform3ui;
	ptr_glProgramUniform3uiv programUniform3uiv;
	ptr_glProgramUniform4i programUniform4i;
	ptr_glProgramUniform4iv programUniform4iv;
	ptr_glProgramUniform4f programUniform4f;
	ptr_glProgramUniform4fv programUniform4fv;
	ptr_glProgramUniform4d programUniform4d;
	ptr_glProgramUniform4dv programUniform4dv;
	ptr_glProgramUniform4ui programUniform4ui;
	ptr_glProgramUniform4uiv programUniform4uiv;
	ptr_glProgramUniformMatrix2fv programUniformMatrix2fv;
	ptr_glProgramUniformMatrix3fv programUniformMatrix3fv;
	ptr_glProgramUniformMatrix4fv programUniformMatrix4fv;
	ptr_glProgramUniformMatrix2dv programUniformMatrix2dv;
	ptr_glProgramUniformMatrix3dv programUniformMatrix3dv;
	ptr_glProgramUniformMatrix4dv programUniformMatrix4dv;
	ptr_glProgramUniformMatrix2x3fv programUniformMatrix2x3fv;
	ptr_glProgramUniformMatrix3x2fv programUniformMatrix3x2fv;
	ptr_glProgramUniformMatrix2x4fv programUniformMatrix2x4fv;
	ptr_glProgramUniformMatrix4x2fv programUniformMatrix4x2fv;
	ptr_glProgramUniformMatrix3x4fv programUniformMatrix3x4fv;
	ptr_glProgramUniformMatrix4x3fv programUniformMatrix4x3fv;
	ptr_glProgramUniformMatrix2x3dv programUniformMatrix2x3dv;
	ptr_glProgramUniformMatrix3x2dv programUniformMatrix3x2dv;
	ptr_glProgramUniformMatrix2x4dv programUniformMatrix2x4dv;
	ptr_glProgramUniformMatrix4x2dv programUniformMatrix4x2dv;
	ptr_glProgramUniformMatrix3x4dv programUniformMatrix3x4dv;
	ptr_glProgramUniformMatrix4x3dv programUniformMatrix4x3dv;
	ptr_glValidateProgramPipeline validateProgramPipeline;
	ptr_glGetProgramPipelineInfoLog getProgramPipelineInfoLog;
	ptr_glVertexAttribL1d vertexAttribL1d;
	ptr_glVertexAttribL2d vertexAttribL2d;
	ptr_glVertexAttribL3d vertexAttribL3d;
	ptr_glVertexAttribL4d vertexAttribL4d;
	ptr_glVertexAttribL1dv vertexAttribL1dv;
	ptr_glVertexAttribL2dv vertexAttribL2dv;
	ptr_glVertexAttribL3dv vertexAttribL3dv;
	ptr_glVertexAttribL4dv vertexAttribL4dv;
	ptr_glVertexAttribLPointer vertexAttribLPointer;
	ptr_glGetVertexAttribLdv getVertexAttribLdv;
	ptr_glViewportArrayv viewportArrayv;
	ptr_glViewportIndexedf viewportIndexedf;
	ptr_glViewportIndexedfv viewportIndexedfv;
	ptr_glScissorArrayv scissorArrayv;
	ptr_glScissorIndexed scissorIndexed;
	ptr_glScissorIndexedv scissorIndexedv;
	ptr_glDepthRangeArrayv depthRangeArrayv;
	ptr_glDepthRangeIndexed depthRangeIndexed;
	ptr_glGetFloati_v getFloati_v;
	ptr_glGetDoublei_v getDoublei_v;
	ptr_glCreateSyncFromCLeventARB createSyncFromCLeventARB;
	ptr_glDebugMessageControlARB debugMessageControlARB;
	ptr_glDebugMessageInsertARB debugMessageInsertARB;
	ptr_glDebugMessageCallbackARB debugMessageCallbackARB;
	ptr_glGetDebugMessageLogARB getDebugMessageLogARB;
	ptr_glGetGraphicsResetStatusARB getGraphicsResetStatusARB;
	ptr_glGetnMapdvARB getnMapdvARB;
	ptr_glGetnMapfvARB getnMapfvARB;
	ptr_glGetnMapivARB getnMapivARB;
	ptr_glGetnPixelMapfvARB getnPixelMapfvARB;
	ptr_glGetnPixelMapuivARB getnPixelMapuivARB;
	ptr_glGetnPixelMapusvARB getnPixelMapusvARB;
	ptr_glGetnPolygonStippleARB getnPolygonStippleARB;
	ptr_glGetnColorTableARB getnColorTableARB;
	ptr_glGetnConvolutionFilterARB getnConvolutionFilterARB;
	ptr_glGetnSeparableFilterARB getnSeparableFilterARB;
	ptr_glGetnHistogramARB getnHistogramARB;
	ptr_glGetnMinmaxARB getnMinmaxARB;
	ptr_glGetnTexImageARB getnTexImageARB;
	ptr_glReadnPixelsARB readnPixelsARB;
	ptr_glGetnCompressedTexImageARB getnCompressedTexImageARB;
	ptr_glGetnUniformfvARB getnUniformfvARB;
	ptr_glGetnUniformivARB getnUniformivARB;
	ptr_glGetnUniformuivARB getnUniformuivARB;
	ptr_glGetnUniformdvARB getnUniformdvARB;

/// \}
};

#define clCullFace CL_OpenGL::functions->cullFace
#define clFrontFace CL_OpenGL::functions->frontFace
#define clHint CL_OpenGL::functions->hint
#define clLineWidth CL_OpenGL::functions->lineWidth
#define clPointSize CL_OpenGL::functions->pointSize
#define clPolygonMode CL_OpenGL::functions->polygonMode
#define clScissor CL_OpenGL::functions->scissor
#define clTexParameterf CL_OpenGL::functions->texParameterf
#define clTexParameterfv CL_OpenGL::functions->texParameterfv
#define clTexParameteri CL_OpenGL::functions->texParameteri
#define clTexParameteriv CL_OpenGL::functions->texParameteriv
#define clTexImage1D CL_OpenGL::functions->texImage1D
#define clTexImage2D CL_OpenGL::functions->texImage2D
#define clDrawBuffer CL_OpenGL::functions->drawBuffer
#define clClear CL_OpenGL::functions->clear
#define clClearColor CL_OpenGL::functions->clearColor
#define clClearStencil CL_OpenGL::functions->clearStencil
#define clClearDepth CL_OpenGL::functions->clearDepth
#define clStencilMask CL_OpenGL::functions->stencilMask
#define clColorMask CL_OpenGL::functions->colorMask
#define clDepthMask CL_OpenGL::functions->depthMask
#define clDisable CL_OpenGL::functions->disable
#define clEnable CL_OpenGL::functions->enable
#define clFinish CL_OpenGL::functions->finish
#define clFlush CL_OpenGL::functions->flush
#define clBlendFunc CL_OpenGL::functions->blendFunc
#define clLogicOp CL_OpenGL::functions->logicOp
#define clStencilFunc CL_OpenGL::functions->stencilFunc
#define clStencilOp CL_OpenGL::functions->stencilOp
#define clDepthFunc CL_OpenGL::functions->depthFunc
#define clPixelStoref CL_OpenGL::functions->pixelStoref
#define clPixelStorei CL_OpenGL::functions->pixelStorei
#define clReadBuffer CL_OpenGL::functions->readBuffer
#define clReadPixels CL_OpenGL::functions->readPixels
#define clGetBooleanv CL_OpenGL::functions->getBooleanv
#define clGetDoublev CL_OpenGL::functions->getDoublev
#define clGetError CL_OpenGL::functions->getError
#define clGetFloatv CL_OpenGL::functions->getFloatv
#define clGetIntegerv CL_OpenGL::functions->getIntegerv
#define clGetString CL_OpenGL::functions->getString
#define clGetTexImage CL_OpenGL::functions->getTexImage
#define clGetTexParameterfv CL_OpenGL::functions->getTexParameterfv
#define clGetTexParameteriv CL_OpenGL::functions->getTexParameteriv
#define clGetTexLevelParameterfv CL_OpenGL::functions->getTexLevelParameterfv
#define clGetTexLevelParameteriv CL_OpenGL::functions->getTexLevelParameteriv
#define clIsEnabled CL_OpenGL::functions->isEnabled
#define clDepthRange CL_OpenGL::functions->depthRange
#define clViewport CL_OpenGL::functions->viewport
#define clDrawArrays CL_OpenGL::functions->drawArrays
#define clDrawElements CL_OpenGL::functions->drawElements
#define clGetPointerv CL_OpenGL::functions->getPointerv
#define clPolygonOffset CL_OpenGL::functions->polygonOffset
#define clCopyTexImage1D CL_OpenGL::functions->copyTexImage1D
#define clCopyTexImage2D CL_OpenGL::functions->copyTexImage2D
#define clCopyTexSubImage1D CL_OpenGL::functions->copyTexSubImage1D
#define clCopyTexSubImage2D CL_OpenGL::functions->copyTexSubImage2D
#define clTexSubImage1D CL_OpenGL::functions->texSubImage1D
#define clTexSubImage2D CL_OpenGL::functions->texSubImage2D
#define clBindTexture CL_OpenGL::functions->bindTexture
#define clDeleteTextures CL_OpenGL::functions->deleteTextures
#define clGenTextures CL_OpenGL::functions->genTextures
#define clIsTexture CL_OpenGL::functions->isTexture
#define clBlendColor CL_OpenGL::functions->blendColor
#define clBlendEquation CL_OpenGL::functions->blendEquation
#define clDrawRangeElements CL_OpenGL::functions->drawRangeElements
#define clTexImage3D CL_OpenGL::functions->texImage3D
#define clTexSubImage3D CL_OpenGL::functions->texSubImage3D
#define clCopyTexSubImage3D CL_OpenGL::functions->copyTexSubImage3D
#define clActiveTexture CL_OpenGL::functions->activeTexture
#define clSampleCoverage CL_OpenGL::functions->sampleCoverage
#define clCompressedTexImage3D CL_OpenGL::functions->compressedTexImage3D
#define clCompressedTexImage2D CL_OpenGL::functions->compressedTexImage2D
#define clCompressedTexImage1D CL_OpenGL::functions->compressedTexImage1D
#define clCompressedTexSubImage3D CL_OpenGL::functions->compressedTexSubImage3D
#define clCompressedTexSubImage2D CL_OpenGL::functions->compressedTexSubImage2D
#define clCompressedTexSubImage1D CL_OpenGL::functions->compressedTexSubImage1D
#define clGetCompressedTexImage CL_OpenGL::functions->getCompressedTexImage
#define clBlendFuncSeparate CL_OpenGL::functions->blendFuncSeparate
#define clMultiDrawArrays CL_OpenGL::functions->multiDrawArrays
#define clMultiDrawElements CL_OpenGL::functions->multiDrawElements
#define clPointParameterf CL_OpenGL::functions->pointParameterf
#define clPointParameterfv CL_OpenGL::functions->pointParameterfv
#define clPointParameteri CL_OpenGL::functions->pointParameteri
#define clPointParameteriv CL_OpenGL::functions->pointParameteriv
#define clGenQueries CL_OpenGL::functions->genQueries
#define clDeleteQueries CL_OpenGL::functions->deleteQueries
#define clIsQuery CL_OpenGL::functions->isQuery
#define clBeginQuery CL_OpenGL::functions->beginQuery
#define clEndQuery CL_OpenGL::functions->endQuery
#define clGetQueryiv CL_OpenGL::functions->getQueryiv
#define clGetQueryObjectiv CL_OpenGL::functions->getQueryObjectiv
#define clGetQueryObjectuiv CL_OpenGL::functions->getQueryObjectuiv
#define clBindBuffer CL_OpenGL::functions->bindBuffer
#define clDeleteBuffers CL_OpenGL::functions->deleteBuffers
#define clGenBuffers CL_OpenGL::functions->genBuffers
#define clIsBuffer CL_OpenGL::functions->isBuffer
#define clBufferData CL_OpenGL::functions->bufferData
#define clBufferSubData CL_OpenGL::functions->bufferSubData
#define clGetBufferSubData CL_OpenGL::functions->getBufferSubData
#define clMapBuffer CL_OpenGL::functions->mapBuffer
#define clUnmapBuffer CL_OpenGL::functions->unmapBuffer
#define clGetBufferParameteriv CL_OpenGL::functions->getBufferParameteriv
#define clGetBufferPointerv CL_OpenGL::functions->getBufferPointerv
#define clBlendEquationSeparate CL_OpenGL::functions->blendEquationSeparate
#define clDrawBuffers CL_OpenGL::functions->drawBuffers
#define clStencilOpSeparate CL_OpenGL::functions->stencilOpSeparate
#define clStencilFuncSeparate CL_OpenGL::functions->stencilFuncSeparate
#define clStencilMaskSeparate CL_OpenGL::functions->stencilMaskSeparate
#define clAttachShader CL_OpenGL::functions->attachShader
#define clBindAttribLocation CL_OpenGL::functions->bindAttribLocation
#define clCompileShader CL_OpenGL::functions->compileShader
#define clCreateProgram CL_OpenGL::functions->createProgram
#define clCreateShader CL_OpenGL::functions->createShader
#define clDeleteProgram CL_OpenGL::functions->deleteProgram
#define clDeleteShader CL_OpenGL::functions->deleteShader
#define clDetachShader CL_OpenGL::functions->detachShader
#define clDisableVertexAttribArray CL_OpenGL::functions->disableVertexAttribArray
#define clEnableVertexAttribArray CL_OpenGL::functions->enableVertexAttribArray
#define clGetActiveAttrib CL_OpenGL::functions->getActiveAttrib
#define clGetActiveUniform CL_OpenGL::functions->getActiveUniform
#define clGetAttachedShaders CL_OpenGL::functions->getAttachedShaders
#define clGetAttribLocation CL_OpenGL::functions->getAttribLocation
#define clGetProgramiv CL_OpenGL::functions->getProgramiv
#define clGetProgramInfoLog CL_OpenGL::functions->getProgramInfoLog
#define clGetShaderiv CL_OpenGL::functions->getShaderiv
#define clGetShaderInfoLog CL_OpenGL::functions->getShaderInfoLog
#define clGetShaderSource CL_OpenGL::functions->getShaderSource
#define clGetUniformLocation CL_OpenGL::functions->getUniformLocation
#define clGetUniformfv CL_OpenGL::functions->getUniformfv
#define clGetUniformiv CL_OpenGL::functions->getUniformiv
#define clGetVertexAttribdv CL_OpenGL::functions->getVertexAttribdv
#define clGetVertexAttribfv CL_OpenGL::functions->getVertexAttribfv
#define clGetVertexAttribiv CL_OpenGL::functions->getVertexAttribiv
#define clGetVertexAttribPointerv CL_OpenGL::functions->getVertexAttribPointerv
#define clIsProgram CL_OpenGL::functions->isProgram
#define clIsShader CL_OpenGL::functions->isShader
#define clLinkProgram CL_OpenGL::functions->linkProgram
#define clShaderSource CL_OpenGL::functions->shaderSource
#define clUseProgram CL_OpenGL::functions->useProgram
#define clUniform1f CL_OpenGL::functions->uniform1f
#define clUniform2f CL_OpenGL::functions->uniform2f
#define clUniform3f CL_OpenGL::functions->uniform3f
#define clUniform4f CL_OpenGL::functions->uniform4f
#define clUniform1i CL_OpenGL::functions->uniform1i
#define clUniform2i CL_OpenGL::functions->uniform2i
#define clUniform3i CL_OpenGL::functions->uniform3i
#define clUniform4i CL_OpenGL::functions->uniform4i
#define clUniform1fv CL_OpenGL::functions->uniform1fv
#define clUniform2fv CL_OpenGL::functions->uniform2fv
#define clUniform3fv CL_OpenGL::functions->uniform3fv
#define clUniform4fv CL_OpenGL::functions->uniform4fv
#define clUniform1iv CL_OpenGL::functions->uniform1iv
#define clUniform2iv CL_OpenGL::functions->uniform2iv
#define clUniform3iv CL_OpenGL::functions->uniform3iv
#define clUniform4iv CL_OpenGL::functions->uniform4iv
#define clUniformMatrix2fv CL_OpenGL::functions->uniformMatrix2fv
#define clUniformMatrix3fv CL_OpenGL::functions->uniformMatrix3fv
#define clUniformMatrix4fv CL_OpenGL::functions->uniformMatrix4fv
#define clValidateProgram CL_OpenGL::functions->validateProgram
#define clVertexAttrib1d CL_OpenGL::functions->vertexAttrib1d
#define clVertexAttrib1dv CL_OpenGL::functions->vertexAttrib1dv
#define clVertexAttrib1f CL_OpenGL::functions->vertexAttrib1f
#define clVertexAttrib1fv CL_OpenGL::functions->vertexAttrib1fv
#define clVertexAttrib1s CL_OpenGL::functions->vertexAttrib1s
#define clVertexAttrib1sv CL_OpenGL::functions->vertexAttrib1sv
#define clVertexAttrib2d CL_OpenGL::functions->vertexAttrib2d
#define clVertexAttrib2dv CL_OpenGL::functions->vertexAttrib2dv
#define clVertexAttrib2f CL_OpenGL::functions->vertexAttrib2f
#define clVertexAttrib2fv CL_OpenGL::functions->vertexAttrib2fv
#define clVertexAttrib2s CL_OpenGL::functions->vertexAttrib2s
#define clVertexAttrib2sv CL_OpenGL::functions->vertexAttrib2sv
#define clVertexAttrib3d CL_OpenGL::functions->vertexAttrib3d
#define clVertexAttrib3dv CL_OpenGL::functions->vertexAttrib3dv
#define clVertexAttrib3f CL_OpenGL::functions->vertexAttrib3f
#define clVertexAttrib3fv CL_OpenGL::functions->vertexAttrib3fv
#define clVertexAttrib3s CL_OpenGL::functions->vertexAttrib3s
#define clVertexAttrib3sv CL_OpenGL::functions->vertexAttrib3sv
#define clVertexAttrib4Nbv CL_OpenGL::functions->vertexAttrib4Nbv
#define clVertexAttrib4Niv CL_OpenGL::functions->vertexAttrib4Niv
#define clVertexAttrib4Nsv CL_OpenGL::functions->vertexAttrib4Nsv
#define clVertexAttrib4Nub CL_OpenGL::functions->vertexAttrib4Nub
#define clVertexAttrib4Nubv CL_OpenGL::functions->vertexAttrib4Nubv
#define clVertexAttrib4Nuiv CL_OpenGL::functions->vertexAttrib4Nuiv
#define clVertexAttrib4Nusv CL_OpenGL::functions->vertexAttrib4Nusv
#define clVertexAttrib4bv CL_OpenGL::functions->vertexAttrib4bv
#define clVertexAttrib4d CL_OpenGL::functions->vertexAttrib4d
#define clVertexAttrib4dv CL_OpenGL::functions->vertexAttrib4dv
#define clVertexAttrib4f CL_OpenGL::functions->vertexAttrib4f
#define clVertexAttrib4fv CL_OpenGL::functions->vertexAttrib4fv
#define clVertexAttrib4iv CL_OpenGL::functions->vertexAttrib4iv
#define clVertexAttrib4s CL_OpenGL::functions->vertexAttrib4s
#define clVertexAttrib4sv CL_OpenGL::functions->vertexAttrib4sv
#define clVertexAttrib4ubv CL_OpenGL::functions->vertexAttrib4ubv
#define clVertexAttrib4uiv CL_OpenGL::functions->vertexAttrib4uiv
#define clVertexAttrib4usv CL_OpenGL::functions->vertexAttrib4usv
#define clVertexAttribPointer CL_OpenGL::functions->vertexAttribPointer
#define clUniformMatrix2x3fv CL_OpenGL::functions->uniformMatrix2x3fv
#define clUniformMatrix3x2fv CL_OpenGL::functions->uniformMatrix3x2fv
#define clUniformMatrix2x4fv CL_OpenGL::functions->uniformMatrix2x4fv
#define clUniformMatrix4x2fv CL_OpenGL::functions->uniformMatrix4x2fv
#define clUniformMatrix3x4fv CL_OpenGL::functions->uniformMatrix3x4fv
#define clUniformMatrix4x3fv CL_OpenGL::functions->uniformMatrix4x3fv
#define clColorMaski CL_OpenGL::functions->colorMaski
#define clGetBooleani_v CL_OpenGL::functions->getBooleani_v
#define clGetIntegeri_v CL_OpenGL::functions->getIntegeri_v
#define clEnablei CL_OpenGL::functions->enablei
#define clDisablei CL_OpenGL::functions->disablei
#define clIsEnabledi CL_OpenGL::functions->isEnabledi
#define clBeginTransformFeedback CL_OpenGL::functions->beginTransformFeedback
#define clEndTransformFeedback CL_OpenGL::functions->endTransformFeedback
#define clBindBufferRange CL_OpenGL::functions->bindBufferRange
#define clBindBufferBase CL_OpenGL::functions->bindBufferBase
#define clTransformFeedbackVaryings CL_OpenGL::functions->transformFeedbackVaryings
#define clGetTransformFeedbackVarying CL_OpenGL::functions->getTransformFeedbackVarying
#define clClampColor CL_OpenGL::functions->clampColor
#define clBeginConditionalRender CL_OpenGL::functions->beginConditionalRender
#define clEndConditionalRender CL_OpenGL::functions->endConditionalRender
#define clVertexAttribIPointer CL_OpenGL::functions->vertexAttribIPointer
#define clGetVertexAttribIiv CL_OpenGL::functions->getVertexAttribIiv
#define clGetVertexAttribIuiv CL_OpenGL::functions->getVertexAttribIuiv
#define clVertexAttribI1i CL_OpenGL::functions->vertexAttribI1i
#define clVertexAttribI2i CL_OpenGL::functions->vertexAttribI2i
#define clVertexAttribI3i CL_OpenGL::functions->vertexAttribI3i
#define clVertexAttribI4i CL_OpenGL::functions->vertexAttribI4i
#define clVertexAttribI1ui CL_OpenGL::functions->vertexAttribI1ui
#define clVertexAttribI2ui CL_OpenGL::functions->vertexAttribI2ui
#define clVertexAttribI3ui CL_OpenGL::functions->vertexAttribI3ui
#define clVertexAttribI4ui CL_OpenGL::functions->vertexAttribI4ui
#define clVertexAttribI1iv CL_OpenGL::functions->vertexAttribI1iv
#define clVertexAttribI2iv CL_OpenGL::functions->vertexAttribI2iv
#define clVertexAttribI3iv CL_OpenGL::functions->vertexAttribI3iv
#define clVertexAttribI4iv CL_OpenGL::functions->vertexAttribI4iv
#define clVertexAttribI1uiv CL_OpenGL::functions->vertexAttribI1uiv
#define clVertexAttribI2uiv CL_OpenGL::functions->vertexAttribI2uiv
#define clVertexAttribI3uiv CL_OpenGL::functions->vertexAttribI3uiv
#define clVertexAttribI4uiv CL_OpenGL::functions->vertexAttribI4uiv
#define clVertexAttribI4bv CL_OpenGL::functions->vertexAttribI4bv
#define clVertexAttribI4sv CL_OpenGL::functions->vertexAttribI4sv
#define clVertexAttribI4ubv CL_OpenGL::functions->vertexAttribI4ubv
#define clVertexAttribI4usv CL_OpenGL::functions->vertexAttribI4usv
#define clGetUniformuiv CL_OpenGL::functions->getUniformuiv
#define clBindFragDataLocation CL_OpenGL::functions->bindFragDataLocation
#define clGetFragDataLocation CL_OpenGL::functions->getFragDataLocation
#define clUniform1ui CL_OpenGL::functions->uniform1ui
#define clUniform2ui CL_OpenGL::functions->uniform2ui
#define clUniform3ui CL_OpenGL::functions->uniform3ui
#define clUniform4ui CL_OpenGL::functions->uniform4ui
#define clUniform1uiv CL_OpenGL::functions->uniform1uiv
#define clUniform2uiv CL_OpenGL::functions->uniform2uiv
#define clUniform3uiv CL_OpenGL::functions->uniform3uiv
#define clUniform4uiv CL_OpenGL::functions->uniform4uiv
#define clTexParameterIiv CL_OpenGL::functions->texParameterIiv
#define clTexParameterIuiv CL_OpenGL::functions->texParameterIuiv
#define clGetTexParameterIiv CL_OpenGL::functions->getTexParameterIiv
#define clGetTexParameterIuiv CL_OpenGL::functions->getTexParameterIuiv
#define clClearBufferiv CL_OpenGL::functions->clearBufferiv
#define clClearBufferuiv CL_OpenGL::functions->clearBufferuiv
#define clClearBufferfv CL_OpenGL::functions->clearBufferfv
#define clClearBufferfi CL_OpenGL::functions->clearBufferfi
#define clGetStringi CL_OpenGL::functions->getStringi
#define clDrawArraysInstanced CL_OpenGL::functions->drawArraysInstanced
#define clDrawElementsInstanced CL_OpenGL::functions->drawElementsInstanced
#define clTexBuffer CL_OpenGL::functions->texBuffer
#define clPrimitiveRestartIndex CL_OpenGL::functions->primitiveRestartIndex
#define clGetInteger64i_v CL_OpenGL::functions->getInteger64i_v
#define clGetBufferParameteri64v CL_OpenGL::functions->getBufferParameteri64v
#define clFramebufferTexture CL_OpenGL::functions->framebufferTexture
#define clVertexAttribDivisor CL_OpenGL::functions->vertexAttribDivisor
#define clMinSampleShading CL_OpenGL::functions->minSampleShading
#define clBlendEquationi CL_OpenGL::functions->blendEquationi
#define clBlendEquationSeparatei CL_OpenGL::functions->blendEquationSeparatei
#define clBlendFunci CL_OpenGL::functions->blendFunci
#define clBlendFuncSeparatei CL_OpenGL::functions->blendFuncSeparatei
#define clIsRenderbuffer CL_OpenGL::functions->isRenderbuffer
#define clBindRenderbuffer CL_OpenGL::functions->bindRenderbuffer
#define clDeleteRenderbuffers CL_OpenGL::functions->deleteRenderbuffers
#define clGenRenderbuffers CL_OpenGL::functions->genRenderbuffers
#define clRenderbufferStorage CL_OpenGL::functions->renderbufferStorage
#define clGetRenderbufferParameteriv CL_OpenGL::functions->getRenderbufferParameteriv
#define clIsFramebuffer CL_OpenGL::functions->isFramebuffer
#define clBindFramebuffer CL_OpenGL::functions->bindFramebuffer
#define clDeleteFramebuffers CL_OpenGL::functions->deleteFramebuffers
#define clGenFramebuffers CL_OpenGL::functions->genFramebuffers
#define clCheckFramebufferStatus CL_OpenGL::functions->checkFramebufferStatus
#define clFramebufferTexture1D CL_OpenGL::functions->framebufferTexture1D
#define clFramebufferTexture2D CL_OpenGL::functions->framebufferTexture2D
#define clFramebufferTexture3D CL_OpenGL::functions->framebufferTexture3D
#define clFramebufferRenderbuffer CL_OpenGL::functions->framebufferRenderbuffer
#define clGetFramebufferAttachmentParameteriv CL_OpenGL::functions->getFramebufferAttachmentParameteriv
#define clGenerateMipmap CL_OpenGL::functions->generateMipmap
#define clBlitFramebuffer CL_OpenGL::functions->blitFramebuffer
#define clRenderbufferStorageMultisample CL_OpenGL::functions->renderbufferStorageMultisample
#define clFramebufferTextureLayer CL_OpenGL::functions->framebufferTextureLayer
#define clMapBufferRange CL_OpenGL::functions->mapBufferRange
#define clFlushMappedBufferRange CL_OpenGL::functions->flushMappedBufferRange
#define clBindVertexArray CL_OpenGL::functions->bindVertexArray
#define clDeleteVertexArrays CL_OpenGL::functions->deleteVertexArrays
#define clGenVertexArrays CL_OpenGL::functions->genVertexArrays
#define clIsVertexArray CL_OpenGL::functions->isVertexArray
#define clGetUniformIndices CL_OpenGL::functions->getUniformIndices
#define clGetActiveUniformsiv CL_OpenGL::functions->getActiveUniformsiv
#define clGetActiveUniformName CL_OpenGL::functions->getActiveUniformName
#define clGetUniformBlockIndex CL_OpenGL::functions->getUniformBlockIndex
#define clGetActiveUniformBlockiv CL_OpenGL::functions->getActiveUniformBlockiv
#define clGetActiveUniformBlockName CL_OpenGL::functions->getActiveUniformBlockName
#define clUniformBlockBinding CL_OpenGL::functions->uniformBlockBinding
#define clCopyBufferSubData CL_OpenGL::functions->copyBufferSubData
#define clDrawElementsBaseVertex CL_OpenGL::functions->drawElementsBaseVertex
#define clDrawRangeElementsBaseVertex CL_OpenGL::functions->drawRangeElementsBaseVertex
#define clDrawElementsInstancedBaseVertex CL_OpenGL::functions->drawElementsInstancedBaseVertex
#define clMultiDrawElementsBaseVertex CL_OpenGL::functions->multiDrawElementsBaseVertex
#define clProvokingVertex CL_OpenGL::functions->provokingVertex
#define clFenceSync CL_OpenGL::functions->fenceSync
#define clIsSync CL_OpenGL::functions->isSync
#define clDeleteSync CL_OpenGL::functions->deleteSync
#define clClientWaitSync CL_OpenGL::functions->clientWaitSync
#define clWaitSync CL_OpenGL::functions->waitSync
#define clGetInteger64v CL_OpenGL::functions->getInteger64v
#define clGetSynciv CL_OpenGL::functions->getSynciv
#define clTexImage2DMultisample CL_OpenGL::functions->texImage2DMultisample
#define clTexImage3DMultisample CL_OpenGL::functions->texImage3DMultisample
#define clGetMultisamplefv CL_OpenGL::functions->getMultisamplefv
#define clSampleMaski CL_OpenGL::functions->sampleMaski
#define clBlendEquationiARB CL_OpenGL::functions->blendEquationiARB
#define clBlendEquationSeparateiARB CL_OpenGL::functions->blendEquationSeparateiARB
#define clBlendFunciARB CL_OpenGL::functions->blendFunciARB
#define clBlendFuncSeparateiARB CL_OpenGL::functions->blendFuncSeparateiARB
#define clMinSampleShadingARB CL_OpenGL::functions->minSampleShadingARB
#define clNamedStringARB CL_OpenGL::functions->namedStringARB
#define clDeleteNamedStringARB CL_OpenGL::functions->deleteNamedStringARB
#define clCompileShaderIncludeARB CL_OpenGL::functions->compileShaderIncludeARB
#define clIsNamedStringARB CL_OpenGL::functions->isNamedStringARB
#define clGetNamedStringARB CL_OpenGL::functions->getNamedStringARB
#define clGetNamedStringivARB CL_OpenGL::functions->getNamedStringivARB
#define clBindFragDataLocationIndexed CL_OpenGL::functions->bindFragDataLocationIndexed
#define clGetFragDataIndex CL_OpenGL::functions->getFragDataIndex
#define clGenSamplers CL_OpenGL::functions->genSamplers
#define clDeleteSamplers CL_OpenGL::functions->deleteSamplers
#define clIsSampler CL_OpenGL::functions->isSampler
#define clBindSampler CL_OpenGL::functions->bindSampler
#define clSamplerParameteri CL_OpenGL::functions->samplerParameteri
#define clSamplerParameteriv CL_OpenGL::functions->samplerParameteriv
#define clSamplerParameterf CL_OpenGL::functions->samplerParameterf
#define clSamplerParameterfv CL_OpenGL::functions->samplerParameterfv
#define clSamplerParameterIiv CL_OpenGL::functions->samplerParameterIiv
#define clSamplerParameterIuiv CL_OpenGL::functions->samplerParameterIuiv
#define clGetSamplerParameteriv CL_OpenGL::functions->getSamplerParameteriv
#define clGetSamplerParameterIiv CL_OpenGL::functions->getSamplerParameterIiv
#define clGetSamplerParameterfv CL_OpenGL::functions->getSamplerParameterfv
#define clGetSamplerParameterIuiv CL_OpenGL::functions->getSamplerParameterIuiv
#define clQueryCounter CL_OpenGL::functions->queryCounter
#define clGetQueryObjecti64v CL_OpenGL::functions->getQueryObjecti64v
#define clGetQueryObjectui64v CL_OpenGL::functions->getQueryObjectui64v
#define clVertexP2ui CL_OpenGL::functions->vertexP2ui
#define clVertexP2uiv CL_OpenGL::functions->vertexP2uiv
#define clVertexP3ui CL_OpenGL::functions->vertexP3ui
#define clVertexP3uiv CL_OpenGL::functions->vertexP3uiv
#define clVertexP4ui CL_OpenGL::functions->vertexP4ui
#define clVertexP4uiv CL_OpenGL::functions->vertexP4uiv
#define clTexCoordP1ui CL_OpenGL::functions->texCoordP1ui
#define clTexCoordP1uiv CL_OpenGL::functions->texCoordP1uiv
#define clTexCoordP2ui CL_OpenGL::functions->texCoordP2ui
#define clTexCoordP2uiv CL_OpenGL::functions->texCoordP2uiv
#define clTexCoordP3ui CL_OpenGL::functions->texCoordP3ui
#define clTexCoordP3uiv CL_OpenGL::functions->texCoordP3uiv
#define clTexCoordP4ui CL_OpenGL::functions->texCoordP4ui
#define clTexCoordP4uiv CL_OpenGL::functions->texCoordP4uiv
#define clMultiTexCoordP1ui CL_OpenGL::functions->multiTexCoordP1ui
#define clMultiTexCoordP1uiv CL_OpenGL::functions->multiTexCoordP1uiv
#define clMultiTexCoordP2ui CL_OpenGL::functions->multiTexCoordP2ui
#define clMultiTexCoordP2uiv CL_OpenGL::functions->multiTexCoordP2uiv
#define clMultiTexCoordP3ui CL_OpenGL::functions->multiTexCoordP3ui
#define clMultiTexCoordP3uiv CL_OpenGL::functions->multiTexCoordP3uiv
#define clMultiTexCoordP4ui CL_OpenGL::functions->multiTexCoordP4ui
#define clMultiTexCoordP4uiv CL_OpenGL::functions->multiTexCoordP4uiv
#define clNormalP3ui CL_OpenGL::functions->normalP3ui
#define clNormalP3uiv CL_OpenGL::functions->normalP3uiv
#define clColorP3ui CL_OpenGL::functions->colorP3ui
#define clColorP3uiv CL_OpenGL::functions->colorP3uiv
#define clColorP4ui CL_OpenGL::functions->colorP4ui
#define clColorP4uiv CL_OpenGL::functions->colorP4uiv
#define clSecondaryColorP3ui CL_OpenGL::functions->secondaryColorP3ui
#define clSecondaryColorP3uiv CL_OpenGL::functions->secondaryColorP3uiv
#define clVertexAttribP1ui CL_OpenGL::functions->vertexAttribP1ui
#define clVertexAttribP1uiv CL_OpenGL::functions->vertexAttribP1uiv
#define clVertexAttribP2ui CL_OpenGL::functions->vertexAttribP2ui
#define clVertexAttribP2uiv CL_OpenGL::functions->vertexAttribP2uiv
#define clVertexAttribP3ui CL_OpenGL::functions->vertexAttribP3ui
#define clVertexAttribP3uiv CL_OpenGL::functions->vertexAttribP3uiv
#define clVertexAttribP4ui CL_OpenGL::functions->vertexAttribP4ui
#define clVertexAttribP4uiv CL_OpenGL::functions->vertexAttribP4uiv
#define clDrawArraysIndirect CL_OpenGL::functions->drawArraysIndirect
#define clDrawElementsIndirect CL_OpenGL::functions->drawElementsIndirect
#define clUniform1d CL_OpenGL::functions->uniform1d
#define clUniform2d CL_OpenGL::functions->uniform2d
#define clUniform3d CL_OpenGL::functions->uniform3d
#define clUniform4d CL_OpenGL::functions->uniform4d
#define clUniform1dv CL_OpenGL::functions->uniform1dv
#define clUniform2dv CL_OpenGL::functions->uniform2dv
#define clUniform3dv CL_OpenGL::functions->uniform3dv
#define clUniform4dv CL_OpenGL::functions->uniform4dv
#define clUniformMatrix2dv CL_OpenGL::functions->uniformMatrix2dv
#define clUniformMatrix3dv CL_OpenGL::functions->uniformMatrix3dv
#define clUniformMatrix4dv CL_OpenGL::functions->uniformMatrix4dv
#define clUniformMatrix2x3dv CL_OpenGL::functions->uniformMatrix2x3dv
#define clUniformMatrix2x4dv CL_OpenGL::functions->uniformMatrix2x4dv
#define clUniformMatrix3x2dv CL_OpenGL::functions->uniformMatrix3x2dv
#define clUniformMatrix3x4dv CL_OpenGL::functions->uniformMatrix3x4dv
#define clUniformMatrix4x2dv CL_OpenGL::functions->uniformMatrix4x2dv
#define clUniformMatrix4x3dv CL_OpenGL::functions->uniformMatrix4x3dv
#define clGetUniformdv CL_OpenGL::functions->getUniformdv
#define clGetSubroutineUniformLocation CL_OpenGL::functions->getSubroutineUniformLocation
#define clGetSubroutineIndex CL_OpenGL::functions->getSubroutineIndex
#define clGetActiveSubroutineUniformiv CL_OpenGL::functions->getActiveSubroutineUniformiv
#define clGetActiveSubroutineUniformName CL_OpenGL::functions->getActiveSubroutineUniformName
#define clGetActiveSubroutineName CL_OpenGL::functions->getActiveSubroutineName
#define clUniformSubroutinesuiv CL_OpenGL::functions->uniformSubroutinesuiv
#define clGetUniformSubroutineuiv CL_OpenGL::functions->getUniformSubroutineuiv
#define clGetProgramStageiv CL_OpenGL::functions->getProgramStageiv
#define clPatchParameteri CL_OpenGL::functions->patchParameteri
#define clPatchParameterfv CL_OpenGL::functions->patchParameterfv
#define clBindTransformFeedback CL_OpenGL::functions->bindTransformFeedback
#define clDeleteTransformFeedbacks CL_OpenGL::functions->deleteTransformFeedbacks
#define clGenTransformFeedbacks CL_OpenGL::functions->genTransformFeedbacks
#define clIsTransformFeedback CL_OpenGL::functions->isTransformFeedback
#define clPauseTransformFeedback CL_OpenGL::functions->pauseTransformFeedback
#define clResumeTransformFeedback CL_OpenGL::functions->resumeTransformFeedback
#define clDrawTransformFeedback CL_OpenGL::functions->drawTransformFeedback
#define clDrawTransformFeedbackStream CL_OpenGL::functions->drawTransformFeedbackStream
#define clBeginQueryIndexed CL_OpenGL::functions->beginQueryIndexed
#define clEndQueryIndexed CL_OpenGL::functions->endQueryIndexed
#define clGetQueryIndexediv CL_OpenGL::functions->getQueryIndexediv
#define clReleaseShaderCompiler CL_OpenGL::functions->releaseShaderCompiler
#define clShaderBinary CL_OpenGL::functions->shaderBinary
#define clGetShaderPrecisionFormat CL_OpenGL::functions->getShaderPrecisionFormat
#define clDepthRangef CL_OpenGL::functions->depthRangef
#define clClearDepthf CL_OpenGL::functions->clearDepthf
#define clGetProgramBinary CL_OpenGL::functions->getProgramBinary
#define clProgramBinary CL_OpenGL::functions->programBinary
#define clProgramParameteri CL_OpenGL::functions->programParameteri
#define clUseProgramStages CL_OpenGL::functions->useProgramStages
#define clActiveShaderProgram CL_OpenGL::functions->activeShaderProgram
#define clCreateShaderProgramv CL_OpenGL::functions->createShaderProgramv
#define clBindProgramPipeline CL_OpenGL::functions->bindProgramPipeline
#define clDeleteProgramPipelines CL_OpenGL::functions->deleteProgramPipelines
#define clGenProgramPipelines CL_OpenGL::functions->genProgramPipelines
#define clIsProgramPipeline CL_OpenGL::functions->isProgramPipeline
#define clGetProgramPipelineiv CL_OpenGL::functions->getProgramPipelineiv
#define clProgramUniform1i CL_OpenGL::functions->programUniform1i
#define clProgramUniform1iv CL_OpenGL::functions->programUniform1iv
#define clProgramUniform1f CL_OpenGL::functions->programUniform1f
#define clProgramUniform1fv CL_OpenGL::functions->programUniform1fv
#define clProgramUniform1d CL_OpenGL::functions->programUniform1d
#define clProgramUniform1dv CL_OpenGL::functions->programUniform1dv
#define clProgramUniform1ui CL_OpenGL::functions->programUniform1ui
#define clProgramUniform1uiv CL_OpenGL::functions->programUniform1uiv
#define clProgramUniform2i CL_OpenGL::functions->programUniform2i
#define clProgramUniform2iv CL_OpenGL::functions->programUniform2iv
#define clProgramUniform2f CL_OpenGL::functions->programUniform2f
#define clProgramUniform2fv CL_OpenGL::functions->programUniform2fv
#define clProgramUniform2d CL_OpenGL::functions->programUniform2d
#define clProgramUniform2dv CL_OpenGL::functions->programUniform2dv
#define clProgramUniform2ui CL_OpenGL::functions->programUniform2ui
#define clProgramUniform2uiv CL_OpenGL::functions->programUniform2uiv
#define clProgramUniform3i CL_OpenGL::functions->programUniform3i
#define clProgramUniform3iv CL_OpenGL::functions->programUniform3iv
#define clProgramUniform3f CL_OpenGL::functions->programUniform3f
#define clProgramUniform3fv CL_OpenGL::functions->programUniform3fv
#define clProgramUniform3d CL_OpenGL::functions->programUniform3d
#define clProgramUniform3dv CL_OpenGL::functions->programUniform3dv
#define clProgramUniform3ui CL_OpenGL::functions->programUniform3ui
#define clProgramUniform3uiv CL_OpenGL::functions->programUniform3uiv
#define clProgramUniform4i CL_OpenGL::functions->programUniform4i
#define clProgramUniform4iv CL_OpenGL::functions->programUniform4iv
#define clProgramUniform4f CL_OpenGL::functions->programUniform4f
#define clProgramUniform4fv CL_OpenGL::functions->programUniform4fv
#define clProgramUniform4d CL_OpenGL::functions->programUniform4d
#define clProgramUniform4dv CL_OpenGL::functions->programUniform4dv
#define clProgramUniform4ui CL_OpenGL::functions->programUniform4ui
#define clProgramUniform4uiv CL_OpenGL::functions->programUniform4uiv
#define clProgramUniformMatrix2fv CL_OpenGL::functions->programUniformMatrix2fv
#define clProgramUniformMatrix3fv CL_OpenGL::functions->programUniformMatrix3fv
#define clProgramUniformMatrix4fv CL_OpenGL::functions->programUniformMatrix4fv
#define clProgramUniformMatrix2dv CL_OpenGL::functions->programUniformMatrix2dv
#define clProgramUniformMatrix3dv CL_OpenGL::functions->programUniformMatrix3dv
#define clProgramUniformMatrix4dv CL_OpenGL::functions->programUniformMatrix4dv
#define clProgramUniformMatrix2x3fv CL_OpenGL::functions->programUniformMatrix2x3fv
#define clProgramUniformMatrix3x2fv CL_OpenGL::functions->programUniformMatrix3x2fv
#define clProgramUniformMatrix2x4fv CL_OpenGL::functions->programUniformMatrix2x4fv
#define clProgramUniformMatrix4x2fv CL_OpenGL::functions->programUniformMatrix4x2fv
#define clProgramUniformMatrix3x4fv CL_OpenGL::functions->programUniformMatrix3x4fv
#define clProgramUniformMatrix4x3fv CL_OpenGL::functions->programUniformMatrix4x3fv
#define clProgramUniformMatrix2x3dv CL_OpenGL::functions->programUniformMatrix2x3dv
#define clProgramUniformMatrix3x2dv CL_OpenGL::functions->programUniformMatrix3x2dv
#define clProgramUniformMatrix2x4dv CL_OpenGL::functions->programUniformMatrix2x4dv
#define clProgramUniformMatrix4x2dv CL_OpenGL::functions->programUniformMatrix4x2dv
#define clProgramUniformMatrix3x4dv CL_OpenGL::functions->programUniformMatrix3x4dv
#define clProgramUniformMatrix4x3dv CL_OpenGL::functions->programUniformMatrix4x3dv
#define clValidateProgramPipeline CL_OpenGL::functions->validateProgramPipeline
#define clGetProgramPipelineInfoLog CL_OpenGL::functions->getProgramPipelineInfoLog
#define clVertexAttribL1d CL_OpenGL::functions->vertexAttribL1d
#define clVertexAttribL2d CL_OpenGL::functions->vertexAttribL2d
#define clVertexAttribL3d CL_OpenGL::functions->vertexAttribL3d
#define clVertexAttribL4d CL_OpenGL::functions->vertexAttribL4d
#define clVertexAttribL1dv CL_OpenGL::functions->vertexAttribL1dv
#define clVertexAttribL2dv CL_OpenGL::functions->vertexAttribL2dv
#define clVertexAttribL3dv CL_OpenGL::functions->vertexAttribL3dv
#define clVertexAttribL4dv CL_OpenGL::functions->vertexAttribL4dv
#define clVertexAttribLPointer CL_OpenGL::functions->vertexAttribLPointer
#define clGetVertexAttribLdv CL_OpenGL::functions->getVertexAttribLdv
#define clViewportArrayv CL_OpenGL::functions->viewportArrayv
#define clViewportIndexedf CL_OpenGL::functions->viewportIndexedf
#define clViewportIndexedfv CL_OpenGL::functions->viewportIndexedfv
#define clScissorArrayv CL_OpenGL::functions->scissorArrayv
#define clScissorIndexed CL_OpenGL::functions->scissorIndexed
#define clScissorIndexedv CL_OpenGL::functions->scissorIndexedv
#define clDepthRangeArrayv CL_OpenGL::functions->depthRangeArrayv
#define clDepthRangeIndexed CL_OpenGL::functions->depthRangeIndexed
#define clGetFloati_v CL_OpenGL::functions->getFloati_v
#define clGetDoublei_v CL_OpenGL::functions->getDoublei_v
#define clCreateSyncFromCLeventARB CL_OpenGL::functions->createSyncFromCLeventARB
#define clDebugMessageControlARB CL_OpenGL::functions->debugMessageControlARB
#define clDebugMessageInsertARB CL_OpenGL::functions->debugMessageInsertARB
#define clDebugMessageCallbackARB CL_OpenGL::functions->debugMessageCallbackARB
#define clGetDebugMessageLogARB CL_OpenGL::functions->getDebugMessageLogARB
#define clGetGraphicsResetStatusARB CL_OpenGL::functions->getGraphicsResetStatusARB
#define clGetnMapdvARB CL_OpenGL::functions->getnMapdvARB
#define clGetnMapfvARB CL_OpenGL::functions->getnMapfvARB
#define clGetnMapivARB CL_OpenGL::functions->getnMapivARB
#define clGetnPixelMapfvARB CL_OpenGL::functions->getnPixelMapfvARB
#define clGetnPixelMapuivARB CL_OpenGL::functions->getnPixelMapuivARB
#define clGetnPixelMapusvARB CL_OpenGL::functions->getnPixelMapusvARB
#define clGetnPolygonStippleARB CL_OpenGL::functions->getnPolygonStippleARB
#define clGetnColorTableARB CL_OpenGL::functions->getnColorTableARB
#define clGetnConvolutionFilterARB CL_OpenGL::functions->getnConvolutionFilterARB
#define clGetnSeparableFilterARB CL_OpenGL::functions->getnSeparableFilterARB
#define clGetnHistogramARB CL_OpenGL::functions->getnHistogramARB
#define clGetnMinmaxARB CL_OpenGL::functions->getnMinmaxARB
#define clGetnTexImageARB CL_OpenGL::functions->getnTexImageARB
#define clReadnPixelsARB CL_OpenGL::functions->readnPixelsARB
#define clGetnCompressedTexImageARB CL_OpenGL::functions->getnCompressedTexImageARB
#define clGetnUniformfvARB CL_OpenGL::functions->getnUniformfvARB
#define clGetnUniformivARB CL_OpenGL::functions->getnUniformivARB
#define clGetnUniformuivARB CL_OpenGL::functions->getnUniformuivARB
#define clGetnUniformdvARB CL_OpenGL::functions->getnUniformdvARB

/// \}

/// \}

