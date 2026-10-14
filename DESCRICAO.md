# Descrição do sistema

Escreva este arquivo sem IA, antes de qualquer código. Apague as instruções
em itálico quando preencher.

## Nome e ideia

Vou fazer a ideia do Inventario de RPG, vai servir para jogadores de RPG de mesa e tera um diferencial de contar vida, mana e habilidades.

## O que o sistema guarda

Listas (somente o mestre pode modificar):
    listaH: guarda todas as habilidades possiveis (as habilidades tem nome, area_total_afetada, area_afetada, efeito_causado, descrição, custo).
    listaI: guarda todos os itens possiveis (os itens tem nome, preço, quantidade, descrição).

personagem:
    localização_batalha: matriz_localização, passos;
    itens: nome, preço, quantidade, descrição, efeito_causado;
    vida: quatidade_V, quantidadeMAX_V ;
    mana: quantidade_M, quantidadeMAX_M ; 

    habilidades: nome, area_total_afetada, area_afetada, efeito_causado, descrição, custo (area_afetada = matriz mostrando a area no mapa que sera afetada, se for uma matriz 3x3 que vai ser afetada então fica 111111111 se for só uma linha vertical de 3x1 então fica 010010010 e uma reta vertical é a mesma coisa que uma reta horizontal, ja que o personagem pode se virar para lançar o poder, então se o inimigo estiver dentro da area de 3x3 então será acertado pelo ataque de 3x1 a pergunta que fica é se os outros personagems vão estar no meio do caminho do 3x1, se isso acontecer então esses tambem vao receber o dano, caso o efeito_causado for um ataque em area. Pensando nisso vou adicionar a area total afetada que será o 3x3 total e a area_afetada será só a area onde o ataque realmente afeta 3x1. Ao inves de contar como binario que em mapas muito grandes daria numeros gigantes, eu vou contar como posições em um xadrez d9, e5 e etc, caso o mapa seja maior que z26 então começa a usar combinações de 2 letras por exempli aa1, ab1, ac1 e etc)(efeito_causado = nome do efeito + valor, exemplo "curar 40");

    dinheiro: quantidade_dinheiro, extrato;

## O que dá para fazer

(a listaI por padrão deve ter 3 armas branca, 1 arma de longo alcance, 2 tipos de set de armadura, maçã e poções de mana e vida)
(a listaH por padrão deve ter bola de fogo e gelo, laminas de vento, envenenamento em area, defesa magica, impulso)
ações listas:
    Mestre_add_H: entra nome, area_total_afetada, area_afetada, efeito_causado, descrição, custo => adiciona habilidade na lista de habilidades e modifica nome para "#x nome" sendo x a posição da habilidade dentro da lista.
    Mestre_remover_H: entra nome => pega o #x procura na lista e remove a habilidade na posição x.
    Mestre_add_I: mesma coisa que o add_H soque pra lista de itens.
    Mestre_remover_I: mesma coisa que o remover_H soque pra lista de itens.

ações personagem:
    criar_novo_personagem: entra nome_personagem, valor_vidaMAX, valor_manaMAX, valor_dinheiro => personagem criado;
    morrer: entra nome_personagem => personagem x morreu && rola d20, se valor <= 10 então deletar ficha x, se não então personagem volta com 1 de vida;
    mover: entra cordenada => if(personagem(passos) <= distancia até cordenada) então matriz_localização modificada para cordenada;
    status_personagem: entra nome => mostra todos os status do personagem em uma interface de texto (para mostrar a aba de habilidades ele mostra nome_habilidade, custo, efeito_causado, area_afetada sendo que a area afetada ele mostra como matriz, entao ele diz 4x4 afetada ou 3x1 afetada, caso o player queira mais informação ele deve usar o comando "status_habilidade");

ações item: 
    adicionar: entra nome => item(nome) in listaI adiciona na ficha (todo nome tem um id, ex: "#27 maçã" então então vai procurar listaI[27-1]) && gastar(itens(preço));
    usar: entra nome => -1 quantidade && print(descrição) && aplicar efeito_causado;
    remendar: entra nome_item, valor_pago => quantidade_item = rount(valor_pago / 2);
    lixo: entra nome_item => deleta item selecionado;

ações vida:
    curar: entra quantidadeMAX_V, quantidade_V, valor_int => (quantidade_V + valor_int) < quantidadeMAX_V ;
    dano: entra quantidade_V, valor_int => (quantidade_V - valor_int) > 0 && if((quantidade_V - dano) <= 0): personagem morrer;
    veneno: entra quantidade_V, turnos_afetados, valor_int => a cada (turnos_afetados) turno o personagem leva (valor_int) de dano na (quantidade_V);
    regeneração: entra quantidade_V, turnos_afetados, valor_int => a cada (turnos_afetados) turno o personagem leva (valor_int) de cura na (quantidade_V);
    aumentarMAX: entra quantidadeMAX_V, valor_int => quantidadeMAX_V = (quantidadeMAX_V + valor_int);
    diminuirMAX: entra quantidadeMAX_V, valor_int => quantidadeMAX_V = (quantidadeMAX_V - valor_int);

ações mana:
    recuperar: curar(quantidadeMAX_M, quantidade_M, valor_int);
    gastarMANA: dano(quantidade_M, valor_int);
    gastar_turnos: veneno(quantidade_M, turnos_afetados, valor_int);
    recuperar_turnos: regeneração(quantidade_M, turnos_afetados, valor_int);
    aumentarMAX: entra quantidadeMAX_M, valor_int => quantidadeMAX_M = (quantidadeMAX_M + valor_int);
    diminuirMAX: entra quantidadeMAX_M, valor_int => quantidadeMAX_M = (quantidadeMAX_M - valor_int);

ações habilidade:
    adicionar: entra nome => habilidade(nome) in listaH adiciona na ficha;
    remover: entra nome => deleta habilidade;
    evoluir: entra nome, valor_area, str_efeito => area_afetada = scanf(valor_area), efeito_causado = scanf(str_efeito), custo = custo * 1.5
    usar: entra nome_habilidade, nome_personagem => if(personagem(localização(matriz_localização)) in habilidade(area_total_afetada)) então aplicar habilidade(efeito_causado) no personagem && gastarMANA(quantidade_M, habilidade(custo));
    parar: entra nome => if(habilidade(nome) == ativada) então desativa;
    status_habilidade: entra nome => printf(nome, area_total_afetada, area_afetada, efeito_causado, descrição, custo)

ações dinheiro:
    gastar: entra valor_int => {quantidade_dinheiro = quantidade_dinheiro - valor_int || if(quantidade_dinheiro - valor_int < 0) então quantidade_dinheiro = 0} && adiciona str no extrato gastou x; 
    receber: entra valor_int => quantidade_dinheiro = quantidade_dinheiro + valor_int && adiciona str no extrato recebeu x;
    ///status_: entra null => printf(quantidade_dinheiro, extrato);

## Regras

não tem nenhum sistema para personagens que voam, as ações de evoluir habilidade e adicionar habilidade devem ser feitas com o auxilio do mestre do RPG, para que o player nao tenha uma habilidade com dano infinito, o dono de um personagem nao pode alterar os status dos outros personagens ou npc, somente o mestre pode interagir com todas as fichas de todos os personagens, os personagens nao morrem de falta de mana, se nao tiver mana o suficiente a habilidade nao pode ser ativada. Não pode haver 2 itens ou habilidades ou personagem com o mesmo nome. Se o item diver durabilidade 0 ele deve ser deletado automaticamente.

## Quando algo dá errado

deve mostrar um texto explicando o erro, por exemplo: habilidade x não pertence ao personagem y portanto nao pode ser usada.

## Classes que eu imagino

acabei escrevendo isso na parte do "oque da para fazer?"
as classes são:
personagem
item
vida
mana
habilidade
dinheiro

## Exemplo de uso

=== MENU PRINCIPAL: SISTEMA DE RPG ===
1. Painel do Mestre
2. Gerenciar Personagem
3. Inventário e Loja
4. Combate e Habilidades
0. Sair do Jogo
Escolha uma opção: 2

--- GERENCIAR PERSONAGEM ---
1. Criar novo personagem
2. Ver status de um personagem
3. Voltar
Escolha uma opção: 1

Digite o nome: Cleber
Vida MAX: 100
Mana MAX: 50
Dinheiro Inicial: 200
[!] Personagem 'Cleber' criado com sucesso!

(Voltando ao menu... O usuário adiciona os itens e habilidades...)
[!] Itens 'Maçã', 'Espada' e 'Arco' adicionados ao inventário de Cleber.
[!] Habilidades 'Bola de fogo', 'Impulso' e 'Defesa mágica' aprendidas!

--- GERENCIAR PERSONAGEM ---
1. Criar novo personagem
2. Ver status de um personagem
3. Voltar
Escolha uma opção: 2
Digite o nome do personagem: Cleber

=== STATUS DA FICHA ===
Nome: Cleber
Vida (HP): 100/100  |  Mana (MP): 50/50  |  Dinheiro: $200
Localização Atual: a1

[ ITENS NO INVENTÁRIO ]
- 1x Maçã
- 1x Espada
- 1x Arco

[ HABILIDADES ]
- Bola de fogo (Custo: 15 MP | Área Afetada: 3x1)
- Impulso (Custo: 5 MP)
- Defesa Mágica (Custo: 20 MP)
=======================
Pressione ENTER para voltar ao menu...
