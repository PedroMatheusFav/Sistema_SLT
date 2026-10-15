#ifndef CATALOGO_HPP
#define CATALOGO_HPP

#include <map>
#include <set>
#include <string>

#include "Efeito.hpp"
#include "Habilidade.hpp"
#include "Item.hpp"

// O catalogo e o "mundo" montado pelo mestre. Ele guarda as tres listas:
//   listaH -> habilidades, com id
//   listaI -> itens, com id
//   listaE -> efeitos, que so tem nome (por isso vai num set)
//
// Regras que moram aqui:
//   - ids comecam em 1 e a "tumba" e so um id que nao existe no map;
//   - o proximo add usa o menor id livre (reaproveita a tumba);
//   - nao pode haver dois com o mesmo nome;
//   - depois de finalizar(), nada mais muda.
class Catalogo {
private:
    std::map<int, Habilidade> listaH;
    std::map<int, Item> listaI;
    std::set<Efeito> listaE;
    bool finalizado;

    // Menor id livre (1, 2, 3...) olhando as chaves em ordem.
    int proximoIdH() const;
    int proximoIdI() const;

    // Diz se ja existe algo com esse nome (a comparação ignora o "#x ").
    bool nomeHabilidadeEmUso(const std::string& nome) const;
    bool nomeItemEmUso(const std::string& nome) const;

public:
    Catalogo();

    // --- mestre: habilidades ---
    // Devolve false quando o mundo ja foi finalizado ou o nome ja existe.
    bool addH(const std::string& nome, const std::string& areaAfetada,
              const std::string& efeitoCausado, const std::string& descricao,
              int custo);
    bool removerH(const std::string& nome);

    // --- mestre: itens ---
    bool addI(const std::string& nome, int preco, int durabMax,
              const std::string& descricao, const std::string& efeitoCausado,
              bool remendavel);
    bool removerI(const std::string& nome);

    // --- mestre: efeitos ---
    bool addE(const std::string& nome);
    bool removerE(const std::string& nome);

    // --- buscas ---
    // Devolvem um ponteiro para o objeto dentro do map, ou 0 se nao achar.
    // Nao precisa de delete: o dono e o proprio map.
    const Habilidade* buscarH(int id) const;
    const Habilidade* buscarH(const std::string& nome) const;
    const Item* buscarI(int id) const;
    const Item* buscarI(const std::string& nome) const;

    // --- ver tudo (so leitura) para a interface listar ---
    const std::map<int, Habilidade>& getListaH() const;
    const std::map<int, Item>& getListaI() const;
    const std::set<Efeito>& getListaE() const;

    // Trava o mundo: o mestre nao mexe mais em nada.
    void finalizar();
    bool estaFinalizado() const;

    // Volta as tres listas para o padrao descrito na DESCRICAO.md
    // (e desfinaliza). Usado quando um save novo comeca.
    void carregarPadrao();
};

#endif
