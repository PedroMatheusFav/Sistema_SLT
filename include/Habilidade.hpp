#ifndef HABILIDADE_HPP
#define HABILIDADE_HPP

#include <string>

// Uma habilidade do catalogo. O id fica separado do nome:
// assim comparar habilidades "ignorando o #x " e so comparar o nome.
class Habilidade {
private:
    int id;
    std::string nome;
    std::string areaAfetada;
    std::string efeitoCausado;
    std::string descricao;
    int custo;  // custo em mana

public:
    Habilidade(int id, const std::string& nome, const std::string& areaAfetada,
               const std::string& efeitoCausado, const std::string& descricao,
               int custo);

    int getId() const;
    std::string getNome() const;
    std::string getAreaAfetada() const;
    std::string getEfeitoCausado() const;
    std::string getDescricao() const;
    int getCusto() const;

    // Monta o texto "#x nome" na hora de mostrar.
    std::string getEtiqueta() const;

    // Ordena pelo nome (para o std::sort) e compara pelo nome (std::find).
    bool operator<(const Habilidade& outro) const;
    bool operator==(const Habilidade& outro) const;
};

#endif
