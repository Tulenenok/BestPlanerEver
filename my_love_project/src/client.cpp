#include "client.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;


std::vector<std::string> InterClient::get_all_tasks_by_userid(int user_id) {
	std::string path = {"/user/"};
	path += std::to_string(user_id) + "/tasks";

	request_.version(10);
	request_.method(http::verb::get);
	request_.target(path);
	request_.set(http::field::host, "localhost");
	request_.set(http::field::accept, "application/json");
	request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);



	std::stringstream in;
	in << request_;
	write_to_server(sock, in.str());
    std::string answer = read_from_server(sock);

	size_t body_index = answer.find("\r\n\r\n") + strlen("\r\n\r\n");
	std::string body_ = answer.substr(body_index);
	//std::cout << body_ << std::endl;

	json ans = json::parse(body_);

	//std::cout << ans << std::endl;
	

	if (answer.find("HTTP/1.0 200 OK") == std::string::npos) {
		std::cerr << "add_new_task bad status" << std::endl;
		std::vector<std::string> ret;
		return ret;
	}
	std::vector<std::string> all_the_tasks = ans["tasks"];

	return all_the_tasks;
}


int InterClient::login(std::string user_login, std::string user_password) {
    std::string path = {"/login"};
	std::string body = "{\"login\":\"" + user_login + "\"," + "\"password\":\"" + user_password + "\"}";

	request_.version(10);
	request_.method(http::verb::get);
	request_.target(path);
	request_.set(http::field::content_length, std::to_string(body.length()));
	request_.set(http::field::host, "localhost");
	request_.set(http::field::accept, "application/json");
	request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	request_.body() = body;


	std::stringstream in;
	in << request_;
	write_to_server(sock, in.str());
    std::string answer = read_from_server(sock);

	size_t body_index = answer.find("\r\n\r\n") + strlen("\r\n\r\n");
	std::string body_ = answer.substr(body_index);
	//std::cout << body_ << std::endl;

	json ans = json::parse(body_);

	//std::cout << ans << std::endl;

	if (answer.find("HTTP/1.0 200 OK") == std::string::npos) {
		std::cerr << "add_new_task bad status" << std::endl;
		return -1;
	}

	//check location
	return 0;
}


int InterClient::add_new_task(int user_id, int task_id, std::string task) {
  	std::string path = {"/user/"};
	path += std::to_string(user_id) + "/tasks/new/" + std::to_string(task_id);
	std::string req = "{\"task\": \"" + task + "\"}";

	request_.version(10);
	request_.method(http::verb::post);
	request_.target(path);
	request_.set(http::field::host, "localhost");
	request_.set(http::field::content_length, std::to_string(req.length()));
	request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	request_.body() = req;

	
	std::stringstream in;
	in << request_;
	//std::cout << in.str()+ << std::endl; 
	write_to_server(sock, in.str());
	std::string answer = read_from_server(sock);
	
	if (answer.find("HTTP/1.0 200 OK") == std::string::npos) {
		std::cerr << "add_new_task bad status" << std::endl;
		return -1;
	}
	//get Location
	return 0;
}


int InterClient::delete_task(int user_id, int task_id) {
	std::string path = {"/user/"};
	path += std::to_string(user_id) + "/tasks/delete/" + std::to_string(task_id);

	request_.version(10);
	request_.method(http::verb::post);
	request_.target(path);
	request_.set(http::field::host, "localhost");
	request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

	
	std::stringstream in;
	in << request_;
	write_to_server(sock, in.str());
	std::string answer = read_from_server(sock);
	
	if (answer.find("HTTP/1.0 200 OK") == std::string::npos) {
		std::cerr << "delete_task bad status" << std::endl;
		return -1;
	}
	return 0;
  }


int InterClient::alter_task(int user_id, int task_id, std::string new_task) {
	std::string path = {"/user/"};
	path += std::to_string(user_id) + "/tasks/alter/" + std::to_string(task_id);
	std::string req = "{\"task\": \"" + new_task + "\"}";

	request_.version(10);
	request_.method(http::verb::post);
	request_.target(path);
	request_.set(http::field::host, "localhost");
	request_.set(http::field::content_length, std::to_string(req.length()));
	request_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	request_.body() = req;
	
	std::stringstream in;
	in << request_;
	write_to_server(sock, in.str());
	std::string answer = read_from_server(sock);
	
	if (answer.find("HTTP/1.0 200 OK") == std::string::npos) {
		std::cerr << "alter_task bad status" << std::endl;
		return -1;
	}
	return 0;
  }


void InterClient::write_to_server(int filedes, std::string msg) {
  size_t left = msg.size();
  ssize_t sent = 0;

  int flags = 0;
  while (left > 0) {
    sent = ::send(filedes, msg.data() + sent, msg.size() - sent, flags);
    if (-1 == sent)
      throw std::runtime_error("write failed: " + std::string(strerror(errno)));

    left -= sent;
  }

  printf("write message %s to socket \n", msg.c_str());
}


std::string InterClient::read_from_server(int filedes) {
	  char buf[1024];
	
	  int n = ::recv(filedes, buf, sizeof(buf), MSG_NOSIGNAL);
	
	  if (-1 == n && errno != EAGAIN)
	    throw std::runtime_error("read failed: " + std::string(strerror(errno)));
	  if (0 == n)
	    throw std::runtime_error("client: " + std::to_string(filedes) +
	                             " disconnected");
	  if (-1 == n)
	    throw std::runtime_error("client: " + std::to_string(filedes) +
	                             " timeouted");
	
	  std::string ret(buf, buf + n);
	
	  return ret;
	}


void InterClient::init_sockaddr(struct sockaddr_in* name,
                   const char* hostname,
                   uint16_t port) {
  name->sin_family = AF_INET;
  name->sin_port = htons(port);

  struct hostent* hostinfo;
  hostinfo = gethostbyname(hostname);
  if (hostinfo == NULL) {
    fprintf(stderr, "Unknown host %s.\n", hostname);
    exit(EXIT_FAILURE);
  }
  name->sin_addr = *(struct in_addr*)hostinfo->h_addr;
}

int InterClient::run() {
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket (client)");
		exit(EXIT_FAILURE);
	}
	std::string host("127.0.0.1");
	int port = std::stoi("8080");
	struct sockaddr_in servername;
	init_sockaddr(&servername, host.c_str(), port);
	if (0 > connect(sock, (struct sockaddr*)&servername, sizeof(servername))) {
		perror("connect (client)");
		exit(EXIT_FAILURE);
	}
	return 0;
}
