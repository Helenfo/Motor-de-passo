// Este programa � respons�vel por identificar qual foi a tecla pressionada de um teclado.

#include "io430.h"

unsigned char TECLA=0;           // vari�vel global TECLA. utilizada para armazenar a tecla pressionada no teclado relacionada ao motor.

extern void Delay(unsigned int x);

extern unsigned char ANTERIOR;
//*----------------------------------------------------------------
// Fun��o: CONF_TECLADO
// Descri��o: Configura IOs do teclado
// Entrada: 
// Sa�da: -
//*---------------------------------------------------------------- 
void CONF_TECLADO(void)
{
  P2SEL&=0xF8;                             // Associa P2.0 a P2.2 �s portas
  P3SEL&=0xF0;                            // Associa P3.0 a P3.3 �s portas
  P3DIR|=0x0F;                           // Configura P3.0 a P3.3 como sa�das
  P2DIR&=0xF8;                          // Configura P2.0 a P2.2 como entrada
  P2IE|=0x07;                          // Habilita interrup��o de P2.0 a P2.2
  P2IES&=0xF8;                        // Define como borda de subida
}

//*----------------------------------------------------------------
// Fun��o: Colunas1
// Descri��o: Identifica qual coluna foi ativada.
// Entrada: ponteiro APT_TEC do vetor TAB_TEC e vari�vel velocidade
// Sa�da: -
//*---------------------------------------------------------------- 
void Colunas1 (unsigned char *APT_TEC)
{ 
  if(P2IN&0x01)                        //Coluna0(P3.0) est� ativada?
  {                                   // se sim
    APT_TEC=APT_TEC+1;               // aponta para pr�ximo valor do vetor, ou seja, para a pr�xima tecla
  }
  else if(P2IN&0x02)                  //Coluna1(P3.1) est� ativada?
  {                                  // se sim
   APT_TEC=APT_TEC+2;               // aponta para para valor do vetor a 2 posi��es
  }
  else if(P2IN&0x04)                  //Coluna2(P3.2) est� ativada?
  {                                  // se sim
   APT_TEC=APT_TEC+3;               // aponta para para valor do vetor a 3 posi��es
  }
  else
  {
    APT_TEC=0;                 // apontador � posicionado na primeira posi��o do vetor de teclas
  }
   TECLA= *APT_TEC;                  // vari�vel global TECLA recebe o conte�do do apontador   
   P3OUT|=0x0F;              // Configura as sa�das P3.0 a P3.3 em n�vel alto
   P2IFG&=0xF8;             // reseta flag de interrup��o
}

//DEFINE A FUN��O DE TRATAMENTO DE INTERRUP��O DA PORTA P2.0
#pragma vector=PORT2_VECTOR  // DEFINE O ENDERE��O DA RTI DA PORTA 2
__interrupt void PORT2 (void)
{
  unsigned char TAB_TEC[]={0x00,'3','2','1','6','5','4','9','8','7','E','0','A'}; // inicializa��o do vetor de teclas
  unsigned char *APT_TEC;      // inicializa apontador do vetor de teclas
  APT_TEC=TAB_TEC;             //apontador de teclas aponta para a primeira posi��o do vetor de teclas
  
  Delay(5);                  // Tempo de repique
  
  if(P2IN&0x07)            // Todas colunas=1?
  {                           // se sim
   P3OUT&=0xF1;              // L0=1(P3.0); L1=0(P3.1); L2=0(P3.2); L3=0(P3.3)
  }
  else
  {
    P2IFG&=0xF8;             // reseta flags de interrup��o
  }
  if(P2IN&0x07)            // Todas colunas=1?
  {                       // se sim
      Colunas1(APT_TEC);
  }
  else
  {
    P3OUT|=0x02;                // L1=1(P3.1)            
    P3OUT&=0xF2;               // L0=0(P3.0); L2=0(P3.2); L3=0(P3.3)
    APT_TEC=APT_TEC+3;        // aponta para para valor do vetor a 3 posi��es 
  }
  
  if(P2IN&0x07)            // Todas colunas=1?
  {                       // se sim
    Colunas1(APT_TEC);
  }
  else
  {
    P3OUT|=0x04;                // L2=1(P3.2)          
    P3OUT&=0xF4;               // L0=0(P3.0); );L1=0(P3.1); L3=0(P3.3)
    APT_TEC=APT_TEC+3;        // aponta para para valor do vetor a 3 posi��es  
  }
  
  if(P2IN&0x07)            // Todas colunas=1?
  {                       // se sim 
    Colunas1(APT_TEC);
  }
  else
  {
    P3OUT|=0x08;                //  L3=1(P3.3)          
    P3OUT&=0xF8;               // L0=0(P3.0); );L1=0(P3.1); L2=0(P3.2)
    APT_TEC= APT_TEC+3;        // aponta para para valor do vetor a 3 posi��es
  }
  
  if(P2IN&0x07)            // Todas colunas=1?
  {                       // se sim
    Colunas1(APT_TEC);
  }
  else 
  { 
    APT_TEC=0;                 // apontador � posicionado na primeira posi��o do vetor de teclas
    TECLA=*APT_TEC;                  // vari�vel global TECLA recebe o conte�do do apontador   
 
    P3OUT|=0x0F;                 // Configura as sa�das P3.0 a P3.3 em n�vel alto L0 a L3=1
    P2IFG&=0xF8;             // reseta flag de interrup��o
  }
}