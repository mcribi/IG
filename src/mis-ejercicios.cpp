// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

#include "grafo-escena.h"
#include "malla-ind.h"
#include "ig-aux.h"
#include "malla-revol.h"
#include "modelo-jer.h"
#include "mis-ejercicios.h"


using namespace std;
using namespace glm;

P1MallaCubo::P1MallaCubo()
:  MallaInd("cubo 8 vétices a color")
{
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         { +0.0, +1.0, +0.0}, // 8
         { +0.0, -1.0, +0.0}, //9
      } ;


    //tiene un triangulo mas en dos de sus caras
   triangulos =
      {  {3,2,8},{2,8,6},{8,6,7},{3,8,7},{0,1,9},{0,4,9},{1,5,9},{9,4,5},
      {0,1,2},{1,3,2},{0,2,6},{0,6,4},{5,6,4},{5,7,6},{1,3,7},{1,7,5},
      } ;

    col_ver = {
      {0.0, 0.0, 0.0}, // 0
      {0.0, 0.0, 1.0}, // 1
      {0.0, 1.0, 0.0}, // 2
      {0.0, 1.0, 1.0}, // 3
      {1.0, 0.0, 0.0}, // 4
      {1.0, 0.0, 1.0}, // 5
      {1.0, 1.0, 0.0}, // 6
      {1.0, 1.0, 1.0}, // 7
      {1.0, 0.0, 0.0},
      {0.0, 1.0, 1.0},
   }; 
}

P1MallaPiramide::P1MallaPiramide()
:  MallaInd( "pirámide con base cuadrada" )
{

   vertices =
      {  { 0.0, 0.0, 0.0 }, // 0
         { -1.0, 0.0, -1.0 }, // 1
         { -1.0, 0.0, 1.0 }, // 2
         { 1.0, 0.0, -1.0 }, // 3
         { 1.0, 0.0, 1.0 }, // 4
         { 0.0, 2.0, 0.0 }, // 5
      } ;


   triangulos =
      {  {0,2,1}, {0,4,2},
         {0,3,4}, {0,1,3},

         {1,2,5}, {2,4,5},
         {3,5,4}, {1,3,5}
      } ;

   col_ver.push_back({0.6, 0.0, 0.1});
   for (unsigned i=1; i<5; i++) {
      col_ver.push_back({1.0, 1.0, 0.0});
   }
   col_ver.push_back({0.0, 1.0, 1.0});

}

P2Barrido::P2Barrido(
      const int m,      // número de vértices del perfil original (m)
      const unsigned n  // número de perfiles (n)
   )
:  MallaInd()
{
    //crea un semicirculo en el perfil en el plano xy
   vector<glm::vec3>  perfil;
   for (float i=0; i<m; i++) {
      float angulo = i*M_PI/(m-1);//semicirculo
      perfil.push_back({sin(angulo), cos(angulo), 0.0});
   }//se meten de arriba a abajo y de izqiuerda a derecha
   
   double desp = 2.0/m;
   for (unsigned i=0; i<n; i++) {
      for (unsigned j=0; j<m; j++) {
         vertices.push_back({perfil[j].x, perfil[j].y, desp*i*(-1)});
      }
   }
    

   for (unsigned i=0; i<n-1; i++) {
      for (unsigned j=0; j<m-1; j++) {
         int k = i*m+j; //indice actual
         triangulos.push_back({k, k+m, k+m+1});
         triangulos.push_back({k, k+m+1, k+1});
      }
   }

    //para cerrar la costura del perfil en el eje y (tie)
    //cada perfil tiene m vertices
    //primer vertice del perfil actual:i*m
    //ultimo vertices del perfil actual: i*m+m-1
    //primer vertice del siguiente perfil: (i+1)*m
    //ultimo vertice del siguiente perfil: (i+1)*m+m-1
   for (unsigned i=0; i<n-1; i++) {
        triangulos.push_back({i*m+m-1, i*m, i*m+m});
        triangulos.push_back({i*m+m-1, i*m+m+m-1, i*m+m});
   }
}

P2Rejilla::P2Rejilla(unsigned n, unsigned m){   //esta mal 
   assert(n > 1 && m > 1);
   float escala_n = 1.0f/(n-1);
   float escala_m = 1.0f/(m-1);
   for(int i=0; i<n; i++){
      for(int j=0; j<m; j++){
         vertices.push_back({i*escala_n,0.0f,j*escala_m});
      }
   }
   for(int i=0; i<n-1; i++){
      for(int j=0; j<m-1; j++){
         triangulos.push_back({i*m+j,i*m+j+1,(i+1)*m+j});
         triangulos.push_back({i*m+j+1,(i+1)*m+j+1,(i+1)*m+j});
      }
   }
   for(int i=0;i<vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }
}

Estrella::Estrella(unsigned n)
{
    unsigned ind_rot_estrella= agregar(rotate(float(0.0), vec3(1.0,0.0,0.0)));
    agregar(rotate(float(M_PI/2.0), vec3(0.0,1.0,0.0)));

    NodoGrafoEscena *cono = new NodoGrafoEscena();
    
    cono->agregar(translate( vec3(1.3,0.0,0.0)));
    cono->agregar(rotate(float(-M_PI/2), vec3(0.0,0.0,1.0)));
    cono->agregar(scale(vec3(0.14/2.0,0.15/3.0,0.14/2.0)));
    cono->agregar(new Cono(2,10));
    agregar(cono);

    
    for (int i=0; i<n-1; i++){
      agregar(rotate(float(2*M_PI/n),vec3{ 0.0,0.0,1.0 }) );
      //agregar(translate(vec3(sin(float(M_PI/n)),cos(float(M_PI/n)),0.0)));
      agregar(cono);
    }

    agregar(scale(vec3(1.3/0.5,1.3/0.5,1.3/0.5)));
    //agregar(rotate(float(M_PI/2),vec3(0.0,1.0,0.0)));
    agregar(translate(vec3(-0.5,-0.5,0.0)));
    agregar(new EstrellaZ(n));

    

    pm_rot_estrella = leerPtrMatriz(ind_rot_estrella);
}

unsigned Estrella::leerNumParametros() const {
    return 1;
}

void Estrella::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
   alpha = 2*M_PI*2.5*t_sec;

   *pm_rot_estrella = (rotate(radians(alpha), vec3{ 1.0,0.0,0.0 }));

}

P3Cuadrado::P3Cuadrado()
{
    vertices={{-1.0,0.0,-1.0},{-1.0,0.0,+1.0},{+1.0,0.0,-1.0},{+1.0,0.0,+1.0}};
    triangulos={{0,1,3},{0,3,2}};
}

EsferaMovil::EsferaMovil(const int num_verts_per, const unsigned nperfiles, float radio){
    vector<vec3> perfil = vector<vec3>();
    float perimetro = 2*M_PI*radio;
    float division_perimtero = perimetro/(num_verts_per-1);
    for(int i=0; i<num_verts_per; i++){
        perfil.push_back(vec3(radio*cos(i*division_perimtero),radio*sin(i*division_perimtero),0.0));
    }
    inicializar(perfil,nperfiles);
}

P3Caja::P3Caja()
{
    NodoGrafoEscena *circulo = new NodoGrafoEscena();
    NodoGrafoEscena *puerta = new NodoGrafoEscena();
    NodoGrafoEscena *p3caja = new NodoGrafoEscena();

    
    p3caja->agregar(rotate(float(M_PI/2),vec3(0.0,0.0,1.0)));
    p3caja->agregar(translate(vec3(1.0,1.0,0.0)));
    p3caja->agregar(new P3Cuadrado());
    

    agregar(new P3Cuadrado());
    agregar(p3caja);
    agregar(rotate(float(M_PI/2),vec3(0.0,1.0,0.0)));
    agregar(p3caja);
    agregar(rotate(float(M_PI/2),vec3(0.0,1.0,0.0)));
    agregar(p3caja);
    agregar(rotate(float(M_PI/2),vec3(0.0,1.0,0.0)));
    agregar(p3caja);
    

    unsigned indice_circulo = circulo->agregar(translate(vec3(0.0,0.0,0.0)));
    circulo->agregar(translate(vec3(0.0,0.35,0.0)));
    circulo->agregar(scale(vec3(0.25,0.25,0.25)));
    circulo->agregar(new EsferaMovil(20,20,1.0));

    unsigned indice_puerta = puerta->agregar(rotate(float(0.0),vec3(1.0,0.0,0.0)));
    puerta->agregar(translate(vec3(0.0,2.0,0.0)));
    puerta->agregar(new P3Cuadrado());

    agregar(puerta);
    agregar(circulo);
    pm_bola = circulo->leerPtrMatriz(indice_circulo);
    pm_puerta = puerta->leerPtrMatriz(indice_puerta);


}

unsigned P3Caja::leerNumParametros() const{
    return 2;
}

void P3Caja::actualizarEstadoParametro(const unsigned iParam, const float t_sec){
    switch(iParam){
        case 0:
            *pm_bola = translate(vec3(0.0,0.5*sin(2*M_PI*t_sec)+0.4,0.0));
        case 1:
            *pm_puerta = rotate(float(M_PI/2*sin(2*M_PI*t_sec)),(vec3(1.0,0.0,0.0)));
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
P3Mesa::P3Mesa()
:  NodoGrafoEscena()
{  
    NodoGrafoEscena * cubo = new NodoGrafoEscena();
    cubo->agregar(translate(vec3(1.0,1.0,1.0)));
    cubo->agregar(new Cubo());

    NodoGrafoEscena * patas = new NodoGrafoEscena();
    patas->agregar(translate(vec3(0.8,0.0,0.8)));
    patas->agregar(scale(vec3(0.1,0.5,0.1)));

    unsigned ind_altura_patas = patas->agregar(scale(vec3(1.0,1.0,1.0)));

    patas->agregar(cubo);
    patas->agregar(translate(vec3(-18.0,0.0,0.0)));
    patas->agregar(cubo);
    patas->agregar(translate(vec3(0.0,0.0,-18.0)));
    patas->agregar(cubo);
    patas->agregar(translate(vec3(18.0,0.0,0.0)));
    patas->agregar(cubo);

    agregar(patas);

    unsigned ind_tras_vert = agregar(translate(vec3(0.0,0.0,0.0)));

    agregar(translate(vec3(-1.0,1.0,-1.0)));
    agregar(scale(vec3(1.0,0.1,1.0)));
    agregar(cubo);

    agregar(scale(vec3(1.0,10.0,1.0)));
    agregar(translate(vec3(0.9,0.2,0.9)));
    agregar(scale(vec3(0.1,0.3,0.1)));
    agregar(cubo);

    unsigned ind_rot1 = agregar(rotate(0.0f, vec3(0.0,0.0,1.0)));

    agregar(translate(vec3(0.0,2.0,0.0)));
    agregar(cubo);

    unsigned ind_rot2 = agregar(rotate(0.0f, vec3(0.0,0.0,1.0)));

    agregar(translate(vec3(0.0,2.0,0.0)));
    agregar(cubo);

    pm_altura_patas = patas->leerPtrMatriz(ind_altura_patas);
    pm_tras_vert = leerPtrMatriz(ind_tras_vert);
    pm_rot1 = leerPtrMatriz(ind_rot1);
    pm_rot2 = leerPtrMatriz(ind_rot2);
}

unsigned P3Mesa::leerNumParametros() const {
    return 2;
};

void P3Mesa::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
    float v;
    switch (iParam)
    {
    case 0:
        v = 1.0+0.5*sin(M_PI*(1/1.3)*t_sec);
        *pm_altura_patas = scale(vec3(1.0,v,1.0));
        *pm_tras_vert = translate(vec3(0.0,v-1,0.0));
        break;
    case 1:
        *pm_rot1 =  (translate(vec3(1.0,2.0,0.0))) * 
                    (rotate(float(2*M_PI*0.3)*t_sec, vec3(0.0,0.0,1.0))) *
                    (translate(vec3(-1.0,-2.0,0.0)));
        *pm_rot2 =  (translate(vec3(1.0,2.0,0.0))) * 
                    (rotate(float(2*M_PI*0.3)*t_sec, vec3(0.0,0.0,-1.0))) *
                    (translate(vec3(-1.0,-2.0,0.0)));
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MiMesa::MiMesa()
:  NodoGrafoEscena()
{  
    NodoGrafoEscena *pata = new NodoGrafoEscena();
    
    
    pata->agregar(translate(vec3(1.0-0.1,0.6,1.0-0.1)));
    pata->agregar(scale(vec3(0.1,0.5,0.1)));
    pata->agregar(new Cubo());
    agregar(pata);

    
    NodoGrafoEscena *tabla = new NodoGrafoEscena();

    tabla->agregar(translate(vec3(0.0,1.0,0.0)));
    tabla->agregar(scale(vec3(1.0,0.1,1.0)));
    tabla->agregar(new Cubo());
    agregar(tabla);

    agregar(translate(vec3(0.0,0.0,-2.0+0.2)));
    agregar(pata);

    agregar(translate(vec3(-2.0+0.2,0.0,0.0)));
    agregar(pata);

    agregar(translate(vec3(0.0,0.0,2.0-0.2)));
    agregar(pata);

    
}

unsigned MiMesa::leerNumParametros() const {
    return 2;
};

void MiMesa::actualizarEstadoParametro(const unsigned iParam, const float t_sec) {
    float v;
    switch (iParam)
    {
    case 0: //seno->valor oscilante, 0.5 es la amplitud de oscilacion (ira desde 1.5 a 0.5 las patas)
        v = 1.0+0.5*sin(M_PI*(1/1.3)*t_sec);    //el +1 eleva la altura, el 1/1.3 es lo rapido que cambia en el tiempo
        *pm_altura_patas = scale(vec3(1.0,v,1.0));
        *pm_tras_vert = translate(vec3(0.0,v-1,0.0));
        break;
    case 1:
        //Mueve el punto de referencia desde el origen (0, 0, 0) a la posición (1.0, 2.0, 0.0).
        //Esto establece el centro de rotación para la animación.
        *pm_rot1 =  (translate(vec3(1.0,2.0,0.0))) * 
                    (rotate(float(2*M_PI*0.3)*t_sec, vec3(0.0,0.0,1.0))) * //0.3 es la velocidad angular
                    (translate(vec3(-1.0,-2.0,0.0)));
        *pm_rot2 =  (translate(vec3(1.0,2.0,0.0))) * //lo mismo pero en sentido contrario
                    (rotate(float(2*M_PI*0.3)*t_sec, vec3(0.0,0.0,-1.0))) *
                    (translate(vec3(-1.0,-2.0,0.0)));
        break;
    default:
        break;
    }
}

