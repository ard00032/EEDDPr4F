/**
*  @file PuntoRecarga.cpp
*  @brief Implementacion de la clase PuntoRecarga.
*/

#include "PuntoRecarga.h"

#include <algorithm>
#include <stdexcept>
#include <bits/ranges_algo.h>

#include "Coche.h"

PuntoRecarga::PuntoRecarga(int id, UTM pos, int maxCoches)
    : id(id), posicion(pos), max(maxCoches) {}

int PuntoRecarga::getId() const {
    return id;
}

UTM PuntoRecarga::getPosicion() const {
    return posicion;
}

int PuntoRecarga::getMax() const {
    return max;
}

Coche PuntoRecarga::getMaxBateria() const {
    if (coches.empty()) {
        throw std::runtime_error("No hay coches en el punto de recarga");
    }
    auto maxCoche = coches.rbegin();
    return *(maxCoche->second);
}

bool PuntoRecarga::addCoche(Coche *c) {
    if (!c || coches.size() >= max) {
        return false;
    }
    for (const auto& par : coches) {
        if (par.second->getId_matricula() == c->getId_matricula()) {
            return false;
        }
    }
    coches.insert({c->getNivelBateria(),c});
    return true;
}

bool PuntoRecarga::delCoche(std::string idMatricula) {
    for (auto it = coches.begin(); it != coches.end(); it++) {
        if (it->second->getId_matricula() == idMatricula) {
            coches.erase(it);
            return true;
        }
    }
    return false;
}

int PuntoRecarga::getNumCoches() const {
    return coches.size();
}

void PuntoRecarga::actualizarBateriaCoche(Coche *c) {
    delCoche(c->getId_matricula());
    addCoche(c);
}












