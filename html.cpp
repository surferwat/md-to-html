/*
Html has three private data members. The private data member vector<HtmlLine> lines
is the lines inline formatted in HTML. The private data member string value is the 
generated html. The private data member int codeBlockFenceCount is a number that 
represents the count of code block fences that have been parsed.

Invariants:
- int codeBlockFenceCount is a nonnegative number.
*/

#include "html.h"
#include <iostream>
#include <vector>
using namespace std;
using namespace surferwat_html;

namespace surferwat_html {
    Html::Html(const vector<HtmlLine>& newLines) {
        lines = newLines;
        value = "";
        codeBlockFenceCount = 0;
    }





    string Html::getValue() {
        return value;
    }





    string Html::generate() {
        for (int i = 0; i < lines.size(); ++i) {
            switch(lines[i].getLineType()) {
                case HtmlLine::LineType::P:
                    if ((i > 0 && lines[i-1].getLineType() == HtmlLine::LineType::Li) && (lines[i+1].getLineType() == HtmlLine::LineType::Li)) {
                        value += lines[i].getValue();
                        continue;
                    }

                    if (i > 0 && codeBlockFenceCount % 2 != 0) {
                        value += lines[i].getValue();
                        continue;
                    }

                    if ((i > 0 && lines[i-1].getLineType() != HtmlLine::LineType::P) || i == 0) {
                        value += "<p>";
                    }

                    value += lines[i].getValue();

                    if ((i > 0 && lines[i+1].getLineType() != HtmlLine::LineType::P) || i == lines.size() - 1) {
                        value += "</p>";
                    }
                    break;
                case HtmlLine::LineType::H1:
                case HtmlLine::LineType::H2:
                case HtmlLine::LineType::H3:
                case HtmlLine::LineType::H4:
                case HtmlLine::LineType::H5:
                case HtmlLine::LineType::H6:
                    {
                        char number = lines[i].hToN(lines[i].getLineType());
                        value += string("<h") + number + ">" + lines[i].getValue() + "</h" + number + ">";
                    }
                    break;
                case HtmlLine::LineType::Li:
                    if ((i > 0 && (lines[i-1].getLineType() != HtmlLine::LineType::Li)) || i == 0) {
                        value += "<ul>";
                    }

                    if (i > 0 && lines[i].getDepth() > lines[i-1].getDepth()) {
                        value+= "<ul>";
                    }

                    value += "<li>";
                    value += lines[i].getValue();
                    value += "</li>";
                    
                    if ((i < lines.size() - 1 && lines[i].getDepth() > lines[i + 1].getDepth())) {
                        int temp = lines[i].getDepth() - lines[i+1].getDepth();
                        while(temp > 0) {
                            value += "</ul>";
                            temp -= 1;
                        }
                    }

                    if (i == lines.size() - 1) {
                        int temp = lines[i].getDepth();
                        while (temp > 0) {
                            value += "</ul>";
                            temp -= 1;
                        }
                    }

                    if (i < lines.size() - 1 && lines[i+1].getLineType() != HtmlLine::LineType::Li || i == lines.size() - 1) {
                        value += "</ul>";
                    }
                    break;
                case HtmlLine::LineType::Hr:
                    value += "<hr>";
                    break;
                case HtmlLine::LineType::CodeBlockFence:
                    if (codeBlockFenceCount % 2 == 0) {
                        value += "<pre>";
                        value += "<code>";
                    } else {
                        value += "</code>";
                        value += "</pre>";
                    }
                    codeBlockFenceCount += 1;
                    break;
                default:
                    if (i > 0 && lines[i-1].getLineType() == HtmlLine::LineType::P && (i < lines.size()-1 && lines[i+1].getLineType() == HtmlLine::LineType::P)) {
                        value += lines[i].getValue();
                    }
            }
        }
        return value;
    }
}