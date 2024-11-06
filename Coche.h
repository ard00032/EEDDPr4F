/**
*  @file Coche.h
*  @brief Definicion de la clase Coche que representa un coche electrico de alquiler.
*/

#ifndef COCHE_H
#define COCHE_H

#include <iostream>
#include <string>


class PuntoRecarga;

class Coche {
private:
    std::string id_matricula;
    std::string marca;
    std::string modelo;
    float nivelBateria;
    PuntoRecarga* puntoActual;

public:

    Coche(std::string mat, std::string mar, std::string mod, float bat);

    std::string getId_matricula() const;
    std::string getMarca() const;
    std::string getModelo() const;
    float getNivelBateria() const;
    PuntoRecarga* getPuntoActual() const;

    void circular();
    bool aparcar(PuntoRecarga* pr);
    void cargarBateria(float cantidad);
};



#endif //COCHE_H
