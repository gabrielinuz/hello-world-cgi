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
#include "../../include/IExploder.h"
#include <iostream>

#include <vector>
#include <string>
#include <sstream>//FOR EXPLODE

using namespace std;

class StringExploder : public IExploder, public IComponent
{
    public:
        StringExploder();
        virtual ~StringExploder();
        vector<string> explode(string const &input, char delimiter);

        //IComponent
        bool implements(string interfaceName);
        void* getInstance();
        void release();

    private:
        //IComponent
        int m_referenceCounter;
        bool m_implemented;
};

StringExploder::StringExploder() : m_referenceCounter(0) {}

StringExploder::~StringExploder(){}

vector<string> StringExploder::explode(string const &input, char delimiter)
{
    vector<string> result;
    istringstream iss(input);

    for (string token; getline(iss, token, delimiter); )
    {
        result.push_back(move(token));
    }

    return result;
}

//IComponent
bool StringExploder::implements(string interfaceName)
{
    return (interfaceName == "IComponent" || interfaceName == "IExploder") ?
        m_implemented = true
            : m_implemented = false;
}

void* StringExploder::getInstance()
{
    if(m_implemented) {  m_referenceCounter++;  return this; }
    return NULL;
}

void StringExploder::release()
{
    m_referenceCounter--;
    if(m_referenceCounter <= 0) delete this;
}

extern "C" IComponent* create();

IComponent* create()
{
    return (IComponent*) new StringExploder;
}
