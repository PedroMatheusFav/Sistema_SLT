#ifndef EFEITO_HPP
#define EFEITO_HPP

#include <string>

// Um efeito do jogo tem so o nome (ex.: "envenenado").
// No Catalogo ele fica num std::set, entao precisa do operator<.
class Efeito {
private:
    std::string nome;

public:
    explicit Efeito(const std::string& nome);

    std::string getNome() const;

    // Ordena e compara pelo nome, para o set e para o std::find.
    bool operator<(const Efeito& outro) const;
    bool operator==(const Efeito& outro) const;
};

#endif
