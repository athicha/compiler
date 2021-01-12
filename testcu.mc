function main() {
  var i, sum, sub;
  i = 1;
  sum = 0;
  sub = 0;
  printf("  i   sum\n");
  while (i < 10) {
    i = add(i, 1);
    sum = add(sum, i);
    sub = minus(sum, 1);
    printf("  %ld   %ld %ld\n", i, sum, sub);
  }
  return 0;
}

function add(a1, a2) {
  var a;
  a = a1 + a2;
  return a;
}

function minus(a1, a2) {
  return a1 -a2;
}
