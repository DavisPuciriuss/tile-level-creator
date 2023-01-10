/*
 * RectEditor.h
 *
 *  Created on: Feb 3, 2017
 *      Author: Bunjukz
 */

#ifndef INCLDS_RECTEDITOR_RECTEDITOR_H_
#define INCLDS_RECTEDITOR_RECTEDITOR_H_

#include <stdio.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_TTF.h>
#include <Graphics/Texture/Texture.h>
#include <Text/Texture/TextTexture.h>
#include <Button/DefaultButton.h>
#include <Inputs/Inputs.h>
#include <Text/Inputfield/Inputfield.h>

#define RECT_TEXT_FIELDS		4
#define MAX_TEXT_FIELDS 		6

class RectEditorCl {
public:
	void 	Init(int, int, int, int, int, TextureC*, TextureC*, TextureC*, TTF_Font*, SDL_Color, SDL_Renderer*);
	void 	Init(SDL_Rect, int, int, TextureC*, TextureC*, TextureC*, TTF_Font*, SDL_Color, SDL_Renderer*);
	void 	Clean();
	void	Reset();
	void 	Update(str_Mouse*, SDL_Event*, SDL_Renderer*);
	void	UpdateEvents(SDL_Event*, str_Mouse*);
	void 	Render(SDL_Renderer*);
	void 	operator=(RectEditorCl& _editor);
	std::string getText(int);
	int			getState();
	SDL_Rect	getBox();
	SDL_Rect	getPos();
	int*		getAssets();
	void		setMain();
	void		setBox(SDL_Rect);
	void		setPos(SDL_Rect);
	void		setAssets(int*);
private:
	TextureC* 			background;
	DefaultButtonClass 	closeWindowBtn;
	TextC				texts[MAX_TEXT_FIELDS];
	InputC				textsfields[MAX_TEXT_FIELDS];
	int 				iState; // 0 = NOT_STARTED, 1 = RUNNING, 2 = QUIT.
	int					iMain;
	int 				iAssets[2];
	SDL_Point			defaultSize;
	SDL_Rect			iLocation;
	SDL_Rect			iBox;
};



#endif /* INCLDS_RECTEDITOR_RECTEDITOR_H_ */
