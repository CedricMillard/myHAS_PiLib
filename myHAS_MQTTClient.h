#ifndef myHAS_MQTTClient_h
#define myHAS_MQTTClient_h

#include <string>
#include "../include/ArrayCed.h"
#include <mosquitto.h>

using namespace std;

class myHAS_MQTTClient
{
	public:
		myHAS_MQTTClient(){}
		~myHAS_MQTTClient();
		
		void start_MQTT(const char * iClientID, const char * iMqttServer, int iMqttPort, const char * iMqttLogin, const char * iMqttPassword);
		//Will use the default config file to fetch the MQTT parameters
		void start_MQTT(const char * iClientID);
		
		static void on_connect(struct mosquitto *mosq, void *obj, int rc);
		static void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
	
		void mqtt_connect(struct mosquitto *mosq, void *obj, int rc);
		virtual void mqtt_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) = 0;
		
		
	protected:
		void addTopic(string iTopic);
		virtual void setTopics() = 0;
		
		Array<string> topicList;
		string ID;
		bool mosquitto_started = false;
		
	private:
		struct mosquitto *mosq;
		
};

#endif
