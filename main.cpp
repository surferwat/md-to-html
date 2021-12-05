#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include "html.h"
#include "htmlline.h"
#include "exception.h"
using namespace std;
using namespace surferwat_htmlline;
using namespace surferwat_html;
using namespace surferwat_exception;

const string TARGET_EXT = ".md";

void isMarkdownFile(string fileName) throw (Exception);
void changeExt(string& targetFileName, string sourceFileName, string sourceExt);
void writerFile(string writeFileName, ofstream& writeFile) throw (Exception);
void readerFile(string readFileName, ifstream& readFile) throw (Exception);
void convertLines(ifstream& readFile, vector<HtmlLine>& lines);
void generateHtml(const vector<HtmlLine>& lines, string writeFileName, ofstream& writeFile) throw (Exception);

int main(int argc, char* argv[]) {
    string readFileName; // name of the Markdown file to be converted
    ifstream readFile; // the Markdown file
    string writeFileName; // name of the HTML file
    ofstream writeFile; // the HTML file
    vector<HtmlLine> lines; // lines with inline HTML

    if (argc == 1) {
        cout << "invalid number of args" << endl;
        exit(0);
    }
    readFileName = argv[1];
    
    try {
        isMarkdownFile(readFileName);
        
        assert(readFileName.substr(readFileName.length() - TARGET_EXT.length(), string::npos) == TARGET_EXT);
        changeExt(writeFileName, readFileName, TARGET_EXT);

        writerFile(writeFileName, writeFile);

        readerFile(readFileName, readFile);
    
        convertLines(readFile, lines);

        generateHtml(lines, writeFileName, writeFile);
    } catch (Exception& e) {
        cerr << e.what() << endl;
        exit(0);
    } catch (HtmlLine::invalidArgError& e) {
        cerr << e.what() << endl;
        exit(0);
    } catch (...) {
        cerr << "unexplained exception.\n";
        exit(0);
    }
    
    cout << "done!\n";
    cout << readFileName << " converted to " << writeFileName << endl;
    
    return 0;
}





void isMarkdownFile(string fileName) throw (Exception) {
    string ext = fileName.substr(fileName.length() - TARGET_EXT.length(), string::npos);

    if (ext != TARGET_EXT) {
        throw Exception("invalid file type");
    }
}





void changeExt(string& targetFileName, string sourceFileName, string sourceExt) {
    string name = sourceFileName.substr(0, sourceFileName.length() - sourceExt.length());
    targetFileName = name + ".html";
}





void writerFile(string writeFileName, ofstream& writeFile) throw (Exception) {
    writeFile.open(writeFileName);
    if (!writeFile.is_open()) {
        throw Exception("failed to open write file");
    } else {
        writeFile.close();
    }
}





void readerFile(string readFileName, ifstream& readFile) throw (Exception) {
    readFile.open(readFileName);
    if (!readFile.is_open()) {
        throw Exception("failed to open read file");
    }
}





void convertLines(ifstream& readFile, vector<HtmlLine>& lines) {
    assert(readFile.is_open());
    string line;
    while(getline(readFile, line)) {
        HtmlLine htmlLine(line);
        lines.push_back(htmlLine);
    };
    readFile.close();
}





void generateHtml(const vector<HtmlLine>& lines, string writeFileName, ofstream& writeFile) throw (Exception) {
    Html html(lines);
    writeFile.open(writeFileName);
    if (!writeFile.is_open()) {
        throw Exception("failed to open write file");
    } else {
        writeFile << "<body>";
        writeFile << html.generate();
        writeFile << "</body>";
        writeFile.close();
    } 
}