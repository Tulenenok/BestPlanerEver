#include <iostream>
#include "client.h"



int main(int argc, char* argv[]) {
  InterClient client;
  //client.login("first_user", "1111");
  client.get_all_tasks_by_userid(1);
  //client.add_new_task(1, 1, "client task");
  //client.delete_task(1, 1);
  //client.alter_task(1, 1, "altered task");
  
  return 0;
}
