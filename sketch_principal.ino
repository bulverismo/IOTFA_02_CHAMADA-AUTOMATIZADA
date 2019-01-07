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
//char charLido;
String escolhaDigitada = "";
//String nomeAluno = "";
int totalAlunosNaLista = 0;
int contador = 0;



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
  Serial.print(F("Sensor contém ")); Serial.print(finger.templateCount); Serial.println(F(" templates"));

  totalAlunosNaLista = contarAlunos();
  Serial.print(F("----->>> DEBUG total alunos na lista >> "));
  Serial.println(totalAlunosNaLista);
  Serial.println(F("Digitar 1 para entrar em modo de cadastro."));
  exibeMsg(F("Chamada digital coloque o dedo"));
/*

  finger.emptyDatabase();

  Serial.println("Now database is empty :)");*/
  //Comando para excluir todas as digitais cadastradas
  
  
}



void loop() {
  
  if (contador == 50){ 
    exibeMsg(F("Chamada digital coloque o dedo"));
    contador = 0;
  }

  contador++;
  
  if (Serial.available() > 0) {
    char charLido = Serial.read();
    
    if(charLido == '\n'){  // se o char lido atualmente for um barra n quer dizer que o que veio do terminal de serial chegou ao fim
      /*
      Serial.println("DEBUGAIVOS = Estoy aquí queriéndote");  // debug da galera
      Serial.println(escolhaDigitada);*/
      
        if (escolhaDigitada == "1"){ // Se o cara digitou um no termnial de serial entrou na opçao secreta
            
          Serial.println("Você digitou 1 e entrou no módulo de cadastro do sistema: ");  // debug da galera
            //entra no módulo de cadastro de aluno
         
          if (imprimirListaAlunos()){
            Serial.println(F("Erro ao imprimir lista de aluno! Remova o cartão SD, confira se lista de alunos esta presente no cartão SD, reinsira o cartão SD e reinicie o módulo!"));
          }else{
            Serial.println(F("Lista de alunos impressa com sucesso"));
          }
                   
          Serial.println(F("Digitar um numero da coluna a esquerda"));
          Serial.println(F("correspondente ao aluno que você quer cadastrar"));
          Serial.println(F("ou digite S para Sair"));
          
          char c ;
          String opcao;
          
          while(1){
          
            if (Serial.available() > 0 ){
            
              c = Serial.read();
              
              if(c == '\n'){
                Serial.print(F("Opção escolhida: "));
                Serial.println(opcao);
                
                if (opcao == "S" or opcao == "s" or opcao == "sair" or opcao == "Sair"){
                  opcao = "";
                  break;
                }
                Serial.println(opcao.length()-1);
                for (int i = opcao.length()-1; i >= 0; i-- ){
                  char c = opcao[i];
                  if (!isDigit(c)){
                    Serial.print(F("Contém a letra = "));
                    Serial.println(c);
                    c = 0;
                    opcao = c;
                    i=0;
                    Serial.println(F("contém uma letra invalida!!"));
                  }
                }

                Serial.println(opcao.toInt());

                
                if(opcao.toInt() > 0 && opcao.toInt() <= totalAlunosNaLista){
                  Serial.println(F("Entrou no módulo de cadastro"));
                  int indexParaCadastro = opcao.toInt();
                  Serial.print(F("Será feito o cadastro de: "));
                  exibirAtualSelecionado(indexParaCadastro);
                  Serial.println(F(""));
                  
                  Serial.println(F("Buscando proxima id para cadastro..."));
                  int idParaCadastrar = calcIdParaCadastro();
                  Serial.print(F("Temos: "));
                  Serial.print(idParaCadastrar);
                  Serial.println(F(" cadastro(s) já realizado(s)"));
                  exibirAtualSelecionado(indexParaCadastro);
                  Serial.print(F(" será cadastrado com a ID "));
                  idParaCadastrar++;
                  Serial.println(idParaCadastrar);

                  Serial.println(F("Confirma realização do cadastro? s/n"));

                  opcao = "";
                  while(1){
                    if(Serial.available()>0){
                      char c = Serial.read();
                      if (c == '\n'){
                        if(opcao == "s"){
                         //opção sim
                         //faz o que tem que fazer
                          Serial.println(F("Escolhida opção sim!"));
                          getFingerprintEnroll(idParaCadastrar);
                          addIdAoArquivo(indexParaCadastro,idParaCadastrar);
                          


                         
                          break; //para sair do laço infinito
                        }
                        if(opcao == "n"){
                          //opçao não
                          //faz o que tem que fazer
                          Serial.println(F("Escolhida opção não!Voltando ao menu principal!"));
                          break; //para sair do laço infinito
                        }
                        //pode ter mais if's ou até mesmo repetição com if dentro para validar estes dados
                        //em caso de validação a string opcao deve ser "zerada"
                        Serial.println(F("Opção invalida! Tente novamente!!!"));
                        opcao = "";
                        
                      }else{
                        opcao += c;
                      }                        
                    }
                  }
                 
                  
                  break;
                }
                
                Serial.println(F("OPÇÃO INVALIDA!!! Tente novamente!"));
                opcao = "";
                  
              }else{
                opcao += c;
              }
            }
          }
            
            //cadastrar = atoi(c);
            /*
            Serial.print(F("cadastrar em: "));
            Serial.println(cadastrar);
            */
            //getFingerprintEnroll(cadastrar);
          }
          
      escolhaDigitada = "";
      
    }else{
      escolhaDigitada += charLido;
    }
    
  }


  //getFingerprintIDez();
  int temAluno = getFingerprintIDez();  //esta função ve sem tem dedo colocado no leitor se tiver
                                         // vai retornar o ID que este dedo esta cadastrado ou vai retornar dizendo que tem dedo colocado no leitor

  
  if (temAluno){  // se tiver aluno registrado para a digital em questão então entra neste módulo
    //módulo de registro
    
    Serial.print(F("Aluno encontrado na ID: "));
    Serial.println(temAluno);
    Serial.print(F("Aluno encontrado no index: "));
    Serial.println(enviaIdRetornaIndex(temAluno));
    //exibirAtualSelecionado(enviaIdRetornaIndex(temAluno));
    Serial.println(F("Pode remover o dedo"));
    registra(enviaIdRetornaIndex(temAluno));
    exibePrimeiroNomeAluno(enviaIdRetornaIndex(temAluno));
    delay(1000);
    
    //registra presença

   // SD.open;
    
  }
  
  delay(5);   

}

int imprimirListaAlunos(){
  File dataFile = SD.open("lista.txt");
  unsigned int index = 1;
  
  if (dataFile) {
    Serial.println(F("Lendo Lista de alunos: "));
    Serial.print(index);
    Serial.print(F(" "));
          
    while(dataFile.available()) {               
      char letra = (dataFile.read());
      Serial.print(letra);
     
      if (letra == '\n' && index < totalAlunosNaLista){
        index++;
        Serial.print(index);
        Serial.print(F(" "));
      }             
    }         
  }else{
    return 1; // retorna 1 se não conseguiu abrir o arquivo
  }
  dataFile.close();
  return 0; // retorna 0 se conseguiu imprimir a lista de alunos
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
        return indexAtual;
      }    
    }
  }
}

 
void registra(int indexEscolhido){

  String nomeAluno = retornaNomeAluno(indexEscolhido);
  int tamanhoNomeAluno = nomeAluno.length();
  int i=0;
  
  File dataFile = SD.open("ATUAL.TXT");
    
  while (dataFile.available()) {
    char c = dataFile.read();
    if(nomeAluno[i] == c ){
      i++;
      tamanhoNomeAluno--;
      Serial.println(i);
      Serial.println(tamanhoNomeAluno);
      if (tamanhoNomeAluno==0){
        break;
      }
    }else{
      i=0;
      tamanhoNomeAluno = nomeAluno.length();
    }
  }  
  dataFile.close();

  if (tamanhoNomeAluno == 0){
    //Serial.println("ja tem cadastro");
  }else{
    //Serial.println("nao tem cadastro");
    dataFile = SD.open("ATUAL.TXT", FILE_WRITE);
    dataFile.println(nomeAluno);
    dataFile.close();
    nomeAluno = "";
  }
  
  
}

void exibePrimeiroNomeAluno(int indexEscolhido){
  String nomeAluno = retornaNomeAluno(indexEscolhido);
  Serial.println(nomeAluno);
  lcd.clear();
  lcd.setCursor(0,0);
  nomeAluno.remove(nomeAluno.indexOf(" "));
  lcd.print(F("Obrigado"));
  lcd.setCursor(0,1);
  lcd.print(nomeAluno);
  delay(1000);
  Serial.println(nomeAluno);
  
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

  
void buscarArquivoLista(){

  if(SD.exists("lista.txt")){
    exibeMsg(F("ARQUIVO ALUNOS  ENCONTRADO"));
  }else{
    exibeMsg(F("ARQUIVO ALUNOS  NAO ENCONTRADO"));
    while (1) { delay(1); } // trava o codigo pois ocorreu erro
  }
}

String retornaNomeAluno(int indexEscolhido){
    
  File dataFile = SD.open("lista.txt");
  int indexAtual = 0;
  String nomeAluno = "";
  
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
          nomeAluno += c;
        }
      }
    }
 
  
  dataFile.close();
  return nomeAluno;
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
      exibeMsg(F("AGUARDANDO"));
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
      exibeMsg(F("AGUARDANDO"));
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
      Serial.println(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(F("No finger detected"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      return p;
    default:
      Serial.println(F("Unknown error"));
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
    exibeMsg("Pesquisando Digital");
    return 0;
  }
  
  /*
  found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID);
  
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence);*/
  return finger.fingerID; 
}



int addIdAoArquivo(int indexParaCadastro,int idParaCadastro)
{
  File dataFile = SD.open("lista.txt");
  int indexAtual = 0;
  String nomeAluno = "";
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

          //Serial.println("estou aqui 1");

          if (dataFile){

            //dataFile.print('\n');

            dataFile.print(nomeAluno);
           // Serial.println("estou aqui 2");

          }else{
            Serial.println(F("nao conseguiu abrir o arquivo de texto lista2.txt "));
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
            Serial.println(F("PRINTADO NO ARQUIVO NOVO."));
         } else {
            Serial.println(F("error opening LISTA2.txt"));
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
    String nomeAluno = "";

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

          Serial.println(F("PRINTADO NO ARQUIVO final."));
         } else {
            Serial.println(F("erro ao tentar abrir LISTA.txt"));
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
    //Serial.print('.');
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

// Função que printa o aluno que esta no index selecionado e retorna o seu ID
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
