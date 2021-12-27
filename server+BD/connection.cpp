#include "connection.h"

#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/beast/http/write.hpp>
#include <iostream>

#include <string>
#include <utility>
#include <sstream>

#include <Header.h>


#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace server3 {

    Connection::Connection(tcp::socket&& socket)
            : stream_(std::move(socket))
    {
        //database = new BestPlanDB;
    }


    void Connection::start()
    {
        net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(&Connection::do_read,
                                         shared_from_this()));
    }


    void Connection::do_read()
    {
        request_ = {};
        // Read a request
        http::async_read(stream_, buffer_, request_,
                         beast::bind_front_handler(
                                 &Connection::handle_read,
                                 shared_from_this()));
    }


    int get_user_id(const std::string& str) {
        size_t first_index = str.find("user/") + strlen("user/");
        std::string sub = str.substr(first_index);
        size_t end_index = sub.find("/");
        int user_id = std::stoi(str.substr(first_index, end_index));
        return user_id;
    }


    std::pair<int, std::string> get_task(const std::string& str) {
        size_t first_index = str.find("/tasks/") + strlen("/tasks/");
        std::string sub = str.substr(first_index);
        size_t end_index = sub.find("/") + strlen("/");
        std::string operation = str.substr(first_index, end_index - 1);
        std::string sub2 = sub.substr(end_index);
        int task_id = std::stoi(sub2);
        return std::make_pair<int, std::string>(std::move(task_id), std::move(operation));
    }

    struct return_values_t {
        std::vector<std::string> tasks;
        std::pair<int, std::string> task;
        std::vector<std::pair<int, std::vector<string>>> from_login;

        int get_task_id_from_task() {
            return std::get<0>(task);
        }

        std::string get_operation_from_task() {
            return std::get<1>(task);
        }

        std::vector<std::string> tasks_from_all_tasks() {
            if (tasks.size() == 0) {
                //std::cerr << "blank return" << std::endl;
                return std::vector<std::string>();
            }
            return tasks;
        }
        
        int user_id_from_login() {
            return std::get<0>(from_login[0]);
        }
        std::vector<std::string> tasks_from_login() {
            if (std::get<1>(from_login[0]).size() == 0) {
                //std::cerr << "blank return" << std::endl;
                return std::vector<std::string>();
            }
            return std::get<1>(from_login[0]);
        }
        int task_id_from_login() {
            return std::get<0>(from_login[0]);
        }
    };


    void Connection::handle_read(beast::error_code e,
                                 std::size_t bytes_transferred)
    {
        if( e == http::error::end_of_stream) {
            return do_close();
        }
        
        if (!e)
        {
            http::response<http::string_body> res{http::status::bad_request, request_.version()};
            std::string request_str = request_.target().to_string();
            std::string response_body;
            
            BestPlanDB database;

            std::cout << "request:\n" << request_ << std::endl << std::endl;        

            if (request_.method() == http::verb::get) {
                if(request_.target().find("/user/") != std::string::npos) {
                    return_values_t return_value;

                    int user_id = get_user_id(request_str);
                    std::string user_part = "\"userId\": " + std::to_string(user_id) + " ";
                    
                    return_value.tasks = database.get_all_tasks(user_id);

                    json j_response;
                   
                    j_response["userID"] = user_id;
                    j_response["tasks"] = return_value.tasks_from_all_tasks();
                                            
                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.result(http::status::ok);
                    res.set(http::field::connection, "Keep-Alive");
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                    //std::cerr << "sent in get all users:\n" << response_body << std::endl;
                }
                else if (request_str.find("/login") != std::string::npos) {
                    return_values_t return_value;
                    json j_req = json::parse(request_.body());
                    return_value.from_login = database.login(j_req["login"], j_req["password"]);

                    json j_response;
                    int user_id = return_value.user_id_from_login();
                    j_response["user_id"] = user_id;

                    j_response["task_id"] = return_value.task_id_from_login();
                    j_response["tasks"] = return_value.tasks_from_login();

                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::location, "/user/" + std::to_string(user_id));
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
	                res.set(http::field::connection, "Keep-Alive");
                    res.result(http::status::ok);
                    response_body = j_response.dump();
                    res.body() = response_body;
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                }
                else {
                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.set(http::field::content_type, "application/json");
                    res.set(http::field::connection, "Keep-Alive");
                    res.result(http::status::not_found);
                    res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                }
            }
            else if (request_.method() == http::verb::post) {
                if(request_.target().find("/tasks/") != std::string::npos) {
                    return_values_t return_value;
                    int user_id = get_user_id(request_str);
                    //std::pair<int, std::string> task = get_task(request_str);
                    return_value.task = get_task(request_str);
                    std::string operation = return_value.get_operation_from_task();
                    int task_id = return_value.get_task_id_from_task();

                    std::string user_part = "\"userId\": " + std::to_string(user_id);
                    std::string task_part = "\"taskId\": " + std::to_string(task_id) + "," + "\"operation\": " + operation;
                    if (operation == "new") {
                        json j_req = json::parse(request_.body());
                        int result = database.New_Task(user_id, task_id, j_req["task"]);
                        if (result != 0) {
                            std::cerr<< "New_task went wrong" << std::endl;
                        }
                    }
                    else if (operation == "alter") {
                        json j_req = json::parse(request_.body());
                        int result = database.New_Task(user_id, task_id, j_req["task"]);
                        if (result != 0) {
                            std::cerr<< "New_task went wrong" << std::endl;
                        }
                    }
                    else if (operation == "delete") {
                        int result = database.Delete_Task(user_id, task_id);
                        if (result != 0) {
                            std::cerr<< "delete_task went wrong" << std::endl;
                        }
                    }
                    else {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.set(http::field::connection, "Keep-Alive");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                                            
                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.result(http::status::ok);
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                }
                else if (request_.target().find("/registration/") != std::string::npos) {
                    json j_req = json::parse(request_.body());
                    int result = database.Insert_New_User(j_req["login"], j_req["email"], j_req["password"]);

                    if (result != 0) {
                        std::cerr<< "New_task went wrong" << std::endl;
                    }

                    return_values_t return_value;
                    return_value.from_login = database.login(j_req["login"], j_req["password"]);
                    int user_id = return_value.user_id_from_login();
                    database.Insert_New_Task(user_id, std::vector<string>(7,"empty task"));


                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.version(10);
                    res.set(http::field::content_type, "application/json; charset=UTF-8");
                    res.set(http::field::connection, "Keep-Alive");
                    res.set(http::field::location, "/user/" + std::to_string(result));
                    res.result(http::status::ok);
                    res.set(http::field::content_length, std::to_string(response_body.length()));
                }
                else {
                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.set(http::field::content_type, "application/json");
                    res.set(http::field::connection, "Keep-Alive");
                    res.result(http::status::not_found);
                    res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                }
            }
            else
            {
                res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                res.set(http::field::content_type, "application/json");
                res.set(http::field::connection, "Keep-Alive");
                res.result(http::status::not_found);
                res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
            }
            auto sp = std::make_shared<http::message<false, http::string_body>>(std::move(res));
            res_ = sp;
            
            http::async_write(stream_,
                    *sp,
                    beast::bind_front_handler(
                            &Connection::handle_write,
                            shared_from_this(),
                            sp->need_eof()));
        }
    }

    void Connection::handle_write(bool close,
                                  beast::error_code e,
                                  std::size_t bytes_transferred)
    {
        if(close)
        {
            return do_close();
        }
        if (!e)
        {
            do_read();
        }
    }


    void Connection::do_close()
    {
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
    }

} // namespace server3
