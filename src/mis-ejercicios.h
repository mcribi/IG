// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

#ifndef MIS_EJERCICIOS_HPP
#define MIS_EJERCICIOS_HPP

#include "grafo-escena.h"
#include "malla-ind.h"
#include "ig-aux.h"
#include "malla-revol.h"

using namespace std;
using namespace glm;

class P1MallaCubo : public MallaInd
{
   public:
      P1MallaCubo();
};

class P2Rejilla : public MallaCuadrado
{
    public:
        P2Rejilla(unsigned n, unsigned m);
};

class P3Cuadrado : public MallaInd
{
    public:
        P3Cuadrado();
};

class EsferaMovil : public MallaRevol {
    public:
        EsferaMovil(const int num_verts_per, const unsigned nperfiles, float radio);
};

class P3Caja : public NodoGrafoEscena
{
    protected:
        glm::mat4 *pm_puerta=nullptr;
        glm::mat4 *pm_bola=nullptr;
    public:
        unsigned leerNumParametros() const;
        virtual void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
        P3Caja();
};

class P1MallaPiramide : public MallaInd
{
   public:
      P1MallaPiramide();
};

class P2Barrido : public MallaInd
{
   public:
      P2Barrido
   (
      const int m,      // número de vértices del perfil original
      const unsigned n  // número de perfiles
   ) ;
};


class P3Mesa : public NodoGrafoEscena
{
   protected:
      glm::mat4 * pm_tras_vert = nullptr;
      glm::mat4 * pm_altura_patas = nullptr;
      glm::mat4 * pm_rot1 = nullptr;
      glm::mat4 * pm_rot2 = nullptr;

   public:
      P3Mesa();
      unsigned leerNumParametros() const ;
      void actualizarEstadoParametro(const unsigned iParam, const float t_sec);

};

class MiMesa : public NodoGrafoEscena
{
   protected:
      glm::mat4 * pm_tras_vert = nullptr;
      glm::mat4 * pm_altura_patas = nullptr;
      glm::mat4 * pm_rot1 = nullptr;
      glm::mat4 * pm_rot2 = nullptr;

   public:
      MiMesa();
      unsigned leerNumParametros() const ;
      void actualizarEstadoParametro(const unsigned iParam, const float t_sec);

};

class Estrella : public NodoGrafoEscena
{
   protected:
    //rotar  entorno al eje X (entorno al centro de la estrella)
    glm::mat4 * pm_rot_estrella = nullptr;
    unsigned n;
    float alpha=0.0f; 

   public:
    unsigned leerNumParametros() const ;
    void actualizarEstadoParametro(const unsigned iParam, const float t_sec);
    Estrella(unsigned n) ;
    
} ;

#endif
