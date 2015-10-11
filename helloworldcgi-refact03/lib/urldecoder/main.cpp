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
#include "../../include/IDecoder.h"
#include <iostream>

#include <string>

using namespace std;

class UrlDecoder : public IDecoder, public IComponent
{
    public:
        UrlDecoder();
        virtual ~UrlDecoder();
        string decode(string &input);

        //IComponent
        bool implements(string interfaceName);
        void* getInstance();
        void release();

    private:
        //IComponent
        int m_referenceCounter;
        bool m_implemented;
};

UrlDecoder::UrlDecoder() : m_referenceCounter(0) {}

UrlDecoder::~UrlDecoder(){}

//Thanks Tominko...
string UrlDecoder::decode(string &input)
{
    string ret;
    char ch;
    int i, ii;
    for (i=0; i<input.length(); i++)
    {
        if (int(input[i])==37)
        {
            sscanf(input.substr(i+1,2).c_str(), "%x", &ii);
            ch=static_cast<char>(ii);
            ret+=ch;
            i=i+2;
        } else
        {
            ret+=input[i];
        }
    }
    return (ret);
}

//IComponent
bool UrlDecoder::implements(string interfaceName)
{
    return (interfaceName == "IComponent" || interfaceName == "IDecoder") ?
        m_implemented = true
            : m_implemented = false;
}

void* UrlDecoder::getInstance()
{
    if(m_implemented) {  m_referenceCounter++;  return this; }
    return NULL;
}

void UrlDecoder::release()
{
    m_referenceCounter--;
    if(m_referenceCounter <= 0) delete this;
}

extern "C" IComponent* create();

IComponent* create()
{
    return (IComponent*) new UrlDecoder;
}
