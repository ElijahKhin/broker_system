#include <iostream>
#include <optional>

int main() {
  std::optional<std::string> opt;
  opt = std::nullopt;
  opt = "Hi";
  if (opt) {
    std::cout << "Opt: " << *opt << '\n';
  } else {
    std::cout << "No opt" << '\n';
  }
  return 0;
}
