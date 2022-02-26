#ifndef MUGEN_CONTROL_H
#define MUGEN_CONTROL_H


class Control {
public:
    virtual ~Control(){}
    bool isHighlighted = false, isEditing = false, isChecked = false;
    virtual void activate(){}
    void switchHighlight(){
        isHighlighted = !isHighlighted;
    }
    virtual void increment(bool largeDecrement){}
    virtual void decrement(bool largeIncrement){}
    int borderSize = 2;
};


#endif //MUGEN_CONTROL_H
