#include "myHAS_MQTTClient.h"
#include <iostream>
#include <unistd.h> 
#include <fstream>
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

myHAS_MQTTClient::~ myHAS_MQTTClient()
{
	cout<<"Stopping Mosquitto services..."<<endl;
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	cout<<"Mosquitto services stopped."<<endl;
}

void myHAS_MQTTClient::start_MQTT(const char * iClientID)
{
	if(mosquitto_started) return;
	
	setTopics();
	
	//Setup
	//cout<<"Reading connection settings"<<endl;
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
	
	start_MQTT(iClientID, conSettings["mqtt_server"].GetString(), conSettings["mqtt_port"].GetInt(), conSettings["mqtt_user"].GetString(), conSettings["mqtt_password"].GetString());
}

void myHAS_MQTTClient::start_MQTT(const char * iClientID, const char * iMqttServer, int iMqttPort, const char * iMqttLogin, const char * iMqttPassword)
{
	if(mosquitto_started) return;
	
	int rc;
	mosquitto_lib_init();
	
	mosq = mosquitto_new(iClientID, true, this);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	mosquitto_username_pw_set(mosq, iMqttLogin, iMqttPassword);

	while(rc=mosquitto_connect(mosq, iMqttServer, iMqttPort, 10)) 
	{
		cout<<"Could not connect to Broker with return code"<<rc<<endl;
		cout<<"Trying again..."<<endl;
		sleep(5);
	}
	cout<<"Connected to Mosquitto..."<<endl;
	
	mosquitto_started = true;

	mosquitto_loop_start(mosq);
}

void myHAS_MQTTClient::on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	((myHAS_MQTTClient *)obj)->mqtt_message(mosq, obj, msg);
}

void myHAS_MQTTClient::on_connect(struct mosquitto *mosq, void *obj, int rc)
{
	((myHAS_MQTTClient *)obj)->mqtt_connect(mosq, obj, rc);
}

void myHAS_MQTTClient::mqtt_connect(struct mosquitto *mosq, void *obj, int rc)
{
	cout<<"Connected to MQTT server"<<endl;
	if(rc) {
		cout<<"Error with result code: "<<rc<<endl;
		exit(-1);
	}
	
	for(int i=0; i<topicList.size(); i++)
    {
		mosquitto_subscribe(mosq, NULL, topicList[i].c_str(), 0);
    }
}

void myHAS_MQTTClient::addTopic(string iTopic)
{
  topicList.add(iTopic);
}
