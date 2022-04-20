#include <iostream>

bool isValidSequence(const std::string &rna) {
  for (auto &x : rna) {
    if (x != 'A' && x != 'G' && x != 'U' && x != 'C')
      return false;
  }
  return true;
}

int **createOpt(int n) {

  int **opt;
  opt = new int *[n - 5];
  for (int i = 0; i < n - 5; i++) {
    opt[i] = new int[n - 5];
    for (int j = 0; j < n - 5; j++)
      opt[i][j] = 0;
  }
  return opt;
}

bool **createVisited(int n) {
  bool **visited;
  visited = new bool *[n - 5];
  for (int i = 0; i < n - 5; i++) {
    visited[i] = new bool[n - 5];
    for (int j = 0; j < n - 5; j++)
      visited[i][j] = false;
  }
  return visited;
}

std::string **createPairHolder(int n) {

  std::string **base_pairs;
  base_pairs = new std::string *[n - 5];
  for (int i = 0; i < n - 5; i++) {
    base_pairs[i] = new std::string[n - 5];
    for (int j = 0; j < n - 5; j++)
      base_pairs[i][j] = "-";
  }
  return base_pairs;
}

bool isBasePair(char x, char y) {
  if ((x == 'A' && y == 'U') || (x == 'U' && y == 'A') ||
      (x == 'C' && y == 'G') || (x == 'G' && y == 'C'))
    return true;
  else
    return false;
}

int calculateOpt(int **opt, int i, int j, int n, const std::string &rna,
                 std::string **base_pairs, bool **visited) {

  if (i < 0 || (j - 4) < 0|| j - i <= 4)
    return 0;

  if (visited[(n - 6) - i][(j - 5)] && (n - 6) - i >= 0 && (j - 5) >= 0)
    return opt[(n - 6) - i][(j - 5)];

  visited[(n - 6) - i][(j - 5)] = true;

  int x;
  std::string x_str{""};
  if (j - 1 - i <= 4)
    x = 0;
  else {
    x = opt[(n - 6) - i][(j - 5) - 1];
    x_str = base_pairs[(n - 6) - i][(j - 5) - 1];
  }

  int y = 0;
  std::string y_str{""};
  for (int t = i; t < j - 4; t++) {
    if (isBasePair(rna[t], rna[j]) && j - t > 4) {
      int rec = 0, i_to_t;
      // if (t - 1 >= 0)
        rec = 1 + calculateOpt(opt, i, t - 1, n, rna, base_pairs, visited) +
              calculateOpt(opt, t + 1, j - 1, n, rna, base_pairs, visited);
      if (rec > y) {
        y = rec;

        std::string s1{""};
        std::string s2{""};
        if ((t - 1) - 5 - 1 >= 0)
          s1 = base_pairs[(n - 6) - i][(t - 1) - 5 - 1];
        if ((n - 6) - (t + 1) >= 0)
          s2 = base_pairs[(n - 6) - (t + 1)][(j - 5) - 1];

        y_str =
            "(" + std::to_string(t) + ", " + std::to_string(j) + ")" + s1 + s2;
      }
    }
  }

  if (x > y) {
    base_pairs[(n - 6) - i][j - 5].append(x_str);
  } else {
    base_pairs[(n - 6) - i][j - 5].append(y_str);
  }

  return std::max(x, y);
}

int main() {
  std::cout << "Enter RNA sequence:" << std::endl;
  std::string rna;
  std::cin >> rna;

  if (!isValidSequence(rna)) {
    std::cerr << "<Invalid RNA sequence>" << std::endl;
    return 0;
  }

  int n = rna.length();

  if (n < 5) {
    std::cout << "<Too small to form a secondary structure>";
    return 0;
  }

  if (n == 5) {
    if (isBasePair(rna[0], rna[n - 1])) {
      std::cout << "1 secondary structure";
      return 0;
    } else {
      std::cout << "No secondary structure";
      return 0;
    }
  }

  int **opt = createOpt(n);
  bool **visited = createVisited(n);
  std::string **base_pairs = createPairHolder(n);

  int j;
  for (int k = 5; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      j = i + k;
      opt[(n - 6) - i][j - 5] =
          calculateOpt(opt, i, j, n, rna, base_pairs, visited);
    }
  }

  int max_matches_i = 0, max_matches_j = 0;
  for (int i = 0; i < n - 5; i++) {
    for (int j = 0; j < n - 5; j++)
      if (opt[i][j] > opt[max_matches_i][max_matches_j]) {
        max_matches_i = i;
        max_matches_j = j;
      }
  }

  // std::cout << (n-6)-max_matches_i << "---------" << max_matches_j+5 << std::endl;
  std::cout << "Maximum number of pairings: "
            << opt[max_matches_i][max_matches_j] << std::endl;
  std::cout << "The base pairs: " << base_pairs[max_matches_j][max_matches_j]
            << std::endl;


  // // Print opt
  // for (int i = 0; i < n - 5; i++) {
  //   for (int j = 0; j < n - 5; j++)
  //     std::cout << base_pairs[i][j] << " ";
  //   std::cout << std::endl;
  // }

  return 0;
}