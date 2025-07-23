# Guess Who? AI – C++

C++ implementation of an automatic player for the "Guess Who?" game using a binary decision tree. Developed as part of the “Estructuras de Datos” (ED) course at the University of Granada.

## 🎮 Features

- Reads a character-attribute matrix (CSV or predefined format)
- Builds a binary tree to represent optimal yes/no questions
- Plays autonomously by asking strategic questions to eliminate characters
- Includes a console-based graphic mode with face images and cover image
- Option to provide an existing dataset or generate a random one

## 🛠️ Technologies

- C++
- File and image handling
- Terminal-based UI (with ASCII art or images)
- Binary tree traversal and construction

## 📂 Structure

- `src/` – All source files
- `data/` – Character files (attributes, default dataset) and images used in graphical version

## ▶️ How to Compile and Run

```bash
g++ -std=c++11 -o guesswho src/main.cpp
./guesswho [data_file_path_or_random] [cross_image_path]
```

## 🧪 Example
```bash
Copiar
Editar
./guesswho aleatorio
```
Generates a game with randomly generated characters (obviously, it cannot be played in graphic mode, for this mode requires the graphic representation of the characters and the cross image.

## 📌 Parameters:
`aleatorio` or `path` – Use random character set or load from file
`cross_path` – Path to the cross image
If you give only two parameters, it will run on text mode, if you give three parameters, it will run on graphic mode.

## 🎓 Academic Context
Created during the 2024/25 academic year for the "Data Structures" course in the Double Degree in Computer Engineering and Mathematics (UGR).

## 📜 License
This repository is for academic and portfolio purposes only.
Contact: javier.ortmol@gmail.com
