#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Catalogo.hpp"
#include "Dinheiro.hpp"
#include "Efeito.hpp"
#include "Habilidade.hpp"
#include "Item.hpp"
#include "Mana.hpp"
#include "Vida.hpp"

// main TEMPORARIO: so serve para testar as classes das etapas 1 e 2.
// Na etapa 6 ele vira o main final, que so cria a Interface.
// Aqui pode usar cout porque este e o ponto de entrada, nao uma classe.

static void mostrarVida(const std::string& rotulo, const Vida& v) {
    std::cout << rotulo << ": " << v.getAtual() << "/" << v.getMaximo()
              << "\n";
}

static void testarVida() {
    std::cout << "=== VIDA ===\n";

    Vida v(100);

    std::cout << "dano 30  -> zerou? " << (v.dano(30) ? "sim" : "nao") << "\n";
    mostrarVida("depois do dano", v);

    v.curar(500);
    std::cout << "curar 500 (trava no maximo)\n";
    mostrarVida("depois da cura", v);

    v.aumentarMax(20);
    mostrarVida("aumentarMax 20", v);

    v.diminuirMax(400);
    mostrarVida("diminuirMax 400 (maximo trava em 1)", v);

    std::cout << "dano 99  -> zerou? " << (v.dano(99) ? "sim" : "nao") << "\n";
    mostrarVida("depois do dano", v);
}

static void testarMana() {
    std::cout << "\n=== MANA ===\n";

    Mana m(50);

    std::cout << "temMana(15)? " << (m.temMana(15) ? "sim" : "nao") << "\n";
    std::cout << "gastar 15 -> " << (m.gastar(15) ? "ok" : "falhou") << "\n";
    std::cout << "mana: " << m.getAtual() << "/" << m.getMaximo() << "\n";

    std::cout << "gastar 100 -> " << (m.gastar(100) ? "ok" : "falhou")
              << " (falta mana)\n";
    std::cout << "mana: " << m.getAtual() << "/" << m.getMaximo() << "\n";

    m.recuperar(1000);
    std::cout << "recuperar 1000 (trava no maximo)\n";
    std::cout << "mana: " << m.getAtual() << "/" << m.getMaximo() << "\n";
}

static void testarDinheiro() {
    std::cout << "\n=== DINHEIRO ===\n";

    Dinheiro d(200);

    std::cout << "receber 50\n";
    d.receber(50);
    std::cout << "  dinheiro: " << d.getQuantidade() << "\n";

    std::cout << "gastar 300 -> " << (d.gastar(300) ? "ok" : "falhou")
              << " (falta dinheiro)\n";
    std::cout << "  dinheiro: " << d.getQuantidade() << "\n";

    std::cout << "gastar 120 -> " << (d.gastar(120) ? "ok" : "falhou") << "\n";
    std::cout << "  dinheiro: " << d.getQuantidade() << "\n";

    std::cout << "extrato:\n";
    const std::vector<std::string>& e = d.getExtrato();
    for (std::vector<std::string>::size_type i = 0; i < e.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << e[i] << "\n";
    }
}

static void testarItens() {
    std::cout << "\n=== ITENS (sort e find) ===\n";

    std::vector<Item> bolsa;
    bolsa.push_back(Item(3, "pocao de vida", 30, 1, "recupera vida", "curar", false));
    bolsa.push_back(Item(1, "espada", 50, 10, "arma branca", "", true));
    bolsa.push_back(Item(2, "maca", 5, 1, "comida", "curar", false));
    bolsa.push_back(Item(4, "armadura", 80, 20, "defesa", "", true));

    // operator< compara pelo nome: o sort deixa em ordem alfabetica.
    std::sort(bolsa.begin(), bolsa.end());

    std::cout << "bolsa ordenada por nome:\n";
    for (std::vector<Item>::size_type i = 0; i < bolsa.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << bolsa[i].getEtiqueta() << " ($"
                  << bolsa[i].getPreco() << ")\n";
    }

    // operator== compara pelo nome: acho o item sem saber o id.
    Item alvo(0, "espada", 0, 0, "", "", false);
    std::vector<Item>::iterator achou =
        std::find(bolsa.begin(), bolsa.end(), alvo);

    if (achou != bolsa.end()) {
        std::cout << "find por nome 'espada' achou: " << achou->getEtiqueta()
                  << " | durabilidade " << achou->getDurabAtual() << "/"
                  << achou->getDurabMax() << "\n";
    } else {
        std::cout << "find por nome 'espada' nao achou nada\n";
    }

    // usarDurabilidade avisa quando o item acaba.
    Item maca(2, "maca", 5, 1, "comida", "curar", false);
    std::cout << "usar a maca -> ainda serve? "
              << (maca.usarDurabilidade() ? "sim" : "nao") << " (durabilidade "
              << maca.getDurabAtual() << ")\n";
}

static void testarHabilidades() {
    std::cout << "\n=== HABILIDADES (sort e find) ===\n";

    std::vector<Habilidade> ficha;
    ficha.push_back(Habilidade(2, "bola de fogo", "3x1", "dano", "joga fogo", 15));
    ficha.push_back(Habilidade(5, "impulso", "1x1", "", "empurra", 5));
    ficha.push_back(Habilidade(1, "gelo", "2x2", "envenenado", "congela", 10));

    std::sort(ficha.begin(), ficha.end());

    std::cout << "habilidades ordenadas por nome:\n";
    for (std::vector<Habilidade>::size_type i = 0; i < ficha.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << ficha[i].getEtiqueta()
                  << " (custo " << ficha[i].getCusto() << " MP | area "
                  << ficha[i].getAreaAfetada() << ")\n";
    }

    Habilidade alvo(0, "bola de fogo", "", "", "", 0);
    std::vector<Habilidade>::iterator achou =
        std::find(ficha.begin(), ficha.end(), alvo);

    if (achou != ficha.end()) {
        std::cout << "find por nome 'bola de fogo' achou: "
                  << achou->getEtiqueta() << " | efeito: "
                  << achou->getEfeitoCausado() << "\n";
    } else {
        std::cout << "find por nome 'bola de fogo' nao achou nada\n";
    }
}

static void testarEfeitos() {
    std::cout << "\n=== EFEITOS (set) ===\n";

    // O set usa operator< e ja guarda sem repetir e em ordem.
    std::set<Efeito> listaE;
    listaE.insert(Efeito("envenenado"));
    listaE.insert(Efeito("curar"));
    listaE.insert(Efeito("dano"));
    listaE.insert(Efeito("envenenado"));  // repetido: o set ignora

    std::cout << "efeitos no set (" << listaE.size() << " unicos):\n";
    std::set<Efeito>::iterator it = listaE.begin();
    while (it != listaE.end()) {
        std::cout << "  - " << it->getNome() << "\n";
        ++it;
    }
}

static void mostrarCatalogo(const Catalogo& c) {
    std::cout << "  [H] ";
    const std::map<int, Habilidade>& listaH = c.getListaH();
    std::map<int, Habilidade>::const_iterator h = listaH.begin();
    while (h != listaH.end()) {
        std::cout << h->second.getEtiqueta() << " | ";
        ++h;
    }
    std::cout << "\n";

    std::cout << "  [I] ";
    const std::map<int, Item>& listaI = c.getListaI();
    std::map<int, Item>::const_iterator i = listaI.begin();
    while (i != listaI.end()) {
        std::cout << i->second.getEtiqueta() << " | ";
        ++i;
    }
    std::cout << "\n";

    std::cout << "  [E] ";
    const std::set<Efeito>& listaE = c.getListaE();
    std::set<Efeito>::const_iterator e = listaE.begin();
    while (e != listaE.end()) {
        std::cout << e->getNome() << " | ";
        ++e;
    }
    std::cout << "\n";
}

static void testarCatalogo() {
    std::cout << "\n=== CATALOGO ===\n";

    Catalogo c;

    c.addH("bola de fogo", "3x1", "dano", "joga fogo", 15);
    c.addH("gelo", "2x2", "envenenado", "congela", 10);
    c.addH("impulso", "1x1", "", "empurra", 5);
    std::cout << "depois de add 3 habilidades:\n";
    mostrarCatalogo(c);

    std::cout << "add 'gelo' de novo -> "
              << (c.addH("gelo", "9x9", "x", "y", 1) ? "entrou" : "recusado")
              << " (nome repetido)\n";

    std::cout << "remover 'gelo' -> "
              << (c.removerH("gelo") ? "ok" : "falhou") << "\n";

    // 'gelo' era o #2, agora e uma tumba. O proximo add deve pegar o #2.
    c.addH("lamina de vento", "3x1", "dano", "corta", 12);
    std::cout << "add 'lamina de vento' reaproveitou o id:\n";
    mostrarCatalogo(c);

    const Habilidade* porNome = c.buscarH("bola de fogo");
    std::cout << "buscarH por nome 'bola de fogo' -> "
              << (porNome != 0 ? porNome->getEtiqueta() : "nao achou") << "\n";

    const Habilidade* porId = c.buscarH(3);
    std::cout << "buscarH por id #3 -> "
              << (porId != 0 ? porId->getEtiqueta() : "nao achou") << "\n";

    const Habilidade* tumba = c.buscarH(4);
    std::cout << "buscarH pelo id livre #4 -> "
              << (tumba != 0 ? tumba->getEtiqueta() : "nao achou") << "\n";

    c.addI("espada", 50, 10, "arma branca", "", true);
    c.addI("maca", 5, 1, "comida", "curar", false);
    c.removerI("espada");
    c.addI("pocao de vida", 30, 1, "recupera vida", "curar", false);
    std::cout << "itens (removi o #1 'espada' e adicionei outra):\n";
    mostrarCatalogo(c);

    c.addE("envenenado");
    c.addE("curar");
    c.addE("envenenado");
    std::cout << "addE 'envenenado' 2x -> "
              << (c.addE("envenenado") ? "entrou" : "recusado") << "\n";

    c.finalizar();
    std::cout << "finalizado? " << (c.estaFinalizado() ? "sim" : "nao")
              << "\n";
    std::cout << "add depois de finalizar -> "
              << (c.addH("defesa magica", "1x1", "", "escudo", 8) ? "entrou"
                                                                 : "recusado")
              << "\n";
    std::cout << "remover depois de finalizar -> "
              << (c.removerH("impulso") ? "ok" : "recusado") << "\n";
}

int main() {
    testarVida();
    testarMana();
    testarDinheiro();
    testarItens();
    testarHabilidades();
    testarEfeitos();
    testarCatalogo();
    return 0;
}
