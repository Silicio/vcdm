/*
  Name: VCDM (Vincenzo Consiglio Dictionary Manager)
  Copyright: 
  Author: Vincenzo Consiglio
  Date: 28/04/05 23.22
  Description: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

typedef struct word{
  char parola[32];
  struct word *next;
}cParola;

cParola *dizRoot=NULL;

// +++++++++++++ PROTOTIPI DELLE FUNZIONI +++++++++++++

int menu();
void pausa();
int fileLength(FILE *f);
char * caricaFile();
cParola * loadDizionario();
void importaDaFile();
void opzioni();
void stampaIntestazione();
void inserisciParola();
int cercaDizionario(cParola *pt, char stringa[]);
int insertWordDizionario(char dizWord[]);


// +++++++++++++ MAIN +++++++++++++

int main(int argc, char *argv[]){
  cParola *temp;
  FILE *fDiz;
  cParola *dizRoot;
  
//  printf("Il dizionario attualmente contiene %d parole\n\n",loadDizionario());
  dizRoot=loadDizionario();

  while(1){
    switch(menu()){
      case 1:{
        inserisciParola();
        break;
      }
      case 2:{
        importaDaFile();
        pausa();
        break;
      }
      case 3:{
        break;
      }
      case 4:{
        break;
      }
      case 5:{
        break;
      }
      case 6:{
        opzioni();
        pausa();
        break;
      }
      case 0:{
        if(!(fDiz=fopen("dizionario.txt","w"))) printf("\n\nErrore nel salvataggio del dizionario!");
        else {
          while(dizRoot){
            fprintf(fDiz,"%s\n",dizRoot->parola);
            temp=dizRoot;
            free(temp);
            dizRoot=dizRoot->next;
          }
          printf("\n\nIl dizionario e' stato salvato correttamente.");
          sleep(1200);
        }
        fclose(fDiz);
        exit(0);
      }
    }
  }
  pausa();	
  return 0;
}

void stampaIntestazione(){
  register i;
  char c=205;
  system("cls");
  printf("\t\t\t\t*****************\n");
  printf("\t\t\t\t** VCDM  v0.3c **\n");
  printf("\t\t\t\t*****************\n\n");
  for (i=0;i<16;i++) printf("%c",c);
  printf(" Autore: Vincenzo Consiglio (scorpion@email.it) ");
  for (i=0;i<16;i++) printf("%c",c);
  printf("\n\n");
}

void pausa(){
  printf("\n\n");
  system("PAUSE");
}

int menu(){
  int scelta=-1;
  while ((scelta<0)||(scelta>6)){
    stampaIntestazione();
    printf("\tMenu Principale\n\n");
    printf("\t1) Aggiungi una nuova parola\n");
    printf("\t2) Importa parole da file\n");
    printf("\t3) #Cerca una parola\n");
    printf("\t4) #Cancella una parola\n");
    printf("\t5) #Cancella tutto il dizionario\n");
    printf("\t6) #Opzioni\n");
    printf("\n\t0) Salva & Esci\n\n>");
    scelta=getch();
    scelta-=48;
    //scanf("%d",&scelta);
  }
  return scelta;
}

// +++++++++++++ INIZIO IMPLEMENTAZIONE DELLE FUNZIONI +++++++++++++

/*
  QUESTA FUNZIONE PRENDE IN INPUT LO STREAM DEL FILE APERTO E RESTITUISCE LA GRANDEZZA
  IN BYTE DEL FILE RELATIVO ALLO STREAM RICEVUTO COME PARAMETRO
*/
int fileLength(FILE *f){
  int pos;
  int end;

  pos=ftell(f);
  fseek(f,0,SEEK_END);
  end=ftell(f);
  fseek(f,pos,SEEK_SET);
  return end;
}

/*
  QUESTA FUNZIONE CARICA UN FILE IL CUI NOME VIENE RICHIESTO E RESTITUISCE UN PUNTATORE
  DI TIPO CHAR AL PRIMO ELEMENTO DELL'AREA DI MEMORIA IN CUI IL FILE E' STATO CARICATO
*/
char * caricaFile(){
  char *inStringa, *inStringa2, path[32];
  int fileSize;
  register i;
  FILE *fp;

  stampaIntestazione();
  printf("Inserire il nome del file da caricare: ");
  scanf("%s",path);  
  if(!(fp=fopen(path,"r"))) {
    printf("\nApertura del file %s in lettura non riuscita!\n\n",path);
    pausa();
    abort();
  }
  else{
    fileSize=(1+fileLength(fp));
    printf("\nApertura del file %s (%d byte) in lettura eseguita\n",path,(fileSize-1));
  }
  inStringa=inStringa2=malloc(fileSize);

// carico il testo dal file inserito in "path" e lo carico nell'area di memoria puntata da inStringa
  while(feof(fp)==NULL){
    *inStringa=fgetc(fp);
    inStringa++;
  }
  inStringa--;
  *inStringa='\0';
/*
// verifico che il file sia stato caricato correttamente stampandolo a video
  printf("Testo originale caricato in memoria:\n\n");
  for(i=0;i<80;i++) printf("*");
  inStringa=inStringa2;
  while(*inStringa!='\0'){
    printf("%c",*inStringa);
    inStringa++;
  }
  printf("\n");
  for(i=0;i<80;i++) printf("*");
*/
  fclose(fp);
  printf("\nChiusura del file %s\n",path);
  pausa();
  return inStringa2;
}

int cercaDizionario(cParola *pt, char stringa[]){
  while((pt)&&(strncmp(pt->parola,stringa,31)<0)) pt=pt->next;
  if((pt)&&(strncmp(pt->parola,stringa,31)==0)) return 1;
  else return 0;    
}

int insertWordDizionario(char dizWord[]){
  short int io;
  extern cParola *dizRoot;
  cParola *corr=NULL, *prev=NULL, *newWord;

  io=cercaDizionario(dizRoot, dizWord);
  if(!io){
    if(!dizRoot){
      dizRoot=(cParola *)malloc(sizeof(cParola));
      strncpy(dizRoot->parola,dizWord,32);
      dizRoot->next=NULL;
    }
    else if (dizRoot){
      prev=corr=dizRoot;
      newWord=(cParola *)malloc(sizeof(cParola));
      while((corr)&&(strncmp(corr->parola,dizWord,31)<0)){
        prev=corr;
        corr=corr->next;
      }
      if(corr==dizRoot){
        newWord->next=dizRoot;
        strncpy(newWord->parola,dizWord,32);
        dizRoot=newWord;
      }
      else if (prev){
        newWord->next=prev->next;
        prev->next=newWord;
        strncpy(newWord->parola,dizWord,32);
      }
    }
    return 1;
  }
  else return 0;
}

cParola * loadDizionario(){
  register i;
  cParola *root=NULL, *corr=NULL, *prev=NULL, *newWord;
  char dizWord[32];
  int x, num=0;
  FILE *fp;

  if(!(fp=fopen("dizionario.txt","r"))) {
    printf("Non e' stato trovato un dizionario. Ne verra' creato uno nuovo!");
    sleep(3000);
    fp=fopen("dizionario.txt","w");
    printf("\n\nE' stato creato il nuovo dizionario");
    sleep(1200);
  }

//  else printf("\nApertura del file dizionario.txt riuscita");
  i=0;
  while(!feof(fp)){
    x=fgetc(fp);
    x=tolower(x);
    dizWord[i]=x; 
    if(x=='\n'){
      dizWord[i]='\0';
      num++;
      if(!root){
        root=(cParola *)malloc(sizeof(cParola));
        strncpy(root->parola,dizWord,32);
        root->next=NULL;
      }
      else if (root){
        prev=corr=root;
        newWord=(cParola *)malloc(sizeof(cParola));
        while((corr)&&(strncmp(corr->parola,dizWord,31)<0)){
          prev=corr;
          corr=corr->next;
        }
        if(corr==root){
          newWord->next=root;
          strncpy(newWord->parola,dizWord,32);
          root=newWord;
        }
        else if (prev){
          newWord->next=prev->next;
          prev->next=newWord;
          strncpy(newWord->parola,dizWord,32);
        }
      }
      i=-1;
    }
    i++;
  }
  if(fp) fclose(fp);
//  printf("\nChiusura del file dizionario.txt");
  printf("Sono state caricate %d parole in memoria dal dizionario.\n",num);
  sleep(1200);
  return root;
}

void inserisciParola(){
  char c=0, newParola[32];
  register i;

  stampaIntestazione();
  printf("Digitare la parola da inserire: ");
/*  i=0;
  while(((c=getch())!=13)&&(i<32)){
    if (((c>64)&&(c<91))||((c>96)&&(c<123))){
      c=toupper(c);
      printf("%c",c);
      newParola[i]=c;
      i++;
    }
  }
  newParola[i]='\0';
  */
  scanf("%s",newParola);
  i=0;
  while(newParola[i]!='\0'){
    newParola[i]=tolower(newParola[i]);
    i++;
  }
  if(insertWordDizionario(newParola)) printf("\nParola inserita nel dizionario");
  else printf("\nLa parola e' gia' presente nel dizionario");
  sleep(1200);
}


void importaDaFile(){
  register i;
  char *inStringa, *inStringaCopy, alfabeto[59], newParola[32];
  cParola *dizTemp;
  FILE *fp;
  
  alfabeto[0]='A';
  alfabeto[1]='B';
  alfabeto[2]='C';
  alfabeto[3]='D';
  alfabeto[4]='E';
  alfabeto[5]='F';
  alfabeto[6]='G';
  alfabeto[7]='H';
  alfabeto[8]='I';
  alfabeto[9]='J';
  alfabeto[10]='K';
  alfabeto[11]='L';
  alfabeto[12]='M';
  alfabeto[13]='N';
  alfabeto[14]='O';
  alfabeto[15]='P';
  alfabeto[16]='Q';
  alfabeto[17]='R';
  alfabeto[18]='S';
  alfabeto[19]='T';
  alfabeto[20]='U';
  alfabeto[21]='V';
  alfabeto[22]='W';
  alfabeto[23]='X';
  alfabeto[23]='Y';
  alfabeto[25]='Z';
  alfabeto[26]='a';
  alfabeto[27]='b';
  alfabeto[28]='c';
  alfabeto[29]='d';
  alfabeto[30]='e';
  alfabeto[31]='f';
  alfabeto[32]='g';
  alfabeto[33]='h';
  alfabeto[34]='i';
  alfabeto[35]='j';
  alfabeto[36]='k';
  alfabeto[37]='l';
  alfabeto[38]='m';
  alfabeto[39]='n';
  alfabeto[40]='o';
  alfabeto[41]='p';
  alfabeto[42]='q';
  alfabeto[43]='r';
  alfabeto[44]='s';
  alfabeto[45]='t';
  alfabeto[46]='u';
  alfabeto[47]='v';
  alfabeto[48]='w';
  alfabeto[49]='x';
  alfabeto[50]='y';
  alfabeto[51]='z';
  alfabeto[52]=' ';
  alfabeto[53]=-32; // à
  alfabeto[54]=-24; // è
  alfabeto[55]=-23; // é
  alfabeto[56]=-20; // ì
  alfabeto[57]=-14; // ò
  alfabeto[58]=-7;  // ù


  inStringa=inStringaCopy=caricaFile(); // CARICO IL FILE DA IMPORTARE IN MEMORIA

/*  printf("\n\nTesto caricato:\n");
  inStringa=inStringaCopy;
  while(*inStringa!='\0'){
    if (*inStringa==-32) printf("%c",133);
    printf("%d",*inStringa);
    inStringa++;
  }
*/  
// ROUTINE PER LA PULIZIA DEL TESTO IN MEMORIA DA CARATTERI NON ALFABETICI
  inStringa=inStringaCopy;
  while(*inStringa!='\0'){
    i=0;
    while((*inStringa!=alfabeto[i])&&(i<59)) i++;
    if(i>58) *inStringa=' ';
    if (*inStringa==-32) *inStringa=133; // à
    else if(*inStringa==-24) *inStringa=138; // è
    else if(*inStringa==-23) *inStringa=130; // é
    else if(*inStringa==-20) *inStringa=141; // ì
    else if(*inStringa==-14) *inStringa=149; // ò
    else if(*inStringa==-7) *inStringa=151; // ù
    inStringa++;
  }
// FINE ROUTINE

  printf("\n\nRisultato pulizia testo:\n");
  inStringa=inStringaCopy;
  while(*inStringa!='\0'){
    printf("%c",*inStringa);
    inStringa++;
  }
  
  fp=fopen("paroleprese.txt","w");
  printf("\n\nParole prese:\n");
  inStringa=inStringaCopy;
  i=0;
  while(*inStringa!='\0'){
    if(i==0){
      while((*inStringa==' ')&&(*inStringa!='\0')) inStringa++;
    }
    newParola[i]=*inStringa;
    newParola[i]=toupper(newParola[i]);
    i++;
    if((*(inStringa+1)==' ')||(*(inStringa+1)=='\0')){
      if ((i>2)&&(i<32)){ // solo parole di 3 o più caratteri fino a 31
         newParola[i]='\0';
         if(insertWordDizionario(newParola)){
           printf("\n%s",newParola);
       //  else printf("\n%s",newParola);
     //   printf("|%s|\n",newParola,*(inStringa+1));
           fprintf(fp,"|%s|\n",newParola);
        }
      }
      inStringa++;
      i=0;
      while((*inStringa==' ')&&(*inStringa!='\0')) inStringa++;
    }
    else inStringa++;
  }
  fclose(fp);
// LIBERO LA MEMORIA
  free(inStringaCopy);
}

void opzioni(){
  FILE *fp;
  char *ptCfg,*temp, config[30];

  if(!(fp=fopen("config.cfg","r+"))){
    printf("\n\nErrore nell'apertura del file config.cfg, file danneggiato o mancante.\n\n");
    pausa();
    abort();   
  }
  ptCfg=temp=malloc(1024);
  fgets(ptCfg,1024,fp);
  stampaIntestazione();
  ptCfg+=8;
  printf("Nome del dizionario: %s",ptCfg);
  printf("\n\nInserisci un nuovo nome, o premi invio per lasciare quello di default:\n");
  gets(&config);
  pausa();
  fclose(fp);
}
