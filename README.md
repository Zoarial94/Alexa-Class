# Alexa-Class

A simple C++ class which can handle Alexa requests and responses.

This class is lightly tested and the main purpose is to get more familiar with cpp.

## Prerequisite

* Port forwarding must be used to have incomming connetions from 443 point to 8080 (Or other configured port) on the server
* [Amazon Developer account](https://developer.amazon.com/alexa)
* A [skill](https://developer.amazon.com/alexa/console/ask) with an endpoint pointing to the server
  * It is possible to use [noip](noip.com) to create a URL for the skill to use

### Dependencies

* [eidheim/Simple-Web-Server](https://github.com/eidheim/Simple-Web-Server) (Placed in `~/`)
* Boost - Used for Asio and property_tree
  * Standalone Asio works too
* OpenSSL

> With my setup, OpenSSL 1.1.0, boost 1.62.0 and g++-6 are used

### Usage 

See test/example.cpp for basic usage

### Build and run example

#### Initialize Class

```cpp
Alexa::AlexaSkill skill(ApplicationID, path, certificate, privateKey, port);
```
The above is how one would first initialize the class.

* Before alexa can connect to the server, the [Applicaion ID](https://developer.amazon.com/docs/custom-skills/handle-requests-sent-by-alexa.html?#get-the-application-id-for-a-skill) must be retrieved to filter unwanted requests.
This will be put in place of `ApplicationID`.

* `path` is the path in the endpoint which is called. For example if the endpoint was `alexa.skill.com` and alexa is calling `alexa.skill.com/testingSkill` `path` would then be `testingSkill`.
> Note: `testingSkill` work and `/testingSkill` does not. If there is no path leave an empty string.

* To use a custom endpoint for a skill a self signed certificate must be uploaded for security purposes. `certificate` is the path to this certificate. More information on this can be found [here](https://developer.amazon.com/docs/custom-skills/configure-web-service-self-signed-certificate.html).

* `privateKey`is the path to the key created in pair with the certificate above.

* `port` is optional. Default it 8080.

#### Configure Server

> TODO: Have `setupServer()` run when calling `start()`

```cpp
skill.setupServer();
```
This function configures the server and prepares it to start

#### Add Intent Handlers

```cpp
skill.intents["TestTalking"] = [](std::shared_ptr<Alexa::Request> request, std::shared_ptr<Alexa::Response> response) {
    std::cout << "IN TEST TALKING CALLBACK FUNCTION" << std::endl;
    response->speech("Talking has been tested");
    response->simpleCard("Testing", "Talking has been tested");
};
```

---

```cpp
skill.intents["TestTalking"]
```
* This will create an item in a map/dictionary


```cpp
[](std::shared_ptr<Alexa::Request> request, std::shared_ptr<Alexa::Response> response)
```
* This creates a lambda function with `request` and `response` as arguments


```cpp
response->speech("Talking has been tested");
```
* This takes the `response` argument and calls the function `speech()` More information on these function will be provided below


```cpp 
response->simpleCard("Testing", "Talking has been tested");
```
* This takes the `response` argument and calls the function `simpleCard()`


#### Start the Server

```cpp
skill.start();
```
This simply starts the server by joining the server to a thread.

### The Request Class

This class is for getting infomation from the request

---

```cpp
std::string getAppId();
```
* Returns the application id used for verification

```cpp
std::string getType();
```
* Returns the type of request from amazon (IntentRequest, LaunchRequest, SessionEndedRequest, etc)
> [More info on requests](https://developer.amazon.com/docs/custom-skills/request-and-response-json-reference.html#session-object) (Scroll up slightly)

> [More info on what the request class contains](https://developer.amazon.com/docs/custom-skills/request-and-response-json-reference.html#session-object)

```cpp
std::string getIntent();
```
* Gets the name of the intent **IF** the request is a type `IntentRequest`

### The Response Class

This is what is used to make alexa respond

---

#### Speech

These functions will affect how alexa will respond verbally

---

```cpp
void speech(std::string s);
```
* Alexa will attempt to say what is given
* `std::string s`
  * A string of what alexa will respond with

```cpp
void ssml(std::string s);
```
* Alexa will attpempt to say what is given
* `std::string s`
  * A string of what alexa will respond with formatted in [SSML](https://developer.amazon.com/docs/custom-skills/speech-synthesis-markup-language-ssml-reference.html) 

#### Card

These functions affect the card that can show in the alexa app

---

```cpp
void simpleCard(std::string title, std::string content);
```
* Creates a simple card on the dashboard of the alexa app
* `std::string title`
  * The title of the card
* `std::string content`
  * The text which will show in the card

```cpp
void standardCard(std::string title, std::string text, std::string image);
```
* Creates a standard card on the dashboard of the alexa app
* `std::string title`
  * Title of the card
* `std::string text`
  * The text displayed in the card
* `std::string image`
  * The link to an image to be diplayed on the card
  
```cpp
void standardCard(std::string title, std::string text, std::string small, std::string large);
```
* Creates a standard card on the dashboard of the alexa app
* `std::string title`
  * Title of the card
* `std::string text`
  * The text displayed in the card
* `std::string small`
  * The link to a small image to be diplayed on the card
* `std::string large`
  * The link to a large image to be diplayed on the card
  
> [More info on cards](https://developer.amazon.com/docs/custom-skills/include-a-card-in-your-skills-response.html#create-a-home-card-to-display-text-and-an-image)

> [Info on image sizes](https://developer.amazon.com/docs/custom-skills/include-a-card-in-your-skills-response.html#image_size)

### Reprompting

Alexa reprompting for a answer

---

```cpp
void reprompt(std::string s);
```
* Makes a reprompt saying when alexa does not understand the user's speech
* `std::string s`
  * A string of what alexa will reprompt with
  
```cpp
void reprompt(std::string s);
```
* Makes a reprompt saying when alexa does not understand the user's speech
* `std::string s`
  * A string of what alexa will reprompt with formatted in [SSML](https://developer.amazon.com/docs/custom-skills/speech-synthesis-markup-language-ssml-reference.html) 
  
### Session

```cpp
void shouldEndSession(bool b);
```
* `bool b`
  * Whether to not this response should end the session
  
> [More info on what the response classes affects](https://developer.amazon.com/docs/custom-skills/request-and-response-json-reference.html#response-object)
