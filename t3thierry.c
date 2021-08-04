#include <stdio.h>
#include <stdlib.h>        
#include <time.h>
#define LEN 150
#include <winsock2.h>

int main(int argc , char *argv[]){
	
	char buf[LEN];
	time_t curtime;
	struct tm loc_time;
    char smscompleta[200];

	WSADATA wsa;
	int sockfd, newsockfd, clilen; 
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		printf("Falha. Codigo de erro : %d",WSAGetLastError());
		return 1;
	}
	
	// Cria o descritor do socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		fprintf(stderr,"ERRO: impossivel criar socket.\n");
        exit(0);
    }

	// Zera e preenche a struct serv_addr com as informacoes de conexao
	//bzero((char *) &serv_addr, sizeof(serv_addr));
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;				// Sempre AF_INET
	serv_addr.sin_addr.s_addr = INADDR_ANY;     // Vai fazer bind em qualquer ip associado
	serv_addr.sin_port = htons(3123);           // Porta fixa 3123

	// Faz o bind do socket, isto e, associa todas as info da struct preenchida com o socket criado
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr,"ERRO: Provavelmente a porta esta em uso.\n");
        exit(0);
	}
	// Coloca em modo servidor, isto e, aguarda conexoes neste socket
	listen(sockfd,5);
	printf("Estou aguardando conexoes!\n");

	//Explicação geral do que fazer::: implementação do loop infinito: 
	//accept= obtendo a data e a hora(montando a string-ex 20-06-2021-18:30:12)
	
	while(1) {                   //Criando o Loop infinito apartir de estudos(com {while})
		
	clilen = sizeof(cli_addr);   // Parte do accept
	// Chamada bloqueante! Aguarda novas conexoes e preenche cli_addr
	// com as informacoes do cliente
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	
	curtime = time(NULL);              	// Obtendo a hora correta pelo servidor.
					                    //Data certa de entrada do servidor.
	loc_time = *localtime (&curtime);  // Mostra hora e data no format padrão?
	
	if (newsockfd < 0) {
		fprintf(stderr,"ERRO: Falha no aceite da conexao.\n");
       	exit(0);
	}
	//Le uma mensagem socket (vinda do cliente)	
	//bzero(buffer,256);
	memset(buffer,0, 256);
		
	// usanodo o sprintf conforme foi ensinado
	sprintf(smscompleta,"%d/%d/%d    %d:%d:%d\n"   ,loc_time.tm_mday,loc_time.tm_mon+1, loc_time.tm_year + 1900, loc_time.tm_hour, loc_time.tm_min, loc_time.tm_sec); 
	
	n = send(newsockfd, smscompleta, strlen(smscompleta) ,0); // utilizando o send para imprimir com o auxilio da variavel{smscompleta} do (sprintf)
	

	if (n < 0) {
    	fprintf(stderr,"ERRO: Falha de escrita no socket.\n");
        exit(0);
    }
	

}
}

	


