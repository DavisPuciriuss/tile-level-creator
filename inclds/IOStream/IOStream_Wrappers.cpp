/*
 * IOStream_Wrappers.cpp
 *
 *  Created on: Feb 20, 2017
 *      Author: Bunjukz
 */

#include <IOStream/IOStream_Wrappers.h>
#include <Tiling/LevelInfo.h>
#include <Graphics/MainGraphics.h>
#include <Brush/Brush.h>
#include <RectEditor/RectEditor.h>

namespace IOStream{
	void Export(std::string directory_In, void* levelInfo_In){
		printf("Exporting... \n");
		std::ofstream mTempExportPath;
		Level_Info* levelInfo = (Level_Info*) levelInfo_In;
		printf("Created the directory, Exporting! \n");
		mTempExportPath.open(directory_In.c_str());
		mTempExportPath << levelInfo->width << " " << levelInfo->height << " " << levelInfo->objectsPlaced << "\n";
		for(int x = 0; x < levelInfo->width; x++){
			for(int y = 0; y < levelInfo->height; y++){
				int tile;
				levelInfo->GetTile(&tile, x, y);
				mTempExportPath << tile << " ";
			}
			mTempExportPath << "\n";
		}
		for(int i=0;i<levelInfo->objectsPlaced;i++){
			ObjectC _obj;
			levelInfo->GetObjects(&_obj, i);
			mTempExportPath << floor(_obj.getCollisionBox().x * levelInfo->gridSize) << " " << floor(_obj.getCollisionBox().y * levelInfo->gridSize) << " " << _obj.getType() << "\n";
		}
		mTempExportPath.close();
		printf("Exported! \n");
	}
	int Import(std::string directory_In, void* levelInfo_In, void* graphics_In){
		GraphicsCl* _g = (GraphicsCl*) graphics_In;
		Level_Info* levelInfo = (Level_Info*) levelInfo_In;
		if(FileExists(directory_In)){
			printf("File Exists!\n");
			std::ifstream importFile;
			importFile.open(directory_In.c_str(), std::ifstream::in);
			if(importFile.good()){
				printf("Importing... \n");
				levelInfo->ResetLevel();
				int w;
				int h;
				int o;
				importFile >> w;
				importFile >> h;
				importFile >> o;
				levelInfo->SetLevelSize(w,h,o);

				for(int x = 0; x < levelInfo->width; x++){
					for(int y = 0; y < levelInfo->height; y++){
						int tileType 	= -1;
						importFile >> tileType;
						if(importFile.fail()){
							printf("Error loading map: Unexpected end of file! \n");
							return 1;
						}
						levelInfo->SetTile(tileType, x, y);
					}
				}

				if(o >= 1){
					for(int i=0;i<o;i++){
						int tempX 		= -1;
						int tempY 		= -1;
						int tempType 	= -1;
						importFile >> tempX;
						importFile >> tempY;
						importFile >> tempType;
						if(importFile.fail()){
							printf("Error loading map: Unexpected end of file! \n");
							return 1;
						}
						if(tempX != -1 && tempY != -1 && tempType != -1){
							SDL_Rect box = {(int)floor(tempX / levelInfo->gridSize),(int)floor(tempY / levelInfo->gridSize), _g->getTexture(2, tempType)->GetWidth(), _g->getTexture(2, tempType)->GetHeight()};
							levelInfo->InitObject(i, box, tempType);
						}
					}
				}
				importFile.close();
				printf("Imported! \n");
			}else{
				printf("Failed to open the necessary file! \n");
				return 1;
			}
		}else{
			printf("File doesn't exist! \n");
			return 1;
		}
		return 0;
	}

	void ExportCollisionInfo(SDL_Rect* box_In, int boxSize_In, int* assetID_In, void* selection_In){
		std::ofstream exportPath;
		std::stringstream dir;
		Selection* selection = (Selection*) selection_In;
		if(selection->what == Selection::TILE)
			dir << ".//Data//Tiles//" << selection->type << ".info";
		else
			dir << ".//Data//Objects//Object_" << selection->type << ".info";
		exportPath.open(dir.str().c_str());
		exportPath << "{" << assetID_In[0] << ", ";
		exportPath << assetID_In[1] << ", ";
		exportPath << 0 << ", ";
		exportPath << boxSize_In << ", ";
		exportPath << "{";
		for(int i = 0; i < boxSize_In; i++){
			exportPath << " {";
			exportPath << box_In[i].x << ", ";
			exportPath << box_In[i].y << ", ";
			exportPath << box_In[i].w << ", ";
			exportPath << box_In[i].h << "}";
			if(i + 1 != boxSize_In)
				exportPath << ", ";
		}
		exportPath << " } }";
		exportPath.close();
		printf("Exporting finished !\n");

	}

	void ImportCollisionInfo(int tileSize, void* selection_In, void* graphics_In, void* editors_In, int* editorSize_Out){
		std::ifstream importPath;
		std::stringstream dir;
		GraphicsCl* _g = (GraphicsCl*) graphics_In;
		Selection* selection = (Selection*) selection_In;
		RectEditorCl* mEditors = (RectEditorCl*) editors_In;
		int looptime= 0;
		int box 	= 0;
		int _w		= 0;
		int _h		= 0;
		int	assets[2];
		int buffer[255]; // Place all of the integers into this array.
		SDL_Rect boxes[5];

		if(selection->what == Selection::TILE){
			dir << ".//Data//Tiles//" << selection->type << ".info";
			_w = tileSize;
			_h = tileSize;
		}else{
			dir << ".//Data//Objects//Object_" << selection->type << ".info";
			_w = _g->getTexture(2,selection->type)->GetWidth();
			_h = _g->getTexture(2,selection->type)->GetHeight();
		}
		importPath.open(dir.str().c_str());
		if(importPath.good()){
			std::string _importSTR;
			std::string _parseSTR;
			std::getline(importPath, _importSTR);
			for(unsigned int i = 0; i < _importSTR.length(); i++){
				if(IOStream::SkippableCharacter(_importSTR[i])){
					if(_parseSTR.length() > 0){
						buffer[looptime] = std::stoi(_parseSTR);
						looptime++;
						_parseSTR.clear();
					}
					continue;
				}
				_parseSTR.push_back(_importSTR[i]);
			}
			int z = 0;
			assets[0] 	= buffer[z++];
			assets[1] 	= buffer[z++];
			z++;
			box 		= buffer[z++];
			for(int i = 0; i < 5; i++){
				boxes[i].x = buffer[z++];
				boxes[i].y = buffer[z++];
				boxes[i].w = buffer[z++];
				boxes[i].h = buffer[z++];
			}
			(*editorSize_Out) = box;
			for(int i = 0; i < (*editorSize_Out); i++){
				mEditors[i].Init(	SDL_Rect {1280 - 200, 175 * i, 200 ,160}, _w, _h,
									_g->getTexture(4,0),
									_g->getTexture(0,0),
									_g->getTexture(5,0),
									_g->getFont("medium"),
									_g->getColor("Black"),
									_g->frame.render);
				mEditors[i].setBox(boxes[i]);
				mEditors[i].setAssets(assets);
			}
			if((*editorSize_Out) > 0){
				mEditors[0].setMain();
			}
		}else{
			std::cout << "There is no collision data to import !" << std::endl;
		}
	}

	void LoadCollisionInfo(int type_In, void* selection_In, Collision_Data** collision_data_Out){
		std::ifstream importPath;
		std::stringstream dir;
		Selection::SELECTION_TYPES* type = (Selection::SELECTION_TYPES*) selection_In;
		//Selection* selection = (Selection*) selection_In;

		int looptime = 0;
		int buffer[255]; // Place all of the integers into this array.

		if((*type) == Selection::TILE)
			dir << ".//Data//Tiles//" << type_In << ".info";
		else
			dir << ".//Data//Objects//Object_" << type_In << ".info";
		importPath.open(dir.str().c_str());
		if(importPath.good()){
			std::string _importSTR;
			std::string _parseSTR;
			std::getline(importPath, _importSTR);
			for(unsigned int i = 0; i < _importSTR.length(); i++){
				if(IOStream::SkippableCharacter(_importSTR[i])){
					if(_parseSTR.length() > 0){
						buffer[looptime++] = std::stoi(_parseSTR);
						_parseSTR.clear();
					}
					continue;
				}
				_parseSTR.push_back(_importSTR[i]);
			}
			int z = 4;
			(*collision_data_Out)[type_In].boxes = buffer[3];
			for(int i = 0; i < 5; i++){
				(*collision_data_Out)[type_In].box[i].x = buffer[z++];
				(*collision_data_Out)[type_In].box[i].y = buffer[z++];
				(*collision_data_Out)[type_In].box[i].w = buffer[z++];
				(*collision_data_Out)[type_In].box[i].h = buffer[z++];
			}
		}else{
			std::cout << "There is no collision data to import !" << std::endl;
		}
	}

	int getIntFromChar (char* _charIn){
		// Windows returns the file name as a 260 char array.
		std::string _buffer;
		char _c[11] = {'-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
		for(int i = 0; i < 260; i++){
			int lastTickAdded = 0;
			for(int z = 0; z < 11; z++)
				if(_charIn[i] == _c[z]){
					_buffer.push_back(_charIn[i]);
					lastTickAdded = 1;
				}
			if(_buffer.length() > 0 && !lastTickAdded)
				return std::stoi(_buffer);
		}
		return 0;
	}

	void getIntFromString(std::string** string_In, int stringSize, int** int_Out){
		std::string _buffer;
		char _c[] = {'-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
		for(int i = 0; i < stringSize; i++)
			for(unsigned int z = 0; z < (*string_In)[i].length(); z++){
				int addedLastTick = 0;
				for(int x = 0; x < 11; x++)
					if((*string_In)[i][z] == _c[x]){
						_buffer.push_back((*string_In)[i][z]);
						addedLastTick = 1;
						continue;
					}
				if(!addedLastTick && _buffer.length() > 0){
						(*int_Out)[i] = std::stoi(_buffer);
						_buffer.clear();
				}
			}
	}

	int SkippableCharacter(char _c){
		if(_c == '{'){
			return 1;
		}else if (_c == ' '){
			return 1;
		}else if (_c == '}'){
			return 1;
		}else if (_c == ','){
			return 1;
		}
		return 0;
	}

	int FileExists(std::string dir){
		std::ifstream tempFile(dir.c_str());
		return tempFile.good();
	}
}
