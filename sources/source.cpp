// Copyright 2018 Your Name <your_email>

#include <header.hpp>
talk_to_server::talk_to_server(const std::string &username)
                    : sock_(context), username_(username) {}

sock &talk_to_server::my_socket() { return sock_; }

std::string talk_to_server::get_username() {
    return username_;
}

void talk_to_server::connect(endpoint ep) {
    sock_.connect(ep);
    std::cout << CONNECTED << std::endl;
}

void talk_to_server::write(const std::string &msg) {
    boost::asio::streambuf buffer{};
    std::ostream out(&buffer);
    out << msg;
    boost::asio::write(my_socket(), buffer);
}

void talk_to_server::read() {
    boost::asio::streambuf buffer{};
    boost::asio::read_until(my_socket(), buffer, '\n');

    std::string answer(std::istreambuf_iterator<char>{&buffer},
            std::istreambuf_iterator<char>{});

    process_request(answer);
    }

void talk_to_server::on_login(const std::string msg) {
    std::cout << msg << std::endl;
    }

void talk_to_server::on_ping(const std::string &msg) {
    if (msg == LIST_CHANED){
        write(PING);
        read();
    }
    std::cout << msg << std::endl;
}


void talk_to_server::on_client(const std::string &msg) {
    std::cout << msg << std::endl;
}

void talk_to_server::process_request(std::string answ) {
    if (answ.find(PING_OK) == 0) on_ping(answ);
    if (answ.find(LOGIN) == 0) on_login(answ);
    if (answ.find(CLIENT) == 0) on_ping(answ);
    if (answ.find(LIST) == 0) on_client(answ);
}

void talk_to_server::action() {
    write(LOGIN + username_ + "\n");
    read();
    while (true) {
        write(PING);
        read();
        write(CLIENTS);
        read();
        unsigned r_time = time(nullptr);
        sleep(rand_r(&r_time) % SEC);
    }
}


void run_client(const std::string & client_name){
    endpoint ep(input_ip::from_string(STR), PORT_NUM);
    talk_to_server client(client_name);
    try {
        client.connect(ep);
        client.action();
    } catch (boost::system::system_error &e){
        std::cout << e.what() << std::endl;
    }
}

int main() {
    std::string client_name;
    std::cin >> client_name;
    client_name = client_name.substr(0, client_name.length());
    std::thread th(boost::bind(run_client, client_name));
    boost::this_thread::sleep(boost::posix_time::millisec(MS));
    th.join();
    return 0;
}
