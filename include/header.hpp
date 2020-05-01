// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <string>

#define PORT_NUM 8001
#define MS 100

const char STR[] = "127.0.0.1";
const char CONNECTED[] = "client connected ";
const char LIST_CHANED[] = "client list chaned\n";
const char PING[] = "ping \n";

using std::thread;
using std::exception;
using sock = boost::asio::ip::tcp::socket;
using acceptor = boost::asio::ip::tcp::acceptor;
using endpoint = boost::asio::ip::tcp::endpoint;
using input_ip = boost::asio::ip::address;
using io_context = boost::asio::io_context;
io_context context;

struct talk_to_server {
private:
    sock sock_;
    std::string username_;
	
public:
    explicit talk_to_server(const std::string &username) :
                            sock_(context), username_(username) {}
    sock &my_socket() { return sock_; }
    std::string get_username() {
        return username_;
    }

    void connect(endpoint ep) {
        sock_.connect(ep);
        std::cout << CONNECTED << std::endl;
    }

    void write(const std::string &msg) {
        boost::asio::streambuf buffer{};
        std::ostream out(&buffer);
        out << msg;
        boost::asio::write(my_socket(), buffer);
    }

    void read() {
        boost::asio::streambuf buffer{};
        boost::asio::read_until(my_socket(), buffer, '\n');

        std::string answer(std::istreambuf_iterator<char>{&buffer},
                           std::istreambuf_iterator<char>{});

        process_request(answer);
    }

    void on_login(const std::string msg) {
        std::cout << msg << std::endl;
    }

    void on_ping(const std::string &msg) {
        if(msg == LIST_CHANED){
            write(PING);
            read();
        }
            std::cout << msg << std::endl;
    }


    void on_client(const std::string &msg) {
        std::cout << msg << std::endl;
    }

    void process_request(std::string answ) {
        if (answ.find("ping") == 0) on_ping(answ);
        if (answ.find("login") == 0) on_login(answ);
        if (answ.find("client") == 0) on_ping(answ);
        if (answ.find("For") == 0) on_client(answ);
    }

    void action() {
        write("login " + username_ + "\n");
        read();
        srand(time(NULL));
        while(true) {
            write("ping \n");
            read();
            write("clients \n");
            read();
            sleep(1+rand()%7);
        }
    }
};

void run_client(const std::string & client_name){
    endpoint ep(input_ip::from_string(STR), PORT_NUM);
    talk_to_server client(client_name);
    client.connect(ep);
    client.action();
}

int main() {
    std::string client_name;
    std::cin >> client_name;
    client_name = client_name.substr(0, client_name.length());
    std::thread th(boost::bind(run_client, client_name));
    boost::this_thread::sleep( boost::posix_time::millisec(MS));
    th.join();
    return 0;
}

#endif // INCLUDE_HEADER_HPP_
