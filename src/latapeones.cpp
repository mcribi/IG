// Nombre: María,  Apellidos: Cribillés Pérez, Titulación: GIM
// email: mariacribilles@correo.ugr.es, DNI o pasaporte: 75573410X

#include "latapeones.h"

using namespace std;
using namespace glm;

Peon::Peon(int nperfiles) {
    pm_tras = leerPtrMatriz(agregar(translate(vec3(0.0)))); //trasladamos el peon al origen
    // agregar(new MallaRevolPLY("../materiales/plys/peon.ply", nperfiles)); //creamos el peon de revolucion a partir del archivo ply
    agregar(new MallaRevolPLY("peon.ply", nperfiles));
}

bool Peon::cuandoClick(const glm::vec3 & centro_oc) {
    *pm_tras = translate(vec3(0.0, 0.0, 1.0));
    return true;
}

Lata::Lata(const std::string & nombre_arch) { //parametro: archivo con la textura
    NodoGrafoEscena* total = new NodoGrafoEscena(); //donde juntaremos la tres partes

    //la lata se divide en tres partes: base + parte central + tapa
    unsigned identTapa = 0;
    unsigned identBase = 1;
    unsigned identCara = 2;

    TexturaXY* cocacola = new TexturaXY(nombre_arch); //definimos la textura que va a ser la de la foto de cocacola
    Material* materialLata = new Material(cocacola, 0.5, 0.6, 0.5, 50.0);
    Material* materialBases = new Material(0.35, 0.45, 0.25, 50.0);

    /* Parámetros material: 
        textura  = nullptr ; //aqui en materialLata le hemos puesto el de cocacola
        k_amb    = p_k_amb ; //coeficiente de componente ambiental: la luz ambiental es una iluminación uniforme que afecta toda la escena de manera igualitaria, simulando luz reflejada indirectamente
        k_dif    = p_k_dif ; //coeficiente de componente difusa: depende de la orientacion del material con respecto a la fuente de luz. Simula cómo la luz se dispersa uniformemente al golpear la superficie.
        k_pse    = p_k_pse ; //Coeficiente de componente especular. La luz especular simula los reflejos brillantes de las superficies, dependiendo de la posición de la cámara y la fuente de luz.
        exp_pse  = p_exp_pse ; //Exponente especular. Valor alto: reflejos pequeños y nitidos (metal o plastico brillante) y valores bajos: reflejos grandes y difusos (superficies rugosas)
    */

    //creamos las tres partes de la lata
    NodoGrafoEscena* base = new NodoGrafoEscena();
    NodoGrafoEscena* parte_central = new NodoGrafoEscena();
    NodoGrafoEscena* tapa = new NodoGrafoEscena();

    //añadir a la base
    base->agregar(materialBases); //a la base le añadimos el material
    //base->agregar(new MallaRevolPLY("../materiales/plys/lata-pinf.ply", 32)); //creamos una malla de revolucion a partir del archivo ply
    base->agregar(new MallaRevolPLY("lata-pinf.ply", 32));

    //añadir a la tapa
    tapa->agregar(materialBases); //material
    //tapa->agregar(new MallaRevolPLY("../materiales/plys/lata-psup.ply", 32)); //cuerpo de revolucion
    tapa->agregar(new MallaRevolPLY("lata-psup.ply", 32));

    //añadir a la parte central
    parte_central->agregar(materialLata); //material
    // parte_central->agregar(new MallaRevolPLY("../materiales/plys/lata-pcue.ply", 32)); //cuerpo de revolucion
    parte_central->agregar(new MallaRevolPLY("lata-pcue.ply", 32));

    //añadimos cada parte al total
    total->agregar(base);
    total->agregar(parte_central);
    total->agregar(tapa);

    agregar(total);
}

LataPeones::LataPeones() { //constructor de un objeto con una lata y tres peones
    NodoGrafoEscena* actual = new NodoGrafoEscena(); //objeto total

    //3 peones y una lata
    unsigned identPeonMadera = 1;
    unsigned identPeonBlanco = 2;
    unsigned identPeonNegro = 3;
    unsigned identLata = 4;

    //TexturaXY* texturaXY_madera = new TexturaXY("../materiales/imgs/text-madera.jpg");
    TexturaXY* texturaXY_madera = new TexturaXY("text-madera.jpg");
    Material* materialPeonMadera = new Material(texturaXY_madera, 0.5, 0.6, 0.5, 50.0);
    Material* materialPeonBlanco = new Material(0.5, 0.2, 0.5, 5.0);
    Material* materialPeonNegro = new Material(0.01, 0.2, 0.5, 50.0);

    //organizamos peones
    NodoGrafoEscena* peones = new NodoGrafoEscena();
    peones->agregar(translate(vec3(0.0, 0.4, 0.7)));
    peones->agregar(scale(vec3(0.25, 0.25, 0.25)));
    
    //creamos los 3 peones
    Peon* peonMadera = new Peon(32);
    Peon* peonNegro = new Peon(32);
    Peon* peonBlanco = new Peon(32);

    //creamos la lata
    //Lata* lata = new Lata("../materiales/imgs/lata-coke.jpg");
    Lata* lata = new Lata("lata-coke.jpg");
    lata->ponerNombre("Lata de Coca-Cola");
    lata->ponerIdentificador(identLata);

    peonMadera->ponerNombre("Peón de madera");
    peonMadera->ponerIdentificador(identPeonMadera);
    peones->agregar(materialPeonMadera);
    peones->agregar(peonMadera);

    peonNegro->ponerNombre("Peón Negro");
    peonNegro->ponerIdentificador(identPeonNegro);
    peones->agregar(translate(vec3(2.5, 0.0, 0.0)));
    peones->agregar(materialPeonNegro);
    peones->agregar(peonNegro);

    peonBlanco->ponerNombre("Peón blanco");
    peonBlanco->ponerIdentificador(identPeonBlanco);
    peones->agregar(translate(vec3(2.5, 0.0, 0.0)));
    peones->agregar(materialPeonBlanco);
    peones->agregar(peonBlanco);


    actual->agregar(peones);

    actual->agregar(lata);

    agregar(actual);
}


VariasLatasPeones::VariasLatasPeones() {
    unsigned identPepsi = 5;
    unsigned identUGR = 6;

    NodoGrafoEscena* lataPepsi = new NodoGrafoEscena();
    NodoGrafoEscena* lataUGR = new NodoGrafoEscena();

    lataPepsi->ponerNombre("Lata de Pepsi");
    lataPepsi->ponerIdentificador(identPepsi);
    lataPepsi->agregar(translate(vec3(1.0, 0.0, 0.0)));
    lataPepsi->agregar(new Lata("lata-pepsi.jpg"));

    lataUGR->ponerNombre("Lata de la UGR");
    lataUGR->ponerIdentificador(identUGR);
    lataUGR->agregar(translate(vec3(2.0, 0.0, 0.0)));
    lataUGR->agregar(new Lata("window-icon.jpg"));

    agregar(lataPepsi);
    agregar(lataUGR);
}
