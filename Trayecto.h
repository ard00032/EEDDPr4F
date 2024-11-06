#ifndef TRAYECTO_H
#define TRAYECTO_H

#include "fecha.h"
#include "Coche.h"

class Trayecto {
private:
    int idTrayecto;
    Fecha fechaInicio;
    Fecha fechaFinal;
    Coche* coche;
    PuntoRecarga* origen;
    PuntoRecarga* destino;

public:
    Trayecto(int id, Fecha inicio, Fecha final);

    void setCoche(Coche* c);
    void setOrigen(PuntoRecarga* orig);
    void setDestino(PuntoRecarga* dest);

    int getIdTrayecto() const;
    Fecha getFechaInicio() const;
    Fecha getFechaFinal() const;
    Coche* getCoche() const;
    PuntoRecarga* getOrigen() const;
    PuntoRecarga* getDestino() const;
};



#endif //TRAYECTO_H
