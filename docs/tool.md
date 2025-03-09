# How to use the tools

[English](tool.md) | [日本語](tool-ja.md)

- [Input](#input)
- [Output](#output)
- [Usage](#usage)
- [Example](#example)

## Input

Prepare data files in either of the following two formats.

- One File (x and y are described side by side)
- Two Files (x and y are described in different files)

### Example of the One File format

```txt
0,1.864202886
0.25,1.603404302
0.75,2.188388163
1,2.264341162
(omitted)
9.25,4.170648488
9.5,5.664498381
9.75,6.677234607
10,7.885698628
```

Note: You can use either a comma or a space as a delimiter.

### Example of the Two Files format

`Data x`

``` txt
0
0.25
0.5
1
(omitted)
8.75
9.0
9.5
10
```

`Data y`

``` txt
1.62
1.81
2.12
2.32
(omitted)
4.54
5.78
6.52
7.91
```

## Output

- `default.csv`  
A CSV file of the input (**before** fitting) data
- `fitting.csv`  
A CSV file of the output (**after** fitting) data
- `result.png`  
The result graph image

## Usage

The format of comannd is following:

```bash
fitting <input file> <options>
```

`<input file>` is the path to the input file (or files).

The example of the One File format:

``` bash
fitting data.csv
```

In the case of the Two Files format, specify in the order `x y`.  
The example of the Two Files format:

``` bash
fitting x.csv y.csv
```

Options specify the fitting model:

| Option | Fitting Model |
|-|-|
|`option1`|For normal data (default, so you can omit)|
|`option2`|For two-phase problem data|

## Example

`sample.csv`

```txt
0,1.864202886
0.25,1.603404302
0.75,2.188388163
1,2.264341162
(omitted)
9.25,4.170648488
9.5,5.664498381
9.75,6.677234607
10,7.885698628
```

`default.csv`

```txt
0.000000 1.864203
0.250000 1.603404
0.750000 2.188388
1.000000 2.264341
(omitted)
9.250000 4.170648
9.500000 5.664498
9.750000 6.677235
10.000000 7.885699
```

`fitting.csv`

```txt
0.000000 1.760525
0.083333 1.755539
0.166667 1.757635
0.250000 1.778883
(omitted)
9.750000 6.855400
9.833333 7.194727
9.916667 7.454149
10.000000 7.600462
```

`result.png（in case of option1)`

![alt text](result_option1.png)

`result.png (in case of option2)`

![alt text](result_option2.png)
