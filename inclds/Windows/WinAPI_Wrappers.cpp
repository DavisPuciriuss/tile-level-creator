/*
 * WinAPI_Wrappers.cpp
 *
 *  Created on: Feb 20, 2017
 *      Author: Bunjukz
 */

#include <Windows/WinAPI_Wrappers.h>
namespace WinAPI {
	void 		SetLastTime(int* time_Out, SYSTEMTIME _sysTime_In){
		int z = 0;
		time_Out[z++] = _sysTime_In.wYear;
		time_Out[z++] = _sysTime_In.wMonth;
		time_Out[z++] = _sysTime_In.wDay;
		time_Out[z++] = _sysTime_In.wHour;
		time_Out[z++] = _sysTime_In.wMinute;
		time_Out[z++] = _sysTime_In.wSecond;
	}
	std::string GetLatestCreatedFile(std::string dir_In){
		HANDLE 			_Handle;
		WIN32_FIND_DATA _fData;
		SYSTEMTIME		_sysTime;
		std::string 	_chosenFile;
		int				_chosenTime[6];
		int				_newTime[6];
		_Handle = FindFirstFile(dir_In.c_str(), &_fData);
		FileTimeToSystemTime(&_fData.ftCreationTime, &_sysTime);
		_chosenFile = _fData.cFileName;
		SetLastTime(_chosenTime, _sysTime);
		while (FindNextFile(_Handle, &_fData) != 0){
			FileTimeToSystemTime(&_fData.ftCreationTime, &_sysTime);
			SetLastTime(_newTime, _sysTime);
			if(CompareLastTime(_chosenTime, _newTime)){
				_chosenFile = _fData.cFileName;
				SetLastTime(_chosenTime, _sysTime);
			}
		}
		FindClose(_Handle);
		return _chosenFile;
	}
	int			CompareLastTime(int* oldTime_In, int* newTime_In){
		for(int i = 0; i < 6; i++)
			if(oldTime_In[i] < newTime_In[i])
				return 1;
		return 0;
	}
	void		GetFileCountDirectory(std::string dir_In, int* count_Out){
		WIN32_FIND_DATA _fData;
		HANDLE 			_Handle;
		int				_directorySize = 0;
		_Handle = FindFirstFile(dir_In.c_str(), &_fData);
		if(_Handle == INVALID_HANDLE_VALUE){
			std::cout << "Couldn't find any file in directory" << dir_In << std::endl;
			return;
		}
		_directorySize++;
		while(FindNextFile(_Handle, &_fData) != 0){
			_directorySize++;
		}
		FindClose(_Handle);
		*count_Out = _directorySize;
	}
	void		GetFileNamesDirectory(std::string dir_In, std::string** names_Out){
		WIN32_FIND_DATA _fData;
		HANDLE 			_Handle;
		int				_directorySize = 0;
		_Handle = FindFirstFile(dir_In.c_str(), &_fData);
		if(_Handle == INVALID_HANDLE_VALUE){
			std::cout << "Couldn't find any file in directory" << dir_In << std::endl;
			return;
		}
		(*names_Out)[_directorySize++] = _fData.cFileName;
		while(FindNextFile(_Handle, &_fData) != 0){
			(*names_Out)[_directorySize++] = _fData.cFileName;
		}
		FindClose(_Handle);
	}
}
