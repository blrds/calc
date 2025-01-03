# Описание архитектуры
Проект представляет собой реализацию симуляции численного решения системы дифференциальных уравнений с использованием модульного подхода.
## Основные компоненты системы
### 1. Конфигурационный файл ```config.conf```
- Файл конфигурации используется для настройки параметров проекта, таких как флаги отладки, параметры численного метода и константы системы
- В коде реализована динамическая загрузка конфигурации на этапе выполнения. Значения с ```y``` активируют флаги, ```n``` отключают, а числовые параметры используются непосредственно
### 2. Основной модуль ```main```
- Основная точка входа программы.
- Обрабатывает пользовательский ввод для управления симуляцией:
  + Запуск вычислений.
  + Завершение работы программы.
- Создаёт дочерний поток, который выполняет функцию ```run``` с бесконечным циклом, пока не будет подан сигнал завершения
- Метод ```run```
  + каждая итерация цикла является отдельным тактом симмуляции
### 3. ```formula.h|.c```
- Состоит из двух сабмодулей: формулы и операции:
  
  Формулы:
    - Содержит реализацию всех мат и бул операций
      + Текущие поддержанные операции:
        Математические: +, -, *, /, %(mod)
        Логические: и, или, не
  
  Операции:
    - Описывает структуру операций, которая включает переменные, функции и параметры.
    - Реализует функции для работы с операциями:
      + Преобразование операций в строку.
      + Создание операций на основе формул.
### 4. ```vars.h|.c```
- Реализует список переменных модели
  + Добавление новых элементов(в конец списка)
  + Поиск элемента в списке по имени переменной
  + Поиск последнего элемента
  + Очистка списка
### 5. ```model.h|.c```
- Реализует обработку `.json` файлов для преобразования его в модель в памяти
- Имеет функции работы с состояниями и переходами
- Использует стороннюю библиотеку cJSON

# Сборка и запуск
## сборка программы
Собирает проект, исполняемый файл `main` кладется в папку `./build/`
## запуск программы
`./build/main`

в качестве параметра запуска так же передать путь до `.json` файл с описанием модели

Пример: `./build/main ./model.json`
## Пересборка
В случае изменения кода, просто выполнить сборку `make all`

В случае изменения параметров в `config.conf`, выполнить make clean, затем собирать
