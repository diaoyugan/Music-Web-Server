#pragma once

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <Windows.h>
#include <thread>

extern std::map<std::wstring, std::string> musicMap;
void startHandleReloadCommand(std::string musicMapFile);

using namespace std;