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
*/

#include "SWRender/precomp.h"
#include "pixel_command_bicubic.h"
#include "API/SWRender/pixel_thread_context.h"
#include "../Renderers/pixel_bicubic_renderer.h"

CL_PixelCommandBicubic::CL_PixelCommandBicubic(int x, int y, int zoom_number, int zoom_denominator, const CL_PixelBuffer &image)
: x(x), y(y), zoom_number(zoom_number), zoom_denominator(zoom_denominator), image(image)
{
}

void CL_PixelCommandBicubic::run(CL_PixelThreadContext *context)
{
	CL_PixelBicubicRenderer bicubic_renderer;
	bicubic_renderer.set_dest(context->colorbuffer0.data, context->colorbuffer0.size.width, context->colorbuffer0.size.height);
	bicubic_renderer.set_src((unsigned int*)image.get_data(), image.get_width(), image.get_height());
	bicubic_renderer.set_core(context->core, context->num_cores);
	bicubic_renderer.render(x, y, zoom_number, zoom_denominator);
}
