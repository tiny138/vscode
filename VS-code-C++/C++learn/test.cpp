#include<iostream>

using namespace std;


class Animal {
public:
    Animal() {
        std::cout << "Animal() is called" << std::endl;
    }

    Animal(const Animal &) {
        std::cout << "Animal (const Animal &) is called" << std::endl;
    }

    Animal &operator=(const Animal &) {
        std::cout << "Animal & operator=(const Animal &) is called" << std::endl;
        return *this;
    }

    ~Animal() {
        std::cout << "~Animal() is called" << std::endl;
    }
};

class Dog {
public:
    Dog(const Animal &animal) : __animal(animal) {
        std::cout << "Dog(const Animal &animal) is called" << std::endl;
    }

    ~Dog() {
        std::cout << "~Dog() is called" << std::endl;
    }

private:
    Animal __animal;
};

int main() {
    Animal animal;
    std::cout << std::endl;
    Dog d(animal);
    std::cout << std::endl;
    return 0;
}