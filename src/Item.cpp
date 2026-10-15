#include "Item.hpp"

#include <string>

Item::Item(int id, const std::string& nome, int preco, int durabMax,
           const std::string& descricao, const std::string& efeitoCausado,
           bool remendavel)
    : id(id),
      nome(nome),
      preco(preco),
      durabMax((durabMax > 0) ? durabMax : 0),
      descricao(descricao),
      efeitoCausado(efeitoCausado),
      remendavel(remendavel) {
    // Quando o item nasce, ele esta inteiro.
    durabAtual = this->durabMax;
}

int Item::getId() const {
    return id;
}

std::string Item::getNome() const {
    return nome;
}

int Item::getPreco() const {
    return preco;
}

int Item::getDurabAtual() const {
    return durabAtual;
}

int Item::getDurabMax() const {
    return durabMax;
}

std::string Item::getDescricao() const {
    return descricao;
}

std::string Item::getEfeitoCausado() const {
    return efeitoCausado;
}

bool Item::getRemendavel() const {
    return remendavel;
}

std::string Item::getEtiqueta() const {
    return "#" + std::to_string(id) + " " + nome;
}

bool Item::usarDurabilidade() {
    if (durabAtual > 0) {
        durabAtual = durabAtual - 1;
    }

    // true = ainda serve; false = zerou e deve sair da ficha.
    return durabAtual > 0;
}

bool Item::operator<(const Item& outro) const {
    return nome < outro.nome;
}

bool Item::operator==(const Item& outro) const {
    return nome == outro.nome;
}
