#pragma once
#include "GeoCalculationController.h"
#include <boost/gil.hpp>
#include <boost/gil/extension/io/tiff.hpp>
#include <fstream> 
#include <filesystem>
#include "GeoCalculationService.h"

#include <math.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
constexpr float M_TO_FEET = 3.281F;

Controllers::GeoCalculationController::GeoCalculationController(std::string const& _baseurl) :IRouter{ _baseurl }
{
	Services::GeoCalculationService::getInstance();
};

void Controllers::GeoCalculationController::HandleRequest(request const& _req, response& _res)
{
	std::string requestBody = _req.body();
	if (requestBody.length() > 0)
	{
		json requestJson = json::parse(requestBody);
		float lat = requestJson["latitude"];
		float lon = requestJson["longitude"];
		json responseJson;
		responseJson["elevation"] = Services::GeoCalculationService::getInstance()->GetElevation({ lat,lon,0 });
		_res.body() = responseJson.dump();
		_res.set(http::field::content_type, "text/plain");
		_res.result(http::status::ok);
	}
}
