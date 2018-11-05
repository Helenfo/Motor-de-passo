#include "io430.h"

// variáveis globais

unsigned char V_DISPLAY=0;  // variável global VARIAVEL_DISPLAY, utilizada para enviar comandos ao displya.

unsigned char velocidade=5;    // variável global velocidade,utilizada como índice do vetor SPEED.

unsigned char ANTERIOR=0;          // inicializa variável que recebe valor anterior do sentido de giro do motor

// inicialização de vetor e strings

const char SPEED[10]={'0','1','2','3','4','5','6','7','8','9'}; // vetor SPEED, contém os valores de velocidade disponíveis para o motor de passo utilizado

const char PARADO[]= "STOP "; // inicialização da string de motor parado

const char ESQUERDA[]= "LEFT "; // inicialização da string de motor girando para a esquerda

const char DIREITA[]= "RIGHT "; // inicialização da string de motor girando para a direita 
 
const char VELOCIDADE[]= "SPEED=";          // inicialização da string de velocidade do motor


// FUNÇÕES EXTERNAS EM ASSEMBLY (DISPLAY)

extern void DISPLAY (void);
extern void CARREGA_DADO(unsigned char TECLA);
extern void ENVIA_COMANDO(void);
extern void ENVIA_DADO(void);

//EXTERNAS EM C (TECLADO)
   // variáveis
extern char TECLA;

//*----------------------------------------------------------------
// Função: CONF_MOTOR
// Descrição: Configura IOs do teclado
// Entrada: 
// Saída: -
//*---------------------------------------------------------------- 
void CONF_MOTOR(void)
{
   P1SEL&=0xF0;                     // Associa p1.0 a p1.3 as portas 
   P1DIR|=0x0F;                     // Configura P1.0 a P1.3 como saídas
}

//*------------------------------------------------------------------------------------------------------------------------------
// Função: Delay
// Descrição: Fornece tempo de atraso necessário para a execução ou estabilização de certa atividade.
// Entrada: Variável x.Corresponde ao valor em milisegundos do tempo a ser executado.
// Saída: -
//*------------------------------------------------------------------------------------------------------------------------------
void Delay (unsigned int x)
{
  unsigned int y;                  // inicializa variável y                     
  while(x!=0)                      // enquanto x não for 0:
  {
    for(y=0;y<187;y++);            //incremento de y, começando pelo valor 0 até o valor 186  
    x--;                           // decrementa x
  }
}
//*-----------------------------------------------------------------------------------------------------------------------------
// Função: RIGHT
// Descrição: Faz com que o motor (porta1-P1.0 a P1.3) gire para a direita
// Entrada: variável velocidade, a qual indica com qual velocidade o motor estará girando
// Saída: -
//*------------------------------------------------------------------------------------------------------------------------------
void RIGHT()
{
  P1OUT=0x0C;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          1 1 0 0
  
  Delay(11-velocidade);         // realiza cálculo do tempo entre cada passo do motor
  
  P1OUT=0x09;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          1 0 0 1
  
  Delay(11-velocidade);         // realiza cálculo do tempo entre cada passo do motor
  
  P1OUT=0x03;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          0 0 1 1
  
  Delay(11-velocidade);         // realiza cálculo do tempo entre cada passo do motor
  
  P1OUT=0x06;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          0 1 1 0
  
  Delay(11-velocidade);         // realiza cálculo do tempo entre cada passo do motor  
}

//*-----------------------------------------------------------------------------------------------------------------------------
// Função: LEFT
// Descrição: Faz com que o motor (porta1-P1.0 a P1.3) gire para a esquerda
// Entrada: variável velocidade, a qual indica com qual velocidade o motor estará girando
// Saída: -
//*------------------------------------------------------------------------------------------------------------------------------
void LEFT()
{
  P1OUT=0x0C;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          1 1 0 0
  
  Delay(11-velocidade);         // realiza cálculo do tempo entre cada passo do motor
  
  P1OUT=0x06;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          0 1 1 0
  
  Delay(11-velocidade);         // realiza cálculo do tempo entre cada passo do motor
  
  P1OUT=0x03;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          0 0 1 1
  
  Delay(11-velocidade);         // realiza cálculo do tempo entre cada passo do motor
  
  P1OUT=0x09;                   // FASES DO SENTIDO HORARIO 1 2 3 4
                                //                          1 0 0 1
  
  Delay(11-velocidade);         // realiza cálculo do tempo entre cada passo do motor  
}
//*-----------------------------------------------------------------------------------------------------------------------------
// Função: ULTIMO_GIRO
// Descrição: Identifica qual foi a última opção de giro selecionada
// Entradas: variável ANTERIOR (variável que contém o valor da última tecla de sentido de giro selecionada) e velocidade(índice do vetor SPEED)
//           strings ESQUERDA e DIREITA( informes dos respectivos sentidos de giro)
// Saída: -
//*------------------------------------------------------------------------------------------------------------------------------
void ULTIMO_GIRO(unsigned char ANTERIOR, unsigned char velocidade)
{
    if(ANTERIOR=='4')                     // se o último sentido de giro selecionado foi 4(para esquerda)
    { 
      unsigned int e=0;                 // inicializa variável e
      for(e=0;e<5;e++)                // enquanto não chegar o fim da string, P5OUT(saída do display) recebe cada valor da string
      {
       P5OUT=ESQUERDA[e];              // ESQUERDA é o vetor de sentido de giro para a esquerda e a variável :'e' é o índice do vetor
       ENVIA_DADO();                  // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente 
      }
    }
    else if(ANTERIOR=='6')        // se o último sentido de giro selecionado foi 6(para direita)
    {
      unsigned int d=0;           // enquanto não chegar o fim da string, P5OUT(saída do display) recebe cada valor da string
      for(d=0;d<6;d++)
      {
       P5OUT=DIREITA[d];           // DIREITA é o vetor de sentido de giro para a direita e a variável :'d' é o índice do vetor
       ENVIA_DADO();              // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente 
      }
    }
    else                        // se não houver sentido de giro selecionado, motor permanece parado
    {
     unsigned int p=0;
      for(p=0;p<5;p++)               // enquanto não chegar o fim da string, P5OUT(saída do display) recebe cada valor da string
      { 
       P5OUT=PARADO[p];              // PARADO é o vetor de estado parado do motor e a variável :'p' é o índice do vetor
       ENVIA_DADO();                // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente 
      } 
    }
}
//*-----------------------------------------------------------------------------------------------------------------------------
// Função: TECLA_MOTOR
// Descrição: Identifica qual foi a tecla que está relacionada ao motor(2,4,6,8 ou 5) que foi pressionada e carrega a respectiva
//instrução sobre o estado do motor no display. 
// Entradas: variáveis globais TECLA  e velocidade
// Saída: -
//*------------------------------------------------------------------------------------------------------------------------------
void TECLA_MOTOR(void)
{ 
  //INICIALIZA STRINGS E VARIÁVEL 
  
 
   if(TECLA=='2')                           // TECLA pressionada é =2?
   {                                       // se sim
    TECLA=0;
    P5OUT=0x01;                           // LIMPA DISPLAY 
    ENVIA_COMANDO();                     // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
   
    if(velocidade<9)                        // estabelece condição de velocidade máxima
    {
        velocidade++;                           // há incremento da velocidade
    }

    ULTIMO_GIRO(ANTERIOR,velocidade); // qual o último foi a última opção de sentido de giro?
    
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
    
    V_DISPLAY=SPEED[velocidade];        // a variável V_DISPLAY recebe a posição do vetor SPEED indicada pela variável velocidade 
    CARREGA_DADO(V_DISPLAY);               // carrega a variável V_DISPLAY no display

   }  
   else if(TECLA=='8')                         // TECLA pressionada é =8?
    {                                         // se sim
      TECLA=0;
      P5OUT=0x01;                       // LIMPA DISPLAY
      ENVIA_COMANDO();                  // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
      
      velocidade--;                       // há decremento da velocidade
      
      if(velocidade<1)                        // estabelece condição de velocidade mínima
    {
      velocidade=1;                         // se o aumento foi maior que a condição mínimo, velocidade permanece no valor mínimo
    }
    
    ULTIMO_GIRO(ANTERIOR,velocidade);   // qual o último foi a última opção de sentido de giro?
     
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
      
    V_DISPLAY=SPEED[velocidade];        // a variável V_DISPLAY recebe a posição do vetor SPEED indicada pela variável velocidade 
    CARREGA_DADO(V_DISPLAY);               // carrega a variável V_DISPLAY no displaY
   
    }
   
   else if(TECLA=='4')                       // TECLA pressionada é =4?
   {                                        // se sim
     
     P5OUT=0x01;                            // LIMPA DISPLAY
     ENVIA_COMANDO();                      // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
     
     
   ANTERIOR=TECLA;                      // variável anterior armazena o ultimo sentido de giro requerido
     TECLA=0;
   unsigned int e=0; 
      for(e=0;e<5;e++)                   // enquanto não chegar o fim da string, P5OUT(saída do display) recebe cada valor da string
      {
       P5OUT=ESQUERDA[e];                // ESQUERDA é o vetor de sentido de giro para a esquerda e a variável :'e' é o índice do vetor
       ENVIA_DADO();                       // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente
      }
  
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
    
     V_DISPLAY=SPEED[velocidade];        // a variável V_DISPLAY recebe a posição do vetor SPEED indicada pela variável velocidade 
     CARREGA_DADO(V_DISPLAY);               // carrega a variável V_DISPLAY no display
     
   }
   else if(TECLA=='6')                       // TECLA pressionada é =6?
    {
     
      P5OUT=0x01;                       // LIMPA DISPLAY
      ENVIA_COMANDO();                  // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
      
      ANTERIOR=TECLA;                   // variável anterior armazena o ultimo sentido de giro requerido
      TECLA=0;
      unsigned int d=0;
      for(d=0;d<6;d++)               // enquanto não chegar o fim da string, P5OUT(saída do display) recebe cada valor da string
      {
       P5OUT=DIREITA[d];               // DIREITA é o vetor de sentido de giro para a direita e a variável :'d' é o índice do vetor
       ENVIA_DADO();                   // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente
      }
      
  
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
    
     V_DISPLAY=SPEED[velocidade];        // a variável V_DISPLAY recebe a posição do vetor SPEED indicada pela variável velocidade 
     CARREGA_DADO(V_DISPLAY);               // carrega a variável V_DISPLAY no display
    }
  
   else if(TECLA=='5')                         // TECLA pressionada é =5
    { 
     
      P5OUT=0x01;                       // LIMPA DISPLAY
      ENVIA_COMANDO();                  // subrotina em assembly que envia ao displayo respectivo comando proposto anteriormente 
      
      ANTERIOR=TECLA;
      TECLA=0;
      unsigned int p=0;
      for(p=0;p<5;p++)             // enquanto não chegar o fim da string, P5OUT(saída do display) recebe cada valor da string
      {
       P5OUT=PARADO[p];           // PARADO é o vetor de estado parado do motor e a variável :'p' é o índice do vetor
       ENVIA_DADO();               // subrotina em assembly que envia ao displayo respectivo dado proposto anteriormente
      }
      
      unsigned int s=0;
      for(s=0;s<6;s++)
      {
        P5OUT=VELOCIDADE[s];
        ENVIA_DADO();
      }
    
       V_DISPLAY=SPEED[velocidade];        // a variável V_DISPLAY recebe a posição do vetor SPEED indicada pela variável velocidade 
       CARREGA_DADO(V_DISPLAY);               // carrega a variável V_DISPLAY no display
       
    }
   

    }
   
 

