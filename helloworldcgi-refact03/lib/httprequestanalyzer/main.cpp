/**
 *  Copyright 2015 Gabriel Nicolás González Ferreira <gabrielinuz@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**/

#include "../../include/IComponent.h"
#include "../../include/IRequestAnalyzer.h"
#include "../../include/IExploder.h"
#include "../../include/IDecoder.h"
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class HttpRequestAnalyzer : public IRequestAnalyzer, public IComponent
{
    public:
        HttpRequestAnalyzer();
        virtual ~HttpRequestAnalyzer();

        void setExploder(IComponent* exploder_component);
        void setDecoder(IComponent* decoder_component);

        //IRequestAnalyzer
        unordered_map<string, string> getRequestMap();

        //IComponent
        bool implements(string interfaceName);
        void* getInstance();
        void release();

    private:
        IComponent* m_exploder_component;
        IExploder* m_exploder;
        IComponent* m_decoder_component;
        IDecoder* m_decoder;

        string getRequestString();


        //IComponent
        int m_referenceCounter;
        bool m_implemented;
};

HttpRequestAnalyzer::HttpRequestAnalyzer() : m_referenceCounter(0) {}

HttpRequestAnalyzer::~HttpRequestAnalyzer()
{
    m_exploder_component->release();
    m_decoder_component->release();
}

void HttpRequestAnalyzer::setExploder(IComponent* exploder_component)
{
    m_exploder_component = exploder_component;

    if(m_exploder_component->implements("IExploder"))
    {
        IExploder* exploder = (IExploder*) m_exploder_component->getInstance();
        m_exploder = exploder;
    }
    else
    {
        cout << "Error: The component doesn't implement the IExploder interface!" << endl;
        exit(-1);
    }
}

void HttpRequestAnalyzer::setDecoder(IComponent* decoder_component)
{
    m_decoder_component = decoder_component;

    if(m_decoder_component->implements("IDecoder"))
    {
        IDecoder* decoder = (IDecoder*) m_decoder_component->getInstance();
        m_decoder = decoder;
    }
    else
    {
        cout << "Error: The component doesn't implement the IDecoder interface!" << endl;
        exit(-1);
    }
}

string HttpRequestAnalyzer::getRequestString()
{
    string requestString = "";

    if(getenv("REQUEST_METHOD") == NULL)
    {
        cout << "The request method is null" << endl;
        exit(-1);
    }
    else
    {
        string request_method = string(getenv("REQUEST_METHOD"));

        if(request_method == "GET") { requestString = string(getenv("QUERY_STRING")); }

        if(request_method == "POST") { cin >> requestString; }
    }

    return requestString;
}

unordered_map<string, string> HttpRequestAnalyzer::getRequestMap()
{
    string requestString = getRequestString();
    vector<string> keysWithValues = m_exploder->explode( m_decoder->decode(requestString), '&' );
    vector<string> keysAndValues;
    vector<string>::iterator i;

    int aux = 0;
    unordered_map<string, string> requestMap;

    for(i = keysWithValues.begin(); i != keysWithValues.end(); i++)
    {
        keysAndValues = m_exploder->explode( keysWithValues[aux], '=' );
        requestMap[keysAndValues[0]] = keysAndValues[1];
        aux++;
    }

    return requestMap;
}

//IComponent
bool HttpRequestAnalyzer::implements(string interfaceName)
{
    return (interfaceName == "IComponent" || interfaceName == "IRequestAnalyzer") ?
        m_implemented = true
            : m_implemented = false;
}

void* HttpRequestAnalyzer::getInstance()
{
    if(m_implemented) {  m_referenceCounter++;  return this; }
    return NULL;
}

void HttpRequestAnalyzer::release()
{
    m_referenceCounter--;
    if(m_referenceCounter <= 0) delete this;
}

extern "C" IComponent* create();

IComponent* create()
{
    return (IComponent*) new HttpRequestAnalyzer;
}
