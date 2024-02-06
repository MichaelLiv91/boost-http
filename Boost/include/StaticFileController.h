#pragma once
#include "IRouter.h"

namespace Controllers
{

	class StaticFilesController : public Routing::IRouter
	{
	public:
		StaticFilesController() = default;
		StaticFilesController(std::string const& _baseurl):IRouter(_baseurl) {};
		~StaticFilesController() = default;

		virtual void HandleRequest(request const& _req, response& _res);

	private:
		void StaticHandler(const Routing::IRouter::request& _request, Routing::IRouter::response& _response);
		
	};

}