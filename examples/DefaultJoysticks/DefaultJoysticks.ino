/* Exemplo de Aplicação JC-ARMI 
	
	"Neste exemplo, usaremos o JC-ARMI sendo controlado pelos Joysticks.
	Se atentar aos limites de velocidade e ângulos para evitar problemas."

		Library originally added Apr 2022
		by Breno M. Oliveira & Lucas M. Scarpin
		library modified 23 Oct 2024
*/

#include <JCARMI.h>
#define joyHorizontalPin  4
#define joyVerticalPin  5
#define joyRotatePin  1
#define joyClawPin  2

float PosHorizontal = 100;
float PosVertical = 100;
float angRotate = 90;
int angClaw = 25;  // Posição inicial da garra


int armMovementSpeed = 5;// Velocidade máxima de avanço da garra (não aumentar além de 5)
int rotationSpeed = 2;// Velocidade máxima de rotação da base (não aumentar além de 4)
int maxClawAngle = 50;// Limite máximo do ângulo da garra (não aumentar além de 50)
int clawSpeed = 5;// Velocidade de abertura/fechamento da garra

void setup() {  
  // Inicia a comunicação com o braço robótico
  jcArmI.begin(11, 10, 9, 8);  
  // Move a garra para a posição inicial (garantir que não force)
  jcArmI.moveClawTo(PosHorizontal, PosVertical);
  jcArmI.grab(angClaw);  // Define a posição inicial da garra
}

void loop() {
  // Lê os valores dos joysticks
  int Jx = analogRead(joyHorizontalPin);
  int Jy = analogRead(joyVerticalPin);
  double Jr = float(analogRead(joyRotatePin));
  int Jc = analogRead(joyClawPin);

  // Mapeamento dos valores do joystick horizontal (avança/retrai a garra)
  if (Jx > 700) Jx = -armMovementSpeed;
  else if (Jx < 300) Jx = armMovementSpeed;
  else Jx = 0;

  // Mapeamento dos valores do joystick vertical (sobe/desce a garra)
  if (Jy > 700) Jy = -armMovementSpeed;
  else if (Jy < 300) Jy = armMovementSpeed;
  else Jy = 0;

  // Verifica se há movimento no joystick vertical (abrir/fechar garra)
  if (Jc > 700 || Jc < 300) {
    // Só ajusta a posição da garra se houver movimento
    if (Jc > 700) Jc = clawSpeed;  // Usa a velocidade de abertura definida
    else if (Jc < 300) Jc = -clawSpeed;  // Usa a velocidade de fechamento definida
    else Jc = 0;
    // Limita o ângulo da garra entre 0 e o limite máximo
    if (angClaw >= 0 && angClaw <= maxClawAngle) {
      angClaw += Jc;
    } else if (angClaw < 0) {
      angClaw = 0;
    } else if (angClaw > maxClawAngle) {
      angClaw = maxClawAngle;
    }
    // Aplica a abertura da garra calculada
    jcArmI.grab(angClaw);
  }

  // Se o joystick da garra não estiver sendo movido, permite rotação
  if (!(Jc > 700 || Jc < 300)) {
    // Controle de rotação com velocidade definida pelo usuário
    if (Jr > 700) Jr = rotationSpeed;
    else if (Jr < 300) Jr = -rotationSpeed;
    else Jr = 0.0;

    // Ajusta a rotação apenas quando o joystick de rotação é movido
    if (Jr != 0.0) {
      angRotate += Jr;  
      if (angRotate < 0) angRotate = 0;
      else if (angRotate > 180) angRotate = 180;
      // Aplica a rotação calculada ao motor de rotação da base
      jcArmI.rotateBaseTo(angRotate);
    }
  }

  // Controle dos movimentos horizontais e verticais
  if (jcArmI.isPointInPolygon(PosHorizontal + Jx, PosVertical + Jy)) {
    PosHorizontal += Jx;
    PosVertical += Jy;
    
    // Move a garra para a nova posição
    jcArmI.moveClawTo(PosHorizontal, PosVertical);
  }

  // Introduz uma pequena pausa para evitar comandos excessivos
  delay(10);
}
