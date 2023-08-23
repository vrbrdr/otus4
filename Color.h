#pragma once

#include <iostream>

class Color {
  public:
    Color() = default;
    Color(double red, double green, double blue) : r{red}, g{green}, b{blue} {};

    double red() const;
    double green() const;
    double blue() const;

  private:
    double r{};
    double g{};
    double b{};

    friend std::istream& operator>>(std::istream& is, Color& p);
};

std::istream& operator>>(std::istream& is, Color& p);