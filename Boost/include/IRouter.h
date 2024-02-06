#pragma once
#include <string>
#include <functional>
#include <boost/beast.hpp>

#include "RequsetHandler.h"
#include <boost/asio.hpp>

namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

namespace http = boost::beast::http;

namespace Routing {
	
	class IRouter
	{
	public:
		typedef http::request<http::string_body> request;
		typedef http::response<http::string_body> response;

		IRouter(std::string const& _baseurl) :m_baseurl(_baseurl) {};
		IRouter(IRouter&) = delete;
		virtual ~IRouter() {};
		virtual void HandleRequest(request const& _req, response& _res) = 0;

		std::string const& getBaseURl() const { return m_baseurl; };

	protected:
		std::string m_baseurl;
	};
}