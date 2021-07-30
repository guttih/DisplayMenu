#include "DisplayButtonList.h"
bool DisplayButtonList::add(DisplayButton button) {
    return LinkedList<DisplayButton*>::add(new DisplayButton(button));
}

DisplayButton *DisplayButtonList::findButtonByText(String text) {
    
    for (int i = 0; i < _size; i++)
    {
        DisplayButton *btn = get(i);
        if (btn->getText().equals(text))
            return btn;
    }
    return NULL;
}

void DisplayButtonList::destory() {
    DisplayButton* p;
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