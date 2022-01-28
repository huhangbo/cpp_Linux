#include <iostream>

using namespace std;

int main() {
    cout << "Hello, World!" << std::endl;
    return 0;
}

class People {
    public:
        string name;
        int age;
    private:
        int num;
    protected:
        void call();
};

void People::call() {

}

class Dog: virtual public People {
    public:
        Dog(){ printf("Dog");}
        ~Dog(){ printf("~Dog");}
};