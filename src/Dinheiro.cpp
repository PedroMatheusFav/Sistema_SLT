#include "Dinheiro.hpp"

Dinheiro::Dinheiro(int quantidade) {
    this->quantidade = (quantidade > 0) ? quantidade : 0;
}

int Dinheiro::getQuantidade() const {
    return quantidade;
}

bool Dinheiro::gastar(int valor) {
    if (valor < 0) {
        return false;
    }

    if (valor > quantidade) {
        // Nao mexe no dinheiro nem no extrato: falta dinheiro.
        return false;
    }

    quantidade = quantidade - valor;
    extrato.push_back("gastou " + std::to_string(valor));
    return true;
}

void Dinheiro::receber(int valor) {
    if (valor <= 0) {
        return;
    }

    quantidade = quantidade + valor;
    extrato.push_back("recebeu " + std::to_string(valor));
}

const std::vector<std::string>& Dinheiro::getExtrato() const {
    return extrato;
}
