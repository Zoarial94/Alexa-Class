#define BOOST_SPIRIT_THREADSAFE
#include "AlexaSkill.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

Alexa::Response::Response() : mShouldEndSession(true) { }

std::string Alexa::Response::say(std::string s) {
	speech(s);
	return getResponse();
}
void Alexa::Response::speech(std::string s) {
	mSpeech.add("type", "PlainText");
	mSpeech.add("text", s);
}
void Alexa::Response::ssml(std::string s) {
	mSpeech.add("type", "SSML");
	mSpeech.add("ssml", s);
}
void Alexa::Response::simpleCard(std::string title, std::string content) {
	mCard.add("type", "Simple");
	mCard.add("title", title);
	mCard.add("content", content);
}
void Alexa::Response::standardCard(std::string title, std::string text, std::string small, std::string large) {
	mCard.add("type", "Standard");
	mCard.add("title", title);
	mCard.add("text", text);
	mCard.put("image.smallImageUrl", small);
	mCard.put("image.largeImageUrl", large);
}
void Alexa::Response::standardCard(std::string title, std::string text, std::string image) {
	mCard.add("type", "Standard");
	mCard.add("title", title);
	mCard.add("text", text);
	mCard.put("image.smallImageUrl", image);
	mCard.put("image.largeImageUrl", image);
}
void Alexa::Response::reprompt(std::string s) {
	mReprompt.add("type", "PlainText");
	mReprompt.add("text", s);
}
void Alexa::Response::ssmlReprompt(std::string s) {
	mReprompt.add("type", "SSML");
	mReprompt.add("ssml", s);
}
void Alexa::Response::addAttribute(std::string key, std::string value) {
	mSessionAttributes.add(key, value);
}
void Alexa::Response::removeAttribute(std::string key) {
	mSessionAttributes.erase(key);
}
void Alexa::Response::shouldEndSession(bool b) {
	mShouldEndSession = b;
}

std::string Alexa::Response::getResponse() {
	std::stringstream ss;
	pt::ptree json;
	json.add("version", "1.0");
	if(!mSessionAttributes.empty()) {
		json.add_child("sessionAttributes", mSessionAttributes);
	}
	if(!mSpeech.empty()) {
		json.add_child("response.outputSpeech", mSpeech);
	}
	if(!mCard.empty()) {
		json.add_child("response.card", mCard);
	}
	if(!mReprompt.empty()) {
		json.add_child("response.reprompt.outputSpeech", mReprompt);
	}
	json.add("response.shouldSessionEnd", mShouldEndSession);
	ss << "HTTP/1.1 200 OK\r\nContent-Type: application/json;charset=UTF-8\r\nContent-Length: ";
	std::stringstream ssjson;
	pt::json_parser::write_json(ssjson, json);
	ss << ssjson.str().length() << "\r\n\r\n" << ssjson.str();
	//std::cout << "==========RESPONSE==========\n" << ss.str(); 
	return ss.str();
}

