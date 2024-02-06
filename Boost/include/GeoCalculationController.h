#pragma once
#include "IRouter.h"
#include "BasicTypes.h"
#include <boost/thread/thread.hpp>

#include "gdal_priv.h"
#include "cpl_conv.h"

namespace Controllers
{
	struct tiffData {
		int pixel_width;
		int pixel_height;
		double bbox_width;
		double bbox_height;
		double bbox_0;
		double bbox_1;
	};

	class GeoCalculationController : public Routing::IRouter
	{
	public:
		GeoCalculationController() = default;
		GeoCalculationController(std::string const& _baseurl);
		~GeoCalculationController() = default;

		virtual void HandleRequest(request const& _req, response& _res);
		
	};

}