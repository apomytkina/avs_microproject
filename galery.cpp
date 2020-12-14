#define HAVE_STRUCT_TIMESPEC
#include <array>
#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

std::ofstream _file;
std::mutex g_lockprint;
constexpr int no_of_visitors = 50;
std::vector<int> business = { 0, 0, 0, 0, 0 };

struct image
{
    int numb;
    std::mutex mutex;
};

struct galery
{
    std::atomic<bool> ready{ false };
    std::array<image, no_of_visitors> images;
};

struct visitor
{
private:
    std::string name;
    galery const& imagegalery;
    image& image1;
    image& image2;
    image& image3;
    image& image4;
    image& image5;
    std::thread  lifethread;

public:
    visitor(std::string_view n, galery const &g, image& im1, image& im2, image& im3, image& im4, image& im5) :
        name(n), imagegalery(g), image1(im1), image2(im2), image3(im3), 
        image4(im4), image5(im5), lifethread(&visitor::startwatching, this)
    {
    }

    ~visitor()
    {
        lifethread.join();
    }

    void startwatching()
    {
        while (!imagegalery.ready);

        do
        {
            watch();
        } while (imagegalery.ready);
    }

    void print(std::string_view text, int numb)
    {
        std::lock_guard<std::mutex> cout_lock(g_lockprint);
        std::cout << std::left << std::setw(10) << std::setfill(' ') << name << text << numb <<std::endl;
    }

    void print(std::string_view text)
    {
        std::lock_guard<std::mutex> cout_lock(g_lockprint);
        std::cout << std::left << std::setw(10) << std::setfill(' ') << name << text << std::endl;
    }

    void watch()
    {
        std::lock(image1.mutex, image2.mutex, image3.mutex, image4.mutex, image5.mutex);

        std::lock_guard<std::mutex> image_1(image1.mutex, std::adopt_lock);
        std::lock_guard<std::mutex> image_2(image2.mutex, std::adopt_lock);
        std::lock_guard<std::mutex> image_3(image3.mutex, std::adopt_lock);
        std::lock_guard<std::mutex> image_4(image4.mutex, std::adopt_lock);
        std::lock_guard<std::mutex> image_5(image5.mutex, std::adopt_lock);

        while (business[image1.numb - 1] > 9) {
            print(" waits for image ", image1.numb);
        }
        print(" watches image ", image1.numb);
        business[image1.numb - 1]++;

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        business[image1.numb - 1]--;
        while (business[image2.numb - 1] > 9) {
            print(" waits for image ", image2.numb);
        }
        print(" watches image ", image2.numb);
        business[image2.numb - 1]++;

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        business[image2.numb - 1]--;
        while (business[image3.numb - 1] > 9) {
            print(" waits for image ", image3.numb);
        }
        print(" watches image ", image3.numb);
        business[image3.numb - 1]++;

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        business[image3.numb - 1]--;
        while (business[image4.numb - 1] > 9) {
            print(" waits for image ", image4.numb);
        }
        print(" watches image ", image4.numb);
        business[image4.numb - 1]++;

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        business[image4.numb - 1]--;
        while (business[image5.numb - 1] > 9) {
            print(" waits for image ", image5.numb);
        }
        print(" watches image ", image5.numb);
        business[image5.numb - 1]++;

        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        business[image5.numb - 1]--;
        print(" finished wathcing");
    }
};

int main(int argc, char* argv[])
{
    galery galery;

    galery.images[0].numb = 1;
    galery.images[1].numb = 2;
    galery.images[2].numb = 3;
    galery.images[3].numb = 4;
    galery.images[4].numb = 5;

    //_file.open(argv[1]);

    std::array<visitor, no_of_visitors> visitors
    {
       {
          { "Visitor 1", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 2", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 3", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 4", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 5", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },
          { "Visitor 6", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 7", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 8", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 9", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 10", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },

          { "Visitor 11", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 12", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 13", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 14", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 15", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },
          { "Visitor 16", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 17", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 18", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 19", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 20", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },

          { "Visitor 21", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 22", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 23", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 24", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 25", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },
          { "Visitor 26", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 27", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 28", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 29", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 30", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },

          { "Visitor 31", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 32", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 33", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 34", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 35", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },
          { "Visitor 36", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 37", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 38", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 39", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 40", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },

          { "Visitor 41", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 42", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 43", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 44", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 45", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },
          { "Visitor 46", galery, galery.images[0], galery.images[1], galery.images[2], galery.images[3], galery.images[4] },
          { "Visitor 47", galery, galery.images[1], galery.images[2], galery.images[3], galery.images[4], galery.images[0] },
          { "Visitor 48", galery, galery.images[2], galery.images[3], galery.images[4], galery.images[0], galery.images[1] },
          { "Visitor 49", galery, galery.images[3], galery.images[4], galery.images[0], galery.images[1], galery.images[2] },
          { "Visitor 50", galery, galery.images[4], galery.images[0], galery.images[1], galery.images[2], galery.images[3] },
       }
    };

    galery.ready = true;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    galery.ready = false;

    return 0;
}
