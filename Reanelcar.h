#ifndef REANELCAR_H
#define REANELCAR_H

#include <list>
#include <map>
#include <string>
#include <vector>
#include "Usuario.h"

class Coche;
class PuntoRecarga;

class Reanelcar {
private:
    std::list<Usuario*> usuarios;
    std::vector<PuntoRecarga*> sitios;
public:
    std::map<std::string,Coche*> coches;
    Reanelcar();
    Usuario* buscarUsrNif(std::string nif);
    std::vector<Usuario*> buscarUsrNomb(std::string nom);
    Coche* buscarCochMat(std::string mat);
    std::vector<Coche*> buscarCochModelo(std::string mod);
    PuntoRecarga* buscarPunto(int id);
    Coche* alquilar(Usuario* u, int idPROrigen, int idPRDestino, Fecha fIni, Fecha fFin);
    bool colocarCochePR(Coche* c, PuntoRecarga* pr);
    std::vector<PuntoRecarga*> obtenerPRmenosCoches();
    void cargarUsuarios(const std::string& nombreArchivo);
    void cargarCoches(const std::string& nombreArchivo);
    void cargarPuntosRecarga(const std::string& nombreArchivo);
    ~Reanelcar();
};



#endif //REANELCAR_H
