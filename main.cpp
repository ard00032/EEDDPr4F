#include "Reanelcar.h"
#include "PuntoRecarga.h"
#include "Coche.h"
#include <iostream>
#include <string>
#include <iomanip>

void distribuirCoches(Reanelcar& sistema) {
    std::vector<Coche*> cochesDisp;
    std::cout << "\nRecopilando coches disponibles..." << std::endl;

    // Recopilar todos los coches del sistema
    for (auto it = sistema.coches.begin(); it != sistema.coches.end(); ++it) {
        if (it->second != nullptr) {
            cochesDisp.push_back(it->second);
            std::cout << "Coche añadido a disponibles: " << it->first << std::endl;
        }
    }

    std::cout << "Total de coches disponibles: " << cochesDisp.size() << std::endl;

    // Primera pasada: distribuir un coche a cada punto vacío
    std::cout << "\nPrimera distribución - un coche por punto vacío:" << std::endl;
    for (int id = 1; id <= 50 && !cochesDisp.empty(); id++) {
        PuntoRecarga* punto = sistema.buscarPunto(id);
        if (punto && punto->getNumCoches() == 0) {
            Coche* coche = cochesDisp.back();
            if (sistema.colocarCochePR(coche, punto)) {
                std::cout << "Coche " << coche->getId_matricula()
                    << " asignado al punto de recarga " << id
                    << " (Primer coche)" << std::endl;
                cochesDisp.pop_back();
            } else {
                std::cout << "Error al colocar el coche " << coche->getId_matricula()
                    << " en el punto " << id << std::endl;
            }
        }
    }

    // Segunda pasada: distribuir los coches restantes
    std::cout << "\nSegunda distribución - coches restantes:" << std::endl;
    int puntoActual = 0;
    while (!cochesDisp.empty()) {
        puntoActual++;
        if (puntoActual > 50) {
            puntoActual = 1;
        }

        PuntoRecarga* punto = sistema.buscarPunto(puntoActual);
        if (punto) {
            std::cout << "Verificando punto " << puntoActual
                << " (Ocupación: " << punto->getNumCoches()
                << "/" << punto->getMax() << ")" << std::endl;

            if (punto->getNumCoches() < punto->getMax()) {
                Coche* coche = cochesDisp.back();
                if (sistema.colocarCochePR(coche, punto)) {
                    std::cout << "Coche " << coche->getId_matricula()
                        << " asignado al punto de recarga " << puntoActual << std::endl;
                    cochesDisp.pop_back();
                } else {
                    std::cout << "Error al colocar el coche " << coche->getId_matricula()
                        << " en el punto " << puntoActual << std::endl;
                }
            }
        }
    }

    std::cout << "\nDistribución finalizada. Verificando ocupación final:" << std::endl;
    for (int id = 1; id <= 50; id++) {
        PuntoRecarga* punto = sistema.buscarPunto(id);
        if (punto) {
            std::cout << "Punto " << id << ": "
                << punto->getNumCoches() << "/"
                << punto->getMax() << " coches" << std::endl;
        }
    }
}

void mostrarEstadisticasDistribucion(Reanelcar& sistema) {
    std::cout << "\n=== ESTADISTICAS DE DISTRIBUCION ===" << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;

    int totalCoches = 0;
    int puntosVacios = 0;
    int puntosLlenos = 0;

    // Revisar cada punto de recarga
    for (int id = 1; id <= 50; id++) {
        PuntoRecarga* punto = sistema.buscarPunto(id);
        if (punto) {
            int numCoches = punto->getNumCoches();
            int maxCoches = punto->getMax();
            totalCoches += numCoches;

            std::cout << "Punto " << std::setfill(' ') << std::setw(2) << id
                     << ": " << std::setw(2) << numCoches << "/"
                     << std::setw(2) << maxCoches << " coches";

            if (numCoches == 0) {
                std::cout << " [VACIO]";
                puntosVacios++;
            } else if (numCoches == maxCoches) {
                std::cout << " [LLENO]";
                puntosLlenos++;
            }

            std::cout << std::endl;
        }
    }

    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
    std::cout << "Total de coches distribuidos: " << totalCoches << std::endl;
    std::cout << "Puntos de recarga vacios: " << puntosVacios << std::endl;
    std::cout << "Puntos de recarga llenos: " << puntosLlenos << std::endl;
    std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
}

int main() {
    Reanelcar sistema;

    std::cout << "Cargando datos del sistema..." << std::endl;

    sistema.cargarUsuarios("../usuarios1.csv");
    std::cout << "Usuarios cargados correctamente." << std::endl;
    sistema.cargarCoches("../coches_v2.csv");
    std::cout << "Coches cargados correctamente." << std::endl;
    sistema.cargarPuntosRecarga("../puntos_recarga.csv");
    std::cout << "Puntos de recarga cargados correctamente." << std::endl;

    std::cout << "\nIniciando distribucion de coches..." << std::endl;
    distribuirCoches(sistema);
    std::cout << "Distribucion de coches completada." << std::endl;
    mostrarEstadisticasDistribucion(sistema);
    return 0;
}
