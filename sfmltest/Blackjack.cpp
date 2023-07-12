#include "Menu.h"
#include <ctime>

using namespace std;

int main() {
    srand(time(NULL));

    Menu menu;
    while (true) {
        menu.display();
    }
       
}
