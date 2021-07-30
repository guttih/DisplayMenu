#include "DisplayPageList.h"
bool DisplayPageList::add(DisplayPage page) {
    return LinkedList<DisplayPage*>::add(new DisplayPage(page));
}

void DisplayPageList::destory() {
    DisplayPage* p;
    for (int i = 0; i < size(); i++) {
        p = get(i);
        if (p != NULL)
        {
            delay(10);
            delete p;
            set(i, NULL);
        }
    }
    clear();
}