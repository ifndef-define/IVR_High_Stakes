#include <iostream>
#include <string>

/*
Print "Hello World!" diagonally to the terminal.

Expected Output:
H
 E
  L
   L
    O
      
      W
       O
        R
         L
          D
           !
*/

using namespace std;

int main() {
    
    string str = "Hello World!";
    int len = str.length();

    for(int i=0; i<len; i++) {
        
        for(int j=0; j<i; j++) {
            cout << " ";
        }
        cout << str[i] << endl;
    }

    return 0;
}   