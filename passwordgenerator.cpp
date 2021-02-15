#include "passwordgenerator.h"

PasswordGenerator::PasswordGenerator(bool bUpperCase, bool bLowerCase, bool bNumbers, bool bSpecChars, int iPasswordLength){
    this->bUpperCase = bUpperCase;
    this->bLowerCase = bLowerCase;
    this->bNumbers = bNumbers;
    this->bSpecChars = bSpecChars;
    this->iPasswordLength = iPasswordLength;
}

int PasswordGenerator::RandomNumber(int iMin, int iMax) const{
    return(rand()%(iMax-iMin+1)+iMin);
}

int PasswordGenerator::GenerateNumber() const{
    return this->RandomNumber(0,9);
}

std::string PasswordGenerator::GenerateUpperCase() const{
    std::string AZ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    return AZ.substr(this->RandomNumber(0,AZ.length()-1),1);
}

std::string PasswordGenerator::GenerateLowerCase() const{
    std::string az = "abcdefghijklmnopqrstuvwxyz";
    return az.substr(this->RandomNumber(0,az.length()-1),1);
}

std::string PasswordGenerator::GenerateSpecChar() const{
    std::string chars = "!'#$%&\"()*+`-.{/:;<=>?@[]\\^_|}";
    return chars.substr(this->RandomNumber(0,chars.length()-1),1);
}

std::string PasswordGenerator::GeneratePassword() const{
    std::string password;
    bool generateDone;

    if(!this->bUpperCase && !this->bLowerCase
            && !this->bNumbers && !this->bSpecChars)
        return "";

    for(int i = 0; i < this->iPasswordLength; i++){
        do{
            generateDone = false;
            switch(this->RandomNumber(0,4)){
                case 0:
                    if(this->bUpperCase){
                        password += this->GenerateUpperCase();
                        generateDone = true;
                    }
                    break;
                case 1:
                    if(this->bLowerCase){
                        password += this->GenerateLowerCase();
                        generateDone = true;
                    }
                    break;
                case 2:
                    if(this->bNumbers){
                        char n[1];
                        itoa(this->GenerateNumber(),n,10);
                        password += n[0];
                        generateDone = true;
                    }
                    break;
                case 3:
                    if(this->bSpecChars){
                        password += this->GenerateSpecChar();
                        generateDone = true;
                    }
                    break;
            }
        }
        while(!generateDone);
    }

    return password;
}
