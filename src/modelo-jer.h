// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

#ifndef MODELO_JER_HPP
#define MODELO_JER_HPP

#include "grafo-escena.h"
#include "malla-ind.h"
#include "ig-aux.h"
#include "malla-revol.h"

using namespace std;
using namespace glm;

class CuboPuente : public MallaInd
{
   public:
      CuboPuente();
};

class MallaTorrePuente : public MallaInd
{
   public:
      MallaTorrePuente(unsigned n);
};

//Clase Cilindro
class CilindroPuente : public MallaRevol
{
   public:
   // Constructor: crea el perfil original y llama a inicializar
   // la base tiene el centro en el origen, el radio y la altura son 1
   CilindroPuente
   (
      const int num_verts_per, // número de vértices del perfil original (m)
      const unsigned nperfiles   // número de perfiles (n)
   ) ;
} ;

class PuenteElevadizo : public NodoGrafoEscena
{
   protected:
    //rotar cilindros
    mat4 * pm_rot_cilindro_izq = nullptr;
    mat4 * pm_rot_cilindro_der = nullptr;

    //subir/bajar rotar puente
    mat4 * pm_rot_puente_izq = nullptr; 
    mat4 * pm_rot_puente_der = nullptr;

    //cabina de seguridad que sube y baja
    mat4 * pm_tras_cabina = nullptr;

   public:
    PuenteElevadizo() ;
    unsigned leerNumParametros() const ;
    virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
    
} ;


#endif