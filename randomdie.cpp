#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(NULL));
    std::cout << "Indovina il numero casuale tra 1 e 6 \n";
    int guess=-1;
    int secretnumber=rand()%6+1;
    while (guess!=secretnumber) {
        std::cin>>guess;
        if (guess!=secretnumber) std::cout<<"Sbagilato! Ritenta\n";
    }
    std::cout<<"Hai vinto!\n";
    return 0;

}
