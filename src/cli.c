#include "extenso/cli.h"
#include "extenso/parser.h"
#include "extenso/util.h"

int interpretador (Context *ctx)
{
    ctx->EXP = ctx->expNum;
    char* resultado, op;
    puts ("\n\t\tANALISADOR DE EXPRESSOES NUMERICAS POR EXTENSO\n");
    CLRBUF;
    while(true)
    {
        clearScreen ();
        puts ("Selecione a entrada:\n a= Arquivo\n t= Teclado\n h= Ajuda\n e= Sair\n\nopcao = ");
        scanf ("%c%*c", &op);
        switch (op)
        {
            case 'a':
                clearScreen ();
                printf ("\tForam analisadas e resolvidas %d expressoes.\n\tOs resultado podem ser encontrados em %s\n", fileParsingInit (ctx), ARQ_SAIDA);
                printRes();
                CLRBUF;
                break;
            case 't':
                clearScreen ();
                puts ("Digite uma expressao numerica: ");
                scanf ("%[^\n]%*c", ctx->EXP);
                resultado = expParsingStart (ctx);
                printf ("\nResultado: %s\n", resultado);
                free (ctx->_TEXP);
                CLRBUF;
                break;
            case 'h':
                clearScreen ();
                handBook ();
                CLRBUF;
                break;
            case 'e': return 0;
            default:
                CLRBUF;
                puts ("Opcao invalida.\n");
        }
    }
}

void handBook (void)
{
    puts("A analise ocorre a partir de regras bem definidas, entao segue uma lista de comandos validos e instrucoes de uso:");
    puts("\n\nNUMEROS: \n");
    puts("A ordem e o plural importam. Nao utilize acentos nem pontuacao.");
    puts("dois milhoes = correto");
    puts("dois milhao = incorreto");
    puts("tres mil e quatro milhoes = incorreto");
    puts("quatro milhoes e tres mil = correto");
    puts("\n\nFORMATO DAS OPERACOES:\n");
    puts("Somar: numero mais numero");
    puts("Subtrair: numero menos numero");
    puts("Multiplicar: numero vezes numero");
    puts("Dividir: numero dividido por numero");
    puts("Resto: numero mod numero");
    puts("Exponencial: numero elevado a numero");
}

int fileParsingInit (Context *ctx)
{
    ctx->EXP = ctx->expNum; /* garante o buffer mesmo quando chamado fora do menu (--batch) */
    FILE* entrada = openFile (ARQ_ENTRADA, "r");
    FILE* saida = openFile (ARQ_SAIDA, "wt");
    int i = 0;
    char *expOut = NULL; /* Resultado da expressão analisada */
    Index temp = criaIndices (entrada, false);
    uint16_t* indices = temp.index;
    int count = temp.tam;
    while (count > 0)
    {
        fseek (entrada, indices[i++], SEEK_SET);
        fgets (ctx->EXP, MAX_GEN, entrada);
        char* endline = strpbrk(ctx->EXP, "\r\n");
        if(endline) *endline = '\0';
        expOut = expParsingStart(ctx);
        fputs (expOut, saida);
        fputc ('\n', saida);
        fflush (saida);
        free (ctx->_TEXP);
        count--;
    }
    free(indices);
    fclose (saida);
    fflush (stdout);
    fclose (entrada);
    return i;
}

void printRes(void)
{
	char ch = '\0';
	printf("\nDeseja visualizar todas as expressoes resolvidas? (S/N)\n");
	scanf("%c%*c", &ch);
	if(ch=='S' || ch=='s' || ch=='\n')
	{
		FILE* saida = openFile (ARQ_SAIDA, "rt");
		size_t s = maiorString(saida) + 1;
		char* handle = (char*) alloc (s, sizeof (char));
		printf("\n\tRESULTADOS (uma expressao por linha):\n\n");
		while(fgets(handle,s,saida))
			printf("%s",handle);
		fclose(saida);
        free(handle);
	}
}

size_t maiorString (FILE* stream)
{
	size_t k = 0;
	size_t maior = 1;
	char c = getc(stream);
	while(c!=EOF)
	{
		k++;
		if(c=='\n')
		{
			maior = (k>maior)?k:maior;
			k=0;
		}
		c=getc(stream);
	}
	rewind(stream);
	return maior;
}

void clearScreen (void)
{
    int n;
    for (n = 0; n < 10; n++)
      printf ("\n\n\n\n\n\n\n\n\n\n");
}
