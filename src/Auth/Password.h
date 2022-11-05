#pragma once

#include <Arduino.h>

struct Password {

    const int Size;
    char* Content;

    Password(int Size): Size(Size), Content(new char[Size]) {}

    Password(const Password&& other): Password(other.Size) {
        memcpy(Content, other.Content, other.Size);
    }

    Password(const Password& other): Password(other.Size) {
        memcpy(Content, other.Content, other.Size);
    }

    ~Password() {
        delete[] Content;
    }

    inline char& operator[] (int index) {
        return Content[index];
    }

    bool operator= (Password other) const {
        if(Size != other.Size)
            return false;
        
        for(int i = 0; i < Size; i++) {
            if(Content[i] != other[i])
                return false;
        }

        return true;
    }

};