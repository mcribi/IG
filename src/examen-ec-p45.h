// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

#ifndef EXAMEN_EC_P45_HPP
#define EXAMEN_EC_P45_HPP
#include "malla-ind.h"
#include "grafo-escena.h"

class MallaP4: public MallaInd
{
    public:
        MallaP4();
};

class NodoP4: public NodoGrafoEscena {
   public:
      NodoP4();
};


class TrianguloN: public MallaInd
{
    public:
        TrianguloN(glm::vec3 ver1, glm::vec3 ver2, glm::vec3 ver3);
        bool cuandoClick(const glm::vec3 & centro_wc) override;
   protected: 
   //   glm::vec3 punto_central={0.0,0.0,0.0};
   int N=0; 
};

class NodoP5: public NodoGrafoEscena {
    public:
        NodoP5();
};



#endif 
