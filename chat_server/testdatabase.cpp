#include <mysql/mysql.h>
#include <iostream>
#include <string>

using namespace std;
int main(){
    
    MYSQL *mysql;
    //初始化数据库句柄
    mysql = mysql_init(NULL);
    
    mysql = mysql_real_connect(mysql,"localhost","root","root","neo",3306,NULL,0);
    if(!mysql)
        {
            std::cout << "mysql_real_connect err" << std::endl;
            return -1;
        }
    if(mysql_query(mysql, "set names utf8;")!=0)
	{
		std::cout<<"set names utf8 error"<<std::endl;
		return -1;
	}

    string sql = "create table if not exists \
        chat_group(groupname varchar(16),groupowner varchar(128),groupmember varchar(4096))charset utf8;";
    
    if(mysql_query(mysql, sql.c_str())){
        cout << "create table chat_group err %"<< mysql_error(mysql) << endl;
        return -1;
    }
    cout << "create table chat_group success" << endl;
    sql = "create table if not exists \
        chat_user(username varchar(128),groupowner varchar(128),friendlist varchar(4096),grouplist varchar(4096))charset utf8;";
    
    if(mysql_query(mysql, sql.c_str())){
        cout << "create table chat_user err" << mysql_error(mysql) << endl;
        return -1;
    }
    cout << "create table chat_user success" << endl;

}