
#include <mysql/mysql.h>
#include <iostream>
#include"Header.h"
#include<algorithm>
#include<cstring>
using namespace std;


MYSQL* BestPlanDB:: MakeConnection() const
{
	MYSQL* conn;

	conn = mysql_init(0);

	conn = mysql_real_connect(conn, "localhost", "root", "newpassword", "DB", 3306, NULL, 0);
	return conn;
}


bool BestPlanDB::CheckConnection(const  MYSQL* connect)
{
	bool status;
	(connect) ? status = true : status = false;
	return status;
}


int BestPlanDB::Insert_New_Task(int user_id,vector<string>tasks)
{
    MYSQL* newconn = MakeConnection();
    MYSQL_ROW row;
    MYSQL_RES* res;

    if (CheckConnection(newconn))
    {

        string  query = "INSERT INTO `Tasks` (`TaskID`, `Task1`, `Task2`, `Task3`, `Task4`, `Task5`, `Task6`, `Task7`, `Userid`) VALUES (NULL";
        for(int i=0;i<7;i++)
        {
            query+=", ";
            query+="\'";
            query+=tasks[i];
            query+="\'";


        }
        query+=", ";
        query+="\'";
        query+=std::to_string(user_id);
        query+="\'";
        query+=");";
        const char* q = query.c_str();

        qstate = mysql_query(newconn, q);

        if (!qstate)
        {

            res = mysql_store_result(newconn);





            // Закрытие соединения
            mysql_close(newconn);

            return 0;
        }
        else
        {
            return -1;
        }
    }
}




int BestPlanDB::New_Task(int user_id,int new_task_id ,std::string new_task)
{
    MYSQL* newconn = MakeConnection();
    MYSQL_ROW row;
    MYSQL_RES* res;
    std::string check;
    switch (new_task_id) {
        case 1: check="Task1";break;
        case 2: check="Task2";break;
        case 3: check="Task3";break;
        case 4: check="Task4";break;
        case 5: check="Task5";break;
        case 6: check="Task6";break;
        case 7: check="Task7";break;


    }
    if (CheckConnection(newconn))
    {

        string  query = "UPDATE `Tasks` SET ";
        query+=check;
        query+="= ";
        query+="\"";
        query +=new_task;
        query+="\"";
        query+=" ";
        query+=" WHERE Userid= ";
        query+=std::to_string(user_id);
        const char* q = query.c_str();

        qstate = mysql_query(newconn, q);

        if (!qstate)
        {

            res = mysql_store_result(newconn);





            // Закрытие соединения
            mysql_close(newconn);

            return 0;
        }
        else
        {
            return -1;
        }
    }
}
vector<string> BestPlanDB::get_all_tasks(int user_id)
{
    vector<string> arr;
    arr.reserve(7);
    MYSQL* newconn = MakeConnection();
    MYSQL_ROW row;
    MYSQL_RES* res;

    if (CheckConnection(newconn))
    {
        string query = "SELECT * FROM `Tasks` WHERE Userid= ";
        query+=to_string(user_id);
        int i=1;
        const char* q = query.c_str();

        qstate = mysql_query(newconn, q);


        if (!qstate)
        {
            res = mysql_store_result(newconn);


            while ((row = mysql_fetch_row(res) ))
            {
                for(;i<8;++i)
                { arr.push_back(row[i]);}



            }
            for(int i=0;i<8;i++)
            {
                if(arr[i]=="")
                {
                    arr[i]="empty task";
                }

            }
            return arr;
            mysql_close(newconn);
        }
        else
        {
            cout << "Query failed: " << mysql_error(newconn) << endl;
            mysql_close(newconn);
        }
    }

}
vector<pair<int, vector<string>>> BestPlanDB:: login(string user_login,string user_password)
{

    MYSQL* newconn = MakeConnection();
    MYSQL_ROW row;
    MYSQL_RES* res;

    if (CheckConnection(newconn))
    {
        string query = "SELECT UserID FROM `User` WHERE Login=";
        query+="\"";
        query +=user_login;
        query+="\"";
        query+=" AND Password=";
        query+="\"";
        query +=user_password;
        query+="\"";
        const char* q = query.c_str();

         qstate  = mysql_query(newconn, q);

        char buf[32];
        int count=0;

        if (!qstate)
        {
            res = mysql_store_result(newconn);
            while ((row = mysql_fetch_row(res)))
            {
                unsigned long* length = mysql_fetch_lengths(res);
                size_t size = length[0] < sizeof(buf)-1 ? length[0] : sizeof(buf)-1;
                memcpy(buf, row[0], size);
                buf[size] = '\0';
                count = (unsigned long)atoi(buf);
            }
            if(count==0)
            {
                vector<string> arr{"nothing"};
                vector<pair<int, vector<string>>> result;
                result.push_back(make_pair(count,arr));


                return result;
            }

            vector<string> arr= BestPlanDB::get_all_tasks(count);
            vector<pair<int, vector<string>>> result;
            result.push_back(make_pair(count,arr));


            return result;
            mysql_close(newconn);
        }
        else
        {
            cout << "Query failed: " << mysql_error(newconn) << endl;
            mysql_close(newconn);
        }


    }
}
int BestPlanDB::Update_Task(int user_id,int new_task_id, std::string new_task)
{
    MYSQL* newconn = MakeConnection();
    MYSQL_ROW row;
    MYSQL_RES* res;
    std::string check;
    switch (new_task_id) {
        case 1: check="Task1";break;
        case 2: check="Task2";break;
        case 3: check="Task3";break;
        case 4: check="Task4";break;
        case 5: check="Task5";break;
        case 6: check="Task6";break;
        case 7: check="Task7";break;


    }
    if (CheckConnection(newconn))
    {

        string  query = "UPDATE `Tasks` SET ";
        query+=check;
        query+="= ";
        query+="\"";
        query +=new_task;
        query+="\"";
        query+=" ";
        query+=" WHERE Userid= ";
        query+=std::to_string(user_id);
        const char* q = query.c_str();

        qstate = mysql_query(newconn, q);

        if (!qstate)
        {

            res = mysql_store_result(newconn);





            // Закрытие соединения
            mysql_close(newconn);

            return 0;
        }
        else
        {
            return -1;
        }
    }
}

int BestPlanDB::Insert_New_User(string user_login,string user_mail,string user_password)
{
    MYSQL* newconn = MakeConnection();
    MYSQL_ROW row;
    MYSQL_RES* res;

    if (CheckConnection(newconn))
    {

        string  query = "INSERT INTO `User` (`UserID`, `Login`, `Password`, `Email`) VALUES (NULL, ";

        query+="\'";
        query +=user_login;
        query+="\'";
        query+=", ";
        query+="\'";
        query +=user_password;
        query+="\'";
        query+=", ";
        query+="\'";
        query +=user_mail;
        query+="\'";
        query+=");";
        const char* q = query.c_str();

        qstate = mysql_query(newconn, q);

        if (!qstate)
        {

            res = mysql_store_result(newconn);





            // Закрытие соединения
            mysql_close(newconn);

            return 0;
        }
        else
        {
            return -1;
        }
    }

}
int BestPlanDB::Delete_Task(int user_id,int task_id)
{
    BestPlanDB::Update_Task(user_id,task_id,"");
    return 0;
}

