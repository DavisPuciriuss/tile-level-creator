/*
 * TextTexture.h
 *
 *  Created on: Jun 6, 2016
 *      Author: Bunjukz
 */

#ifndef INCLDS_TEXT_TEXTTEXTURE_H_
#define INCLDS_TEXT_TEXTTEXTURE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

class TextC{
public:
	TextC();
	~TextC();

	int 	LFT(std::string,SDL_Renderer*,SDL_Color,TTF_Font*,int);
	void 	Render(int ,int ,SDL_Renderer* mRender = NULL);
	void	Clean();

	int 	GetWidth();
	int 	GetHeight();
private:
	std::string currentText;
	SDL_Texture* mTexture;
	int mWidth, mHeight;
};



#endif /* INCLDS_TEXT_TEXTTEXTURE_H_ */
