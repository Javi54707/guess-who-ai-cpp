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
- `data/` – Character files (attributes, default dataset)
- `assets/` – Images used in graphical version
- `output/` – Saved games or results (optional)

## ▶️ How to Compile

```bash
g++ -std=c++11 -o guesswho src/*.cpp
./guesswho [mode] [data_file_or_random] [images_dir]
```

## Example
```bash
Copiar
Editar
./guesswho gui aleatorio assets/
```

## Parameters:
`gui` or `text` – Run in graphical or text mode
`aleatorio` or `path` – Use random character set or load from file
`assets/` – Directory with character images and default cover

## 🧪 Sample assets
You may include a few small sample images and a basic attributes file in the assets/ and data/ folders to showcase functionality.

## 🎓 Academic Context
Created during the 2024/25 academic year for the "Data Structures" course in the Double Degree in Computer Engineering and Mathematics (UGR).

## 📜 License
This repository is for academic and portfolio purposes only.
Contact: javier.ortmol@gmail.com
