#include <iostream>
#include "StaticFileController.h"
#include "GeoCalculationController.h"
#include "WebSocket.h"

int main() {
    
    auto conf = std::make_shared<Controllers::StaticFilesController>("/");
    auto geo = std::make_shared<Controllers::GeoCalculationController>("geo");
    

	Servers::HttpServer http{ 9900 };
    http.addHandler(conf);
    http.addHandler(geo);
    
    //http.startServer();
    Servers::WebSocket ws(&http);
    ws.startServer();

    while (true);
    return 0;
}