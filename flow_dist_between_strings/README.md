Расстояние между двумя строками равной длины — это количество индексов, в которых символы этих строк не равны.
Есть 2 битовые строки p и s (|p| <=|s|), но неготорые биты заменены на знаки вопроса. Определим расстояние между строками как суммарное расстояние от p до всех подстрок s, имеющих длину |p|. 
Надо заполнить знаки вопроса так, чтобы определенное выше расстояние между строками было наименьшим.

#### Формат ввода
Две строки - s и p.

#### Формат вывода
В первой строке выводит минимальное расстояние от p до s после восстановления всех знаков вопроса.
Во второй и третьей строках соответственно выводит s и p, в которых символы ‘?’ заменены на ‘0’ и ‘1’.
