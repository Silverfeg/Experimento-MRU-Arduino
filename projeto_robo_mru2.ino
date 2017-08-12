//declaração de bibliotecas
#include <IRremote.h>  // biblioteca para usar o controle remoto
#include<LiquidCrystal.h> //biblioteca para usar o display LCD
#include <Bounce.h> //biblioteca para evitar trepidação
#include <l298n.h> //biblioteca de controle de motores
#include <StopWatch.h> //biblioteca de cronometro

//definição dos pinos dos botoes e sensor
#define botao1 10 //botoes de controle do LCD
#define botao2 9  //botoes de controle do LCD
#define sensor 2  //sensor infravermelho

//definição dos pinos dos motores
#define M1_IN1 A1 //pino 1 de sinal para a ponteH motor 1
#define M1_IN2 A2 //pino 2 de sinal para a ponteH motor1 
#define M1_EN 6// pino PWM para a ponteH motor 1

#define M2_IN3 A3 //pino 1 de sinal para a ponteH motor 2
#define M2_IN4 A4 //pino 2 de sinal para a ponteH motor 2
#define M2_EN 5 // pino PWM para a ponteH motor2

//declaração variáveis do display LCD
LiquidCrystal lcd (12, 11, 13, 4, 3, 7); // declaração do display lcd
int tela; //variavel de controle da tela do lcd
int selecao = 0; //variavel controle selecao velocidade
int ponto_medicao = 0; //inicialização dos pontos de medição
int tempo1,tempo2,tempo3,tempo4; //variavel do cronometro
float velocidade = 0; //variavel que apresenta a velocidade final
int RECV_PIN = 8;  //pino do receptor do controle remoto
float armazenavalor;  //valor recebido pelo controle remoto
IRrecv irrecv(RECV_PIN);  //declaracao do receptor do controle remoto
decode_results results;  // variável específica da classe do controle remoto que armazena o que for recebido pelo receptor do controle remoto


//declaração de objetos com debouncing para os motores e sensores
Bounce pushbutton = Bounce(botao1, 10);
Bounce pushbutton2 = Bounce(botao2, 10);
Bounce bounce_sensor = Bounce(sensor,10);

//declaracao do objeto cronometro
StopWatch sw_millis;

//declaração de objetos dos motores
l298n motor1(M1_IN1, M1_IN2, M1_EN);
l298n motor2(M2_IN3, M2_IN4, M2_EN);

void setup()
{
  //Rotina de inicialização do LCD
  Serial.begin(9600); // comando de inicializaçao  da comunicação serial entre o arduino e o computador
  lcd.begin(16, 2);  // comando de inicialização do LCD indicando o tamnho dele
  
  //declaração dos botões como dispositivos de entrada
  pinMode(botao1, INPUT); 
  pinMode(botao2, INPUT);
  pinMode(8, INPUT);
  
  //comandos para enviar mensagem de boas vindas do software no LCD
  lcd.setCursor(0,0); //define que o cursos do lcd esteja na posição inicial
  lcd.print("Robo");  //escreve a palavra Robo a partir da posição que o cursor se encontra
  lcd.setCursor(0,1); //posiciona o cursor em outra posição
  lcd.print("MRU"); //escreve a palavra MRU a partir da posição em que o cursos se encontra
  delay(3000);  //faz o sotware esperar por 3 segundos para que o usuário possa visualizar a mensagem 
  lcd.clear();  //limpa o display lcd 
  tela = 1;   //inicializa a variável que indica qual velocidade o usuário está visualizando 
  
  
  irrecv.enableIRIn(); // Inicializa o receptor IR  
}

void loop()
{
  if (irrecv.decode(&results))  //verificação se algum botão foi pressionado do controle remoto
  {  
   
    armazenavalor = (results.value);  // armazena código recebido pelo receptor do controle remoto
    if (armazenavalor == 0xFF02FD){   // verifica se o código recebido representa o botão de muda a velocidade que o usuário poderá selecionar
      
       motor1.parar();  //manda o comando para a ponteH parar o motor1
        motor2.parar(); //manda o comando para a ponteH parar o motor1
        lcd.clear();    //limpa o display lcd
        selecao = 0;    //inicializa variável que indica e alguma velocidade está selecionada, o valor 0 representa que nenhuma velocidade está selecionada
        if(tela == 1) tela++; //avança a e tela que o usuário está visualizando
           else if(tela == 2) tela++; //avança a tela que o usuário está visualizando
                 else if(tela == 3) tela = 1; //volta para a primeira tela
      
    }
    if(armazenavalor == 0xFFC23D)  // verifica se o código recebido representa o botão que seleciona uma velocidade e roda uma rotina
    {
       if(tela == 1){ //verifica se o usuário está na primeira velocidade
         selecao = 1;  //seleciona a velocidade que está sendo visualizada(1)
         sw_millis.start(); //começa o cronômetro
       }
       
                 else if(tela == 2){ //verifica se o usuário está na segunda velocidade
         selecao = 2; //selecina a velocidade que está sendo visualizada(2)
         sw_millis.start(); //inicia o cronômetro
       }
           else if(tela == 3){ //verifica se o usuário está na terceira velocidade 
             selecao = 3; // seleciona a velocidade que está sendo visualizada pelo usuário(3)
             sw_millis.start(); //inicia o cronômetro
           }
    }
     irrecv.resume(); //Le o próximo valor  
  }
  

  
  switch(tela){ //switch que executa uma série de ações de acordo com o valor que tela tiver quando ele for chamada
    
    case 1: // caso em que o usuário está na primeira velocidade 
    lcd.setCursor(0,0); //coloca o cursor na posição inicial
    lcd.print("Velocidade 1"); //indica a velocidade que pode ser selecionada com o botão "play"
    
    //posiciona o cursor do LCD na posição desejada
    lcd.setCursor(6,1); 
    
    if(selecao == 1){ //verifica se a velocidade 1 foi selecionada
    motor1.set_vel(127); //determina a velocidade em que o motor1 irá girar
    motor2.set_vel(127); //determina a velocidade em que o motor2 irá girar 
    motor1.mover_sentido1(); //move o motor1 em um sentido
    motor2.mover_sentido1(); //move o motor2 em um sentido
    
    lcd.setCursor(0,1); //posiciona o cursor do LCD na posição desejada
    lcd.print("Selecionada"); //mostra na tela de LCD para o usuário que a velocidade desejada foi selecionada
     
    if (bounce_sensor.update()){ //verifica se o sensor teve uma mudança de estado 
     if (bounce_sensor.fallingEdge()){ //verifica se a mudança foi de um nível lógico alto para um nível lógico baixo(HIGH para LOW)
       if(ponto_medicao == 0){ //verifica se o robô ainda não passou por nenhum ponto de medição
       tempo1 = sw_millis.elapsed(); //armazena na variável o tempo decorrido desde o início do experimento
       ponto_medicao = 1;  //estabelece que o móvel já passou pelo primeiro ponto de medição
       sw_millis.reset(); //reseta o cronômetro
       sw_millis.stop();  //para o cronômetro
       sw_millis.start(); //inicia o cronômetro
       }
       else if(ponto_medicao == 1){ //verifica se o robô passou já por um ponto de medição
        tempo2 = sw_millis.elapsed(); //armazena na variável o tempo decorrido desde o último ponto 
        sw_millis.reset(); //reinicia o cronômetro
        sw_millis.stop();  //para o cronômetro
        sw_millis.start(); //inicia o cronômetro
        ponto_medicao = 2; //estabelece que o móvel já passou por dois pontos de medição
       }
       else if(ponto_medicao == 2){
        tempo3 = sw_millis.elapsed();
        sw_millis.reset(); 
        sw_millis.stop();
        sw_millis.start();
        ponto_medicao = 3;
       }
       else if(ponto_medicao == 3){
        tempo4 = sw_millis.elapsed();
        sw_millis.reset(); 
        sw_millis.stop();
        sw_millis.start();
        ponto_medicao = 4;
       }
       else if(ponto_medicao == 4){
         motor1.parar();
         motor2.parar();
         selecao = 0;
         ponto_medicao = 0;
         lcd.clear();
         tela=99;
       }
     }
   } 
    

    }
    break;    
    
      case 2:
      lcd.setCursor(0,0);
      lcd.print("Velocidade 2");
      lcd.setCursor(0,1);
      
      lcd.setCursor(6,1);
      
    
      if(selecao == 2){
        motor1.set_vel(200);
        motor2.set_vel(200);
        motor1.mover_sentido1();
        motor2.mover_sentido1();
       lcd.setCursor(0,1);
    lcd.print("Selecionada");
    
    if (bounce_sensor.update()){
     if (bounce_sensor.fallingEdge()){
       if(ponto_medicao == 0){
       tempo1 = sw_millis.elapsed();
       ponto_medicao = 1;
       sw_millis.reset(); 
       sw_millis.stop();
       sw_millis.start();  
       }
       else if(ponto_medicao == 1){
        tempo2 = sw_millis.elapsed();
        sw_millis.reset(); 
        sw_millis.stop();
        sw_millis.start();
        ponto_medicao = 2;
       }
       else if(ponto_medicao == 2){
        tempo3 = sw_millis.elapsed();
        sw_millis.reset(); 
        sw_millis.stop();
        sw_millis.start();
        ponto_medicao = 3;
       }
       else if(ponto_medicao == 3){
        tempo4 = sw_millis.elapsed();
        sw_millis.reset(); 
        sw_millis.stop();
        sw_millis.start();
        ponto_medicao = 4;
       }
       else if(ponto_medicao == 4){
         motor1.parar();
         motor2.parar();
         selecao = 0;
         ponto_medicao = 0;
         lcd.clear();
         tela=99;
       }
     }
   } 
    

    }
    break;    
        
        case 3:
        lcd.setCursor(0,0);
        lcd.print("Velocidade 3");
      
      lcd.setCursor(0,1);
      
      lcd.setCursor(6,1);
      
      
        if(selecao == 3){
          motor1.set_vel(250);
          motor2.set_vel(250);
          motor1.mover_sentido1();
          motor2.mover_sentido1();
         lcd.setCursor(0,1);
    lcd.print("Selecionada");
    
    if (bounce_sensor.update()){
     if (bounce_sensor.fallingEdge()){
       if(ponto_medicao == 0){
       tempo1 = sw_millis.elapsed();
       ponto_medicao = 1;
       sw_millis.reset(); 
       sw_millis.stop();
       sw_millis.start();  
       }
       else if(ponto_medicao == 1){
        tempo2 = sw_millis.elapsed();
        sw_millis.reset(); 
        sw_millis.stop();
        sw_millis.start();
        ponto_medicao = 2;
       }
       else if(ponto_medicao == 2){
        tempo3 = sw_millis.elapsed();
        sw_millis.reset(); 
        sw_millis.stop();
        sw_millis.start();
        ponto_medicao = 3;
       }
       else if(ponto_medicao == 3){
        tempo4 = sw_millis.elapsed();
        sw_millis.reset(); 
        sw_millis.stop();
        sw_millis.start();
        ponto_medicao = 4;
       }
       else if(ponto_medicao == 4){
         motor1.parar();
         motor2.parar();
         selecao = 0;
         ponto_medicao = 0;
         lcd.clear();
         tela= 99;
       }
     }
   } 
    

    }
    break;    
        
  
  case 99: //caso para quando todas as medições já foram realizadas em qualquer velocidade
  
  lcd.setCursor(0,0); //coloca o cursor do LCD em sua posição inicial
  lcd.print("Tempo 1"); //apresenta a mensagem Tempo1 no display lcd a partir da posição do cursor
  lcd.setCursor(0,1);  //posiciona o cursor do LCD na posição desejada
  lcd.print(tempo1);  //mostra na tela do display LCD o tempo entre o início e o primeiro ponto de medição
  delay(2000); //para o programa por 2 segundos mostrando a ultima mensagem 
  lcd.setCursor(0,0);
  lcd.print("Tempo 2");
  lcd.setCursor(0,1);
  lcd.print(tempo2);
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("Tempo 3");
  lcd.setCursor(0,1);
  lcd.print(tempo3);
  delay(2000);
  lcd.setCursor(0,0);
  lcd.print("Tempo 4");
  lcd.setCursor(0,1);
  lcd.print(tempo4);
  delay(2000);
  lcd.clear();//limpa o display lcd
  tela = 1; //volta para a tela inicial
  break;
  }
} 





