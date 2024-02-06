#include "StaticFileController.h"
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "StaticFileService.h"

void Controllers::StaticFilesController::StaticHandler(const Routing::IRouter::request& _request, Routing::IRouter::response& _response)
{
	std::string target = _request.target();
	std::vector<std::string> result;
	boost::split(result, target, boost::is_any_of("/"));

	try {
		std::string content = Services::StaticFileService::getInstance()->GetContent("." + target);
		std::string type = "application/json";
		if (boost::ends_with(target, "text"))
		{
			type = "text/plain";
		}
		_response.body() = content;
		_response.set(http::field::content_type, type.c_str());
		_response.result(http::status::ok);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		_response.result(http::status::not_found);
	}
}

void Controllers::StaticFilesController::HandleRequest(request const& _req, response& _res)
{
	auto target = _req.target();
	StaticHandler(_req, _res);
}