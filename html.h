/*
Html is a class that generates an HTML file from a given number of 
lines inline formatted in HTML.

string getValue();
    post: returns the generated html

string generate();
    post: adds HTML block element tags to each line; returns the 
    generated html
*/

#ifndef HTML_H
#define HTML_H
#include "htmlline.h"
#include <vector>
using namespace std;
using namespace surferwat_htmlline;

namespace surferwat_html {
    class Html {
        public:
            Html(const vector<HtmlLine>& newLines);
            string getValue();
            string generate();
        private:
            vector<HtmlLine> lines;
            string value;
            int codeBlockFenceCount;
    };
}

#endif