#pragma once
#pragma once
#include <iostream>
#include <cstring>
using namespace std;

class HospitalException {
    char message[200];
public:
    HospitalException(const char* msg) { strcpy_s(message, 200, msg); }
    virtual const char* what() { return message; }
};
class InvalidInputException : public HospitalException {
public:
    InvalidInputException() : HospitalException("Invalid input.") {}
};
class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException() : HospitalException("Insufficient funds.") {}
};
class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException() : HospitalException("Time slot unavailable.") {}
};
class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException() : HospitalException("Required file not found.") {}
};