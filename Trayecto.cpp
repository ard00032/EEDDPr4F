#include "Trayecto.h"

Trayecto::Trayecto(int id = 0, Fecha inicio = Fecha(), Fecha final = Fecha())
    : idTrayecto(id), fechaInicio(inicio), fechaFinal(final),
      coche(nullptr), origen(nullptr), destino(nullptr) {}

void Trayecto::setCoche(Coche *c) {
    coche = c;
}

void Trayecto::setOrigen(PuntoRecarga *orig) {
    origen = orig;
}

void Trayecto::setDestino(PuntoRecarga *dest) {
    destino = dest;
}

int Trayecto::getIdTrayecto() const{
    return idTrayecto;
}

Fecha Trayecto::getFechaInicio() const {
    return fechaInicio;
}

Fecha Trayecto::getFechaFinal() const {
    return fechaFinal;
}

Coche* Trayecto::getCoche() const {
    return coche;
}

PuntoRecarga* Trayecto::getOrigen() const {
    return origen;
}

PuntoRecarga* Trayecto::getDestino() const {
    return destino;
}

