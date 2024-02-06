#pragma once
#include <string>
#include <stdexcept>
#include <mutex>

namespace Services
{

	class StaticFileService
	{
	public:
		~StaticFileService() {
			delete s_Instance;
		}
		static StaticFileService* getInstance();
		std::string GetContent(std::string const& _filepath);

	private:
		StaticFileService() = default;
		static std::once_flag s_flag;
		static StaticFileService* s_Instance;
	};

}