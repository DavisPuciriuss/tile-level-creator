/*
 * SDL_WindowClass.h
 *
 *  Created on: Mar 13, 2017
 *      Author: Bunjukz
 */

#ifndef INCLDS_GRAPHICS_WINDOW_SDL_WINDOWCLASS_H_
#define INCLDS_GRAPHICS_WINDOW_SDL_WINDOWCLASS_H_

#include <SDL.h>
#include <string>

class SDL_WindowClass {
public:
	void SDLCreateWindow(int w, int h,unsigned int fps, const std::string& title, unsigned int flags);
	void DeleteWindow();
	void CreateRenderer(const SDL_RendererFlags& flags);
	void DeleteRenderer();
	void SetFPS(int fps);
	void LimitFPS(const Uint32& frameTicks);
	void GetDimensions(int* w_out, int* h_out);
	int	 GetW() {return m_w;};
	int	 GetH() {return m_h;};
	SDL_Window* 		window = nullptr;
	SDL_Renderer* 		render = nullptr;
private:
	int 			m_w;
	int 			m_h;
	unsigned int	m_fps;
	std::string		m_title;
};



#endif /* INCLDS_GRAPHICS_WINDOW_SDL_WINDOWCLASS_H_ */
