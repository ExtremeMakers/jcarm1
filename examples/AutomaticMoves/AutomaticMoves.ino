/* Exemplo de Aplicação JC-ARMI 
	
	"Neste exemplo, a garra se moverá automaticamente entre dois pontos (inicial e final).
	Quando a garra atingir cada ponto, funções correspondentes serão chamadas para realizar ações
	como abrir ou fechar a garra."

		Library originally added Apr 2022
		by Breno M. Oliveira & Lucas M. Scarpin
		library modified 23 Oct 2024
*/

#include <JCARMI.h>

#define delayTime 2000  // Tempo de espera (em milissegundos) em cada ponto

float PosHorizontal = 100;  // Posição inicial X
float PosVertical = 100;    // Posição inicial Y
float finalPosX = 150;      // Posição final X
float finalPosY = 50;       // Posição final Y
float angRotate = 90;       // Posição inicial de rotação
int angClaw = 25;           // Posição inicial da garra (aberta ou fechada)

int armMovementSpeed = 5;   // Velocidade máxima de avanço da garra (não aumentar além de 5)
int rotationSpeed = 2;      // Velocidade máxima de rotação da base (não aumentar além de 4)
int maxClawAngle = 50;      // Limite máximo do ângulo da garra
int clawSpeed = 5;          // Velocidade de abertura/fechamento da garra

void setup() {  
  // Inicia a comunicação com o braço robótico
  jcArmI.begin(11, 10, 9, 8);  
  
  // Define a posição inicial da garra
  jcArmI.moveClawTo(PosHorizontal, PosVertical);
  jcArmI.grab(angClaw);  
}

void loop() {
 
  moveToPoint(finalPosX, finalPosY); // Move a garra até o ponto final
  pontoFinal(); // Executa ações no ponto final
  delay(delayTime); // Aguardar no ponto final por um tempo
  moveToPoint(PosHorizontal, PosVertical); // Move a garra de volta para o ponto inicial
  pontoInicial(); // Executa ações no ponto inicial  
  delay(delayTime); // Aguardar no ponto inicial por um tempo
}

// Função para mover a garra entre os pontos
void moveToPoint(float targetX, float targetY) {
  // Calcula o deslocamento até a posição alvo
  float deltaX = targetX - PosHorizontal;
  float deltaY = targetY - PosVertical;
  
  // Move a garra em direção ao ponto alvo
  while (abs(deltaX) > 1 || abs(deltaY) > 1) {
    if (deltaX > 0) {
      PosHorizontal += min(armMovementSpeed, deltaX);  // Avança em X
    } else if (deltaX < 0) {
      PosHorizontal += max(-armMovementSpeed, deltaX); // Recuar em X
    }
    
    if (deltaY > 0) {
      PosVertical += min(armMovementSpeed, deltaY);  // Avança em Y
    } else if (deltaY < 0) {
      PosVertical += max(-armMovementSpeed, deltaY); // Recuar em Y
    }

    // Move a garra para a nova posição
    jcArmI.moveClawTo(PosHorizontal, PosVertical);
    
    // Recalcula o deslocamento
    deltaX = targetX - PosHorizontal;
    deltaY = targetY - PosVertical;

    // Introduz um pequeno delay para evitar movimentos bruscos
    delay(10);
  }
}

// Função chamada quando a garra atinge o ponto inicial
void pontoInicial() {
  Serial.println("Chegou ao ponto inicial.");
    // Ações no ponto inicial (exemplo: fechar a garra)
  angClaw = 0;  // Fecha a garra
  jcArmI.grab(angClaw);
  Serial.println("Garra fechada no ponto inicial.");
}

// Função chamada quando a garra atinge o ponto final
void pontoFinal() {
  Serial.println("Chegou ao ponto final.");  
  // Ações no ponto final (exemplo: abrir a garra)
  angClaw = maxClawAngle;  // Abre a garra
  jcArmI.grab(angClaw);
  Serial.println("Garra aberta no ponto final.");
}
