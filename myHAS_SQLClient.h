#ifndef myHAS_SQLClient_h
#define myHAS_SQLClient_h

#include <string>
#include "../include/ArrayCed.h"
#include <mysql/mysql.h>

using namespace std;

class myHAS_SQLClient
{
	public:
		myHAS_SQLClient();
		~myHAS_SQLClient();
		bool updateObject(string iTable, int iID, string sAttribute, string sValue);
		
		int getIntValue(string iTable, int iID, string sAttribute);
		int getIntValue(string iTable, string iAttribute, string iValue, string oAttribute);
		int getIntValue(string iTable, string iAttribute, int iValue, string oAttribute);
		string getStringValue(string iTable, int iID, string sAttribute);
		string getStringValue(string iTable, string iAttribute, string iValue, string oAttribute);
		string getStringValue(string iTable, string iAttribute, int iValue, string oAttribute);
		
		Array<int> getIntList(string iTable, string sAttribute);
		Array<string> getStringList(string iTable, string sAttribute);
	
	private:
		MYSQL *pSQLCon =NULL;
};

#endif
