#include <iostream>
#include <unordered_map>
#include <random>
#include <string>

const int N = 45000;

class Random
{
public:
    // Function to generate random strings of a given length
    static std::string generate_random_string(int size)
    {
        const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::random_device rd;        // Obtain a random number from hardware
        std::mt19937 generator(rd()); // Seed the generator
        std::uniform_int_distribution<> distribution(0, characters.size() - 1);

        int length = size + 1;
        std::string random_string;
        for (size_t i = 0; i < length; ++i)
            random_string += characters[distribution(generator)];
        return random_string;
    }
    static std::string random_int()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, N - 1);
        return std::to_string(dist(gen));
    }
};