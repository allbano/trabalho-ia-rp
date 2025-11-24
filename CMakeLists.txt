cmake_minimum_required(VERSION 3.15)

# Nome do Projeto e Versão
project(TrabalhoIA_Classificador VERSION 1.3.0 LANGUAGES CXX)

# --- 1. Configurações do C++ ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- 2. Tipo de Build e Otimizações ---
# Se não especificado, usa Release (Otimizado)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Flags de compilação (Otimização crítica para Redes Neurais)
if(MSVC)
    # Windows (Visual Studio)
    add_compile_options(/W4 /O2 /permissive-)
else()
    # Linux/Mac (GCC/Clang)
    # -O3: Otimização máxima
    # -march=native: Usa instruções específicas do seu processador (AVX, etc)
    add_compile_options(-Wall -Wextra -O3 -march=native)
endif()

# --- 3. Definição dos Fontes da Biblioteca (Core) ---
# Aqui listamos todos os arquivos .cpp que NÃO são o main.
# Importante: Verifique se o nome do arquivo em src/utils é 
# 'file_system_utils.cpp' ou 'FileSystemUtils.cpp' e ajuste abaixo se necessário.
set(CORE_SOURCES
    src/net/neural_network.cpp
    src/io/text_processor.cpp
    src/utils/file_system_utils.cpp
)

# --- 4. Criação da Biblioteca Estática "core_lib" ---
# Isso compila a lógica do projeto separada do executável
add_library(core_lib STATIC ${CORE_SOURCES})

# Define onde estão os arquivos .hpp (headers)
target_include_directories(core_lib PUBLIC include)

# --- 5. Criação do Executável Principal ---
add_executable(classificador src/main.cpp)

# Linkagem: O executável precisa da biblioteca core para funcionar
target_link_libraries(classificador PRIVATE core_lib)

# --- 6. Configuração de Testes e Benchmarks (Opcional) ---
# Só ativa se as pastas existirem
if(EXISTS "${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt")
    enable_testing()
    add_subdirectory(tests)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/benchmarks/CMakeLists.txt")
    add_subdirectory(benchmarks)
endif()

# --- 7. Feedback Visual ---
message(STATUS "-------------------------------------------------")
message(STATUS "Projeto: ${PROJECT_NAME}")
message(STATUS "Modo de Build: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Fontes Core: ${CORE_SOURCES}")
message(STATUS "-------------------------------------------------")
