///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJack.                                            //
//    Copyright (C) 2014-2015 Jacob Dawid <jacob@omg-it.works>               //
//                                                                           //
//    QJack is free software: you can redistribute it and/or modify          //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QJack is distributed in the hope that it will be useful,               //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QJack. If not, see <http://www.gnu.org/licenses/>.          //
//                                                                           //
//    It is possible to obtain a closed-source license of QJack.             //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// Own includes
#include "driver.h"

// JACK includes
#include <jack/control.h>

namespace QJack {

Driver::Driver(jackctl_driver_t *driver)
{
    _jackDriver = driver;
}

QString Driver::name() {
    if(!isValid()) {
        return QString();
    }

    return QString(jackctl_driver_get_name(_jackDriver));
}

Driver::DriverType Driver::type() {
    if(!isValid()) {
        return DriverTypeInvalid;
    }

    switch (jackctl_driver_get_type(_jackDriver)) {
    case JackMaster:
        return DriverTypeMaster;
    case JackSlave:
        return DriverTypeSlave;
    default:
        return DriverTypeInvalid;
    }
}

ParameterMap Driver::parameters() {
    if(!isValid()) {
        return ParameterMap();
    }

    ParameterMap parameterMap;
    const JSList *parameters = jackctl_driver_get_parameters(_jackDriver);
    while(parameters) {
        Parameter p = Parameter((jackctl_parameter_t*)parameters->data);
        parameterMap.insert(p.name(), p);
        parameters = parameters->next;
    }
    return parameterMap;
}

int Driver::parseParameters(int argc, char* argv[]) {
    if(!isValid()) {
        return -1;
    }

    return jackctl_driver_params_parse(_jackDriver, argc, argv);
}

}
