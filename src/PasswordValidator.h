#pragma once

#include "Password.h"

class PasswordValidator {

public:

    template<uint32_t T>
    bool isValid(Password<T>& password);

};