/*Aplicacion de consola dirigido a Ferreterias que contendro  Un gestor de
usuarios donde se podrá agregar a los usuarios mediante una contraseña de 
administrador e iniciar el menú mediante los usuarios existentes.
Luego contará con un administrador de inventarios donde se podrá ver el 
inventario, eliminar,buscar, agregar productos nuevos y existentes, cambiar los
precios y también se podrá ver los productos que están por debajo del
stock mínimo y todos los datos de los productos se estarán guardados
en un archivo en binario.
Por otro lado, está la parte de ventas donde se podrá visualizar los 
productos a la venta, vender productos y emitir una boleta donde se
mencionar el nombre y DNI del cliente como también la fecha y hora
exacta de la venta.*/

/*
en este proyecto final se utilizo los conocimientos obtenidos en clase 
como:
-Listas dinamicas
-Manipulacion de archivo
-structuras
*/

/*Proyecto publicado en Github
https://github.com/EGJhon/FundProg2Final/tree/main
*/
//Equipo: 
// -Estrada Gutierrez Jhon
// -Morales Honores Said
// -Poma Crispin Dayana

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <conio.h>
#include <fstream>

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
    int pass;
};

typedef struct enlace{
    int idPro;
    int cant;
    struct enlace *sig;
} Listapro;

#define tamListapro sizeof(Listapro)

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//################administracion#######################
void mostrarMensaje(string mensaje);
void ingresarnuvinv();
void mostrarinv(string msg);
void eliminarinv();
void ingresarinv();
bool existeinv(int id);
void mostrafueraestock();
bool interfazadmininv();
void titulo(string mensaje);
void cambiarprecio();
void InterfazbusquedaNombreInv();
bool busquedaNombreInv(char nom[], int *id);
void color();
//###################MENU##################################
void menu(int id,string nomusr);
//###################venta#######################
bool menuventas();
void mostrarinvVenta(string msg);
void interfazventa(Listapro *&lisp,int *cont);
void Boleta(Listapro *lisp);
float mostrarBoletaLinea(producto prod, int fila,int cant);
void guardarboleta(Listapro *lisp,string nomCli,int dni);
producto buscar(int id);
void venderprod(int id,int cant);
string tiempo();
//#################seguridad########################
bool inicio(int *id,string *nomusr);
void crearusuario();
bool existeusr(int id);
bool ingresar(int *id2,string *nomusr);
usuario buscarusr(int id);
int contrasena();


#define passAD 111111

FILE *archivo;
FILE *usuarios;


//###################################### Main ################################################

int main(){
	system("mode con: cols=75 lines=25");
    int id;
    string nomusr;
    system("color 80");
    if (inicio(&id,&nomusr)){
        menu(id,nomusr);
    }
    system("cls");
    mostrarMensaje("gracias por usar el programa");
    return 0;
}

//########################### Administracion #############################

void ingresarnuvinv(){
    char var[30];
    archivo=fopen("Data.prod","a+b");
    producto prod;
    do{
    	system("cls");
    	titulo("Ingresar Produto Nuevo");
        do{
	        cout<<"ingresa el ID del producto: ";
	        cin>>prod.id;
		}while(existeinv(prod.id));
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
        fwrite(&prod,sizeof(prod),1,archivo);
        cout<<"\ndesea seguir ingresando productos (s/n): ";
        cin>>var;
    }while(strcmp(var,"n")!=0);
    fclose(archivo);
	
}

void ingresarinv(){
    FILE *ID;
    int id, cant,opc;
    system("cls");
    cout<<endl;
    mostrarinv(" INGRESAR PRODUCTO ");
    cout<<endl;
    cout<<"ingresar por nombre=1 o id=0: ";
    cin>>opc;
    switch (opc){
    case 1:
        char var[30];
        cout<<"ingresa el nombre del producto:"<<endl;
        cin>>var;
        busquedaNombreInv(var,&id);
        break;   	
    default:
        cout<<"ingresa el ID del producto:"<<endl;
        cin>>id;
        break;
    }
    cout<<"digita la cantidad que se ingresara:"<<endl;
    cin>>cant;
    if(existeinv(id)){
    	archivo=fopen("Data.prod","a+b");
    	ID=fopen("ID","w+b");
    	producto prod;
    	fseek(archivo,0,0);
    	while (fread(&prod,sizeof(prod),1,archivo)!=0){
	        if(prod.id != id)
	            fwrite(&prod,sizeof(prod),1,ID);
	        else{
	        	cout<<"el stock anterio era: "<<prod.stock<<endl;
	            prod.stock+=cant;
	            cout<<"el stock actual es: "<<prod.stock<<endl;
	            fwrite(&prod,sizeof(prod),1,ID);
	        }
	   	}
	   	fclose(archivo);
	    fclose(ID);
	    remove("Data.prod");
	    rename("ID", "Data.prod");
        system("pause"); 
	}
   	else{
   		mostrarMensaje("este id no existe en el archivo, es nuevo");
	}
}

void eliminarinv(){
	FILE *ID;
    int id,opc;
    system("cls");
    mostrarinv(" ELIMINAR PRODUCTO ");
    cout<<endl;
    cout<<"eliminar por nombre=1 o id=0: ";
    cin>>opc;
    switch (opc){
    case 1:
        char var[30];
        cout<<"ingresa el nombre del producto:"<<endl;
        cin>>var;
        busquedaNombreInv(var,&id);
        break;   	
    default:
        cout<<"ingresa el ID del producto:"<<endl;
        cin>>id;
        break;
    }
    if(existeinv(id)){
    	archivo=fopen("Data.prod","r+b");
    	ID=fopen("ID","w+b");
    	producto prod;
    	while (fread(&prod,sizeof(prod),1,archivo)!=0){
	        if(prod.id != id)
	            fwrite(&prod,sizeof(prod),1,ID);
	   	}
	   	fclose(archivo);
	    fclose(ID);
	    remove("Data.prod");
	    rename("ID", "Data.prod");
	    mostrarMensaje("el producto a sido eliminado");
	}
   	else{
   		mostrarMensaje("este id o nombre no existe en el archivo");
	}
}

void cambiarprecio(){
	FILE *ID;
    int id,opc;
    mostrarinv(" CAMBIAR PRECIO ");
    cout<<"cambiar por nombre=1 o id=0: ";
    cin>>opc;
    switch (opc){
    case 1:
        char var[30];
        cout<<"ingresa el nombre del producto:"<<endl;
        cin>>var;
        busquedaNombreInv(var,&id);
        break;   	
    default:
        cout<<"ingresa el ID del producto:"<<endl;
        cin>>id;
        break;
    }
    if(existeinv(id)){
    	archivo=fopen("Data.prod","r+b");
    	ID=fopen("ID","w+b");
    	producto prod;
        float prenuv;
    	while (fread(&prod,sizeof(prod),1,archivo)!=0){
	        if(prod.id == id){
                cout<<"el precio actual es: "<<prod.precio<<endl;
                cout<<"ingrese nuevo precio: ";
                cin>>prenuv;
                prod.precio=prenuv;
	            fwrite(&prod,sizeof(prod),1,ID);
            }
            else
                fwrite(&prod,sizeof(prod),1,ID);
            
	   	}
	   	fclose(archivo);
	    fclose(ID);
	    remove("Data.prod");
	    rename("ID", "Data.prod");
	    mostrarMensaje("el precio del producto ha cambiado");
	}
   	else{
   		mostrarMensaje("este id no existe en el inventario");
	}
}

void mostrarinv(string msg){
    FILE *ID;
    producto prod;
    ID=fopen("Data.prod","r+b");
    system("cls");
    titulo(msg);
    gotoxy(6, 4);
    cout << " ID      Nombre      Costo    Precio   Stock   Stock Min" << endl;
    gotoxy(3, 5);
    cout << "-----------------------------------------------------------------" << endl;
    int filaI=6;
    while (fread(&prod,sizeof(prod),1,ID)!=0){
		gotoxy(7, filaI);cout << prod.id;
        gotoxy(12, filaI);cout << prod.nombre;
        gotoxy(27, filaI);cout << prod.costo;
        gotoxy(36, filaI);cout << prod.precio;
        gotoxy(45, filaI);cout << prod.stock;
        gotoxy(55, filaI);cout << prod.stockmin;
		filaI+=2;
    }
    fclose(ID);
    gotoxy(0, filaI+6);
}

bool existeinv(int id){
    FILE *ID;
    ID=fopen("Data.prod","r+b");
    producto prod;
    fseek(archivo,0,0);
    bool resul=false;
    while (fread(&prod,sizeof(prod),1,ID)!=0){
        if(prod.id == id)
			resul = true;
    }
    fclose(ID);
    return resul;
}

void titulo(string mensaje){
    int logmen = mensaje.length();
    int anchoP = 75; 
    int espaciosIzq = (anchoP - logmen-6) / 2;
    for (int i = 0; i < anchoP; ++i) {
        cout << "=";
    }
    cout << endl;
    gotoxy(espaciosIzq,1);
    cout<<".: "<< mensaje <<" :."<<endl;
    for (int i = 0; i < anchoP; ++i) {
        cout << "=";
    }
    cout << endl;
}

void mostrarMensaje(string mensaje) {
    system("cls");
    titulo(mensaje);
    system("pause");
}

void mostrafueraestock(){
	FILE *ID;
    producto prod;
    system("cls");
    ID=fopen("Data.prod","r+b");   	
    titulo("productos por debajo del Stock minimo");
    gotoxy(15, 4);
    cout << " ID      Nombre     Stock   Stock Min" << endl;
    gotoxy(15, 5);
    cout << "------------------------------------------" << endl;
    int filaI=6;
    while (fread(&prod,sizeof(prod),1,ID)!=0){
    	if(prod.stock<prod.stockmin){
			gotoxy(16, filaI);cout << prod.id;
            gotoxy(23, filaI);cout << prod.nombre;
            gotoxy(35, filaI);cout << prod.stock;
            gotoxy(45, filaI);cout << prod.stockmin;
			filaI+=2;
    	}
	}
	fclose(archivo);
}


bool interfazadmininv(){
    int opc;
    do{
        do{
    	system("cls");
        titulo("ADMINISTRACION DE INVENTARIO");
        cout<<endl;
        int lado=17;
        gotoxy(lado,5);cout<<"1. mostrar inventario"<<endl;
        gotoxy(lado,6);cout<<"2. ingresar producto existente"<<endl;
        gotoxy(lado,7);cout<<"3. ingresar producto nuevo"<<endl;
        gotoxy(lado,8);cout<<"4. mostra productos que no superan el stock minimo"<<endl;
        gotoxy(lado,9);cout<<"5. eliminar producto"<<endl;
        gotoxy(lado,10);cout<<"6. cambiar precio"<<endl;
        gotoxy(lado,11);cout<<"7. busqueda por nombre"<<endl;
        gotoxy(lado,12);cout<<"8. volver a menu"<<endl;                
        gotoxy(lado,13);cout<<"9. salir"<<endl;
        gotoxy(lado,14);cout<<endl;
        cout<<"==========================================================================="<<endl;
            gotoxy(lado-2,17);cout<<"ingresa la opcion: ";
            cin>>opc;
        }while(opc>9);
        switch(opc){
            case 1:
                    mostrarinv("INVENTARIO");
                    system("pause");
                    break;
            case 2:
            		int id,cant;
                    ingresarinv();
                    break;
            case 3:
                    ingresarnuvinv();
                    system("pause");
                    break;
            case 4:
            		mostrafueraestock();
            		system("pause");
            		break;
            case 5:
            		eliminarinv();
            		break;
            case 6:
                    cambiarprecio();
                    system("pause");
            		break;
            case 7:
                    InterfazbusquedaNombreInv();
            		break;          
            case 8:
                    return false;
            		break;
            case 9:
                    return true;
                    break;
        }
    }while(opc!=8 || opc!=9  );  
}

void InterfazbusquedaNombreInv(){
    char var[30];
    int id;
    system("cls");
    titulo("BUSQUEDA POR NOMBRE");
    cout<<"ingresa nombre de producto: ";
    cin>>var;
    bool resul=false;
    resul=busquedaNombreInv(var,&id);
    if (resul){
        string mensaje="el producto existe y el ID es ";
        mensaje.append(to_string(id));
        mostrarMensaje(mensaje);
    } 
    else
        mostrarMensaje("el producto no existe en el inventario");    

}

bool busquedaNombreInv(char nom[], int *id){
    FILE *ID;
    ID=fopen("Data.prod","r+b");
    producto prod;
    bool resul=false;
    while (fread(&prod,sizeof(prod),1,ID)!=0){
        if(strcmp(nom,prod.nombre)==0){
            resul =true;
            break;
        }
    }
    fclose(ID);
    if (resul)
        *id=prod.id;
    else
        *id=NULL;
    return resul;  
}

//################################## Menu ############################################################

void menu(int id,string nomusr){
       int opc;
       bool salir;
    do{
        do{
    	system("cls");
        titulo("MENU");
        int lado=17;
        cout<<endl;
        gotoxy(5,3);cout<<"Bienvenido "<<nomusr;                                  
        gotoxy(60,3);cout<<"ID: "<<id;
        gotoxy(lado,5);cout<<"1. administracion de inventario";
        gotoxy(lado,6);cout<<"2. venta de producto";   
        gotoxy(lado,7);cout<<"3. Temas";               
        gotoxy(lado,8);cout<<"4. salir";
        gotoxy(0,10);cout<<"===========================================================================";
        gotoxy(lado-2,12);cout<<"ingresa la opcion: ";
        cin>>opc;
        }while(opc>4);
        switch(opc){
            case 1:
                    salir=interfazadmininv();
                    break;
            case 2:
            		salir=menuventas();
                    break;
            case 3:
					color();
                    salir=false;
					break;
        }
    }while(opc!=4 && !salir  );   
}

void color(){
    int opc;
    do{
        system("cls");
        titulo("TEMA");
        int lado=25;
        gotoxy(lado,4);cout<<"1. oscuro";
        gotoxy(lado,5);cout<<"2. claro";
        gotoxy(lado,6);cout<<"3. Oceano";
        gotoxy(lado,7);cout<<"4. Matrix";                     
        gotoxy(lado,8);cout<<"5. salir";
        gotoxy(0,10);cout<<"===========================================================================";
        gotoxy(lado,12);cout<<"ingresa la opcion: ";
        cin>>opc;
    }while(opc>5);
    switch (opc){
    case 1:
        system("color 03");
        break;
    case 2:
        system("color 80");
        break;
    case 3:
        system("color 37");
        break;
    case 4:
        system("color 02");
        break;
    }
}
//#######################################ventas#############################################
bool menuventas(){
	Listapro *lisp;
    int opc,cont=0;
    lisp=nullptr;
    do{
    	do{
    	system("cls");
        titulo("VENTAS");
        int lado=25;
        cout<<endl;
        gotoxy(lado,5);cout<<"1. mostrar inventario";
        gotoxy(lado,6);cout<<"2. vender productos";
        gotoxy(lado,7);cout<<"3. volver al menu";
        gotoxy(lado,8);cout<<"4. salir";
		if(cont>0){
            gotoxy(lado,9);cout<<"5. mostrar Boleta";
        }
        gotoxy(0,11);cout<<"==========================================================================="<<endl;
        gotoxy(lado-2,13);cout<<"ingresa la opcion: ";
        cin>>opc;
        }while(opc>4 && not(opc==5 && cont>0) );
        switch(opc){
            case 1:
                    mostrarinvVenta("Productos");
                    system("pause");
                    break;
            case 2:
                    interfazventa(lisp,&cont);
                    system("pause");
                    break;
            case 3:
                    return false;
            		break;
            case 4:
                    return true;
                    break;
            case 5:
            		
					Boleta(lisp);
                    system("pause");
                    lisp=nullptr;
                    cont=0;
                    break;
        }
    }while(opc!=3 || opc!=4 );  
}

void mostrarinvVenta(string msg){
    FILE *ID;
    producto prod;
    ID=fopen("Data.prod","r+b");
    system("cls");
    int filaI=6;
    titulo(msg);
    gotoxy(15, 4);
    cout << "ID      Nombre      Precio   Stock" << endl;
    gotoxy(10, 5);
    cout << "-------------------------------------------" << endl;
    while (fread(&prod,sizeof(prod),1,ID)!=0){
		gotoxy(15, filaI);cout << prod.id;
        gotoxy(22, filaI);cout << prod.nombre;
        gotoxy(36, filaI);cout << prod.precio;
        gotoxy(45, filaI);cout << prod.stock;
        filaI+=1;
    }
    fclose(ID);
    gotoxy(0, filaI+1);
}

void interfazventa(Listapro *&lisp,int *cont){
	char opc;
    Listapro *nuevo,*q;
    int id,cant,opc2;
	do{
        system("cls");
    	cout<<endl;
    	mostrarinvVenta(" VENTA DE PRODUCTO ");
    	cout<<endl;
    	cout<<"comprar por nombre=1 o id=0: ";
        cin>>opc2;
        switch (opc2){
        case 1:
            char var[30];
            cout<<"ingresa el nombre del producto: ";
            cin>>var;
            busquedaNombreInv(var,&id);
            break;   	
        default:
            cout<<"ingresa el ID del producto: ";
            cin>>id;
            break;
        }
    	cout<<"digita la cantidad que se comprara: ";
    	cin>>cant;
        if (existeinv(id)){    
            nuevo=(Listapro *)malloc(tamListapro );
            nuevo->idPro=id;
            nuevo->cant=cant;
            nuevo->sig=nullptr; 
            if (lisp==nullptr){
                lisp=nuevo;
                *cont+=1;
                venderprod(id,cant);
            }
            else{
                q=lisp;
                while (q->sig!=nullptr){
                    q=q->sig;
                }
                    q->sig=nuevo;
                    *cont+=1;
                    venderprod(id,cant);
            }
            cout<<"desea seguir comprando (s/n): ";
            cin>>opc;
        }
        else{
            mostrarMensaje("El ID o nombre del producto no existe");
            opc='s';
        }

    }while(opc!='n');
}

void Boleta(Listapro *lisp){
    Listapro *q;
    producto prod;
    int filaI=1;
    string nomCli;
    int dni,Ruc;
    system("cls");
    titulo("INGRESE DATOS DEL CLIENTE");
    cout<<"ingrese nombre de cliente: ";
    cin>>nomCli;
    cout<<"ingrese DNI del cliente: ";
    cin>>dni;
    system("cls");
    gotoxy(0,filaI);cout<<"==========================================================================="<<endl;
    gotoxy(5,filaI+1);cout<<"              .: BOLETA     FERRETERIA TUERCA Y TORNILLO :."<<endl;
    gotoxy(0,filaI+2);cout<<"==========================================================================="<<endl;
    gotoxy(5,filaI+3);cout<<"Nombre: "<<nomCli;gotoxy(35,filaI+3);cout<<"fecha: "<<tiempo()<<endl;
    gotoxy(5,filaI+4);cout<<"DNI: "<<dni<<endl;
    gotoxy(0,filaI+5);cout<<"==========================================================================="<<endl;    
    gotoxy(10, filaI+6);cout << " ID      Nombre      cantidad    precio U    total" << endl;
    gotoxy(5, filaI+7);cout << "---------------------------------------------------------------" << endl;
    q=lisp;
    float pago=0;
    while (q!=nullptr){
        prod=buscar(q->idPro);
		pago+=mostrarBoletaLinea(prod, (filaI+8),q->cant);
		filaI+=2;
        q=q->sig;
    }
    gotoxy(5, filaI+8);cout<<" total a pagar es :"<< pago<<" S/."<<endl;
    guardarboleta(lisp,nomCli,dni);
}

float mostrarBoletaLinea(producto prod, int fila,int cant) {
    gotoxy(11, fila);cout << prod.id;
    gotoxy(18, fila);cout << prod.nombre;
    gotoxy(32, fila);cout << cant;
	gotoxy(43, fila);cout << prod.precio<<" S/.";
   	gotoxy(55, fila);cout << prod.precio * cant<< " S/.";
    return prod.precio * cant;
}

void guardarboleta(Listapro *lisp,string nomCli,int dni){
    Listapro *q;
    producto prod;
    string boleta="Boleta";
    boleta.append(nomCli);
    boleta.append(".txt");
    ofstream archivo(boleta);
    archivo<<"===========================================================================\n";
    archivo<<"              .: BOLETA     FERRETERIA TUERCA Y TORNILLO :.\n";
    archivo<<"===========================================================================\n";
    archivo<<"  Nombre: "<<nomCli<<"                        fecha: "<<tiempo()<<"\n";
    archivo<<"  DNI: "<<dni<<"\n";
    archivo<<"===========================================================================\n";    
    archivo<< " ID      Nombre      cantidad    precio U    total\n";
    archivo<< "---------------------------------------------------------------\n";
    q=lisp;
    float pago=0;
    archivo.close();
    while (q!=nullptr){
        prod=buscar(q->idPro);
        ofstream archivo(boleta, ios::app);
        archivo<<"  "<< prod.id<<"  "<<prod.nombre<<"           "<< q->cant<<"      "<< prod.precio<<" S/."<<"      "<< prod.precio * q->cant<< " S/.\n";
        pago+=(prod.precio * (q->cant));
        q=q->sig;
        if(q==nullptr){
            archivo<<"\n--------------------------------------------------------------\n";
            archivo<<"el total a pagar es: "<<pago<<" S/.\n";
            archivo.close();
            break;
        }
        archivo.close();
    }
}

producto buscar(int id){
    FILE *ID;
    ID=fopen("Data.prod","r+b");
    producto prod;
    fseek(archivo,0,0);
    while (fread(&prod,sizeof(prod),1,ID)!=0){
        if(prod.id == id){
       		fclose(ID);
			return prod;
        }    
    }

}

void venderprod(int id,int cant){
    FILE *ID;
    if(existeinv(id)){
    	archivo=fopen("Data.prod","a+b");
    	ID=fopen("temp","w+b");
    	producto prod;
    	fseek(archivo,0,0);
    	while (fread(&prod,sizeof(prod),1,archivo)!=0){
	        if(prod.id != id)
	            fwrite(&prod,sizeof(prod),1,ID);
	        else{
	        	cout<<"el stock anterio era: "<<prod.stock<<endl;
	            prod.stock-=cant;
	            cout<<"el stock actual es: "<<prod.stock<<endl;
	            fwrite(&prod,sizeof(prod),1,ID);
	        }
	   	}
	   	fclose(archivo);
	    fclose(ID);
	    remove("Data.prod");
	    rename("temp", "Data.prod"); 
	}
   	else{
   		mostrarMensaje("este id no es valido");
	}
}

string tiempo(){ 
    chrono::system_clock::time_point now = chrono::system_clock::now();
    time_t tiempo_actual = chrono::system_clock::to_time_t(now);
    return ctime(&tiempo_actual);
}

//#########################################seguridad##############################################

bool inicio(int *id,string *nomusr){
	int opc,cont=0;
    bool pase=false;
        do{
        do{
    	system("cls");
        titulo("FERRETRIA TUERCA Y TORNILLO");
        int lado=25;
        gotoxy(lado,4);cout<<"1. iniciar seccion";
        gotoxy(lado,5);cout<<"2. crear usuario";               
        gotoxy(lado,6);cout<<"3. salir";
        gotoxy(0,8);cout<<"==========================================================================="<<endl;
        gotoxy(lado-2,10);cout<<"ingresa la opcion: ";
        cin>>opc;
        }while(opc>3);
        switch(opc){
            case 1:
                    pase=ingresar(id,nomusr);
                    if (pase==false){
                        cont=3;
                    }
                    
                    break;
            case 2:
            		crearusuario();
                    break;
        }
    }while((opc!=3 && !pase) && !(cont == 3));  
    if (cont==3){
        mostrarMensaje("Ha superado el numero de intentos");
        system("pause");
    }
    return pase;
}

void crearusuario(){
    char var[30];
    int pass;
    usuarios=fopen("Usr.usr","a+b");
    usuario usr;
    for (int i = 0; i < 3; i++){
        system("cls");
        titulo("ingresar contrasena de administrador");
        cout<<"intento("<<i+1<<") contrasena: ";
        pass=contrasena();
        if (pass==passAD){
            do{
                system("cls");
                titulo("CREACION DE USUARIO");
                cout<<"ingresa el ID del nuevo usuario: ";
                cin>>usr.id;
                if(existeusr(usr.id)){
                    mostrarMensaje("el id no este disponible");                    
                }
            }while(existeusr(usr.id));
            cout<<"ingresa nombre del usuario: ";
            cin>>var;
            strcpy(usr.nombre,var);
            cout<<"ingresa contrasena (numerica de 6 digitos): ";
            usr.pass=contrasena();
            fwrite(&usr,sizeof(usr),1,usuarios);
            fclose(usuarios);
            system("cls");
            mostrarMensaje("ususario creado");
            break;
        }
        else{
            system("cls");
            mostrarMensaje("la contrasena es incorrecta");
            system("pause");
        }    
    }
}

bool existeusr(int id){
    FILE *ID;
    ID=fopen("Usr.usr","r+b");
    usuario usr;
    fseek(archivo,0,0);
    bool resul=false;
    while (fread(&usr,sizeof(usr),1,ID)!=0){
        if(usr.id == id)
			resul = true;
    }
    fclose(ID);
    return resul;
}

bool ingresar(int *id2,string *nomusr){
    char var[30];
    int pass,id;
    bool pase;
    usuarios=fopen("Usr.usr","r+b");
    usuario usr;
    for (int i = 0; i < 3; i++){
        system("cls");
        titulo("INGRESAR ID Y CONTRASENA DE USUARIO");
        cout<<"intento("<<i+1<<") ingrese ID: ";
        cin>>id;
        *id2=id;
        cout<<"contrasena: ";
        pass=contrasena();
        if(existeusr(id)){
            usr=buscarusr(id);
            if (usr.pass==pass){
                *nomusr=usr.nombre;   
                return true;
            }
            else{
                mostrarMensaje("el usuario no existe o la contrasena es incorrecta");
            }
        }
        else{
            mostrarMensaje("el usuario no existe o la contrasena es incorrecta");
        }
    }
    return false;
}

usuario buscarusr(int id){
    FILE *ID;
    ID=fopen("Usr.usr","r+b");
    usuario usr;
    fseek(ID,0,0);
    while (fread(&usr,sizeof(usr),1,ID)!=0){
        if(usr.id == id){
       		fclose(ID);
			return usr;
        }    
    }
}

int contrasena(){
    int contr;
    string pass;
    char num;
    for(int i=0;i<6;i++) {
        num = getch(); 
        cout << "*"; 
        pass += num; 
    }
    contr=stoi(pass);
    return contr;
}


