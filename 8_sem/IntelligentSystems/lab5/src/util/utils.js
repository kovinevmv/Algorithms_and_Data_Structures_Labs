function mod(n, m) {
  return ((n % m) + m) % m;
}

function limit(n, l) {
  if ((n <= l) || (n >= 360 - l)) {
    return n
  }
  if (n <= 180) {
    return l
  }
  return 360 - l
}

function align(n) {
  if (n > 180) {
    n -= 360
  }
  return n
}

module.exports = { mod, limit, align }
