#include "io430.h"

// vari�veis globais

unsigned char V_DISPLAY=0;  // vari�vel global VARIAVEL_DISPLAY, utilizada para enviar comandos ao displya.

unsigned char velocidade=5;    // vari�vel global velocidade,utilizada como �ndice do vetor SPEED.

unsigned char ANTERIOR=0;          // inicializa vari�vel que recebe valor anterior do sentido de giro do motor

// inicializa��o de vetor e strings

const char SPEED[10]={'0','1','2','3','4','5','6','7','8','9'}; // vetor SPEED, cont�m os valores de velocidade dispon�veis para o motor de passo utilizado

const char PARADO[]= "STOP "; // inicializa��o da string de motor parado

const char ESQUERDA[]= "LEFT "; // inicializa��o da string de motor girando para a esquerda

const char DIREITA[]= "RIGHT "; // inicializa��o da string de motor girando para a direita 
 
const char VELOCIDADE[]= "SPEED=";          // inicializa��o da string de velocidade do motor


// FUN��ES EXTERNAS EM ASSEMBLY (DISPLAY)

extern void DISPLAY (void);
extern void CARREGA_DADO(unsigned char TECLA);
extern void ENVIA_COMANDO(void);
extern void ENVIA_DADO(void);

//EXTERNAS EM C (TECLADO)
   // vari�veis
extern char TECLA;

//*----------------------------------------------------------------
// Fun��o: CONF_MOTOR
// Descri��o: Configura IOs do teclado
// Entrada: 
// Sa�da: -
//*---------------------------------------------------------------- 
void CONF_MOTOR(void)
{
   P1SEL&=0xF0;                     // Associa p1.0 a p1.3 as portas 
   P1DIR|=0x0F;                     // Configura P1.0 a P1.3 como sa�das
}

//*------------------------------------------------------------------------------------------------------------------------------
// Fun��o: Delay
// Descri��o: Fornece tempo de atraso necess�rio para a execu��o ou estabiliza��o de certa atividade.
// Entrada: Vari�vel x.Corresponde ao valor em milisegundos do tempo a ser executado.
// Sa�da: -
//*------------------------------------------------------------------------------------------------------------------------------
void Delay (unsigned int x)
{
  unsigned int y;                  // inicializa vari�vel y                     
  while(x!=0)                      // enquanto x n�o for 0:
  {
    for(y=0;y<187;y++);            //incremento de y, come�ando pelo valor 0 at� o valor 186  
    x--;                           // decrementa x
  }
}
//*-----------------------------------------------------------------------------------------------------------------------------
// Fun��o: RIGHT
// Descri��o: Faz com que o motor (porta1-P1.0 a P1.3) gire para a direita
// Entrada: vari�vel velocidade, a qual indica com qual velocidade o motor estar� girando
// Sa�da: -
//*------------------------------------------------------------------------------------------------------------------------------
void RIGHT()
{
  P1OUT=0x0C;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          1 1 0 0
  
  Delay(11-velocidade);         // realiza c�lculo do tempo entre cada passo do motor
  
  P1OUT=0x09;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          1 0 0 1
  
  Delay(11-velocidade);         // realiza c�lculo do tempo entre cada passo do motor
  
  P1OUT=0x03;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          0 0 1 1
  
  Delay(11-velocidade);         // realiza c�lculo do tempo entre cada passo do motor
  
  P1OUT=0x06;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          0 1 1 0
  
  Delay(11-velocidade);         // realiza c�lculo do tempo entre cada passo do motor  
}

//*-----------------------------------------------------------------------------------------------------------------------------
// Fun��o: LEFT
// Descri��o: Faz com que o motor (porta1-P1.0 a P1.3) gire para a esquerda
// Entrada: vari�vel velocidade, a qual indica com qual velocidade o motor estar� girando
// Sa�da: -
//*------------------------------------------------------------------------------------------------------------------------------
void LEFT()
{
  P1OUT=0x0C;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          1 1 0 0
  
  Delay(11-velocidade);         // realiza c�lculo do tempo entre cada passo do motor
  
  P1OUT=0x06;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          0 1 1 0
  
  Delay(11-velocidade);         // realiza c�lculo do tempo entre cada passo do motor
  
  P1OUT=0x03;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          0 0 1 1
  
  Delay(11-velocidade);         // realiza c�lculo do tempo entre cada passo do motor
  
  P1OUT=0x09;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          1 0 0 1
  
  Delay(11-velocidade);         // realiza c�lculo do tempo entre cada passo do motor  
}
//*-----------------------------------------------------------------------------------------------------------------------------
// Fun��o: ULTIMO_GIRO
// Descri��o: Identifica qual foi a �ltima op��o de giro selecionada
// Entradas: vari�vel ANTERIOR (vari�vel que cont�m o valor da �ltima tecla de sentido de giro selecionada) e velocidade(�ndice do vetor SPEED)
//           strings ESQUERDA e DIREITA( informes dos respectivos sentidos de giro)
// Sa�da: -
//*------------------------------------------------------------------------------------------------------------------------------
void ULTIMO_GIRO(unsigned char ANTERIOR, unsigned char velocidade)
{
    if(ANTERIOR=='4')                     // se o �ltimo sentido de giro selecionado foi 4(para esquerda)
    { 
      unsigned int e=0;                 // inicializa vari�vel e
      for(e=0;e<5;e++)                // enquanto n�o chegar o fim da string, P5OUT(sa�da do display) recebe cada valor da string
      {
       P5OUT=ESQUERDA[e];              // ESQUERDA � o vetor de sentido de giro para a esquerda e a vari�vel :'e' � o �ndice do vetor
       ENVIA_DADO();                  // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente 
      }
    }
    else if(ANTERIOR=='6')        // se o �ltimo sentido de giro selecionado foi 6(para direita)
    {
      unsigned int d=0;           // enquanto n�o chegar o fim da string, P5OUT(sa�da do display) recebe cada valor da string
      for(d=0;d<6;d++)
      {
       P5OUT=DIREITA[d];           // DIREITA � o vetor de sentido de giro para a direita e a vari�vel :'d' � o �ndice do vetor
       ENVIA_DADO();              // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente 
      }
    }
    else                        // se n�o houver sentido de giro selecionado, motor permanece parado
    {
     unsigned int p=0;
      for(p=0;p<5;p++)               // enquanto n�o chegar o fim da string, P5OUT(sa�da do display) recebe cada valor da string
      { 
       P5OUT=PARADO[p];              // PARADO � o vetor de estado parado do motor e a vari�vel :'p' � o �ndice do vetor
       ENVIA_DADO();                // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente 
      } 
    }
}
//*-----------------------------------------------------------------------------------------------------------------------------
// Fun��o: TECLA_MOTOR
// Descri��o: Identifica qual foi a tecla que est� relacionada ao motor(2,4,6,8 ou 5) que foi pressionada e carrega a respectiva
//instru��o sobre o estado do motor no display. 
// Entradas: vari�veis globais TECLA  e velocidade
// Sa�da: -
//*------------------------------------------------------------------------------------------------------------------------------
void TECLA_MOTOR(void)
{ 
  //INICIALIZA STRINGS E VARI�VEL 
  
 
   if(TECLA=='2')                           // TECLA pressionada � =2?
   {                                       // se sim
    TECLA=0;
    P5OUT=0x01;                           // LIMPA DISPLAY 
    ENVIA_COMANDO();                     // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
   
    if(velocidade<9)                        // estabelece condi��o de velocidade m�xima
    {
        velocidade++;                           // h� incremento da velocidade
    }

    ULTIMO_GIRO(ANTERIOR,velocidade); // qual o �ltimo foi a �ltima op��o de sentido de giro?
    
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
    
    V_DISPLAY=SPEED[velocidade];        // a vari�vel V_DISPLAY recebe a posi��o do vetor SPEED indicada pela vari�vel velocidade 
    CARREGA_DADO(V_DISPLAY);               // carrega a vari�vel V_DISPLAY no display

   }  
   else if(TECLA=='8')                         // TECLA pressionada � =8?
    {                                         // se sim
      TECLA=0;
      P5OUT=0x01;                       // LIMPA DISPLAY
      ENVIA_COMANDO();                  // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
      
      velocidade--;                       // h� decremento da velocidade
      
      if(velocidade<1)                        // estabelece condi��o de velocidade m�nima
    {
      velocidade=1;                         // se o aumento foi maior que a condi��o m�nimo, velocidade permanece no valor m�nimo
    }
    
    ULTIMO_GIRO(ANTERIOR,velocidade);   // qual o �ltimo foi a �ltima op��o de sentido de giro?
     
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
      
    V_DISPLAY=SPEED[velocidade];        // a vari�vel V_DISPLAY recebe a posi��o do vetor SPEED indicada pela vari�vel velocidade 
    CARREGA_DADO(V_DISPLAY);               // carrega a vari�vel V_DISPLAY no displaY
   
    }
   
   else if(TECLA=='4')                       // TECLA pressionada � =4?
   {                                        // se sim
     
     P5OUT=0x01;                            // LIMPA DISPLAY
     ENVIA_COMANDO();                      // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
     
     
   ANTERIOR=TECLA;                      // vari�vel anterior armazena o ultimo sentido de giro requerido
     TECLA=0;
   unsigned int e=0; 
      for(e=0;e<5;e++)                   // enquanto n�o chegar o fim da string, P5OUT(sa�da do display) recebe cada valor da string
      {
       P5OUT=ESQUERDA[e];                // ESQUERDA � o vetor de sentido de giro para a esquerda e a vari�vel :'e' � o �ndice do vetor
       ENVIA_DADO();                       // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente
      }
  
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
    
     V_DISPLAY=SPEED[velocidade];        // a vari�vel V_DISPLAY recebe a posi��o do vetor SPEED indicada pela vari�vel velocidade 
     CARREGA_DADO(V_DISPLAY);               // carrega a vari�vel V_DISPLAY no display
     
   }
   else if(TECLA=='6')                       // TECLA pressionada � =6?
    {
     
      P5OUT=0x01;                       // LIMPA DISPLAY
      ENVIA_COMANDO();                  // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
      
      ANTERIOR=TECLA;                   // vari�vel anterior armazena o ultimo sentido de giro requerido
      TECLA=0;
      unsigned int d=0;
      for(d=0;d<6;d++)               // enquanto n�o chegar o fim da string, P5OUT(sa�da do display) recebe cada valor da string
      {
       P5OUT=DIREITA[d];               // DIREITA � o vetor de sentido de giro para a direita e a vari�vel :'d' � o �ndice do vetor
       ENVIA_DADO();                   // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente
      }
      
  
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
    
     V_DISPLAY=SPEED[velocidade];        // a vari�vel V_DISPLAY recebe a posi��o do vetor SPEED indicada pela vari�vel velocidade 
     CARREGA_DADO(V_DISPLAY);               // carrega a vari�vel V_DISPLAY no display
    }
  
   else if(TECLA=='5')                         // TECLA pressionada � =5
    { 
     
      P5OUT=0x01;                       // LIMPA DISPLAY
      ENVIA_COMANDO();                  // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
      
      ANTERIOR=TECLA;
      TECLA=0;
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
       CARREGA_DADO(V_DISPLAY);               // carrega a vari�vel V_DISPLAY no display
       
    }
   

    }
   
 

