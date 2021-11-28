## Projeto 1.  
Resolvendo problemas clássicos de concorrência e sincronização usando Semáforos e Monitores

## Regras do projeto.

- Existem alunos de So e de Computação
- Existem apenas dez lugares abertos para os alunos na sala
- Os alunos de computação poderão entrar na sala somente depois de cinco alunos de SO entrarem na sala
- Os lugares serão preenchidos por ordem de chegada
- Os alunos de SO assinam uma lista de entrada após entrarem na sala
- As apresentações serão feitas por ordem de chegada
- As apresentações irão acontecer somente se a sala estiver cheia
- Serão 4 rodadas de apresentações
- O professor irá dar uma nota entre 0 e 10 para o aluno após sua apresentação
- após receber a nota o aluno assina a lista de saida
- Os alunos de computação poderão sair em qualquer apresentação
- os alunos de computação irão testar a cada segundo se irão sair ou não em cima de um número randômico de 0 à 10. Caso o número for menor que 2, o aluno irá sair da sala

## Implementação
Esse trabalho foi implementado na linguagem C.

## como rodar

### rodar implementação com monitor
~~~
cd ex_monitor
make
./main
~~~

### rodar implementação com semaforo
~~~
cd ex_semaforo
make
./main
~~~

### Póssiveis problemas
Ao compilar, exibirá alguns warnings em cima dos métodos escritos nos arquivos ".h". É possivel rodar os códigos normalmente sem nenhum problema.