#include <iostream>
#include "MySqlHelper.h"
#include <string>

int main()
{
	MySqlHelper* sqlHelper = new MySqlHelper("localhost", "root", "6475158");

	sqlHelper->initConnection();
	sqlHelper->switchDataBase("testDb");
	sqlHelper->executeQuery("drop table if exists DataTbl;");
	sqlHelper->executeQuery("create table if not exists DataTbl(id int not null auto_increment primary key, DataFld varchar(20));");
	sqlHelper->executeQuery("insert into DataTbl(DataFld) values('firstVal');");
	sqlHelper->executeQuery("insert into DataTbl(DataFld) values('secondVal');");
	sqlHelper->executeQuery("insert into DataTbl(DataFld) values('thirdVal');");

	//sqlHelper->firstPrint();
	//sqlHelper->secondPrint();
	sqlHelper->printCommaSeparatedValues();

	delete sqlHelper;
}