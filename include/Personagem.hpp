#ifndef PERSONAGEM_HPP
#define PERSONAGEM_HPP

#include <list>
#include <string>
#include <vector>

#include "Catalogo.hpp"
#include "Dinheiro.hpp"
#include "Efeito.hpp"
#include "Habilidade.hpp"
#include "Item.hpp"
#include "Mana.hpp"
#include "Vida.hpp"

// A ficha de um personagem. Guarda os numeros (vida, mana, dinheiro) e as
// listas: itens (vector, guarda a ordem e aceita repetido), habilidades
// (vector de copias do catalogo) e efeitos (list, porque um efeito repete e a
// remocao no meio fica barata).
//
// A classe nao imprime nada: quando precisa dizer o que aconteceu, recebe um
// std::string& mensagem por parametro e escreve ali.
class Personagem {
private:
    std::string nome;
    bool vivo;
    Vida vida;
    Mana mana;
    Dinheiro dinheiro;
    std::vector<Item> itens;
    std::vector<Habilidade> habilidades;
    std::list<Efeito> efeitos;

    // Posicao na lista, ou -1 se nao existir.
    int indiceItem(const std::string& nome) const;
    int indiceHabilidade(const std::string& nome) const;

    // Le um texto tipo "dano; envenenado" e aplica cada pedaco.
    // catalogo so e usado no "aprender" (livro); nas habilidades vai 0.
    void aplicarTexto(const std::string& texto, int valor,
                      const Catalogo* catalogo, const std::string& nomeDoItem,
                      std::string& mensagem);

public:
    Personagem(const std::string& nome, int vidaMax, int manaMax,
               int dinheiroInicial);

    // --- leitura da ficha ---
    std::string getNome() const;
    bool estaVivo() const;
    int getVidaAtual() const;
    int getVidaMaxima() const;
    int getManaAtual() const;
    int getManaMaxima() const;
    const Dinheiro& getDinheiro() const;
    const std::vector<Item>& getItens() const;
    const std::vector<Habilidade>& getHabilidades() const;
    const std::list<Efeito>& getEfeitos() const;

    // Diz se a ficha tem esse item / essa habilidade (comparando pelo nome).
    bool temItem(const std::string& nome) const;
    bool temHabilidade(const std::string& nome) const;

    // --- vida ---
    bool curar(int valor);
    bool dano(int valor);  // se zerar, o personagem morre
    void aumentarMaxVida(int valor);
    void diminuirMaxVida(int valor);

    // --- mana ---
    bool recuperar(int valor);
    bool gastarMana(int valor);  // false quando falta mana
    bool temMana(int valor) const;
    void aumentarMaxMana(int valor);
    void diminuirMaxMana(int valor);

    // --- dinheiro ---
    bool gastarDinheiro(int valor);  // false quando falta dinheiro
    void receberDinheiro(int valor);

    // --- itens ---
    bool comprar(const Item& item);
    bool vender(const std::string& nome);
    // Quando devolve false, a mensagem explica o motivo (nao tem o item, morto...).
    bool usarItem(const std::string& nome, int valor, const Catalogo& catalogo,
                  std::string& mensagem);
    bool remendar(const std::string& nomeItem, int valorPago);
    bool jogarNoLixo(const std::string& nomeItem);

    // --- habilidades ---
    bool aprender(const Habilidade& habilidade);
    bool modificarHabilidade(const std::string& nome, const Habilidade& nova);
    bool removerHabilidade(const std::string& nome);
    // Quando devolve false, a mensagem explica o motivo (morto, sem mana...).
    bool usarHabilidade(const std::string& nome, int valor,
                        std::string& mensagem);

    // --- efeitos ---
    void aplicarEfeito(const Efeito& efeito);
    // Remove os efeitos nas posicoes (1, 2, 3...) que aparecem na tela.
    bool limparEfeitos(const std::vector<int>& posicoes);

    // --- status ---
    void morrer();
    void reviver();  // volta com 1 de vida
};

#endif
