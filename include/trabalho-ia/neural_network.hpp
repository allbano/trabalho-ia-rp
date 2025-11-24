#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include "utils.hpp"
#include <vector>

// Implementação de MLP (Multilayer Perceptron)
class NeuralNetwork {
private:
    std::vector<std::vector<double>> weightsInputHidden;
    std::vector<double> biasHidden;
    std::vector<std::vector<double>> weightsHiddenOutput;
    std::vector<double> biasOutput;

    void initializeWeights();

public:
    NeuralNetwork();

    // Calcula a saída da rede para uma entrada
    double predict(const std::vector<double>& input);

    // Treina a rede usando Backpropagation
    void train(const std::vector<std::vector<double>>& inputs, const std::vector<double>& targets);
};

#endif // NEURAL_NETWORK_HPP
