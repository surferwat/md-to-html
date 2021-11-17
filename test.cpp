#include <iostream>
#include <string>
#include <vector>
#include "htmlline.h"
#include "html.h"
using namespace std;
using namespace surferwat_htmlline;
using namespace surferwat_html;

void test(string expect, string input);
vector<string> split(string str, string delimiter);

int main() {
    cout << "\n----- paragrah -----\n";
	test("<p>a paragraph</p>", "a paragraph");
	test("<p>a paragraph<br>hogehoge</p>", "a paragraph  \nhogehoge");

	cout << "\n----- heading -----\n";
	test("<h1>h1</h1>", "# h1");
	test("<h2>h2</h2>", "## h2");
	test("<h3>h3</h3>", "### h3");
	test("<h4>h4</h4>", "#### h4");
	test("<h5>h5</h5>", "##### h5");
	test("<h6>h6</h6>", "###### h6");
	test("<p>####### h7</p>", "####### h7");
	test("<p>###dummyh3</p>", "###dummyh3");
	test("<p>C## is not heading</p>", "C## is not heading");

	cout << "\n----- list -----\n";
	test("<ul><li>list1</li></ul>", "- list1");
	test("<ul><li>list1</li><li>list2</li></ul>", "- list1\n- list2");
	// TODO: Sublist is not a standard syntax.
	// It should be <li>list1<ul><li>sublist1</li></ul></li></ul>
	// but now got <li>list1</li><ul><li>sublist1</li></ul></ul>
	test("<ul><li>list1</li><ul><li>sublist1</li></ul></ul>", "- list1\n  - sublist1");
	test("<ul><li>list1</li><ul><li>sublist1</li><ul><li>subsublist1</li></ul></ul></ul>", "- list1\n  - sublist1\n    - subsublist1");
	test("<ul><li>list1</li><ul><li>sublist1</li></ul><li>list2</li></ul>", "- list1\n  - sublist1\n- list2");
	test("<ul><li>a</li><ul><li>aa</li><ul><li>aaa</li></ul></ul><li>b</li></ul>", "- a\n  - aa\n    - aaa\n- b");
	test("<ul><li>a</li><ul><li>aa</li><ul><li>aaa</li></ul><li>bb</li></ul></ul>", "- a\n  - aa\n    - aaa\n  - bb");
	test("<ul><li><h1>h1</h1></li></ul>", "- # h1");
	//test("<ul><li>a -b</li><li>c</li></ul>", "- a\n  -b\n- c")

	cout << "\n----- link -----\n";
	test("<p><a href=\"http://example.com\">link</a></p>", "[link](http://example.com)");
	test("<p><a href=\"http://example.com\">link(2)</a></p>", "[link(2)](http://example.com)");
	test("<p>inline text<a href=\"http://example.com\">link</a>.</p>", "inline text[link](http://example.com).");
	test("<p>[dummylink] (http://example.com)</p>", "[dummylink] (http://example.com)");

	cout << "\n----- heading with inline elements -----\n";
	test("<h1><a href=\"http://example.com\">link</a></h1>", "# [link](http://example.com)");
	test("<h1>- dummylist</h1>", "# - dummylist");

	cout << "\n----- list with inline elements -----\n";
	test("<ul><li><a href=\"http://example.com\">link</a></li></ul>", "- [link](http://example.com)");
	test("<ul><li>This is <a href=\"http://example.com\">link</a> list.</li></ul>", "- This is [link](http://example.com) list.");
	test("<ul><li><h1>h1</h1></li></ul>", "- # h1");

	cout << "\n----- heading after a list -----\n";
	test("<ul><li>list1</li></ul><h1>h1</h1>", "- list1\n# h1");
	test("<ul><li>list1</li></ul><h1>h1</h1>", "- list1\n\n# h1");
	test("<ul><li>a</li><ul><li>b</li></ul></ul><h1>h1</h1>", "- a\n  - b\n# h1");

	cout << "\n----- multiple lines -----\n";
	test("<h1>h1</h1><p>text</p>", "# h1\ntext");

	cout << "\n----- emphasis -----\n";
	test("<p><em>emphasis</em></p>", "*emphasis*");
	test("<p><em>emphasis</em></p>", "_emphasis_");
	test("<p><strong>strong</strong></p>", "**strong**");
	test("<p><strong>strong</strong></p>", "__strong__");

	cout << "\n----- codeblock -----\n";
	test("<pre><code>codeblock</code></pre>", "```\ncodeblock\n```");

	cout << "\n----- break -----\n";
	test("<p>break<br></p>", "break  ");

	cout << "\nOK\n";    
}





void test(string expect, string input) {
    vector<string> lines = split(input, "\n");
    vector<HtmlLine> htmlLines;

    for (auto line : lines) {
        HtmlLine htmlLine(line);
        htmlLines.push_back(htmlLine);
    }

    Html html(htmlLines);
    html.generate();
	
    if (html.getValue() == expect) {
		for (auto line : lines) cout << line;
        cout << " => " << html.getValue() << endl;
    } else {
		for (auto line : lines) cout << line;
        cout << " => " << expect << " but got " << html.getValue() << endl;
        exit(0);
    }
}





vector<string> split(string str, string delimiter = " ") {
    vector<string> tokens;
    int start = 0;
    int end = str.find(delimiter);

    while (end != -1) { 
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start, end - start));
    
    return tokens;
}