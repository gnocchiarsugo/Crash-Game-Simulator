  auto sum = [a, b]() {
        return a + b;
    };


std::function<const unsigned &()> func([i]() -> const unsigned &
                                               { return i; });

| Syntax     | Meaning                                           |
| ---------- | ------------------------------------------------- |
| `[a, b]`   | Capture `a` and `b` by value                      |
| `[&a, &b]` | Capture `a` and `b` by reference                  |
| `[=]`      | Capture all used local variables by value         |
| `[&]`      | Capture all used local variables by reference     |
| `[=, &b]`  | Capture everything by value except `b` by ref     |
| `[&, b]`   | Capture everything by reference except `b` by val |
