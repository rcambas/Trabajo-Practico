#include <iostream>
#include <cstdio>
using namespace std;

// TP - Octagon Pro

//Falta terminar el maincard, lo hice literalmente como pide la consigna (5 COMBATIENTES no combates, pero no tiene sentido)
//Falta hacer las categorías de pesos y ordenar el top 10 en base a eso
//Al menos como lo pensé yo, el programa carga automáticamente los datos del archivo al iniciar (si es que existe dicho archivo)

struct Atleta
{
    int ID;
    char nombre[30];
    char apodo[30];
    float peso;
    int victorias;
    int derrotas;
};

struct Nodo
{
    Atleta dato;
    Nodo *siguiente;
};

void mostrarTOP10 (Nodo *);
void inscribirAtleta (Nodo *&);
void generarMainCard (Nodo *);
void actualizarRecord (Nodo *);
void guardarGimnasio(Nodo *);
void cargarGimnasio(Nodo *&);
void liberarMemoria(Nodo *&);
int contarAtletas(Nodo *); //Extra para la funcion generarMaincard, solo cuenta cuantos atletas hay

int main() {
    Nodo* lista =NULL;
    int opcion;

    cargarGimnasio(lista);

    do {
        cout <<endl<< " -[- OCTAGON PRO -]-" <<endl;       //Iba a hacer esta parte en una funcion llamada menu, pero ya que estamos mejor acá

        cout << "----------------------------------------" <<endl;
        cout <<"1. Inscribir un atleta" <<endl;
        cout <<"2. Mostrar el MainCard" <<endl;
        cout <<"3. Actualizar victorias o derrotas" <<endl;
        cout <<"4. Ver el TOP 10" <<endl;
        cout <<"5. Guardar todo y salir" <<endl;
        cout <<"--------------------------------------" <<endl;
        cout <<"Ingrese una opcion: " <<endl;
        cin >>opcion;
        cout <<endl;

        switch (opcion) 
        {
            case 1:
                inscribirAtleta(lista);
                break;
            case 2:
                generarMainCard(lista);
                break;
            case 3:
                actualizarRecord(lista);
                break;
            case 4:
                mostrarTOP10(lista);
                break;
            case 5:
                cout <<"Adios!" <<endl;
                break;
        }
    } while (opcion != 5);

    guardarGimnasio(lista);                             //Todos los cambios se guardan en el archivo atletas.dat :P
    liberarMemoria(lista);                              //Liberar memoria!

    return 0;
}

void mostrarTOP10(Nodo* lista){                               //Lo hice por puntaje, todavía no hice la división por categorías de peso

    if (lista == NULL) {
        cout << "No hay atletas. No hay ranking disponible." << endl;
        return;
    }

    Atleta top10[10]; 
    int cantidad = 0;
    Nodo* aux = lista;

    while (aux != NULL) {
        int puntaje = aux->dato.victorias - aux->dato.derrotas;
        int posicion = 0;

        while (posicion < cantidad && (top10[posicion].victorias - top10[posicion].derrotas) >= puntaje) 
        {
            posicion++;
        }

        if (posicion < 10) 
        {
            int limite = (cantidad < 10) ? cantidad : 9;    
            for (int i = limite; i > posicion; i--) {
                top10[i] = top10[i - 1];
            }
            
            top10[posicion] = aux->dato;                 // Insertamos al nuevo integrante del Top 10 en su posicion ganada
            
            if (cantidad < 10) {                          // Si todavia no llenamos los 10 lugares, aumentamos el contador
                cantidad++;
            }
        }
        aux = aux->siguiente;
    }

    cout << "-----------------------------------------" << endl;
    cout << "              RANKING TOP 10             " << endl;
    cout << "-----------------------------------------" << endl;
    for (int i = 0; i < cantidad; i++) 
    {
        cout << i+1 << ". " << top10[i].nombre << ". Alias " << top10[i].apodo <<endl;
        cout <<"Peso: " << top10[i].peso << "kg  ---  ";
        cout <<"Vic.: " << top10[i].victorias << " - Der.: " << top10[i].derrotas <<endl;
        cout <<"Puntaje: " << top10[i].victorias - top10[i].derrotas <<endl;
        cout << "-----------------------------------------" << endl;
    }
}

void inscribirAtleta (Nodo *&lista){
    Nodo* nuevoatleta = new Nodo();

    Nodo *aux1 = lista;
    Nodo *aux2;
 
    cout <<" Ingrese los datos del nuevo atleta -->" <<endl;
    cout <<"ID: "; cin >>nuevoatleta->dato.ID;
    cin.ignore();                                                    //Esto me lo corrigió Gemini jahsdas, por el tema del enter y el getline la verdad ni idea eso
    cout <<"Nombre: "; cin.getline (nuevoatleta->dato.nombre,30);
    cout <<"Apodo: "; cin.getline (nuevoatleta->dato.apodo,30);
    cout <<"Peso: "; cin >>nuevoatleta->dato.peso;
    cout <<"Victorias: "; cin >>nuevoatleta->dato.victorias;
    cout <<"Derrotas: "; cin >>nuevoatleta->dato.derrotas;

    cout <<"Atleta inscrito correctamente! " <<endl;

    while (aux1 != NULL && aux1->dato.peso < nuevoatleta->dato.peso)
    {
        aux2 = aux1;
        aux1 = aux1->siguiente;
    }

    if (aux1 == lista)
    {  
        lista = nuevoatleta;
    } else{
        aux2->siguiente = nuevoatleta;
    }
    
    nuevoatleta->siguiente = aux1;
}

int contarAtletas(Nodo *lista){          //Contamos la cantidad de atletas para evitar que
    int contador = 0;                    //generarMainCard funcione si hay menos de 5 atletas
    Nodo* aux = lista;                   //No se especifica que tenga que ser así, simplemente
    while (aux != NULL)                  //yo lo interpreté así
    {
        contador++;
        aux = aux->siguiente;
    }
    return contador;
}

void generarMainCard(Nodo* lista){

    int total = contarAtletas(lista);                //Esta parte se explica sola
    if (total == 0)
    {
        cout << "La lista de atletas esta vacia." <<endl;
        return;
    } else if (total < 5)
    {
        cout << "Solo hay " << total << " atletas inscriptos" <<endl;
        cout << "Se requieren al menos 5 para anotar a los atletas estelares" <<endl;
        return;
    }

    Atleta atletasEstelares[5]; 
    Nodo* aux = lista;

    cout <<"Maincard generada!" <<endl;

    for (int i=0; i<5; i++)          //Listado de los 5 atletas estelares
    {
        atletasEstelares[i] = aux->dato; 
        cout <<"Atleta nro " << i+1 <<": " <<atletasEstelares[i].nombre <<endl;
        aux = aux->siguiente;
    }
}


void actualizarRecord(Nodo* lista){
    int buscarID;
    bool coincidencia = 0;
    Nodo* aux = lista;

    cout <<"Ingrese el ID del atleta a modificar: ";
    cin >>buscarID;

    while (aux != NULL && !coincidencia) 
    {
        if (aux->dato.ID == buscarID) 
        {
            coincidencia =true;
            int eleccion = 0;
            
            cout <<"Se encontro al atleta -> " << aux->dato.nombre <<" con ID " << buscarID <<endl;
            cout <<"Victorias actuales: " << aux->dato.victorias <<endl;
            cout <<"Derrotas actuales: " << aux->dato.derrotas <<endl;

            do                                               //No sé si esto cuenta como APB y si está bien o mal, sino se saca el do while y el else, y dejamos los if's
            {
                cout <<"Desea modificar (1)Victorias o (2)Derrotas: " <<endl;
                cin  >>eleccion;
            if (eleccion == 1)
            {
                cout <<"Ingrese nuevas Victorias: ";
                cin >> aux->dato.victorias;
            } else if (eleccion == 2)
            {
                cout <<"Ingrese nuevas Derrotas: ";
                cin >> aux->dato.derrotas;
            } else {
                cout <<"Ingrese un numero valido ";
            } 
            } while (eleccion != 1 && eleccion != 2);
            
            cout <<"Datos actualizados" <<endl;
        }
        aux = aux->siguiente; 
    }

    if (!coincidencia) {
        cout <<"No hay ningun atleta con dicho ID o es incorrecto" <<endl;
    }
}

void guardarGimnasio(Nodo* lista) {
    Nodo* aux = lista; 

    FILE* archivo = fopen ("atletas.dat", "wb");
    if (archivo == NULL)
    {
        cout <<"Error, no se pudo crear el archivo" <<endl;
        return;
    }

    while (aux != NULL) 
    {
        fwrite(&aux->dato, sizeof(Atleta), 1, archivo);
        aux = aux->siguiente;
    }

    fclose(archivo);
    cout <<"Se guardaron los datos correctamente en 'atletas.dat'" <<endl;
}

void cargarGimnasio(Nodo*& lista) {
    FILE* archivo = fopen ("atletas.dat", "rb");
    Atleta auxAtleta;
    
    if (archivo == NULL) 
    {
        cout <<"(Aviso, aun no existe el archivo)" <<endl;
        return;
    }

    while (fread(&auxAtleta, sizeof(Atleta), 1, archivo) == 1) 
    {
        Nodo* nuevo = new Nodo();
        nuevo->dato = auxAtleta;
        nuevo->siguiente = NULL;
        Nodo *aux1 = lista;
        Nodo *aux2 = NULL;

        while (aux1 != NULL && aux1->dato.peso < nuevo->dato.peso) {
            aux2 = aux1;
            aux1 = aux1->siguiente;
        }

        if (aux1 == lista) {
            lista = nuevo;
        } else {
            aux2->siguiente = nuevo;
        }
        nuevo->siguiente = aux1;
    }

    fclose(archivo);
    cout << "Se cargaron los datos de los atletas" <<endl;
}

void liberarMemoria(Nodo *&lista) {                  //Esto fue recomendación de Gemini, para liberar memoria. Sinceramente ni idea jahdsad
    Nodo* aux;
    while (lista != NULL) {
        aux = lista;
        lista = lista->siguiente;
        delete aux;
    }
