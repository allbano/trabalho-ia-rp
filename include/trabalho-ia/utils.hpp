#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <vector>

// --- Configurações Globais do Modelo ---
namespace Config {
const int INPUT_SIZE = 100; // Dimensão dos vetores fornecidos
const int HIDDEN_SIZE = 32; // Neurônios na camada oculta
const int OUTPUT_SIZE = 1;  // Classificação binária
const double LEARNING_RATE = 0.05;
const int EPOCHS = 1000;
} // namespace Config

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
    // NOVA FUNÇÃO: Normalização L2 (Euclidiana)
    // Transforma o vetor para que sua magnitude seja 1.0
    inline void normalizeVector(std::vector<double> &vec) {
      double sumSq = 0.0;
      for (double val : vec) 
        sumSq += val * val;
      double norm = std::sqrt(sumSq);
    
      if (norm > 1e-9) { // Evita divisão por zero
        for (double &val : vec)
          val /= norm;
    }
  }
} // namespace Math

#endif // UTILS_HPP
