// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <ctime>
#include <string>

#define PORT_NUM 8001
#define MS 100
#define SEC 7

const char STR[] = "127.0.0.1";
const char CONNECTED[] = "client connected ";
const char LIST_CHANED[] = "client list chaned\n";
const char PING[] = "ping \n";
const char PING_OK[] = "ping";
const char LOGIN[] = "login";
const char CLIENT[] = "client";
const char LIST[] = "List";
const char CLIENTS[] = "clients \n";


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
    explicit talk_to_server(const std::string &username);
    sock &my_socket();
    std::string get_username();
    void connect(endpoint ep);
    void write(const std::string &msg);
    void read();
    void on_login(const std::string msg);
    void on_ping(const std::string &msg);
    void on_client(const std::string &msg);
    void process_request(std::string answ);
    void action() ;
};

#endif // INCLUDE_HEADER_HPP_
