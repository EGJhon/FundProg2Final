#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

struct producto{
    int id;
    char nombre[20];
    float costo;
    float precio;
    int stock;
    int stockmin;
};

//creamos el tipo lista dinamica que contendra id y la cantidad de productos comprados
typedef struct enlace{
    int idPro;
    int cant;
    struct enlace *siguiente;
} listapro;

#define tamListapro sizeof(listapro)

// prototipos
void menu();
void ingresarinv();
void mostrarinv();
void venderinv();
void voleta(listapro *venta);

// creacion de untero archivo como global
FILE *archivo;

int main(){
    
    return 0;
}

void ingresarinv(){
    string var;
    archivo=open("Data.prod","a+b");
    producto prod;
    do{
        cout<<"ingresa el ID del producto: ";
        cin>>prod.id;
        cout<<"ingresa nombre del producto: ";
        cin>>var;
        strcpy(prod.nombre,var);
        cout<<"ingresa el costo del producto: ";
        cin>>prod.costo;
        cout<<"ingresa el precio del producto: ";
        cin>>prod.precio;
        cout<<"ingresa el stock del producto: ";
        cin>>prod.stock;
        cout<<"ingresa el stock minimo del producto: ";
        cin>>prod.stockmin;
        cout<<"\ndesea seguir ingresando productos (s/n): ";
        cin>>var;
    }while(strcmp(var,"n")!=0);
    
}
