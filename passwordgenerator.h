#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <cstdlib>
#include <string>

class PasswordGenerator{

private:
    bool bUpperCase, bLowerCase, bNumbers, bSpecChars;
    int iPasswordLength;

    int RandomNumber(int iMin, int iMax) const;
    int GenerateNumber() const;
    std::string GenerateUpperCase() const;
    std::string GenerateLowerCase() const;
    std::string GenerateSpecChar() const;

public:
    PasswordGenerator(bool bUpperCase = true, bool bLowerCase = true, bool bNumbers = true, bool bSpecChars = false, int iPasswordLength = 20);
    std::string GeneratePassword() const;
};

#endif // PASSWORDGENERATOR_H
