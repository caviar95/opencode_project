#pragma once
#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

#include "money.h"

namespace ddd {

// 值对象：订单号。
class OrderId {
public:
  explicit OrderId(std::string v) : value_(std::move(v)) {}
  const std::string& value() const { return value_; }
  bool operator==(const OrderId& o) const { return value_ == o.value_; }
  bool operator<(const OrderId& o) const { return value_ < o.value_; }

private:
  std::string value_;
};

// 值对象：客户标识。
class CustomerId {
public:
  explicit CustomerId(std::string v) : value_(std::move(v)) {}
  const std::string& value() const { return value_; }
  bool operator==(const CustomerId& o) const { return value_ == o.value_; }

private:
  std::string value_;
};

// 值对象：收货地址，不可变。
class Address {
public:
  Address(std::string city, std::string street, std::string detail)
      : city_(std::move(city)), street_(std::move(street)), detail_(std::move(detail)) {}
  const std::string& city() const { return city_; }
  const std::string& street() const { return street_; }
  const std::string& detail() const { return detail_; }

private:
  std::string city_, street_, detail_;
};

}  // namespace ddd