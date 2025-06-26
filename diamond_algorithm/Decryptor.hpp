#ifndef DECRYPTOR_HPP
#define DECRYPTOR_HPP

#include <string>  // added missing include - for using std::string
#include "Grid.hpp"  // includes the Grid class definition
#include "Cycle.hpp"  // includes the Cycle class definition


class Decryptor {
public:
    explicit Decryptor(int rounds, bool verbose = false);
    // constructor: sets up a Decryptor object.
    // rounds: the number of decryption rounds to perform.
    // verbose: a flag to control detailed output (true = show details).
    // explicit: prevents unintended type conversions.

    [[nodiscard]] std::string decrypt(const std::string& encryptedMessage) const;
    // decrypts an encrypted message.
    // encryptedMessage: the message to be decrypted.
    // [[nodiscard]]: indicates that the return value should be used.
    // const: indicates that this function does not modify the Decryptor object.

    [[nodiscard]] std::string decryptWithDisplay(const std::string& encryptedMessage) const;
    // decrypts an encrypted message and displays the process.
    // encryptedMessage: the message to be decrypted.
    // [[nodiscard]]: indicates that the return value should be used.

    [[nodiscard]] const std::string& getDiamondLetters() const { return diamondLetters; }
    // retrieves the diamondLetters string.
    // diamondLetters: a string (presumably) storing letters extracted in a diamond pattern.
    // const: indicates that this function does not modify the Decryptor object.

    static void displayDecryptionHeader(int pass, int total);
    // displays a header for a decryption pass.
    // pass: the current decryption pass number.
    // total: the total number of decryption passes.
    // static: can be called without creating a Decryptor object.

    static void displayGridState(const Grid& grid);
    // displays the current state of a Grid object.
    // grid: the Grid object to display.
    // static: can be called without creating a Decryptor object.

    static void displayLayerExtraction(int layer, const std::vector<std::pair<int, int>>& path);
    // displays information about the extraction from a specific layer.
    // layer: the layer number.
    // path: the extraction path (coordinates).
    // static: can be called without creating a Decryptor object.

    static void displayFinalResult(const std::string& result);
    // displays the final decryption result.
    // result: the decrypted message.
    // static: can be called without creating a Decryptor object.

private:
    int rounds;  // stores the number of decryption rounds
    bool verbose; // flag to control verbose output
    std::string diamondLetters; // stores extracted diamond letters
    [[nodiscard]] std::string decryptSingleRound(const std::string& encrypted) const;
    // decrypts the message for a single round.
    // encrypted: the message to decrypt in this round.
    // [[nodiscard]]: indicates that the return value should be used.

    static std::string prepareForNextRound(const std::string& message);
    // prepares the message for the next decryption round (e.g., trimming).
    // message: the message to prepare.
    // static: can be called without creating a Decryptor object.
};

#endif