#ifndef DISPLAYBUTTONLIST_H
#define DISPLAYBUTTONLIST_H


#include "LinkedList.h"
#include "DisplayButton.h"

class DisplayButtonList : public LinkedList<DisplayButton*> {

private:
    
    /**
     * @brief The cleanup function used by the list's deconstructor;
     * 
     */
    void destory();

public:
    bool add(DisplayButton page);

    /**
     * @brief Searches for a button by the button text
     * 
     * @param text 
     * @return DisplayButton* if a button was found, otherwise NULL
     */
    DisplayButton *findButtonByText(String text);
    
    /**
     * @brief The count of items in the list
     * 
     * @return int 
     */
    int count() { return size(); };

    
    ~DisplayButtonList() { destory(); }
    
};

#endif