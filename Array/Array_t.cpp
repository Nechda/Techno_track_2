#include "Array.hpp"
#include <string>
#include <ctime>
#include <assert.h>

using namespace std;

class Furniture
{
    private:
        string m_name;
    public:
        Furniture() : m_name("Empty") {}
        Furniture(string name) : m_name(name) {}
        ~Furniture() {}
        Furniture(const Furniture& other){ m_name = other.m_name; }
        Furniture& operator = (const Furniture& other)
        {
            m_name = string(other.m_name);
            return *this;
        }
        friend std::ostream& operator<< (std::ostream &out, const Furniture &f)
        {
            printf("Furniture{ m_name : \"%s\"; }", f.m_name.c_str());
            fflush(stdout);
            return out;
        }
};



static const string furnitureNames[] =
{
    "bed", "sofa", "table", "chair",
    "bookshelf", "wardrobe", "bath"
};


template<typename T>
static void statusCheck(T& arr, ARR_Error expectedError, size_t testNumber = 0)
{
    ARR_Error code = arr.status();
    if (code != expectedError)
    {
        printf("\n~~~Test %d failed~~~\n", testNumber);
        printf("Status: %s; Expected: %s\n", T::errToStr(code), T::errToStr(expectedError));
        arr.dump();
    }
    else
    {
        printf("Ok ");
    }
}

void arr_test()
{
    const int N_TEST = 100;

    srand(time(nullptr));

    try
    {
        Array<Furniture, 10> room;
        for (size_t i = 0; i < room.size(); i++)
        {
            room[i] = Furniture(furnitureNames[rand() % 7]);;
            room.updateHash();
        }

        statusCheck(room, ARR_Error::ARR_OK);

        //Первый тип тестов
        //изменяем один элемент в массиве
        Furniture* ptr = &room[0];
        for (size_t i = 0; i < N_TEST; i++)
        {
            size_t index = rand() % room.size();
            Furniture save = room[index];
            ptr[index] = Furniture("blah-blah-blah-blah");
            statusCheck(room, ARR_Error::ARR_WRONG_HASH, 1);
            room.updateHash(); // нужно обновить иначе мы упадем при вызове оператора []
            room[index] = save;
            room.updateHash(); // нужно обновить иначе упадем попозже...
            statusCheck(room, ARR_Error::ARR_OK, 1);
        }

        //Второй тип тестов
        //изменяем один элемент в массиве
        for (size_t i = 0; i < N_TEST; i++)
        {
            size_t index = rand() % room.size();
            Furniture save = room[index];
            memset((ptr + index), rand(), sizeof(Furniture));
            statusCheck(room, ARR_Error::ARR_WRONG_HASH, 2);
            memcpy((ptr + index), &save, sizeof(Furniture));
            room.updateHash(); // нужно обновить иначе упадем попозже...
            statusCheck(room, ARR_Error::ARR_OK, 2);
        }

        //Портим канарейки
        room.LEFT_CANARY = rand() & 0xFFFFFF;
        statusCheck(room, ARR_Error::ARR_ATTACK_ON_LEFT, 3);
        room.LEFT_CANARY = MAGIC_VALUE;

        room.RIGHT_CANARY = rand() & 0xFFFFFF;
        statusCheck(room, ARR_Error::ARR_ATTACK_ON_RIGHT, 3);
        room.RIGHT_CANARY = MAGIC_VALUE;


        //проверим как обрабатывается выход за пределы массива
        for (size_t i = 0; i < (N_TEST >> 2); i++)
        {
            try
            {
                size_t index = (rand() & 0xFF) + 15;
                room.at(index) = Furniture();
            }
            catch (const std::exception& e)
            {
                if(!strstr(e.what(), "Out of range."))
                {
                    printf("~~~ Test failed ~~~\n");
                    printf("Catched exception tells: %s\n", e.what());
                }
                else
                {
                    printf("Ok ");
                }
            }
        }

        room.dump();

    }
    catch (const std::exception& e)
    {
        printf("\n~~~ We cathced unexpected exception ~~~\n");
        printf("Exception.what() : %s\n", e.what());
    }

}