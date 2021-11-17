# md-to-html

## Description

md-to-html is a CLI to generate an HTML file from a given Markdown file. It is essentially a language conversion of the open-source program named mdtofile written in Go by @d0iasm. 

Input

* `<example.md>` - a Markdown file

Sample screen output

```
done!
example.md converted to example.html
```

## EBNF for Markdown grammar
```
Document = { Block }, EOF ;
Block = Paragraph | Headings | Lists | CodeBlockFence ;
Inline = Link | Rawtext | Emphasis | Strong | Break ;
Paragraph = String, { String }, Newline ;
Headings = H1 | H2 | H3 | H4 | H5 | H6 ;
H1 = "#", Inline ;
H2 = "#" * 2, Inline ;
H3 = "#" * 3, Inline ;
H4 = "#" * 4, Inline ;
H5 = "#" * 5, Inline ;
H6 = "#" * 6, Inline ;
Lists = List, ( List | Lists )* ;
List = ( (" ")*, "-", " ", Inline ) | Lists ;
CodeBlockFence = "```";
Link = "[" Rawtext "]", "(" Rawtext ")";
Emphasis = "*" Rawtext "*" | "_" Rawtext "_" ;
Strong = "**" Rawtext "**" | "__" Rawtext "__" ;
Break = Rawtext "  " ;
String = { Character } ;
Newline = "\n" ;
```

## Installation

```
git clone https://github.com/surferwat/md-to-html.git
```

## Usage

Step 1: compile program

```
g++ -o md-to-html.out main.cpp exception.h exception.cpp htmlline.h htmlline.cpp html.h html.cpp -std=c++11
```

Step 2: run program

```
./md-to-html.out example.md
```

## Test

Step 1: compile program

```
g++ -o test.out test.cpp exception.h exception.cpp htmlline.h htmlline.cpp html.h html.cpp -std=c++11
```

Step 2: run test

```
./test.out
```

## To Do
* [ ] add suport for css
* [ ] refactor error handling
* [ ] review implementation of break

## References
* [mdtohtml](https://github.com/d0iasm/mdtohtml)
* [ECMAScript syntax](https://www.cplusplus.com/reference/regex/ECMAScript/)
* [Creating an Exception Class](https://www.oreilly.com/library/view/c-cookbook/0596007612/ch09s02.html)