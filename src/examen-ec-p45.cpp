// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

#include "examen-ec-p45.h"

/// //EJERCICIO 4
MallaP4::MallaP4():MallaInd("MallaP4")
{
    vertices =
      {  
         { 1.0, 2.0, 1.0 }, // 0 pico
         { 0.0, 0.0, 0.0 }, // 1
         { 2.0, 0.0, 0.0 }, // 2

         { 1.0, 2.0, 1.0 }, // 3 pico
         { 2.0, 0.0, 0.0 }, // 4
         { 2.0, 0.0, 2.0 }, //5

         { 1.0, 2.0, 1.0 }, // 6 pico
         { 2.0, 0.0, 2.0 }, // 7
         { 0.0, 0.0, 2.0 }, // 8

         { 1.0, 2.0, 1.0 }, // 9 pico
         { 0.0, 0.0, 2.0 }, // 10
         { 0.0, 0.0, 0.0 }, // 11

         //base: 

         { 0.0, 0.0, 0.0 }, // 12
         { 2.0, 0.0, 0.0 }, // 13
         { 2.0, 0.0, 2.0 }, // 14
         { 0.0, 0.0, 2.0 } // 15




      } ;


   triangulos =
      {  
         {0, 1,2}, {3,4,5}, 
         {6,7,8}, {9,10,11},

         {12,13,14}, {12,14,15}
      } ;

   cc_tt_ver = { 
      { 0.5, 1.0-1.0 }, // pico 0
      { 0.0, 1.0-0.0}, //  1
      { 1.0, 1.0-0.0 }, // 2
      
      { 0.5, 1.0-1.0 }, // 3 pico
      { 0.0, 1.0-0.0}, //  4
      { 1.0, 1.0-0.0 }, // 5
      
      { 0.5, 1.0-1.0 }, // 6 pico
      { 0.0, 1.0-0.0}, //  7
      { 1.0, 1.0-0.0 }, // 8
      
      { 0.5, 1.0-1.0 },// 9 pico
      { 0.0, 1.0-0.0}, //  10
      { 1.0, 1.0-0.0 }, // 11
      

      //para la base: 
      { 0.0, 1.0-0.0 },// 12
      { 1.0, 1.0-0.0}, //  13
      { 1.0, 1.0-1.0 }, // 14
      { 0.0, 1.0-1.0 } // 15


   };  

   calcularNormales();
}

NodoP4::NodoP4(){
   agregar( new Material( new Textura("window-icon.jpg") , 0.7, 0.5, 0.0, 30.0) );
   agregar( new MallaP4() );
}

//////////////////////////////// EJERICCIO 5 ////////////////////////////////
TrianguloN::TrianguloN(glm::vec3 ver1, glm::vec3 ver2, glm::vec3 ver3):MallaInd("MallaP5")
{
   ponerColor(glm::vec3(1.0,1.0,1.0));
   vertices={ver1,ver2,ver3}; //son los vertices pasados por parametro

   triangulos =
      {  
         {0, 1, 2}
      } ;
      
   calcularNormales();

   glm::vec3 punto_central;
   punto_central.x=(ver1.x+ver2.x+ver3.x)/3.0f;
   punto_central.y=(ver1.y+ver2.y+ver3.y)/3.0f;
   punto_central.z=(ver1.z+ver2.z+ver3.z)/3.0f;

   if (punto_central.x==1 &&  punto_central.y==2.0f/3.0f && punto_central.z==1.0f/3.0f)
      N=1;
   else if (punto_central.x==5/3 &&  punto_central.y==2.0f/3.0f && punto_central.z==1)
      N=2;
   else if (punto_central.x==1 &&  punto_central.y==2.0f/3.0f && punto_central.z==5.0f/3.0f)
      N=3;
   else if (punto_central.x==1.0f/3.0f &&  punto_central.y==2.0f/3.0f && punto_central.z==1)
      N=4;
}

bool TrianguloN::cuandoClick(const glm::vec3 & centro_wc) {
   std::cout << "Se ha seleccionado el triangulo " << N+1 << std::endl;

   if (leerColor() == glm::vec3(1.0,0.0,0.0))//si esta rojo lo pongo a blanco
      ponerColor({1.0,1.0,1.0});
   else
      ponerColor({1.0,0.0,0.0});

   return true; 
}


NodoP5::NodoP5(){
   NodoGrafoEscena *piramide = new NodoGrafoEscena();

   TrianguloN *triangulo1 = new TrianguloN({1.0,2.0,1.0},{0.0,0.0,0.0},{2.0,0.0,0.0});
   triangulo1->ponerIdentificador(1);
   piramide->agregar(triangulo1);

   TrianguloN *triangulo2 = new TrianguloN({1.0,2.0,1.0},{2.0,0.0,0.0},{2.0,0.0,2.0});
   triangulo2->ponerIdentificador(2);
   piramide->agregar(triangulo2);

   TrianguloN *triangulo3 = new TrianguloN({1.0,2.0,1.0},{2.0,0.0,2.0},{0.0,0.0,2.0});
   triangulo3->ponerIdentificador(3);
   piramide->agregar(triangulo3);

   TrianguloN *triangulo4 = new TrianguloN({1.0,2.0,1.0},{0.0,0.0,2.0},{0.0,0.0,0.0});
   triangulo4->ponerIdentificador(4);
   piramide->agregar(triangulo4);
    
   agregar(piramide);
}
