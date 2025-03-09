# How to use the libraries

[English](library.md) | [日本語](library-ja.md)

- [Input](#input)
- [Output](#output)
- [Usage](#usage)
- [Example](#example)

## Input

### Callable Function

The function call in this library is following:

```c
ddspline(x,y,n,flag)
```

### Arguments

- 1st argument `x`  
The array of x of the experimental data
- 2nd argument `y`  
The array of y of the experimental data
- 3rd argument `n`  
The number of elements in the input array (`x` and `y`)
- 4th argument `flag`  
The type of fitting model. The options are following:

| Option | Fitting Model |  
|:-:|-|  
|`0`|For normal data|  
|`1`|For two-phase problem data|

## Output

- `default.csv`  
A CSV file of the input (**before** fitting) data
- `fitting.csv`  
A CSV file of the output (**after** fitting) data
- `result.png`  
The result graph image

## Usage

1. Include the header `dspline_fitting.h` to your code

    `main.c`

    ```c
    #include <stdio.h>
    #include <stdlib.h>
    ///////

    #include "dspline_fitting.h"    // Add

    ///////
    ```

2. Call function `ddspline`

    `main.c`

    ```c

    /////

    ddspline(x,y,n,flag);   // add

    /////
    ```

    Refer to the [Input](#input) for usage of `ddspline`

3. Compile your code with the option `-ldspline`

    ``` bash
    gcc main.c -ldspline
    ```

## Example

`user.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "dspline_fitting.h"

int main() {
    double x[20] = {0.00, 0.55, 1.07, 1.65, 2.17, 2.66, 3.17, 3.73,4.27, 4.79, 5.33, 5.85, 6.41, 6.90, 7.44, 7.98, 8.52, 9.00, 9.48, 10.00};
    double y[20] = {2.41, 1.70, 1.97, 1.68, 2.76, 1.93, 3.73, 2.73,2.90, 4.52, 3.66, 1.47, 0.84, -0.94, 0.52, 0.13, 1.76, 4.02, 5.24, 9.45};

    int n = 20;

    dspline *result = ddspline(x, y, n, 0);

    printf("Data after fitting:\n");
    for (int i = 0; i < result->nn; i++) {
        printf("fx[%d] = %lf, fy[%d] = %lf\n", i, result->fx[i], i, result->fy[i]);
    }

    free(result->fx);
    free(result->fy);
    free(result);

    return 0;
}
```

`result.png（option1)`

![alt text](result_option1.png)

`result.png (option2)`

![alt text](result_option2.png)
