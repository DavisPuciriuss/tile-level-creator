/*
 * TextBox.h
 *
 *  Created on: Jun 6, 2016
 *      Author: Bunjukz
 */

#ifndef INCLDS_TEXT_TEXTBOX_TEXTBOX_H_
#define INCLDS_TEXT_TEXTBOX_TEXTBOX_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <Text/Texture/TextTexture.h>
#include <Graphics/Texture/Texture.h>
#include <Inputs/Inputs.h>

class InputC{
public:
	InputC();
	~InputC();
	int 		Update(SDL_Renderer* mRender);
	void		Render(SDL_Renderer*);
	void		SetPos(int,int);
	void		PassPreferences(SDL_Color,TTF_Font*,int, int);
	void		passTextures(TextureC* bg, TextureC* hovered, TextureC* focused);
	void		passTextures(TextureC*, int, int);
	int			checkEvent(str_Mouse* mi,SDL_Event* e);
	void		setText(std::string text);
	std::string	GetText();
private:
	int stopTextInput();
	int startTextInput();

	SDL_Color	mTextTextureColor;
	TTF_Font*	mTextTextureFont;
	int 		textUpdated,
				mFocused,
				mFullTextBox,
				hovered,
				characters;
	int			fontSize;
	unsigned int		maxChars;

	std::string shownText; // Final result that is shown in the text box.

	TextC		mCharTexture;
	TextureC*	bgTexture;
	TextureC*	hoveredTexture;
	TextureC*	focusedTexture;
	SDL_Rect 	mRect;
};


#endif /* INCLDS_TEXT_TEXTBOX_TEXTBOX_H_ */
