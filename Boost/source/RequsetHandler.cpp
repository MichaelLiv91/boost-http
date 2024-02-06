#include "IRouter.h"
#include "RequsetHandler.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

namespace beast = boost::beast;
namespace http = beast::http;

Routing::RequestHandler::RequestHandler()
	:m_handlers()
{
	
}

void Routing::RequestHandler::RegisterHandler(std::shared_ptr<IRouter>_router)
{
	m_handlers.insert({ _router->getBaseURl(), _router });
}

void Routing::RequestHandler::HandleRequest(const Routing::IRouter::request& _request, Routing::IRouter::response& _response)
{
	string target = _request.target();
	vector<string> result;
	boost::split(result, target, boost::is_any_of("/"));
	auto lookFor = result[1];


	if (lookFor != "socket.io")
	{
		if (result[1] != "api")
		{
			lookFor = "/";
		}
		else
		{
			lookFor = result[2];
		}
	}
	
	auto find = m_handlers.find(lookFor);
	if (find != m_handlers.end())
	{
		_response.version(_request.version());
		_response.set(http::field::access_control_allow_origin, "*");
		_response.set(http::field::access_control_allow_headers, "Content-Type");
		_response.keep_alive(_request.keep_alive());
		m_handlers[find->first]->HandleRequest(_request, _response);
		_response.set(http::field::server, "Wingman Server");
		_response.prepare_payload();
	}
}

void Routing::RequestHandler::HandleRequest(websocketpp::server<websocketpp::config::asio>::connection_ptr con_ptr)
{
	http::request<http::string_body> _request;
	Routing::IRouter::response _response;


	boost::asio::ip::tcp::socket& socket = con_ptr->get_raw_socket();
	// Create a boost::beast::basic_streambuf to store the data
	beast::flat_buffer buffer;

	boost::beast::tcp_stream stream(std::move(socket));

	// Read the response from the stream into the buffer
	boost::beast::error_code ec;
	boost::asio::read(stream, buffer, ec);
	// Error code for potential errors during reading
	beast::error_code ec;

	//// Use a std::istringstream as the input stream
	//std::istringstream iss(httpRequestString);

	//// Create a basic_sync_stream from the std::istringstream
	//using stream_type = boost::asio::basic_streambuf<std::allocator<char>>;
	//stream_type streambuf;
	//streambuf.commit(iss.str().size());
	//streambuf.sputn(iss.str().c_str(), iss.str().size());
	//

	//// Use the provided utility function to read from the stream and parse into the buffer
	//// The HTTP message will be constructed and stored in _request
	//http::read(streambuf, buffer, _request, ec);
	
	/*beastRequest.method();
	beastRequest.target(wsRequest->get_uri()->get_resource());
	beastRequest.version(wsRequest->get_version());*/

	//// Copy headers from WebSocket++ request to Beast request
	//for (const auto& h : wsRequest->get_headers()) {
	//	beastRequest.set(h.first, h.second);
	//}
	//
	HandleRequest(_request, _response);

}
