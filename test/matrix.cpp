#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

struct Stream
{
    int x;
    int y;
    int speed;
    int length;
};

int main()
{
    // Clear screen and hide cursor for a clean effect
    std::cout << "\033[2J\033[?25l";

    const int width = 75;
    const int height = 22;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disChar(33, 126);
    std::uniform_int_distribution<> disLen(5, 14);
    std::uniform_int_distribution<> disSpeed(1, 3);

    std::vector<Stream> streams;
    for (int i = 0; i < width; i += 2)
    {
        streams.push_back({i, -disLen(gen), disSpeed(gen), disLen(gen)});
    }

    // Run animation for ~300 frames (approx 15 seconds)
    for (int frame = 0; frame < 300; ++frame)
    {
        // Move cursor back to top-left (ANSI escape code)
        std::cout << "\033[H";

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                bool rendered = false;
                for (const auto &s : streams)
                {
                    if (s.x == x && y >= s.y && y < s.y + s.length)
                    {
                        char c = static_cast<char>(disChar(gen));
                        if (y == s.y + s.length - 1)
                        {
                            // Bright white/green head of the stream
                            std::cout << "\033[1;37m" << c << "\033[0m";
                        }
                        else
                        {
                            // Dim green tail
                            std::cout << "\033[0;32m" << c << "\033[0m";
                        }
                        rendered = true;
                        break;
                    }
                }
                if (!rendered)
                {
                    std::cout << ' ';
                }
            }
            std::cout << '\n';
        }

        // Update stream positions downwards
        for (auto &s : streams)
        {
            s.y += s.speed;
            if (s.y >= height)
            {
                s.y = -s.length;
                s.x = (s.x + 4) % width;
                s.speed = disSpeed(gen);
                s.length = disLen(gen);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    // Restore cursor before exiting
    std::cout << "\033[?25h\n";
    return 0;
}