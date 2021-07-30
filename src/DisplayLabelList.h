#ifndef DISPLAYLABELLIST_H
#define DISPLAYLABELLIST_H


#include "LinkedList.h"
#include "DisplayLabel.h"

class DisplayLabelList : public LinkedList<DisplayLabel*> {

private:
    
    /**
     * @brief The cleanup function used by the list's deconstructor;
     * 
     */
    void destory();

public:
    bool add(DisplayLabel page);

    /**
     * @brief Searches for a button by the button text
     * 
     * @param text 
     * @return DisplayLabel* if a button was found, otherwise NULL
     */
    DisplayLabel *findButtonByText(String text);
    
    /**
     * @brief The count of items in the list
     * 
     * @return int 
     */
    int count() { return size(); };

    
    ~DisplayLabelList() { destory(); }
    
};

#endif