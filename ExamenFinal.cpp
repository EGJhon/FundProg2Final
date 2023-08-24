#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>

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

// funcion gotoxy()
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// prototipos
//################administracion#######################
void mostrarMensaje(string mensaje);
void ingresarnuvinv();
void mostrarinv(string msg);
void eliminarinv();
void ingresarinv();
bool existeinv(int id);
void mostrarProductoLinea(producto prod, int fila);
void mostrarProductostock(producto prod, int fila,int col);
void mostrafueraestock();
bool interfazadmininv();
void menu(int id,string nomusr);
void cambiarprecio();
//###################venta#######################
bool menuventas();
void mostrarinvVenta(string msg);
void mostrarProductoLineaventa(producto prod, int fila);
void interfazventa(Listapro *&lisp,int *cont);
void Boleta(Listapro *lisp);
float mostrarBoletaLinea(producto prod, int fila,int col,int cant);
producto buscar(int id);
void venderprod(int id,int cant);
string tiempo();
//##################seguridad########################
bool inicio(int *id,string *nomusr);
void crearusuario();
bool existeusr(int id);
bool ingresar(int *id2,string *nomusr);
usuario buscarusr(int id);


#define passAD 111111
// creacion de punteros archivo como global
FILE *archivo;
FILE *usuarios;

//###################################### Main ################################################

int main(){
    SMALL_RECT rect;
    COORD coord;

    coord.X = 75; 
    coord.Y = 25; 

    rect.Left = 0;
    rect.Top = 0;
    rect.Right = coord.X - 1;
    rect.Bottom = coord.Y - 1;

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(console, coord);
    SetConsoleWindowInfo(console, TRUE, &rect);

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
    	cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               .:ingresar nuevo productos:."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<endl;
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
    int id, cant;
    system("cls");
    cout<<endl;
    mostrarinv(" INGRESAR PRODUCTO ");
    cout<<endl;
    cout<<"ingresa el ID del producto:"<<endl;
    cin>>id;
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
    int id;
    system("cls");
    mostrarinv(" ELIMINAR PRODUCTO ");
    cout<<endl;
    cout<<"ingresa el ID del producto:"<<endl;
    cin>>id;
    if(existeinv(id)){
    	archivo=fopen("Data.prod","a+b");
    	ID=fopen("ID","w+b");
    	producto prod;
    	fseek(archivo,0,0);
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
   		mostrarMensaje("este id no existe en el archivo, es nuevo");
	}
}

void cambiarprecio(){
	FILE *ID;
    int id;
    mostrarinv(" CAMBIAR PRECIO ");
    cout<<"ingresa el ID del producto: ";
    cin>>id;
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
    int posx= msg.length()+2;
    posx=(75-posx)/2;
    producto prod;
    ID=fopen("Data.prod","r+b");
    system("cls");
    int filaI=1;
    gotoxy(5,filaI);
    	cout<<"==================================================================="<<endl;
    gotoxy(posx,filaI+1);
        cout<<".:"<<msg<<":."<<endl;
    gotoxy(5,filaI+2);
        cout<<"==================================================================="<<endl;
    gotoxy(5, filaI+4);
    cout << " ID      Nombre      Costo    Precio   Stock   Stock Min" << endl;
    gotoxy(5, filaI+5);
    cout << "-------------------------------------------------------------------" << endl;
    while (fread(&prod,sizeof(prod),1,ID)!=0){
		mostrarProductoLinea(prod, filaI+6);
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

void mostrarProductoLinea(producto prod, int fila) {
    gotoxy(5, fila);
    cout << prod.id;

    gotoxy(12, fila);
    cout << prod.nombre;

    gotoxy(26, fila);
    cout << prod.costo;

    gotoxy(35, fila);
    cout << prod.precio;

    gotoxy(45, fila);
    cout << prod.stock;

    gotoxy(55, fila);
    cout << prod.stockmin;
}

void mostrarMensaje(string mensaje) {
    system("cls");
    system("color 84");
    int logmen = mensaje.length();
    int anchoP = 75; 
    int espaciosIzquierda = (anchoP - logmen) / 2;
    for (int i = 0; i < anchoP; ++i) {
        cout << "=";
    }
    cout << endl;
    gotoxy(espaciosIzquierda,1);
    cout<< mensaje <<endl;
    for (int i = 0; i < anchoP; ++i) {
        cout << "=";
    }
    cout << endl;
    system("pause");
    system("color 80");
}

void mostrafueraestock(){
	FILE *ID;
    producto prod;
    system("cls");
    ID=fopen("Data.prod","r+b");
   	int filaI=1;
   	int col=10;
    gotoxy(col,filaI);
    	cout<<"=========================================================="<<endl;
    gotoxy(col,filaI+1);
        cout<<"        .:productos por debajo del Stock minimo:."<<endl;
    gotoxy(col,filaI+2);
        cout<<"=========================================================="<<endl;
    gotoxy(col, filaI+4);
    cout << " ID      Nombre     Stock   Stock Min" << endl;
    gotoxy(col, filaI+5);
    cout << "------------------------------------------" << endl;
    while (fread(&prod,sizeof(prod),1,ID)!=0){
    	if(prod.stock<prod.stockmin){
			mostrarProductostock(prod, filaI+6,col);
			filaI+=2;
    	}
	}
	fclose(archivo);
    gotoxy(0, filaI+7);
}

void mostrarProductostock(producto prod, int fila,int col) {
    gotoxy(col+1, fila);
    cout << prod.id;

    gotoxy(col+8, fila);
    cout << prod.nombre;

    gotoxy(col+20, fila);
    cout << prod.stock;

	gotoxy(col+30, fila);
    cout << prod.stockmin;
}

bool interfazadmininv(){
    int opc;
    do{
        do{
    	system("cls");
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"                     .: administracion de invetario :."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<endl;
        cout<<"            1. mostrar inventario"<<endl;
        cout<<"            2. ingresar producto existente"<<endl;
        cout<<"            3. ingresar producto nuevo"<<endl;
        cout<<"            4. mostra productos que no superan el stock minimo"<<endl;
        cout<<"            5. eliminar producto"<<endl;
        cout<<"            6. cambiar precio"<<endl;
        cout<<"            7. volver a menu"<<endl;                
        cout<<"            8. salir"<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
            cout<<"ingresa la opcion: ";
            cin>>opc;
        }while(opc>8);
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
                    return false;
            		break;
            case 8:
                    return true;
                    break;
        }
    }while(opc!=7 || opc!=8  );  
}

//################################## Menu ############################################################

void menu(int id,string nomusr){
       int opc;
       bool salir;
    do{
        do{
    	system("cls");
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               	            .: MENU :."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<"Bienvenido "<<nomusr<<"                                  ID: "<<id<<endl;
        cout<<endl;
        cout<<"             1. adminitrar inventario"<<endl;
        cout<<"             2. venta de producto"<<endl;               
        cout<<"             3. salir"<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
            cout<<"ingresa la opcion: ";
            cin>>opc;
        }while(opc>3);
        switch(opc){
            case 1:
                    salir=interfazadmininv();
                    break;
            case 2:
            		salir=menuventas();
                    break;
        }
    }while(opc!=3 && !salir  );   
}

//#######################################ventas#############################################
bool menuventas(){
	Listapro *lisp;
    int opc,cont=0;
    lisp=nullptr;
    do{
    	do{
    	system("cls");
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"              	            .: VENTAS :."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<endl;
        cout<<"            1. mostrar inventario"<<endl;
        cout<<"            2. vender productos"<<endl;
        cout<<"            3. volver al menu"<<endl;
        cout<<"            4. salir"<<endl;
		if(cont>0){
            cout<<"            5. mostrar Boleta"<<endl;
        }
        cout<<endl;
        cout<<"========================================================================"<<endl;
            cout<<"ingresa la opcion: ";
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
    int filaI=1;
    gotoxy(5,filaI);
    	cout<<"======================================================="<<endl;
    gotoxy(5,filaI+1);
        cout<<"               	.:"<<msg<<":."<<endl;
    gotoxy(5,filaI+2);
        cout<<"======================================================="<<endl;
    gotoxy(5, filaI+4);
    cout << " ID      Nombre      Precio   Stock" << endl;
    gotoxy(5, filaI+5);
    cout << "---------------------------------------------------------------------" << endl;
    while (fread(&prod,sizeof(prod),1,ID)!=0){
		mostrarProductoLineaventa(prod, filaI+6);
		filaI+=2;
    }
    fclose(ID);
    gotoxy(0, filaI+6);
}

void mostrarProductoLineaventa(producto prod, int fila) {
    gotoxy(5, fila);
    cout << prod.id;

    gotoxy(12, fila);
    cout << prod.nombre;

    gotoxy(26, fila);
    cout << prod.precio;

    gotoxy(35, fila);
    cout << prod.stock;
}

void interfazventa(Listapro *&lisp,int *cont){
	char opc;
    Listapro *nuevo,*q;
    int id,cant;
    	system("cls");
    	cout<<endl;
    	mostrarinvVenta(" VENTA DE PRODUCTO ");
    	cout<<endl;
    	cout<<"ingresa el ID del producto:"<<endl;
    	cin>>id;
    	cout<<"digita la cantidad que se comprara:"<<endl;
    	cin>>cant;
	do{
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
            if (opc!='n'){
                system("cls");
    	        cout<<endl;
    	        mostrarinvVenta(" VENTA DE PRODUCTO ");
    	        cout<<endl;
                cout<<"ingrese ID de producto: ";
                cin>>id;
                cout<<"ingrese cantidad del producto a comprar: ";
                cin>>cant;
            }
        }
        else{
            opc='s';
            mostrarMensaje(" el id que ingresate no es valido");
        }

    }while(opc!='n');
}

void Boleta(Listapro *lisp){
    Listapro *q;
    producto prod;
    int filaI=1;
    char nomCli[30];
    int dni,Ruc;
    system("cls");
    gotoxy(5,filaI);
    	cout<<"========================================================================"<<endl;
    gotoxy(5,filaI+1);
        cout<<"               		.: BOLETA :."<<endl;
    gotoxy(5,filaI+2);
        cout<<"========================================================================"<<endl;
    cout<<"ingrese nombre de cliente: ";
    cin>>nomCli;
    cout<<"ingrese DNI del cliente: ";
    cin>>dni;
    system("cls");

    gotoxy(5,filaI);
    	cout<<"========================================================================"<<endl;
    gotoxy(5,filaI+1);
        cout<<"              .: BOLETA     FERRETEIA TUERCA Y TORNILLO:."<<endl;
    gotoxy(5,filaI+2);
        cout<<"========================================================================"<<endl;
    gotoxy(5,filaI+3);
        cout<<" Nombre: "<<nomCli;
        gotoxy(35,filaI+3);
        cout<<"fecha: "<<tiempo()<<endl;
    gotoxy(5,filaI+4);
        cout<<"DNI: "<<dni<<endl;
    gotoxy(5,filaI+5);
        cout<<"========================================================================"<<endl;    
    gotoxy(5, filaI+6);
    cout << " ID      Nombre      cantidad    precio U    total" << endl;
    gotoxy(5, filaI+7);
    cout << "---------------------------------------------------------------------" << endl;
    q=lisp;
    float pago=0;
    while (q!=nullptr){
        prod=buscar(q->idPro);
		pago+=mostrarBoletaLinea(prod, (filaI+8) , 5,q->cant);
		filaI+=2;
        q=q->sig;
    }
    gotoxy(5, filaI+8);
    cout<<" total a pagar es :"<< pago<<endl;
    gotoxy(-5, filaI+10);
}

float mostrarBoletaLinea(producto prod, int fila,int col,int cant) {
    gotoxy(col+1, fila);
    cout << prod.id;

    gotoxy(col+8, fila);
    cout << prod.nombre;

    gotoxy(col+22, fila);
    cout << cant;

	gotoxy(col+33, fila);
    cout << prod.precio<<" S/.";
    
   	gotoxy(col+45, fila);
    cout << prod.precio * cant<< " S/.";
    
    return (float)prod.precio * cant;
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
    	system("cls");
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               	  .: FERRETERIA TUERCA Y TORNILLO :."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<endl;
        cout<<"            1. iniciar seccion"<<endl;
        cout<<"            2. crear usuario"<<endl;               
        cout<<"            3. salir"<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        do{
            cout<<"ingresa la opcion: ";
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
        system("cls");
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
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               .:ingresar contrasena de administrador:."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"contrasena: ";
        cin>>pass;
        if (pass==passAD){
            system("cls");
            cout<<"========================================================================"<<endl;
            cout<<endl;
            cout<<"               .:Creacion de nuevo usuario:."<<endl;
            cout<<endl;
            cout<<"========================================================================"<<endl;
            cout<<endl;
            do{
                cout<<"ingresa el ID del nuevo usuario: ";
                cin>>usr.id;
            }while(existeusr(usr.id));
            cout<<"ingresa nombre del usuario: ";
            cin>>var;
            strcpy(usr.nombre,var);
            cout<<"ingresa contrasena (numerica de 6 digitos): ";
            cin>>usr.pass;
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
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               .: ingresar ID y contrasena de usuario :."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"ingrese ID: ";
        cin>>id;
        *id2=id;
        cout<<"contrasena: ";
        cin>>pass;
        if(existeusr(id)){
            usr=buscarusr(id);
            if (usr.pass==pass){
                *nomusr=usr.nombre;   
                return true;
            }
            else{
                system("cls");
                mostrarMensaje("el usuario no existe o la contrasena es incorrecta");
            }
        }
        else{
            system("cls");
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
