/*
 * RectEditor.cpp
 *
 *  Created on: Feb 3, 2017
 *      Author: Bunjukz
 */

#include <RectEditor/RectEditor.h>

void RectEditorCl::operator =(RectEditorCl& _editor){
	for(int i = 0; i < MAX_TEXT_FIELDS; i++){
		this->textsfields[i].setText(_editor.getText(i));
	}
	this->iBox 		= _editor.getBox();
	this->iState 	= 0;
}

void RectEditorCl::Init(int x, int y, int w, int h, int _tileSize, TextureC* _bg, TextureC* _btnSheet, TextureC* _bgText, TTF_Font* _font, SDL_Color _color, SDL_Renderer* _render){
	iState		= 0;
	iMain		= 0;
	background	= _bg;

	iLocation	= {x,y,w,h};
	defaultSize = {_tileSize, _tileSize};
	iBox		= {0,0,_tileSize,_tileSize};

	closeWindowBtn.Init(SDL_Rect {iLocation.x + iLocation.w - 20, iLocation.y, 20, 20}, "X", _render, _btnSheet, _font, _color);
	int textSize = 12;
	texts[0].LFT("x: ", _render, _color, _font, textSize);
	texts[1].LFT("y: ", _render, _color, _font, textSize);
	texts[2].LFT("w: ", _render, _color, _font, textSize);
	texts[3].LFT("h: ", _render, _color, _font, textSize);
	texts[4].LFT("a_1: ", _render, _color, _font, textSize);
	texts[5].LFT("a_2: ", _render, _color, _font, textSize);
	for(int i = 0; i < MAX_TEXT_FIELDS; i++){
		textsfields[i].PassPreferences(_color, _font, 4, textSize);
		textsfields[i].passTextures(_bgText, 80,25);
		textsfields[i].SetPos(iLocation.x + 10 + 75, iLocation.y + i * 25 + 10);
	}
}

void RectEditorCl::Init(SDL_Rect _location, int _w, int _h, TextureC* _bg, TextureC* _btnSheet, TextureC* _bgText, TTF_Font* _font, SDL_Color _color, SDL_Renderer* _render){
	iState		= 0;
	iMain		= 0;
	background	= _bg;

	iLocation	= _location;
	defaultSize = {_w, _h};
	iBox		= {0,0,_w,_h};

	closeWindowBtn.Init(SDL_Rect {iLocation.x + iLocation.w - 20, iLocation.y, 20, 20}, "X", _render, _btnSheet, _font, _color);
	int textSize = 12;
	texts[0].LFT("x: ", _render, _color, _font, textSize);
	texts[1].LFT("y: ", _render, _color, _font, textSize);
	texts[2].LFT("w: ", _render, _color, _font, textSize);
	texts[3].LFT("h: ", _render, _color, _font, textSize);
	texts[4].LFT("a_1: ", _render, _color, _font, textSize);
	texts[5].LFT("a_2: ", _render, _color, _font, textSize);
	for(int i = 0; i < MAX_TEXT_FIELDS; i++){
		textsfields[i].PassPreferences(_color, _font, 4, textSize);
		textsfields[i].passTextures(_bgText, 80,25);
		textsfields[i].SetPos(iLocation.x + 10 + 75, iLocation.y + i * 25 + 10);
	}
}

void RectEditorCl::Clean(){
	iState 		= 0;
	background	= nullptr;
	iLocation	= {};
	iBox		= {};
	closeWindowBtn.Clean();
	for(int i = 0; i < MAX_TEXT_FIELDS; i++){
		texts[i].Clean();
	}
}

void RectEditorCl::Update(str_Mouse* _mi, SDL_Event* _e, SDL_Renderer* _render){
	switch(iState){
	case 0:
		iState = 1;
		break;
	case 1:
		/*
		 * Update the closeBtn.
		 * Update the text boxes.
		 */
		if(closeWindowBtn.Update(_mi))
			iState = 2;
		if(iMain){
			for(int i = 0; i < MAX_TEXT_FIELDS; i++){
				// Update all of the textfields
				textsfields[i].Update(_render);
			}
		}else{
			for(int i = 0; i < RECT_TEXT_FIELDS; i++){
				// Update the first four textfields. (last two are assetID_1 and assetID_2.
				textsfields[i].Update(_render);
			}
		}
		break;
	case 2:
		/*
		 * This window is being closed.
		 */
		break;
	}
}

void RectEditorCl::UpdateEvents(SDL_Event* _e, str_Mouse* _mi){
	if(iMain){
		for(int i = 0; i < MAX_TEXT_FIELDS; i++){
			textsfields[i].checkEvent(_mi, _e);
		}
	}else{
		for(int i = 0; i < RECT_TEXT_FIELDS; i++){
			textsfields[i].checkEvent(_mi, _e);
		}
	}
}

void RectEditorCl::Render(SDL_Renderer* _render){
	if(iState == 1){
		background->Render(iLocation.x, iLocation.y, iLocation.w , iLocation.h, nullptr, _render);
		if(iMain){
			for(int i = 0; i < MAX_TEXT_FIELDS; i++){
				// Render all of the textfields.
				texts[i].Render(iLocation.x + 10,iLocation.y + i * 25 + 10,_render);
				textsfields[i].Render(_render);
			}
		}else{
			for(int i = 0; i < RECT_TEXT_FIELDS; i++){
				// Render the first four textfields.
				texts[i].Render(iLocation.x + 10,iLocation.y + i * 25 + 10,_render);
				textsfields[i].Render(_render);
			}
		}
		closeWindowBtn.Render(_render,0);

	}
}

void RectEditorCl::Reset(){
	iState = 0;
}

void RectEditorCl::setMain(){
	if(!iMain)
		iMain = 1;
}

void RectEditorCl::setBox(SDL_Rect _boxIn){
	iBox = _boxIn;
	textsfields[0].setText(std::to_string(iBox.x));
	textsfields[1].setText(std::to_string(iBox.y));
	textsfields[2].setText(std::to_string(iBox.w));
	textsfields[3].setText(std::to_string(iBox.h));
}

void RectEditorCl::setAssets(int* _assetsIn){
	iAssets[0] = _assetsIn[0];
	iAssets[1] = _assetsIn[1];
	textsfields[4].setText(std::to_string(iAssets[0]));
	textsfields[5].setText(std::to_string(iAssets[1]));
}

void RectEditorCl::setPos(SDL_Rect _boxIn){
	iLocation = _boxIn;
}

SDL_Rect RectEditorCl::getPos(){
	return iLocation;
}

int RectEditorCl::getState(){
	return iState;
}

SDL_Rect RectEditorCl::getBox(){
	if(textsfields[0].GetText().length() != 0)
		iBox.x = std::stoi(textsfields[0].GetText());
	else
		iBox.x = 0;
	if(textsfields[1].GetText().length() != 0)
		iBox.y = std::stoi(textsfields[1].GetText());
	else
		iBox.y = 0;
	if(textsfields[2].GetText().length() != 0)
		iBox.w = std::stoi(textsfields[2].GetText());
	else
		iBox.w = defaultSize.x;
	if(textsfields[3].GetText().length() != 0)
		iBox.h = std::stoi(textsfields[3].GetText());
	else
		iBox.h = defaultSize.y;
	return iBox;
}
std::string RectEditorCl::getText(int index){
	return textsfields[index].GetText();
}
int* RectEditorCl::getAssets(){
	// Check if text box 1 has anything written in it.
	// Check if text box 2 has anything written in it.
	// Create an array, convert strings to integer and return the array.
	if(textsfields[4].GetText().length() != 0)
		iAssets[0] = std::stoi(textsfields[4].GetText());
	else
		iAssets[0] = -1;
	if(textsfields[5].GetText().length() != 0)
		iAssets[1] = std::stoi(textsfields[5].GetText());
	else
		iAssets[1] = -1;
	return iAssets;
}
