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


void Parto::generate(){
    x += 1;
    double PDF = shape_parameter * (pow(scale_parameter, shape_parameter));
    PDF /= pow(x, (shape_parameter + 1));
    double random_number = QRandomGenerator::global()->generateDouble();
    if(random_number > PDF){
        //emit paretoDistribution::create_packet();
    } else {

    }
}

