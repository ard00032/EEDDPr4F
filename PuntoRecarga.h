/**
 * @file PuntoRecarga.h
 * @brief Clase que representa un punto de recarga para vehiculos electricos
 */

#ifndef PUNTORECARGA_H
#define PUNTORECARGA_H

#include <string>
#include <map>
#include "UTM.h"

class Coche;

class PuntoRecarga {
private:
  int id;
  UTM posicion;
  int max;
  std::multimap<float,Coche*> coches;

public:
  PuntoRecarga(int id, UTM pos, int maxCoches);

  int getId() const;
  UTM getPosicion() const;
  int getMax() const;

  Coche getMaxBateria() const;
  bool addCoche(Coche* c);
  bool delCoche(std::string idMatricula);
  int getNumCoches() const;
  void actualizarBateriaCoche(Coche* c);
};



#endif //PUNTORECARGA_H
