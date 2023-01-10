/*
 * TextBox.cpp
 *
 *  Created on: Jun 6, 2016
 *      Author: Bunjukz
 */

#include <Text/Inputfield/Inputfield.h>

InputC::InputC(){
	mTextTextureFont 	= nullptr;
	bgTexture			= nullptr;
	hoveredTexture		= nullptr;
	focusedTexture		= nullptr;
	mFocused	 		= 0;
	textUpdated 		= 0;
	mFullTextBox 		= 0;
	hovered				= 0;
	characters			= 0;
	fontSize			= 10;
	maxChars			= 8;
}

InputC::~InputC(){
	mCharTexture.Clean();
	mTextTextureFont 	= nullptr;
	bgTexture			= nullptr;
	hoveredTexture		= nullptr;
	focusedTexture		= nullptr;
	mFocused	 		= 0;
	textUpdated 		= 0;
	mFullTextBox 		= 0;
	hovered				= 0;
	characters			= 0;
	maxChars			= 0;
}

int InputC::stopTextInput(){
	mFocused = 0;
	hovered = 0;
	SDL_StopTextInput();
	return 2;
}

int InputC::startTextInput(){
	mFocused = 1;
	SDL_StartTextInput();
	return 1;
}

int InputC::checkEvent(str_Mouse* mi,SDL_Event* e){
	if(mFocused){
		SDL_StartTextInput();
		if(e->type == SDL_KEYDOWN){
			if(e->key.keysym.sym == SDLK_RETURN){ //Enter pressed
				return stopTextInput();
			}else if(e->key.keysym.sym == SDLK_BACKSPACE){ //Backspace pressed and text is longer than zero characters.
				if(shownText.length() >= 1){
					shownText.pop_back();
					textUpdated = 1;
				}
			}
		}else if(e->type == SDL_TEXTINPUT && !mFullTextBox){
			if(!((e->text.text[0] == 'c' || e->text.text[0] == 'C') && (e->text.text[0] == 'v' || e->text.text[0] == 'V') && (SDL_GetModState() & KMOD_CTRL))){
				// No copying / pasting.
				shownText.push_back(e->text.text[0]);
				textUpdated = 1;
			}
		}
	}
	int inside = 1;
	if(mi->location.x<mRect.x){ inside = false; }
	else if(mi->location.x>mRect.x + mRect.w){ inside = 0; }
	else if(mi->location.y<mRect.y){ inside = 0; }
	else if(mi->location.y>mRect.y + mRect.h){ inside = 0; }
	if(!inside && mi->m1 == 1 && mFocused){
		return stopTextInput();
	}
	if(inside && mi->m1 == 1 && !mFocused){
		return startTextInput();
	}
	if(inside && mi->m1 && !mFocused){
		return startTextInput();
	}
	if(mi->moved){
		if(inside && !hovered){
			hovered = 1;
		}else if (!inside && hovered){
			hovered = 0;
		}
	}
	return 0;
}

int InputC::Update(SDL_Renderer* mRender){
	if(shownText.length() >= maxChars){
		mFullTextBox = 1;
	}else{
		mFullTextBox = 0;
	}

	if(textUpdated){
		if(shownText.length() > 0){ // The length of shown text isn't zero
			mCharTexture.LFT(shownText,mRender,mTextTextureColor,mTextTextureFont,fontSize);
		}else{ // There isn't any text to render.
			mCharTexture.LFT(" ", mRender, mTextTextureColor, mTextTextureFont,fontSize);
		}
		textUpdated = 0;
	}

	return 0;
}

void InputC::Render(SDL_Renderer* mRender){
	bgTexture->Render(mRect.x, mRect.y, mRect.w, mRect.h, nullptr, mRender);
	/*
	if(!hovered && !mFocused){
		//bgTexture->Render(mRect.x,mRect.y - bgTexture->GetHeight() /2,nullptr,mRender);
		bgTexture->RenderZoom(mRect.x, mRect.y, mRender, 0);
	}else if(hovered && !mFocused){
		hoveredTexture->RenderZoom(mRect.x, mRect.y, mRender, 0);
	}else{
		focusedTexture->RenderZoom(mRect.x, mRect.y, mRender, 0);
	}
	*/
	mCharTexture.Render(mRect.x, mRect.y, mRender);
}

void InputC::passTextures(TextureC* bg, int w, int h){
	bgTexture = bg;
	mRect.h = h;
	mRect.w = w;
}

void InputC::passTextures(TextureC* bg, TextureC* hovered, TextureC* focused){
	bgTexture = bg;
	hoveredTexture = hovered;
	focusedTexture = focused;
	mRect.h = bgTexture->GetHeight();
	mRect.w = bgTexture->GetWidth();
}

void InputC::setText(std::string text){
	while(shownText.length() != 0){
		shownText.pop_back();
	}
	for(unsigned int i = 0; i < text.length(); i++){
		if(i >= maxChars){
			break;
		}
		shownText.push_back(text[i]);
	}
	textUpdated = 1;
}

void InputC::PassPreferences(SDL_Color mColor ,TTF_Font* mFont ,int newMaxChars, int newFontSize){
	setText("");
	mTextTextureColor 	= mColor;
	mTextTextureFont 	= mFont;
	maxChars			= newMaxChars;
	fontSize			= newFontSize;
}
void InputC::SetPos(int x,int y){
	mRect.x = x;
	mRect.y = y;
}
std::string InputC::GetText(){
	return shownText;
}
