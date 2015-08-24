# Проект - трекинг объектов с помощью цветовой гистограммы.

## Общая структура проекта

Данный репозиторий содержит:

  - `include` -- директория с заголовочным файлом, содержащим интерфейс трекера.
  - `samples` -- директория с исходным кодом приложения, которое запускает
    трекинг на видео. Имя алгоритма и путь к видео передаются параметрами
    командной строки. Кроме того, может быть указан файл с описанием
    ground-truth траектории, в этом случае приложение также выдаст метрики
    precision и recall для запускаемого трекера.
  - `dataset` -- директория с примерами видео. Для каждого видео имеется
    одноименный файл с расширением `.txt`, в котором указана ground-truth
    траектория.
  - `.gitignore` -- список файлов, находящихся в директории проекта,
     но игнорируемые git'ом.
  - `.travis.yml` -- конфигурационный файл для системы автоматического
     тестирования Travis-CI.
  - `CMakeLists.txt` -- общий файл для сборки проекта с помощью CMake.
  - `README.md` -- настоящий файл.


<!-- LINKS -->

[practice1]: https://github.com/Itseez-NNSU-SummerSchool2015/practice1-devtools
[git-intro]: https://github.com/Itseez-NNSU-SummerSchool2015/practice1-devtools#Общие-инструкции-по-работе-с-git
[cmake-msvs]: https://github.com/Itseez-NNSU-SummerSchool2015/practice1-devtools#Сборка-проекта-с-помощью-cmake-и-microsoft-visual-studio
[tasks]: https://github.com/Itseez-NNSU-SummerSchool2015/practice4-tracking#Задачи
[lucas-canade-tutorial]: http://opencv-python-tutroals.readthedocs.org/en/latest/py_tutorials/py_video/py_lucas_kanade/py_lucas_kanade.html