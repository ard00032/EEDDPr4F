/**
*  @file Reanelcar.cpp
*  @brief Implementacion de la clase Reanelcar para gestionar los objetos Coche y Usuario.
*/

#include "Reanelcar.h"
#include "Usuario.h"
#include "PuntoRecarga.h"
#include "Coche.h"

#include <algorithm>
#include <limits>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>

Reanelcar::Reanelcar() {}

Usuario *Reanelcar::buscarUsrNif(std::string nif) {
    auto it = std::find_if(usuarios.begin(), usuarios.end(),
        [&nif](const Usuario* u) {
            return u->getNif() == nif;
        });
    return it != usuarios.end() ? *it : nullptr;
}

std::vector<Usuario *> Reanelcar::buscarUsrNomb(std::string nom) {
    std::vector<Usuario*> resultado;
    for (auto usuario : usuarios) {
        if (usuario->getNombre() == nom) {
            resultado.push_back(usuario);
        }
    }
    return resultado;
}

Coche *Reanelcar::buscarCochMat(std::string mat) {
    auto it = coches.find(mat);
    return it != coches.end() ? it->second : nullptr;
}

std::vector<Coche *> Reanelcar::buscarCochModelo(std::string mod) {
    std::vector<Coche*> resultado;
    for (const auto& par : coches) {
        if (par.second->getModelo() == mod) {
            resultado.push_back(par.second);
        }
    }
    return resultado;
}

PuntoRecarga *Reanelcar::buscarPunto(int id) {
    auto it = std::find_if(sitios.begin(), sitios.end(),
        [id](const PuntoRecarga* p) {
            return p->getId() == id;
        });
    return it != sitios.end() ? *it : nullptr;
}

Coche *Reanelcar::alquilar(Usuario *u, int idPROrigen, int idPRDestino, Fecha fIni, Fecha fFin) {
    if (!u) {
        return nullptr;
    }
    PuntoRecarga* origen = buscarPunto(idPROrigen);
    PuntoRecarga* destino = buscarPunto(idPRDestino);

    if (!origen || !destino) {
        return nullptr;
    }

    try {
        Coche maxBateriaCoche = origen->getMaxBateria();
        Coche* cocheAlquilar = buscarCochMat(maxBateriaCoche.getId_matricula());
        if (!cocheAlquilar) {
            return nullptr;
        }
        origen->delCoche(cocheAlquilar->getId_matricula());
        if (!u->iniciaViaje(u,idPROrigen,idPRDestino, fIni, fFin)) {
            origen->addCoche(cocheAlquilar);
            return nullptr;
        }
        return cocheAlquilar;
    }catch (const std::runtime_error& e) {
        return nullptr;
    }
}

bool Reanelcar::colocarCochePR(Coche* c, PuntoRecarga* pr) {
    if (!c || !pr) {
        std::cerr << "Error: Coche o punto de recarga nulo" << std::endl;
        return false;
    }

    // Verificar si el coche ya está asignado a otro punto de recarga
    if (c->getPuntoActual() != nullptr) {
        c->getPuntoActual()->delCoche(c->getId_matricula());
    }

    if (pr->addCoche(c)) {
        bool resultado = c->aparcar(pr);
        if (resultado) {
            std::cout << "Coche colocado exitosamente" << std::endl;
        } else {
            std::cout << "Error al aparcar el coche" << std::endl;
        }
        return resultado;
    }

    std::cout << "No se pudo añadir el coche al punto de recarga" << std::endl;
    return false;
}

std::vector<PuntoRecarga *> Reanelcar::obtenerPRmenosCoches() {
    if (sitios.empty()) {
        return std::vector<PuntoRecarga *>();
    }
    int minCoches = std::numeric_limits<int>::max();
    for (const auto& pr : sitios) {
        if (pr->getNumCoches() < minCoches) {
            minCoches = pr->getNumCoches();
        }
    }
    std::vector<PuntoRecarga *> resultado;
    for (const auto& pr : sitios) {
        if (pr->getNumCoches() == minCoches) {
            resultado.push_back(pr);
        }
    }
    return resultado;
}

void Reanelcar::cargarUsuarios(const std::string &nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            // Leer línea completa
            std::istringstream ss(linea);
            std::string nif, clave, nombre, direccion;

            // Leer los datos del usuario
            if (std::getline(ss, nif, ',') &&
                std::getline(ss, clave, ',') &&
                std::getline(ss, nombre, ',') &&
                std::getline(ss, direccion)) {
                // Limpiar espacios en blanco al final de las cadenas
                nif.erase(nif.find_last_not_of(" \n\r\t") + 1);
                clave.erase(clave.find_last_not_of(" \n\r\t") + 1);
                nombre.erase(nombre.find_last_not_of(" \n\r\t") + 1);
                direccion.erase(direccion.find_last_not_of(" \n\r\t") + 1);

                try {
                    // Crear nuevo usuario y añadirlo a la lista
                    Usuario* nuevoUsuario = new Usuario(nif,clave,nombre,direccion,this);
                    if (buscarUsrNif(nif) != nullptr) {
                        delete nuevoUsuario;
                        std::cerr << "Ya existe un usuario con NIF: " << nif << std::endl;
                        continue;
                    }
                    usuarios.push_back(nuevoUsuario);
                }catch (const std::exception& e) {
                    std::cerr << "Error al crear usuario: " << e.what() << std::endl;
                }
                }else {
                    std::cerr << "Error al leer la linea: " << linea << std::endl;
                }
        }
        archivo.close();
    }else {
        std::cerr << "Error al abrir el archivo de usuarios." << std::endl;
    }
}

void Reanelcar::cargarCoches(const std::string &nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        std::string linea;
        // Saltar la primera línea (encabezados)
        std::getline(archivo, linea);

        while (std::getline(archivo, linea)) {
            // Primero, vamos a eliminar cualquier espacio en blanco al final de la línea
            linea.erase(std::remove(linea.begin(), linea.end(), '\r'), linea.end());
            linea.erase(std::remove(linea.begin(), linea.end(), '\n'), linea.end());

            // Dividir la línea manualmente
            size_t pos = 0;
            std::vector<std::string> tokens;
            std::string token;
            while ((pos = linea.find(",")) != std::string::npos) {
                token = linea.substr(0, pos);
                // Eliminar espacios en blanco al inicio y final del token
                token.erase(0, token.find_first_not_of(" \t"));
                token.erase(token.find_last_not_of(" \t") + 1);
                tokens.push_back(token);
                linea.erase(0, pos + 1);
            }
            // Añadir el último token (nivel de batería)
            linea.erase(0, linea.find_first_not_of(" \t"));
            linea.erase(linea.find_last_not_of(" \t") + 1);
            tokens.push_back(linea);

            // Verificar que tenemos los 4 campos necesarios
            if (tokens.size() == 4) {
                std::string matricula = tokens[0];
                std::string marca = tokens[1];
                std::string modelo = tokens[2];
                std::string bateria_str = tokens[3];

                try {
                    // Verificar si ya existe un coche con esa matrícula
                    if (buscarCochMat(matricula) != nullptr) {
                        std::cerr << "Ya existe un coche con matricula: " << matricula << std::endl;
                        continue;
                    }

                    // Convertir el string de batería a float
                    float nivelBateria = std::stof(bateria_str);

                    // Crear nuevo coche y añadirlo al map
                    Coche* nuevoCoche = new Coche(matricula, marca, modelo, nivelBateria);
                    coches[matricula] = nuevoCoche;

                } catch (const std::exception& e) {
                    std::cerr << "Error al procesar coche [" << matricula << "]: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Formato incorrecto en la línea. Tokens encontrados: " << tokens.size() << std::endl;
                for (size_t i = 0; i < tokens.size(); i++) {
                    std::cerr << "Token " << i << ": '" << tokens[i] << "'" << std::endl;
                }
            }
        }
        archivo.close();
    } else {
        std::cerr << "Error al abrir el archivo de coches." << std::endl;
    }
}

void Reanelcar::cargarPuntosRecarga(const std::string &nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        std::string linea;
        // Saltar la primera línea (encabezados)
        std::getline(archivo, linea);

        while (std::getline(archivo, linea)) {
            std::istringstream ss(linea);
            std::string id_str, lat_str, lon_str, max_str;

            // Leer los datos del punto de recarga
            if (std::getline(ss, id_str, ',') &&
                std::getline(ss, lat_str, ',') &&
                std::getline(ss, lon_str, ',') &&
                std::getline(ss, max_str)) {

                try {
                    // Limpiar espacios en blanco
                    id_str.erase(id_str.find_last_not_of(" \n\r\t") + 1);
                    lat_str.erase(lat_str.find_last_not_of(" \n\r\t") + 1);
                    lon_str.erase(lon_str.find_last_not_of(" \n\r\t") + 1);
                    max_str.erase(max_str.find_last_not_of(" \n\r\t") + 1);

                    // Convertir strings a los tipos apropiados
                    int id = std::stoi(id_str);
                    double latitud = std::stod(lat_str);
                    double longitud = std::stod(lon_str);
                    int max_coches = std::stoi(max_str) * 2;

                    // Verificar si ya existe un punto con ese ID
                    if (buscarPunto(id) != nullptr) {
                        std::cerr << "Ya existe un punto de recarga con ID: " << id << std::endl;
                        continue;
                    }

                    // Crear el punto UTM y el punto de recarga
                    UTM posicion(latitud, longitud);
                    PuntoRecarga* nuevoPunto = new PuntoRecarga(id, posicion, max_coches);
                    sitios.push_back(nuevoPunto);

                } catch (const std::exception& e) {
                    std::cerr << "Error al crear punto de recarga: " << e.what() << std::endl;
                }
                } else {
                    std::cerr << "Error al leer la línea: " << linea << std::endl;
                }
        }
        archivo.close();
    } else {
        std::cerr << "Error al abrir el archivo de puntos de recarga." << std::endl;
    }
}


Reanelcar::~Reanelcar() {
    for (Usuario* usuario : usuarios) {
        delete usuario;
    }
    usuarios.clear();

    for (auto& par : coches) {
        delete par.second;
    }
    coches.clear();

    for (PuntoRecarga* punto : sitios) {
        delete punto;
    }
    sitios.clear();
}
















