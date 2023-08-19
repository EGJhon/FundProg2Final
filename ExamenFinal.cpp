#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip> //para una funcion que estoy utilizando
#include <windows.h>//es para gotoxy

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

// funcion gotoxy()
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// prototipos
//################JHON#######################
void mostrarMensaje(string mensaje);
void ingresarnuvinv();
void mostrarinv();
void eliminarinv(int id);
void ingresarinv(int id,int cant);
bool existeinv(int id);
void mostrarProductoEnCuadro(producto prod, int fila);
void mostrarProductostock(producto prod, int fila,int col);
void mostrafueraestock();
void interfazingre(int *id,int *cant);
bool interfazadmininv();
void interfazeliminar(int *id);
//###################Saidh#######################

//##################kevin########################


// creacion de punteros archivo como global
FILE *archivo;
FILE *usuarios;

int main(){
    
    return 0;
}



//########################### Jhon #############################

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

void ingresarinv(int id,int cant){
    FILE *temp;
    if(existeinv(id)){
    	archivo=fopen("Data.prod","a+b");
    	temp=fopen("temp","w+b");
    	producto prod;
    	fseek(archivo,0,0);
    	while (fread(&prod,sizeof(prod),1,archivo)!=0){
	        if(prod.id != id)
	            fwrite(&prod,sizeof(prod),1,temp);
	        else{
	        	cout<<"el stock anterio era: "<<prod.stock<<endl;
	            prod.stock+=cant;
	            cout<<"el stock actual es: "<<prod.stock<<endl;
	            fwrite(&prod,sizeof(prod),1,temp);
	        }
	   	}
	   	fclose(archivo);
	    fclose(temp);
	    remove("Data.prod");
	    rename("temp", "Data.prod");
	    //return true;
	}
   	else{
   		mostrarMensaje("este id no existe en el archivo, es nuevo");
		//return false;	
	}
}

void eliminarinv(int id){
	FILE *temp;
    if(existeinv(id)){
    	archivo=fopen("Data.prod","a+b");
    	temp=fopen("temp","w+b");
    	producto prod;
    	fseek(archivo,0,0);
    	while (fread(&prod,sizeof(prod),1,archivo)!=0){
	        if(prod.id != id)
	            fwrite(&prod,sizeof(prod),1,temp);
	   	}
	   	fclose(archivo);
	    fclose(temp);
	    remove("Data.prod");
	    rename("temp", "Data.prod");
	    mostrarMensaje("el producto a sido eliminado");
	}
   	else{
   		mostrarMensaje("este id no existe en el archivo, es nuevo");
	}
}

void mostrarinv(string msg){
    FILE *temp;
    producto prod;
    temp=fopen("Data.prod","r+b");
    system("cls");
    int filaI=1;
    gotoxy(5,filaI);
    	cout<<"========================================================================"<<endl;
    gotoxy(5,filaI+1);
        cout<<"               		.:"<<msg<<":."<<endl;
    gotoxy(5,filaI+2);
        cout<<"========================================================================"<<endl;
    gotoxy(5, filaI+4);
    cout << " ID      Nombre      Costo    Precio   Stock   Stock Min" << endl;
    gotoxy(5, filaI+5);
    cout << "----------------------------------------------------------" << endl;
    while (fread(&prod,sizeof(prod),1,temp)!=0){
		mostrarProductoEnCuadro(prod, filaI+6);
		filaI+=2;
    }
    fclose(temp);
    gotoxy(0, filaI+6);
}

bool existeinv(int id){
    FILE *temp;
    temp=fopen("Data.prod","r+b");
    producto prod;
    fseek(archivo,0,0);
    bool resul=false;
    while (fread(&prod,sizeof(prod),1,temp)!=0){
        if(prod.id == id)
			resul = true;
    }
    fclose(temp);
    return resul;
}

void mostrarProductoEnCuadro(producto prod, int fila) {
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
    int longitud = mensaje.length();
    int espacioDisponible = 70; 
    int espaciosIzquierda = (espacioDisponible - longitud) / 2;
    for (int i = 0; i < espacioDisponible; ++i) {
        cout << "=";
    }
    cout << endl;
    cout << setw(espaciosIzquierda + longitud) << mensaje <<endl;
    for (int i = 0; i < espacioDisponible; ++i) {
        cout << "=";
    }
    cout << endl;
}

void mostrafueraestock(){
	FILE *temp;
    producto prod;
    system("cls");
    temp=fopen("Data.prod","r+b");
   	int filaI=1;
   	int col=10;
    gotoxy(col,filaI);
    	cout<<"========================================================================"<<endl;
    gotoxy(col,filaI+1);
        cout<<"           .:productos por debajo del Stock minimo:."<<endl;
    gotoxy(col,filaI+2);
        cout<<"========================================================================"<<endl;
    gotoxy(col, filaI+4);
    cout << " ID      Nombre     Stock   Stock Min" << endl;
    gotoxy(col, filaI+5);
    cout << "-----------------------------------------" << endl;
    while (fread(&prod,sizeof(prod),1,temp)!=0){
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
    	system("cls");
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               .:administracion de invetario:."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<endl;
        cout<<"            1. mostrar inventario"<<endl;
        cout<<"            2. ingresar producto existente"<<endl;
        cout<<"            3. ingresar producto nuevo"<<endl;
        cout<<"            4. mostra productos que no superan el stock minimo"<<endl;
        cout<<"            5. eliminar producto"<<endl;
        cout<<"            6. volver a menu"<<endl;                
        cout<<"            7. salir"<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        do{
            cout<<"ingresa la opcion: ";
            cin>>opc;
        }while(opc>7);
        switch(opc){
            case 1:
                    mostrarinv("INVENTARIO");
                    system("pause");
                    break;
            case 2:
            		int id,cant;
            		interfazingre(&id,&cant);
                    ingresarinv(id, cant);
                    system("pause");
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
            		int id2;
            		interfazeliminar(&id2);
            		eliminarinv(id);
            		system("pause");
            		break;
            case 6:
                    return true;
            		break;
            case 7:
                    return false;
                    break;
        }
    }while(opc!=6 || opc!=7  );  

}

void interfazingre(int *id,int *cant){
		system("cls");
    	cout<<endl;
    	mostrarinv(" INGRESAR PRODUCTO ");
    	cout<<endl;
    	cout<<"ingresa el ID del producto:"<<endl;
    	cin>>*id;
    	cout<<"digita la cantidad que se ingresara:"<<endl;
    	cin>>*cant;
}

void interfazeliminar(int *id){
		system("cls");
	    cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               .:eliminar producto:."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
    	cout<<endl;
    	cout<<"ingresa el ID del producto:"<<endl;
    	cin>>*id;
}

void menu(){
       int opc;
       bool salir;
    do{
    	system("cls");
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<"               .:administracion de invetario:."<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        cout<<endl;
        cout<<endl;
        cout<<"            1. adminitrar inventario"<<endl;
        cout<<"            2. venta de producto"<<endl;               
        cout<<"            3. salir"<<endl;
        cout<<endl;
        cout<<"========================================================================"<<endl;
        do{
            cout<<"ingresa la opcion: ";
            cin>>opc;
        }while(opc>3);
        switch(opc){
            case 1:
                    salir=interfazadmininv();
                    break;
            case 2:
            		
                    break;
        }
    }while(opc!=3 && !salir  );   
}

