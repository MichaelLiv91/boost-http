#pragma once
#include <HttpServer.h>
#include <iostream>
#include <string>
#include "WebSocketController.h"

#include "websocketpp/config/asio_no_tls.hpp"
#include <websocketpp/server.hpp>

namespace Servers
{

	//class WebSocket : public IServer
	//{
	//public:
	//	WebSocket(HttpServer* _http) :IServer{ *_http }, m_http(_http), m_ws{ nullptr }{
	//		auto socket = std::make_shared<Controllers::WebSockectController>("socket.io");
	//		m_http->addHandler(socket);

	//		m_endpoint.set_message_handler(bind(&WebSocket::on_message, this, std::placeholders::_1, std::placeholders::_2));
	//		m_endpoint.set_access_channels(websocketpp::log::alevel::all^ websocketpp::log::alevel::frame_payload);

	//		// Initialize Asio
	//		m_endpoint.init_asio();
	//		m_endpoint.listen(m_port);
	//		m_endpoint.set_http_handler([](websocketpp::connection_hdl _hndl) {
	//			std::cout << "HTTP";
	//			});
	//		m_endpoint.start_accept();
	//	};
	//	~WebSocket() {};
	//	void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);
	//private:
	//	void serverRun();
	//	HttpServer* m_http;
	//	beast::websocket::stream<tcp::socket>* m_ws;
	//	beast::flat_buffer m_buffer;
	//	server m_endpoint;
	//};
	using namespace websocketpp;
	class WebSocket
	{
		
		typedef server<config::asio> server_type;
	public:
		WebSocket(HttpServer* _http) : m_http(_http), m_ws{ nullptr }{
			auto socket = std::make_shared<Controllers::WebSockectController>("socket.io");
			m_http->addHandler(socket);

			m_endpoint.set_message_handler(bind(&WebSocket::on_message, this, std::placeholders::_1, std::placeholders::_2));
			m_endpoint.set_access_channels(websocketpp::log::alevel::all^ websocketpp::log::alevel::frame_payload);
			
			
		};
		~WebSocket() {};
		void on_message(websocketpp::connection_hdl hdl, server_type::message_ptr msg);

		void startServer()
		{
		
			// Initialize Asio
			m_endpoint.init_asio();
			m_endpoint.listen(9900);
			m_endpoint.set_http_handler([this](websocketpp::connection_hdl _hndl) 
				{
					server_type::connection_ptr con = m_endpoint.get_con_from_hdl(_hndl);
					m_http->getHandler().HandleRequest(con);
					//m_endpoint.send_http_response()
					

				});

			m_endpoint.start_accept();
			m_endpoint.run();
			
		};
	private:
		
		HttpServer* m_http;
		beast::websocket::stream<tcp::socket>* m_ws;
		beast::flat_buffer m_buffer;
		server_type m_endpoint;
	};

}