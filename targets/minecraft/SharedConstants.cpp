
#include "minecraft/SharedConstants.h"

#include <string>

const std::string SharedConstants::VERSION_STRING = "1.6.4";
const bool SharedConstants::TEXTURE_LIGHTING = true;

std::string SharedConstants::readAcceptableChars() {
    // 4J-PB - I've added ã in (for Portuguese in bed string) and added the
    // character at the same place in the default.png font
    std::string result =
        " !\"#$%&'()*+,-./"
        "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'"
        "abcdefghijklmnopqrstuvwxyz{|}~"
        " ÇüéâäàåçêëèïîìÄÅÉæÆôöòûùÿÖÜø£Ø×ƒáíóúñÑªº¿®¬½¼¡«»ã";
    return result;
}

bool SharedConstants::isAllowedChatCharacter(char ch) {
    // return ch != '§' && (acceptableLetters.indexOf(ch) >= 0 || (int) ch >
    // 32);
    //  4J Unused
    return true;
}

std::string SharedConstants::acceptableLetters;

void SharedConstants::staticCtor() {
    acceptableLetters = readAcceptableChars();
}

const char
    SharedConstants::ILLEGAL_FILE_CHARACTERS[ILLEGAL_FILE_CHARACTERS_LENGTH] = {
        // 4J-PB  - corrected
        '/', '\n', '\r', '\t', '\0', '\f', '`', '?',
        '*', '\\', '<',  '>',  '|',  '\"', ':'};