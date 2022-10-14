#include <iostream>
#include <random>
#include <ctime>

int main()
{
    std::default_random_engine e(static_cast<unsigned int>(time(nullptr)));
    e();
    std::uniform_int_distribution<unsigned int> u(1, 1000);
    unsigned int price = u(e);
    unsigned int guess = 0;
    std::cout << "Now you can try to guess the price.(1~1000)" << std::endl;
    while (true)
    {
        if (std::cin >> guess && guess >= 1 && guess <= 1000)
        {
            if (guess > price)
                std::cout << "You've guessed higher than it's price, try again." << std::endl;
            else if (guess < price)
                std::cout << "You've guessed lower than it's price, try again." << std::endl;
            else
            {
                std::cout << "You've guessed right!" << std::endl;
                break;
            }
        }
        else
            std::cout << "What the hell are you doing? Try again!" << std::endl;
        std::cin.clear();
        std::cin.sync();
    }
}