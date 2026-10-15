#include <iostream>
#include <string>
#include <vector>

#include "Dinheiro.hpp"
#include "Mana.hpp"
#include "Vida.hpp"

// main TEMPORARIO da etapa 1: so serve para testar Vida, Mana e Dinheiro.
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

int main() {
    testarVida();
    testarMana();
    testarDinheiro();
    return 0;
}
