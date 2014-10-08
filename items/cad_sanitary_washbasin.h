#ifndef CAD_SANITARY_WASHBASIN_H
#define CAD_SANITARY_WASHBASIN_H

#include "caditem.h"

class CAD_sanitary_washBasin : public CADitem
{
public:
    CAD_sanitary_washBasin();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_WASHBASIN_H
