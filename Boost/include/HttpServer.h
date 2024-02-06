#include "IServer.h"
#include "RequsetHandler.h"

namespace Servers
{
	namespace asio = boost::asio;
	namespace http = boost::beast::http;
	using tcp = asio::ip::tcp;

	class HttpServer: public IServer
	{
	public:
		typedef http::request<http::string_body> request;
		typedef http::response<http::string_body> response;
		HttpServer(boost::asio::ip::port_type _port, boost::asio::ip::tcp _ip = tcp::v4());
		~HttpServer() {
			
		};

		void addHandler(std::shared_ptr<Routing::IRouter>_router);
		void HandleRequest(const request& _request, response& _response);
		Routing::RequestHandler& getHandler() { return m_requestHandler; };
	private:
		void serverRun();
		Routing::RequestHandler m_requestHandler;
		
	};

}