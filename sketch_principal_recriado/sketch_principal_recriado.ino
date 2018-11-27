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
int escolha;

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


}



void loop() {

  if (Serial.available()) {
    escolha = Serial.read();
  }
  if (escolha == 1){
    //entra no módulo de cadastro de aluno
  }

  //getFingerprintIDez();
  int temAluno = getFingerprintIDez();

  
  if (temAluno){
    //módulo de registro
    
    Serial.print("aluno encontrado na ID: ");
    Serial.print(temAluno);
    Serial.println("Pode remover o dedo");
    delay(100);
    
    //registra presença

    SD.open
    
  }
  
  delay(5);   

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
