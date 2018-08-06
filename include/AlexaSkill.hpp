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
		pt::ptree mJson;
		public:
		/**
			Reads istream and saves it to mJson
			
			@param raw - buffer from HTTP request
		*/
		void readJson(std::istream& raw);
		/**
			Gets appilication ID for verification
		
			@return - Application ID from mJson
		*/
		std::string getAppId();
		/**
			Gets types of request from mJson (IntentRequest, LaunchRequest, SessionEndedRequest, etc)
		
			@return - The type of request
		*/
		std::string getType();
		/**
			Gets name of the intent if request is IntentRequest
		
			@return - Name of the request
		*/
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
		//Default is true
		bool mShouldEndSession;

		public:
		//Constructor
		//Sets mShouldEndSession to true
		Response();
		/**
			Convenience function for getting a HTTP response with just a verbal response
		
			@param s - What alexa will say
		*/
		std::string say(std::string s);
		/**
			Returns the whole response to be sent. Header and body of the HTTP response.
		
			@return - Returns a json formatted string of all the information provided to be sent back to Alexa
		*/
		std::string getResponse();
		/**
			Adds an attribute to mSessionAttributes
			
			@param key - The key which will be stored in the map
			@param value - The value the key will point to
		*/
		void addAttribute(std::string key, std::string value);
		/**
			Removes attribute from mSessionAttributes
		
			@param - The key to remove from the map
		*/
		void removeAttribute(std::string key);
		/**
			Creates PlainText speech object
			
			@param s - What Alexa will say
		*/
		void speech(std::string s);
		/**
			Creates SSML speech object
		
			@param s - A SSML formatted speech object of what Alexa will say
		*/
		void ssml(std::string s);
		/**
			Creates Simple card object in the Alexa app
			
			@param title - The title of the card
			@param content - The text which is displayed in the card
		*/
		void simpleCard(std::string title, std::string content);
		/**
			Creates Standard card object in the Alexa app
		
			@param title - The title of the card
			@param text - The text displayed in the card
			@param image - A link to an image which is displayed
		*/
		void standardCard(std::string title, std::string text, std::string image);
		/**
			Creates Standard card object in the Alexa app
		
			@param title - The title of the card
			@param text - The text displayed in the card
			@param small - A link to a lower resolution image which is displayed
			@param large - A link to a larger resolution image which is displayed
		*/
		void standardCard(std::string title, std::string text, std::string small, std::string large);
		/**
			What alexa will ask if a response is nessessary

			@param s - The text Alexa will say
		*/
		void reprompt(std::string s);
		/**
			What alexa will ask if a response is nessessary

			@param s - The text Alexa will say formatted in SSML
		*/
		void ssmlReprompt(std::string s);
		//Changes mShouldEndSession
		//@param b - What mShouldEndSession is set to
		void shouldEndSession(bool b);
	};

	class AlexaSkill {
		//A pointer to the server object
		std::unique_ptr<HttpsServer> server;
		/** 
			applicationId - The App Id YOUR Alexa account is linked too
			path - The path the server will use to recive the Alexa requests

			See https://developer.amazon.com/docs/custom-skills/configure-web-service-self-signed-certificate.html to create certificate and private key

			certificate - The path (absolute or relative) to the certificate file for HTTPS verification
			privateKey - The path to the private key file for HTTPS verification
			port - The port to listen on
		*/
		std::string applicationId, path, certificate, privateKey;
		int port;

		//Pointer to Request object
		std::shared_ptr<Request> alexaRequest;
		//Pointer to Response object
		std::shared_ptr<Response> alexaResponse;

		public:
		/**
			Constructor
			Sets alexaRequest and alexaResponse to empty objects

			@param appId - The id used in applicationId
			@param path - The path used in path
			@param cert - The path to the certificate used in certificate
			@param key - The path to the private key used in privateKey
		*/
		AlexaSkill(std::string appId, std::string path, std::string cert, std::string key);
		
		/**
			Constructor
			Sets alexaRequest and alexaResponse to empty objects

			@param appId - The id used in applicationId
			@param path - The path used in path
			@param cert - The path to the certificate used in certificate
			@param key - The path to the private key used in privateKey
			@param port - The port number to listen to
		*/
		AlexaSkill(std::string appId, std::string path, std::string cert, std::string key, int port);
		
		//Where intent names are mapped to functions
		//Functions return nothing and have a Request object and a Response object as the arguments
		std::map<std::string, std::function<void(std::shared_ptr<Request>, std::shared_ptr<Response>)>> intents;
		/**
			Creates server with certificate and privateKey and saves it to server.
			For each item in intents this function will add a resource to the server which will activate a function which is mapped to the intent
		*/
		void setupServer();
		//Start server
		//Creates a thread of the server and joins it
		void start();
	};
}

#endif
