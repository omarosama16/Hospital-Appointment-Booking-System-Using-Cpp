#include <iostream>
#include <cassert>
using namespace std;
int add(int a, int b) {
    return a + b;
}
bool isEven(int x) {
    return x % 2 == 0;
}
int main() {

    assert(add(2, 3) == 5);
    assert(add(0, 0) == 0);
    assert(add(-1, 1) == 0);

    assert(isEven(2) == true);
    assert(isEven(3) == false);

    cout << "All tests passed successfully!" << endl;

    return 0;
}
