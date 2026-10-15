#ifndef DINHEIRO_HPP
#define DINHEIRO_HPP

#include <string>
#include <vector>

// Guarda a quantidade de dinheiro e o extrato (lista de textos).
class Dinheiro {
private:
    int quantidade;
    std::vector<std::string> extrato;

public:
    explicit Dinheiro(int quantidade);

    int getQuantidade() const;

    // Devolve false (e nao mexe em nada) quando falta dinheiro.
    // Quando da certo, desconta e registra "gastou x" no extrato.
    bool gastar(int valor);

    // Soma o valor e registra "recebeu x" no extrato.
    void receber(int valor);

    // O extrato em ordem: do primeiro lancamento para o ultimo.
    const std::vector<std::string>& getExtrato() const;
};

#endif
