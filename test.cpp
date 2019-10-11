#include <iostream>
#include <set>
#include <string>

using namespace std;

int main()
{
    set<string> s1;
    set<string>::iterator it;
    
    s1.insert("Bart");
    s1.insert("Anna");
    s1.insert("Carlos");
    s1.insert("Anna"); // program does not crash...has no effect
    
    cout << "s1 has " << s1.size() << " elements:" << endl;
    for (it=s1.begin(); it != s1.end(); ++it){
      *it = "hi";
    }
    for (it=s1.begin(); it != s1.end(); ++it)
    {
        cout << *it << endl;
    }

    s1.erase("Bart");
    cout << "s1 has " << s1.size() << " elements:" << endl;
    for (it=s1.begin(); it != s1.end(); ++it)
    {
        cout << *it << endl;
    }
    
    
    return 0;
}

