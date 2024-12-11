# Sistema de Empréstimo de Salas

## Descrição

Este projeto é um sistema de empréstimo de salas desenvolvido em C utilizando a biblioteca GTK para a interface gráfica. O sistema permite que usuários façam login, cadastrem novos usuários e reservem salas para datas específicas.

## Estrutura do Projeto

- `src/`: Contém os arquivos fonte do projeto.
  - `csv_operations.c` e `csv_operations.h`: Funções para operações com arquivos CSV.
  - `ui_functions.c` e `ui_functions.h`: Funções para manipulação da interface gráfica.
  - `main.c`: Arquivo principal que inicializa a aplicação GTK.
  - `interface.glade`: Arquivo de interface gráfica criado com Glade.

- `data/`: Contém arquivos CSV com dados de login e reservas.

- `README.md`: Este arquivo.

## Dependências

- GTK+ 3.20 ou superior
- Compilador GCC

## Como Compilar

1. Certifique-se de ter o GTK+ instalado no seu sistema.
2. Navegue até o diretório do projeto.
3. Compile o projeto usando o GCC:

```sh
cd src && gcc -o Sistema-Emprestimo-de-Salas *.c `pkg-config --cflags --libs gtk+-3.0`
```
### Executando

```sh
./Sistema-Emprestimo-de-Salas
```
