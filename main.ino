#include <Adafruit_Fingerprint.h>
#include <SD.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#define CS 10
const PROGMEM LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const PROGMEM SoftwareSerial biometricoSerial(4, 3);
// Cria objeto "dedo" para utilizar o leitor biometrico
const PROGMEM Adafruit_Fingerprint finger = Adafruit_Fingerprint(&biometricoSerial);

typedef struct {              
  String nome;
  uint8_t id;
}LISTA; 

LISTA listaGeral[3];


uint8_t selecionado;
uint8_t atualSelecionado;
uint8_t tamanhoTotal;

File alunos;

uint8_t botaoAcima = 5;
uint8_t botaoAbaixo = 6;
uint8_t botaoSeleciona = 7;

void setup() {

atualSelecionado = 0;
selecionado = 0;
tamanhoTotal = 0;

pinMode(botaoAcima , INPUT);
pinMode(botaoAbaixo , INPUT);
pinMode(botaoSeleciona , INPUT);

Serial.begin(9600);

// *********************************

lcd.init();
lcd.backlight();

// *********************************

  exibeMsg(F("TESTANDO LEITOR BIOMETRICO"));
  // Definir a taxa de dados para a porta do serial do sensor
  finger.begin(57600);
  if (finger.verifyPassword()) { // teste para ver se detectou 
    exibeMsg(F("SENSOR BIOMETRICO ENCONTRADO"));
  } else {
    exibeMsg(F("BIOMETRICO NAO DETECTADO"));
    while (1) { delay(1); } // trava o codigo pois ocorreu erro 
  }


// *********************************

//>>>>>>>>>SD
// pino que vai ser utilizado para o SD
  pinMode(CS, OUTPUT);
  while (!SD.begin(CS)) {
    while (1) {
      exibeMsg(F("INSIRA SD E REINICIE O MODULO"));
      delay(10000);
    } // trava o codigo pois ocorreu erro 
  }
  exibeMsg(F("CARTAO SD DETEC-TADO"));
  buscarArquivoAlunos(); // Função para buscar dentro da raiz do sd o arquivo alunos.txt
  selecionado=1;
  atualSelecionado = 2;

// *******************************

  gerarLista();
  exibeMsg(F("LISTA GERADA COMSUCESSO"));

    exibeMsg(F("Modulo Inicializado com sucesso"));
    delay(3000);
    
    exibeMsg(F("GRUPO IOTFA     FADERGS"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("GRUPO IOTFA     FADERGS"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("GRUPO IOTFA     FADERGS"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("GRUPO IOTFA     FADERGS"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("GRUPO IOTFA     FADERGS"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("MODULO DE DEMONSTRACAO"));
    delay(2000);
// ***************************

}

void loop() {

  if(selecionado)
  {
    exibeMsg(listaGeral[atualSelecionado].nome);
    delay(3000);
    seleciona();
    registra();
    /*atualSelecionado = atualSelecionado+1;*/
  }else{
    exibeMsg(F("Nada selecionado"));
  }
  
  exibeMsg(F("FIM DA DEMONSTRACAO"));
  /*
  if (atualSelecionado >2) {
    atualSelecionado = 0;  
  }*/
  delay(100000000);
}
void registra(){
    alunos = SD.open("alunos.txt", FILE_WRITE);
  /*if (alunos){ 
  String apend = "";
    while (aluno.available()) {
      char leu = aluno.read();
      apend += leu;
      if (leu == '\n'){
        apend = "";
      }
      if(listaGeral[atualSelecionado].nome == apend+" \0"){
        exibeMsg(apend);
      }
    }
  }*/
  alunos.print('3');
  alunos.close();
  alunos = SD.open("03092018.txt",FILE_WRITE);
  alunos.print("RAFAEL ESTAVA PRESENTE");
  alunos.close();
  exibeMsg(F("REGISTRADO DIGI-TAL E PRESENCA"));
  delay(5000);
  }
void seleciona(){
    //selecionado=0;
    exibeMsg("CADASTRAR DIGI- TAL DE " + retornaPrimeiroNomeAluno());
    delay(3000);
   // exibeMsg(F("SE SIM, SEGURAR BOTAO SELECIONA"));
   //delay(3000);
    botaoSeleciona = HIGH;
    if (botaoSeleciona == HIGH){
      if(cadastrar()){
        //exibeMsg("CADASTRADO COM SUCESSO");
        //delay(3000);
      }
    }else{
      exibeMsg(F("CANCELADO"));
    delay(3000);
    }
}

String retornaPrimeiroNomeAluno(){
  uint8_t x = 0;
  char ch;
  String primeiroNome = "";
  while(true) 
  {
    ch = listaGeral[atualSelecionado].nome[x];
    if (ch == ' ')
    {
      return primeiroNome;
    }
    else
    {
      primeiroNome += ch;
    }
    x++;
  }
}


boolean cadastrar() {
  
  uint8_t idParaCadastro;

  uint8_t qtdIdsCadastradas = retornaIdsCadastradas();
  
  if (listaGeral[atualSelecionado].id == 0){
    
   idParaCadastro = qtdIdsCadastradas + 1;
    
  }else{
    
    idParaCadastro = listaGeral[atualSelecionado].id;
  }
  // 2 cadastra id na digital escolhida

   while (!getFingerprintEnroll(idParaCadastro));

   String idParaPorNoArquivo = String(idParaCadastro);
   //exibeMsg(F("CADASTRADO"));
   //exibeMsg("REGISTRANDO PRE-SENCA");
    delay(3000);

  // 3 abrir arquivo alunos.txt e adiciona o id ao nome
 /* alunos = SD.open("alunos.txt", FILE_WRITE);
  if (alunos){ 
  String apend = "";
    while (aluno.available()) {
      char leu = aluno.read();
      apend += leu;
      if (leu == '\n'){
        apend = "";
      }
      if(listaGeral[atualSelecionado].nome == apend+" \0"){
        exibeMsg(apend);
      }
    }
  }
  alunos.print('3');
  alunos.close();
  alunos = SD.open("03092018.txt",FILE_WRITE);
  alunos.print("RAFAEL ESTAVA PRESENTE");
  alunos.close();*/
  // 4 retorna true se tudo ok ou false se der algo errado
  return true;

}

unsigned int retornaIdsCadastradas(){

  uint8_t qtdIds = 0;
  uint8_t idsSemCadastro = 0;
  for (uint8_t cont=0; cont < tamanhoTotal ; cont++){
    if (listaGeral[cont].id == 0){
        idsSemCadastro++;
      }
  }
  qtdIds = tamanhoTotal-idsSemCadastro;
  return qtdIds;
  
}

void buscarArquivoAlunos(){

  if(SD.exists("alunos.txt")){
    exibeMsg(F("ARQUIVO ALUNOS  ENCONTRADO"));
  }else{
    exibeMsg(F("ARQUIVO ALUNOS  NAO ENCONTRADO"));
    while (1) { delay(1); } // trava o codigo pois ocorreu erro 
  }
}



void gerarLista(){
  
  uint8_t atual=0;
  alunos = SD.open("alunos.txt");
  while (alunos.available()){
    listaGeral[atual].nome = leiaNome()+'\0';
    listaGeral[atual].id = leiaId();
    tamanhoTotal++;   
    atual++;
  }
  alunos.close();  // fecha o arquivo alunos.txt
}


uint8_t leiaId()  // função para retornar o ID de um aluno ou o numero 0 caso ele não tenha um id relacionado
{
  char ch;
  String retorno = "";
  uint8_t retornoInt;
  while (alunos.available())      // enquanto ainda houver algo para ler no arquivos alunos.txt faça...
  {
    ch = alunos.read();           // ler o char atual

    if (isDigit(ch)){             //se for inteiro vai acumular em uma String 
      retorno += ch;
    }else
    if (ch == '\n')               // se encontrar o char de quebra de linha então acabou ....
    {
      retornoInt = retorno.toInt();       // convert o numero encontrado em um inteiro
      return retornoInt;                  // retorna o id correspondente ou o numero 0 caso não tenha um 
                                          // id relacionado e este nome
    }
  }
  // esta linha debaixo é para retornar o ultimo id lido no fim do arquivo
  retornoInt = retorno.toInt(); 
  return retornoInt;
}

String leiaNome()  // função para montar uma string com o conteudo de 
{
  String recebido = "";           // String vazia para ser preenchida com o conteudo de uma linha
  char ch;
  while (alunos.available())      // enquanto ainda houver algo para ler no arquivos alunos.txt faça...
  {
    ch = alunos.read();           // recebe o char atual
    if (ch == '#')        // se encontrar o sustenido que indica que após ele vem o ID do aluno ....
    {
      //Serial.println(recebido);
      return recebido;    // retorna o que estiver na string
    }
    else
    if (ch != '\n')                          // senão ...
    {
      recebido += ch;             // acrescenta ao fim da string recebido o char atual
    }
  }
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
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("TENTE NOVAMENTE"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("TENTE NOVAMENTE"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("TENTE NOVAMENTE"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("TENTE NOVAMENTE"));
    delay(300);
    lcd.clear();
    delay(300);
    exibeMsg(F("TENTE NOVAMENTE"));
    delay(300);
    lcd.clear();
    delay(300);
}
