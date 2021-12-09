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



        void Connection::handle_read(beast::error_code e,
                                     std::size_t bytes_transferred)
        {
            if( e == http::error::end_of_stream)
                return do_close();

            if (!e)
            {
                http::response<http::string_body> res{http::status::bad_request, request_.version()};
                std::string request_str = request_.target().to_string();
                std::string response_body;


                BestPlanDB database;

                // int status = database.Insert_New_User("login1", "main1", "password1");
                // if (status == 0) {
                //     std::cout << "user_inserted" << std::endl;
                // }
                // std::vector<std::string> tasks = {"NEWTASK", "NEWTASK1", "NEWTASK1", "NEWTASK1", "NEWTASK1", "NEWTASK1", "NEWTASK1"};
                // status = database.Delete_Task(5, 5);
                // if (status == 0) {
                //     std::cout << "task_added" << std::endl;
                // }


                if (request_.method() == http::verb::get) {
                    if(request_.target().find("/user/") != std::string::npos) {
                        int user_id = get_user_id(request_str);
                        std::string user_part = "\"userId\": " + std::to_string(user_id) + " ";
                        std::vector<string> taskss = database.get_all_tasks(user_id);

                        std::cout << taskss.size() <<std::endl;
                        json j_response;
                        ////////////form json from vector
                        
                        j_response["userID"] = user_id;
                        j_response["tasks"] = "tasks";
                                                
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.result(http::status::ok);

                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                    }
                    else if (request_str.find("/login") != std::string::npos) {

                        std::cout << request_.body() << std::endl;
                        json j_req = json::parse(request_.body());

                        vector<pair<int, vector<string>>> user_vec = database.login(j_req["login"], j_req["password"]);

                        json j_response;
                        int user_id = std::get<0>(user_vec[0]);
                        j_response["user_id"] = user_id;

                        std::pair<int, vector<string>> pair_ = user_vec[0];
                        j_response["task_id"] = std::get<0>(pair_);
                        //vector to json
                        for (auto& task : std::get<1>(pair_)) {
                            std::cout << task <<std::endl;
                        }


                        std::string string_tasks = "{";
                        for (auto& task : std::get<1>(pair_)) {
                            string_tasks += "\"task\":" + task + ", " ;
                        }
                        string_tasks += "}";

                        std::cout << "final json:\n" << string_tasks << std::endl;

                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::location, "/user/" + std::to_string(user_id));
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.result(http::status::ok);
                        response_body = j_response.dump();
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                    }
                    else {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                }
                else if (request_.method() == http::verb::post) {
                    if(request_.target().find("/tasks/") != std::string::npos) {
                        int user_id = get_user_id(request_str);
                        std::pair<int, std::string> task = get_task(request_str);
                        std::string user_part = "\"userId\": " + std::to_string(user_id);
                        std::string task_part = "\"taskId\": " + std::to_string(std::get<0>(task)) + "," + "\"operation\": " + std::get<1>(task);

                        if (std::get<1>(task) == "new") {
                            json j_req = json::parse(request_.body());
                            int result = database.New_Task(user_id, std::get<0>(task), j_req["task"]);
                            if (result != 0) {
                                std::cerr<< "New_task went wrong" << std::endl;
                            }
                        }
                        else if (std::get<1>(task) == "alter") {
                            json j_req = json::parse(request_.body());
                            int result = database.New_Task(user_id, std::get<0>(task), j_req["task"]);
                            if (result != 0) {
                                std::cerr<< "New_task went wrong" << std::endl;
                            }
                        }
                        else if (std::get<1>(task) == "delete") {
                            int result = database.Delete_Task(user_id, std::get<0>(task));
                            if (result != 0) {
                                std::cerr<< "delete_task went wrong" << std::endl;
                            }
                        }
                        else {
                            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                            res.set(http::field::content_type, "application/json");
                            res.result(http::status::not_found);
                            res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                        }

                                                
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.version(10);
                        res.set(http::field::content_type, "application/json; charset=UTF-8");
                        res.result(http::status::ok);
                        response_body = "{" + user_part + "," + task_part  + "," + "\"data\" : \"some data\"" + "}";
                        res.body() = response_body;
                        res.set(http::field::content_length, std::to_string(response_body.length()));
                    }
                    else {
                        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                        res.set(http::field::content_type, "application/json");
                        res.result(http::status::not_found);
                        res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
                    }
                }
                else
                {
                    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
                    res.set(http::field::content_type, "application/json");
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

