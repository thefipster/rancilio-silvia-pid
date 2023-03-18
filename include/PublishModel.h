#ifndef PUBLISHMODEL_H
#define PUBLISHMODEL_H

struct PublishModel
{
    double boilerTemp;
    double headTemp;
    double pidControl;
    bool heaterState;

    void copyFrom(PublishModel model)
    {
        boilerTemp = model.boilerTemp;
        headTemp = model.headTemp;
        pidControl = model.pidControl;
        heaterState = model.heaterState;
    }
};

#endif