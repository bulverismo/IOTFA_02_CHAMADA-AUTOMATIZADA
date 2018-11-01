/*
  SD card file dump

 This example shows how to read a file from the SD card using the
 SD library and send it over the serial port.

 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created  22 December 2010
 by Limor Fried
 modified 9 Apr 2012
 by Tom Igoe

 his example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
int posicao = 1;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.


}

void loop() {
uint8_t i = exibirAtualSelecionado(posicao);
Serial.print(" ");
Serial.println(i);
posicao++;
}


void contarAlunos(){
  File dataFile = SD.open("lista.txt");

// Parte da função que conta quantos alunos tem na lista

  if (dataFile) {
    char c;
    int indexAtual = 0;
    while (dataFile.available()) {
      c = dataFile.read();
      if(c == '\r'){
        indexAtual = indexAtual+1;
      }
    }
  }

  dataFile.close();  // fecha o arquivo de texto que havia sido aberto
}

uint8_t exibirAtualSelecionado(int indexEscolhido)
{

// Função que printa o aluno que esta no index selecionado

  File dataFile = SD.open("lista.txt");
  int indexAtual = 0;

  while (dataFile.available()) {
      char c = dataFile.read();
      if(c == '\r'){
        indexAtual++;
        if(indexAtual == indexEscolhido){
          break;
        }
      }else{
        // teste para ver se o indice que eu estou neste momento é o indice que
        // estou procurando, se for, então junto com as outros opcoes que sao
        // para tratamento do que vai ser impresso, entra na funcao e printa
        // o nome sem armazer esse nome em lugar nenhum
        if(indexAtual == indexEscolhido-1 && c != '#' && !isDigit(c)){
          Serial.print(c);
        }
      }
    }

    // Essa parte da função serve para encontrar o ID que esta gravado dentro
    // do leitor biometrico

    dataFile.seek(dataFile.position()-5);  // reposiciona o cursor para poder
                                           // pegar o id associado a este nome
                                           // ou ver que nao tem nome associado
                                           // a este id

   // pega o id e poe em um vetor de char

    char tmpChar[3];
    int cont = 0;
    while(dataFile.available()){
      char c = dataFile.read();
      if(isDigit(c)){
        tmpChar[cont] = c;
        cont++;
      }else if(c == '\r'){
        tmpChar[cont] = '\0';
        break;
      }

    }
    // parte da conversao em do vetor de char,
    // vai retorna 0 caso nao tenha id associado ao nome
    // tem que converte para uint8_t para poder usar na função
    // da leitora biometrica
    uint8_t convertido = atoi(tmpChar);

    dataFile.close();  // fecha o arquivo de texto que havia sido aberto

    return convertido; // retorna id ja no formato uint8_t
}
