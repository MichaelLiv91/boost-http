#pragma once
#include <boost/unordered_map.hpp>
#include <memory>
#include <boost/beast.hpp>
#include <functional>
#include <boost/utility/string_view.hpp>
#include <boost/asio.hpp>
#include "websocketpp/config/asio_no_tls.hpp"
#include <websocketpp/server.hpp>
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;


namespace Routing
{
	
	class IRouter;
	class RequestHandler
	{
	public:
		typedef std::string route;
		typedef http::request<http::string_body> request;
		typedef http::response<http::string_body> response;
		typedef boost::unordered_map
			<boost::string_view,
			std::shared_ptr<IRouter> > handlersMap;

		RequestHandler();
		void RegisterHandler(std::shared_ptr<IRouter>_router);
		void HandleRequest(const request& _request, response& _response);
		void HandleRequest(websocketpp::server<websocketpp::config::asio>::connection_ptr con_ptr);

	private:
		handlersMap m_handlers;
	};




}
