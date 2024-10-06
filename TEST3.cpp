#include<iostream>
#include<mysql.h>
#include<mysqld_error.h>
#include<windows.h>
using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PASSW = "111307";
const char* DB = "mydbst";
int main(){
	MYSQL* conn;
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,HOST,USER,PASSW,DB,3306,NULL,0)){
		cout<<mysql_error(conn);
	}
	else{
		cout<<"logged in..."<<endl<<endl;
		Sleep(2000);
	}
	bool exit=false;
	while(!exit){
		system("cls");
		int val;
		cout<<"1 insert"<<endl;
		cout<<"2  del"<<endl;
		cout<<"3 update"<<endl;
		cout<<"0 exit"<<endl;
		cout<<"enter value";
		cin>>val;
		if(val==1){
			string id ,name,grade;
			cout<<"enter id"<<endl;
			cin>>id;
			cout<<"enter name"<<endl;
			cin>>name;
			cout<<"enter grade"<<endl;
			cin>>grade;
			
			string query = "INSERT INTO s (id,name,garde) VALES('"+id+"', '"+name+"', '"+grade+"')";
			if(mysql_query(conn,query.c_str())){
				cout<<mysql_error(conn)<<endl;
			}
			else{
				cout<<"inserted"<<endl;
			}
			Sleep(2000);
		}
	}
	mysql_close(conn);
	return 0;
}
