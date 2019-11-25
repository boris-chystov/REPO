# Lab Work 2 | Teoria Algorítmica da Informação

Professor: Dr Armando Pinho

Alunos: Borys Chystov, Dante Marinho e Francisco Santos

## Instruções para rodar o programa do Lab Work 2 em Linux
### Requerimentos
- g++-9
- std versão 17

### Procedimentos para compilação
Dentro da pasta raiz do programa, onde contém o ficheiro CmakeList, executar o comando:

g++-9 -pthread main2.cpp -std=c++17 -o main2

Para executar a classificação, é preciso executar o seguinte comando:

g++-9 wavfind.cpp -std=c++17 -o wavfind

Para calcular o SNR(signal-to-noise-ratio) e o máximo erro absoluto "per sample" a partir de dois ficheiros introduzidos, é preciso executar o seguinte comando:

g++-9 wavcmp.cpp -std=c++17 -o wavcmp


### Procedimentos para rodar o programa

Este programa não tem parâmetros, portanto, basta iniciar o executável:

./main2

ou

./wavfind

ou

./wavcmp

### Comandos úteis (caso necessário)
- Dar permissão de execucao ao arquivo "programa":

sudo chmod 744 programa
