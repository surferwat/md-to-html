/*
HtmlLine is a class that parses a string replacing markdown syntax with its 
corresponding html syntax (e.g., **strong** => <strong>strong</strong>). 

LineType getLineType()
    post: returns the html block element type for the line (e.g., LineType::P, 
    if a paragraph)

string getValue()
    post: returns the parsed line

int getDepth()
    post: returns the depth (i.e., number of consecutive white spaces at the 
    start of the line), which is relevant for identifying nested lists

char hToN(const LineType& lineType) const
    post: returns the heading number
*/

#ifndef HTMLLINE_H
#define HTMLLINE_H
#include <string>
#include <regex>
#include "exception.h"
using namespace std;
using namespace surferwat_exception;

namespace surferwat_htmlline {
    class HtmlLine {
        public:
            class invalidArgError: public Exception {
                public:
                    invalidArgError() {
                        setMsg("");
                    }
                    invalidArgError(const string& newMsg) {
                        setMsg(newMsg);
                    }
            };
            enum LineType { NewLine, P, H1, H2, H3, H4, H5, H6, Li, Hr, CodeBlockFence };
            HtmlLine(string newLine);
            LineType getLineType();
            string getValue();
            int getDepth();
            char hToN(const LineType& lineType) const throw (invalidArgError);
        private:
            LineType lineType;
            string value;
            int depth;
            static regex strongRegex;
            static regex emphasisRegex;
            static regex linkRegex;
            static regex headingInRegex;
            static regex headingRegex;
            static regex listRegex;
            static regex horizontalRegex;
            static regex codeBlockFenceRegex;
            LineType nToH(int n) throw (invalidArgError);
            void convertStrong(string& htmlLine);
            void convertEmphasis(string& htmlLine);
            void convertLink(string& htmlLine);
            void convertBreak(string& htmlLine);
            void convertHeadingIn(string& htmlLine);
            void convertHeading(string& htmlLine, bool& isBlockElement);
            void convertList(string& htmlLine, bool& isBlockElement);
            void convertHorizontal(string& htmlLine, bool& isBlockElement);
            void convertCodeBlockFence(string& htmlLine, bool& isBlockElement);
            void translateInline(string& htmlLine);
            void translateBlock(string& htmlLine, bool& isBlockElement);
            void initialize(string& htmlLine);
    };
}

#endif