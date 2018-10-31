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

 This example code is in the public domain.

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

exibirAtualSelecionado(posicao);
posicao++;
}


void exibirAtualSelecionado(int indexEscolhido)
 {
  File dataFile = SD.open("datalog.txt");

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
    dataFile.seek(0); // funcao que retorna a leitura do arquivo para o ponto inicial

 // Parte da função que printa o aluno que esta no index selecionado

   // Serial.println("Alunos contados com sucesso");
  //  Serial.print("Numero alunos nesta lista ");
 //   Serial.println(indexAtual);
    //Serial.print(F("Agora vamos encontrar o aluno que esta no indice "));
    Serial.println(" ");
 //   Serial.print("Index ==> ");
 //   Serial.println(indexEscolhido);

    indexAtual = 0;
    while (dataFile.available()) {
      c = dataFile.read();
      if(c == '\r'){
        indexAtual = indexAtual+1;
        if(indexAtual == indexEscolhido){
          break;
        }
      }else{
        if(indexAtual == indexEscolhido-1 && c != '#' && !isDigit(c)){
          Serial.print(c);
        }
      }
    }
  //  Serial.println(" ");
   // Serial.println(F("Nome do aluno exibido com sucesso!"));
  //  Serial.println(F("Este aluno tem uma id associada a ele?"));

   // Serial.print("posicao atual ");
   // Serial.println(dataFile.position());
   // Serial.print("posicao para ir ");
   // Serial.println(dataFile.position()-5);


    dataFile.seek(dataFile.position()-5);  // reposiciona o cursor para poder pegar o id associado a este nome ou ver que nao tem nome associado a este id

// pega o id e um vetor e converte para uint8_t para poder usar na função da leitora biometrica

    char tmpChar[3];
    int cont = 0;
    while(dataFile.available()){
      c = dataFile.read();
      if(isDigit(c)){
        tmpChar[cont] = c;
        cont++;
      }else if(c == '\r'){
        tmpChar[cont] = '\0';
        break;
      }

    }
  //  Serial.print("tmpChar eh ");
 //   Serial.println(tmpChar);

    uint8_t convertido = atoi(tmpChar);  // parte da conversao em si e retorna 0 caso nao tenha nada associado para esse aluno / index

  //  Serial.print("Seu ID convertido para uint8_t ");
 //   Serial.println(convertido);

    if(convertido != 0){
 //     Serial.println("Sim, esse aluno tem uma id associada a ele");
    }else{
  //    Serial.println("Nao, esse aluno nao tem uma id associada a ele");
    }

    dataFile.close();  // fecha o arquivo de texto que havia sido aberto
  }
  else {
    Serial.println("error opening datalog.txt"); // printa erro se nao conseguir abrir o arquivo
  }
}
