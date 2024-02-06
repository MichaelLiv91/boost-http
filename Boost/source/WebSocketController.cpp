#pragma once
#include "WebSocketController.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>

using json = nlohmann::json;

void Controllers::WebSockectController::HandleRequest(request const& _req, response& _res)
{
	char websocket[][10] = { "websocket" };
	json responseJson;
	json requestJson;
	std::vector<std::string> result;
	bool isLegal = true;
	std::string target = _req.target();
	std::size_t questionMarkPos = _req.target().find('?');
	std::string params = _req.target().substr(questionMarkPos, target.length());
	auto gen = boost::uuids::random_generator()();
	boost::split(result, params, boost::is_any_of("&"));
	for (auto& item : result)
	{
		std::vector<std::string> temp_result;
		boost::split(temp_result, item, boost::is_any_of("="));
		requestJson[temp_result[0]] = temp_result[1];
	}

	switch (m_state)
	{
	case Handshake::GET_SID:
		
		
		m_sid = boost::uuids::to_string(gen);
		responseJson["sid"] = m_sid.c_str();
		responseJson["upgrades"] = websocket;
		responseJson["pingInterval"] = 25000;
		responseJson["pingTimeout"] = 25000;
		responseJson["maxPayload"] = 1000000;

		_res.body() = '0' + responseJson.dump();
		
		_res.set(http::field::content_type, "text/plain; charset=UTF-8");
		_res.result(http::status::ok);
		m_state = Handshake::TEST_SID;
		break;
	case Handshake::TEST_SID:
		
		try
		{
			if (requestJson["sid"] == m_sid)
			{
				_res.body() = "OK";
				_res.set(http::field::content_type, "text/html");
				_res.result(http::status::ok);
				m_state = Handshake::GET_WB_SID;
			}
			else
			{
				m_state = Handshake::GET_SID;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		break;
	case Handshake::GET_WB_SID:
		if (requestJson["sid"] == m_sid)
		{
			responseJson["sid"] = m_sid.c_str();
			_res.body() = "40" + responseJson.dump();
			_res.set(http::field::content_type, "text/plain; charset=UTF-8");
			_res.result(http::status::ok);
			m_state = Handshake::FINISH;
		}
		break;
	case Handshake::SWITCH_PROTOCOL:
		/*for (auto& item : _req)
		{
			if (item.name_string() == "Sec-WebSocket-Key")
			{
				std::cout << item.name() << ": " << item.value() << std::endl;
				if (requestJson["sid"] == m_sid)
				{
					auto accept_key = calculate_websocket_accept_key(item.value());
					_res.set(http::field::upgrade, "websocket");
					_res.set(http::field::connection, "Upgrade");
					_res.set(http::field::sec_websocket_accept, accept_key);
					_res.result(http::status::switching_protocols);
					m_state = Handshake::FINISH;
				}
				break;
			}
			else
			{
				m_state = Handshake::GET_SID;
			}
		}*/

		
		break;

	case Handshake::FINISH:
		static bool finish = false;
		if (!finish)
		{
			_res.body() = "6";
			_res.set(http::field::connection, "keep-alive");
			_res.set(http::field::keep_alive, "timeout=5");
			_res.set(http::field::content_type, "text/plain; charset=UTF-8");
			_res.result(http::status::ok);
			m_state = Handshake::FINISH;
			finish = true;
		}
		break;
	}
}





