//**********************************************************************************************************************
//S�o utilizadas as teclas 2, 4, 5, 6 e 8. O display dever� estar sempre atualizado com a situa��o atual do motor. Portanto, devem ser sembre exibidas a dire��o de giro e a velocidade, exceto quando o motor estiver parado.
//Neste caso dever� ser exibida apenas a mensagem "Parado".

//As teclas e suas fun��es s�o:

//Tecla  Fun��o							
//5      parar o motor		
//4      girar para a esquerda	
//6      girar para a direita	
//2      velocidade++ 		
//8      velocidade-- 
 
//Logo ap�s o RESET o motor deve estar parado e o valor da velocidade deve ser igual a 5.
//************************************************************************************************************************
#include "io430.h"

// FUN��ES EXTERNAS EM ASSEMBLY (DISPLAY)

extern void DISPLAY (void);
extern void CARREGA_DADO(unsigned char TECLA);
extern void CONF_DISPLAY(void);
extern void ENVIA_COMANDO(void);
extern void ENVIA_DADO(void);


//EXTERNAS EM C (MOTOR)

   //fun��es
extern void MOTOR (void);
extern void CONF_MOTOR(void);
extern void TECLA_MOTOR();
extern void RIGHT();
extern void LEFT();

    // strings 
extern char PARADO[];
extern char VELOCIDADE[1];
extern char SPEED[];
 
    // vari�veis
extern char velocidade;
extern unsigned char V_DISPLAY;
extern char ANTERIOR;


//EXTERNAS EM C (TECLADO)

   // fun��es

extern void TECLADO (void);
extern void CONF_TECLADO(void);
 
   // vari�veis
extern char TECLA;


int main( void )
{
  // Stop watchdog timer to prevent time out reset
   WDTCTL = WDTPW + WDTHOLD;

  //CONFIGURA TECLADO
   CONF_TECLADO();


  // CONFIGURA MOTOR
  CONF_MOTOR();

       
 // CONFIGURA DISPLAY

   CONF_DISPLAY();
   asm("EINT");             // habilitar interrup��o geral

 // INICIO EFETIVO DO PROGRAMA  
   
   P3OUT|=0x0F;                    // Inicializa as sa�das P3.0 a P3.3(linhas do teclado) em n�vel alto   
    unsigned int p=0;
      for(p=0;p<5;p++)             // enquanto n�o chegar o fim da string, P5OUT(sa�da do display) recebe cada valor da string
      {
       P5OUT=PARADO[p];           // PARADO � o vetor de estado parado do motor e a vari�vel :'p' � o �ndice do vetor
       ENVIA_DADO();               // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente
      }
      
        unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
          V_DISPLAY=SPEED[velocidade];        // a vari�vel V_DISPLAY recebe a posi��o do vetor SPEED indicada pela vari�vel velocidade 
    CARREGA_DADO(V_DISPLAY);               // carrega a vari�vel V_DISPLAY no displaY
   
  ////////
  
   while(1)
   {
      TECLA_MOTOR(); 
      
     if(ANTERIOR=='4')             // enquanto o valor da vari�vel ANTERIOR que armazena o �ltimo sentido de giro definido for = 4(direita):
    { 
     RIGHT();            // motor gira para a direita
    }
    if(ANTERIOR=='6')              // enquando o valor da vari�vel ANTERIOR que armazena o �ltimo sentido de giro definido for = 6(esquerda):
    {
     LEFT();           // motor gira para a esquerda
    }
  
  ///mudouu
   
     
      if(ANTERIOR=='5')
      {
      P1OUT=0x00;
      }
   }
}



