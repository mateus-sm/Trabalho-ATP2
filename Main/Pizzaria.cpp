#include <stdio.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

struct TpData{
	int d, m, a;
};

struct TpCliente{
	char telefone[20]; //Chave primaria
	char cep[15], nome[50], endereco[50], cidade[30];
};

struct TpMotoqueiro{
	TpData data;
	char cpf[20]; //Chave primaria
	char telefone[20], nome[50], endereco[50];
};

struct TpPizzas{
	int codigo; //Chave primaria
    float valor;
	char descricao[30];
};

struct TpPedidos{
	TpData dataPedido;
    int numero; //Chave primaria
    char telefone[20];
    int codigo;
    char cpf[20];
	char situacao[30];
};

int validarCPF(char ncpf[15]);
int validarInt(char str[11]);

void cadastrarCliente(void);
void cadastrarMotoqueiro(void);
void cadastrarPizza(void);
void cadastrarPedido(void);

void exibirCliente(void);
void exibirMotoqueiro(void);
void exibirPizza(void);
void exibirPedidos(void);

void alterarCliente(void);
void alterarMotoqueiro(void);
void alterarPizza(void);
void alterarPedido(void);

void exclusaofisicaCliente(void);
void exclusaofisicaMotoqueiro(void);
void exclusaofisicaPizza(void);
void exclusaofisicaPedido(void);

int buscaCodigo(FILE *ptr, int cod);
int buscaCPF(FILE *ptr, char texto[50]);
int buscaTelefone(FILE *ptr, char texto[50]);
int buscaPedido(FILE *ptr, int pedido);

char menu(void);


int main(void){
	char op;
	
	do {
		op = menu();
		clrscr();
		
		switch(op){
			case 'A':
				cadastrarCliente();
				break;
				
			case 'B':
				cadastrarMotoqueiro();
				break;
				
			case 'C':
				cadastrarPizza();
				break;
				
			case 'D':
				cadastrarPedido();
				break;	

			case 'E':
				exibirCliente();
				break;	

			case 'F':
				exibirMotoqueiro();
				break;

			case 'G':
				exibirPizza();
				break;

			case 'H':
				exibirPedidos();
				break;	
			
			case 'I':
				alterarCliente();
				break;	
				
			case 'J':
				alterarMotoqueiro();
				break;	
				
			case 'K':
				alterarPizza();
				break;
				
			case 'L':
				alterarPedido();
				break;
				
			case 'M':
				exclusaofisicaCliente();
				break;
				
			case 'N':
				exclusaofisicaMotoqueiro();
				break;
				
			case 'O':
				exclusaofisicaPizza();
				break;
				
			case 'P':
				exclusaofisicaPedido();
				break;
		}
			
	} while(op != 27);
}

void exclusaofisicaPedido(void){
	TpPedidos aux;
	int flag, auxnum;
	
	FILE *ptr = fopen("Pedidos.dat", "rb");
	
	if(ptr == NULL)
		printf("ERRO de abertura\n");
	else{
		printf("Digite o NUMERO do pedido que deseja excluir: \n");
		fflush(stdin);
		scanf("%d", &auxnum);
		
		flag = buscaPedido(ptr, auxnum);
		
		while (flag == -1 && auxnum > 0){
			printf("Insira um numero do pedido CADASTRADO: \n");
			fflush(stdin);
			scanf("%d", &auxnum);
		
			flag = buscaPedido(ptr, auxnum);
		}
		
		if(auxnum > 0){
			fseek(ptr, flag, 0); //vai para a pos encontrada
			fread(&aux, sizeof(TpPedidos), 1, ptr);
			printf("---> Dados do PEDIDO <---\n");
			printf("Numero: %d\n", aux.numero);
			printf("Telefone: %s\n", aux.telefone);
			printf("Codigo da pizza: %d\n", aux.codigo);
			printf("CPF: %s\n", aux.cpf);
			printf("Situacao: %s\n", aux.situacao);
			printf("Data do pedido: %d/%d/%d \n", aux.dataPedido.d, aux.dataPedido.m, aux.dataPedido.a);
			
			printf("Deseja prosseguir com a exclusao? (Y-Yes/N-No)\n");
			
			if(toupper(getche()) == 'Y'){
				FILE *ptrnovo = fopen("novo.dat", "wb"); //wb permite criar o arquivo //se ja estiver criado o arquivo � zerado
				fseek(ptr, 0, 0);
				fread(&aux, sizeof(TpPedidos), 1, ptr);
					
				while(!feof(ptr)){ //enquanto o arquivo p essoa n�o chegar no fim
					if(auxnum != aux.numero)
						fwrite(&aux, sizeof(TpPedidos), 1, ptrnovo);
			
					fread(&aux, sizeof(TpPedidos), 1, ptr);//ve se nao � EOF
				}
				
				//fecha e renomeia os arquivos
					fclose(ptr);
					fclose(ptrnovo);
					remove("Pedidos.dat");
					rename("novo.dat", "Pedidos.dat");
					printf("\nExclusao realizada com sucesso!\n");

			} else{
				printf("\nExclusao nao realizada\n");
				fclose(ptr);
			}
		}
		fclose(ptr);
		getch();
		clrscr();	
	}
}

void exclusaofisicaCliente(void){
	TpCliente aux;
	int flag;
	char tel[30];

	FILE *ptr = fopen("Clientes.dat", "rb");
	
	if(ptr == NULL)
		printf("ERRO de abertura\n");
	else{
		printf("Digite o TELEFONE do cliente que deseja excluir: \n");
		fflush(stdin);
		gets(tel);
		
		flag = buscaTelefone(ptr, tel);
		
		while (flag == -1 && strlen(tel) > 0){
			printf("Insira um telefone CADASTRADO: \n");
			fflush(stdin);
			gets(tel);
		
			flag = buscaTelefone(ptr, tel);
		}
		
		if(strlen(tel) > 0){
			fseek(ptr, flag, 0); //vai para a pos encontrada
			fread(&aux, sizeof(TpCliente), 1, ptr);
			printf("---> Dados do CLIENTE <---\n");
			printf("Telefone: %s\n", aux.telefone);
			printf("Nome: %s\n", aux.nome);
			printf("Endereco: %s\n", aux.endereco);
			printf("Cidade: %s\n", aux.cidade);
			printf("Cep: %s\n", aux.cep);
			
			printf("Deseja prosseguir com a exclusao? (Y-Yes/N-No)\n");
			
			if(toupper(getche()) == 'Y'){
				FILE *ptrnovo = fopen("novo.dat", "wb"); //wb permite criar o arquivo //se ja estiver criado o arquivo � zerado
				fseek(ptr, 0, 0);
				fread(&aux, sizeof(TpCliente), 1, ptr);
					
				while(!feof(ptr)){ //enquanto o arquivo p essoa n�o chegar no fim
					if(strcmp(tel, aux.telefone) != 0)
						fwrite(&aux, sizeof(TpCliente), 1, ptrnovo);
			
					fread(&aux, sizeof(TpCliente), 1, ptr);//ve se nao � EOF
				}
				
				//fecha e renomeia os arquivos
					fclose(ptr);
					fclose(ptrnovo);
					remove("Clientes.dat");
					rename("novo.dat", "Clientes.dat");
					printf("\nExclusao realizada com sucesso!\n");

			} else{
				printf("\nExclusao nao realizada\n");
				fclose(ptr);
			}
		}
		fclose(ptr);
		getch();
		clrscr();	
	}
}

void exclusaofisicaMotoqueiro(void){
	TpMotoqueiro aux;
	int flag;
	char cpf[30];

	FILE *ptr = fopen("Motoqueiros.dat", "rb");
	
	if(ptr == NULL)
		printf("ERRO de abertura\n");
	else{
		printf("Digite o CPF do motoqueiro que deseja excluir: \n");
		fflush(stdin);
		gets(cpf);
		
		flag = buscaCPF(ptr, cpf);
		
		while (flag == -1 && strlen(cpf) > 0){
			printf("Insira um CPF CADASTRADO: \n");
			fflush(stdin);
			gets(cpf);
		
			flag = buscaCPF(ptr, cpf);
		}
		
		if(strlen(cpf) > 0){
			fseek(ptr, flag, 0); //vai para a pos encontrada
			fread(&aux, sizeof(TpMotoqueiro), 1, ptr);
			printf("---> Dados do MOTOQUEIRO <---\n");
			printf("CPF: %s\n", aux.cpf);
			printf("Nome: %s\n", aux.nome);
			printf("Endereco: %s\n", aux.endereco);
			printf("Telefone: %s\n", aux.telefone);
			printf("Data de admissao: %d/%d/%d \n", aux.data.d, aux.data.m, aux.data.a);
			
			printf("Deseja prosseguir com a exclusao? (Y-Yes/N-No)\n");
			
			if(toupper(getche()) == 'Y'){
				FILE *ptrnovo = fopen("novo.dat", "wb"); //wb permite criar o arquivo //se ja estiver criado o arquivo � zerado
				fseek(ptr, 0, 0);
				fread(&aux, sizeof(TpMotoqueiro), 1, ptr);
					
				while(!feof(ptr)){ //enquanto o arquivo p essoa n�o chegar no fim
					if(strcmp(cpf, aux.cpf) != 0)
						fwrite(&aux, sizeof(TpMotoqueiro), 1, ptrnovo);
			
					fread(&aux, sizeof(TpMotoqueiro), 1, ptr);//ve se nao � EOF
				}
				
				//fecha e renomeia os arquivos
					fclose(ptr);
					fclose(ptrnovo);
					remove("Motoqueiros.dat");
					rename("novo.dat", "Motoqueiros.dat");
					printf("\nExclusao realizada com sucesso!\n");

			} else{
				printf("\nExclusao nao realizada\n");
				fclose(ptr);
			}
		}
		fclose(ptr);
		getch();
		clrscr();	
	}
}

void exclusaofisicaPizza(void){
	TpPizzas aux;
	int flag, auxcod;

	FILE *ptr = fopen("Pizzas.dat", "rb");
	
	if(ptr == NULL)
		printf("ERRO de abertura\n");
	else{
		printf("Digite o CODIGO DA PIZZA que deseja excluir: \n");
		fflush(stdin);
		scanf("%d",&auxcod);
		
		flag = buscaCodigo(ptr, auxcod);
		
		while (flag == -1 && auxcod > 0){
			printf("Insira um codigo da pizza CADASTRADO: \n");
			fflush(stdin);
			scanf("%d",&auxcod);
			
			flag = buscaCodigo(ptr, auxcod);
		}
		
		if(auxcod > 0){
			fseek(ptr, flag, 0); //vai para a pos encontrada
			fread(&aux, sizeof(TpPizzas), 1, ptr);
			printf("---> Dados da PIZZA <---\n");
			printf("Codigo: %d\n", aux.codigo);
			printf("Descricao: %s\n", aux.descricao);
			printf("Valor: %.2f\n", aux.valor);
			
			printf("Deseja prosseguir com a exclusao? (Y-Yes/N-No)\n");
			
			if(toupper(getche()) == 'Y'){
				FILE *ptrnovo = fopen("novo.dat", "wb"); //wb permite criar o arquivo //se ja estiver criado o arquivo � zerado
				fseek(ptr, 0, 0);
				fread(&aux, sizeof(TpPizzas), 1, ptr);
					
				while(!feof(ptr)){ //enquanto o arquivo p essoa n�o chegar no fim
					if( aux.codigo != auxcod)
						fwrite(&aux, sizeof(TpPizzas), 1, ptrnovo);
			
					fread(&aux, sizeof(TpPizzas), 1, ptr);//ve se nao � EOF
				}
				
				//fecha e renomeia os arquivos
					fclose(ptr);
					fclose(ptrnovo);
					remove("Pizzas.dat");
					rename("novo.dat", "Pizzas.dat");
					printf("\nExclusao realizada com sucesso!\n");

			} else{
				printf("\nExclusao nao realizada\n");
				fclose(ptr);
			}
		}
		fclose(ptr);
		getch();
		clrscr();	
	}
}

void alterarPedido(void){
	TpPedidos aux;
	int flag;
	char op;
	
	FILE *ptrpedido = fopen("Pedidos.dat", "rb+");
	FILE *ptrcliente = fopen("Clientes.dat", "rb+");
	FILE *ptrmotoqueiro = fopen("Motoqueiros.dat", "rb+");
	FILE *ptrpizza = fopen("Motoqueiros.dat", "rb+");
	
	if(ptrpedido == NULL)
		printf("ERRO de abertura \n");
	else{
		printf("Insira o NUMERO do Pedido que desja ALTERAR: \n");
		fflush(stdin);
		scanf("%d",&aux.numero);
		
		flag = buscaPedido(ptrpedido, aux.numero);
		
		while (flag == -1 && aux.numero > 0){
			printf("Insira um NUMERO do pedido cadastrado: \n");
			fflush(stdin);
			scanf("%d",&aux.numero);
			
			flag = buscaPedido(ptrpedido, aux.numero);
		}
		
		if(aux.numero > 0){
			printf("O que deseja ALTERAR?\n");
			printf("[A] NUMERO\n");
			printf("[B] TELEFONE\n");
			printf("[C] CODIGO DA PIZZA\n");
			printf("[D] CPF DO MOTOQUEIRO\n");
			printf("[E] SITUACAO\n");
			printf("[F] DATA DO PEDIDO\n");
			
			op = toupper(getche());
			
			fseek(ptrpedido, flag, 0);
			fread(&aux,sizeof(TpCliente), 1, ptrpedido);
			fseek(ptrpedido, flag, 0);
			
			if(op == 'A'){
				printf("\nInsira o novo NUMERO do pedido: \n");
				scanf("%d", &aux.numero);
				
				while(aux.numero <= 0){
					printf("Insira um numero VALIDO: \n");
					scanf("%d", &aux.numero);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'B'){
				printf("\nInsira o novo TELEFONE do pedido: \n");
				fflush(stdin);
				gets(aux.telefone);
				flag = buscaTelefone(ptrcliente, aux.telefone);
				
				while(strlen(aux.telefone) <= 0 || flag == -1){
					printf("Insira um telefone CADASTRADO: \n");
					fflush(stdin);
					gets(aux.telefone);
					flag = buscaTelefone(ptrcliente, aux.telefone);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'C'){
				printf("\nInsira o novo CODIGO DA PIZZA do pedido \n");
				scanf("%d", &aux.codigo);
				flag = buscaCodigo(ptrpizza, aux.codigo);
				
				while(aux.codigo < 0 || flag == -1){
					printf("Insira um codigo da pizza CADASTRADO \n");
					scanf("%d", &aux.codigo);
					flag = buscaCodigo(ptrpizza, aux.codigo);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'D'){
				printf("\nInsira o novo CPF DO MOTOQUEIRO do pedido \n");
				fflush(stdin);
				gets(aux.cpf);
				flag = buscaCPF(ptrmotoqueiro, aux.cpf);
				
				while(strlen(aux.cpf) <= 0 || flag == -1){
					printf("Insira um cpf CADASTRADO: \n");
					fflush(stdin);
					gets(aux.cpf);
					flag = buscaCPF(ptrmotoqueiro, aux.cpf);
				}
				
				printf("Dados ALTERADOS\n");
			} else if(op == 'E'){
				printf("\nPara qual SITUACAO do pedido voce deseja alterar: \n");
				printf("[A] Em preparacao \n");
				printf("[B] Em rota de entrega \n");
				printf("[C] Entregue \n");
				printf("[D] Cancelado \n");
				
				op = toupper(getche());
				
				if(op == 'A')
					strcpy(aux.situacao,"Em preparacao");
				else if(op == 'B')
					strcpy(aux.situacao,"Em rota de entrega");
				else if(op == 'C')
					strcpy(aux.situacao,"Entregue");
				else
					strcpy(aux.situacao,"Cancelado");
				
				printf("\nDados ALTERADOS\n");
			}else if(op == 'F'){
				printf("\nInsira a nova DATA DO PEDIDP: [dd mm aaaa]\n");
				scanf("%d %d %d", &aux.dataPedido.d, &aux.dataPedido.m, &aux.dataPedido.a);
				
				while(aux.dataPedido.d > 31 || aux.dataPedido.d < 1 || aux.dataPedido.m > 12 || aux.dataPedido.m < 1 || aux.dataPedido.a > 2024 || aux.dataPedido.a < 1){
					printf("Insira uma data VALIDA\n");
					scanf("%d %d %d", &aux.dataPedido.d, &aux.dataPedido.m, &aux.dataPedido.a);
				}
				
				printf("\nDados ALTERADOS\n");
			}else
				printf("Altera��o de dados CANCELADA\n");
				
			fwrite(&aux, sizeof(TpPedidos), 1, ptrpedido);

		}
		
		getch();
		fclose(ptrpedido);
		fclose(ptrpizza);
		fclose(ptrcliente);
		fclose(ptrmotoqueiro);
		clrscr();
	}
	
}
void alterarPizza(void){
	TpPizzas aux;
	int flag;
	char op;
	
	FILE *ptr = fopen("Pizzas.dat", "rb+");
	
	if(ptr == NULL)
		printf("ERRO de abertura \n");
	else{
		printf("Insira o CODIGO da Pizza que desja ALTERAR: \n");
		fflush(stdin);
		scanf("%d",&aux.codigo);	
		flag = buscaCodigo(ptr, aux.codigo);
		
		while (flag == -1 && aux.codigo > 0){
			printf("Insira um CODIGO cadastrado: \n");
			fflush(stdin);
			scanf("%d",&aux.codigo);
			flag = buscaCodigo(ptr, aux.codigo);

		}
		
		if(aux.codigo > 0){
			fseek(ptr, flag, 0);
			printf("O que deseja ALTERAR?\n");
			printf("[A] CODIGO\n");
			printf("[B] DESCRICAO\n");
			printf("[C] VALOR\n");
			
			op = toupper(getche());
			
			fseek(ptr, flag, 0);
			fread(&aux,sizeof(TpCliente), 1, ptr);
			fseek(ptr, flag, 0);
			
			if(op == 'A'){
				printf("\nInsira o novo CODIGO: \n");
				scanf("%d", &aux.codigo);
				
				while(aux.codigo <= 0){
					printf("Insira um codigo VALIDO: \n");
					scanf("%d", &aux.codigo);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'B'){
				printf("\nInsira a nova DESCRICAO: \n");
				fflush(stdin);
				gets(aux.descricao);
				
				while(strlen(aux.descricao) <= 0){
					printf("Insira uma descricao VALIDA: \n");
					gets(aux.descricao);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'C'){
				printf("\nInsira o novo VALOR: \n");
				scanf("%f", &aux.valor);
				
				while(aux.valor <= 0){
					printf("Insira um valor VALIDO: \n");
					scanf("%f", &aux.valor);
				}
				
				printf("Dados ALTERADOS\n");
			}else
				printf("\nAltera��o de dados CANCELADA\n");
			
			fwrite(&aux, sizeof(TpPizzas), 1, ptr);

		}
		
		getch();
		fclose(ptr);
		clrscr();
	}
	
}

void alterarMotoqueiro(void){
	TpMotoqueiro aux;
	int flag;
	char op;
	
	FILE *ptr = fopen("Motoqueiros.dat", "rb+");
	
	if(ptr == NULL)
		printf("ERRO de abertura\n");
	else{
		printf("Insira o CPF do Motoqueiro que desja ALTERAR:\n");
		fflush(stdin);
		gets(aux.cpf);
		flag = buscaCPF(ptr, aux.cpf);
		
		while (flag == -1 && strlen(aux.cpf) > 0) {
			printf("Insira um CPF cadastrado:\n");
			fflush(stdin);
			gets(aux.cpf);
	
			flag = buscaCPF(ptr, aux.cpf);
		}
		
		if(strlen(aux.cpf) > 0){
			printf("O que deseja ALTERAR?\n");
			printf("[A] CPF\n");
			printf("[B] NOME\n");
			printf("[C] ENDERECO\n");
			printf("[D] TELEFONE\n");
			printf("[E] DATA DE ADMISSAO\n");
			
			op = toupper(getche());
			
			//printf("\nO tamanho de um reg eh: %d e o ponteiro esta em: %d\n", sizeof(TpMotoqueiro), flag);
			
			fseek(ptr, flag, 0);
			fread(&aux,sizeof(TpMotoqueiro), 1, ptr);
			fseek(ptr, flag, 0);
			
			if(op == 'A'){
				printf("\nInsira o novo CPF: \n");
				gets(aux.cpf);
				flag = validarCPF(aux.cpf);
				
				while(strlen(aux.cpf) <= 0 || flag != 1){
					printf("Insira um cpf VALIDO: \n");
					gets(aux.cpf);
					flag = validarCPF(aux.cpf);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'B'){
				printf("\nInsira o novo NOME: \n");
				gets(aux.nome);
				
				while(strlen(aux.nome) <= 0){
					printf("Insira um nome VALIDO: \n");
					gets(aux.nome);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'C'){
				printf("\nInsira o novo ENDERECO: \n");
				gets(aux.endereco);
				
				while(strlen(aux.endereco) <= 0){
					printf("Insira um endereco VALIDO: \n");
					gets(aux.endereco);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'D'){
				printf("\nInsira o novo TELEFONE: \n");
				gets(aux.telefone);
				
				while(strlen(aux.telefone) <= 0){
					printf("Insira um telefone VALIDO: \n");
					gets(aux.telefone);
				}
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'E'){
				printf("\nInsira a nova DATA DE ADMISSAO: [dd mm aaaa]\n");
				scanf("%d %d %d", &aux.data.d, &aux.data.m, &aux.data.a);
				
				while(aux.data.d > 31 || aux.data.d < 1 || aux.data.m > 12 || aux.data.m < 1 || aux.data.a > 2024 || aux.data.a < 1){
					printf("Insira uma data VALIDA\n");
					scanf("%d %d %d", &aux.data.d, &aux.data.m, &aux.data.a);
				}
				
				printf("Dados ALTERADOS\n");
			}else
				printf("\nAltera��o de dados CANCELADA\n");
				
			fwrite(&aux, sizeof(TpMotoqueiro), 1, ptr);

		}
		
		getch();
		fclose(ptr);
		clrscr();	
	}
	
}

void alterarCliente(void){
	TpCliente aux;
	int flag;
	char op;
	
	FILE *ptr = fopen("Clientes.dat", "rb+");
	
	if(ptr == NULL)
		printf("ERRO de abertura\n");
	else{
		printf("Insira o TELEFONE do Cliente que desja ALTERAR:\n");
		fflush(stdin);
		gets(aux.telefone);
		flag = buscaTelefone(ptr, aux.telefone);
		
		while (flag == -1 && strlen(aux.telefone) > 0) {
			printf("Insira um TELEFONE cadastrado:\n");
			fflush(stdin);
			gets(aux.telefone);
	
			flag = buscaTelefone(ptr, aux.telefone);
		}
		
		if(strlen(aux.telefone) > 0){
			
			printf("O que deseja ALTERAR?\n");
			printf("[A] TELEFONE\n");
			printf("[B] NOME\n");
			printf("[C] ENDERECO\n");
			printf("[D] CIDADE\n");
			printf("[E] CEP\n");
			
			op = toupper(getche());
			
			//printf("\nO tamanho de um reg eh: %d e o ponteiro esta em: %d\n", sizeof(TpCliente), flag);
			
			fseek(ptr, flag, 0);
			fread(&aux,sizeof(TpCliente), 1, ptr);
			fseek(ptr, flag, 0);
			
			if(op == 'A'){
				printf("\nInsira o novo TELEFONE: \n");
				gets(aux.telefone);
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'B'){
				printf("\nInsira o novo NOME: \n");
				gets(aux.nome);
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'C'){
				printf("\nInsira o novo ENDERECO: \n");
				gets(aux.endereco);
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'D'){
				printf("\nInsira a nova CIDADE: \n");
				gets(aux.cidade);
				
				printf("Dados ALTERADOS\n");
			}else if(op == 'E'){
				printf("\nInsira o novo CEP: \n");
				gets(aux.cep);
				
				printf("Dados ALTERADOS\n");
			}else
				printf("\nAltera��o de dados CANCELADA\n");
				
			fwrite(&aux, sizeof(TpCliente), 1, ptr);
			
		}
		
		getch();
		fclose(ptr);
		clrscr();
	}
		
}
int buscaTelefone(FILE *ptr, char texto[50]) {
	TpCliente aux;

	fseek(ptr, 0, 0);
	
	if (ptr == NULL) {
		printf("ERRO de abertura\n");
	} else {
		fread(&aux, sizeof(TpCliente), 1, ptr);

		while(!feof(ptr) && strcmp(texto, aux.telefone) != 0) {
			fread(&aux, sizeof(TpCliente), 1, ptr);
		}

		if (strcmp(aux.telefone, texto) == 0)
			return ftell(ptr) - sizeof(TpCliente);
		else 
			return -1;
	}
}

int buscaPedido(FILE *ptr, int pedido){
	TpPedidos aux;

	fseek(ptr, 0, 0);

	if (ptr == NULL) {
		printf("ERRO de abertura\n");
	} else {
		fread(&aux, sizeof(TpPedidos), 1, ptr);

		while(!feof(ptr) && aux.numero != pedido) {
			fread(&aux, sizeof(TpPedidos), 1, ptr);
		}

		if (aux.numero == pedido)
			return ftell(ptr) - sizeof(TpPedidos);
		else 
			return -1;
		
		
	}

}

int buscaCPF(FILE *ptr, char texto[50]) {
	TpMotoqueiro aux;

	fseek(ptr, 0, 0);

	if (ptr == NULL)
		printf("ERRO de abertura\n");
	else
		fread(&aux, sizeof(TpMotoqueiro), 1, ptr);

		while(!feof(ptr) && strcmp(texto, aux.cpf) != 0) {
			fread(&aux, sizeof(TpMotoqueiro), 1, ptr);
		}
		
		if(strcmp(aux.cpf, texto) == 0) 
			return ftell(ptr) - sizeof(TpMotoqueiro);
		else 
			return -1;

}

int buscaCodigo(FILE *ptr, int cod) {
	TpPizzas aux;

	fseek(ptr, 0, 0);

	if (ptr == NULL)
		printf("ERRO de abertura\n");
	else {
		fread(&aux, sizeof(TpPizzas), 1, ptr);

		while(!feof(ptr) && aux.codigo != cod) {
			fread(&aux, sizeof(TpPizzas), 1, ptr);
		}

		if (aux.codigo == cod) {
			return ftell(ptr) - sizeof(TpPizzas);
		} else {
			return -1;
		}	
	}

}

void cadastrarPedido(void) {
	TpPedidos aux;
	int flag;
	FILE *ptrpedido = fopen("Pedidos.dat", "ab");
	FILE *ptrcliente = fopen("Clientes.dat", "rb+");
	FILE *ptrmotoqueiro = fopen("Motoqueiros.dat", "rb+");
	FILE *ptrpizza = fopen("Pizzas.dat", "rb+");
	 
	printf("Insira o NUMERO do Pedido:\n");
	fflush(stdin);
	scanf("%d", &aux.numero);
	
	while(aux.numero > 0) {
		printf("Insira o TELEFONE do Cliente:\n");
		fflush(stdin);
		gets(aux.telefone);

		flag = buscaTelefone(ptrcliente, aux.telefone);
		//printf("\nBusca terminou flag vale: %d\n", flag);
		
		while (flag == -1 && strlen(aux.telefone) > 0) {
			printf("Insira um TELEFONE cadastrado:\n");
			fflush(stdin);
			gets(aux.telefone);

			flag = buscaTelefone(ptrcliente, aux.telefone);
		}

		if (strlen(aux.telefone) > 0) {
			printf("Insira o CODIGO dessa Pizza:\n");
			scanf("%d", &aux.codigo);

			flag = buscaCodigo(ptrpizza, aux.codigo);
			while (flag == -1 && aux.codigo > 0) {
				printf("Insira o CODIGO dessa Pizza:\n");
				scanf("%d", &aux.codigo);

				flag = buscaCodigo(ptrpizza, aux.codigo);
			}			

			if (aux.codigo > 0) {
				printf("Digite o CPF do Motoqueiro\n");
				fflush(stdin);
				gets(aux.cpf);
				
				flag = buscaCPF(ptrmotoqueiro, aux.cpf);
				while (flag == -1 && strlen(aux.cpf) > 0) {
					printf("Digite o CPF do Motoqueiro\n");
					fflush(stdin);
					gets(aux.cpf);

					flag = buscaCPF(ptrmotoqueiro, aux.cpf);
				}

				if (strlen(aux.cpf) > 0) {
					strcpy(aux.situacao, "Em preparacao"); 				
					
					printf("Insira a DATA do Pedido: [dd mm aaaa]\n");
					scanf("%d %d %d", &aux.dataPedido.d, &aux.dataPedido.m, &aux.dataPedido.a);

					fwrite(&aux, sizeof(TpPedidos), 1, ptrpedido);	

					printf("\nInsira o NUMERO do Pedido:\n");
					fflush(stdin);
					scanf("%d", &aux.numero);
				}
			}	
		}
	}
	
	clrscr();
	fclose(ptrpedido);
	fclose(ptrpizza);
	fclose(ptrcliente);
	fclose(ptrmotoqueiro);
}

void cadastrarPizza(void) {
	TpPizzas aux;
	FILE *ptrarquivo = fopen("Pizzas.dat", "ab");
	 
	printf("Insira o CODIGO da Pizza:\n");
	fflush(stdin);
	scanf("%d", &aux.codigo);
	
	while (aux.codigo > 0) {
		printf("Insira a DESCRICAO dessa Pizza:\n");
		fflush(stdin);
		gets(aux.descricao);
		printf("Insira o VALOR dessa Pizza:\n");
		scanf("%f", &aux.valor);
		
		fwrite(&aux, sizeof(TpPizzas), 1, ptrarquivo);
		
		printf("\nInsira o CODIGO da Pizza:\n");
		fflush(stdin);
		scanf("%d", &aux.codigo);
	}
	
	clrscr();
	fclose(ptrarquivo);
}


int validarCPF(char ncpf[15]) {
	int i = 0, j = 10;
	int dig1, dig2;
	int soma = 0, resto;
	
	//se algum caracter for < 0 ou > 9 //quebra o codigo 
	if(validarInt(ncpf) != 1){
		return 0;
	}
		
	while (i < 9){
		soma += (ncpf[i] - 48) * j;
		i++;
		j--;
	}
	
	resto = soma % 11;
	if (resto < 2) // se resto < 2 o priemiro digito tem que ser = 0
		dig1 = 0;
	else  // se nao o digito tem que ser igual ao resto -11
		dig1 = 11 - resto;
	
	if ((ncpf[9] - 48) != dig1) //cpf inv�lido
		return 0;
	else {
		i = 0, j = 11, soma = 0;
	
		while (i < 10) {
			soma += (ncpf[i] - 48) * j;
			i++;
			j--;
		}
	
		resto = soma % 11;
		if (resto < 2)
			dig2 = 0;
		else
			dig2 = 11 - resto;
		
	}
	
	if ((ncpf[10] - 48) != dig2)
		return 0;
	else
		return 1;
}

int validarInt(char str[11]){
	int i;
	int TL = strlen(str);
	
	for(i = 0 ; i < TL - 1 ; i++)
		if(str[i] < 48 || str[i] > 57)
			return 0;
	
	return 1;
}

//ainda tenho que fazer a valida��o do n� telefone
void cadastrarCliente(void) {
	TpCliente aux;
	FILE *ptrarquivo = fopen("Clientes.dat", "ab");
	 
	printf("Insira o TELEFONE do cliente que deseja cadastrar:\n");
	fflush(stdin);
	gets(aux.telefone);
	
	while (strcmp(aux.telefone, "\0") != 0) {
		printf("Insira o NOME desse cliente:\n");
		gets(aux.nome);
		printf("Insira o ENDERECO desse cliente:\n");
		gets(aux.endereco);
		printf("Insira a CIDADE desse cliente:\n");
		gets(aux.cidade);
		printf("Insira o CEP desse cliente:\n");
		gets(aux.cep);
		
		fwrite(&aux, sizeof(TpCliente), 1, ptrarquivo);
		
		printf("\nInsira o TELEFONE do cliente que deseja cadastrar:\n");
		gets(aux.telefone);
		fflush(stdin);
	}
	
	clrscr();
	fclose(ptrarquivo);
}

void cadastrarMotoqueiro(void) {
	TpMotoqueiro aux;
	int flag;
	
	FILE *ptrarquivo = fopen("Motoqueiros.dat", "ab");
	 
	printf("Insira o CPF do motoqueiro que deseja cadastrar:\n");
	fflush(stdin);
	gets(aux.cpf);
	flag = validarCPF(aux.cpf);
	
	//validacao do CPF
	while(flag != 1 && strlen(aux.cpf) > 0) {
		printf("CPF invalido, insira um CPF valido:\n");
		fflush(stdin);
		gets(aux.cpf);
		flag = validarCPF(aux.cpf);
	}
	
	while(strlen(aux.cpf) > 0) {
		printf("Insira o NOME desse motoqueiro:\n");
		fflush(stdin);
		gets(aux.nome);
		printf("Insira o ENDERECO desse motoqueiro:\n");
		gets(aux.endereco);
		printf("Insira o TELEFONE desse motoqueiro:\n");
		gets(aux.telefone);
		printf("Insira a DATA DE ADMISSAO desse motoqueiro: [dd mm aaaa]\n");
		scanf("%d %d %d", &aux.data.d, &aux.data.m, &aux.data.a);
		
		fwrite(&aux, sizeof(TpMotoqueiro), 1, ptrarquivo);
		
		printf("\nInsira o CPF do motoqueiro que deseja cadastrar:\n");
		fflush(stdin);
		gets(aux.cpf);
		flag = validarCPF(aux.cpf);

		//validacao do CPF
		while (flag != 1 && strlen(aux.cpf) > 0) {
			printf("CPF invalido, insira um CPF valido:\n");
			fflush(stdin);
			gets(aux.cpf);
			flag = validarCPF(aux.cpf);
		}
	}
	
	clrscr();
	fclose(ptrarquivo);
}

void exibirCliente(void) {
	TpCliente aux;
	FILE *ptrarquivo = fopen("Clientes.dat", "rb");
	
	if (ptrarquivo == NULL)
		printf("ERRO de abertura\n");
	else{
		fread (&aux, sizeof(TpCliente), 1, ptrarquivo);

		while (!feof(ptrarquivo)) {
			printf("TELEFONE: %s\n", aux.telefone);
			printf("NOME: %s\n", aux.nome);
			printf("ENDERECO: %s\n", aux.endereco);
			printf("CIDADE: %s\n", aux.cidade);
			printf("CEP: %s\n", aux.cep);
			fread(&aux, sizeof(TpCliente), 1, ptrarquivo);
		}
		
		fclose(ptrarquivo);
	}
	
	getch();
	clrscr();
}

void exibirMotoqueiro(void) {
	TpMotoqueiro aux;
	FILE *ptrarquivo = fopen("Motoqueiros.dat", "rb");
	
	if (ptrarquivo == NULL)
		printf("ERRO de abertura\n");
	else {
		fread(&aux, sizeof(TpMotoqueiro), 1, ptrarquivo);
		while (!feof(ptrarquivo)){
			printf("CPF: %s\n", aux.cpf);
			printf("NOME: %s\n", aux.nome);
			printf("ENDERECO: %s\n", aux.endereco);
			printf("TELEFONE: %s\n", aux.telefone);
			printf("DATA DE ADMISSAO: %d/%d/%d\n", aux.data.d, aux.data.m, aux.data.a);
			fread(&aux, sizeof(TpMotoqueiro), 1, ptrarquivo);
		}
		
		fclose(ptrarquivo);
	}
	
	getch();
	clrscr();
}

void exibirPizza(void) {
	TpPizzas aux;
	FILE *ptrarquivo = fopen("Pizzas.dat", "rb");
	
	if (ptrarquivo == NULL)
		printf("ERRO de abertura\n");
	else{
		fread (&aux, sizeof(TpPizzas), 1, ptrarquivo);

		while (!feof(ptrarquivo)) {
			printf("CODIGO: %d\n", aux.codigo);
			printf("DESCRICAO: %s\n", aux.descricao);
			printf("VALOR: %.2f\n", aux.valor);
			fread(&aux, sizeof(TpPizzas), 1, ptrarquivo);
		}
		
		fclose(ptrarquivo);
	}
	
	getch();
	clrscr();
}

void exibirPedidos(void) {
	TpPedidos aux;
	FILE *ptrarquivo = fopen("Pedidos.dat", "rb");
	
	if (ptrarquivo == NULL)
		printf("ERRO de abertura\n");
	else{
		fread (&aux, sizeof(TpPedidos), 1, ptrarquivo);

		while (!feof(ptrarquivo)) {
			printf("NUMERO: %d\n", aux.numero);
			printf("TELEFONE: %s\n", aux.telefone);
			printf("CODIGO: %d\n", aux.codigo);
			printf("CPF: %s\n", aux.cpf);
			printf("SITUACAO: %s\n", aux.situacao);
			printf("DATA DO PEDIDO: %d/%d/%d\n", aux.dataPedido.d, aux.dataPedido.m, aux.dataPedido.a);
			fread(&aux, sizeof(TpPedidos), 1, ptrarquivo);
		}
		
		fclose(ptrarquivo);
	}
	
	getch();
	clrscr();
}

char menu(void) {
	printf("# # # # MENU # # # # \n");
	textcolor(1);
	printf("[A] Cadastrar CLIENTES\n");
	printf("[B] Cadastrar MOTOQUEIROS\n");
	printf("[C] Cadastrar PIZZAS\n");
	printf("[D] Cadastrar PEDIDOS\n");
	textcolor(2);
	printf("[E] Exibir CLIENTES\n");
	printf("[F] Exibir MOTOQUEIROS\n");
	printf("[G] Exibir PIZZAS\n");
	printf("[H] Exibir PEDIDOS\n");
	textcolor(3);
	printf("[I] Alterar CLIENTES\n");
	printf("[J] Alterar MOTOQUEIROS\n");
	printf("[K] Alterar PIZZAS\n");
	printf("[L] Alterar PEDIDOS\n");
	textcolor(5);
	printf("[M] Excluir CLIENTES\n");
	printf("[N] Excluir MOTOQUEIROS\n");
	printf("[O] Excluir PIZZAS\n");
	printf("[P] Excluir PEDIDOS\n");
	textcolor(7);
	
	return toupper(getche());
}
