#include "gmp.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

// Segfault at 10901 iterations
// Probably stack overflow
// How do you do NodeJS's process.nextTick() here lol

// Increasing stack size makes it not overflow, but now there's a memory leak *somewhere*
// I have no idea where it's happening

// Not using namespace std because why not

std::chrono::milliseconds initialTime;
std::string inputNumber;
int iterations;
int delay;

std::string invertArray(char str[]) {
    int length = strlen(str);
    char tmp;
    for (int i = 0; i < length / 2; i++) {
        tmp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = tmp;
    }

    std::string out = str;
    return out;
}

std::string invertString(std::string str) {
    int length = str.length();
    char arr[length + 1];
    strcpy(arr, str.c_str());
    std::string out = invertArray(arr);
    return out;
}

void checkPalindromic(std::string numberString, int iterations, int currentIteration, int delay, std::chrono::milliseconds initialTime) {
    currentIteration++;

    if (currentIteration != iterations) {
        mpz_t number;
        mpz_init(number);
        mpz_set_ui(number, 0);
        mpz_set_str(number, numberString.c_str(), 10);

        std::string inum = mpz_get_str(NULL, 10, number);
        std::string fnum = invertString(inum);

        mpz_t rnumber;
        mpz_init(rnumber);
        mpz_set_ui(rnumber, 0);
        mpz_set_str(rnumber, fnum.c_str(), 10);

        mpz_t sum;
        mpz_init(sum);
        mpz_set_ui(sum, 0);
        // mpz_set_str(sum, inum.c_str(), 10);     <-- probably no need for this line

        mpz_add(sum, number, rnumber);

        std::string sumstr = mpz_get_str(NULL, 10, sum);
        std::string revsum = invertString(sumstr);

        std::cout << "Iteration " << currentIteration << "; " << inum << " + " << fnum << " = " << sumstr << std::endl;
        std::cout << "\033]0;Iteration " << currentIteration << " of " << iterations << "\007";

        if (sumstr == revsum) {
            std::chrono::milliseconds currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
            std::chrono::milliseconds cseconds = currentTime - initialTime;

            std::cout << "Palindromic after " << std::to_string(currentIteration) << " iterations; Took " << std::to_string(cseconds.count()) << "ms\n";
        } else {
            std::cout << "Not yet palindromic... trying again\n";
            if (delay != 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }

            mpz_clear(number);
            mpz_clear(rnumber);
            mpz_clear(sum);
            
            checkPalindromic(sumstr, iterations, currentIteration, delay, initialTime);
        }
    } else {
        std::chrono::milliseconds currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
        std::chrono::milliseconds cseconds = currentTime - initialTime;

        std::cout << "Not palindromic as of " << std::to_string(iterations) << " iterations; Took " << std::to_string(cseconds.count()) << "ms\n";
    }

    return;
};

int main() {

    std::cout << "Input Number: ";
    std::cin >> inputNumber;
    std::cout << "Input Iterations: ";
    std::cin >> iterations;
    std::cout << "Input Delay (ms): ";
    std::cin >> delay;

    initialTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    checkPalindromic(inputNumber, iterations, 0, delay, initialTime);
    return 0;
}
