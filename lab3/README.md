##  LAB_3

### Задание:
Задание состоит из последовательности связанных между собой пунктов.
Главная задача: для каждого пункта решить сформулированную в нём задачу наиболее эффективно.  Для подавляющего, кроме, возможно, первого пункта применение циклов не будет считаться корректным решением.

Задание состоит из последовательности связанных междцу собой пунктов. Для каждого из них необходимо написать код, решающий поставленную задачу наиболее эффективным способом.

### Запуск:

``` bash
# build project
make clean && make

# run program
./main
```

### Класс Box:
Для выполнения этого задания, необходимо было немного доработать код `Box` из прошлого задания, а именно - добавить операторы сравнения для корректной работы алгоритмов сортировки.
- PUBLIC (new):
    - **bool operator<(const Box &other) const;** - оператор сравнения (меньше);
    - **bool operator>(const Box &other) const;** - оператор сравнения (больше);
    - **bool operator==(const Box &other) const;** - операто сравнения (равенство);

### main.cpp:
1) Создать вектор v1 размером от 500 до 1000 элементов, число элементов не известно до запуска программы. Элементами вектора являются экземпляры класса из Л.Р. №2 имеющие произвольные(случайные) значения.

```C++
random_device rd;
mt19937 gen(rd());

// размер будет случайным числом в диапозоне от 500 до 1000
uniform_int_distribution<> size_dist(500, 1000);
int size = size_dist(gen);

vector<Box> v1;
v1.reserve(size);

// Значения будут случайными числами в диапозоне от 1 до 1000
uniform_int_distribution<> interval(1, 1000);

for (int i = 0; i < size; ++i) {
    v1.emplace_back(interval(gen));
}
```
2) Создать вектор v2, поместив в него последние 200 элементов вектора v1. Рассмотрите решение для произвольных b и e, которые задают позиции первого и последнего копируемых элементов.

```C++
int b = v1.size() - 200;
int e = v1.size();

if (b < 0)
    b = 0;
if (e > static_cast<int>(v1.size()))
    e = v1.size();

vector<Box> v2(v1.begin() + b, v1.begin() + e);

// при произвольных b и e особых изменений не произойдет, только необходимо будет добавить проверку на то, какое число больше (b ? e)
```

3) Сформировать список list1, поместив в него первые n (от 20 до 50) наибольших элементов вектора v1 (указанные элементы должны быть отсортированы до помещения их в список).

```C++
// n - случайное число в диапозоне от 20 до 50
uniform_int_distribution<> intervalForN(20, 50);
int n = intervalForN(gen);

vector<Box> largest;

largest.resize(n);
// получаем вектор из n наибольших значений v1
partial_sort_copy(v1.begin(), v1.end(), largest.begin(), largest.end(), greater<Box>());

// создаем list1 и заполняем значениями из largest
list<Box> list1(largest.begin(), largest.end());
```

4) Сформировать список list2, поместив в него последние n (от 20 до 50) наименьших элементов вектора v2, порядок элементов не важен.

аналогично пункту 3

5) Удалить из векторов v1 и v2 перемещенные элементы. Скорректируйте размеры векторов после удаления из них элементов.

```C++
// создаем мапы, где будут храниться значения, которые необходимо удалить из v1 & v2
map<int, Box> forRemoveV1;
map<int, Box> forRemoveV2;

// заполняем их
for (const auto &box : list1) {
    forRemoveV1[box.getNumber()] = box;
}

for (const auto &box : list2) {
    forRemoveV2[box.getNumber()] = box; 
}

// удаляем из v1 & v2 объекты классов, которые есть в мапах на удаление.
v1.erase(remove_if(v1.begin(), v1.end(),
                    [&forRemoveV1](const Box &box) {
                    return forRemoveV1.find(box.getNumber()) !=
                            forRemoveV1.end();
                    }),
        v1.end());

v2.erase(remove_if(v2.begin(), v2.end(),
                    [&forRemoveV2](const Box &box) {
                    return forRemoveV2.find(box.getNumber()) !=
                            forRemoveV2.end();
                    }),
        v2.end());
```

6) Для списка list1 найти элемент со средним значением. Перегруппировать элементы списка так, чтобы в начале оказались все элементы, большие среднего значения.

```C++
// находим среднее значение
double sum = accumulate(
    list1.begin(), list1.end(), 0.0,
    [](double acc, const Box &box) { return acc + box.getNumber(); });
double mean = sum / static_cast<double>(list1.size());

// перегрупировываем список в соответствии с заданием
partition(list1.begin(), list1.end(),
        [mean](const Box &box) { return box.getNumber() > mean; });

```

7) Удалите из списка list2 все нечётные элементы (или используйте другой критерий, который однозначно делит экземпляры вашего класса на два непересекающихся множества).

```C++
// удаляем все нечетные элементы из list2 (elem % 2 != 0)
list2.remove_if([](const Box &box) { return box.getNumber() % 2 != 0; });

```

8) Создайте вектор v3 из элементов, которые присутствуют и в векторе v1 и в векторе v2.

```C++
// инициализируем v3 нужного размера
vector<Box> v3;
v3.reserve(v1.size() + v2.size());

// заполняем v3 нужными элементами (элементами, которые есть и в v1 и в v2)
copy_if(v1.begin(), v1.end(), back_inserter(v3), [&](const Box &box) {
return find(v2.begin(), v2.end(), box) != v2.end();
});
```

9) Для списков list1 и list2 из списка с большим числом элементов удалите первые n так, чтобы оба списка имели бы одинаковый размер. Сформируйте из них список list3, который будет хранить пары <первый элемент списка list1, первый элемент списка list2>, <второй элемент списка list1, второй элемент списка list2>  и т. д.

```C++
// размеры списков
size_t size1 = list1.size();
size_t size2 = list2.size();

// удаляем из большого списка первые n элементов, чтобы размеры list1 & list2 совпали
if (size1 > size2) {
    size_t diff = size1 - size2;

    auto it = list1.begin();
    advance(it, diff);
    list1.erase(list1.begin(), it);
} else {
    size_t diff = size2 - size1;

    auto it = list2.begin();
    advance(it, diff);
    list2.erase(list2.begin(), it);
}

// создаем список пар (list3) элементов list1 & list2
typedef pair<Box, Box> BoxPair;
list<BoxPair> list3;

transform(
    list1.begin(), list1.end(), list2.begin(), back_inserter(list3),
    [](const Box &a, const Box &b) -> BoxPair { return make_pair(a, b); });

```

10) Решите предыдущую задачу для векторов v1 и v2 без предварительного приведения векторов к одному размеру. Пар с пустыми значениями быть не должно.

```C++
typedef pair<Box, Box> BoxPair;
vector<BoxPair> result;

size_t min_size = min(v1.size(), v2.size());

// результирующий вектор должен обладать размером, равным наименьшему из значений длин векторов v1 & v2
result.reserve(min_size);

// помещаем в result пары элементов v1 & v2
transform(
    v1.begin(), v1.begin() + min_size, v2.begin(), back_inserter(result),
    [](const Box &a, const Box &b) -> BoxPair { return make_pair(a, b); });

```


