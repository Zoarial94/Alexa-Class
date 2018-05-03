#ifndef ALEXASKILL_HPP
#define ALEXASKILL_HPP

//Dependencies

//Web Server
#include "server_https.hpp"
//std::shared_ptr
#include <memory>
//std::function 
#include <functional>
//std::map
#include <map>
//boost::property_tree::ptree - Object to hold json responses and requests
#include <boost/property_tree/ptree.hpp>

//HTTPS Server Object
using HttpsServer = SimpleWeb::Server<SimpleWeb::HTTPS>;

namespace pt = boost::property_tree;

//Namespaces and classes
namespace Alexa {
	
	//Info on requests and responses are at https://developer.amazon.com/docs/custom-skills/request-and-response-json-reference.html
	
	//Holds request from alexa with convenience functions
	class Request {
		//Holds raw request
		pt::ptree json;
		public:
		//Reads istream and saves it to json
		void readJson(std::istream&);
		//Gets appilication ID for verification
		std::string getAppId();
		//Gets types of request ()
		std::string getType();
		//Gets name of the intent
		std::string getIntent();
	};

	//Holds response created by user
	class Response {
		//Holds json formatted session attributes
		pt::ptree mSessionAttributes;
		//Holds json formatted speech object
		pt::ptree mSpeech;
		//Holds json formated card object
		pt::ptree mCard;
		//Holds json formatted reprompt object
		pt::ptree mReprompt;
		//Wheither session should end after request
		bool mShouldEndSession;

		public:
		//Constructor
		Response();
		//
		std::string say(std::string);
		//
		std::string getResponse();
		//Adds attribute to mSessionAttributes
		void addAttribute(std::string, std::string);
		//Removes attribute from mSessionAttributes
		void removeAttribute(std::string);
		//Creates PlainText speech object
		//
		void speech(std::string);
		//Creates SSML speech object
		void ssml(std::string);
		//Creates Simple card object
		void simpleCard(std::string, std::string);
		//Creates Standard card object using
		void standardCard(std::string, std::string, std::string);
		
		void standardCard(std::string, std::string, std::string, std::string);
		
		void reprompt(std::string);
		
		void ssmlReprompt(std::string);
		
		void shouldEndSession(bool);
	};

	class AlexaSkill {
		std::unique_ptr<HttpsServer> server;
		std::string applicationId, path, certificate, privateKey;

		std::shared_ptr<Request> alexaRequest;
		std::shared_ptr<Response> alexaResponse;

		public:
		//Constructor
		AlexaSkill(std::string, std::string, std::string, std::string);
		//Where intent names are mapped to functions
		std::map<std::string, std::function<void(std::shared_ptr<Request>, std::shared_ptr<Response>)>> intents;
		//Setup server and functions to be called when request is recieved
		void setupServer();
		//Start server
		void start();
	};
}

#endif
