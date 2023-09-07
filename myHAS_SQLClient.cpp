#include "myHAS_SQLClient.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/stringbuffer.h"

using namespace rapidjson;

//#define _DEBUG_


myHAS_SQLClient::myHAS_SQLClient()
{
	//Setup
	cout<<"Reading connection settings"<<endl;
	ifstream confFile;
	string settingsJson;
	confFile.open("/var/www/html/config.env");
	
	if(confFile.is_open())
	{
		getline(confFile,settingsJson);
		confFile.close();
	}
	
	//Decode json
	Document conSettings;
	conSettings.Parse(settingsJson.c_str());
	
	cout<<"Starting mySQL..."<<endl;
	pSQLCon = mysql_init(NULL);
	while(!pSQLCon)
	{
		cout<<"mysql_init failed, trying again..."<<endl;
		sleep(5);
		pSQLCon = mysql_init(NULL);
	}
	
	cout<<"mySQL started OK..."<<endl;
	
	cout<<"Connecting to dB..."<<endl;
	
	while(mysql_real_connect(pSQLCon, conSettings["bdd_address"].GetString(), conSettings["bdd_user"].GetString(), conSettings["bdd_password"].GetString(), conSettings["bdd_name"].GetString(), 0, NULL, 0)==NULL)
	{
		cout<<"Error connecting to dB, trying again"<<endl;
		sleep(5);
	}
	
	cout<<"Connected to dB..."<<endl;
}

myHAS_SQLClient::~myHAS_SQLClient()
{
	cout<<"Closing SQL connection..."<<endl;
	mysql_close(pSQLCon);
	cout<<"SQL connection closed."<<endl;
}

bool myHAS_SQLClient::updateObject(string iTable, int iID, string sAttribute, string sValue)
{
	//Test if the object exists in the table
	
	string request ="SELECT * FROM "+iTable+" WHERE Id="+to_string(iID);
	if(!mysql_query(pSQLCon, request.c_str()))
	{
		MYSQL_RES *result = mysql_store_result(pSQLCon);
		if (!result) return false;
		int nbResult = mysql_num_rows(result);
		mysql_free_result(result);
		
		if (nbResult==0)
		{
			//Create new object in table
			string request2="";
			request2+="INSERT INTO "+iTable+" (Id,"+sAttribute+") VALUES("+to_string(iID)+",'"+sValue+"')";
			if(mysql_query(pSQLCon, request2.c_str()))
			{
				cout<<"Failed to create new row in table: "<<mysql_error(pSQLCon)<<endl;
				return false;
			}
#ifdef _DEBUG_
 			else
				cout<<iTable<<" "<<iID<<" created in table"<<endl;
#endif			
		}
		else
		{
			//Update value in table 
			string request2="";
			request2+="UPDATE "+iTable+" SET "+sAttribute+"='"+sValue+"' WHERE Id="+to_string(iID);

			if(mysql_query(pSQLCon, request2.c_str()))
			{
				cout<<"Failed to update table: "<<mysql_error(pSQLCon)<<endl;
                cout<<request2<<endl;
				return false;
			}
#ifdef _DEBUG_
			else
				cout<<iTable<<" "<<iID<<" updated in table"<<endl;
#endif				
		}
	}
	else
		return false;
	
	return true;
}

int myHAS_SQLClient::getIntValue(string iTable, int iID, string sAttribute)
{
	int result = -999999;
	
	string request="SELECT "+sAttribute+" FROM "+iTable+" WHERE Id="+to_string(iID);

    if(!mysql_query(pSQLCon, request.c_str()))
	{
		MYSQL_RES *res = mysql_store_result(pSQLCon);
		if (!res) return result;
		int nbResult = mysql_num_rows(res);
		if (nbResult!=1) 
		{
			mysql_free_result(res);
			return result;
		}
		
		result = stoi((string)(mysql_fetch_row(res)[0]));
				
		mysql_free_result(res);
	}
	return result;
}

string myHAS_SQLClient::getStringValue(string iTable, int iID, string sAttribute)
{
	string result ="";
	
	string request="SELECT "+sAttribute+" FROM "+iTable+" WHERE Id="+to_string(iID);
	if(!mysql_query(pSQLCon, request.c_str()))
	{
		MYSQL_RES *res = mysql_store_result(pSQLCon);
		if (!res) return result;
		int nbResult = mysql_num_rows(res);
		if (nbResult!=1) 
		{
			mysql_free_result(res);
			return result;
		}
		
		result = (string)mysql_fetch_row(res)[0];
		
		mysql_free_result(res);
	}
	
	return result;
}

Array<int> myHAS_SQLClient::getIntList(string iTable, string sAttribute)
{
	Array<int> result;
	
	
	
	return result;
}

Array<string> myHAS_SQLClient::getStringList(string iTable, string sAttribute)
{
	Array<string> result;
	
	string request="SELECT "+sAttribute+" FROM "+iTable;
	if(!mysql_query(pSQLCon, request.c_str()))
	{
		MYSQL_RES *res = mysql_store_result(pSQLCon);
		if (!res) return result;
		int nbResult = mysql_num_rows(res);
		if (nbResult==0) 
		{
			mysql_free_result(res);
			return result;
		}
		MYSQL_ROW row;
		while(row = mysql_fetch_row(res))
		{
			result.add((string)row[0]);
		}
		
		mysql_free_result(res);
	}
	
	return result;
}

int myHAS_SQLClient::getIntValue(string iTable, string iAttribute, string iValue, string oAttribute)
{
    int result = -999999;
	
	string request="SELECT "+oAttribute+" FROM "+iTable+" WHERE "+ iAttribute + "=\""+iValue+"\"";

	if(!mysql_query(pSQLCon, request.c_str()))
	{
		MYSQL_RES *res = mysql_store_result(pSQLCon);
		if (!res) return result;
		int nbResult = mysql_num_rows(res);
		if (nbResult!=1) 
		{
			mysql_free_result(res);
			return result;
		}
		
		result = stoi((string)(mysql_fetch_row(res)[0]));
				
		mysql_free_result(res);
	}
	
	return result;
}

int myHAS_SQLClient::getIntValue(string iTable, string iAttribute, int iValue, string oAttribute)
{
    int result = -999999;
	
	string request="SELECT "+oAttribute+" FROM "+iTable+" WHERE "+ iAttribute + "=" + to_string(iValue);
	if(!mysql_query(pSQLCon, request.c_str()))
	{
		MYSQL_RES *res = mysql_store_result(pSQLCon);
		if (!res) return result;
		int nbResult = mysql_num_rows(res);
		if (nbResult!=1) 
		{
			mysql_free_result(res);
			return result;
		}
		
		result = stoi((string)(mysql_fetch_row(res)[0]));
				
		mysql_free_result(res);
	}
	
	return result;
}

string myHAS_SQLClient::getStringValue(string iTable, string iAttribute, string iValue, string oAttribute)
{
    string result ="";
	
	string request="SELECT "+oAttribute+" FROM "+iTable+" WHERE "+ iAttribute +"=\"" + iValue + "\"";
	if(!mysql_query(pSQLCon, request.c_str()))
	{
		MYSQL_RES *res = mysql_store_result(pSQLCon);
		if (!res) return result;
		int nbResult = mysql_num_rows(res);
		if (nbResult!=1) 
		{
			mysql_free_result(res);
			return result;
		}
		
		result = (string)mysql_fetch_row(res)[0];
		
		mysql_free_result(res);
	}
	
	return result;
}

string myHAS_SQLClient::getStringValue(string iTable, string iAttribute, int iValue, string oAttribute)
{
    string result ="";
    
    string request="SELECT "+oAttribute+" FROM "+iTable+" WHERE "+ iAttribute +"=" + to_string(iValue);
	if(!mysql_query(pSQLCon, request.c_str()))
	{
		MYSQL_RES *res = mysql_store_result(pSQLCon);
		if (!res) return result;
		int nbResult = mysql_num_rows(res);
		if (nbResult!=1) 
		{
			mysql_free_result(res);
			return result;
		}
		
		result = (string)mysql_fetch_row(res)[0];
		
		mysql_free_result(res);
	}
	
	return result;
}
