// implementation of the data structures string class
// trent wilson
// 5/16/2017

#include <iostream>
#include <cstring>
#include <DSString.h>

std::ostream& operator<< (std::ostream& output, const String& inputVar)
{
    for (int i = 0; i < inputVar.stringLen; i++)
    {
        output << inputVar.stringVar[i]; //inputVar is the param object being passed (needed bc not member function. stringVar is the data member)
    }
    return output;
}

std::istream& operator>>(std::istream& input, String& inputVar)
{
    inputVar.stringLen = 0;
    inputVar.stringVar = new char[inputVar.stringLen + 1];
    inputVar.stringVar[inputVar.stringLen] = '\0';

    long long lenCounter = 0;

    char inputChar;
    while (input.get(inputChar))
    {
        // end-of-input delimiter (change according to your needs)
        if (inputChar == '\n') break;

        // if buffer is exhausted, reallocate it twice as large
        if (lenCounter == inputVar.stringLen)
        {
            //this is needed for me bc i chose stringlen to be w/o null term.
            //so starts at 0. so *2 would do nothing
            if (inputVar.stringLen == 0) inputVar.stringLen++;
            inputVar.stringLen *= 2;
            char *newString = new char[inputVar.stringLen + 1];
            strcpy(newString, inputVar.stringVar);
            delete[] inputVar.stringVar;
            inputVar.stringVar = newString;

        }
        // store input char
        inputVar.stringVar[lenCounter] = inputChar;
        //put this null char here instead of below.
        //subtle bug making my string class do crazy things
        inputVar.stringVar[lenCounter + 1] = '\0';

        lenCounter++;
    }
    //missing this line was BUGGING me. stringlen gets multiplied by two to make room
    //but needs to be actual string len at the end.
    inputVar.stringLen = lenCounter;
    //inputVar.stringVar[lenCounter] = '\0';
    return input;
}

String getLine (std::istream& input, String& inputString, DSVector<char> EOI_Marker)
{
//    inputString.stringLen = 0;
//    inputString.stringVar = new char[inputString.stringLen + 1];
//    inputString.stringVar[inputString.stringLen] = '\0';

    //this var tracks where we are along the array for extracting and inserting
    long long lenCounter = 0;

    char inputChar;
    while (input.get(inputChar))
    {
        // if we hit the end-of-input marker.. bail on this loop.
        bool EOI_Check = false;
        for(int i = 0; i < EOI_Marker.get_size(); i++)
            if (inputChar == EOI_Marker[i]) EOI_Check = true;
        if (EOI_Check == true)
            break;

        // if buffer is exhausted, reallocate it twice as large
        /* valgrind fix #1: wow, helpful. because below the if statement
         * we add the input char at lencounter spot and then add the nullchar (2 chars).
         * if lencounter is one less than stringlen we wont grow the size (it was len counter == stringlen)
         * and we will add the char to the last element. but THEN we add the nullchar to one
         * element after the array has ended. aka heap block overrun.
         * but making this change also leads to situations where lencounter > stringlen - 1 so..
         * we need to have >= not ==. thanks valgrind! */
        if (lenCounter >= inputString.stringLen - 1)
        {
            //needed bc i chose stringlen to be w/o null term. so starts at 0. so *2 would do nothing
            if (inputString.stringLen == 0)
                inputString.stringLen++;
            inputString.stringLen *= 2;
            char *newString = new char[inputString.stringLen];
            strcpy(newString, inputString.stringVar);
            delete[] inputString.stringVar;
            inputString.stringVar = newString;
        }
        // finally store input char
        inputString.stringVar[lenCounter] = inputChar;
        inputString.stringVar[lenCounter + 1] = '\0';
        lenCounter++;
    }
    //inputString.stringVar[lenCounter] = '\0';
    //missing this line was BUGGING me. stringlen gets multiplied by two to make room
    //but needs to be actual string len at the end.
    inputString.stringLen = lenCounter;
    return inputString;
}


String::String() //TDD doesn't check this but it was given in the header?
{
    stringVar = new char[1];
    //stringVar = {'\0'};
    stringVar[0] = '\0';
    stringLen = 0;
}

String::String(const char* inputVar)
{
    stringLen = std::strlen(inputVar);
    stringVar = new char[stringLen + 1];
    std::strcpy(stringVar, inputVar);
    stringVar[stringLen] = '\0';
}

String::String(const String& rightVar) //copy constructor
{
    stringLen = std::strlen(rightVar.stringVar); //this could be stringLen = rightVar.stringLen, right? only avoid assignment with pointers?
    stringVar = new char[stringLen + 1];
    std::strcpy(stringVar, rightVar.stringVar);
    stringVar[stringLen] = '\0';
}

String& String::operator= (const char* inputVar)
{
    //stringLen = std::strlen(inputVar);
    (inputVar != '\0') ? stringLen = std::strlen(inputVar) : stringLen = 0; //segfault with strlen(null)

    stringVar = new char[stringLen + 1];

    for(long long i = 0; i < stringLen; i++)
    {
        stringVar[i] = inputVar[i];
    }
    stringVar[stringLen] = '\0';
    //this is what i want to return when i want to return the user defined object that this member function is part of, huh? neato
    return *this;
}

String& String::operator= (const String& rightVar)
{  
    //stringLen = std::strlen(rightVar.stringVar);
    // had: stringLen = rightVar.stringLen; until valgrind
    stringLen = rightVar.size();

    /* valgrind #4: pointed out that the default constructor's 1 byte block was being leaked for every
     * string. realized we create a new, and then in the assignment, we create another new. bad!
     * lets delete the old before making the new. thanks valgrind! */

    delete[] stringVar;
    stringVar = new char[stringLen + 1];

    for(long long i = 0; i < stringLen; i++)
    {
        stringVar[i] = rightVar.stringVar[i];
    }
    stringVar[stringLen] = '\0';
    return *this;
}

String String::operator+ (const char* rightVar)
{
    long long stringLen = std::strlen(rightVar);
    // the sum of both strings size.
    unsigned newLen = this->stringLen + stringLen;
    //+1 for null char
    char holderVar[newLen + 1];
    //put left vals into first half of holderVar
    for(long long i = 0; i < this->stringLen; i++)
    {
        holderVar[i] = this->stringVar[i];
    }
    //put right vals into second half of holderVar
    for(long long i = 0; i < newLen - this->stringLen; i++)
    {
        holderVar[i + this->stringLen] = rightVar[i];
    }
    //end the array with the null terminator
    holderVar[newLen] = '\0';

    //quick edit here to return this not char or string or whatever. compare to other overloaded + below. test.
    this->stringLen = newLen;
    delete[] this->stringVar;
    stringVar = new char[stringLen + 1];
    std::strcpy(stringVar, holderVar);
    stringVar[stringLen] = '\0';

    return *this;

}

String String::operator+ (const String& rightVar)
{
    // the sum of both strings size.
    unsigned newLen = stringLen + rightVar.stringLen;
    //+1 for null char
    char holderVar[newLen + 1];
    //put left vals into first half of holderVar
    for(long long i = 0; i < stringLen; i++)
    {
        holderVar[i] = stringVar[i];
    }
    //put right vals into second half of holderVar
    for(long long i = 0; i < newLen - stringLen; i++)
    {
        holderVar[i + stringLen] = rightVar.stringVar[i];
    }
    //end the array with the null terminator
    holderVar[newLen] = '\0';

    String returnString(holderVar);

    return returnString;
}

bool String::operator== (const char* inputVar)
{

    long long inputLen = std::strlen(inputVar);

    if (stringLen != inputLen) return false;

    for(long long i = 0; i < stringLen; i++)
    {
        if(stringVar[i] != inputVar[i]) return false;
    }
    return true;

//    bool returnVar;
//    strcmp(stringVar,inputVar) == 0 ? returnVar = true : returnVar = false;
//    return returnVar;
}

bool String::operator== (const String& rightVar)
{

    if (stringLen != rightVar.stringLen) return false;

    for(long long i = 0; i < stringLen; i++)
    {
        if(stringVar[i] != rightVar.stringVar[i]) return false;
    }
    return true;

//    bool returnVar;
//    strcmp(stringVar,rightVar.stringVar) == 0 ? returnVar = true : returnVar = false;
//    return returnVar;

}

bool String::operator> (const String& rightVar)
{
    bool returnVar;
    strcmp(stringVar,rightVar.stringVar) > 0 ? returnVar = false : returnVar = true;
    return returnVar;
}
bool String::operator< (const String& rightVar)
{
    bool returnVar;
    strcmp(stringVar,rightVar.stringVar) > 0 ? returnVar = true : returnVar = false;
    return returnVar;
}

char& String::operator[] (const int element)
{
    if (element >= 0) return stringVar[element];
    //this could be more clearly written as minus the abs val of the element. -3. stringLen - 3 aka 3 from the back
    return stringVar[stringLen + element];
}

int String::size() const
{
    return stringLen;
}

String String::substring(int x, int y) const
{
    int leftVar, rightVar;

    if (x >= 0)
        leftVar = x;
    else
        leftVar = stringLen + x + 1; //negative means offset from end

    if (y >= 0)
        rightVar = y;
    else
        rightVar = stringLen + y + 1; //negative means offset from end

    //rightsize array plus null term spot
    char* holderVar = new char[rightVar - leftVar + 1];

    int counter = 0;
    for (int i = leftVar; i <= rightVar; i++)
    {
        holderVar[counter] = stringVar[i];
        counter++;
    }

    //null term last element
    holderVar[rightVar - leftVar] = '\0';

    String returnString(holderVar);
    delete[] holderVar; //deletes dynamic mem in this function. no leaks here.

    return returnString;
}

/*
String String::substringSearch(char inputChar, int startInt)
{
    int counter = 0; //tracks how many of the inputchar we've come across so far
    int elementNum = 0; //tracks where we are on the string
    while (counter != startInt)
    {
        if (stringVar[elementNum] == inputChar)
            counter++;
        elementNum++;
    }

    char* tempSubStr = new char[stringLen - elementNum + 3]; //size is from where we are till end
    //if we are going from the last INPUTCHAR to EOL then we need to stop at end of str
    if(startInt != substringSearchMax)
    {
        int i = 0;
        while(stringVar[elementNum] != inputChar) //this is us going from one INPUTCHAR to the next INPUTCHAR
        {
            tempSubStr[i] = stringVar[elementNum];
            elementNum++;
            i++;
        }
    }
    else
    {
        int i = 0;
        while(elementNum < stringLen) //this is us going from the last INPUTCHAR to the end
        {
            tempSubStr[i] = stringVar[elementNum];
            elementNum++;
            i++;
        }
    }
    tempSubStr[elementNum] = '\0';
    String returnSubString = tempSubStr;
    delete[] tempSubStr;
    return returnSubString;
}
*/

String String::substringSearch(char inputChar, int startInt)
{
    int counter = 0;
    int elementNum = 0; //tracks where we are on the full line string
    while (counter != startInt)
    {
        if (stringVar[elementNum] == inputChar)
            counter++;
        elementNum++;
    }
    int lowerBound = elementNum; //we've saved the element number of the first char of our word
    if(startInt != substringSearchMax) //if we arent grabbing the last word in the full line string
    {
        while (stringVar[elementNum] != inputChar) //what if there are two spaces in a row? solve this later
            elementNum++;
    }
    else
    {
        while (elementNum <= stringLen) //what if there are two spaces in a row? solve this later
            elementNum++;
    }
    int upperBound = elementNum - 1;

    char* tempSubStr = new char [upperBound - lowerBound + 1];
    counter = 0;
    for (int i = lowerBound; i <= upperBound; i++)
    {
        tempSubStr[counter] = stringVar[i];
        counter++;
    }
    String returnSubString = tempSubStr;
    delete[] tempSubStr;
    return returnSubString;
}

void String::setSubstringSearchMax(char inputChar)
{
    int counter = 0;
    for(int i = 0; i < stringLen; i++)
    {
        if (stringVar[i] == inputChar)
            counter++;
    }
    substringSearchMax = counter;
}

int String::getSubstringSearchMax()
{
    return substringSearchMax;
}


char* String::c_str()
{
    char* cString = new char[stringLen + 1];
    //cString = stringVar; //no
    std::strcpy(cString, stringVar);
    //not plus 1 bc total elements - 1 is last element when it starts w 0
    cString[stringLen] = '\0';
    return cString;
}

int String::toInt()
{
    int intVar = atoi(stringVar);
    return intVar;
}

String String::toUpper() const
{
    char* upperCArray = new char[stringLen + 1];
    for(int i = 0; i < stringLen; i++)
        *(upperCArray + i) = toupper(*(stringVar + i));

    upperCArray[stringLen] = '\0';

    String upperString(upperCArray);
    delete[] upperCArray;

    return upperString;
}


String::~String()
{
    delete[] stringVar;
}

//std::istream& operator>> (std::istream& input, const char* inputVar)
//{

//    inputVar.stringLen = std::strlen(inputVar);
//    inputVar.stringVar = new char[stringLen + 1];
//    //std::strcpy(stringVar, inputVar);

//    for (int i = 0; i < stringVar.stringLen + 1; i++)
//    {
//        input >> stringVar.stringVar[i]; //inputVar is the param object being passed (needed bc not member function. stringVar is the data member)
//    }

//    return input;
//}
