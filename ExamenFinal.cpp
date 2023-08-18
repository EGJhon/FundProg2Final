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

struct usuario{
    int id;
    char nombre[15];
    char pass[];
};

typedef struct enlace{
    int idPro;
    int cant;
    struct enlace *siguiente;
} listapro;

#define tamListapro sizeof(listapro)

// prototipos
void inicio();
void crearusr();
void submenuventa();
void submenuproduc();
void menu();
void ingresarinv();
void mostrarinv();
void mostrarprod();
void vender();
void voleta(listapro *venta);

// creacion de punteros archivo como global
FILE *archivo;
FILE *usuarios;

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
        fwrite(&prod,sizeof(produc),1,archivo);
        cout<<"\ndesea seguir ingresando productos (s/n): ";
        cin>>var;
    }while(strcmp(var,"n")!=0);
}
void crearusr(){
    bool Acceso;
    char pass[6],nomb[15];
    usuario usr[];
    int id;
    usuarios = open("usuario.usr","a+b");
    
}
void inicio(){
    bool Acceso;
    char pass[6],nomb[15];
    usuario usr[];
    int id;
    usuarios = open("usuario.usr","rb");
    
}

void menu(){
    int opc;
    do{
        cout<<"=========================================================================="<<endl;
        cout<<endl;
        cout<<"               .:MENU:."<<endl;
        cout<<endl;
        cout<<"            1. administracion"<<endl;
        cout<<"            2. venta"<<endl;
        cout<<"            3. salir"
        cout<<endl;
        cout<<"=========================================================================="<<endl;
        do{
            cout<<"ingresa la opcion: ";
            cin>>opc;
        }while(opc>3);
        switch(opc){
            case 1:
                    submenuadmin();
                    break;
            case 2:
                    submenuventa();
                    break;
            default:
                    break;
        }
    }while(opc!=3);
    
}

void submenuproduc(){
    int opc;
    do{
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               .:productos:."<<endl;
        cout<<endl;
        cout<<"            1. mostrar inventario"<<endl;
        cout<<"            2. ingresar producto a invetario"<<endl;
        cout<<"            3. mostra productos que no superan el stock minimo"<<endl;
        cout<<"            4. salir"
        cout<<endl;
        cout<<"========================================================================"<<endl;
        do{
            cout<<"ingresa la opcion: ";
            cin>>opc;
        }while(opc>3);
        switch(opc){
            case 1:
                    mostrarinv();
                    break;
            case 2:
                    ingresarinv();
                    break;
            case 3:
                    mostrarFStock();
                    break;
            default:
                    break;
        }
    }while(opc!=3);  
}

void submenuventa(){
    int opc;
    do{
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               .:venta:."<<endl;
        cout<<endl;
        cout<<"            1. mostrar productos"<<endl;
        cout<<"            2. vender"<<endl;
        cout<<"            3. salir"
        cout<<endl;
        cout<<"========================================================================"<<endl;
        do{
            cout<<"ingresa la opcion: ";
            cin>>opc;
        }while(opc>3);
        switch(opc){
            case 1:
                    mostrarinv();
                    break;
            case 2:
                    vender();
                    break;
            default:
                    break;
        }
    }while(opc!=3);  
}
