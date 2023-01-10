/*
 * SDL_WindowClass.cpp
 *
 *  Created on: Mar 13, 2017
 *      Author: Bunjukz
 */

#include <Graphics/Window/SDL_WindowClass.h>

void SDL_WindowClass::SDLCreateWindow(int w, int h,unsigned int fps, const std::string& title, unsigned int flags) {
	if(window != nullptr)
		DeleteWindow();
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);
	m_w 	= w;
	m_h 	= h;
	m_title = title;
	m_fps 	= 1000/fps;
}

void SDL_WindowClass::DeleteWindow() {
	if(window != nullptr)
		SDL_DestroyWindow(window);
	window = nullptr;
}

void SDL_WindowClass::CreateRenderer(const SDL_RendererFlags& flags) {
	if(render != nullptr)
		DeleteRenderer();
	render = SDL_CreateRenderer(window, -1, flags);
}

void SDL_WindowClass::DeleteRenderer() {
	if(render != nullptr)
		SDL_DestroyRenderer(render);
	render = nullptr;
}

void SDL_WindowClass::SetFPS(int fps){
	m_fps = 1000/fps;
}

void SDL_WindowClass::LimitFPS(const Uint32& frameTicks){
	if(frameTicks < m_fps){
		SDL_Delay(m_fps - frameTicks);
	}
}
void SDL_WindowClass::GetDimensions(int* w_out, int* h_out){
	if(w_out != nullptr)
		*w_out = m_w;
	if(h_out != nullptr)
		*h_out = m_h;
}
