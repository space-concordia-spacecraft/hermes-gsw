#ifndef GSW_MODULES_H
#define GSW_MODULES_H

class UIWindowTemplate {
public:
    virtual void RenderGUI() = 0;

    virtual UIWindowTemplate *GetPtr() = 0;
};

#endif //GSW_MODULES_H
