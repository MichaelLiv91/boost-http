#pragma once
#include "IRouter.h"

namespace Controllers
{

	class WebSockectController : public Routing::IRouter
	{
		enum class Handshake {
			GET_SID = 0,
			TEST_SID,
			GET_WB_SID,
			SWITCH_PROTOCOL,
			FINISH
		};

	public:
		WebSockectController() = default;
		WebSockectController(std::string const& _baseurl) :IRouter(_baseurl), m_state{}, m_sid{}, m_clientToken{}, m_sock{nullptr}{};
		~WebSockectController() = default;

		virtual void HandleRequest(request const& _req, response& _res);

	private:
		Handshake m_state;
		std::string m_sid;
		std::string m_clientToken;
		tcp::socket* m_sock;
	};

}