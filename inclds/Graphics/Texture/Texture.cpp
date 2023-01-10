/*
 * Texture.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: Bunjukz
 */

#include <Graphics/Texture/Texture.h>

TextureC::TextureC(){
	mWidth 		= 0;
	mHeight 	= 0;
	mTexture 	= NULL;
}

TextureC::~TextureC(){
	if(mTexture!=NULL){
		SDL_DestroyTexture(mTexture);
		mTexture 	= NULL;
		mWidth 		= 0;
		mHeight 	= 0;
	}
}
int TextureC::LFF(std::string path, SDL_Renderer* mRender){
	SDL_Texture* newTexture;
	SDL_Surface* mSurface = IMG_Load(path.c_str());
	if(mSurface == NULL){
		printf("Couldn't load texture: %s, ERROR: %s \n",path.c_str(),IMG_GetError());
		return 1;
	}else{
		newTexture = SDL_CreateTextureFromSurface(mRender, mSurface);
		if(!newTexture){
			printf("Couldn't create texture: %s, ERROR: %s \n",path.c_str(),SDL_GetError());
			return 1;
		}else{
			mHeight = mSurface->h;
			mWidth 	= mSurface->w;
		}
		SDL_FreeSurface(mSurface);
	}
	mTexture = newTexture;
	return 0;

}

void TextureC::Clean(){
	SDL_DestroyTexture(mTexture);
	mWidth 	= 0;
	mHeight = 0;
}

void TextureC::Render(int x, int y, int w, int h, SDL_Rect* mClip, SDL_Renderer* mRender){
	SDL_Rect renderQuad;
	if(w==0&&h==0){
		renderQuad.x = x;
		renderQuad.y = y;
		renderQuad.w = mWidth;
		renderQuad.h = mHeight;
		/*
		if(mClip != NULL){
			renderQuad.h = mClip->h;
			renderQuad.w = mClip->w;
		}
		*/
	}else{
		renderQuad.x = x;
		renderQuad.y = y;
		renderQuad.w = w;
		renderQuad.h = h;
	}
	SDL_RenderCopy(mRender,mTexture,mClip,&renderQuad);
}

void TextureC::RenderZoom(int x, int y, int w, int h, SDL_Rect* mClip, SDL_Renderer* mRender, float _zoom){
	SDL_Rect renderQuad;
	if(w==0&&h==0){
		renderQuad.x = x;
		renderQuad.y = y;
		renderQuad.w = mWidth * _zoom;
		renderQuad.h = mHeight * _zoom;
		/*
		if(mClip != NULL){
			renderQuad.h = mClip->h;
			renderQuad.w = mClip->w;
		}
		*/
	}else{
		renderQuad.x = x;
		renderQuad.y = y;
		renderQuad.w = w * (1 - _zoom);
		renderQuad.h = h * (1 - _zoom);
	}
	SDL_RenderCopy(mRender,mTexture,mClip,&renderQuad);
}

void TextureC::RenderZoom(int x, int y, SDL_Renderer* mRenderer, float zoom){
	SDL_Rect renderQuad;
	renderQuad.x = x;
	renderQuad.y = y;
	renderQuad.w = mWidth * zoom;
	renderQuad.h = mHeight * zoom;
	SDL_RenderCopy(mRenderer, mTexture, NULL, &renderQuad);
}

void TextureC::RenderZoomCentered(int x, int y, SDL_Renderer* mRenderer, float zoom){
	SDL_Rect renderQuad;
	float w = mWidth * zoom, h = mHeight * zoom;
	renderQuad.w = floor(w);
	renderQuad.h = floor(h);
	renderQuad.x = x - floor(w * 0.5);
	renderQuad.y = y - floor(h * 0.5);

	SDL_RenderCopy(mRenderer, mTexture, NULL, &renderQuad);
}

int TextureC::GetHeight(){return mHeight;}
int TextureC::GetWidth(){return mWidth;}
