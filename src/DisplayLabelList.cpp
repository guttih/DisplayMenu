#include "DisplayLabelList.h"
bool DisplayLabelList::add(DisplayLabel button) {
    return LinkedList<DisplayLabel*>::add(new DisplayLabel(button));
}

DisplayLabel *DisplayLabelList::findButtonByText(String text) {
    
    for (int i = 0; i < _size; i++)
    {
        DisplayLabel *btn = get(i);
        if (btn->getText().equals(text))
            return btn;
    }
    return NULL;
}

void DisplayLabelList::destory() {
    DisplayLabel* p;
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