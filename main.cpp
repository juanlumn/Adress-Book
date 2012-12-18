#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

class Fecha{
private:
    string fecha;
    int dia;
    int mes;
    int anyo;
public:
    friend istream& operator >> (istream &medio, Fecha &f){
        cout<<"-- Fecha de nacimiento del nuevo contacto --"<<endl<<endl;
        if (medio==cin)cout<<"Dia:";
        medio>>f.dia;
        if (medio==cin)cout<<"Mes:";
        medio>>f.mes;
        if (medio==cin)cout<<"Año:";
        medio>>f.anyo;
        system("clear");
        return(medio);
    }
    friend ostream& operator << (ostream &medio, Fecha f){
        if (medio==cout)medio<<"Fecha de nacimiento:"<<f.dia<<"/"<<f.mes<<"/"<<f.anyo<<endl;
        else medio<<f.dia<<" "<<f.mes<<" "<<f.anyo<<" ";
        return(medio);
    }
};

class Direccion{
private:
    string calle;
    int numero;
    string localidad;
public:
    //Operator >> (infrmation input)
    friend istream& operator >> (istream &medio,Direccion &direccion){
        system("clear");
        cout<<"-- Dirección del nuevo contacto --"<<endl<<endl;
        if (medio==cin)cout << "Calle:";
        medio >> direccion.calle;
        if (medio==cin) cout << "Numero:";
        medio >> direccion.numero;
        if (medio==cin) cout << "Localidad:";
        medio >> direccion.localidad;
        return (medio);
    }
    //Operator << (information output)
    friend ostream& operator << (ostream &medio,Direccion direccion){
        if (medio==cout)medio << "C\\"<<direccion.calle<<" Nº "<<direccion.numero<< ", "<<direccion.localidad<<endl<<endl;
        else medio<<direccion.calle<<" "<<direccion.numero<<" "<<direccion.localidad<<endl;
        return (medio);
    }
};
class Contacto{
    friend class Agenda;
private:
    string aux;
    string nombre;
    string apellidos;
    Fecha fechadenacimiento;
    Direccion direccion;
public:
    //Input perator
    friend istream& operator >> (istream &medio, Contacto &contacto){
        cout<<"-- Nombre completo del nuevo contacto --"<<endl<<endl;
        if(medio==cin)cout<<"Nombre:";
        medio>>contacto.nombre;
        if(medio==cin)cout<<"Apellidos:";
        medio>>contacto.apellidos;
        system("clear");
        medio>>contacto.fechadenacimiento;
        medio>>contacto.direccion;
        return (medio);
    }
    //Output operator
    friend ostream& operator << (ostream &medio, Contacto contacto){
        //medio = cout si es para mostrar los contactos
        if (medio==cout)medio<<"Nombre completo:"<<contacto.nombre<<" "<<contacto.apellidos<<endl;
        //si no medio hace referencia a la escritura del fichero
        else  medio<<contacto.nombre<<" "<<contacto.apellidos<<" ";
        //lo mismo se repite en fecha y direccion
        medio<<contacto.fechadenacimiento;
        medio<<contacto.direccion;
        return (medio);
    }
    //Search matches between the input and the contacts name
    bool buscar(string name){
        if (name.compare(nombre)==0)return(true);
        else return(false);
    }
    
};

class Agenda{
    friend class Menu;
    friend Agenda operator +=(Agenda agenda, Agenda contacto);
private:
    Contacto *contactos;
    Contacto *aux;
    Contacto nomb;
    int N;
public:
    int numero;
    Agenda(void){
        ifstream fichero("Agenda.txt");
        if(fichero.good()==true){
            fichero>>N;
            aux = new Contacto [N];
            for (int i=0;i<N;i++) fichero>>aux[i];
            contactos=aux;
        }
        else{
            N=0;
            contactos=NULL;
        }
        return;
    }
    //Destructor
    ~ Agenda(){
        if (contactos!=NULL) delete []contactos;
        return;
    }
    //Operator to add a contact dynamically
    void operator += (Contacto c){
        aux = new Contacto [N+1];
        for (int i=0; i<N; i++){
            aux[i]=contactos[i];
        }
        if (contactos!= NULL) delete []contactos;
        contactos=aux;
        contactos[N]=c;
        N++;
        ordenar();
        return;
    }
    //Operator to show contacts
    friend ostream& operator << (ostream &medio, Agenda &a){
        for (int i=0; i<a.N; i++) medio<<a.contactos[i];
        return (medio);
    }
    //Function to order alphabetically contacts
    void ordenar(void){
        string cadena1,cadena2;
        Contacto aux;
        int evaluacion;
        if (N>1){
            for (int j=0;j<N;j++){
                for (int i=2;i<=N;i++){
                    cadena1=contactos[i-2].nombre;
                    cadena2=contactos[i-1].nombre;
                    evaluacion=cadena1.compare(cadena2);
                    if(evaluacion>0){
                        aux=contactos[i-2];
                        contactos[i-2]=contactos[i-1];
                        contactos[i-1]=aux;
                    }
                }
            }
        }
        return;
    }
    //Function to seek conctacts
    void buscar(void){
        string name;
        bool centinela=true;
        cout<<"-- Búsqueda de contactos --"<<endl<<endl;
        cout<<"Introduzca el nombre:";cin>>name;
        //Busca entre todos los contactos
        for (int i=0; i<N; i++){
            if (contactos[i].buscar(name)){
                cout<<contactos[i];
                centinela=false;
            }
        }
        if (centinela)cout<<"No existe ningún contacto llamado "<<""<<name<<".";
        cin.get();
        cin.get();
    }
    void guardar(void){
        ofstream fichero("Agenda.txt");
        fichero<<N<<endl;
        for (int i=0;i<N;i++)fichero<<contactos[i]<<endl;
        fichero.close();
    }
    
};
class Menu{
private:
    Agenda agenda;
    Contacto contacto;
public:
    void opciones(void){
        int opcion;
        do{
            system("clear");
            cout<<"Bienvenido a su agenda personal, tiene "<<agenda.N<<" contactos."<<endl<<endl;
            cout<<"1. - Nuevo contacto."<<endl;//New contact
            cout<<"2. - Listar contactos."<<endl;//Show contacts
            cout<<"3. - Buscar contactos por nombre."<<endl;//Seek contacts
            cout<<"4. - Salir."<<endl;//Exit
            cout<<"Introduzca opción [1-4]:";cin>>opcion;
            system("clear");
            switch (opcion){
            case 1:
                cin>>contacto;
                agenda+=contacto;
                break;
            case 2:
                cout<<agenda;
                cin.get();
                cin.get();
                break;
            case 3:
                agenda.buscar();
                break;
            case 4:
                agenda.guardar();
                break;
            default:
                cout<<"Opción no válida."<<endl;
            }
        }while(opcion!=4);
        return;
    }
};
int main(void)
{
    Menu m;
    m.opciones();
    return (0);
}
