lib_LTLIBRARIES = libclan23GL1.la

libclan23GL1_la_SOURCES = \
	gl1_frame_buffer_provider.cpp \
	gl1_graphic_context_provider.cpp \
	gl1_render_buffer_provider.cpp \
	gl1_target.cpp \
	gl1_target_provider.cpp \
	gl1_texture_provider.cpp \
	gl1_window_description.cpp \
	graphic_context_gl1.cpp \
	light_model.cpp \
	light_source.cpp \
	material.cpp \
	opengl1.cpp \
	precomp.cpp \
	setup_gl1.cpp \
	texture_unit.cpp \
	gl1_frame_buffer_provider.h \
	gl1_graphic_context_provider.h \
	gl1_render_buffer_provider.h \
	gl1_target_provider.h \
	gl1_texture_provider.h \
	gl1_window_description_impl.h \
	gl1_window_provider.h \
	pbuffer.h \
	pbuffer.cpp \
	precomp.h
if WIN32
libclan23GL1_la_SOURCES += \
	WGL/pbuffer_impl.cpp \
	WGL/pbuffer_impl.h \
	WGL/gl1_creation_helper.cpp \
	WGL/gl1_creation_helper.h \
	WGL/gl1_window_provider_wgl.cpp \
	WGL/gl1_window_provider_wgl.h
else
libclan23GL1_la_SOURCES += \
	GLX/pbuffer_impl.h \
	GLX/pbuffer_impl.cpp \
	GLX/gl1_window_provider_glx.cpp \
	GLX/gl1_window_provider_glx.h
endif
libclan23GL1_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanGL1)

libclan23GL1_la_CXXFLAGS=$(clanGL1_CXXFLAGS) $(extra_CFLAGS_clanGL1)

# EOF #
