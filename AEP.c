#include <stdio.h>
#include <string.h>
#include <locale.h>

char criptografar(char c) {
    if (c >= '0' && c <= '9') {
        char mapeamento[] = {'@', '#', '$', '%', '&', '*', '!', '+', '=', '?'};
        return mapeamento[c - '0']; 
    } else if (c >= 'a' && c <= 'z') {
        return (c - 'a' + 1) + '0';
    } else if (c >= 'A' && c <= 'Z') {
        return (c - 'A' + 1) + '0';
    }
    return c;
}

char descriptografar(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0' + 'a' - 1;  
    } else if (c == '@') return '0';
    else if (c == '#') return '1';
    else if (c == '$') return '2';
    else if (c == '%') return '3';
    else if (c == '&') return '4';
    else if (c == '*') return '5';
    else if (c == '!') return '6';
    else if (c == '+') return '7';
    else if (c == '=') return '8';
    else if (c == '?') return '9';
    return c;
}

void criptografarSenha(char* senha) {
    int i;
    for (i = 0; senha[i] != '\0'; i++) {
        senha[i] = criptografar(senha[i]);
    }
}

void descriptografarSenha(char* senha) {
    int i;
    for (i = 0; senha[i] != '\0'; i++) {
        senha[i] = descriptografar(senha[i]);
    }
}

int validarUsuarioSenha(FILE *fUser, char* usuario, char* senha) {
    int result = 0;

    
    if (strstr(senha, usuario) != NULL) {
        result = 1; 
    } else {
        char user[100], pass[100];
        rewind(fUser); 
        while (fscanf(fUser, "%s,%s\n", user, pass) != EOF) {
            if (strcmp(user, usuario) == 0) {
                result = 2; 
                break;
            }
        }
    }

    return result;
}

void registro(FILE *fUser) {
    int numUsuarios;
    printf("Digite o numero de usuarios que deseja cadastrar: ");
    scanf("%d", &numUsuarios);

    while (numUsuarios > 0) {
        char user[100], password[100], password2[100];
        printf("-------REGISTRAR-------\n");
        printf("Usuario: ");
        scanf("%s", user);
        printf("Senha: ");
        scanf("%s", password);
        criptografarSenha(password);

        int resultado = validarUsuarioSenha(fUser, user, password);
        if (resultado == 1) {
            printf("A senha deve ser diferente do nome de usuario.\n");
        } else if (resultado == 2) {
            printf("Usuário já existe, tente outro nome.\n");
        } else {
            printf("Confirmar senha: ");
            scanf("%s", password2);
            criptografarSenha(password2);
            printf("-----------------------\n");

            if (strcmp(password, password2) == 0) {
                fprintf(fUser, "%s,%s\n", user, password);
                printf("Usuario cadastrado com sucesso\n");
                numUsuarios--;
            } else {
                printf("As senhas não conferem. Tente novamente.\n");
            }
        }
    }
    printf("Finalizado\n");
}

void removerUsuario() {
    FILE *fUser, *fTemp;
    fUser = fopen("user.txt", "r");
    if (fUser == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fTemp = fopen("temp.txt", "w");
    if (fTemp == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        fclose(fUser);
        return;
    }

    char usuario[100], senha[100], usuarioExcluir[100];
    int encontrado = 0;

    printf("\nDigite o nome do usuario a ser excluido: ");
    scanf("%s", usuarioExcluir);

    while (fscanf(fUser, "%99[^,],%99[^\n]\n", usuario, senha) != EOF) {
        if (strcmp(usuario, usuarioExcluir) != 0) {
            fprintf(fTemp, "%s,%s\n", usuario, senha); 
        } else {
            encontrado = 1; 
        }
    }

    fclose(fUser);
    fclose(fTemp);

    if (encontrado) {
        remove("user.txt");  
        rename("temp.txt", "user.txt");  
        printf("Usuário excluído com sucesso.\n");
    } else {
        printf("Usuário não encontrado.\n");
        remove("temp.txt");  
    }
}

void altUsuario() {
    FILE *fUser, *fTemp;
    fUser = fopen("user.txt", "r");
    if (fUser == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fTemp = fopen("temp.txt", "w");
    if (fTemp == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        fclose(fUser);
        return;
    }

    char usuario[100], senha[100], usuarioAlterado[100], novaSenha[100];
    int encontrado = 0;

    printf("\nDigite o usuario a ser alterado: ");
    scanf("%s", usuarioAlterado);

    while (fscanf(fUser, "%99[^,],%99[^\n]\n", usuario, senha) != EOF) {
        if (strcmp(usuario, usuarioAlterado) == 0) {
            encontrado = 1;
            printf("Usuário encontrado. Digite a nova senha: ");
            scanf("%s", novaSenha);
            criptografarSenha(novaSenha); 
            fprintf(fTemp, "%s,%s\n", usuario, novaSenha);
        } else {
            fprintf(fTemp, "%s,%s\n", usuario, senha);
        }
    }

    fclose(fUser);
    fclose(fTemp);

    
    if (encontrado) {
        remove("user.txt"); 
        rename("temp.txt", "user.txt");  
        printf("Usuário alterado com sucesso.\n");
    } else {
        printf("Usuário não encontrado.\n");
        remove("temp.txt"); 
    }
}

void listUsuario() {
    FILE *fUser;
    fUser = fopen("user.txt", "r");
    if (fUser == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char user[100], password[100];
    while (fscanf(fUser, "%99[^,],%99[^\n]\n", user, password) != EOF){
        descriptografarSenha(password);
        printf("Usuario: %s, Senha: %s\n", user, password);
    }
    fclose(fUser);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    FILE *fUser;
    fUser = fopen("user.txt", "a+");
    if (fUser == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char op;
    printf("Bem Vindo!\n");
    printf("Digite qual operação deseja fazer:\n 1-Registrar Usuario\n 2-Remover Usuario\n 3-Alterar Usuario\n 4-Listar Usuarios\n 5-Sair\n");
    while (getchar() != '\n'); 

    do {
        printf("Digite sua escolha: ");
        scanf("%c", &op);
        while (getchar() != '\n'); 

        switch (op) {
            case '1':
                registro(fUser);
                break;
            case '2':
                removerUsuario();
                break;
            case '3': 
                altUsuario();
                break;
            case '4':
                listUsuario();
                break;
            case '5':
                printf("Fim do programa\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (op != '5');

    fclose(fUser);
    return 0;
}
