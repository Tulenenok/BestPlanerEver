#pragma once
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sstream>


#include <boost/bind/bind.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <string>
#include <vector>




using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>


class InterClient {
public:

	InterClient() {
		run();
	}

	~InterClient() {
    	close(sock);
	}

  std::vector<std::string> get_all_tasks_by_userid(int user_id);

  int login(std::string user_login, std::string user_password);
  
  int registration(std::string user_login, std::string user_password, std::string user_email);

  int add_new_task(int user_id, int task_id, std::string task);

  int delete_task(int user_id, int task_id);

  int alter_task(int user_id, int task_id, std::string new_task);


private:	
	void write_to_server(int filedes, std::string msg);
	
	std::string read_from_server(int filedes);
	
	void init_sockaddr(struct sockaddr_in* name,
	                   const char* hostname,
	                   uint16_t port);


	int run();
private:
	int sock;
  	http::request<http::string_body>  request_;
  	http::response<http::string_body> response_;
};
