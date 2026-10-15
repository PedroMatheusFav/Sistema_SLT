#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

// Um item do catalogo. O id fica separado do nome (mesma ideia da Habilidade).
class Item {
private:
    int id;
    std::string nome;
    int preco;
    int durabAtual;
    int durabMax;
    std::string descricao;
    std::string efeitoCausado;  // pode ser vazio
    bool remendavel;

public:
    Item(int id, const std::string& nome, int preco, int durabMax,
         const std::string& descricao, const std::string& efeitoCausado,
         bool remendavel);

    int getId() const;
    std::string getNome() const;
    int getPreco() const;
    int getDurabAtual() const;
    int getDurabMax() const;
    std::string getDescricao() const;
    std::string getEfeitoCausado() const;
    bool getRemendavel() const;

    // Monta o texto "#x nome" na hora de mostrar.
    std::string getEtiqueta() const;

    // Gasta 1 de durabilidade. Devolve true se o item AINDA tem durabilidade;
    // devolve false quando chegou a 0 (aí o item deve ser apagado).
    bool usarDurabilidade();

    // Ordena pelo nome (para o std::sort) e compara pelo nome (std::find).
    bool operator<(const Item& outro) const;
    bool operator==(const Item& outro) const;
};

#endif
