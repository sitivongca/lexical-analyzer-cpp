#include <iostream>
#include <fstream>
#include <string>
#include "Queue.h"

using namespace std;

// Get a cpp file
// Look loop through each line
// In each line look for certain keywords, +-*/ ++ -- = anything beginning with letters
// If it's c++ stuff the type is native 
// find the data type
// Keep checking for spaces


struct keywords
{
    string types[9] = { "Type", "Identifer", "EOS", ",", "{}", "()", "Native", "Number", "unknown" };
    string nativeDelimiter[7] = { "cout", "cin", "#include", "namespace", "using", "return", "std"};
    string operators[7] = { "+", "-", "*", "/", "++", "--", "="};
    string seperators[6] = { ",", "(" , ")", "{", "}", "//" };
    string dataTypes[5] = { "int", "char" ,  "string" ,  "float" ,  "double" };
    

};

struct ParseVals
{
    string type;
    string value;
    string lineNum;
    Queue tokens;
};


class Parser
{
public:
    string file;
    ofstream newFile;
    ParseVals parseInfo;
    keywords keys;
    void read()
    {
        ifstream cppInput(file);
        string line;
        int curLine = 1;

        if (cppInput.is_open())
        {
            while (getline(cppInput, line))
            {
                parse(curLine, line);
                ++curLine;
            }
            cppInput.close();
        }
        else
            cout << "Couldn't access file\n";



        cppInput.close();
    }

    void parse(int curLine, string line)
    {  
        string delimiter = " ";
        size_t pos = 0;
        string token;
        while ((pos = line.find(delimiter)) != string::npos) 
        {
            token = line.substr(0, pos);
            checkword(curLine, token);
            line.erase(0, pos + delimiter.length());
        }
        token = line.substr(0, pos);
        checkword(curLine, token);

    }
    
    // nightmare function
    void checkword(int curLine, string token)
    {
        // what if instead you check if there is a ; and then just add it after all of this 


        bool tokenLetterRange = (((token[0] >= 65) && token[0] <= 90)) || (((token[0] >= 97) && token[0] <= 122));
        bool tokenNumberRange = (token[0] >= 48) && (token[0] <= 57);
        bool isEOS = true;
        bool native = false;

        if (token.find(";") == string::npos)
        {
            isEOS = false;
        }
        // remove it from the string add on it after
        if (isEOS)
        {
            token = token.substr(0, token.size() - 1);
        }
        if ((int)token[0] == 9)
        {
            token.erase(0, 1);
        }

        // First check if native
        for (int i = 0; i < sizeof(keys.nativeDelimiter) / sizeof(string); ++i)
        {
            if ((token == keys.nativeDelimiter[i]) || (token.substr(0, token.size() - 1)) == keys.nativeDelimiter[i])
            {
                // value: int
                // type: indentifier
                // line number: 5
                
                parseInfo.type = keys.types[6]; // Native
                parseInfo.lineNum = to_string(curLine);
                parseInfo.value = keys.nativeDelimiter[i]; //What the word is
                parseInfo.tokens.add(parseInfo.type);
                parseInfo.tokens.add(parseInfo.lineNum);
                parseInfo.tokens.add(parseInfo.value);

                native = true;
            }
        }

        // Then check if its a type
        for (int i = 0; i < sizeof(keys.dataTypes) / sizeof(string); ++i)
        {
            if (token == keys.dataTypes[i])
            {
                parseInfo.type = keys.types[0]; // Type
                parseInfo.lineNum = to_string(curLine);
                parseInfo.value = keys.dataTypes[i]; // datatype
                parseInfo.tokens.add(parseInfo.type);
                parseInfo.tokens.add(parseInfo.lineNum);
                parseInfo.tokens.add(parseInfo.value);

                return;
            }
        }


        //then check if it's an indentifer
        
        if (tokenLetterRange && !native)
        {
                parseInfo.type = keys.types[1]; // Indentifer
                parseInfo.lineNum = to_string(curLine);
                parseInfo.tokens.add(parseInfo.type);
                parseInfo.tokens.add(parseInfo.lineNum);

                if (token.find("(") != string::npos)
                {
                    parseInfo.value = token.substr(0, token.find("("));
                    parseInfo.tokens.add(parseInfo.value);
                    token = token.erase(0, token.find("("));
                }
                else
                {
                    parseInfo.value = token;

                    parseInfo.tokens.add(parseInfo.value);
                }




        }
        else
        {
            if (tokenNumberRange)
            {
                parseInfo.type = keys.types[7]; // "Number"
                parseInfo.lineNum = to_string(curLine);
                parseInfo.value = token; // "5"
                parseInfo.tokens.add(parseInfo.type);
                parseInfo.tokens.add(parseInfo.lineNum);
                parseInfo.tokens.add(parseInfo.value);
            }

        }


        //check if it's a seperator
        for (int i = 0; i < sizeof(keys.seperators) / sizeof(string); ++i)
        {
            if (token.find(keys.seperators[i]) != string::npos)
            {
                parseInfo.type = keys.seperators[i]; // "("
                parseInfo.lineNum = to_string(curLine);
                parseInfo.value = keys.seperators[i]; // "("
                parseInfo.tokens.add(parseInfo.type);
                parseInfo.tokens.add(parseInfo.lineNum);
                parseInfo.tokens.add(parseInfo.value);

                token.erase(0, token.find(keys.seperators[i]));
            }
        }

        //then check if it's an operator
        for (int i = 0; i < sizeof(keys.operators) / sizeof(string); ++i)
        {
            if (token.find(keys.operators[i]) != string::npos)
            {
                parseInfo.type = keys.operators[i]; // "+"
                parseInfo.lineNum = to_string(curLine);
                parseInfo.value = keys.operators[i]; // "+"
                parseInfo.tokens.add(parseInfo.type);
                parseInfo.tokens.add(parseInfo.lineNum);
                parseInfo.tokens.add(parseInfo.value);

                token.erase(0, token.find(keys.operators[i]));
            }
        }

        if (isEOS)
        {
            parseInfo.type = "EOS"; // "EOS"
            parseInfo.lineNum = to_string(curLine);
            parseInfo.value = ";"; // ";"
            parseInfo.tokens.add(parseInfo.type);
            parseInfo.tokens.add(parseInfo.lineNum);
            parseInfo.tokens.add(parseInfo.value);

        }
    }
    void write()
    {
        newFile.open("lexical.txt");

        while (parseInfo.tokens.count != 0)
        {
        newFile << "type: " << parseInfo.tokens.peek() << " ";
        parseInfo.tokens.remove();
        newFile << "line number: " << parseInfo.tokens.peek() << " ";
        parseInfo.tokens.remove();
        newFile << "value: " << parseInfo.tokens.peek() << "\n";
        parseInfo.tokens.remove();
        }

        newFile.close();
    }
};

int main()
{
    int stop = 0;
    Parser parser;

    while (stop == 0)
    {
        cout << "Input file is called test.cpp\n";
        cout << "What file do you want to read?: ";
        cin >> parser.file;
        parser.read();
        parser.write();
        

    }
}
