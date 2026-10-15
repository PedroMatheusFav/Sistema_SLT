#include "Personagem.hpp"

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

#include "Catalogo.hpp"
#include "Efeito.hpp"
#include "Habilidade.hpp"
#include "Item.hpp"

// Tira espacos do comeco e do fim de um pedaco de efeito (" dano ").
static std::string semEspacos(const std::string& texto) {
    std::string::size_type inicio = texto.find_first_not_of(" \t");
    if (inicio == std::string::npos) {
        return "";
    }
    std::string::size_type fim = texto.find_last_not_of(" \t");
    return texto.substr(inicio, fim - inicio + 1);
}

// Diz se o texto tem aquele efeito exato (token por token, separado por ';').
// Comparar o texto inteiro nao serve: "naoreviver" nao e "reviver".
static bool temEfeito(const std::string& texto, const std::string& alvo) {
    std::string::size_type inicio = 0;
    while (inicio <= texto.size()) {
        std::string::size_type fim = texto.find(';', inicio);
        std::string token;
        if (fim == std::string::npos) {
            token = texto.substr(inicio);
            inicio = texto.size() + 1;
        } else {
            token = texto.substr(inicio, fim - inicio);
            inicio = fim + 1;
        }
        if (semEspacos(token) == alvo) {
            return true;
        }
    }
    return false;
}

Personagem::Personagem(const std::string& nome, int vidaMax, int manaMax,
                       int dinheiroInicial)
    : nome(nome),
      vivo(true),
      vida(vidaMax),
      mana(manaMax),
      dinheiro(dinheiroInicial) {
}

int Personagem::indiceItem(const std::string& nome) const {
    for (std::vector<Item>::size_type i = 0; i < itens.size(); ++i) {
        // operator== compara so o nome.
        if (itens[i] == Item(0, nome, 0, 0, "", "", false)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int Personagem::indiceHabilidade(const std::string& nome) const {
    for (std::vector<Habilidade>::size_type i = 0; i < habilidades.size();
         ++i) {
        if (habilidades[i] == Habilidade(0, nome, "", "", "", 0)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

std::string Personagem::getNome() const {
    return nome;
}

bool Personagem::estaVivo() const {
    return vivo;
}

int Personagem::getVidaAtual() const {
    return vida.getAtual();
}

int Personagem::getVidaMaxima() const {
    return vida.getMaximo();
}

int Personagem::getManaAtual() const {
    return mana.getAtual();
}

int Personagem::getManaMaxima() const {
    return mana.getMaximo();
}

const Dinheiro& Personagem::getDinheiro() const {
    return dinheiro;
}

const std::vector<Item>& Personagem::getItens() const {
    return itens;
}

const std::vector<Habilidade>& Personagem::getHabilidades() const {
    return habilidades;
}

const std::list<Efeito>& Personagem::getEfeitos() const {
    return efeitos;
}

bool Personagem::temItem(const std::string& nome) const {
    return indiceItem(nome) >= 0;
}

bool Personagem::temHabilidade(const std::string& nome) const {
    return indiceHabilidade(nome) >= 0;
}

bool Personagem::curar(int valor) {
    return vida.curar(valor);
}

bool Personagem::dano(int valor) {
    // Morto ja esta com vida 0: nao ha o que tirar.
    if (valor <= 0 || !vivo) {
        return false;
    }

    bool zerou = vida.dano(valor);
    if (zerou) {
        // Regra 9: vida 0 = morto.
        morrer();
    }
    return zerou;
}

void Personagem::aumentarMaxVida(int valor) {
    vida.aumentarMax(valor);
}

void Personagem::diminuirMaxVida(int valor) {
    vida.diminuirMax(valor);
}

bool Personagem::recuperar(int valor) {
    return mana.recuperar(valor);
}

bool Personagem::gastarMana(int valor) {
    return mana.gastar(valor);
}

bool Personagem::temMana(int valor) const {
    return mana.temMana(valor);
}

void Personagem::aumentarMaxMana(int valor) {
    mana.aumentarMax(valor);
}

void Personagem::diminuirMaxMana(int valor) {
    mana.diminuirMax(valor);
}

bool Personagem::gastarDinheiro(int valor) {
    return dinheiro.gastar(valor);
}

void Personagem::receberDinheiro(int valor) {
    dinheiro.receber(valor);
}

bool Personagem::comprar(const Item& item) {
    // Regra 6: sem dinheiro nao compra (o gastar avisa com false).
    if (!dinheiro.gastar(item.getPreco())) {
        return false;
    }

    // Copia o item para a ficha. A copia guarda o id do catalogo so para
    // montar a etiqueta e o preco que foi pago.
    itens.push_back(item);
    return true;
}

bool Personagem::vender(const std::string& nome) {
    int i = indiceItem(nome);
    if (i < 0) {
        return false;
    }

    dinheiro.receber(itens[i].getPreco());
    itens.erase(itens.begin() + i);
    return true;
}

bool Personagem::usarItem(const std::string& nome, int valor,
                          const Catalogo& catalogo, std::string& mensagem) {
    int i = indiceItem(nome);
    if (i < 0) {
        mensagem = "nao tem o item " + nome;
        return false;
    }

    const std::string efeito = itens[i].getEfeitoCausado();
    bool ehReviver = temEfeito(efeito, "reviver");

    // Regra 9: morto so usa o item de reviver.
    if (!vivo && !ehReviver) {
        mensagem = "personagem morto: so pode usar o item de reviver";
        return false;
    }

    mensagem = "usou " + itens[i].getEtiqueta() + ": " +
               itens[i].getDescricao();

    // Aplica os efeitos ANTES de gastar a durabilidade: assim o ultimo uso
    // ainda vale.
    aplicarTexto(efeito, valor, &catalogo, itens[i].getNome(), mensagem);

    if (!itens[i].usarDurabilidade()) {
        // Regra 5: durabilidade 0 = sai da ficha.
        itens.erase(itens.begin() + i);
        mensagem += " (acabou e saiu da bolsa)";
    }
    return true;
}

bool Personagem::remendar(const std::string& nomeItem, int valorPago) {
    int i = indiceItem(nomeItem);
    if (i < 0) {
        return false;
    }
    if (valorPago <= 0) {
        return false;
    }
    // Comida, pocao e livro nao sao remendaveis.
    if (!itens[i].getRemendavel()) {
        return false;
    }
    // Paga primeiro; se faltar dinheiro, nem remenda.
    if (!dinheiro.gastar(valorPago)) {
        return false;
    }

    itens[i].remendar(valorPago);
    return true;
}

bool Personagem::jogarNoLixo(const std::string& nomeItem) {
    int i = indiceItem(nomeItem);
    if (i < 0) {
        return false;
    }

    itens.erase(itens.begin() + i);
    return true;
}

bool Personagem::aprender(const Habilidade& habilidade) {
    if (indiceHabilidade(habilidade.getNome()) >= 0) {
        return false;  // nao aprende duas vezes a mesma
    }

    habilidades.push_back(habilidade);
    return true;
}

bool Personagem::modificarHabilidade(const std::string& nome,
                                     const Habilidade& nova) {
    int i = indiceHabilidade(nome);
    if (i < 0) {
        return false;
    }

    // Muda so a copia deste personagem, o catalogo nao e tocado.
    habilidades[i] = nova;
    return true;
}

bool Personagem::removerHabilidade(const std::string& nome) {
    int i = indiceHabilidade(nome);
    if (i < 0) {
        return false;
    }

    habilidades.erase(habilidades.begin() + i);
    return true;
}

bool Personagem::usarHabilidade(const std::string& nome, int valor,
                                std::string& mensagem) {
    if (!vivo) {
        mensagem = "personagem morto: so pode ser revivido";
        return false;
    }

    int i = indiceHabilidade(nome);
    if (i < 0) {
        mensagem = "habilidade " + nome + " nao pertence a ficha de " + this->nome;
        return false;
    }

    int custo = habilidades[i].getCusto();
    // Regra 7: sem mana suficiente nao usa e avisa.
    if (!mana.temMana(custo)) {
        mensagem = "mana insuficiente: custa " + std::to_string(custo) +
                   ", tem " + std::to_string(mana.getAtual());
        return false;
    }
    mana.gastar(custo);

    mensagem = "usou " + habilidades[i].getEtiqueta() + " (custo " +
               std::to_string(custo) + " MP)";

    // Nas habilidades nao existe "aprender", entao o catalogo vai nulo.
    aplicarTexto(habilidades[i].getEfeitoCausado(), valor, 0, "", mensagem);
    return true;
}

void Personagem::aplicarEfeito(const Efeito& efeito) {
    efeitos.push_back(efeito);
}

bool Personagem::limparEfeitos(const std::vector<int>& posicoes) {
    if (posicoes.empty()) {
        return false;
    }

    // Ordeno do maior para o menor: ao tirar o de tras, a numeracao dos
    // que ficam na frente nao muda. O unique evita remover duas vezes a
    // mesma posicao (o que tiraria o efeito errado).
    std::vector<int> ordenadas = posicoes;
    std::sort(ordenadas.begin(), ordenadas.end());
    ordenadas.erase(std::unique(ordenadas.begin(), ordenadas.end()),
                    ordenadas.end());

    bool removeu = false;
    for (std::vector<int>::size_type k = ordenadas.size(); k > 0; --k) {
        int pos = ordenadas[k - 1];
        if (pos < 1 || pos > static_cast<int>(efeitos.size())) {
            continue;
        }

        std::list<Efeito>::iterator it = efeitos.begin();
        std::advance(it, pos - 1);
        efeitos.erase(it);
        removeu = true;
    }
    return removeu;
}

void Personagem::morrer() {
    vivo = false;
    // Zera a vida tambem: assim o status nunca fica "morto com HP > 0".
    if (vida.getAtual() > 0) {
        vida.dano(vida.getAtual());
    }
}

void Personagem::reviver() {
    if (vivo) {
        return;
    }

    vivo = true;
    // Regra 9: o efeito reviver devolve o personagem com 1 de vida.
    vida.curar(1);
}

void Personagem::aplicarTexto(const std::string& texto, int valor,
                              const Catalogo* catalogo,
                              const std::string& nomeDoItem,
                              std::string& mensagem) {
    if (texto.empty()) {
        return;
    }

    std::vector<std::string> partes;

    // Separo o texto pelo ';' e trato cada pedaco.
    std::string::size_type inicio = 0;
    while (inicio <= texto.size()) {
        std::string::size_type fim = texto.find(';', inicio);
        std::string token;
        if (fim == std::string::npos) {
            token = texto.substr(inicio);
            inicio = texto.size() + 1;  // encerra o laco
        } else {
            token = texto.substr(inicio, fim - inicio);
            inicio = fim + 1;
        }

        token = semEspacos(token);
        if (token.empty()) {
            continue;
        }

        if (token == "curar") {
            vida.curar(valor);
            partes.push_back("curou " + std::to_string(valor));
        } else if (token == "recuperar") {
            // Como curar/dano, usa o valor digitado na hora.
            mana.recuperar(valor);
            partes.push_back("recuperou " + std::to_string(valor) + " de mana");
        } else if (token == "dano") {
            bool zerou = dano(valor);
            partes.push_back("levou " + std::to_string(valor) + " de dano");
            if (zerou) {
                partes.push_back("morreu");
            }
        } else if (token == "reviver") {
            if (!vivo) {
                reviver();
                partes.push_back("reviveu com 1 de vida");
            } else {
                partes.push_back("ja estava vivo");
            }
        } else if (token == "aprender") {
            if (catalogo != 0) {
                const Habilidade* achou = catalogo->buscarH(nomeDoItem);
                if (achou == 0) {
                    partes.push_back("nao existe habilidade " + nomeDoItem);
                } else if (aprender(*achou)) {
                    partes.push_back("aprendeu " + achou->getNome());
                } else {
                    partes.push_back("ja sabia " + achou->getNome());
                }
            }
        } else {
            // Qualquer outro nome vira um efeito ativo na lista.
            aplicarEfeito(Efeito(token));
            partes.push_back("efeito " + token);
        }
    }

    if (!partes.empty()) {
        mensagem += " -> ";
        for (std::vector<std::string>::size_type k = 0; k < partes.size();
             ++k) {
            if (k > 0) {
                mensagem += ", ";
            }
            mensagem += partes[k];
        }
    }
}
