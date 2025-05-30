# MiniParserVisualizer

## Описание

**MiniParserVisualizer** — это консольное приложение на C++, предназначенное для парсинга файлов с точками в форматах `.txt`, `.bin`, `.json` и визуализации результатов с помощью Python-скрипта.  
Проект реализует пайплайн, который включает в себя:

1. Парсинг входных файлов с точками.
2. Передачу данных в Python-скрипт для визуализации.
3. Построение графика с точками, сгруппированными по файлам и группам.
4. Сохранение результата в файл `output.png`.

## Структура проекта

- `src/` — исходный код C++ (парсер, генератор, утилиты)
- `data/` — тестовые входные файлы (`file1.txt`, `file2.bin`, `file3.json`)
- `scripts/` — вспомогательные скрипты (`plot.py`, `run_pipeline.sh`)
- `build_my_project.sh` — автоматическая сборка и установка
- `CMakeLists.txt` — конфигурация CMake
- `README.md` — этот файл

### Дерево проекта

```plain
MiniParserVisualizer/
├── build_my_project.sh
├── CMakeLists.txt
├── README.md
├── data/
│   ├── file1.txt
│   ├── file2.bin
│   └── file3.json
├── scripts/
│   ├── plot.py
│   └── run_pipeline.sh
├── src/
│   ├── Parser.cpp
│   ├── Generator.cpp
│   └── Utils.cpp
└── tests/
    └── ParserTests.cpp
```

## Программные требования

### Основные

- **CMake**: версия 3.28 или выше
- **Компилятор C++**: GCC 13.3+ с поддержкой стандарта C++23
- **Python**: версия 3.10 или выше
- **pip**: менеджер пакетов Python

### Зависимости C++

- [nlohmann/json](https://github.com/nlohmann/json) — для работы с JSON
- [GTest](https://github.com/google/googletest) — для модульного тестирования

### Зависимости Python

- [matplotlib](https://matplotlib.org/) — для визуализации данных

> **Примечание:** Все зависимости можно установить через стандартные менеджеры пакетов (apt, pip и т.д.).

## Сборка и установка

1. Склонируйте к себе репозиторий и перейдите в него:

    ```bash
    git clone https://github.com/Joraslav/MiniParserVisualizer.git
    cd MiniParserVisualizer
    ```

2. Выполните скрипт сборки:

    ```bash
    ./build_my_project.sh
    ```

> **Примечание:** При необходимости, перед выполнением пункта 2, выполните следующую команду: chmod +x ./build_my_project.sh.

После успешной сборки и установки исполняемые файлы и скрипты будут находиться в директории:

Данный репозиторий уде предоставляет файлы для тестирования работоспособности проекта. Если файлы отсутствуют, то для их генерации выполните:

```bash
./build/data_generator
```

Будут созданы файлы:

- `data/file1.txt`
- `data/file2.bin`
- `data/file3.json`

## Запуск пайплайна

Для запуска полного пайплайна (парсинг и визуализация) используйте:

```sh
$HOME/miniparser_install/bin/run_pipeline.sh data/file1.txt data/file2.bin data/file3.json
```

В результате будет построен график и сохранён в `output.png`.

## Описание форматов

- **TXT:**  

  ```plain
  t<group>:<x>,<y>
  ```

  Пример: `t1:1,3`

- **BIN:**  
  Каждый point — 4 байта:  
  - group: 8 бит (старшие)
  - x: 12 бит
  - y: 12 бит (младшие)

- **JSON:**  

  ```json
  {
    "points": [
      {"group": "1", "x": 1, "y": 1},
      ...
    ]
  }
  ```

## Пример вывода

В результате работы пайплайна будет создан файл `output.png` с визуализацией точек по группам и файлам.

![output.png](output.png)

## Тестирование

Для запуска тестов используйте стандартные средства CTest:

```bash
cd build
ctest
```
