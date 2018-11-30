#include <Adafruit_Fingerprint.h>
#include <SD.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>



const int chipSelect = 10;
const PROGMEM LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const PROGMEM SoftwareSerial biometricoSerial(4, 3);
// Cria objeto "dedo" para utilizar o leitor biometrico
const PROGMEM Adafruit_Fingerprint finger = Adafruit_Fingerprint(&biometricoSerial);
char escolha;
String nomeAluno = "";
int totalAlunosNaLista = 0;



void setup() {

Serial.begin(9600);
Serial.println(F("Incializando Protótipo"));
// *********************************

lcd.init();
lcd.backlight();

// *********************************
  Serial.println(F("Inicializando Leitor Biométrico"));
  exibeMsg(F("TESTANDO LEITOR BIOMETRICO"));
  // Definir a taxa de dados para a porta do serial do sensor
  finger.begin(57600);
  if (finger.verifyPassword()) { // teste para ver se detectou
    exibeMsg(F("SENSOR BIOMETRICO ENCONTRADO"));
    Serial.println(F("SENSOR BIOMETRICO ENCONTRADO"));
  } else {
    exibeMsg(F("BIOMETRICO NAO DETECTADO"));
    Serial.println(F("BIOMETRICO NAO DETECTADO"));
    while (1) { delay(1); } // trava o codigo pois ocorreu erro
  }


// *********************************

//>>>>>>>>>SD
// pino que vai ser utilizado para o SD
  pinMode(chipSelect, OUTPUT);
  while (!SD.begin(chipSelect)) {
    while (1) {
      exibeMsg(F("INSIRA SD E REINICIE O MODULO"));
      Serial.println(F("INSIRA SD E REINICIE O MODULO"));
      delay(10000);
    } // trava o codigo pois ocorreu erro
  }
  exibeMsg(F("CARTAO SD DETEC-TADO"));
  Serial.println(F("CARTAO SD DETECTADO"));

// *******************************
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");

  totalAlunosNaLista = contarAlunos();
  Serial.print(F("----->>> DEBUG total alunos na lista >> "));
  Serial.println(totalAlunosNaLista);

}



void loop() {

  if (Serial.available()) {
    escolha = Serial.read();
  }
  if (escolha == '1'){
    //entra no módulo de cadastro de aluno
    escolha = Serial.read();
    Serial.println("Digite id para cadastro: ");
    uint8_t cadastrar;
    char c;
    while(1){
      if (Serial.available()){
        c = Serial.read(); 
        break;
      }
    }
    
    cadastrar = atoi(c);
    
    getFingerprintEnroll(cadastrar);
  }
  escolha = '0';

  //getFingerprintIDez();
  int temAluno = getFingerprintIDez();

  
  if (temAluno){
    //módulo de registro
    
    Serial.print(F("Aluno encontrado na ID: "));
    Serial.println(temAluno);
    Serial.print(F("Obrigado "));
    Serial.println(enviaIdRetornaIndex(temAluno));
    Serial.println(F("Pode remover o dedo"));
    delay(1000);
    
    //registra presença

   // SD.open;
    
  }
  
  delay(5);   

}
int enviaIdRetornaIndex(int idParaConsulta){
  
  File dataFile = SD.open("lista.txt");
  int indexAtual = 0;

  while (dataFile.available()) {
    
    char c = dataFile.read();
    if(c == '\n'){
      indexAtual++;
      
      dataFile.seek(dataFile.position()-5); // ler a posição que atualmente se encontra 
                                          // e voltar 5 characteres
    
      char tmpChar[3];
      int cont = 0;
    
      while(dataFile.available()){
        char c = dataFile.read();
       
        if(isDigit(c)){  
              
          tmpChar[cont] = c;
          cont++;
        
        // converte de char para numero
        }else if(c == '\n'){
          tmpChar[cont] = '\0';
          break;
        }
        
     }
    
      uint8_t convertido = atoi(tmpChar);
    
      // se for o mesmo que o idParaConsulta então return
      if (convertido == idParaConsulta){
      
        dataFile.close();
        return convertido;
      }    
    }
  }

    
    
}

 
void registra(){

  }
void seleciona(){
    //selecionado=0;

}

String retornaPrimeiroNomeAluno(){

}


boolean cadastrar() {


}

unsigned int retornaIdsCadastradas(){

}

void buscarArquivoLista(){

  if(SD.exists("lista.txt")){
    exibeMsg(F("ARQUIVO ALUNOS  ENCONTRADO"));
  }else{
    exibeMsg(F("ARQUIVO ALUNOS  NAO ENCONTRADO"));
    while (1) { delay(1); } // trava o codigo pois ocorreu erro
  }
}

void gerarLista(){

}


void exibeMsg(String msg){
  // limpar a tela do lcd
  lcd.clear();
  // seta o cursor para a posição zero
  lcd.setCursor(0,0);

  uint8_t comprimentoString = msg.length(); // retorna um inteiro
                                                 // com o comprimento
                                                 // da string
  if(comprimentoString<17){  // se for de tamanho que cabe no lcd imprime
    lcd.print(msg);
  }else{  // se nao for de tamanho que cabe na tela
    // quebrar a string em duas
    String subMsg1 = msg.substring(0,16); // função que divide a String
                                          // do index 0 até o 16
                                          // o que tem em 17 nao vai
    String subMsg2 = msg.substring(16,comprimentoString);
                                          // mesmo que antes
    // exibi cada string em uma linha
    lcd.print(subMsg1);
    lcd.setCursor(0,1);
    lcd.print(subMsg2); // delay para ficar exibindo a msg 2 segundos
  }
}


uint8_t getFingerprintEnroll(uint8_t idParaCadastro) {

  int p = -1;
  exibeMsg(F("Aguardando dedo"));
  delay(1500);//exibeMsg(String(idParaCadastro));
 //delay(1200);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //exibeMsg(F("Imagem tirada"));
      //delay(1500);
      break;
    case FINGERPRINT_NOFINGER:
      exibeMsg(F("."));
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
    tenteNovamente();
      break;
    case FINGERPRINT_IMAGEFAIL:
    tenteNovamente();
      break;
    default:
    tenteNovamente();
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      //exibeMsg(F("Imagem convertida"));
     // delay(3000);
      break;
    case FINGERPRINT_IMAGEMESS:
    tenteNovamente();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    tenteNovamente();
      return p;
    case FINGERPRINT_FEATUREFAIL:
    tenteNovamente();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
    tenteNovamente();
      return p;
    default:
    tenteNovamente();
      return p;
  }

  exibeMsg(F("Remover dedo"));
  delay(3000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  //exibeMsg(F("ID ")); exibeMsg(String(idParaCadastro));
  //delay(3000);
  p = -1;
  exibeMsg(F("Coloque o mesmo dedo novamente"));
  delay(1500);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //exibeMsg(F("Imagem tirada"));
      //delay(1500);
      exibeMsg(F("Pode remover o dedo"));
      delay(2000);
      break;
    case FINGERPRINT_NOFINGER:
      exibeMsg(F("."));
      delay(2000);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
    tenteNovamente();
      break;
    case FINGERPRINT_IMAGEFAIL:
    tenteNovamente();
      break;
    default:
    tenteNovamente();
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      //exibeMsg(F("Imagem convertida"));
      //delay(2000);
      break;
    case FINGERPRINT_IMAGEMESS:
    tenteNovamente();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    tenteNovamente();
      return p;
    case FINGERPRINT_FEATUREFAIL:
    tenteNovamente();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
    tenteNovamente();
      return p;
    default:
    tenteNovamente();
      return p;
  }

  // OK converted!
  //exibeMsg(F("Criando modelo para #"));  exibeMsg(String(idParaCadastro));
  //delay(3000);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    //exibeMsg(F("Impressoes combinadas!"));
    //delay(3000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    tenteNovamente();
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    tenteNovamente();
    return p;
  } else {
    tenteNovamente();
    return p;
  }

  //exibeMsg(F("ID ")); exibeMsg(String(idParaCadastro));
  //delay(3000);
  p = finger.storeModel(idParaCadastro);
  if (p == FINGERPRINT_OK) {
    exibeMsg(F("Armazenado!!!"));
    delay(3000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    tenteNovamente();
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    tenteNovamente();
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    tenteNovamente();
    return p;
  } else {
    tenteNovamente();
    return p;
  }
}

void tenteNovamente(){
    exibeMsg(F("TENTE NOVAMENTE"));
    delay(3000);
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Found a print match!"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println(F("Did not find a match"));
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }   
  
  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID); 
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return 0;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){
    return 0;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
    Serial.println(F("com dedo"));
    return 0;
  }
  
  /*
  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID);
  
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence);*/
  return finger.fingerID; 
}



int addIdAoArquivo(int indexParaCadastro,int idParaCadastro)
{
  File dataFile = SD.open("lista.txt");
  int indexAtual = 0;
// Parte da função que conta quantos alunos tem na lista

  if (dataFile) {
    char c;
    while (dataFile.available()) {
      c = dataFile.read();
      Serial.print(c);
      if(c == '\n'){
        indexAtual = indexAtual+1;
        if (indexAtual == indexParaCadastro){

          unsigned long posiAtualLista2 = dataFile.position();
          dataFile.close();

          dataFile = SD.open("lista2.txt", FILE_WRITE);

          String adiciona = "#" + String(idParaCadastro)+'\n';


          nomeAluno += adiciona;

          Serial.println("estou aqui 1");

          if (dataFile){

            //dataFile.print('\n');

            dataFile.print(nomeAluno);
            Serial.println("estou aqui 2");

          }else{
            Serial.println("nao conseguiu abrir o arquivo de texto lista2.txt ");
          }


          dataFile.close();


          dataFile = SD.open("lista.txt");
          dataFile.seek(posiAtualLista2);

          Serial.println(F("Nome do aluno printado com sucesso"));

          nomeAluno = "";

        }else{
          unsigned long posiAtualLista1 = dataFile.position();
          dataFile.close();
          dataFile = SD.open("lista2.txt", FILE_WRITE);
          if (dataFile) {
            dataFile.print(nomeAluno);
            dataFile.print('\n');
            /*if (indexAtual == totalAlunosNaLista){
              dataFile.print('\n');
            }*/
            dataFile.close();
            Serial.println("PRINTADO NO ARQUIVO NOVO.");
         } else {
            Serial.println("error opening LISTA2.txt");
         }
            dataFile = SD.open("lista.txt");
            dataFile.seek(posiAtualLista1);
            nomeAluno = "";
        }
      }
      if(c != '\n'){
       nomeAluno += c;
      }
    }
  }

  dataFile.close();  // fecha o arquivo de texto que havia sido aberto


 // exclui antigo lista.txt

  SD.remove("lista.txt");

 // passa do lista2.txt para lista.txt

  dataFile = SD.open("lista2.txt");
  indexAtual = 0;

  if (dataFile) {
    char c;

    while (dataFile.available()) {
      c = dataFile.read();

      if(c == '\n'){
        indexAtual = indexAtual+1;

        unsigned long posiAtualLista1 = dataFile.position();
        dataFile.close();

        dataFile = SD.open("lista.txt", FILE_WRITE);

        if (dataFile) {
          dataFile.print(nomeAluno);
          dataFile.print('\n');
          dataFile.close();

          Serial.println("PRINTADO NO ARQUIVO final.");
         } else {
            Serial.println("erro ao tentar abrir LISTA.txt");
         }

           dataFile = SD.open("lista2.txt");
           dataFile.seek(posiAtualLista1);
           nomeAluno = "";

        }

        if(c != '\n'){
          nomeAluno += c;
        }
      }
    }




 //exclui lista2.txt


  SD.remove("lista2.txt");


  return indexAtual;
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
      if(c == '\n'){
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
      if(c == '\n'){
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
      }else if(c == '\n'){
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
      if(c == '\n'){
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
      }else if(c == '\n'){
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
