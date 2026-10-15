# Descrição do sistema

## Nome e ideia

Vou fazer a ideia do Inventario de RPG, vai servir para jogadores de RPG de mesa e tera um diferencial de contar vida, mana e habilidades.

## O que o sistema guarda

Listas (somente o mestre pode modificar):
    listaH: guarda todas as habilidades possíveis. Cada habilidade tem nome,
    área afetada (ex.: 3x1), efeito_causado (um ou mais efeitos), descrição e
    custo (em mana).
    listaI: guarda todos os itens possíveis. Cada item tem nome, preço,
    durabilidade máxima, descrição, efeito_causado (pode ser vazio/NULL) e se
    é remendável (sim ou não). Comidas, poções e livros não são remendáveis;
    espadas e armaduras são.
    listaE: guarda todos os efeitos possíveis. Cada efeito tem só nome.

listas por padrão:
    listaI: 3 armas brancas, 1 arma de longo alcance, 2 sets de armadura,
    maçã, poção de vida, poção de mana e poção de reviver.
    listaH: bola de fogo e gelo, lâminas de vento, envenenamento em área,
    defesa mágica, impulso.
    listaE: envenenado, regeneração, curar, dano, reviver.

Cada entrada recebe um id no formato "#x nome", com x começando em 1. Ao
remover, a posição vira uma tumba (Null) e é reaproveitada pela próxima
entrada adicionada.

personagem (1 por save):
    nome e status (vivo ou morto);
    vida: quantidade atual e quantidade máxima;
    mana: quantidade atual e quantidade máxima;
    dinheiro: quantidade e extrato (lista de textos do que entrou e saiu);
    itens: cópia do item comprado (nome, preço pago, durabilidade atual,
    descrição, efeito_causado);
    habilidades: cópia da habilidade aprendida (nome, área afetada,
    efeito_causado, descrição, custo);
    efeitos: lista de nomes de efeitos ativos. Um mesmo efeito pode aparecer
    várias vezes (ex.: 2x envenenado).

## O que dá para fazer

No começo o programa mostra a tela de saves, com 3 espaços. Se o espaço está
vazio, o modo mestre abre para montar o mundo e criar 1 personagem; se já
está pronto, abre direto o modo jogador. O jogador pode trocar entre os 3
espaços a qualquer momento.

ações listas:
    Mestre_add_H: entra nome, área afetada, efeito_causado, descrição e custo => adiciona em listaH como "#x nome".
    Mestre_remover_H: entra nome => acha o "#x" e esvazia a posição (tumba).
    Mestre_add_I: mesma coisa que o add_H, só que para lista de itens (entra nome, preço, durabilidade máxima, descrição, efeito_causado, remendável).
    Mestre_remover_I: mesma coisa que o remover_H, só que para lista de itens.
    Mestre_add_E: entra nome => adiciona em listaE.
    Mestre_remover_E: entra nome => esvazia a posição.
    Mestre_finalizar_mundo: trava as listas e vai para o modo jogador.

ações personagem:
    criar_novo_personagem: entra nome, vida máxima, mana máxima e dinheiro inicial => personagem criado;
    status_personagem: entra nome => mostra todos os status do personagem em uma interface de texto (na aba de habilidades mostra nome_habilidade, custo, efeito_causada e área afetada; quem quiser mais detalhe usa "status_habilidade");
    trocar_personagem: muda o save ativo;
    excluir_ficha: entra o número do save => pergunta "deseja apagar a ficha de Cleber vivo?" (ou "morto?") e, se confirmar, apaga;
    ver_efeitos: mostra a lista numerada dos efeitos ativos no personagem;
    limpar_efeitos: mostra a lista numerada e o jogador digita os números que quer tirar; a leitura repete até digitar "n" ou até acabarem os efeitos.

ações item:
    comprar/adicionar: entra nome (com id, ex.: "#27 maçã") => procura em listaI, confere o dinheiro, copia o item para a ficha e desconta o preço;
    vender: entra nome => soma o preço ao dinheiro e remove o item da ficha;
    usar: entra nome => -1 durabilidade, imprime a descrição e aplica o efeito_causado. Se o item for um livro (efeito "aprender"), o personagem aprende a habilidade de mesmo nome;
    remendar: entra nome_item e valor_pago => só item remendável: durabilidade = durabilidade + round(valor_pago / 2), sem passar da máxima, e desconta o valor_pago do dinheiro. Item não remendável dá erro;
    lixo: entra nome_item => deleta o item da ficha.

ações vida:
    curar: entra valor => soma na vida e trava no máximo;
    dano: entra valor => subtrai da vida; se chegar a 0, o personagem fica
    morto;
    aumentarMAX: entra valor => muda a vida máxima;
    diminuirMAX: entra valor => muda a vida máxima.

ações mana:
    recuperar: entra valor => soma na mana e trava no máximo;
    gastar: entra valor => subtrai da mana;
    aumentarMAX: entra valor => muda a mana máxima;
    diminuirMAX: entra valor => muda a mana máxima.

ações habilidade:
    adicionar/aprender: entra nome => copia a habilidade de listaH para a ficha (normalmente usando o livro);
    remover: entra nome => deleta a habilidade da ficha;
    modificar: entra nome => muda qualquer atributo só da cópia daquele personagem;
    usar: entra nome_habilidade => confere a mana, desconta o custo e aplica o efeito_causado na lista de efeitos do personagem; imprime no terminal o que aconteceu (ex.: "o inimigo tomou 40 de dano, envenenamento, fadiga");
    status_habilidade: entra nome => mostra nome, área afetada, efeito_causado, descrição e custo.

ações dinheiro:
    gastar: entra valor => se não tiver o valor, dá erro; senão desconta e
    registra "gastou x" no extrato;
    receber: entra valor => soma e registra "recebeu x" no extrato.

## Regras

1. Só o mestre monta o mundo, e só no setup de um save vazio. No início o programa analisa listaH, listaI e listaE: se estiverem no padrão, o mestre abre; depois de finalizar, nada mais muda no catálogo e o mestre não volta até o programa ser reiniciado. Não pode haver 2 itens, 2 habilidades ou 2 personagens com o mesmo nome (a comparação ignora o "#x ").

2. Não há arquivo. Os "saves" servem para ter até 3 personagens na mesma execução, com o jogador podendo trocar entre eles, e tudo é esquecido quando o programa fecha. Mesmo assim, cada alteração é guardada no estado do save na hora em que acontece.

3. Removi o sistema de turnos, o mapa e o movimento. Não há localização: as áreas (área afetada) servem só para descrever a habilidade.

4. Não há inflação: o preço de um item fica fixo depois do setup, e o item na bolsa guarda o preço pago.

5. Item com durabilidade 0 é deletado automaticamente.

6. Não dá para comprar sem dinheiro: antes de comprar, o programa confere se há dinheiro suficiente; se não tiver, mostra mensagem de erro.

7. Não dá para usar habilidade sem mana suficiente (se tiver 0 de mana, mostra mensagem de erro dizendo isso). Personagem não morre por falta de mana.

8. HP pode ficar cheio: vida e mana nunca passam do máximo, o valor trava no teto.

9. Quando a vida chega a 0, o personagem fica com status morto. Morto não usa item nem habilidade: só pode ser revivido. O efeito reviver devolve o personagem à vida com 1 de vida.

10. O dono de um personagem não pode alterar os status dos outros personagens; quem mexe em tudo é o mestre, e só no setup.

11. Modificar altera só a cópia daquele personagem, não o catálogo. Um livro é um item com efeito_causado "aprender", e o nome do livro é o nome da habilidade que ele ensina.

12. Não tem nenhum sistema para personagens que voam.

13. quando o efeito é curar ou dano, o valor é perguntado ao jogador na hora.

14. - efeitos separados por ";";
    - morto só usa o item de reviver (volta com 1 de vida);
    - apagar ficha zera o save (o mestre monta o mundo de novo);
    - texto de uso neutro.



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

=== SALVAMENTO ===
1. Save 1 - Cleber (vivo | 100/100)
2. Save 2 - vazio
3. Save 3 - vazio
4. Excluir uma ficha
0. Sair
Escolha: 2

--- PAINEL DO MESTRE (mundo novo) ---
1. Habilidades
2. Itens
3. Efeitos
4. Criar personagem
5. Finalizar mundo
Escolha: 4
Nome: Bianca
Vida MAX: 100
Mana MAX: 50
Dinheiro inicial: 200
[!] Personagem 'Bianca' criado.

Escolha: 5
[!] Mundo finalizado. Bem-vindo, Bianca!

=== MENU DO JOGADOR ===
1. Status do personagem
2. Inventário e Loja
3. Habilidades
4. Efeitos
5. Dinheiro
6. Trocar de personagem
0. Voltar aos saves
Escolha: 3

--- HABILIDADES ---
1. Usar habilidade
2. Modificar habilidade
3. Remover habilidade
4. Status de habilidade
0. Voltar
Escolha: 1
Nome da habilidade: #2 bola de fogo
[!] Mana: 50 -> 35
[!] O inimigo tomou 40 de dano, envenenamento, fadiga.

=== STATUS DA FICHA ===
Nome: Bianca (vivo)
Vida (HP): 65/100  |  Mana (MP): 35/50  |  Dinheiro: $200

[ ITENS ]
- 1x Maçã
- 1x Espada (Durabilidade 10/50)

[ HABILIDADES ]
- #2 bola de fogo (Custo: 15 MP | Área Afetada: 3x1)
- #5 impulso (Custo: 5 MP)

[ EFEITOS ]
- envenenado
- fadiga
=======================
Pressione ENTER para voltar ao menu...
