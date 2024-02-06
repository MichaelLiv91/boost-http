#include "WebSocket.h"

#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = asio::ip::tcp;
namespace http = beast::http;           // from <boost/beast/http.hpp>
 
 
 
void Servers::WebSocket::on_message(websocketpp::connection_hdl hdl, server_type::message_ptr msg) {
    // Handle incoming messages
    std::cout << "Received message: " << msg->get_payload() << std::endl;

    // Echo the message back to the client
    m_endpoint.send(hdl, msg->get_payload(), msg->get_opcode());
}

//
//
//
//std::string base64_encode(const std::string& input) {
//    BIO* bio, * b64;
//    BUF_MEM* bufferPtr;
//
//    b64 = BIO_new(BIO_f_base64());
//    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
//    bio = BIO_new(BIO_s_mem());
//    bio = BIO_push(b64, bio);
//
//    BIO_write(bio, input.c_str(), static_cast<int>(input.length()));
//    BIO_flush(bio);
//    BIO_get_mem_ptr(bio, &bufferPtr);
//
//    std::string result(bufferPtr->data, bufferPtr->length);
//    BIO_free_all(bio);
//
//    return result;
//}
//std::string calculate_websocket_accept_key(const std::string& client_key) {
//    // Concatenate client_key with the WebSocket GUID
//    std::string concatenated = client_key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
//
//    // Calculate SHA-1 hash
//    unsigned char hash[SHA_DIGEST_LENGTH];
//    SHA1(reinterpret_cast<const unsigned char*>(concatenated.c_str()), concatenated.length(), hash);
//
//    // Base64 encode the hash
//    std::string base64_encoded = base64_encode(std::string(reinterpret_cast<char*>(hash), SHA_DIGEST_LENGTH));
//
//    return base64_encoded;
//}
//
//
//void Servers::WebSocket::serverRun()
//{
//    try {
//       
//        // Create and bind an acceptor
//      
//        while (true) {
//            // Wait for a connection
//            tcp::socket socket(m_io_context);
//            m_acceptor->accept(socket);
//
//            // Perform WebSocket handshake
//            beast::flat_buffer buffer;
//            beast::http::request<beast::http::string_body> request;
//            beast::http::read(socket, buffer, request);
//
//    //        if (request.method() == beast::http::verb::get &&
//    //            request[beast::http::field::upgrade] == "websocket" &&
//    //            request[beast::http::field::connection] == "Upgrade") {
//
//
//
//				//// Respond to the WebSocket handshake
//				//beast::http::response<beast::http::empty_body> response{};
//				//for (auto& item : request)
//				//{
//				//	if (item.name_string() == "Sec-WebSocket-Key")
//				//	{
//				//		std::cout << item.name() << ": " << item.value() << std::endl;
//
//
//				//		auto accept_key = calculate_websocket_accept_key(item.value());
//				//		response.set(http::field::upgrade, "websocket");
//				//		response.set(http::field::connection, "Upgrade");
//				//		response.set(http::field::sec_websocket_accept, accept_key);
//				//		response.result(http::status::switching_protocols);
//				//		break;
//				//	}
//				//}
//
//				//// Send the handshake response
//				//beast::http::write(socket, response);
//
//				//// Now the connection is upgraded to a WebSocket connection
//				//std::cout << "WebSocket connection established" << std::endl;
//    //           
//				//// Read and write WebSocket messages
//				//if (m_ws == nullptr)
//				//{
//    //                m_ws = new beast::websocket::stream<tcp::socket>(std::move(socket));
//    //            }
//
//    //            m_ws->async_accept(request, [this](beast::error_code ec) {
//    //                if (!ec) {
//    //                    // WebSocket connection successfully accepted, you can initiate further reads or writes here
//    //                    std::cout << "WebSocket connection accepted" << std::endl;
//
//    //                    // Example: initiate read
//    //                    m_ws->async_read(
//    //                        m_buffer,
//    //                        [this](beast::error_code ec, std::size_t bytes_transferred) {
//    //                            if (!ec) {
//    //                                // Handle the received WebSocket message
//    //                                std::cout << "Received message: " << beast::buffers_to_string(m_buffer.data()) << std::endl;
//    //                            }
//    //                            else {
//    //                                // Handle read error
//    //                                std::cerr << "Error reading WebSocket message: " << ec.message() << std::endl;
//    //                            }
//    //                        }
//    //                    );
//    //                }
//    //                else {
//    //                    // Handle accept error
//    //                    std::cerr << "Error accepting WebSocket connection: " << ec.message() << std::endl;
//    //                }
//    //                });
//    //        }
//    //        else {
//                http::response<http::string_body> response;
//                m_http->HandleRequest(request, response);
//                beast::http::write(socket, response);
//            //}
//        }
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//
//}

