#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <string>
#include <iomanip>
#define PI 3.14159265
using namespace std;

class pTorneo{
    private:
        float probabilidad[16];
        float suma;
        float max;
        int individuosB[16][5];
        int individuosE[16];
        int individuosBC[16][5];
        int individuosBD[16][5];
        float aptitud[16];
        int barajeo1[16];
        int barajeo2[16];
        int padres[16];
        int contador;
        int miembros;
        int genes;
        int pcruza;
        int mutar;
    public:
        pTorneo(int,int,int,int,int);
        int generaciones;
        float mayores[100];
        float menores[100];
        void creaIndividuos();
        void convBinario(int,int);
        void calcAptitud();
        float fitness(float);
        void calcProbabilidad();
        void barajeo();
        bool flip(float);
        void cruza();
        void mutacion();
        void maxmin();
        int convEntero(int);
        float calcProbabilidadD(int);
        void reasignar();
        void getmaxmin();
        void Tabla1(int);
        void Tabla2(int);
        void Tabla3(int);
        void Tabla4(int);
        float getSuma(float*);
        int getMaxI(int*);
        int getMinI(int*);
        float getMaxF(float*);
        float getMinF(float*);
        float getMayores(int);
        float getMenores(int);
};

pTorneo::pTorneo(int imiembros,int igenes,int igeneraciones,int ipcruza,int imutar){
    miembros = imiembros;
    genes = igenes;
    generaciones = igeneraciones;
    pcruza = ipcruza;
    mutar = imutar;
    contador=0;
}

void pTorneo::creaIndividuos(){
    int limite=0,random;
    int individuos[16];
    for(int i=0;i<genes;i++){
        limite += pow(2,i);
    }
    for(int i=0;i<miembros;i++){
        random =  rand() % limite + 1;//numero para el individuo
        individuosE[i]= random;
        convBinario(random,i);
    }
}

void pTorneo::convBinario(int numero,int valori){
    int dividendo, resto, divisor = 2,cont = 1,j=genes-1;
    for(int i=0;i<genes;i++){
        individuosB[valori][i] = 0;
    }
    dividendo = numero;
    while(dividendo >= divisor){
    resto = dividendo % 2;
    if(resto == 1)
        individuosB[valori][j] = 1;
    else
       individuosB[valori][j] = 0;
       dividendo = dividendo/divisor;
       cont++;
       j--;
    }
    if(dividendo == 1)
       individuosB[valori][j] = 1;
    else
       individuosB[valori][j] = 0;
}

void pTorneo::calcAptitud(){
    suma = 0;
    for(int i=0;i<miembros;i++){
        aptitud[i] = fitness(float(individuosE[i]));
        suma += aptitud[i];
    }
}

float pTorneo::fitness(float individuo){
    float res=0;
    res= float(individuo-5)/float(2+sin(individuo*PI/180));
    if(res<0)
        return res*-1;
    else
        return res;
}

void pTorneo::calcProbabilidad(){
    for(int i=0;i<miembros;i++){
        probabilidad[i] = aptitud[i]/(float)suma;
    }
}

void pTorneo::barajeo(){

    barajeo1[0] = -1;
    barajeo2[0] = -1;
    float p=0;
    int comodin,contador1=0,ban=0,contador2=0;
    while(contador1<16){
        comodin = rand() % miembros;
        for(int j=0;j<contador1;j++){
            if(barajeo1[j] == comodin){
                ban=1;
                break;
            }
        }
        if(ban==0){
            barajeo1[contador1] = comodin;
            contador1++;
        }
        ban=0;
    }
    contador1 = 0;
    while(contador1<16){
        comodin = rand() % miembros;
        for(int j=0;j<contador1;j++){
            if(barajeo2[j] == comodin){
                ban=1;
                break;
            }
        }
        if(ban==0){
            barajeo2[contador1] = comodin;
            contador1++;
        }
        ban=0;
    }
    for(int i=0;i<(miembros-1);i++){
        p = (float) rand()/RAND_MAX; //el valor de 0.7<=p<=1

        if(flip(p)){//flip sale TRUE
            if(barajeo2[i]>barajeo2[i+1]) //Asiganmos el que tenga mayor jerarquía
                padres[contador2] = barajeo1[i];
            else
                padres[contador2] = barajeo1[i+1];

        }else if(!flip(p)){//flip sale FALSE
            if(barajeo2[i]<barajeo2[i+1]) //Asiganmos el que tenga menor jerarquía
                padres[contador2] = barajeo1[i];
            else
                padres[contador2] = barajeo1[i+1];

        }
        contador2++;
        i++;
    }
    for(int i=0;i<(miembros-1);i++){
        p = (float) rand()/RAND_MAX; //el valor de 0.7<=p<=1

        if(flip(p)){//flip sale TRUE
            if(barajeo2[i]>barajeo2[i+1]) //Asiganmos el que tenga mayor jerarquía
                padres[contador2] = barajeo2[i];
            else
                padres[contador2] = barajeo2[i+1];

        }else if(!flip(p)){//flip sale FALSE
            if(barajeo2[i]<barajeo2[i+1]) //Asiganmos el que tenga menor jerarquía
                padres[contador2] = barajeo2[i];
            else
                padres[contador2] = barajeo2[i+1];

        }
        contador2++;
        i++;
    }
}

bool pTorneo::flip(float p){
    if(p<0.7)
        return false;
    else
        return true;
}

void pTorneo::cruza(){

    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            individuosBC[i][j] = individuosB[padres[i]][j];
        }
    }
    for(int i=0;i<miembros-1;i++){
        for(int k=0;k<pcruza;k++){
            individuosBD[i][k] = individuosBC[i][k];
            individuosBD[i+1][k] = individuosBC[i+1][k];
        }
        for(int j=pcruza;j<genes;j++){
            individuosBD[i][j] = individuosBC[i+1][j];
            individuosBD[i+1][j] = individuosBC[i][j];
        }
        i++;
    }
}

void pTorneo::mutacion(){
    int imutar,gmutar;
    for(int i=0;i<mutar;i++){
        imutar = rand() % (miembros-1) + 1;
        for(int j=0;j<genes;j++){
            if(individuosBD[imutar][j] == 0){
                individuosBD[imutar][j] = 1;
                break;
            }
        }
    }
}

void pTorneo::maxmin(){
    int tmax,tmin,temp;
    tmax = convEntero(0);
    tmin = convEntero(0);
    for(int i=1;i<miembros;i++){
        temp = convEntero(i);
        if(temp > tmax){
            tmax = 	temp;
        }
        if(temp < tmin){
            tmin = temp;
        }
    }
    mayores[contador] = calcProbabilidadD(tmax);
    menores[contador] = calcProbabilidadD(tmin);
    contador++;
}

int pTorneo::convEntero(int conv){
    int regresa = 0,exponente=0;
    for(int i=genes-1;i>=0;i--){
        if(individuosBD[conv][i] == 1){
            regresa += pow(2,exponente);
        }
        exponente++;
    }
    return regresa;
}

float pTorneo::calcProbabilidadD(int num){
    int sum=0;
    for(int i=0;i<miembros;i++){
        sum += convEntero(i);
    }
    return num/(float)sum;
}

void pTorneo::reasignar(){
    //individuosB,individuosE,individuosBD
    //individuosBD -> individuosB;
    //individuosBD -> convEntero ->individuosE;
    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            individuosB[i][j] = individuosBD[i][j];
        }
        individuosE[i] = convEntero(i);
    }
}

void pTorneo::getmaxmin(){
    for(int i=0;i<generaciones;i++){
        cout << "max: " << mayores[i] << endl;
        cout << "min: " << menores[i] << endl;
        cout << endl;
    }
}


void pTorneo::Tabla1(int generacion){
    ofstream archivo;
    archivo.open("Tabla1.txt",ios::app);
    archivo << generacion << "a generacion | Tabla 1" << endl;
    archivo << "Aptitud F(x)=abs|(x-5)/(2+Sen(x))|"<<endl;
    archivo << "No. | Poblacion Inicial | Valor x | Aptitud |Probabilidad"<<endl;
    archivo << "____________________________________________________________"<<endl;
    string binario[miembros];
    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            if(individuosB[i][j]==0)
                binario[i]+='0';
            else
                binario[i]+='1';
        }
    }
    for(int i = 0; i < miembros;i++){
        archivo<<(i+1)<<setw(4)<< "|"<<setw(12)<< binario[i]<< setw(8)<<"|"<< setw(5)<<individuosE[i]<<setw(5)<<"|"<<setw(7)<<aptitud[i]<<setw(3) <<"|"<< setw(5)<<probabilidad[i]<<endl;
    }
    float promedio= 1/ (float)miembros;
    archivo << "____________________________________________________________"<<endl;
    archivo<<"Suma"<<setw(32)<<"|"<<setw(8)<<suma<<setw(2)<<"|"<<"1"<<endl;
    archivo<<"Promedio"<<setw(28)<<"|"<<setw(8)<<(suma/miembros)<<setw(2)<<"|"<<promedio<<endl;
    archivo<<"Max"<<setw(33)<<"|"<<setw(8)<<getMaxF(aptitud)<<setw(2)<<"|"<<getMaxF(probabilidad)<<endl;
    archivo<<"Min"<<setw(33)<<"|"<<setw(8)<<getMinF(aptitud)<<setw(2)<<"|"<<getMinF(probabilidad)<<endl;
    archivo<<endl<<endl;
    archivo.close();
}

void pTorneo::Tabla2(int generacion){

    ofstream archivo;
    archivo.open("Tabla2.txt",ios::app);
    archivo << generacion << "a generacion | Tabla de seleccion de padres" << endl;
    archivo << "Aptitud F(x)=abs|(x-5)/(2+Sen(x))|"<<endl;
    archivo << "No. | Poblacion Inicial | Aptitud |Barajeo 1| Barajeo 2"<<endl;
    archivo << "_________________________________________________________"<<endl;
    string binario[miembros];
    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            if(individuosB[i][j]==0)
                binario[i]+='0';
            else
                binario[i]+='1';
        }
    }
    for(int i = 0; i < miembros;i++){
        archivo<<(i+1)<<setw(4)<< "|"<<setw(12)<< binario[i]<<setw(8)<<"|"<<setw(7)<<aptitud[i]<<setw(3) <<"|"<< setw(5)<<(barajeo1[i]+1)<<setw(5)<<"|" <<setw(5)<<(barajeo2[i]+1)<<endl;
    }
    archivo<<endl<<endl;
    archivo.close();
}


void pTorneo::Tabla3(int generacion){
    ofstream archivo;
    archivo.open("Tabla3.txt",ios::app);
    archivo << generacion << "a generacion | Tabla 3" << endl;
    archivo << "Aptitud F(x)=abs|(x-5)/(2+Sen(x))|"<<endl;
    archivo << "No. |    Cruza    |Pto. de cruza| Descendencia | Valor x | Aptitud "<<endl;
    archivo << "____________________________________________________________________"<<endl;
    string binarioC[miembros], binarioD[miembros];
    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            if(individuosBC[i][j]==0)
                binarioC[i]+='0';
            else
                binarioC[i]+='1';

            if(individuosBD[i][j]==0)
                binarioD[i]+='0';
            else
                binarioD[i]+='1';

        }
    }
    float binD[miembros];
    for(int i = 0; i < miembros;i++){
        archivo<<(i+1)<<setw(4)<< "|"<<setw(10)<< binarioC[i]<< setw(4)<<"|"<<setw(6) <<pcruza<<setw(7)<<" |"<<setw(10)<<binarioD[i]<<setw(6)<<"|"<<setw(5)<<convEntero(i)<<setw(5)<<"|"<<setw(5)<<fitness(float(convEntero(i)))<<endl; //SE TIENE QUE OBTENER EL VALOR
        binD[i]=fitness(float(convEntero(i)));
    }
    float promedio= getSuma(binD)/ (float)miembros;
    archivo << "____________________________________________________________________"<<endl;
    archivo<<"Suma"<<setw(55)<<"|"<<setw(8)<<getSuma(binD)<<endl;
    archivo<<"Promedio"<<setw(51)<<"|"<<setw(8)<<promedio<<endl;
    archivo<<"Max"<<setw(56)<<"|"<<setw(8)<<getMaxF(binD)<<endl;
    archivo<<"Min"<<setw(56)<<"|"<<setw(8)<<getMinF(binD)<<endl;
    archivo<<endl<<endl;
    archivo.close();
}



void pTorneo::Tabla4(int generacion){ //Se tiene que mutar aqui porque el arreglo individuosBD se modifica
    if(generacion==1 || generacion==generaciones){
        ofstream archivo;
        archivo.open("Tabla4.txt",ios::app);
        archivo << generacion << "a generacion | Tabla 4" << endl;
        archivo << "Aptitud F(x)=abs|(x-5)/(2+Sen(x))|"<<endl;
        archivo << "No. | Descendencia | Mutacion | Valor x | Aptitud "<<endl;
        archivo << "____________________________________________________"<<endl;
        string binarioD[miembros], binarioDM[miembros];
        for(int i=0;i<miembros;i++){
            for(int j=0;j<genes;j++){
                if(individuosBD[i][j]==0)
                    binarioD[i]+='0';
                else
                    binarioD[i]+='1';

            }
        }
        mutacion();
        for(int i=0;i<miembros;i++){
            for(int j=0;j<genes;j++){
                if(individuosBD[i][j]==0)
                    binarioDM[i]+='0';
                else
                    binarioDM[i]+='1';

            }
        }

        float binD[miembros];
        for(int i = 0; i < miembros;i++){
            archivo<<(i+1)<<setw(4)<< "|"<<setw(10)<< binarioD[i]<< setw(4)<<"|"<<setw(6)<<binarioDM[i]<<setw(6)<<"|"<<setw(5)<<convEntero(i)<<setw(5)<<"|"<<setw(5)<<fitness(float(convEntero(i)))<<endl; //SE TIENE QUE OBTENER EL VALOR
            binD[i]=fitness(float(convEntero(i)));
        }


        float promedio= getSuma(binD)/ (float)miembros;
        archivo << "____________________________________________________"<<endl;
        archivo<<"Suma"<<setw(38)<<"|"<<setw(8)<<getSuma(binD)<<endl;
        archivo<<"Promedio"<<setw(34)<<"|"<<setw(8)<<promedio<<endl;
        archivo<<"Max"<<setw(39)<<"|"<<setw(8)<<getMaxF(binD)<<endl;
        archivo<<"Min"<<setw(39)<<"|"<<setw(8)<<getMinF(binD)<<endl;
        archivo<<endl<<endl;
        archivo.close();
    }else{
        mutacion();
    }
}

float pTorneo::getSuma(float *aux){
    float suma=0;
    for(int i=0;i<miembros;i++)
        suma+=aux[i];
    return suma;
}

int pTorneo::getMaxI(int *aux){
    int max=aux[0];
    for(int i=0;i<miembros;i++)
        if(aux[i] > max)
            max = aux[i];
    return max;
}

int pTorneo::getMinI(int *aux){
    int min=aux[0];
    for(int i=0;i<miembros;i++)
        if(aux[i] < min)
            min = aux[i];
    return min;
}

float pTorneo::getMaxF(float *aux){
    float max=aux[0];
    for(int i=0;i<miembros;i++)
        if(aux[i] > max)
            max = aux[i];
    return max;
}

float pTorneo::getMinF(float *aux){
    float min=aux[0];
    for(int i=0;i<miembros;i++)
        if(aux[i] < min)
            min = aux[i];
    return min;
}
float pTorneo::getMayores(int indice){
    return mayores[indice];
}
float pTorneo::getMenores(int indice){
    return menores[indice];
}


