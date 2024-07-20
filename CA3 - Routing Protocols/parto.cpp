#include "parto.hpp"
#include<QRandomGenerator>

Parto::Parto(){
    x = 0;
    scale_parameter = 10;
    shape_parameter = 1;
}

Parto::Parto(double sp ,double shp){
    x = 0;
    scale_parameter = sp;
    shape_parameter = shp;
}


bool Parto::generate(){
    x += 1;
    double PDF = shape_parameter * (pow(scale_parameter, shape_parameter));
    PDF /= pow(x, (shape_parameter + 1));
    double random_number = QRandomGenerator::global()->generateDouble();
    if(random_number > PDF){
        return 1;
    } else {
        return 0;
    }
}

