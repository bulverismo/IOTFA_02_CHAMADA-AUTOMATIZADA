/*
 *
 *
 * Funcoes modificadas e codigo sendo recriado do zero
 *
 *
 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
int posicao = 1;
int totalAlunosNaLista = 0;
uint8_t botaoAcima = 5;
uint8_t botaoAbaixo = 6;
uint8_t botaoSeleciona = 4;

boolean selecionar = false;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
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

   totalAlunosNaLista = contarAlunos();


   // Seta os botoes

   pinMode(botaoAcima , INPUT);
   pinMode(botaoAbaixo , INPUT);
   pinMode(botaoSeleciona , INPUT);

  Serial.print("----->>> DEBUG :  ");
  Serial.println(totalAlunosNaLista);

}

void loop() {

uint8_t i = exibirAtualSelecionado(posicao);
Serial.print(" ");
Serial.println(i);
   while(1){
    
    
        if(digitalRead(botaoAcima) == HIGH){
          if(posicao == totalAlunosNaLista){
            posicao = 1;
            break;
          }else{
            posicao++;
            break;
          }
          }else if(digitalRead(botaoAbaixo) == HIGH){
            if(posicao == 1){
              posicao = totalAlunosNaLista;
              break;
            }else{
              posicao--;
              break;
            }
          }else if(digitalRead(botaoSeleciona) == HIGH){
            selecionar = true;
            break;
          }
        
    }
    
    if(selecionar){
      // bla
      Serial.print("aluno ");
      exibirAtualSelecionado(posicao);
      Serial.println(" foi selecionado");
      Serial.print("Numero de IDS cadastradas ");
      Serial.println(calcIdParaCadastro());
      //deseleciona
      selecionar = false;
    }
    
    delay(180);

}

int calcIdParaCadastro(){
  int contador = 1;
  int totalIds = 0;
  while(1){
    if(retornaIdAtualSelecionado(contador)!=0){
      totalIds++;
    }
    if (contador == totalAlunosNaLista){
      return totalIds;
    }
    contador++;
    Serial.print('.');
  }
}


int contarAlunos()
{
  File dataFile = SD.open("lista.txt");
  int indexAtual = 0;
// Parte da função que conta quantos alunos tem na lista

  if (dataFile) {
    char c;
    while (dataFile.available()) {
      c = dataFile.read();
      if(c == '\r'){
        indexAtual = indexAtual+1;
      }
    }
  }
  dataFile.close();  // fecha o arquivo de texto que havia sido aberto
  return indexAtual;
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

uint8_t retornaIdAtualSelecionado(int indexEscolhido)
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
