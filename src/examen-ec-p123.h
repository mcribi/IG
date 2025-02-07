// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X
#ifndef EXAMEN_EC_P123_HPP
#define EXAMEN_EC_P123_HPP

#include "grafo-escena.h"
#include "malla-ind.h"

using namespace std;
using namespace glm;

class  P1Malla : public MallaInd
{
   public:
      P1Malla();
};

class  P2MallaTriangulada : public MallaInd
{
   public:
      P2MallaTriangulada(unsigned n);
};

class GrafoCubosVertical : public NodoGrafoEscena
{
   protected:
    //cada cubo rota entorno al eje que pasa por su centro y origen
    glm::mat4 * pm_rot_cubo2 = nullptr;
    glm::mat4 * pm_tras_vert = nullptr;
    glm::mat4 * pm_altura_patas = nullptr; 
    float alpha=0.0f;

   public:
    unsigned leerNumParametros() const ;
    void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
    GrafoCubosVertical(unsigned h, unsigned w) ;
    
} ;

#endif
