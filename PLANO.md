# Plano de implementação — Inventário de RPG

Plano baseado em `DESCRICAO.md`. Cada etapa termina com algo que compila e que
dá para testar. Uma classe por par de arquivos, C++11 e só a biblioteca
padrão, objetos guardados direto nos containers (sem `new`/`delete`), e as
classes de lógica sem `cin`/`cout`.

## 1. Escopo

O que o sistema faz, segundo a descrição:

- O mestre monta o mundo (listaH, listaI, listaE) no setup de um save vazio e
  cria 1 personagem; depois o mundo trava.
- O jogador cuida da ficha: vida, mana, dinheiro, itens, habilidades e
  efeitos.
- Até 3 saves por execução, com troca entre eles, sem arquivo (some ao
  fechar).
- Sem mapa, sem turnos e sem localização: a área afetada só descreve a
  habilidade.

## 2. Arquitetura: lógica separada da interface

- `include/` e `src/`: as classes do sistema. Recebem dados por parâmetro e
  devolvem `bool`, números ou listas. Nada de `cin`/`cout`.
- `include/Interface.hpp` e `src/Interface.cpp`: os menus. Só aqui tem
  `cin`/`cout`.
- `src/main.cpp`: cria a interface e inicia o programa.
- `exemplos/`: arquivos de entrada para rodar sempre.

## 3. Classes

| Classe | O que guarda | Principais métodos |
| --- | --- | --- |
| `Vida` | atual, máximo | `curar`, `dano` (avisa se zerou), `aumentarMax`, `diminuirMax` |
| `Mana` | atual, máximo | `recuperar`, `gastar`, `temMana(int)`, `aumentarMax`, `diminuirMax` |
| `Dinheiro` | quantidade, extrato (textos) | `gastar` (falha se faltar), `receber`, `getExtrato` |
| `Habilidade` | id, nome, areaAfetada, efeitoCausado, descricao, custo | `getEtiqueta` ("#x nome"), `operator<`, `operator==` |
| `Item` | id, nome, preco, durabAtual, durabMax, descricao, efeitoCausado, remendavel | `getEtiqueta`, `usarDurabilidade`, `operator<`, `operator==` |
| `Efeito` | nome | `getNome`, `operator<` |
| `Catalogo` | listaH, listaI, listaE, `finalizado` | `addH`/`addI`/`addE`, `removerH`/`removerI`/`removerE`, `buscarH`/`buscarI`, `finalizar` |
| `Personagem` | nome, vivo/morto, `Vida`, `Mana`, `Dinheiro`, itens, habilidades, efeitos | `comprar`, `vender`, `usarItem`, `remendar`, `jogarNoLixo`, `aprender`, `usarHabilidade(nome, valor)`, `aplicarEfeito`, `limparEfeitos`, `morrer`, `reviver` |
| `Save` | `configurado`, um `Catalogo`, um `Personagem` | `foiConfigurado`, `criarPersonagem` |
| `Saves` | 3 `Save` e o índice ativo | `getAtivo`, `trocar`, `excluir`, `slotVazio` |

Observações:

- `Habilidade` e `Item` guardam o id separado do nome; a etiqueta `"#x nome"`
  é montada só na hora de mostrar. Assim comparar nomes "ignorando o #x " é
  só comparar `nome`.
- A "tumba" é o id fora do `map`; o próximo `add` usa o menor id livre, como
  a descrição pede.
- `Save` e `Saves` existem só para organizar os 3 espaços. Se você achar que
  ficou classe demais, dá para juntar as duas em uma só.

## 4. Containers da STL e por quê

| Onde | Container | Por quê |
| --- | --- | --- |
| `Catalogo.listaH` | `std::map<int, Habilidade>` | a ação procura pelo id `#x`; a chave é o id e a "tumba" é o id que não existe |
| `Catalogo.listaI` | `std::map<int, Item>` | mesmo motivo do listaH |
| `Catalogo.listaE` | `std::set<Efeito>` | os efeitos do catálogo são únicos e já ficam em ordem |
| `Personagem.itens` | `std::vector<Item>` | guarda a ordem de compra e permite dois itens com o mesmo nome (2 maçãs) |
| `Personagem.habilidades` | `std::vector<Habilidade>` | são cópias; dá para listar em ordem com `sort` |
| `Personagem.efeitos` | `std::list<Efeito>` | um efeito pode repetir e a remoção no meio ("limpar") fica barata |
| `Dinheiro.extrato` | `std::vector<std::string>` | só acrescenta no fim e depois mostra tudo |
| `Saves` | `std::vector<Save>` | 3 posições fixas, acessadas por índice |

Não precisei de `unordered_map`: o `map` já resolve a busca por id e o
volume de dados é pequeno. Se você quiser ver a diferença dos dois, a gente
comenta antes de trocar.

## 5. Requisitos de STL do enunciado, onde entram

- **`map`**: `listaH` e `listaI`, no `Catalogo`.
- **`set` ou `list`**: `listaE` é `set` e `Personagem.efeitos` é `list`.
- **`sort` ou `find` com `operator<`/`operator==` da classe**: `Item` e
  `Habilidade` definem os dois operadores. Vou usar `std::sort` para mostrar
  catálogo e bolsa em ordem de nome e `std::find` para achar item/habilidade
  da ficha pelo nome.

## 6. Arquivos do projeto

```text
include/        Vida.hpp Mana.hpp Dinheiro.hpp Habilidade.hpp Item.hpp
                Efeito.hpp Catalogo.hpp Personagem.hpp Save.hpp Saves.hpp
                Interface.hpp
src/            Vida.cpp Mana.cpp Dinheiro.cpp Habilidade.cpp Item.cpp
                Efeito.cpp Catalogo.cpp Personagem.cpp Save.cpp Saves.cpp
                Interface.cpp main.cpp
exemplos/       cenario1.txt ...
```

O comando do `README.md` (`g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o
sistema`) já cobre tudo; só mudo se algum arquivo precisar entrar.

## 7. Etapas

### Etapa 1 — Números e esqueleto
- Cria `Vida`, `Mana`, `Dinheiro` e um `main` temporário.
- Testar: curar trava no máximo; `dano` avisa quando zera; `gastar` falha sem
  dinheiro; `receber` registra no extrato.
- Commit: "etapa 1: vida, mana e dinheiro".

### Etapa 2 — Item, Habilidade, Efeito
- Cria as três classes de dados, com `operator<`, `operator==` e `getEtiqueta`.
- Testar com `std::sort` (lista ordenada por nome) e `std::find` (acha pelo
  nome).
- Commit: "etapa 2: item, habilidade e efeito".

### Etapa 3 — Catalogo
- Cria `Catalogo` com os três containers, ids, tumba e reaproveitamento,
  buscas por id e por nome e `finalizar`.
- Testar: adicionar, remover o #1, adicionar de novo e ver que o #1 é
  reaproveitado; buscar por nome; listar em ordem.
- Commit: "etapa 3: catalogo do mestre".

### Etapa 4 — Personagem
- Cria `Personagem` com vida, mana, dinheiro, itens, habilidades e efeitos.
- Implementa comprar/vender, usar item (durabilidade, efeito e livro que
  ensina), remendar, lixo, aprender/modificar/remover habilidade, usar
  habilidade (desconta mana e aplica efeitos), morrer/reviver e limpar
  efeitos. Os efeitos `curar` e `dano` recebem o valor por parâmetro; quem
  pergunta o valor é a interface, não a lógica.
- Testar um roteiro completo sem menu, direto no `main`.
- Commit: "etapa 4: personagem e ações".

### Etapa 5 — Saves
- Cria `Save` e `Saves` com 3 espaços, `configurado`, troca e exclusão (com a
  pergunta "vivo/morto").
- Testar a troca entre os 3 e a exclusão de um save.
- Commit: "etapa 5: os 3 saves".

### Etapa 6 — Interface
- Cria a `Interface` e o `main` final: tela de saves, painel do mestre, menu
  do jogador, e a leitura robusta (texto onde se espera número → erro e
  repete). Quando o item/habilidade tem efeito `curar` ou `dano`, a interface
  pergunta o valor antes de aplicar.
- Cria `exemplos/cenario1.txt` e roda.
- Commit: "etapa 6: interface em modo texto".

### Etapa 7 — Revisão
- Roda todos os exemplos, confere as mensagens de erro, ajusta o que faltar e
  atualiza o `README.md` (comando de compilação, se mudou).
- Commit: "etapa 7: exemplos e ajustes".

## 8. Decisões e pontos a confirmar

- Efeitos com valor (`curar` e `dano`): o valor é **digitado pelo jogador na
  hora** de usar. A interface pergunta e passa por parâmetro para a lógica;
  os outros efeitos são só nome.
- A bolsa guarda uma cópia do item com o id do catálogo só para mostrar a
  etiqueta; o preço usado é o preço pago. Se preferir sem id na bolsa, é
  fácil tirar.
