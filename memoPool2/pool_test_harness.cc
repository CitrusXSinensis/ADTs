#include "pool.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    Pool *pools[4];
    pools[0] = pools[1] = pools[2] = pools[3] = nullptr;
    char cmd, lhs;
    size_t length;
    Pool p;
    Pool *pp;
    Chunk c;
    while(cin >> cmd) {
        switch(cmd) {
            case 'a':
                cin >> lhs;
                cin >> length;
                addChunk(*pools[lhs-'a'], length);
                break;
            case 'p' :
                cin >> lhs;
                printPool(cout, *pools[lhs-'a']);
                cout << endl;
                break;
            case 'r' :
                cin >> lhs;
                freePool(pools[lhs-'a']);
                delete pools[lhs-'a'];
                p = readPool();
                pp = new Pool;
                pp->chunks = p.chunks;
                pp->capacity = p.capacity;
                pp->size = p.size;
                pools[lhs-'a'] = pp;
                break;
            case 'c' :
                cin >> lhs;
                cout << pools[lhs-'a']->size << "/" \
                     << pools[lhs-'a']->capacity << endl;
                break;
            case 'f' :
                cin >> lhs;
                cin >> length;
                c = findFreeMem(*pools[lhs-'a'], length);
                if (c.length) {
                    if (c.length < length) {
                            cout << "Invalid chunk." << endl;
                    } else {
                        for (size_t i = 0; i < length; ++i) {
                            c.mem[i] = 42;
                        }
                    }
                    cout << "Chunk: " << c.length << endl;
                    delete[] c.mem;
                } else { cout << "Nullptr" << endl;}
                break;
            default:
                cerr << "Invalid command: " << cmd << \
                     ". This means your test case is invalid!" << endl;
                break;
        }
    }
    for (size_t i = 0; i < 4; ++i) {
        if (pools[i]) {
            freePool(pools[i]);
            delete pools[i];
        }
    }
}
