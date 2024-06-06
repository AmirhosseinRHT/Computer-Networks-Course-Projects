#ifndef PARTO_HPP
#define PARTO_HPP

class Parto
{
public:
    Parto();
    Parto(double sp , double shp);
    bool generate();

private:
    int x;
    double scale_parameter;
    double shape_parameter;
};

#endif // PARTO_HPP
