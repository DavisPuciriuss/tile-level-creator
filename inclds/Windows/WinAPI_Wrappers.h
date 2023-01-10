/*
 * WinAPI_Wrappers.h
 *
 *  Created on: Feb 20, 2017
 *      Author: Bunjukz
 */

#ifndef INCLDS_WINDOWS_WINAPI_WRAPPERS_H_
#define INCLDS_WINDOWS_WINAPI_WRAPPERS_H_

#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
namespace WinAPI {

	void 			SetLastTime(int* time_Out, SYSTEMTIME _sysTime_In);
	std::string 	GetLatestCreatedFile(std::string dir_In);
	int				CompareLastTime(int* oldTime_In, int* newTime_In);
	void			GetFileCountDirectory(std::string dir_In, int* count_Out);
	void			GetFileNamesDirectory(std::string dir_In, std::string** names_Out);

}


#endif /* INCLDS_WINDOWS_WINAPI_WRAPPERS_H_ */
