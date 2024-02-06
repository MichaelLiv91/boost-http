#include "StaticFileService.h"
#include <fstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <mutex>



std::once_flag Services::StaticFileService::s_flag{};
Services::StaticFileService* Services::StaticFileService::s_Instance = nullptr;

Services::StaticFileService* Services::StaticFileService::getInstance()
{
	if (s_Instance == nullptr)
	{
		std::call_once(s_flag, [] {s_Instance = new StaticFileService{}; });
	}

	return s_Instance;
}

std::string Services::StaticFileService::GetContent(std::string const& _filepath)
{
	std::ifstream file(_filepath);
	if (!file.is_open()) {
		throw std::runtime_error("File not found: " + _filepath);
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return content;

}