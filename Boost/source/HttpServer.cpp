#include "HttpServer.h"
#include <iostream>

Servers::HttpServer::HttpServer(boost::asio::ip::port_type _port, boost::asio::ip::tcp _ip) : IServer{ _port, _ip }
{

}

void Servers::HttpServer::serverRun()
{
    try {
        // Create and bind an acceptor
        while (true) {
            // Create a socket and accept a connection
            std::vector<tcp::socket> vec;
            auto socket = tcp::socket(m_io_context);
            m_acceptor->accept(socket);

            // Read and parse the HTTP request
            boost::beast::flat_buffer buffer;
            http::request<http::string_body> request;
            http::read(socket, buffer, request);

            // Process the request and generate a response
            http::response<http::string_body> response;

            m_requestHandler.HandleRequest(request, response);
            // Send the response
            http::write(socket, response);

			
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Servers::HttpServer::addHandler(std::shared_ptr<Routing::IRouter>_router)
{
	m_requestHandler.RegisterHandler(_router);
}

void Servers::HttpServer::HandleRequest(const request& _request, response& _response)
{
    m_requestHandler.HandleRequest(_request, _response);
}

