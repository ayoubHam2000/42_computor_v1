#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>

struct eq_term
{
  unsigned int   degree;
  float coefficent;
};

struct equation_t
{
  std::vector<eq_term>  eq;
  unsigned int          degree;
};

enum PHASE
{
  COEFFICENT,
  DEGREE,
  OTHER
};


std::string format_equation(const std::string &equation)
{
  std::string eq = equation;

  auto iter = std::remove_if(eq.begin(), eq.end(), [](char c) -> bool {
    return c == ' ';
  });
  eq.erase(iter, eq.end());

  auto res = std::find(eq.begin(), eq.end(), '=');
  if (res != eq.end())
  {
    for (auto iter = res; iter != eq.end(); iter++)
    {
      if (*iter == '-')
        *iter = '+';
      else if (*iter == '+')
        *iter = '-';
    }
    if (*(res + 1) == '+' || *(res + 1) == '-')
      eq.erase(res);
    else
      *res = '-';
  }
  else
  {
    std::cerr << "Bad equation." << std::endl;
  }
  return (eq);
}

void  construct_eqaution(equation_t  &equation_info, const std::string &eq)
{
  int                   i;
  unsigned int          max_degree;
  std::stringstream     ss(eq);
  std::vector<eq_term>  &equation = equation_info.eq;
  eq_term               term;
  char                  c;

  i = 2;
  while (i >= 0)
  {
    term.degree = i;
    term.coefficent = 0;
    equation.push_back(term);
    i--;
  }

  i = COEFFICENT;
  max_degree = 0;
  while (ss)
  {
    if (i == COEFFICENT)
    {
      ss >> term.coefficent;
      i = OTHER;
    }
    else if (i == DEGREE)
    {
      ss >> term.degree;

      if (term.degree > max_degree)
        max_degree = term.degree;

      auto iter = std::find_if(equation.begin(), equation.end(), [&term](const eq_term &item) -> bool {
        return item.degree == term.degree;
      });
      if (iter != equation.end())
        iter->coefficent += term.coefficent;
      else
        equation.push_back(term);
      i = COEFFICENT;
    }
    else
    {
      ss >> c;
      if (c == '^')
        i = DEGREE;
    }
  }

  std::sort(equation.begin(), equation.end(), [](const eq_term &t1, const eq_term &t2) -> bool {
    return t1.degree > t2.degree;
  });

  equation_info.degree = max_degree;
}

void  print_reduce_form(const std::vector<eq_term> &equation)
{
  std::cout << "Reduced form: ";
  int flag = 0;
  for (size_t k = 0; k < equation.size(); k++)
  {
    eq_term item = equation[k];
    if (item.coefficent != 0)
    {
      if (item.coefficent > 0)
        std::cout << "+ ";
      else
        std::cout << "- ";
      std::cout << abs(item.coefficent) << " * X^" << item.degree << " ";
      flag = 1;
    }
  }
  if (flag == 0)
    std::cout << "0 ";
  std::cout << "= 0" <<  std::endl;
}

void  solve(const eq_term &t1, const eq_term &t2, const eq_term &t3)
{
  if (t1.coefficent == 0.0 && t2.coefficent == 0.0 && t3.coefficent == 0.0)
  {
    std::cout << "This equation has endless solution in R" << std::endl;
  }
  else if (t1.coefficent == 0.0 && t2.coefficent == 0.0)
  {
    std::cout << "This equation has no solution in R" << std::endl;
  }
  else if (t1.coefficent == 0.0)
  {
    float x = (-t3.coefficent / t2.coefficent);
    std::cout << "The solution in R is: " << std::endl;
    std::cout << x << std::endl;
  }
  else
  {
    float delta = t2.coefficent * t2.coefficent - 4 * t1.coefficent * t3.coefficent;
    if (delta < 0)
    {
      std::cout << "Can't solve this equation in R: discriminant < 0" << std::endl;
    }
    else if (delta > 0)
    {
      float sq_delta = sqrt(delta);
      float x1 = (-t2.coefficent + sq_delta) / (2 * t1.coefficent);
      float x2 = (-t2.coefficent - sq_delta) / (2 * t1.coefficent);
      std::cout << "Discriminant is strictly positive, the two solutions in R are:" << std::endl;
      std::cout << x1 << std::endl;
      std::cout << x2 << std::endl;
    }
    else
    {
      float x = (-t2.coefficent) / (2 * t1.coefficent);
      std::cout << "Discriminant is equal 0, the solution in R is:" << std::endl;
      std::cout << x << std::endl;
    }
  }
}

void  computor(const equation_t  &equation_info)
{
  print_reduce_form(equation_info.eq);
  std::cout << "Polynomial degree: " << equation_info.degree << std::endl;
  if (equation_info.degree <= 2)
    solve(equation_info.eq[0], equation_info.eq[1], equation_info.eq[2]);
  else
    std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl;
}

int main(int ac, char **av)
{
  equation_t  equation_info;
  std::string equation_input;

  if (ac == 2)
  {
    equation_input = std::string(av[1]);
    equation_input = format_equation(equation_input);
    construct_eqaution(equation_info, equation_input);
    computor(equation_info);
  } else {
    std::cout << "computor <equation>" << std::endl;
  }
  return (0);
}
