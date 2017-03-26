// test_video.cpp
/*
 * FRT - A Godot platform targeting single board computers
 * Copyright (c) 2017  Emanuele Fornara
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define FRT_MOCK_GODOT_GL_CONTEXT
#include "frt.h"

#include <math.h>
#include <assert.h>
#include <unistd.h>

#include <GLES2/gl2.h>

#define FRT_IMPLEMENT_REGISTRY
#include "bits_frt.h"

using namespace frt;

int main(int argc, char *argv[]) {
	Video *video = (Video *)Registry::instance()->probe_single();
	assert(video);
	Vec2 size(640, 480);
	ContextGL *gl = video->create_the_gl_context(size);
	assert(gl);
	gl->initialize();
	gl->make_current();
	const int position_period = 300;
	const int visibility_period = 100;
	int y = 0;
	for (int i = 0; i < position_period; i++, y++) {
		video->move_pointer(Vec2(y, y));
		glClearColor(0., 0., sin((M_PI * i) / position_period), 1.);
		glClear(GL_COLOR_BUFFER_BIT);
		switch (i % visibility_period) {
			case 0:
				video->show_pointer(true);
				break;
			case visibility_period / 2:
				video->show_pointer(false);
				break;
			default:
				break;
		}
		gl->swap_buffers();
	}
	video->show_pointer(false); // !vsync disables pointer updates anyway
	gl->set_use_vsync(false);
	for (int i = 0; i < position_period; i++, y++) {
		video->move_pointer(Vec2(y, y));
		glClearColor(sin((M_PI * i) / position_period), 0., 0., 1.);
		glClear(GL_COLOR_BUFFER_BIT);
		gl->swap_buffers();
		usleep(2000); // too fast otherwise
	}
	video->cleanup();
}