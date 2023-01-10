/*
 * Texture.h
 *
 *  Created on: Mar 18, 2016
 *      Author: Bunjukz
 */

#ifndef INCLUDES_GRAPHICS_TEXTURE_TEXTURE_H_
#define INCLUDES_GRAPHICS_TEXTURE_TEXTURE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <tgmath.h>

class TextureC {
public:
	TextureC();
	~TextureC();
	int 	LFF(std::string path, SDL_Renderer* mRender);
	void 	Render(int x ,int y ,int w ,int h , SDL_Rect* mRect = NULL ,SDL_Renderer* mRender = NULL);
	void 	RenderZoom(int x ,int y ,int w ,int h , SDL_Rect* mRect = NULL ,SDL_Renderer* mRender = NULL, float _zoom = 0.0);
	void 	RenderZoom(int, int, SDL_Renderer*, float);
	void	RenderZoomCentered(int x, int y, SDL_Renderer* mRenderer, float zoom);
	void	Clean();

	int 	GetWidth();
	int 	GetHeight();
private:
	SDL_Texture* mTexture;
	int mWidth, mHeight;
};



#endif /* INCLUDES_GRAPHICS_TEXTURE_TEXTURE_H_ */
