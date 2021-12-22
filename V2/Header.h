#pragma once
#include <mysql.h>
#include <iostream>
#include<string>
#include<algorithm>
#include<vector>
using namespace std;
int qstate;
class BestPlanDB : public MYSQL
{
public:
	BestPlanDB() = default;
	BestPlanDB(const BestPlanDB&) = delete;
	~BestPlanDB() = default;

	BestPlanDB& operator = (const BestPlanDB&) = delete;

    int New_Task(int user_id,int new_task_id, std::string new_task);
    int Update_Task(int user_id,int new_task_id, std::string new_task);
    vector<string> get_all_tasks(int user_id);
    vector<pair<int, vector<string>>>login(std::string user_login,std::string user_password);
    int Insert_New_User(string user_login,string user_mail,string user_password);
    int Insert_New_Task(int user_id,vector<string>tasks);
    int Delete_Task(int user_id,int new_task_id);
private:

	MYSQL* MakeConnection() const;
	bool CheckConnection(const MYSQL* connect);

};