#code snippets

functions

```
thing: (a: num) => void = {
}
thing: (a: num) => void {
}
thing: (a: num): void {
}
thing(a: num): void {
}
thing(a: num) => void {
}
thing: (num) => void = (a) {
}
```

- all type signatures will will begin after a `:`
- some languages show the return with another specifer like `() => void`
  - `=>` or `->` or `:` are omitted in cons

types

```
type car {
  make: string,
  model: string
}

```
- there is no signature indicator `:` after car, because it is the signature

```
fn drive: (a: car) void {

}
```
