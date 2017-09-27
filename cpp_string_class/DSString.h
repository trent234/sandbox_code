//hommade string class
//more functionality that mirrors the std library string class to come
//plus brilliant additional functionality!!
//trent wilson 6/14/2017

#ifndef STRING
#define STRING

#include <iostream>
#include <cstring>
#include "dsvector.h"

class String
{
    friend std::ostream& operator<< (std::ostream&, const String&);
    friend std::istream& operator>> (std::istream&, String&);

    //input stream, output string, list of chars to know when the word stops, stop all char
    friend String getLine (std::istream&, String&, DSVector<char>);

    //friend String operator+ (const char*, const String&); //to many args?

    public:
        String(); //blank string. not tested in the TDD?
        String(const char*); //entering a string literal
        String(const String&); //copy constructor

        String& operator= (const char*);
        String& operator= (const String&);
        String operator+ (const char*);
        String operator+ (const String&);
        bool operator== (const char*);
        bool operator== (const String&);
        bool operator> (const String&);
        bool operator< (const String&);
        char& operator[] (const int);

        int size() const;
        String substring(int, int) const;       
        char* c_str();
        int toInt();
        String toUpper() const; //returns an upper copy. ideal? maybe nonconst changes to upper? this way is safe for now

        //made this basically for index proj. but char = one either side of substring
        //int is the number of that char to start the substr at
        String substringSearch(char, int);
        //basically a utility function for the above. but need access for loop counter
        void setSubstringSearchMax(char);
        int getSubstringSearchMax();

        ~String();

    private:
        char* stringVar;
        long long stringLen;


        int substringSearchMax;
};

#endif
