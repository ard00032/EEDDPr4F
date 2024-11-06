/**
* @file Usuario.cpp
* @brief Implementacion de la clase Usuario.
*/

#include "Usuario.h"
#include "Reanelcar.h"
#include "Trayecto.h"
#include "PuntoRecarga.h"

Usuario::Usuario(std::string n, std::string c, std::string nom, std::string dir, Reanelcar* r = nullptr)
    : nif(n), clave(c), nombre(nom), direccion(dir), linkReanel(r) {}

std::string Usuario::getNif() const {
    return nif;
}

std::string Usuario::getClave() const {
    return clave;
}

std::string Usuario::getNombre() const {
    return nombre;
}

std::string Usuario::getDireccion() const {
    return direccion;
}

void Usuario::setLinkReanel(Reanelcar *r) {
    linkReanel = r;
}


Trayecto *Usuario::crearTrayecto(int idPuntoInicio, int idPuntoFinal, Fecha fIni, Fecha fFin) {
    if (!linkReanel) {
        return nullptr;
    }
    PuntoRecarga* origen = linkReanel->buscarPunto(idPuntoInicio);
    PuntoRecarga* destino = linkReanel->buscarPunto(idPuntoFinal);
    if (!origen || !destino) {
        return nullptr;
    }

    Trayecto* nuevoTrayecto = new Trayecto(ruta.size() + 1, fIni, fFin);
    nuevoTrayecto->setOrigen(origen);
    nuevoTrayecto->setDestino(destino);
    ruta.insert({fIni, nuevoTrayecto});

    return nuevoTrayecto;
}

bool Usuario::iniciaViaje(Usuario *u, int idPuntoInicio, int idPuntoFinal, Fecha fIni, Fecha fFin) {
    if (!linkReanel || !u) {
        return false;
    }
    Coche* cocheAlquilado = linkReanel->alquilar(u, idPuntoInicio, idPuntoFinal, fIni, fFin);
    if (!cocheAlquilado) {
        return false;
    }

    Trayecto* nuevoTrayecto = crearTrayecto(idPuntoInicio, idPuntoFinal, fIni, fFin);
    if (!nuevoTrayecto) {
        PuntoRecarga* origen = linkReanel->buscarPunto(idPuntoInicio);
        if (origen) {
            linkReanel->colocarCochePR(cocheAlquilado, origen);
        }
        return false;
    }
    nuevoTrayecto->setCoche(cocheAlquilado);
    return true;
}

Coche *Usuario::aparcaCoche(Coche *c, PuntoRecarga *pr) {
    if (!c || !pr || !linkReanel) {
        return nullptr;
    }
    if (linkReanel->colocarCochePR(c, pr)) {
        for (auto it = ruta.rbegin(); it != ruta.rend(); it++) {
            Trayecto* trayecto = it->second;
            if (trayecto && trayecto->getCoche() == c) {
                trayecto->setCoche(nullptr);
                break;
            }
        }
        return c;
    }
    return nullptr;
}

std::vector<Trayecto *> Usuario::getTrayectosFecha(Fecha f) {
    std::vector<Trayecto*> trayectosFecha;
    auto range = ruta.equal_range(f);
    for (auto it = range.first; it != range.second; it++) {
        trayectosFecha.push_back(it->second);
    }
    return trayectosFecha;
}












