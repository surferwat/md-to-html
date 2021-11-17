/*
HtmlLine has three private data members. The private data member LineType lineType 
is the HTML block element type for the line. The private data member string value 
is the parsed line with inline formatted with HTML. The private data member int depth 
is the consecutive number of white spaces meant to represent the level of a nested 
list.

Invariants:
- int depth is a nonnegative number.
*/

#include "htmlline.h"
#include <iostream>
#include <string>
#include <regex>
using namespace std;

namespace surferwat_htmlline {
    HtmlLine::HtmlLine(string newLine) {
        initialize(newLine);
    }





    HtmlLine::LineType HtmlLine::getLineType() {
        return lineType;
    }





    string HtmlLine::getValue() {
        return value;
    }





    int HtmlLine::getDepth() {
        return depth;
    }




    
    char HtmlLine::hToN(const HtmlLine::LineType& lineType) const throw (invalidArgError) {
        switch(lineType) {
            case HtmlLine::LineType::H1:
                return '1';
                break;
            case HtmlLine::LineType::H2:
                return '2';
                break;
            case HtmlLine::LineType::H3:
                return '3';
                break;
            case HtmlLine::LineType::H4:
                return '4';
                break;
            case HtmlLine::LineType::H5:
                return '5';
                break;
            case HtmlLine::LineType::H6:
                return '6';
                break;
            default:
                throw invalidArgError("a heading should be in the range of 1 to 6, but got " + to_string(lineType));
        }
    }





    regex HtmlLine::strongRegex = regex("(\\*\\*.+?\\*\\*)|(\\_\\_.+?\\_\\_)");





    regex HtmlLine::emphasisRegex = regex("(\\*.+?\\*)|(\\_.+?\\_)");





    regex HtmlLine::linkRegex = regex("(\\[.+?\\])(\\(.+?\\))");





    regex HtmlLine::headingInRegex = regex("^ *- +(#{1,6}) (.+)");





    regex HtmlLine::headingRegex = regex("(^#{1,6}) (.+)");





    regex HtmlLine::listRegex = regex("^( *)- (.+)");





    regex HtmlLine::horizontalRegex = regex("^-{3}|_{3}|\\*{3}");





    regex HtmlLine::codeBlockFenceRegex = regex("^`{3}|~{3}");





    HtmlLine::LineType HtmlLine::nToH(int n) throw (invalidArgError) {
        switch(n) {
            case 1:
                return HtmlLine::LineType::H1;
                break;
            case 2: 
                return HtmlLine::LineType::H2;
                break;
            case 3:
                return HtmlLine::LineType::H3;
                break;
            case 4:
                return HtmlLine::LineType::H4;
                break;
            case 5: 
                return HtmlLine::LineType::H5;
                break;
            case 6:
                return HtmlLine::LineType::H6;
                break;
            default:
                throw invalidArgError("a heading should be in the range of 1 to 6, but got " + to_string(n));
        }
    }





    void HtmlLine::convertStrong(string& htmlLine) {
        string temp = htmlLine;
        smatch sm;
        // **<text>**
        // __<text>__
        while(regex_search(temp, sm, strongRegex)) {
            if (!sm.empty()) {
                int start = sm.prefix().length();
                int end = start + sm[0].length();
                string strongTag = "<strong>" + htmlLine.substr(start+2, end-start-4) + "</strong>";
                htmlLine = htmlLine.substr(0, start) + strongTag + htmlLine.substr(end, string::npos);
                temp = htmlLine;
            }
        }
    }





    void HtmlLine::convertEmphasis(string& htmlLine) {
        string temp = htmlLine;
        smatch sm;
        // *<text>*
        // _<text>_
        while(regex_search(temp, sm, emphasisRegex)) {
            if (!sm.empty()) {
                int start = sm.prefix().length();
                int end = start + sm[0].length();
                string emphasisTag = "<em>" + htmlLine.substr(start+1, end-start-2) + "</em>";
                htmlLine = htmlLine.substr(0, start) + emphasisTag + htmlLine.substr(end, string::npos);
                temp = htmlLine;
            }
        }
    }





    void HtmlLine::convertLink(string& htmlLine) {
        string temp = htmlLine;
        smatch sm;
        // [<text>](<url>)
        while(regex_search(temp, sm, linkRegex)) {
            if (!sm.empty()) {
                string text = sm[1];
                string url = sm[2];
                string liTag = "<a href=\"" + url.substr(1, url.length() - 2) + "\">" + text.substr(1, text.length() - 2) + "</a>";
                int start = sm.prefix().length();
                int end = start + sm[0].length();
                htmlLine = htmlLine.substr(0, start) + liTag + htmlLine.substr(end, string::npos);
                temp = htmlLine;
            }
        }
    }





    void HtmlLine::convertBreak(string& htmlLine) {
        if (htmlLine.size() > 2 && htmlLine.substr(htmlLine.size() - 2, 2) == "  ") {
            htmlLine = htmlLine.substr(0, htmlLine.size() - 2) + "<br>";
        }
    }





    void HtmlLine::convertHeadingIn(string& htmlLine) {
        smatch sm;
        // #, ##, ..., or ######
        // title
        regex_search(htmlLine, sm, headingInRegex);
        if (!sm.empty()) {
            string n = to_string(sm[1].length()); // heading number
            string title = sm[2];
            string headingInTag = "<h" + n + ">" + title + "</h" + n + ">";
            int start = htmlLine.length() - 1 - sm[1].length() - sm[2].length(); // hack as gcc does not support sm[1].first
            htmlLine = htmlLine.substr(0,start) + headingInTag;
        }
    }





    void HtmlLine::convertHeading(string& htmlLine, bool& isBlockElement) {
        if (!isBlockElement) {
            smatch sm;
            // #, ##, ..., or ######
            // title
            regex_search(htmlLine, sm, headingRegex);
            if (!sm.empty()) {
                int n = sm[1].length();
                string title = sm[2];
                htmlLine = title;

                lineType = nToH(n);
                value = htmlLine;
                depth = 0;

                isBlockElement = true;
            }
        }
    }





    void HtmlLine::convertList(string& htmlLine, bool& isBlockElement) {
        if (!isBlockElement) {
            smatch sm;
            // white spaces before "-"
            regex_search(htmlLine, sm, listRegex);
            if (!sm.empty()) {
                int whiteSpaces = sm[1].length();
                string item = sm[2];
                htmlLine = item;
                
                lineType = HtmlLine::LineType::Li;
                value = htmlLine;
                depth = whiteSpaces / 2; // two white spaces is equivalent to one nested level

                isBlockElement = true;
            }
        }
    }





    void HtmlLine::convertHorizontal(string& htmlLine, bool& isBlockElement) {
        if (!isBlockElement) {
            smatch sm;
            // ---, ___, or ***
            regex_search(htmlLine, sm, horizontalRegex);
            if (!sm.empty()) {
                lineType = HtmlLine::LineType::Hr;
                value = htmlLine;
                depth = 0;
                
                isBlockElement = true;
            }
        }
    }





    void HtmlLine::convertCodeBlockFence(string& htmlLine, bool& isBlockElement) {
        if (!isBlockElement) {
            smatch sm;
            // ```
            regex_search(htmlLine, sm, codeBlockFenceRegex);
            if (!sm.empty()) {
                lineType = HtmlLine::LineType::CodeBlockFence;
                value = htmlLine;
                depth = 0;

                isBlockElement = true;
            }
        }
    }





    void HtmlLine::translateInline(string& htmlLine) {
        convertStrong(htmlLine);
        convertEmphasis(htmlLine);
        convertLink(htmlLine);
        convertBreak(htmlLine);
        convertHeadingIn(htmlLine);
    }





    void HtmlLine::translateBlock(string& htmlLine, bool& isBlockElement) {
        convertHeading(htmlLine, isBlockElement);
        convertList(htmlLine, isBlockElement);
        convertHorizontal(htmlLine, isBlockElement);
        convertCodeBlockFence(htmlLine, isBlockElement);
    }





    void HtmlLine::initialize(string& htmlLine) {
        bool isBlockElement = false;

        if (htmlLine == "\n" || htmlLine.length() == 0) {
            lineType = HtmlLine::LineType::NewLine;
            value = ""; // TODO(surferwat): consider using <br> instead of ""
            depth = 0;
        } else {
            translateInline(htmlLine);
            translateBlock(htmlLine, isBlockElement);
            if (!isBlockElement) {
                lineType = HtmlLine::LineType::P;
                value = htmlLine;
                depth = 0;
            }
        }
    }
}