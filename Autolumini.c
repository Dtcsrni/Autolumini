/*
*Autolumini V1.0
*Autor: Erick Renato Vega Cerón
*4 de Agosto de 2019
*
*Este software controlará un sistema de control automático de luces en los baños de una universidad,
*de tal forma que pueda detectar por medio de 4 sensores ultrasónicos si una persona ha entrado o salido,
*apagando o encendiendo las luces adecuadamente.
*El MCU entra en modo ahorro de energía cuando detecta que no se encuentra nadie dentro de los baños.
*
*/



void Rele(int rele, int estado);
float LeerUltrasonico(int puerto);
 
void main() {
//Declaración de variables a usar
int PilaPersonasH;
int PilaPersonasM;
int RE0;
int RE1;


//Definición de puertos
    //Definición de puerto D (puertos de entrada para leer la señal "echo" de cada sensor ultrasónico)
       ANSEL = 0b00000000; // Pines analógicos como digitales
       ANSELH = 0;
       PORTB = 0;                     //Se limpia el puerto
       TRISB = 0b00000000; // PORTB.0, PORTB.1, PORTB.2 y PORTB.3 son configurados como entradas. Los demás como salidas
       PORTD = 0;                     //Se limpia el puerto
       TRISD = 0b00001111; // PORTD.0, PORTD.1, PORTD.2 y PORTD.3 como entradas
       PORTE = 0;                     //Se limpia el puerto
       TRISD = 0b00000000; // PORTE como salidas
       T1CON = 0x10; //inicializar el módulo timer
       TMR1H = 0;                  //Se establece el valor inicial del timer en su parte alta
       TMR1L = 0;                  //Se establece el valor inicial del timer en su parte baja

    while(1){
             //Se revisa si la pila de personas del baño de hombres es mayor a cero, o sea, si hay personas dentro
             if(PilaPersonasH>0){
                 RE1=1;              //Si las hay entonces se manda a 1 la variable RE1 correspondiente al Relé 1
             }
             else{
                 RE1=0;              //Si no hay personas, el relé se manda a 0
             }
             if(PilaPersonasM>0){    //Se revisa si la pila de personas del baño de mujeres es mayor a cero, o sea, si hay personas dentro
                 RE0=1;              //Si las hay entonces se manda a 1 la variable RE1 correspondiente al Relé 1
             }
             else{
                 RE0=0;              //Si no hay personas, el relé se manda a 0
             }
          
            //Se activan o desactivan los relés de acuerdo a sus variables
             if(RE0==1){
                  Rele(0,1);
             }
             else{
                  Rele(0,0);
             }
             if(RE1==1){
                 Rele(1,1);
             }
             if(RE1==0){
                 Rele(1,0);
             }

             if(LeerUltrasonico(0)==1 || LeerUltrasonico(2)==1){ //Leer sensores de RD0 y RD2 y verificar si se detecta obstáculo (retorno es 1)
                          //
             
             }

    }

}

void Rele(int rele, int estado){


}

int LeerUltrasonico(int puerto){           //0 es el puerto RD0, 1 es RD1, 2 es RD2, 3 es RD3
      long retorno=0, distancia, tiempo;   //variables a utilizar
       //se manda pulso alto al puerto trigger del sensor indicado en el argumento
      switch(puerto){
        case 0:
        PORTB.RB0=1;
        break;
        case 1:
         PORTB.RB1=1;
        break;
        case 2:
        PORTB.RB2=1;
        break;
        case 3:
        PORTB.RB3=1;
        break;
      }
      Delay_us(10);    //se mantiene por 10 microsegundos
      //y luego se manda pulso bajo
      switch(puerto){
        case 0:
        PORTB.RB0=0;
        break;
        case 1:
         PORTB.RB1=0;
        break;
        case 2:
        PORTB.RB2=0;
        break;
        case 3:
        PORTB.RB3=0;
        break;
      }
      //después se espera la señal desde el sensor ultrasónico en el puerto de echo correspondiente
      switch(puerto){
        case 0:
        while(!PORTD.RD0);
        break;
        case 1:
         while(!PORTD.RD1);
        break;
        case 2:
        while(!PORTD.RD2);
        break;
        case 3:
        while(!PORTD.RD3);
        break;
      }

       //se inicia el timer
       T1CON.F0 = 1;
       //Y después se espera hasta que la señal del echo termine
       switch(puerto){
        case 0:
        while(PORTD.RD0);
        break;
        case 1:
        while(PORTD.RD1);
        break;
        case 2:
        while(PORTD.RD2);
        break;
        case 3:
        while(PORTD.RD3);
        break;
      }
       //luego entonces se detiene el timer
       T1CON.F0 = 0;
       
       tiempo = (TMR1L | (TMR1H<<8));   //Se lee entonces el timer
       
       distancia = tiempo/58.82;    //se calcula la distancia
       if(distancia<100)  {     //se verifica si la distancia sensada es menor a 1 metro
          retorno = 1;                 //si lo es entonces retorno = 1
       }
       else{
        retorno = 0;  //en caso de que no, retorno = 0
       }
       
 return retorno;    //se retorna el resultado
}
