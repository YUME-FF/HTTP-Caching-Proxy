#include "ResponseInfo.h"
#include "cache.h"
#include "httpcommand.h"
#include "proxy.h"
#include "mythread.h"

std::mutex Logger::mtx;
//Logger* Logger::instance = nullptr;


void cleanfile() {
    std::ofstream ofs(logFileLocation, std::ios::out | std::ios::trunc);
    ofs.close();
}

void testResponse() {
    std::string buffer =
        "HTTP/1.1 200 OK\r\n"
        "Date: Thu, 22 Feb 2023 12:34:56 GMT\r\n"
        "Cache-Control: max-age=3600, public, must-revalidate, s-maxAge=10\r\n"
        "Expires: Thu, 22 Feb 2023 13:34:56 GMT\r\n"
        "Last-Modified: Wed, 21 Feb 2023 12:34:56 GMT\r\n"
        "ETag: \"123456789\"\r\n"
        "Content-Length: 42\r\n"
        "\r\n"
        "Hello, world! This is a test.";
    TimeMake t;
    ResponseInfo response;
    response.parseResponse(buffer, t.getTime());
    response.printCacheFields();
}

void test(Proxy server, int thread_id) {
    server.run(thread_id);
}

int main() {
    cleanfile();
    // testResponse();
    Proxy server;
    try {
        server.initListenfd("12345");
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << "Server is listening on port: " << server.getPort() << std::endl;

    std::string ip;
    int thread_id = 0;
    while (true) {
        thread_id++;
        try {
            auto result = server.acceptConnection(ip);
            std::string c_ip = result.first;
            int connection_fd = result.second;
            server.startThread(thread_id);
            MyThread th(&Proxy::handleRequest, &server, c_ip, connection_fd, thread_id);
            //std::thread th(&Proxy::handleRequest, &server, c_ip, connection_fd, thread_id);
            // th.join();
            //th.detach();
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
    }
    return EXIT_SUCCESS;
}