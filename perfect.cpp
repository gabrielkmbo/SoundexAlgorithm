/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "SimpleTest.h"
#include "perfect.h"
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n)
{
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++)
    {
        if (n % divisor == 0)
        {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n)
{
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop)
{
    for (long num = 1; num < stop; num++)
    {
        if (isPerfect(num))
        {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* The smarterSum funciton takes one arguement 'n' and conducts
 * a similar test to divisorSum, but instead of looping from 1
 * to n, it loops from 1 to the square root of n
 * Returning the total sum of divisors for the inputted number.
 * @param n the inputted number with divisors that will be summed
 * @return the divisor sum
 */
long smarterSum(long n)
{
    long total = 0;
    for (long divisor = 1; divisor <= sqrt(n); divisor++)
    {
        if (n % divisor == 0) {
            total += divisor;
            if(n/divisor !=divisor && divisor !=1)
            {
                total += n/divisor;
            }
        }
    }
    return total;
}

/* The isPerfectSmarter function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfectSmarter(long n)
{
    return (n != 0) && (n == smarterSum(n));
}

/* The findPerfectsSmarter function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfectsSmarter(long stop)
{
    for (long num = 1; num < stop; num++)
    {
        if (isPerfectSmarter(num))
        {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This is the findNthPerfectEuclid function which takes in a number n
 * and checks whether it meets the conditions to be the power to the perfect
 * Euclid number.
 * @param n is the Nth value in the 2^k -1 function
 * @return the perfect euclid if n matches the conditions
 */
long findNthPerfectEuclid(long n)
{
    for(int k = 1; k <= n; k++)
    {
        long m = pow(2, k) - 1;
        if(divisorSum(m) == 1)
        {
                return pow(2, k - 1) * m;
        }
    }
    return 0;
}

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("Finding whether Perfect Euclid numbers are correctly checked by the findNthPerfectEuclid(n) function")
{
    EXPECT(isPerfect(findNthPerfectEuclid(13)));
    EXPECT(isPerfect(findNthPerfectEuclid(2)));
    EXPECT(isPerfect(findNthPerfectEuclid(19)));
    EXPECT(isPerfect(findNthPerfectEuclid(2281)));
}

STUDENT_TEST("Multiple time trials of findPerfectsSmarter on increasing input sizes up to 60s")
{
    int smallest = 800000, largest = 16000000;
    for (int size = smallest; size <= largest; size *= 2)
    {
        TIME_OPERATION(size, findPerfectsSmarter(size));
    }
}

STUDENT_TEST("Checks if divisorSum and smarterSum are equal")
{
    long dSum1 = divisorSum(25);
    long sSum1 = smarterSum(25);
    EXPECT(dSum1 == sSum1);

    long dSum2 = divisorSum(36);
    long sSum2 = smarterSum(36);
    EXPECT(dSum2 == sSum2);

    long dSum3 = divisorSum(1023);
    long sSum3 = smarterSum(1023);
    EXPECT(dSum3 == sSum3);
}

STUDENT_TEST("Checks if isPerfect is receiving a negative input")
{
    long n = divisorSum(-1);
    EXPECT(isPerfect(n));
}

STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes")
{
    int smallest = 32000, largest = 1000000;
    for (int size = smallest; size <= largest; size *= 2)
    {
        TIME_OPERATION(size, findPerfects(size));
    }
}

/*
 * Here is sample test demonstrating how to use a loop to set the input sizes
 * for a sequence of time trials.
 */
//STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes") {
//    int smallest = 1000, largest = 8000;
//    for (int size = smallest; size <= largest; size *= 2) {
//        TIME_OPERATION(size, findPerfects(size));
//    }
//}


/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}
