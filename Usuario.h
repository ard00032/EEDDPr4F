#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>
#include <map>
#include "fecha.h"
#include "PuntoRecarga.h"

class Trayecto;
class Reanelcar;
class Coche;

class Usuario {
private:
    std::string nif;
    std::string clave;
    std::string nombre;
    std::string direccion;
    Reanelcar* linkReanel;
    std::multimap<Fecha,Trayecto*> ruta;

public:
    Usuario(std::string n, std::string c, std::string nom, std::string dir, Reanelcar* r);

    std::string getNif() const;
    std::string getClave() const;
    std::string getNombre() const;
    std::string getDireccion() const;
    void setLinkReanel(Reanelcar* r);

    Trayecto* crearTrayecto(int idPuntoInicio, int idPuntoFinal, Fecha fIni, Fecha fFin);
    bool iniciaViaje(Usuario* u, int idPuntoInicio, int idPuntoFinal, Fecha fIni, Fecha fFin);
    Coche* aparcaCoche(Coche* c, PuntoRecarga* pr);
    std::vector<Trayecto*> getTrayectosFecha(Fecha f);
};



#endif //USUARIO_H
