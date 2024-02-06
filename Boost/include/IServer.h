#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = asio::ip::tcp;


namespace Servers
{
	class IServer
	{
	public:
		IServer(boost::asio::ip::port_type _port, boost::asio::ip::tcp _ip = tcp::v4()) :m_io_context{}, m_acceptor{}, m_serverThread{}, m_ip{ _ip }, m_port{ _port }{
			m_acceptor = new tcp::acceptor(m_io_context, { _ip, _port });
		};

		IServer(IServer const& _other) :m_io_context{}, m_acceptor{ _other.m_acceptor}, m_serverThread{}, m_ip{ _other.m_ip }, m_port{ _other.m_port }{
			
		};

		~IServer() {
			
			m_acceptor->close();
			delete m_acceptor;
			delete m_serverThread;
		};

		void startServer()
		{
			m_serverThread = new std::thread([this] {
				this->serverRun(); 
				});
			
			m_serverThread->detach();
		}

	protected:
		virtual void serverRun() = 0;
		asio::io_context m_io_context;
		tcp::acceptor* m_acceptor;
		std::thread* m_serverThread;
		boost::asio::ip::tcp m_ip;
		boost::asio::ip::port_type m_port;
	};
}