/**
*  @file Coche.cpp
*  @brief Implementacion de la clase Coche.
*/

#include "Coche.h"

#include "PuntoRecarga.h"

Coche::Coche(std::string mat = "", std::string mar = "", std::string mod = "", float bat = 100.0)
    : id_matricula(mat), marca(mar), modelo(mod), nivelBateria(bat), puntoActual(nullptr) {}

std::string Coche::getId_matricula() const {
    return id_matricula;
}

std::string Coche::getMarca() const {
    return marca;
}

std::string Coche::getModelo() const {
    return modelo;
}

float Coche::getNivelBateria() const {
    return nivelBateria;
}

PuntoRecarga* Coche::getPuntoActual() const {
    return puntoActual;
}

void Coche::circular() {
    if (puntoActual) {
        puntoActual->delCoche(id_matricula);
        puntoActual = nullptr;
    }
    nivelBateria = std::max(0.0f, nivelBateria - 10.0f);
}

bool Coche::aparcar(PuntoRecarga *pr) {
    if (!pr) {
        return false;
    }
    if (puntoActual) {
        puntoActual->delCoche(id_matricula);
    }

    if (pr->addCoche(this)) {
        puntoActual = pr;
        float nuevaBateria = std::min(100.0f, nivelBateria + 20.0f);
        if (nuevaBateria != nivelBateria) {
            nivelBateria = nuevaBateria;
            pr->actualizarBateriaCoche(this);
        }
        return true;
    }
    return false;
}

void Coche::cargarBateria(float cantidad) {
    if (puntoActual) {
        nivelBateria = std::min(100.0f, nivelBateria + cantidad);
        puntoActual->actualizarBateriaCoche(this);
    }
}









