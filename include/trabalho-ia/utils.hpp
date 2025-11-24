#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <cmath>

// --- Configurações Globais do Modelo ---
namespace Config {
    const int INPUT_SIZE = 100;   // Dimensão dos vetores fornecidos
    const int HIDDEN_SIZE = 24;   // Neurônios na camada oculta
    const int OUTPUT_SIZE = 1;    // Classificação binária
    const double LEARNING_RATE = 0.01;
    const int EPOCHS = 500;
}

// --- Estruturas de Dados ---
struct Dataset {
    std::vector<std::vector<double>> inputs;
    std::vector<double> targets;
};

// --- Funções Matemáticas Auxiliares (Inline para performance) ---
namespace Math {
    inline double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    inline double sigmoidDerivative(double x) {
        return x * (1.0 - x);
    }
}

#endif // UTILS_HPP
