/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h"
#include "soundex.h"
using namespace std;

/* This function is intended to return a string which
 * includes only the letter characters from the original
 * (all non-letter characters are excluded)
 *
 * There was a bug: the function would assume that
 * the first index of the string would be an alphabet
 * letter, this was fixed by setting result to an empty
 * string.
 */
string lettersOnly(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/*
 * The encode letters function assigns a code
 * of 0-6 based on which letter each index is.
 * @param c is the char that is capitalized to
 * align with whichever code it fits with
 * @return the numerical char code
 */
char encodeLetters(char c)
{
    switch (toupper(c))
    {
    case 'A': case 'E': case 'I': case 'O': case 'U': case 'H': case 'W': case 'Y':
        return '0';
    case 'B': case 'F': case 'P': case 'V':
        return '1';
    case 'C': case 'G': case 'J': case 'K': case 'Q': case 'S': case 'X': case 'Z':
        return '2';
    case 'D': case 'T':
        return '3';
    case 'L':
        return '4';
    case 'M': case 'N':
        return '5';
    case 'R':
        return '6';
    default:
        return '0';
    }
}

/*
 * The coalesce function takes in a string
 * and parses through the string to delete any
 * repeated consecutive indexes, leaving only one copy.
 * @param s, a string of numbers that is inputted
 * into a numerical code by the soundex function
 * @return the coalesced numerical code
 */
string coalesce(string s)
{
    string noRepeats = "";
    int ogSize = s.size();
    for(int i = 0; i<ogSize; i++)
    {
        if(s[i] != s[i+1])
        {
            noRepeats += s[i];
        }
    }
    return noRepeats;
}

/*
 * The firstLetter function takes in two parameters, the original name and
 * a string s representing the coalesced code.
 * The result is recoded to remove the first number, replacing it with the
 * corresponding upper-case letter. The rest of the code is added at the end.
 * @param originalName is the string for the name with only letters, used to replace
 * the first number.
 * @param s is the nearly finished code, needing to replace the first digit.
 * @return the code with the first digit as a letter corresponded to the surname.
 */
string firstLetter(string originalName, string s)
{
    string name = lettersOnly(originalName);
    string codeWithLetter = "";
    codeWithLetter += toupper(name[0]);
    for(int i = 1; i<s.size(); i++)
    {
        codeWithLetter += s[i];
    }
    return codeWithLetter;
}

/*
 * The removeZeros function takes in a string s
 * and removes the 0's in between.
 * @param s which includes potential zeros
 * @return a string similar to s, just without any 0s.
 */
string removeZeros(string s)
{
    string nonZeroResult = "";
    nonZeroResult = s[0];
    for(int i = 1; i<s.size(); i++)
    {
        if(s[i] != '0')
        {
            nonZeroResult += s[i];
        }
    }
    return nonZeroResult;
}

/*
 * The soundex function takes in one argument, a string s and
 * uses the lettersOnly function to convert the string into a string
 * of only alphabetical letters. Then encodes a digit to each of the letters,
 * coalescing it into a code that has no consecutive repeated numbers.
 * This code is this modified so that the first index is the first letter of
 * the string s, and the code is padded with 0's and only 4 index long.
 * @param s takes in a surname that will be converted into a phonetic code.
 * @return the final phonetic code
 */
string soundex(string s)
{
    string name = lettersOnly(s);
    string result = "";

    for(int i = 0; i < name.size(); i++)
    {
        char code = encodeLetters(name[i]);
        result += code;
    }

    result = coalesce(result);
    result = firstLetter(name, result);
    result = removeZeros(result);

    while (result.size() < 4)
    {
        result += '0';
    }

    return result.substr(0, 4);

}

/* The soundexSerach function takes in a filepath that
 * has a database of encoded surnames. Then it takes in
 * inputted data from the user that then checks the filepath
 * database to see if there are any matching codes.
 * After sorting the code, the similar surnames are printed out.
 * @param filepath is the name of the text file database.
 * @return the similar surname to that the user inputted.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames
    bool proceedQuestion = true;
    while(proceedQuestion)
    {
        string surname = getLine("Enter a surname (RETURN to quit): ");
        if(surname.empty())
        {
            break;
        }
        string code = soundex(surname);
        Vector<string> matches;
        for(auto &name : allNames)
        {
            if(soundex(name) == code)
            {
                matches.add(name);
            }
        }
        matches.sort();
        cout << "Soundex code is " << code << endl;
        cout << "Matches from database: " << matches << endl;
        cout << endl;
    }

    cout << "All done!" << endl;
}


/* * * * * * Test Cases * * * * * */

STUDENT_TEST("Testing if question 11 on the short answer is correct")
{
    EXPECT_EQUAL(soundex("Angelou"), "A524");
}

STUDENT_TEST("Testing if several 0's scattered through a string can be filtered out")
{
    EXPECT_EQUAL(removeZeros("1020022031GA0123YY"), "122231GA123YY");
}

STUDENT_TEST("Testing if the first letter of a name can be added to the code,"
             "replacing the first digit")
{
    EXPECT_EQUAL(firstLetter("Gabriel Bo", "12345"), "G2345");
}

STUDENT_TEST("Test if coalesce function can merge 5 repeating consecutive digits into 1, "
             "but not a repeating non-consecutive")
{
    EXPECT_EQUAL(coalesce("1111191"), "191");
}

STUDENT_TEST("Test if encoded letters turn into the right digits")
{
    char a = 'a';
    char b = 'b';
    char c = 'c';
    EXPECT_EQUAL(encodeLetters(a), '0');
    EXPECT_EQUAL(encodeLetters(b), '1');
    EXPECT_EQUAL(encodeLetters(c), '2');
}

STUDENT_TEST("Test exclude of punctuation, digits, and spaces with a non-letter in the first index")
{
    string s = "9Gomez";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "Gomez");
}

/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("Test exclude of punctuation, digits, and spaces")
{
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout")
{
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase")
{
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase")
{
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros")
{
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen")
{
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels")
{
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel")
{
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes")
{
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes")
{
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's")
{
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh")
{
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case")
{
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}
