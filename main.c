#include <stdio.h>
#include <stdlib.h>

#define TAM_CACHE 6

// protótipos
void implementa_LRU(char cache[], char seq[], int tamSeq);
void implementa_LFU(char cache[], char seq[], int tamSeq);

// variáveis globais
int contMiss;
int contHit;

int main() {
  char cache[TAM_CACHE];

  // SEQUÊNCIAS DE ACESSO

  // sequencia de acessos com alta localidade temporal ("duas fases") (favorece
  // LRU)
  char seq1[] = {'A', 'B', 'C', 'D', 'E', 'F', 'A', 'C', 'B', 'F', 'D',
                 'E', 'B', 'A', 'F', 'E', 'C', 'D', 'A', 'F', 'B', 'E',
                 'G', 'H', 'I', 'J', 'K', 'L', 'G', 'I', 'H', 'L', 'J',
                 'K', 'H', 'G', 'L', 'K', 'I', 'J', 'G', 'L', 'H', 'K'};
  int tamSeq1 = sizeof(seq1);

  // sequencia com acessos repetitivos a um subconjunto dos dados (favorece LFU)
  char seq2[] = {'A', 'B', 'C', 'D', 'E', 'F', 'A', 'B', 'C', 'D', 'A',
                 'B', 'C', 'D', 'A', 'B', 'C', 'D', 'A', 'C', 'B', 'D',
                 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'A',
                 'B', 'C', 'D', 'A', 'B', 'C', 'D', 'A', 'B'};
  int tamSeq2 = sizeof(seq2);

  // caso híbrido
  char seq3[] = {'a'};
  int tamSeq3 = sizeof(seq3);

  printf("Sequência de acessos 1:\n");
  implementa_LRU(cache, seq1, tamSeq1);
  implementa_LFU(cache, seq1, tamSeq1);
  printf("\n");

  printf("Sequência de acessos 2:\n");
  implementa_LRU(cache, seq2, tamSeq2);
  implementa_LFU(cache, seq2, tamSeq2);
  printf("\n");

  /*printf("Sequência de acessos 3:");
  implementa_LRU(cache, seq3, tamSeq3);
  implementa_LFU(cache, seq3, tamSeq3);
  printf("\n");*/

  return 0;
}

void implementa_LRU(char cache[], char seq[], int tamSeq) {
  contHit = 0;
  contMiss = 0;

  char lruPilha[TAM_CACHE]; // inicializa a pilha e cache
  for (int i = 0; i < TAM_CACHE; i++) {
    lruPilha[i] = 0;
    cache[i] = 0;
  }

  for (int i = 0; i < tamSeq; i++) {

    int hitFlag = 0;

    for (int j = 0; j < TAM_CACHE;
         j++) { // percorre a cache pra ver se o elemento já está lá

      if (seq[i] == cache[j]) { // caso de HIT
        contHit += 1;
        hitFlag = 1;

        int posicPilha = -1;
        for (int k = 0; k < TAM_CACHE; k++) { // procura o elemento na pilha
          if (lruPilha[k] == seq[i]) {
            posicPilha = k;
            break;
          }
        }
        for (int k = posicPilha; k > 0;
             k--) { // desloca os elementos da pilha que estão acima do elemento
                    // encontrado pra baixo
          lruPilha[k] = lruPilha[k - 1];
        }

        lruPilha[0] = seq[i]; // coloca o elemento encontrado no topo
        break;
      }
    }
    if (hitFlag == 0) { // caso de MISS (não teve hit)
      contMiss += 1;
      int vazioFlag = 0;

      for (int j = 0; j < TAM_CACHE; j++) { // se tiver espaço vazio na cache
        if (cache[j] == 0) {
          vazioFlag = 1;
          cache[j] = seq[i];
          break;
        }
      }

      if (vazioFlag ==
          0) { // se não tinha espaço vazio na cache -> subst. com LRU

        for (int k = 0; k < TAM_CACHE;
             k++) { // percorre a cache até achar o elemento no fundo da pilha
          if (cache[k] == lruPilha[TAM_CACHE - 1]) {
            cache[k] = seq[i];
            break;
          }
        }
      }

      for (int k = TAM_CACHE - 1; k > 0; k--) {
        lruPilha[k] = lruPilha[k - 1];
      }
      lruPilha[0] = seq[i];
    }
    hitFlag = 0;
  }
  printf("LRU: %d hits e %d misses\n", contHit, contMiss);
}

void implementa_LFU(char cache[], char seq[], int tamSeq) {
  // ZERA os hits e os misses
  contMiss = 0;
  contHit = 0;
  // variáveis de controle
  int flag_hit, flag_vazia;
  int indice_menor;

  // 65 = 'A' em ASCII 0
  int cont[26];

  // ZERA todas as posições do vetor de contadores
  for (int i = 0; i < 26; i++) {
    cont[i] = 0;
  }

  // ZERA todas as posições do CACHE
  for (int i = 0; i < TAM_CACHE; i++) {
    cache[i] = 0;
  }

  // LOOP principal de leitura
  for (int i = 0; i < tamSeq; i++) {
    flag_hit = 0;
    flag_vazia = 0;
    indice_menor = 0;

    // INCREMENTA a contagem no vetor de contagem
    cont[seq[i] - 65]++; //(65 = 'A' em ASCII)

    // LOOP de verificação da cache (tentativa de hit)
    for (int j = 0; j < TAM_CACHE; j++) {
      if (seq[i] == cache[j]) {
        contHit++;
        flag_hit = 1;
        break;
      }
    }

    if (flag_hit == 0) {
      // LOOP para inserção de dado (caso haja uma posição vazia na cache)
      for (int i = 0; i < TAM_CACHE; i++) {
        if (cache[i] == 0) {
          cache[i] = seq[i];
          contMiss++;
          flag_vazia = 1;
          break;
        }
      }

      if (flag_vazia == 0) {
        // LOOP de remoção de um dado da cache (para inserção de um novo)
        indice_menor = 0;
        for (int j = 1; j < TAM_CACHE; j++) {
          if (cont[cache[j] - 65] < cont[cache[indice_menor] - 65]) {
            indice_menor = j;
          }
        }
        cache[indice_menor] = seq[i]; // insere o novo dado na cache
        contMiss++;
      }
    }
  }
  printf("LFU: %d hits e %d misses\n", contHit, contMiss);
}