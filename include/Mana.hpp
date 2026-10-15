#ifndef MANA_HPP
#define MANA_HPP

// Guarda a mana atual e a mana maxima.
// Sem cin/cout: quem mostra a mensagem de erro e a interface.
class Mana {
private:
    int atual;
    int maximo;

public:
    // Comeca com a mana cheia (atual = maximo).
    explicit Mana(int maximo);

    int getAtual() const;
    int getMaximo() const;

    // Diz se ha mana suficiente para pagar o custo.
    bool temMana(int custo) const;

    // Soma o valor e trava no maximo. Devolve true se recuperou alguma coisa.
    bool recuperar(int valor);

    // Subtrai o valor, mas so se tiver mana bastante.
    // Devolve false (e nao gasta nada) quando falta mana.
    bool gastar(int valor);

    // Muda a mana maxima. Se o maximo cair abaixo do atual, o atual acompanha.
    void aumentarMax(int valor);
    void diminuirMax(int valor);
};

#endif
