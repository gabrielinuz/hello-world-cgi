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

#include <iostream>
#include <unordered_map>
#include <string>

#include "include/ComponentFactory.h"
#include "include/IComponent.h"
#include "include/IRequestAnalyzer.h"

using namespace std;


int main()
{
    cout << "Content-type:text/html\r\n\r\n";
    cout << "<!DOCTYPE html>\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Hello World - Second CGI Program</title>\n";
    cout << "<meta charset='utf-8'>\n";
    cout << "</head>\n";
    cout << "<body>\n";

    cout << "<form action='helloworld.exe' method='POST'>";
    cout << "<label>Introduzca su nombre:</label>\n";
    cout << "<input type='text' name='txt1' id='txt1'>\n";
    cout << "<input type='submit' name='sbmt1' value='enviar'>\n";
    cout << "</form>\n";

    unordered_map<string, string> requestMap;

    /*COMPONENT SECTION*/
    IComponent* request_analyzer_component = ComponentFactory::createFrom("./lib/libhttprequestanalyzer");
    if(request_analyzer_component->implements("IRequestAnalyzer"))
    {
        /*COMPONENT INSTANCE*/
        IRequestAnalyzer* request_analyzer = (IRequestAnalyzer*) request_analyzer_component->getInstance();

        /*COMPOSITION OF COMPONENT*/
        IComponent* exploder_component = ComponentFactory::createFrom("./lib/libstringexploder");
        request_analyzer->setExploder(exploder_component);

        IComponent* decoder_component = ComponentFactory::createFrom("./lib/liburldecoder");
        request_analyzer->setDecoder(decoder_component);
        /*COMPOSITION OF COMPONENT*/

        /*USE COMPONENT*/
        requestMap = request_analyzer->getRequestMap();
    }
    else
    {
        cout << "Error: The component doesn't implement the IRequestAnalyzer interface!" << endl;
        exit(-1);
    }
    request_analyzer_component->release();
    /*COMPONENT SECTION*/

    cout << "<h2>Hello! " << requestMap["txt1"] << " This is my Refactoring 03 CGI program</h2>\n";

    cout << "</body>\n";
    cout << "</html>\n";
    return 0;
}
