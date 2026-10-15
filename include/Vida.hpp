#ifndef VIDA_HPP
#define VIDA_HPP

// Guarda a vida atual e a vida maxima.
// A classe nao imprime nada: ela devolve bool quando precisa avisar algo.
class Vida {
private:
    int atual;
    int maximo;

public:
    // Comeca com a vida cheia (atual = maximo).
    explicit Vida(int maximo);

    int getAtual() const;
    int getMaximo() const;

    // Soma o valor e trava no maximo. Devolve true se curou alguma coisa.
    bool curar(int valor);

    // Subtrai o valor. Devolve true se a vida chegou a zero (personagem morreu).
    bool dano(int valor);

    // Muda a vida maxima. Se o maximo cair abaixo do atual, o atual acompanha.
    void aumentarMax(int valor);
    void diminuirMax(int valor);
};

#endif
