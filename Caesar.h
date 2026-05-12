#ifndef CAESAR_H
#define CAESAR_H

#include <string>
using namespace std;

class Caesar {
private:
    int shift;

public:
    Caesar(int shift) {
        this->shift = shift;
    }

    string encrypt(string text, int s) {
        for (int i = 0; i < text.length(); i++) {
            if (text[i] >= 'a' && text[i] <= 'z') {
                text[i] = (text[i] - 'a' + s) % 26 + 'a';
            }
        }
        return text;
    }

    string decrypt(string text) {
        return encrypt(text, 26 - shift);
    }
};

#endif